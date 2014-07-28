
#ifndef COCEANSIM_H
#define COCEANSIM_H

// CUDA runtime
#include <cuda_runtime.h>
#include <helper_functions.h>    // includes cuda.h and cuda_runtime_api.h
#include <helper_cuda.h>         // helper functions for CUDA error check
#include <cuda_gl_interop.h>
#include <helper_cuda_gl.h>      // helper functions for CUDA/GL interop
#include <math_constants.h>

// CUDA lib
#include <cufft.h>

class COceanSim
{
public:
	/*	Constructor. */
	COceanSim();

	/*	Destructor. */
	virtual ~COceanSim();

	void initialize();
	void cleanUp();

	void render();

	void WriteToFile(CFile &fp);

	void ReadFromFile(CFile &fp);

	void setVisible(bool bVisible);

protected:
	void initializeCUDA( );

	void initializeVBO();

	void renderVBO();

	void modifyVBO();

	// GL functionality
	bool initGL();
	void createVBO(GLuint *vbo, int size);
	void deleteVBO(GLuint *vbo);
	void createMeshIndexBuffer(GLuint *id, int w, int h);
	void createMeshPositionVBO(GLuint *id, int w, int h);
	GLuint loadGLSLProgram(const char *vertFileName, const char *fragFileName);
	int attachShader(GLuint prg, GLenum type, const char *name);

protected:
	void generate_h0(float2 *h0);


protected:
	bool runCUDAHost();
	void setDefault();

	float urand();
	float gauss();
	float phillips(float Kx, float Ky, float Vdir, float V, float A, float dir_depend);

private:

	////////////////////////////////////////////////////////////////////////////////
	// constants
	const unsigned int windowW , windowH ;

	const unsigned int meshSize ;
	const unsigned int spectrumW ;
	const unsigned int spectrumH ;

	const int frameCompare ;

	// OpenGL vertex buffers
	GLuint posVertexBuffer;
	GLuint heightVertexBuffer, slopeVertexBuffer;
	struct cudaGraphicsResource *cuda_posVB_resource, *cuda_heightVB_resource, *cuda_slopeVB_resource; // handles OpenGL-CUDA exchange

	GLuint indexBuffer;
	GLuint shaderProg;

	// FFT data
	cufftHandle fftPlan;
	float2 *d_h0;   // heightfield at time 0
	float2 *h_h0;
	float2 *d_ht;   // heightfield at time t
	float2 *d_slope;

	// pointers to device object
	float *g_hptr;
	float2 *g_sptr;

	// simulation parameters
	const float g ;              // gravitational constant
	const float A ;              // wave scale factor
	const float patchSize ;        // patch size
	float windSpeed;
	float windDir ;
	float dirDepend ;

	StopWatchInterface *timer;
	float animTime ;
	float prevTime ;
	float animationRate ;

	// Auto-Verification Code
	const int frameCheckNumber ;
	int fpsCount ;        // FPS count for averaging
	int fpsLimit ;        // FPS limit for sampling
	unsigned int frameCount ;
	unsigned int g_TotalErrors ;

	bool animate ;
	bool drawPoints ;
	bool wireFrame ;
	bool g_hasDouble ;

	bool		bRenderOcean;

};


#endif // COCEANSIM_H

