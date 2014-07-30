// vgNode.cpp: implementation of the vgNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "FrameWork.h"
#include "vgNode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace vgFoundationSystem
{
	
	
	void vgNodeAbstract::WriteToFile(CFile &fp)
	{
		
		fp.Write(&m_id, sizeof(int));
		
		fp.Write(&m_BoundryBox.maxPoint.x , sizeof(float));
		fp.Write(&m_BoundryBox.maxPoint.y , sizeof(float));
		fp.Write(&m_BoundryBox.maxPoint.z , sizeof(float));
		fp.Write(&m_BoundryBox.minPoint.x , sizeof(float));
		fp.Write(&m_BoundryBox.minPoint.y , sizeof(float));
		fp.Write(&m_BoundryBox.minPoint.z , sizeof(float));
		
		fp.Write(&m_LayerId , sizeof(int));
		
		fp.Write(&m_Type , sizeof(int));
		
		fp.Write(&m_numofverts , sizeof(int));


		fp.Write(&m_name, 20);
		
		
	}
	vgNode::vgNode()
	{
		this->m_id=-1;
		this->m_propid=-1;
		this->m_isDeleted=false;
		this->m_isSelected=false;
		this->m_isVisable=true;
		//wcscpy(this->m_LayerName, _T(""));
		//wcscpy(this->m_name, _T(""));
		strcpy(this->m_LayerName, (""));

		
		strcpy(this->m_name, (""));
		
		m_Scale.reset(1.0, 1.0, 1.0);
		m_Rotation.reset(0.0, 0.0, 0.0);

		RenderType = 0;
		
	}
	
	vgNode::~vgNode()
	{
		
	}
	
	void vgNode::RotatePoint(float baseU, float baseV, float angle, float *pointU, float *pointV)
	{
		float U1; 
		float V1;
		U1=*pointU;
		V1=*pointV;
		double angle1=GetAngle(baseU, baseV, U1, V1);
		double disp=CallDisp(baseU, baseV, U1, V1);
		U1=baseU + disp * cos(angle + angle1);
		V1=baseV + disp * sin(angle + angle1);
		*pointU=U1;
		*pointV=V1;
	}
	
	void vgNode::Render()
	{
		
	}
	
	void vgNode::Rotate(short axis, float Angle)
	{
		//    分别写出绕各个轴旋转，0-x，1-y，2-z
		//    if(axis == 0)
		//    {
		//      for(each Vertex in Node)
		//用GetPivot函数得到旋转的基点
		//        RotatePoint(GetPivot.y, getPivot.z, angle Vertex.y,vertex.z);
		//    }
		//    if(axis == 1)
		//      {
		//        for(each Vertex in Node)
		//        RotatePoint(GetPivot.z, getPivot.x, angle Vertex.z,vertex.x)
		//       }
		//	  if(axis == 2)
		//     { 
		//        for(each Vertex in Node)
		//        RotatePoint(GetPivot.x, getPivot.y, angle Vertex.x,vertex.y)
		//      }
		//     
		//}
	}
	
	void vgNode::Init()
	{
		
	}
	
	void vgNode::Translate(Vector3 offset)
	{
		// 	for(each Vertex in Node)
		// 	{
		// 		Vertex.x += offset.X
		// 		Vertex.y += offset.Y
		// 		Vertex.z += offset.Z
		// 	}
		
		
	}
	
	void vgNode::Scale(Vector3 scale)
	{
		// 	Vector v;
		// 	for(each Vertex in Node)
		// 	//用GetPivot函数得到缩放的基点
		// 	v=Vertex-GetPivot();
		// 	v*scale;
		// 	Vertex=GetPivot()+v;
		
	}
	
	void vgNode::Terminate()
	{
		
	}
	
	double vgNode::GetAngle( float baseU , float baseV , float pointU , float pointV )
	{
		double r, angl;
		//如果基点和计算点是同一个点
		if(fabs(baseU-pointU)<=0.001 && fabs(baseV-pointV)<=0.001)
			return 0;
		if(fabs(baseU-pointU)<=0.001)   //如果两个点在一个竖直的直线上
		{
			if(pointV>baseV)
				angl=(double)(PI/2);
			else
				angl=(double)(PI*3/2);
			return angl;
			
		}
		else
		{
			//得到两点距离
			r=(double)sqrt((pointU-baseU)*(pointU-baseU) + (pointV-baseV)*(pointV-baseV));
			double kk=(pointU-baseU)/r;
			angl=(double)acos(kk);
			if(pointV<baseV)
				angl=(double)(PI*2)-angl;
			return angl;
		}
	}
	
	double vgNode::CallDisp( float U1, float V1, float U2, float V2 )
	{
		return (double)sqrt((U1-U2)*(U1-U2) + (V1-V2)*(V1-V2));	
	}
	
	unsigned long vgNode::PositDataToNode(char *data , CFile &fp)
	{
		
		return 0 ;
	}
	
	void vgNode::WriteNodeToFile(CFile &fp)
	{
		return;
	}
	
	void vgNode::RenderShadows()
	{
		return;
	}


	void vgNode::render_ForShadow()
	{
		return;
	}

	void vgNode::update_ForShadowEveryTime()
	{	
		return;
	}
	
	vgBoundryBox vgNode::ComputeBoundBox()
	{
		return m_BoundryBox;		
	}
	

////////////////////////m_position 更新//////////////
	void vgNode::UpdatePos()
	{
		
		m_Position.x = (m_BoundryBox.maxPoint.x + m_BoundryBox.minPoint.x) / 2;

		m_Position.y = (m_BoundryBox.maxPoint.y + m_BoundryBox.minPoint.y) / 2;

		m_Position.z = (m_BoundryBox.maxPoint.z + m_BoundryBox.minPoint.z) / 2;
		
		m_prePositon = m_Position;

		
	}

	void vgNode::RenderWithoutTex(float r, float g, float b)
	{
		return;
	}
}