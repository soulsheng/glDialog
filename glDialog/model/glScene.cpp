// glWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "glScene.h"

#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

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

	CFile fp;

	int numOfObj;

	long *ObjPosIndex;

	vgFoundationSystem::vgObject *pobj;

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

		pobj = new vgFoundationSystem::vgObject;

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

void glScene::AddObject( vgFoundationSystem::vgObject* pObject )
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

void glScene::initialize(cameranode *pCamera)
{
	ReadFilePos =0;
	m_pCamera = pCamera;
}

void glScene::skipByte( CFile &fp, int nByteCount )
{
	char*	nByteSkip = new char[nByteCount];
	fp.Read( nByteSkip, nByteCount );
	delete [] nByteSkip;
}

void glScene::OpenIOImage( std::string filename )
{
	string ImgPathName(filename);

	std::string::size_type kk = ImgPathName.length();

	ImgPathName.erase(kk - 3, std::string::npos);

	ImgPathName = ImgPathName + "ioimg";

	try
	{
		vgFoundationSystem::ArchiveManager &archmgr = vgFoundationSystem::ArchiveManager::getSingleton();

		vgFoundationSystem::ArchivePtr parch( new vgFoundationSystem::VgimgArchive(ImgPathName ,"ioimg") );

		parch->load();

		archmgr.addArchive( 300, parch );

		//			VG_TRACE( archmgr.getDetails() );
	}
	catch (vgFoundationSystem::Exception &e)
	{
		MessageBox( NULL , e.getFullDescription().c_str() , "ERROR" , MB_OK );
	}
}

void glScene::OpenIOI( std::string filename )
{
	OpenIOImage( filename );

	CFile pfile;
	if (!pfile.Open(filename.c_str() , CFile::modeRead))
	{
		ostringstream ErrorMes;

		ErrorMes<<filename.c_str()<<"open failed!";

		AfxMessageBox(ErrorMes.str().c_str());

		return;
	}

	int filelength = pfile.GetLength();

	char *vgfileheader;
	vgfileheader = new char[20];
	pfile.Read(vgfileheader, 20);

	unsigned long treepos;
	pfile.Read(&treepos , sizeof(unsigned long));

	int	m_nodenum;
	pfile.Read(&m_nodenum , sizeof(int));

	///////////////////////////////////////////////////////////////////////
#if 0	// skip camera read, 133 bytes
	vgCurrentCamera.ReadCameraFromFile(pfile);
#else
	skipByte( pfile, 61 );
	
	// read 24 bytes
	pfile.Read( m_pCamera->g_eye , sizeof(float)*3 );
	pfile.Read( m_pCamera->g_dir , sizeof(float)*3 );
	for ( int i = 0; i<3; i++ )
		m_pCamera->g_dir[i] -= m_pCamera->g_eye[i];

	skipByte( pfile, 48 );
#endif

#if 0	// skip light read, 12 bytes
	pfile.Read(&Scene.m_LightPosForShadow.x , sizeof(float));
	pfile.Read(&Scene.m_LightPosForShadow.y , sizeof(float));
	pfile.Read(&Scene.m_LightPosForShadow.z , sizeof(float));
#else
	skipByte( pfile, 12 );
#endif


#if 0	// skip small map read, 25 bytes
	Scene.vgMap.ReadFromFile(pfile);
#else
	skipByte( pfile, 25 );
#endif

#if 0	// skip ocean read, 1 byte
	Scene.oceanSim.ReadFromFile(pfile);
#else	
	skipByte( pfile, 1 );
#endif

	/////////////////////动画//////////////////////////////////////
#if 0	// skip animation read, 4 bytes
	int num;
	pfile.Read(&num, sizeof(int));
#else
	skipByte( pfile, 4 );
#endif	
	

#if 0	// skip Index data, 60 * m_nodenum bytes
	char* Indexdata = new char[60 * m_nodenum];   //40 = abstract length

	pfile.Read(Indexdata , 60 * m_nodenum);

	AddAbstractNodeFromVgFile(Indexdata , m_nodenum);
#else
	skipByte( pfile, 60 * m_nodenum );
#endif


	int treelength = filelength - treepos;


#if 1	// skip quad tree read
	pfile.Seek(treepos , CFile::begin);
	char *treedata = new char[treelength];
	pfile.Read(treedata , treelength);

	GenTreefromFile(treedata , &rootnode);

	delete[] treedata;
	treedata = NULL;
#else
	skipByte( pfile, treelength );
#endif

	///////////////////////////////////////////////////////////////////////////////
	ReadDataToScene(pfile ,&rootnode);

	FreeNode( &rootnode );

	pfile.Close();

}

