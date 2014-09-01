// glWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "glWnd.h"

// glWnd

IMPLEMENT_DYNAMIC(glWnd, CWnd)

glWnd::glWnd()
{
	step = 0.0;
	s = 0.1;

	

}

glWnd::~glWnd()
{
	cleanup();
}


BEGIN_MESSAGE_MAP(glWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// glWnd 消息处理程序




int glWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return glWndBase::OnCreate( lpCreateStruct );
}


void glWnd::OnPaint()
{
	glWndBase::OnPaint();
}

void glWnd::cleanup()
{
	m_ocean.cleanUp();
	m_scene.cleanup();

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
		m_pTextureManager = new vgFoundationSystem::TextureManager();
	}
	else 
	{
		assert( 0 && "TextureManager已被创建! " );
	}

	// Create Archive manager if there is no Archive manager yet
	if( vgFoundationSystem::ArchiveManager::getSingletonPtr() == 0 )
	{
		m_pArchiveManger = new vgFoundationSystem::ArchiveManager( true );
	}
	else 
	{
		assert( 0 && "ArchiveManager已被创建! " );
	}

	m_scene.initialize( &m_camera );
	m_ocean.initialize();
	m_ocean.setVisible( true );
}



void glWnd::OnSize(UINT nType, int cx, int cy)
{
	glWndBase::OnSize( nType, cx, cy );	
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

void glWnd::render()
{
	m_ocean.render();
	m_scene.render();
}
