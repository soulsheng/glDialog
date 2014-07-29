// vgObject.cpp: implementation of the vgObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "iniopengl.h"
#include "vgObjNode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace vgFoundationSystem{
	
	
	vgObject::vgObject()
	{
		m_Scale.x = 1;
		m_Scale.y = 1; 
		m_Scale.z = 1;
		
		IsLOD=false;
		TYPE = 1;
		
		farvis=-1;
		nearvis=0;
		
		bIsChild=false;
		bHasChild=false;


		pParent = NULL;

		pChild = NULL;
		
		mShadowVolume  = -1;
	}
	
	vgObject::~vgObject()
	{
		
		delete[] pMats;
		
		///////////////////////点处理//////////////////////////
		
		delete[] pVertex;
				
		for (int i = 0; i < m_numOfMat ; i ++)
		{
			delete[] pMatRender[i].pFace;
		}
		delete[] pMatRender;
		
		delete[] pFaceNoMat;
				
		delete[] pNormalOfFaces;	
	
		delete[] pVert;			

		delete[] pShadowFaces;		
	}
	
	
	void vgObject::Render()
	{
		if (m_isVisable)
		{
			glPushMatrix();
			glPushAttrib( GL_ALL_ATTRIB_BITS );

			glClientActiveTexture(GL_TEXTURE0);
			glTexCoordPointer(2, GL_FLOAT, sizeof(float)*8, &pVertex->texCoorS );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glVertexPointer(3, GL_FLOAT, sizeof(float)*8, &pVertex->positionX );
			glEnableClientState( GL_VERTEX_ARRAY );

			for (int i = 0; i < m_numOfMat; i++)
			{
				if (pMatRender[i].b_hastexture == true)
				{
// 					glDisable(GL_BLEND);
// 
// 					glDisable(GL_ALPHA_TEST);
					
					glEnable(GL_ALPHA_TEST);
					
					glAlphaFunc(GL_GREATER, 0);
					
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glEnable(GL_BLEND);
					
					glColor3d(1, 1, 1);		
					
					glEnable(GL_TEXTURE_2D);
					
					//glBindTexture(GL_TEXTURE_2D, pObject->pMats[i].texureId);
					
					pMats[i].pTexture->useBind();
					
					
					//glInterleavedArrays(GL_T2F_N3F_V3F, 0 , pVertex);
					
					glDrawElements(GL_TRIANGLES, pMatRender[i].m_numOfFace * 3,
						GL_UNSIGNED_INT,pMatRender[i].pFace);
					
					glDisable(GL_TEXTURE_2D);

					glDisable(GL_BLEND);
				}
				else
				{
					glDisable(GL_TEXTURE_2D);
					
					glColor3f (float(pMats[i].materialinfo.color[0]) / 255 ,float(pMats[i].materialinfo.color[1]) / 255, float(pMats[i].materialinfo.color[2])/255);
					
					//glInterleavedArrays(GL_T2F_N3F_V3F, 0 , pVertex);
					
					glDrawElements(GL_TRIANGLES, pMatRender[i].m_numOfFace * 3,
						GL_UNSIGNED_INT,pMatRender[i].pFace);
					
				}	
			}
			if (m_numOfNoMat != 0)
			{
				glDisable(GL_TEXTURE_2D);
				glColor3f(0.5, 0.5, 0.5);
				//glInterleavedArrays(GL_T2F_N3F_V3F, 0 , pVertex);			
				glDrawElements(GL_TRIANGLES, m_numOfNoMat * 3,
					GL_UNSIGNED_INT,pFaceNoMat);
			}
			
			
			glDisableClientState( GL_VERTEX_ARRAY );
			glClientActiveTexture(GL_TEXTURE0);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glPopMatrix();
			glPopAttrib();
			
			
		}
		
		
	}
	
	
	void vgObject::DateObj(char *data)
	{
		int i;
		long pos = 0;
		

		RenderType = *(int *)(data + pos); //新增~10.8
		
		pos += sizeof(int);

		//////////////////////读入字符串/////////
		
		for (i = 0 ; i < 20; i ++)
		{
			m_name[i] = *(data + pos);
			pos ++; 
		}

		m_numOfFaces = *(long *) (data + pos);	
		pos += sizeof(long);
	/*		CString aa(m_strName);*/
		
		//		m_name = aa;
		
		/////////////////////////材质处理//////////////////////
		
		m_numOfMat = *(int *) (data + pos);
		
		pos += sizeof(int);
		
		pMats = new vgMaterial [m_numOfMat];
		
		for (i = 0; i <m_numOfMat ; i++)
		{
			pMats[i].materialinfo =  *(vgMaterialInfo *)(data + pos);
			
			try
			{
				vgFoundationSystem::TextureManager &mgr = 
					vgFoundationSystem::TextureManager::getSingleton();
				
				
				string aa(pMats[i].materialinfo.strFile);
				
				pMats[i].pTexture = mgr.getTexturePtr(aa);
				
			}
			catch (vgFoundationSystem::Exception& e)
			{
				MessageBox( NULL , e.getFullDescription().c_str() ,"载入", MB_OK);
			}
			
			pos += sizeof(vgMaterialInfo);
		}
		
		
		///////////////////////点处理//////////////////////////
		
		m_numOfVerts = *(int *)(data + pos);
		
		pos += sizeof(int);
		
		pVertex = new vgVertexTNP[m_numOfVerts];
		
		memcpy(pVertex , data+pos , sizeof(vgVertexTNP) * m_numOfVerts);
		
		pos += sizeof(vgVertexTNP) * m_numOfVerts;
		
		// 
		// 	for (i = 0 ; i < m_numOfVerts; i++)
		// 	{
		// 		pVertex[i] = *(vgVertexTNP *)(data + pos);
		// 		
		// 		pos += sizeof(vgVertexTNP);
		// 
		// 	}
		
		
		//////////////////////有材质的面/////////////////////////
		
		pMatRender = new vgObjRenderControl[m_numOfMat];
		
		for (i = 0; i < m_numOfMat ; i ++)
		{
			pos += ReadRenderControl(data+pos , pMatRender[i] );
		}
		
		m_numOfNoMat = *(int *)(data + pos);
		
		pos += sizeof(int);
		
		
		pFaceNoMat = new vgFaceindex[m_numOfNoMat];
		
		memcpy(pFaceNoMat , data+pos , sizeof(vgFaceindex) * m_numOfNoMat);
		
		pos += sizeof(vgFaceindex) * m_numOfNoMat;

		// 三角面法线
		pNormalOfFaces = new Vector3[m_numOfFaces];	
		memcpy(pNormalOfFaces , data+pos , 
		sizeof(Vector3) * m_numOfFaces);	
 		pos += sizeof(Vector3) * m_numOfFaces;

		// 实际点数目	
		m_numOfVertexs = *(long *)(data + pos);	
		pos += sizeof(long);	
	
		// 点坐标数组
		pVert = new Vector3[m_numOfVertexs];	
		memcpy(pVert , data+pos , 
		sizeof(Vector3) * m_numOfVertexs);	
		pos += sizeof(Vector3) * m_numOfVertexs;
		
		// 阴影面所以结构体
		pShadowFaces = new sw_FaceIndex[m_numOfFaces];
		memcpy(pShadowFaces , data+pos , 
		sizeof(sw_FaceIndex) * m_numOfFaces);	
		pos += sizeof(sw_FaceIndex) * m_numOfFaces;
		
		
	}
	
	
	int vgObject::ReadRenderControl(char *data , vgObjRenderControl &RenderControl )
	{
		int datapos = 0;
		
		RenderControl.b_hastexture = *(bool *)(data + datapos);
		
		datapos += sizeof(bool);
		
		RenderControl.m_numOfFace = *(int *)(data + datapos);
		
		datapos += sizeof(int);
		
		RenderControl.pFace = new vgFaceindex[RenderControl.m_numOfFace];
		
		memcpy(RenderControl.pFace , data+datapos , sizeof(vgFaceindex) * RenderControl.m_numOfFace);
		
		datapos += sizeof(vgFaceindex) * RenderControl.m_numOfFace;
		
		return datapos;
		
	}
	
	void vgObject::WriteNodeToFile(CFile &fp)
	{
		int i = -1;
		long newpos;
		
		
		Posinfile = fp.GetPosition();
		
		fp.Write(&TYPE , sizeof(int));	               //写类型type

		fp.Write(&RenderType, sizeof(int));            //渲染类型
		
		fp.Write(&m_id , sizeof(int));                 //写该节点的ID

		fp.Write(m_LayerName , 20);

///////////////////////LOD////////////////////////////////////
// 		if (pParent == NULL)
// 		{
// 			fp.Write(&i, sizeof(int));
// 		}
// 		else
// 			fp.Write(&pParent->m_id, sizeof(int));   //需改动
		
		if (pChild == NULL)
		{
			fp.Write(&i, sizeof(int));
		}
		else
			fp.Write(&pChild->m_id, sizeof(int));

		fp.Write(&nearvis, sizeof(int));

		fp.Write(&farvis , sizeof(int));

////////////////////////////////////////////////////////////

		fp.Write(&m_isVisable, sizeof(bool));
		
		//////////////////////////////////////////////////////
		
		fp.Write(&m_BoundryBox.maxPoint.x , sizeof(float));
		fp.Write(&m_BoundryBox.maxPoint.y , sizeof(float));
		fp.Write(&m_BoundryBox.maxPoint.z , sizeof(float));
		fp.Write(&m_BoundryBox.minPoint.x , sizeof(float));
		fp.Write(&m_BoundryBox.minPoint.y , sizeof(float));
		fp.Write(&m_BoundryBox.minPoint.z , sizeof(float));
		///////////////////////////////////////////////////
		fp.Write(&m_name, sizeof(m_name));
		
		fp.Write(&m_numOfFaces, sizeof(m_numOfFaces));
		
		fp.Write(&m_numOfMat , sizeof(int));
		
		for (i= 0 ; i < m_numOfMat ; i++)
		{
			fp.Write(&pMats[i].materialinfo.aphlatext , sizeof(int));
			fp.Write(&pMats[i].materialinfo.strName , sizeof(pMats[i].materialinfo.strName));
			fp.Write(&pMats[i].materialinfo.strFile , sizeof(pMats[i].materialinfo.strFile));
			fp.Write(&pMats[i].materialinfo.alphaFile, sizeof(pMats[i].materialinfo.alphaFile));
			fp.Write(&pMats[i].materialinfo.color , sizeof(pMats[i].materialinfo.color));
			fp.Write(&pMats[i].materialinfo.texureId, sizeof(pMats[i].materialinfo.texureId));
			pMats[i].fileoffset = fp.GetPosition();
		}
		
		
		
		/////////////////////////以上 写材质，以下写点///////////////////////
		
		fp.Write(&m_numOfVerts, sizeof(int));
		
		fp.Write(pVertex , sizeof(vgVertexTNP) * m_numOfVerts);
		/////////////////////////////
		for (i = 0; i < m_numOfMat; i++)
		{
			fp.Write(&pMatRender[i].b_hastexture, sizeof(bool));
			fp.Write(&pMatRender[i].m_numOfFace, sizeof(int));
			fp.Write(pMatRender[i].pFace, sizeof(vgFaceindex) * pMatRender[i].m_numOfFace);
		}
		/////////////////////
		fp.Write(&m_numOfNoMat , sizeof(int));
		
		fp.Write(pFaceNoMat , sizeof(vgFaceindex) * m_numOfNoMat);
// ///////////////////////
		fp.Write(pNormalOfFaces, sizeof(Vector3) * m_numOfFaces);
	
		fp.Write(&m_numOfVertexs , sizeof(long));           //正方体该值为8
	
		fp.Write(pVert, sizeof(Vector3) * m_numOfVertexs);

		fp.Write(pShadowFaces, sizeof(sw_FaceIndex) * m_numOfFaces);
		
		newpos = fp.GetPosition();
		
		nodelength = newpos - Posinfile;
		
		
	}
	
	unsigned long vgObject::PositDataToNode( char *data )
	{
		int i;
		unsigned long pos = 0;

		RenderType = *(int *)(data + pos);  //新增
		
		pos += sizeof(int);
		
		m_id = *(int *)(data + pos);
		pos += sizeof(int);
		
		for (i = 0 ; i < 20; i ++)
		{
			m_LayerName[i] = *(data + pos);
			pos ++; 
		}
///////////////////////LOD////////////////////////
// 		m_parentID = *(int *)(data + pos);
// 
// 		pos += sizeof(int);

		m_childID = *(int *)(data + pos);

		pos += sizeof(int);

		nearvis = *(int *) (data + pos);

		pos += sizeof(int);

		farvis = *(int *)(data + pos);
		
		pos += sizeof(int);
//////////////////////////////////////////////
		m_isVisable = *(bool *)(data + pos);

		pos += sizeof(bool);

		
		m_BoundryBox.maxPoint.x = *(float *)(data + pos);
		pos += sizeof(float);
		m_BoundryBox.maxPoint.y = *(float *)(data + pos);
		pos += sizeof(float);
		m_BoundryBox.maxPoint.z = *(float *)(data + pos);
		pos += sizeof(float);
		m_BoundryBox.minPoint.x = *(float *)(data + pos);
		pos += sizeof(float);
		m_BoundryBox.minPoint.y = *(float *)(data + pos);
		pos += sizeof(float);
		m_BoundryBox.minPoint.z = *(float *)(data + pos);
		pos += sizeof(float);

		///////////////////////////////////////////////////////////

		UpdatePos();
		
		//////////////////////读入字符串/////////
		
		for (i = 0 ; i < 20; i ++)
		{
			m_name[i] = *(data + pos);
			pos ++; 
		}

		m_numOfFaces = *(long *) (data + pos);	
		pos += sizeof(long);
		
		/////////////////////////材质处理//////////////////////
		
		m_numOfMat = *(int *) (data + pos);
		
		pos += sizeof(int);
		
		pMats = new vgMaterial [m_numOfMat];
		
		for (i = 0; i <m_numOfMat ; i++)
		{
			pMats[i].materialinfo =  *(vgMaterialInfo *)(data + pos);
			
			try
			{
				vgFoundationSystem::TextureManager &mgr = 
					vgFoundationSystem::TextureManager::getSingleton();
				
				
				string aa(pMats[i].materialinfo.strFile);

				//处理aa
				
				pMats[i].pTexture = mgr.getTexturePtr(aa);
				
			}
			catch (vgFoundationSystem::Exception& e)
			{
				MessageBox( NULL , e.getFullDescription().c_str() ,"载入", MB_OK);
			}
			
			pos += sizeof(vgMaterialInfo);
		}
		
		
		///////////////////////点处理//////////////////////////
		
		m_numOfVerts = *(int *)(data + pos);
		
		pos += sizeof(int);
		
		pVertex = new vgVertexTNP[m_numOfVerts];
		
		memcpy(pVertex , data+pos , sizeof(vgVertexTNP) * m_numOfVerts);
		
		pos += sizeof(vgVertexTNP) * m_numOfVerts;
		
		// 
		// 	for (i = 0 ; i < m_numOfVerts; i++)
		// 	{
		// 		pVertex[i] = *(vgVertexTNP *)(data + pos);
		// 		
		// 		pos += sizeof(vgVertexTNP);
		// 
		// 	}
		
		
		//////////////////////有材质的面/////////////////////////
		
		pMatRender = new vgObjRenderControl[m_numOfMat];
		
		for (i = 0; i < m_numOfMat ; i ++)
		{
			pos += ReadRenderControl(data+pos , pMatRender[i] );
		}
		
		m_numOfNoMat = *(int *)(data + pos);
		
		pos += sizeof(int);
		
		
		pFaceNoMat = new vgFaceindex[m_numOfNoMat];
		
		memcpy(pFaceNoMat , data+pos , sizeof(vgFaceindex) * m_numOfNoMat);
		
		pos += sizeof(vgFaceindex) * m_numOfNoMat;

		// 三角面法线
		pNormalOfFaces = new Vector3[m_numOfFaces];	
		memcpy(pNormalOfFaces , data+pos , 
		sizeof(Vector3) * m_numOfFaces);	
 		pos += sizeof(Vector3) * m_numOfFaces;

		// 实际点数目	
		m_numOfVertexs = *(long *)(data + pos);	
		pos += sizeof(long);	
	
		// 点坐标数组
		pVert = new Vector3[m_numOfVertexs];	
		memcpy(pVert , data+pos , 
		sizeof(Vector3) * m_numOfVertexs);	
		pos += sizeof(Vector3) * m_numOfVertexs;
		
		// 阴影面所以结构体
		pShadowFaces = new sw_FaceIndex[m_numOfFaces];
		memcpy(pShadowFaces , data+pos , 
		sizeof(sw_FaceIndex) * m_numOfFaces);	
		pos += sizeof(sw_FaceIndex) * m_numOfFaces;
		
		return pos;
		
	}
	
	void vgObject::WriteDDS(CFile &fp ,const string &filename)
	{
		
	}
	
	void vgObject::Translate( Vector3 offset )
	{
		int i;
		for (i =0 ; i < m_numOfVerts ; i++)
		{
			pVertex[i].positionX += offset.x;
			pVertex[i].positionY += offset.y;
			pVertex[i].positionZ += offset.z;
		}

		for (i = 0; i < m_numOfVertexs; i++)
		{
			pVert[i]  = pVert[i] + offset;
		}
		
		ComputeBoundBox();

		UpdatePos();

	}
	
	void vgObject::Rotate( short axis, float Angle )
	{
		
		int i;
		float centerx,centery,centerz;
		
		centerx = (m_BoundryBox.maxPoint.x + m_BoundryBox.minPoint.x ) / 2.0;
		
		centery = (m_BoundryBox.maxPoint.y + m_BoundryBox.minPoint.y ) / 2.0;
		
		centerz = (m_BoundryBox.maxPoint.z + m_BoundryBox.minPoint.z ) / 2.0;
		
		if(axis == 0)
		{
			for(i =0 ; i < m_numOfVerts ; i++)
			{
				
				RotatePoint(centery, centerz, Angle, &pVertex[i].positionY,&pVertex[i].positionZ);

			}

			for (i = 0; i < m_numOfVertexs; i++)
			{
				RotatePoint(centery, centerz, Angle, &pVert[i].y, &pVert[i].z);
			}

			for (i = 0; i < m_numOfFaces; i++)
			{
				RotatePoint(0,0,Angle,&pNormalOfFaces[i].y, &pNormalOfFaces[i].z);
			}

		}
		if(axis == 1)
		{
			for(i =0 ; i < m_numOfVerts ; i++)
			{
				RotatePoint(centerz, centerx, Angle, &pVertex[i].positionZ,&pVertex[i].positionX);


			}
			for (i = 0; i < m_numOfVertexs; i++)
			{
				RotatePoint(centerz, centerx, Angle, &pVert[i].z, &pVert[i].x);
			}
			for (i = 0; i < m_numOfFaces; i++)
			{
				RotatePoint(0,0,Angle,&pNormalOfFaces[i].z, &pNormalOfFaces[i].x);
			}
		}
		if(axis == 2)
		{ 
			for(i =0 ; i < m_numOfVerts ; i++)
			{
				RotatePoint(centerx, centery, Angle, &pVertex[i].positionX,&pVertex[i].positionY);

			}
			for (i = 0; i < m_numOfVertexs; i++)
			{
				RotatePoint(centerx, centery, Angle, &pVert[i].x, &pVert[i].y);

			}

			for (i = 0; i < m_numOfFaces; i++)
			{
				RotatePoint(0,0,Angle,&pNormalOfFaces[i].x, &pNormalOfFaces[i].y);
			}
		}

		
		ComputeBoundBox();
		UpdatePos();

	}
	
	void vgObject::Scale( Vector3 Scale )
	{
		int i;

		float centerx,centery,centerz,tempx,tempy,tempz;
		
		centerx = (m_BoundryBox.maxPoint.x + m_BoundryBox.minPoint.x ) / 2.0;
		
		centery = (m_BoundryBox.maxPoint.y + m_BoundryBox.minPoint.y ) / 2.0;
		
		centerz = (m_BoundryBox.maxPoint.z + m_BoundryBox.minPoint.z ) / 2.0;
		
		for (i =0 ; i < m_numOfVerts ; i++)
		{
			tempx = pVertex[i].positionX - centerx;
			
			tempx = tempx * Scale.x;
			
			pVertex[i].positionX = centerx + tempx;
			///////////////////////////////////////////////////
			tempy = pVertex[i].positionY - centery;
			
			tempy = tempy * Scale.y;
			
			pVertex[i].positionY = centery + tempy;
			///////////////////////////////////////////////////
			tempz = pVertex[i].positionZ - centerz;
			
			tempz = tempz * Scale.z;
			
			pVertex[i].positionZ = centerz + tempz;
			
		}

		for (i =0 ; i < m_numOfVertexs; i++)
		{
			tempx = pVert[i].x- centerx;
			
			tempx = tempx * Scale.x;
			
			pVert[i].x = centerx + tempx;
			///////////////////////////////////////////////////
			tempy = pVert[i].y - centery;
			
			tempy = tempy * Scale.y;
			
			pVert[i].y = centery + tempy;
			///////////////////////////////////////////////////
			tempz = pVert[i].z - centerz;
			
			tempz = tempz * Scale.z;
			
			pVert[i].z = centerz +tempz;

		}
		
		ComputeBoundBox();
		UpdatePos();

	}
	
	void vgObject::RenderShadows()
	{
		return;
	}

	void vgObject::render_ForShadow()
	{

	//	update_ForShadowEveryTime();
		glCallList( mShadowVolume );

	}

	void vgObject::update_ForShadowEveryTime()
	{
		Vector3 lightDir = vgQuadtreeSceneManager::getSingleton().Scene.m_LightPosForShadow;
	
//		Vector3 lightDir(0.1,-0.5,0.1);

		lightDir.normalise();
		
		
		int iface = 0;
		
		// 首先更新每个normal值
		for (  iface = 0 ; iface < m_numOfFaces ; ++ iface )
		{
			sw_FaceIndex& cur_face = pShadowFaces[ iface ];
			
			assert( cur_face.normal >= 0 );
			Vector3 &curNormal = 
				pNormalOfFaces[ cur_face.normal ];
			
			if ( curNormal.dotProduct( lightDir ) <= 0 )
			{
				cur_face.faceVisible = true;
			}
			else
			{
				// 黑暗时，什么也不投射。
				cur_face.faceVisible = false;
			}
		}
		
		// 更新visible的赋值
		for ( iface = 0 ; iface < m_numOfFaces ; ++ iface )
		{
			sw_FaceIndex& cur_face = pShadowFaces[ iface ];
			
			//检查-1时
			
			if ( cur_face.faceVisible == true )
			{
				if ( cur_face.p1p2AdjFaceIndex != -1 )
				{
					const bool& linevisible =
						pShadowFaces[ cur_face.p1p2AdjFaceIndex ].faceVisible;
					cur_face.p1p2Visible = (linevisible == false ? true : false );
				}
				else
				{
					cur_face.p1p2Visible = true;
				}
				
				if ( cur_face.p2p3AdjFaceIndex != -1 )
				{
					const bool& linevisible =
						pShadowFaces[ cur_face.p2p3AdjFaceIndex ].faceVisible;
					cur_face.p2p3Visible = (linevisible == false ? true : false );
				}
				else
				{
					cur_face.p2p3Visible = true;
				}
				
				if ( cur_face.p3p1AdjFaceIndex != -1 )
				{
					const bool& linevisible =
						pShadowFaces[ cur_face.p3p1AdjFaceIndex ].faceVisible;
					cur_face.p3p1Visible = (linevisible == false ? true : false );
				}
				else
				{
					cur_face.p3p1Visible = true;
				}
				
			} // end of cur_face.faceVisible == true
			
		}
		
		this->buildShadowVolume();		
	}

	void vgObject::buildShadowVolume()
	{
		if( mShadowVolume != -1 )
		{
			glDeleteLists( mShadowVolume , 0 );
		}
		
		mShadowVolume = glGenLists(1);
		
		glNewList( mShadowVolume , GL_COMPILE );
		{
			glDisable( GL_LIGHTING );
			
			
			Vector3 lightDir = vgQuadtreeSceneManager::getSingleton().Scene.m_LightPosForShadow;
			
			//lightDir.x += 0.001f;
			
			lightDir.normalise();
			
			glPushMatrix();
			
// 			float centerx,centery,centerz;
// 			
// 			centerx = (m_BoundryBox.maxPoint.x + m_BoundryBox.minPoint.x ) / 2;
// 			centery = (m_BoundryBox.maxPoint.y + m_BoundryBox.minPoint.y ) / 2;
// 			centerz = (m_BoundryBox.maxPoint.z + m_BoundryBox.minPoint.z ) / 2;
// 			
// 			glTranslatef(centerx , centery , centerz);
// 			glRotatef(m_Rotation.x , 1, 0, 0);
// 			glRotatef(m_Rotation.y , 0, 1, 0);
// 			glRotatef(m_Rotation.z , 0, 0, 1);
// 			glTranslatef(-centerx , -centery , -centerz);
// 			
// 			glTranslatef(centerx , centery , centerz);
// 			glScalef(m_Scale.x , m_Scale.y , m_Scale.z);
// 			glTranslatef(-centerx , -centery , -centerz);
// 			
// 			glTranslatef(m_Position.x , m_Position.y, m_Position.z);
			
			//glDisable( GL_TEXTURE_2D );
			
			
			//////////////////////////////////////////////////////////////////////////
			for ( int iface = 0 ; iface < m_numOfFaces ; ++ iface )
			{
				sw_FaceIndex& cur_face = pShadowFaces[ iface ];
				
				assert( cur_face.p1 >= 0 );
				Vector3 &point1 = pVert[ cur_face.p1 ];
				assert( cur_face.p2 >= 0 );
				Vector3 &point2 = pVert[ cur_face.p2 ];
				assert( cur_face.p3 >= 0 );
				Vector3 &point3 = pVert[ cur_face.p3 ];
				
				
				Vector3 farpoint1;
				farpoint1 = point1 + lightDir * 10000;
				Vector3 farpoint2;
				farpoint2 = point2 + lightDir * 10000;
				Vector3 farpoint3;
				farpoint3 = point3 + lightDir * 10000;
				
				if ( cur_face.faceVisible == true )
				{
					//glBegin( GL_TRIANGLES );
					//{
					//	glVertex3fv( point1.v );
					//	glVertex3fv( point2.v );
					//	glVertex3fv( point3.v );
					//}
					//glEnd();
					
					//glColor4f( 1 , 0.5 , 1 , 0.3f);
					
					//glBegin( GL_LINES);
					glBegin( GL_QUADS );
					{
						if ( cur_face.p1p2Visible )
						{
							glVertex3fv( point1.v );
							glVertex3fv( farpoint1.v );
							glVertex3fv( farpoint2.v );
							glVertex3fv( point2.v );
						}
						if ( cur_face.p2p3Visible )
						{
							
							glVertex3fv( point2.v );
							glVertex3fv( farpoint2.v );
							glVertex3fv( farpoint3.v );
							glVertex3fv( point3.v );
						}
						if ( cur_face.p3p1Visible )
						{
							glVertex3fv( point3.v );
							glVertex3fv( farpoint3.v );
							glVertex3fv( farpoint1.v );
							glVertex3fv( point1.v );
						}
					}
					glEnd();
				}
				
			}
			
			glPopMatrix();
			
		}
		glEndList();
		
		
}


