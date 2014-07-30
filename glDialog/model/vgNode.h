// vgNode.h: interface for the vgNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VGNODE_H__C2A6D981_7061_4DD7_88D2_63377B0DF3CB__INCLUDED_)
#define AFX_VGNODE_H__C2A6D981_7061_4DD7_88D2_63377B0DF3CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vgBoundryBox.h"
#include "ModleStruct.h"
//using namespace vgFoundationSystem;
namespace vgFoundationSystem{
	
	class vgNodeAbstract : public CObject
	{
	public:
		int          m_id;
		unsigned int m_offset;
		unsigned int m_size;
		vgBoundryBox   m_BoundryBox;
		bool         m_isInMemory;        /*�Ƿ��Ѵ����ڴ���*/
		int          m_LayerId;
		int          m_Type;
		int          m_numofverts;
		TCHAR        m_name[20];
//////////////UI////////////////////////
		HTREEITEM    hitem;
		
	public:
		vgNodeAbstract(){};
		vgNodeAbstract(int id, vgBoundryBox BBox, bool isInMemory, int layerId, int Type, int facenum,TCHAR name[20])
		{
			m_id = id;
			m_BoundryBox = BBox;
			m_isInMemory = isInMemory;
			m_LayerId = layerId;
			m_Type = Type;
			for (int i = 0 ; i < 20; i++)
			{
				m_name[i] = name[i];
			}

		};
		void WriteToFile(CFile &fp);
		
	};
	
	class vgNode
	{
	public:
		int        m_id;
		int        m_propid;            /*����id��ΪLOD����*/
		TCHAR      m_name[20];
		TCHAR      m_LayerName[20];             /*�����Ĳ�*/
		
		bool       m_isSelected;        /*��ѡ��״̬*/
		bool       m_isCollideCheck;    /*��ײ���״̬*/
		bool       m_isVisable;         /*�Ƿ�ɼ�*/
		bool       m_isDeleted;         /*�Ƿ���ɾ�����*/
		
		vgBoundryBox m_BoundryBox;
		Vector3     m_Pivot;
	public:
		Vector3     m_prePositon;

		Vector3     m_Position;          /*position,rotation,scale������gltranslate()�Ⱥ�������*/

		Vector3     m_PreRotation;		
		Vector3     m_Rotation;          /*����������ʾ������¼������redo,undo��*/
		
		Vector3     m_PreScale;
		Vector3     m_Scale;
		//	vector     m_Size;
		//enum Axis{X, Y, Z};
		//enum Axis RotateAxis;
		short m_numOfMat;
		vgMaterial *pMats;
		long     nodelength;
		
		long     Posinfile;
		//////////////////////////////////////////////////////////////////////////
		long	m_numOfFaces;

		int m_numOfVerts;

		int TYPE;
		////////////
		int RenderType;//0����һ�����Ⱦ���У�1����blend���У�2����depth����
	public:
		//strcpy
		void       SetId(int idValue)                   {m_id = idValue;}
		
		int        GetId()                              {return m_id;}
		
		void       SetName(TCHAR nameString[20])        {}//{wcscpy(m_name, nameString)/*m_name = nameString*/;}//����һ��֧�ֵ�stingת��
		
		TCHAR*     GetName()                            {return m_name;}
		
		void       SetLayer(TCHAR layerName[20])        {}//{wcscpy(m_LayerName, layerName);}
		
		TCHAR*     GetLayer()                           {return m_LayerName;}
		
		void       SetSelected(bool isSelected)         {m_isSelected = isSelected;}
		
		bool       GetSelected()                        {return m_isSelected;}
		
		void       SetCollideCheck(bool isCollideCheck) {m_isCollideCheck = isCollideCheck;}
		
		bool       GetCollideCheck()                    {return m_isCollideCheck;} 
		
		//	void       SetInVisual(bool isInvisual)         {m_isInVisual = isInvisual;}
		
		//	bool       GetInVisual()                        {return m_isInVisual;}
		
		void       SetVisable(bool isVisable)           {m_isVisable = isVisable;}
		
		bool       GetVisable()                         {return m_isVisable;}
		
		void       SetDeleted(bool isDeleted)           {m_isDeleted = isDeleted;}
		
		bool       GetDeleted()                         {return m_isDeleted;}
		
		vgBoundryBox GetBoundryBox()                      {return m_BoundryBox;}
		
		Vector3    GetPivot()      {return m_Pivot;}
		
		void       SetPosition(Vector3 position)         {m_Position = position;}
		
		Vector3    GetPosition()   {return m_Position;}
		
		void       SetRotation(Vector3 rotation)         {m_Rotation = rotation;}
		
		Vector3    GetRotation()   {return m_Rotation;}
		
		void       SetScale(Vector3 scale)               {m_Scale = scale;}
		
		Vector3     GetScale()      {return m_Scale;}
		/////////////////////////////////////////////////////////////////////////////////////////
		
		
		//void RotatePoint(float baseU , float baseV , float angle , float *pointU , float *pointV);
		
		//void RotatePoint(float,float,float,float,float);
	public:
		virtual void Init();
		
		virtual void Render();
		
		virtual void Terminate();	
		
		virtual void RenderShadows();

		virtual vgBoundryBox ComputeBoundBox();

		virtual void RenderWithoutTex(float r, float g, float b);

		
	public:
		void RotatePoint(float baseU , float baseV , float angle , float *pointU , float *pointV);
		//Χ��һ����ת��һ�㡣����Ϊ�ο��㣬��ת�Ƕȣ�Ҫ��ת�ĵ�
		
		double GetAngle(float baseU , float baseV , float pointU , float pointV);
		
		double CallDisp(float U1, float V1, float U2, float V2);
		
	public:
		virtual void Translate(Vector3 offset);
		
		virtual void Rotate(short axis, float Angle);
		
		virtual void Scale(Vector3 Scale);
	public:
		void UpdatePos();

		virtual void update_ForShadowEveryTime();

		virtual void render_ForShadow();
		
		virtual void WriteNodeToFile(CFile &fp);
		
		virtual	unsigned long PositDataToNode(char *data , CFile &fp);
		
		vgNode();
		virtual ~vgNode();//����Ϊ�麯���������Ժ�����ڴ�й©
		
};
}
#endif // !defined(AFX_VGNODE_H__C2A6D981_7061_4DD7_88D2_63377B0DF3CB__INCLUDED_)