void glScene::ReadDataToScene(CFile &fp, scenetreenode *node)
{
	if (node == NULL)
	{
		return;
	}
	else
	{
		if (true)                   //判断是否符合读取条件，该处直接用根节点来读取场景
		{
			unsigned long pos = 0;

			unsigned long size;

			char *data;

			data = new char[node->size];

			fp.Seek(node->offset , CFile::begin);

			fp.Read(data , node->size);

			for (int i = 0; i < node->NumofNode ; i++)
			{
				size = AddNodeFromVgFile(data + pos);

				pos += size;			
			}

			delete[] data;

			data = NULL;

		}
		else
		{
			ReadDataToScene(fp, node->lchild);

			ReadDataToScene(fp, node->lupchild);

			ReadDataToScene(fp, node->rchild);

			ReadDataToScene(fp, node->rupchild);
		}

	}

}

unsigned long glScene::AddNodeFromVgFile( char *data )
{
	unsigned long pos = 0;

	int Type;

	Type = *(int *)(data + pos);

	pos += sizeof(int);

	// if (Type == VG_OBJECT)                   //vgObject
	{
		vgFoundationSystem::vgObject *objnode = new vgFoundationSystem::vgObject;

		pos += objnode->PositDataToNode(data + pos);

		AddObject(objnode);

	}

	return pos;	

}

void glScene::GenTreefromFile(char *data, scenetreenode *node)
{
	if (!data)
	{
		return;
	}
	else
	{		
		node->aabb.maxPoint.x = *(float *)(data+ReadFilePos);
		ReadFilePos += sizeof(float);
		node->aabb.maxPoint.y = *(float *)(data+ReadFilePos);
		ReadFilePos += sizeof(float);
		node->aabb.maxPoint.z = *(float *)(data+ReadFilePos);
		ReadFilePos += sizeof(float);
		node->aabb.minPoint.x = *(float *)(data+ReadFilePos);
		ReadFilePos += sizeof(float);
		node->aabb.minPoint.y = *(float *)(data+ReadFilePos);
		ReadFilePos += sizeof(float);
		node->aabb.minPoint.z = *(float *)(data+ReadFilePos);
		ReadFilePos += sizeof(float);

		node->centerpoint.x = *(float *)(data + ReadFilePos);
		ReadFilePos += sizeof(float);
		node->centerpoint.y = *(float *)(data + ReadFilePos);
		ReadFilePos += sizeof(float);
		node->centerpoint.z = *(float *)(data + ReadFilePos);
		ReadFilePos += sizeof(float);

		node->m_depth = *(int *)(data +ReadFilePos);
		ReadFilePos += sizeof(int);
		node->m_isdivided = *(bool *)(data + ReadFilePos);
		ReadFilePos += sizeof(bool);


		node->b_Lhas = *(bool *)(data + ReadFilePos);
		ReadFilePos += sizeof(bool);

		node->b_LUPhas = *(bool *)(data + ReadFilePos);
		ReadFilePos += sizeof(bool);

		node->b_Rhas = *(bool *)(data + ReadFilePos);
		ReadFilePos += sizeof(bool);

		node->b_RUPhas = *(bool *)(data + ReadFilePos);
		ReadFilePos += sizeof(bool);

		node->NumofNodepface = *(int *)(data + ReadFilePos);
		ReadFilePos += sizeof(int);

		node->NumofNode = *(int *)(data + ReadFilePos);
		ReadFilePos += sizeof(int);

		for (int i = 0; i < node->NumofNode; i++)
		{
			int k = *(int *)(data + ReadFilePos);

			ReadFilePos += sizeof(int);

			node->NodeID.push_back(k);
		}

		node->offset = *(unsigned long *)(data + ReadFilePos);
		ReadFilePos += sizeof(unsigned long);

		node->size = *(unsigned long *)(data + ReadFilePos);
		ReadFilePos += sizeof(unsigned long);

		ostringstream aaa;

		aaa<<node->b_Lhas<<" "<<node->b_LUPhas<<" "<<node->b_Rhas<<" "<<node->b_RUPhas;


		if (node->m_isdivided == true)
		{
			if (node->b_Lhas)
			{
				node->lchild = new scenetreenode;
				node->lchild->paraent = node;
				GenTreefromFile(data, node->lchild);
			}

			else
				node->lchild = NULL;

			if (node->b_LUPhas)
			{
				node->lupchild = new scenetreenode;	
				node->lupchild->paraent = node;
				GenTreefromFile(data, node->lupchild);
			}

			else
				node->lupchild = NULL;
			if (node->b_Rhas)
			{
				node->rchild = new scenetreenode;
				node->rchild->paraent = node;
				GenTreefromFile(data, node->rchild);
			}

			else
				node->rchild = NULL;

			if (node->b_RUPhas)
			{
				node->rupchild = new scenetreenode;
				node->rupchild->paraent = node;
				GenTreefromFile(data, node->rupchild);
			}

			else
				node->rupchild = NULL;
		}
		else
		{

			node->lchild = NULL;

			node->lupchild = NULL;

			node->rchild = NULL;

			node->rupchild = NULL;

			return;
		}
	}

}

