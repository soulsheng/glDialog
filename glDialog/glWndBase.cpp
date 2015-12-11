// glWndBase.cpp : 实现文件
//

#include "stdafx.h"
#include "glWndBase.h"
#include "bmpHandler.h"

#define CLIP_FAR_DISTANCE	100000	// 10000
#define FRAME_BUFFER_WIDTH	1024
#define FRAME_BUFFER_HEIGHT	600

// glWndBase

IMPLEMENT_DYNAMIC(glWndBase, CWnd)

glWndBase::glWndBase()
{
	step = 0.0;
	s = 0.1;

	m_pFrameBuffer = NULL;

}

glWndBase::~glWndBase()
{
	cleanup();
}


BEGIN_MESSAGE_MAP(glWndBase, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// glWndBase 消息处理程序




int glWndBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	initializeGL();


	return 0;
}


void glWndBase::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	wglMakeCurrent(hdc, hglrc); 

	renderAll();


#if 0
	s+=0.005;
	if(s>1.0)
		s=0.1;
	step = step + 1.0;
	if (step > 360.0)
		step = step - 360.0;
	glPushMatrix();
	glScalef(s,s,s);
	glRotatef(step,0.0,1.0,0.0);
	glRotatef(step,0.0,0.0,1.0);
	glRotatef(step,1.0,0.0,0.0);
	DrawColorBox();
	glPopMatrix();
	glFlush();
#endif
	SwapBuffers(hdc);

	// 不为绘图消息调用 CWnd::OnPaint()
}

int glWndBase::MySetPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR), // pfd结构的大小 
		1, // 版本号 
		PFD_DRAW_TO_WINDOW | // 支持在窗口中绘图 
		PFD_SUPPORT_OPENGL | // 支持 OpenGL 
		PFD_DOUBLEBUFFER, // 双缓存模式 
		PFD_TYPE_RGBA, // RGBA 颜色模式 
		24, // 24 位颜色深度 
		0, 0, 0, 0, 0, 0, // 忽略颜色位 
		0, // 没有非透明度缓存 
		0, // 忽略移位位 
		0, // 无累加缓存 
		0, 0, 0, 0, // 忽略累加位 
		32, // 32 位深度缓存 
		0, // 无模板缓存 
		0, // 无辅助缓存 
		PFD_MAIN_PLANE, // 主层 
		0, // 保留 
		0, 0, 0 // 忽略层,可见性和损毁掩模 
	}; 

	int iPixelFormat; 

	// 为设备描述表得到最匹配的像素格式 
	if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		MessageBox("ChoosePixelFormat Failed", NULL, MB_OK);
		return 0;
	}

	// 设置最匹配的像素格式为当前的像素格式 
	if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat Failed", NULL, MB_OK);
		return 0;
	}

	return 1;
}


void glWndBase::DrawColorBox(void)
{
	GLfloat p1[]={0.5,-0.5,-0.5}, p2[]={0.5,0.5,-0.5},
		p3[]={0.5,0.5,0.5}, p4[]={0.5,-0.5,0.5},
		p5[]={-0.5,-0.5,0.5}, p6[]={-0.5,0.5,0.5},
		p7[]={-0.5,0.5,-0.5}, p8[]={-0.5,-0.5,-0.5};

	GLfloat m1[]={1.0,0.0,0.0}, m2[]={-1.0,0.0,0.0},
		m3[]={0.0,1.0,0.0}, m4[]={0.0,-1.0,0.0},
		m5[]={0.0,0.0,1.0}, m6[]={0.0,0.0,-1.0};

	GLfloat c1[]={0.0,0.0,1.0}, c2[]={0.0,1.0,1.0},
		c3[]={1.0,1.0,1.0}, c4[]={1.0,0.0,1.0},
		c5[]={1.0,0.0,0.0}, c6[]={1.0,1.0,0.0},
		c7[]={0.0,1.0,0.0}, c8[]={1.0,1.0,1.0};

	glBegin (GL_QUADS); //绘制多个四边形

	glColor3fv(c1);
	glNormal3fv(m1);
	glVertex3fv(p1);
	glColor3fv(c2);
	glVertex3fv(p2);
	glColor3fv(c3);
	glVertex3fv(p3);
	glColor3fv(c4);
	glVertex3fv(p4);

	glColor3fv(c5);
	glNormal3fv(m5);
	glVertex3fv(p5);
	glColor3fv(c6);
	glVertex3fv(p6);
	glColor3fv(c7);
	glVertex3fv(p7);
	glColor3fv(c8);
	glVertex3fv(p8);

	glColor3fv(c5);
	glNormal3fv(m3);
	glVertex3fv(p5);
	glColor3fv(c6);
	glVertex3fv(p6);
	glColor3fv(c3);
	glVertex3fv(p3);
	glColor3fv(c4);
	glVertex3fv(p4);

	glColor3fv(c1);
	glNormal3fv(m4);
	glVertex3fv(p1);
	glColor3fv(c2);
	glVertex3fv(p2);
	glColor3fv(c7);
	glVertex3fv(p7);
	glColor3fv(c8);
	glVertex3fv(p8);

	glColor3fv(c2);
	glNormal3fv(m5);
	glVertex3fv(p2);
	glColor3fv(c3);
	glVertex3fv(p3);
	glColor3fv(c6);
	glVertex3fv(p6);
	glColor3fv(c7);
	glVertex3fv(p7);

	glColor3fv(c1);
	glNormal3fv(m6);
	glVertex3fv(p1);
	glColor3fv(c4);
	glVertex3fv(p4);
	glColor3fv(c5);
	glVertex3fv(p5);
	glColor3fv(c8);
	glVertex3fv(p8);

	glEnd();

}

