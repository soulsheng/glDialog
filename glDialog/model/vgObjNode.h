// vgObject.h: interface for the vgObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_vgObject_H__D57F5D8E_1928_408B_BF2D_1A6F4C98EA9A__INCLUDED_)
#define AFX_vgObject_H__D57F5D8E_1928_408B_BF2D_1A6F4C98EA9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ModleStruct.h"
#include "vgNode.h"
#include "vgBoundryBox.h"
namespace vgFoundationSystem{
	
	class vgObject : public vgNode  
	{
	public:
		vgObject();
		virtual ~vgObject();
		
	public:

		void RenderWithoutTex(float r, float g, float b);
		void buildShadowVolume();
		void update_ForShadowEveryTime();
		void render_ForShadow();
		void WriteDDS(CFile &fp , const string &filename);
		unsigned long PositDataToNode(char *data);
		void WriteNodeToFile(CFile &fp);
		
		void DateObj(char *data);
		
		int ReadRenderControl(char *data , vgObjRenderControl &RenderControl );
		
//		char m_strName[255];
		
		/*	int m_numOfVerts;*/
		
		vgVertexTNP *pVertex;
		
		int m_numOfNoMat;
		
		/////////////////////////node 类型
		
		vgFaceindex *pFaceNoMat;
		
		vgObjRenderControl *pMatRender;	
		
		void Render();

		void RenderShadows();
		
		void Translate(Vector3 offset);
		
		void Rotate(short axis, float Angle);
		
		void Scale(Vector3 Scale);
		
		int m_ParentID;
		///////////////////
		bool IsLOD;
		
		bool bHasChild;
		bool bIsChild;
		
		vgObject *pChild;
		vgObject *pParent;
		
 		int m_childID;
//  	int m_parentID;
		
		int nearvis;
		int farvis;		
		int id;
		int  m_leval; 
		/////////////////////新增//////////
		Vector3		*pNormalOfFaces;


		long	m_numOfVertexs;
		Vector3		*pVert;

		sw_FaceIndex	*pShadowFaces;
		int		mShadowVolume;

		
	private:
		vgBoundryBox ComputeBoundBox();

	};
}
#endif // !defined(AFX_vgObject_H__D57F5D8E_1928_408B_BF2D_1A6F4C98EA9A__INCLUDED_)
