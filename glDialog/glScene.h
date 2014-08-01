#pragma once

#include <gl/GL.h>
#include "vgPrerequisites.h"

#include "vgObjNode.h"

#include "tvgArchiveManager.h"
#include "tvgTextureManager.h"

using namespace vgFoundationSystem;

// glScene

struct scenetreenode
{
public:

	scenetreenode *lchild;
	scenetreenode *rchild;
	//add
	scenetreenode *lupchild;
	scenetreenode *rupchild;

	scenetreenode *paraent;
	vgBoundryBox aabb;
	Vector3 centerpoint;
	int NumofNode;

	bool m_isdivided;
	bool b_Lhas, b_Rhas, b_LUPhas, b_RUPhas;
	int m_depth;
	vector<int> NodeID;

	int NumofNodepface;                  //  �������

	unsigned long offset;
	unsigned long size;
};

class glScene
{

public:
	glScene();
	virtual ~glScene();

	// add member function and variable
	void OpenIOI( std::string filename );
	void LoadModel( std::string filename );
	void AddObject( vgObject* pObject );
	void renderObject( );
	void cleanup();
	void initialize();
	void render();

protected:
	void skipByte( CFile &fp, int nByteCount );

	void ReadDataToScene(CFile &fp, scenetreenode *node);
	unsigned long AddNodeFromVgFile( char *data );
	void GenTreefromFile(char *data , scenetreenode *node);
	void FreeNode(scenetreenode *node);

protected:
	
	std::vector<vgObject*>	m_objects;
	scenetreenode rootnode;

	long ReadFilePos;

};


