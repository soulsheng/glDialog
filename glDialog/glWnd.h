#pragma once

#include "glWndBase.h"

#include "vgObjNode.h"

#include "tvgArchiveManager.h"
#include "tvgTextureManager.h"

#include "glScene.h"
#include "oceanFFT/COceanSim.h"

//using namespace vgFoundationSystem;

// glWnd

class glWnd : public glWndBase
{

public:
	glWnd();
	virtual ~glWnd();

	virtual	void initialize();
	virtual	void render();

	// add member function and variable
	void cachePath( std::string filename );
	void OpenIOI( std::string filename );
	void LoadModel( std::string filename );
	void cleanup();

	vgFoundationSystem::TextureManager	*m_pTextureManager;
	vgFoundationSystem::ArchiveManager *m_pArchiveManger;

	glScene	m_scene;
	COceanSim	m_ocean;

};


