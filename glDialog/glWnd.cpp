// glWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "glDialog.h"
#include "glWnd.h"


// glWnd

IMPLEMENT_DYNAMIC(glWnd, CWnd)

glWnd::glWnd()
{
	step = 0.0;
	s = 0.1;

	//	视点位置
	g_eye[0] = 0.0f;	
	g_eye[1] = 0.0f;	
	g_eye[2] = 1000.0f;	

	//	焦点位置
	g_look[0] = 0.0f;	
	g_look[1] = 0.0f;	
	g_look[2] = -1.0f;	// 朝屏幕里面看

	//	头顶法向
	g_up[0] = 0.0f;		
	g_up[1] = 1.0f;		// 头顶向上，不歪
	g_up[2] = 0.0f;		

}

glWnd::~glWnd()
{
	cleanup();
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hglrc);//删除渲染描述表
	::ReleaseDC(m_hWnd,hdc);//释放设备描述表
}


BEGIN_MESSAGE_MAP(glWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// glWnd 消息处理程序




int glWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
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
	
	initialize();


	return 0;
}


void glWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除颜色缓存和深度缓存

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		g_eye[0],	g_eye[1],	g_eye[2],
		g_look[0],	g_look[1],	g_look[2],
		g_up[0],	g_up[1],	g_up[2]);

	renderObject();

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

	// 不为绘图消息调用 CWnd::OnPaint()
}

int glWnd::MySetPixelFormat(HDC hdc)
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

void glWnd::LoadModel( std::string filename )
{
	// cache path
	std::string filefolderpath = filename;
	filefolderpath.erase( filename.find_last_of("\\/") , std::string::npos );

	try
	{
		ArchiveManager &archmgr = ArchiveManager::getSingleton();

		ArchivePtr parch( new FileSystemArchive(filefolderpath ,"filesystem") );

		parch->load();

		static int weight = 400;

		archmgr.addArchive( weight ++ , parch );
	}
	catch (vgFoundationSystem::Exception &e)
	{
		AfxMessageBox( e.getFullDescription().c_str() );
	}


	CFile fp;

	int numOfObj;

	long *ObjPosIndex;

	vgObject *pobj;

	if (!fp.Open(filename.c_str() , CFile::modeRead))
	{
		ostringstream ErrorMes;

		ErrorMes<<filename.c_str()<<"open failed!";

		AfxMessageBox(ErrorMes.str().c_str());

		return;
	}

	///////////////文件格式///////////////////
	char	fileFormat[32]="";
	fp.Read( fileFormat, 32);
	char	fmt[6]="";
	strncpy( fmt, fileFormat, 5);
	assert ( !strcmp( fmt, "VRGIS") );

	// 文件版本
	long		fileVersion;
	fp.Read( &fileVersion, sizeof(long) );
	assert ( 100 == fileVersion );
	///////////////////////////////////////

	fp.Read(&numOfObj , sizeof(int));

	ObjPosIndex = new long[numOfObj];

	fp.Read(ObjPosIndex , sizeof(long) * numOfObj);

	////////////////////////以上读入文件头，以下读入obj数据////////////////////////

	for (int i = 0; i < numOfObj ; i++)
	{

		pobj = new vgObject;

		long objchunklength;

		long pos = 0;

		fp.Seek(ObjPosIndex[i] , CFile::begin);

		fp.Read(&objchunklength , sizeof(long));

		char *objdata = new char[objchunklength];

		fp.Read(objdata , objchunklength);
		////////////////////////////////////////////////////////////////
		// 		IndexObject->m_id = GetNodeId()/*i+nodenumold*/;
		// 
		// 		IndexObject->m_isInMemory = true;

		pobj->m_BoundryBox.maxPoint.x = *(float *)(objdata + pos);
		pos += sizeof(float);

		pobj->m_BoundryBox.minPoint.x = *(float *)(objdata + pos);
		pos += sizeof(float);

		pobj->m_BoundryBox.maxPoint.y = *(float *)(objdata + pos);
		pos += sizeof(float);

		pobj->m_BoundryBox.minPoint.y = *(float *)(objdata + pos);
		pos += sizeof(float);

		pobj->m_BoundryBox.maxPoint.z = *(float *)(objdata + pos);
		pos += sizeof(float);

		pobj->m_BoundryBox.minPoint.z = *(float *)(objdata + pos);
		pos += sizeof(float);
		/////////////////////////////给位置赋值/////////////////////////////////
		pobj->UpdatePos();

		pobj->DateObj(objdata + pos);

		AddObject(pobj);

		delete[] objdata;

		objdata = NULL;
	}

	fp.Close();


}

void glWnd::AddObject( vgObject* pObject )
{
	m_objects.push_back( pObject );
}

void glWnd::renderObject()
{
	for ( int i=0; i<m_objects.size(); i++ )
	{
		m_objects[i]->Render();
	}
}

void glWnd::cleanup()
{
	for ( int i=0; i<m_objects.size(); i++ )
	{
		delete m_objects[i];
	}
	m_objects.clear();

	if ( m_pTextureManager != NULL )
	{
		delete m_pTextureManager;
	}

	if ( m_pArchiveManger != NULL )
	{
		delete m_pArchiveManger;
	}
}

void glWnd::initialize()
{

	// Create Texture manager if there is no Texture manager yet
	if( vgFoundationSystem::TextureManager::getSingletonPtr() == 0 )
	{
		m_pTextureManager = new TextureManager();
	}
	else 
	{
		assert( 0 && "TextureManager已被创建! " );
	}

	// Create Archive manager if there is no Archive manager yet
	if( vgFoundationSystem::ArchiveManager::getSingletonPtr() == 0 )
	{
		m_pArchiveManger = new ArchiveManager( true );
	}
	else 
	{
		assert( 0 && "ArchiveManager已被创建! " );
	}

}



void glWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	glViewport (0, 0, (GLsizei)(cx), (GLsizei)(cy));				// Reset The Current Viewport
	glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity ();													// Reset The Projection Matrix
	gluPerspective (45.0f, (GLfloat)(cx)/(GLfloat)(cy),			// Calculate The Aspect Ratio Of The Window
		1.0f, 10000.0f);		
	glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity ();			
}
