#pragma once

#include <gl/GL.h>
#include "vgPrerequisites.h"

#include "vgObjNode.h"

#include "tvgArchiveManager.h"
#include "tvgTextureManager.h"

using namespace vgFoundationSystem;

// glWnd

class glScene
{

public:
	glScene();
	virtual ~glScene();

	// add member function and variable
	void LoadModel( std::string filename );
	void AddObject( vgObject* pObject );
	void renderObject( );
	void cleanup();
	void initialize();
	void render();

protected:
	
	std::vector<vgObject*>	m_objects;

};


