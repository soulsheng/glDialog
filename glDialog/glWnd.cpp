// glWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "glWnd.h"

#define CLIP_FAR_DISTANCE	100000	// 10000

// glWnd

IMPLEMENT_DYNAMIC(glWnd, CWnd)

glWnd::glWnd()
{
	step = 0.0;
	s = 0.1;

	

}

glWnd::~glWnd()
{
	m_ocean.cleanUp();
	cleanup();
}


BEGIN_MESSAGE_MAP(glWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// glWnd ��Ϣ�������




int glWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	initialize();


	return 0;
}


void glWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����ɫ�������Ȼ���

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	m_camera.look();

	m_scene.render();
	m_ocean.render();
#if 1
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

	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
}

int glWnd::MySetPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR), // pfd�ṹ�Ĵ�С 
		1, // �汾�� 
		PFD_DRAW_TO_WINDOW | // ֧���ڴ����л�ͼ 
		PFD_SUPPORT_OPENGL | // ֧�� OpenGL 
		PFD_DOUBLEBUFFER, // ˫����ģʽ 
		PFD_TYPE_RGBA, // RGBA ��ɫģʽ 
		24, // 24 λ��ɫ��� 
		0, 0, 0, 0, 0, 0, // ������ɫλ 
		0, // û�з�͸���Ȼ��� 
		0, // ������λλ 
		0, // ���ۼӻ��� 
		0, 0, 0, 0, // �����ۼ�λ 
		32, // 32 λ��Ȼ��� 
		0, // ��ģ�建�� 
		0, // �޸������� 
		PFD_MAIN_PLANE, // ���� 
		0, // ���� 
		0, 0, 0 // ���Բ�,�ɼ��Ժ������ģ 
	}; 

	int iPixelFormat; 

	// Ϊ�豸������õ���ƥ������ظ�ʽ 
	if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		MessageBox("ChoosePixelFormat Failed", NULL, MB_OK);
		return 0;
	}

	// ������ƥ������ظ�ʽΪ��ǰ�����ظ�ʽ 
	if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat Failed", NULL, MB_OK);
		return 0;
	}

	return 1;
}


void glWnd::DrawColorBox(void)
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

	glBegin (GL_QUADS); //���ƶ���ı���

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

void glWnd::cleanup()
{
	m_scene.cleanup();

	if ( m_pTextureManager != NULL )
	{
		delete m_pTextureManager;
	}

	if ( m_pArchiveManger != NULL )
	{
		delete m_pArchiveManger;
	}


	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hglrc);//ɾ����Ⱦ������
	::ReleaseDC(m_hWnd,hdc);//�ͷ��豸������

}

void glWnd::initialize()
{
	MySetPixelFormat(::GetDC(m_hWnd));

	// ��û�ͼ������
	hdc = ::GetDC(m_hWnd);
	// ������Ⱦ������
	hglrc = wglCreateContext(hdc);
	// ʹ��ͼ������Ϊ��ǰ�����̵ֳĵ�ǰ��ͼ������
	wglMakeCurrent(hdc, hglrc); 

	// ��ʼ��Glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		AfxMessageBox( "glewInit failed, something is seriously wrong." );
	}

	glClearColor(0.5f,0.6f,0.8f,1.0f);

	// Create Texture manager if there is no Texture manager yet
	if( vgFoundationSystem::TextureManager::getSingletonPtr() == 0 )
	{
		m_pTextureManager = new vgFoundationSystem::TextureManager();
	}
	else 
	{
		assert( 0 && "TextureManager�ѱ�����! " );
	}

	// Create Archive manager if there is no Archive manager yet
	if( vgFoundationSystem::ArchiveManager::getSingletonPtr() == 0 )
	{
		m_pArchiveManger = new vgFoundationSystem::ArchiveManager( true );
	}
	else 
	{
		assert( 0 && "ArchiveManager�ѱ�����! " );
	}

	m_scene.initialize( &m_camera );
	m_ocean.initialize();
	m_ocean.setVisible( true );
}



void glWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	glViewport (0, 0, (GLsizei)(cx), (GLsizei)(cy));				// Reset The Current Viewport
	glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity ();													// Reset The Projection Matrix
	gluPerspective (45.0f, (GLfloat)(cx)/(GLfloat)(cy),			// Calculate The Aspect Ratio Of The Window
		1.0f, CLIP_FAR_DISTANCE );		
	glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity ();			
}

void glWnd::cachePath( std::string filename )
{
	// cache path
	std::string filefolderpath = filename;
	filefolderpath.erase( filename.find_last_of("\\/") , std::string::npos );

	try
	{
		vgFoundationSystem::ArchiveManager &archmgr = vgFoundationSystem::ArchiveManager::getSingleton();

		vgFoundationSystem::ArchivePtr parch( new vgFoundationSystem::FileSystemArchive(filefolderpath ,"filesystem") );

		parch->load();

		static int weight = 400;

		archmgr.addArchive( weight ++ , parch );
	}
	catch (vgFoundationSystem::Exception &e)
	{
		AfxMessageBox( e.getFullDescription().c_str() );
	}
}

void glWnd::LoadModel( std::string filename )
{
	cachePath( filename );

	m_scene.LoadModel( filename );
}

void glWnd::OpenIOI( std::string filename )
{
	cachePath( filename );

	m_scene.OpenIOI( filename );
}
