// glWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "glScene.h"


glScene::glScene()
{
	
}

glScene::~glScene()
{
	cleanup();
}


void glScene::render()
{
	renderObject();
}

void glScene::LoadModel( std::string filename )
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

void glScene::AddObject( vgObject* pObject )
{
	m_objects.push_back( pObject );
}

void glScene::renderObject()
{
	for ( int i=0; i<m_objects.size(); i++ )
	{
		m_objects[i]->Render();
	}
}

void glScene::cleanup()
{
	for ( int i=0; i<m_objects.size(); i++ )
	{
		delete m_objects[i];
	}
	m_objects.clear();

}

void glScene::initialize()
{

}