vgBoundryBox vgObject::ComputeBoundBox()
{
	m_BoundryBox.maxPoint.x = m_BoundryBox.minPoint.x = pVertex[0].positionX;
	
	m_BoundryBox.maxPoint.y = m_BoundryBox.minPoint.y = pVertex[0].positionY;
	
	m_BoundryBox.maxPoint.z = m_BoundryBox.minPoint.z = pVertex[0].positionZ;
	
	for (int i =1 ; i < m_numOfVerts ; i++)
	{
		if (pVertex[i].positionX >m_BoundryBox.maxPoint.x)
		{
			m_BoundryBox.maxPoint.x = pVertex[i].positionX;
		}
		
		if (pVertex[i].positionX < m_BoundryBox.minPoint.x)
		{
			m_BoundryBox.minPoint.x = pVertex[i].positionX;
		}
		
		if (pVertex[i].positionY >m_BoundryBox.maxPoint.y)
		{
			m_BoundryBox.maxPoint.y = pVertex[i].positionY;
		}
		
		if (pVertex[i].positionY < m_BoundryBox.minPoint.y)
		{
			m_BoundryBox.minPoint.y = pVertex[i].positionY;
		}
		
		if (pVertex[i].positionZ >m_BoundryBox.maxPoint.z)
		{
			m_BoundryBox.maxPoint.z = pVertex[i].positionZ;
		}
		
		if (pVertex[i].positionZ < m_BoundryBox.minPoint.z)
		{
			m_BoundryBox.minPoint.z = pVertex[i].positionZ;
		}
		
	}
	
	return m_BoundryBox;
	
}