void glScene::FreeNode( scenetreenode *node )
{
	if (!node)
	{
		return;
	}

	if (node->lchild != NULL) 
		FreeNode(node->lchild);
	if(node->lupchild != NULL)
		FreeNode(node->lupchild);
	if(node->rchild != NULL)
		FreeNode(node->rchild);
	if(node->rupchild != NULL)
		FreeNode(node->rupchild);

	if (node->m_depth > 1 &&
		node->lchild == NULL && node->lupchild == NULL
		&& node->lupchild == NULL && node->rupchild == NULL	)
	{
		delete node;
		node = NULL;
	}
}

scenetreenode::scenetreenode()
{
	paraent = NULL;
	m_depth = 1;
	m_isdivided = true;
	NumofNode = 0;
	NumofNodepface = 0 ;

	lchild = NULL;
	rchild = NULL;
	lupchild = NULL;
	rupchild = NULL;

	b_Lhas = false;
	b_Rhas = false;
	b_RUPhas = false;
	b_LUPhas = false;
}

cameranode::cameranode()
{
	//	视点位置
	g_eye[0] = 0.0f;	
	g_eye[1] = 0.0f;	
	g_eye[2] = 1000.0f;	

	//	视线方向
	g_dir[0] = 0.0f;	
	g_dir[1] = 0.0f;	
	g_dir[2] = -1.0f;	// 默认朝屏幕里面看

	//	头顶法向
	g_up[0] = 0.0f;		
	g_up[1] = 1.0f;		// 头顶向上，不歪
	g_up[2] = 0.0f;		

	g_Angle = 0.0f;
	g_speed = 2.0f;
}

void cameranode::look()
{
	if (KEY_DOWN(VK_SHIFT) && (g_speed <= 10000))    g_speed   =g_speed*1.05;//按SHIFT时的加速
	if (KEY_DOWN(VK_CONTROL))  g_speed   =g_speed/1.05;//按CONTROL时的减速
	if (KEY_DOWN(VK_LEFT))     g_Angle-=0.5;//左转
	if (KEY_DOWN(VK_RIGHT))    g_Angle+=0.5;//右转

	if (KEY_DOWN(VK_UP))				//前进
	{
		g_eye[0]+=g_dir[0]*g_speed;
		g_eye[2]+=g_dir[2]*g_speed;
	}
	if (KEY_DOWN(VK_DOWN))			//后退
	{
		g_eye[0]-=g_dir[0]*g_speed;
		g_eye[2]-=g_dir[2]*g_speed; 
	}

	if (KEY_DOWN(VK_HOME))	g_eye[1] +=	g_speed/2;//上升
	if (KEY_DOWN(VK_END))	g_eye[1] -= g_speed/2;//下降

	g_dir[0] = cos(PI*g_Angle/180.0f);
	g_dir[2] = sin(PI*g_Angle/180.0f);

	gluLookAt(
		g_eye[0],	g_eye[1],	g_eye[2],
		g_eye[0] + g_dir[0],	g_eye[1] + g_dir[1],	g_eye[2] + g_dir[2],
		g_up[0],	g_up[1],	g_up[2]);

}
