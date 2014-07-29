
#include "stdafx.h"
#include "COceanSim.h"

#define		LENGTH_OCEAN	1

////////////////////////////////////////////////////////////////////////////////
// kernels
//#include <oceanFFT_kernel.cu>

extern "C"
	void cudaGenerateSpectrumKernel(float2 *d_h0,
	float2 *d_ht,
	unsigned int in_width,
	unsigned int out_width,
	unsigned int out_height,
	float animTime,
	float patchSize);

extern "C"
	void cudaUpdateHeightmapKernel(float  *d_heightMap,
	float2 *d_ht,
	unsigned int width,
	unsigned int height);

extern "C"
	void cudaCalculateSlopeKernel(float *h, float2 *slopeOut,
	unsigned int width, unsigned int height);

COceanSim::COceanSim()
	:windowW(512)
	,windowH(512)
	,meshSize(256)
	,spectrumW(meshSize + 4)
	,spectrumH(meshSize + 1)
	,frameCompare(4)
	,g(9.81f)
	,A(1e-7f)
	,patchSize(100)
	,frameCheckNumber(4)
{
	setDefault();
}

COceanSim::~COceanSim()
{

}

void COceanSim::initializeCUDA()
{
	findCudaGLDevice(0, NULL);

	// create FFT plan
	cufftPlan2d(&fftPlan, meshSize, meshSize, CUFFT_C2C);

	

}

void COceanSim::initializeVBO()
{
	// allocate memory
	int spectrumSize = spectrumW*spectrumH*sizeof(float2);
	checkCudaErrors(cudaMalloc((void **)&d_h0, spectrumSize));
	h_h0 = (float2 *) malloc(spectrumSize);
	generate_h0(h_h0);
	checkCudaErrors(cudaMemcpy(d_h0, h_h0, spectrumSize, cudaMemcpyHostToDevice));

	int outputSize =  meshSize*meshSize*sizeof(float2);
	checkCudaErrors(cudaMalloc((void **)&d_ht, outputSize));
	checkCudaErrors(cudaMalloc((void **)&d_slope, outputSize));

	// create vertex buffers and register with CUDA
	createVBO(&heightVertexBuffer, meshSize*meshSize*sizeof(float));
	checkCudaErrors(cudaGraphicsGLRegisterBuffer(&cuda_heightVB_resource, heightVertexBuffer, cudaGraphicsMapFlagsWriteDiscard));

	createVBO(&slopeVertexBuffer, outputSize);
	checkCudaErrors(cudaGraphicsGLRegisterBuffer(&cuda_slopeVB_resource, slopeVertexBuffer, cudaGraphicsMapFlagsWriteDiscard));

	// create vertex and index buffer for mesh
	createMeshPositionVBO(&posVertexBuffer, meshSize, meshSize);
	createMeshIndexBuffer(&indexBuffer, meshSize, meshSize);
}

void COceanSim::renderVBO()
{

}

void COceanSim::modifyVBO()
{

}

bool COceanSim::runCUDAHost()
{
	size_t num_bytes;

	// generate wave spectrum in frequency domain
	cudaGenerateSpectrumKernel(d_h0, d_ht, spectrumW, meshSize, meshSize, animTime, patchSize);

	// execute inverse FFT to convert to spatial domain
	cufftExecC2C(fftPlan, d_ht, d_ht, CUFFT_INVERSE);

	// update heightmap values in vertex buffer
	checkCudaErrors(cudaGraphicsMapResources(1, &cuda_heightVB_resource, 0));
	checkCudaErrors(cudaGraphicsResourceGetMappedPointer((void **)&g_hptr, &num_bytes, cuda_heightVB_resource));

	cudaUpdateHeightmapKernel(g_hptr, d_ht, meshSize, meshSize);

	checkCudaErrors(cudaGraphicsUnmapResources(1, &cuda_heightVB_resource, 0));

	// calculate slope for shading
	checkCudaErrors(cudaGraphicsMapResources(1, &cuda_slopeVB_resource, 0));
	checkCudaErrors(cudaGraphicsResourceGetMappedPointer((void **)&g_sptr, &num_bytes, cuda_slopeVB_resource));

	cudaCalculateSlopeKernel(g_hptr, g_sptr, meshSize, meshSize);

	checkCudaErrors(cudaGraphicsUnmapResources(1, &cuda_slopeVB_resource, 0));

	return true;
}