void vgObject::RenderWithoutTex( float r, float g, float b )
{
	glPushMatrix();

	glDisable(GL_TEXTURE_2D);
	
	for (int i = 0; i < m_numOfMat; i++)
	{
		if (pMatRender[i].b_hastexture == true)
		{
			
			// 			glEnable(GL_ALPHA_TEST);
			// 				
			// 			glAlphaFunc(GL_GREATER, 0);
			
			glColor3d(r, g, b);		
			
			// 			glEnable(GL_TEXTURE_2D);
			// 				
			// 				//glBindTexture(GL_TEXTURE_2D, pObject->pMats[i].texureId);
			// 				
			// 			pMats[i].pTexture->useBind();
			
			
			glInterleavedArrays(GL_T2F_N3F_V3F, 0 , pVertex);
			
			glDrawElements(GL_TRIANGLES, pMatRender[i].m_numOfFace * 3,
				GL_UNSIGNED_INT,pMatRender[i].pFace);
			
			//			glDisable(GL_TEXTURE_2D);
		}
		else
		{
			/*			glDisable(GL_TEXTURE_2D);*/
			
			glColor3f (r ,g, b);
			
			glInterleavedArrays(GL_T2F_N3F_V3F, 0 , pVertex);
			
			glDrawElements(GL_TRIANGLES, pMatRender[i].m_numOfFace * 3,
				GL_UNSIGNED_INT,pMatRender[i].pFace);
			
		}	
	}
	if (m_numOfNoMat != 0)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(r, g, b);
		glInterleavedArrays(GL_T2F_N3F_V3F, 0 , pVertex);			
		glDrawElements(GL_TRIANGLES, m_numOfNoMat * 3,
			GL_UNSIGNED_INT,pFaceNoMat);
	}
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
	
}
}