void glWndBase::cleanup()
{

	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hglrc);//删除渲染描述表
	::ReleaseDC(m_hWnd,hdc);//释放设备描述表

}

void glWndBase::initializeGL()
{
	MySetPixelFormat(::GetDC(m_hWnd));

	// 获得绘图描述表
	hdc = ::GetDC(m_hWnd);
	// 创建渲染描述表
	hglrc = wglCreateContext(hdc);
	// 使绘图描述表为当前调用现程的当前绘图描述表
	wglMakeCurrent(hdc, hglrc); 

	// 初始化Glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		AfxMessageBox( "glewInit failed, something is seriously wrong." );
	}

	glClearColor(0.5f,0.6f,0.8f,1.0f);
	glEnable( GL_DEPTH_TEST );// 开启遮挡


#if 1
	GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[]=	{ 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One
	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);
#endif
}



void glWndBase::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	glViewport (0, 0, (GLsizei)(cx), (GLsizei)(cy));				// Reset The Current Viewport
	glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity ();													// Reset The Projection Matrix
	gluPerspective (45.0f, (GLfloat)(cx)/(GLfloat)(cy),			// Calculate The Aspect Ratio Of The Window
		0.01f, CLIP_FAR_DISTANCE );		
	glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity ();			
}


void glWndBase::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_camera.OnMouseMove( nFlags, point.x, point.y );

	CWnd::OnMouseMove(nFlags, point);
}


BOOL glWndBase::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_camera.OnMouseWheel( zDelta );

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void glWndBase::renderAll()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除颜色缓存和深度缓存

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	m_camera.look();

	render();
}

void glWndBase::screenShot()
{
#if 1 // frame buffer
	glPushAttrib(GL_VIEWPORT_BIT);

	if (m_pFrameBuffer != NULL)
	{
		delete m_pFrameBuffer;
	}

	int width, height;
	int x, y;
	int viewport[4]; /* x, y, width, height */
	glGetIntegerv(GL_VIEWPORT, viewport);
	x = viewport[0];
	y = viewport[1];
	width = viewport[2];
	height = viewport[3];

	m_pFrameBuffer = new FrameBuffer( width, height );
	m_pFrameBuffer->create();

	m_pFrameBuffer->bind();
#endif

	renderAll();


#if 1 // frame buffer
	char* buf = new char[ 3 * width * height ];
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(x, y, width, height , GL_BGR , GL_UNSIGNED_BYTE , buf);

	//vgImage::CximageWrapper bmpimg;

	//bmpimg.createFromArray(buf, m_bmpWidth, m_bmpHeight);

	//bmpimg.saveToFile(m_bmpFilePath);
	BMPHandler::saveImage( "1.bmp", buf, height, width );

	delete[] buf;

	m_pFrameBuffer->unbind();

	m_pFrameBuffer->destroy();

	glPopAttrib();
#endif
}


void glWndBase::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( nChar == 'O' || nChar == 'o')
	{
		screenShot();
	}
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}