void COceanSim::render()
{
	if (!bRenderOcean)
	{
		return;
	}

	// update timer
	float time = sdkGetTimerValue(&timer);

	if (animate)
	{
		animTime += (time - prevTime) * animationRate;
	}
	prevTime = time;

	// run CUDA kernel to generate vertex positions
	runCUDAHost();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set view matrix
/*	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(translateX, translateY, translateZ);
	glRotatef(rotateX, 1.0, 0.0, 0.0);
	glRotatef(rotateY, 0.0, 1.0, 0.0);
*/
	glPushAttrib( GL_ALL_ATTRIB_BITS );

	// render from the vbo
	glBindBuffer(GL_ARRAY_BUFFER, posVertexBuffer);
	glVertexPointer(4, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, heightVertexBuffer);
	glClientActiveTexture(GL_TEXTURE0);
	glTexCoordPointer(1, GL_FLOAT, 0, 0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, slopeVertexBuffer);
	glClientActiveTexture(GL_TEXTURE1);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glUseProgram(shaderProg);

	// Set default uniform variables parameters for the vertex shader
	GLuint uniHeightScale, uniChopiness, uniSize;

	uniHeightScale = glGetUniformLocation(shaderProg, "heightScale");
	glUniform1f(uniHeightScale, 0.5f);

	uniChopiness   = glGetUniformLocation(shaderProg, "chopiness");
	glUniform1f(uniChopiness, 1.0f);

	uniSize        = glGetUniformLocation(shaderProg, "size");
	glUniform2f(uniSize, (float) meshSize, (float) meshSize);

	// Set default uniform variables parameters for the pixel shader
	GLuint uniDeepColor, uniShallowColor, uniSkyColor, uniLightDir;

	uniDeepColor = glGetUniformLocation(shaderProg, "deepColor");
	glUniform4f(uniDeepColor, 0.0f, 0.1f, 0.4f, 1.0f);

	uniShallowColor = glGetUniformLocation(shaderProg, "shallowColor");
	glUniform4f(uniShallowColor, 0.1f, 0.3f, 0.3f, 1.0f);

	uniSkyColor = glGetUniformLocation(shaderProg, "skyColor");
	glUniform4f(uniSkyColor, 1.0f, 1.0f, 1.0f, 1.0f);

	uniLightDir = glGetUniformLocation(shaderProg, "lightDir");
	glUniform3f(uniLightDir, 0.0f, 1.0f, 0.0f);
	// end of uniform settings

	glColor3f(1.0, 1.0, 1.0);

	if (drawPoints)
	{
		glDrawArrays(GL_POINTS, 0, meshSize * meshSize);
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		glPolygonMode(GL_FRONT_AND_BACK, wireFrame ? GL_LINE : GL_FILL);
		glDrawElements(GL_TRIANGLE_STRIP, ((meshSize*2)+2)*(meshSize-1), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glClientActiveTexture(GL_TEXTURE0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glClientActiveTexture(GL_TEXTURE1);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glUseProgram(0);
	glPopAttrib();
}

void COceanSim::setDefault()
{
	d_h0 = 0;   // heightfield at time 0
	h_h0 = 0;
	d_ht = 0;   // heightfield at time t
	d_slope = 0;

	// pointers to device object
	g_hptr = NULL;
	g_sptr = NULL;

	// simulation parameters
	windSpeed = 100.0f;
	windDir = CUDART_PI_F/3.0f;
	dirDepend = 0.07f;

	timer = NULL;
	animTime = 0.0f;
	prevTime = 0.0f;
	animationRate = -0.001f;

	// Auto-Verification Code
	fpsCount = 0;        // FPS count for averaging
	fpsLimit = 1;        // FPS limit for sampling
	frameCount = 0;
	g_TotalErrors = 0;

	animate = true;
	drawPoints = false;
	wireFrame = false;
	g_hasDouble = false;

	bRenderOcean = false;

}

void COceanSim::cleanUp()
{
	sdkDeleteTimer(&timer);
	checkCudaErrors(cudaGraphicsUnregisterResource(cuda_heightVB_resource));
	checkCudaErrors(cudaGraphicsUnregisterResource(cuda_slopeVB_resource));

	deleteVBO(&posVertexBuffer);
	deleteVBO(&heightVertexBuffer);
	deleteVBO(&slopeVertexBuffer);

	checkCudaErrors(cudaFree(d_h0));
	checkCudaErrors(cudaFree(d_slope));
	free(h_h0);
	cufftDestroy(fftPlan);
}

void COceanSim::createVBO( GLuint *vbo, int size )
{
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool COceanSim::initGL( )
{
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);
	prevTime = sdkGetTimerValue(&timer);

	shaderProg = loadGLSLProgram("oceanFFT\\ocean.vert", "oceanFFT\\ocean.frag");

	return true;
}

void COceanSim::deleteVBO( GLuint *vbo )
{
	glDeleteBuffers(1, vbo);
	*vbo = 0;
}

void COceanSim::createMeshIndexBuffer( GLuint *id, int w, int h )
{
	int size = ((w*2)+2)*(h-1)*sizeof(GLuint);

	// create index buffer
	glGenBuffersARB(1, id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *id);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

	// fill with indices for rendering mesh as triangle strips
	GLuint *indices = (GLuint *) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

	if (!indices)
	{
		return;
	}

	for (int y=0; y<h-1; y++)
	{
		for (int x=0; x<w; x++)
		{
			*indices++ = y*w+x;
			*indices++ = (y+1)*w+x;
		}

		// start new strip with degenerate triangle
		*indices++ = (y+1)*w+(w-1);
		*indices++ = (y+1)*w;
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void COceanSim::createMeshPositionVBO( GLuint *id, int w, int h )
{
	createVBO(id, w*h*4*sizeof(float));

	glBindBuffer(GL_ARRAY_BUFFER, *id);
	float *pos = (float *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	if (!pos)
	{
		return;
	}

	for (int y=0; y<h; y++)
	{
		for (int x=0; x<w; x++)
		{
			float u = x / (float)(w-1);
			float v = y / (float)(h-1);
			*pos++ = (u*2.0f-1.0f)*LENGTH_OCEAN;
			*pos++ = 0.0f;
			*pos++ = (v*2.0f-1.0f)*LENGTH_OCEAN;
			*pos++ = 1.0f;
		}
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
// Attach shader to a program
int COceanSim::attachShader(GLuint prg, GLenum type, const char *name)
{
	GLuint shader;
	FILE *fp;
	int size, compiled;
	char *src;

	fp = fopen(name, "rb");

	if (!fp)
	{
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	src = (char *)malloc(size);

	fseek(fp, 0, SEEK_SET);
	fread(src, sizeof(char), size, fp);
	fclose(fp);

	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char **)&src, (const GLint *)&size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, (GLint *)&compiled);

	if (!compiled)
	{
		char log[2048];
		int len;

		glGetShaderInfoLog(shader, 2048, (GLsizei *)&len, log);
		printf("Info log: %s\n", log);
		glDeleteShader(shader);
		return 0;
	}

	free(src);

	glAttachShader(prg, shader);
	glDeleteShader(shader);

	return 1;
}

// Create shader program from vertex shader and fragment shader files
GLuint COceanSim::loadGLSLProgram( const char *vertFileName, const char *fragFileName )
{
	GLint linked;
	GLuint program;

	program = glCreateProgram();

	if (!attachShader(program, GL_VERTEX_SHADER, vertFileName))
	{
		glDeleteProgram(program);
		fprintf(stderr, "Couldn't attach vertex shader from file %s\n", vertFileName);
		return 0;
	}

	if (!attachShader(program, GL_FRAGMENT_SHADER, fragFileName))
	{
		glDeleteProgram(program);
		fprintf(stderr, "Couldn't attach fragment shader from file %s\n", fragFileName);
		return 0;
	}

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		glDeleteProgram(program);
		char temp[256];
		glGetProgramInfoLog(program, 256, 0, temp);
		fprintf(stderr, "Failed to link program: %s\n", temp);
		return 0;
	}

	return program;
}

void COceanSim::generate_h0( float2 *h0 )
{
	for (unsigned int y = 0; y<=meshSize; y++)
	{
		for (unsigned int x = 0; x<=meshSize; x++)
		{
			float kx = (-(int)meshSize / 2.0f + x) * (2.0f * CUDART_PI_F / patchSize);
			float ky = (-(int)meshSize / 2.0f + y) * (2.0f * CUDART_PI_F / patchSize);

			float P = sqrtf(phillips(kx, ky, windDir, windSpeed, A, dirDepend));

			if (kx == 0.0f && ky == 0.0f)
			{
				P = 0.0f;
			}

			//float Er = urand()*2.0f-1.0f;
			//float Ei = urand()*2.0f-1.0f;
			float Er = gauss();
			float Ei = gauss();

			float h0_re = Er * P * CUDART_SQRT_HALF_F;
			float h0_im = Ei * P * CUDART_SQRT_HALF_F;

			int i = y*spectrumW+x;
			h0[i].x = h0_re;
			h0[i].y = h0_im;
		}
	}
}

float COceanSim::urand()
{
	return rand() / (float)RAND_MAX;
}

// Generates Gaussian random number with mean 0 and standard deviation 1.
float COceanSim::gauss()
{
	float u1 = urand();
	float u2 = urand();

	if (u1 < 1e-6f)
	{
		u1 = 1e-6f;
	}

	return sqrtf(-2 * logf(u1)) * cosf(2*CUDART_PI_F * u2);
}

// Phillips spectrum
// (Kx, Ky) - normalized wave vector
// Vdir - wind angle in radians
// V - wind speed
// A - constant
float COceanSim::phillips(float Kx, float Ky, float Vdir, float V, float A, float dir_depend)
{
	float k_squared = Kx * Kx + Ky * Ky;

	if (k_squared == 0.0f)
	{
		return 0.0f;
	}

	// largest possible wave from constant wind of velocity v
	float L = V * V / g;

	float k_x = Kx / sqrtf(k_squared);
	float k_y = Ky / sqrtf(k_squared);
	float w_dot_k = k_x * cosf(Vdir) + k_y * sinf(Vdir);

	float phillips = A * expf(-1.0f / (k_squared * L * L)) / (k_squared * k_squared) * w_dot_k * w_dot_k;

	// filter out waves moving opposite to wind
	if (w_dot_k < 0.0f)
	{
		phillips *= dir_depend;
	}

	// damp out waves with very small length w << l
	float w = L / 10000;
	//phillips *= expf(-k_squared * w * w);

	return phillips;
}

void COceanSim::initialize()
{
	initGL();

	initializeVBO();

	initializeCUDA();

}

void COceanSim::WriteToFile( CFile &fp )
{
	fp.Write(&bRenderOcean , sizeof(bool));
}

void COceanSim::ReadFromFile( CFile &fp )
{
	fp.Read(&bRenderOcean , sizeof(bool));
}

void COceanSim::setVisible( bool bVisible )
{
	bRenderOcean = bVisible;
}
