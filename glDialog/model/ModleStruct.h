#ifndef _MODLESTRUCT_H_
#define _MODLESTRUCT_H_

#include "tvgTexture.h"

class TexturePtr;
///obj材质中最基本的信息，若需要支持材质球中的调整，需重新加入一些成员
struct vgMaterialInfo
{
	int aphlatext;
	char  strName[255];			// The texture name
	char  strFile[255];			// The texture file name (If this is set it's a texture map)
//	char  *texturedata;
	char  alphaFile[255];
	unsigned char  color[3];				// The color of the object (R, G, B)
	unsigned int   texureId;				// the texture ID
//	vgFoundationSystem::TexturePtr  pTexture;	

//  	int b_needread;
//  	long materialoffset;
//  	long materialsize;
} ;
struct vgMaterialData 
{
	int width;
	int length;
	long size;
	char *data;
};
struct vgMaterial
{
	vgMaterialInfo materialinfo;

	long fileoffset;

	vgFoundationSystem::TexturePtr pTexture;
};

///交错数组中用到的点的结构体
struct vgVertexTNP
{
	
public:
	
	float texCoorS;
	
	float texCoorT;
	
	float normalX;
	
	float normalY;
	
	float normalZ;
	
	float positionX;
	
	float positionY;
	
	float positionZ;
	
};

struct vgFaceindex
{
	int x;
	
	int y;
	
	int z;
	
};

struct vgObjRenderControl 
{
	bool b_hastexture;

	int m_numOfFace;
	
	vgFaceindex *pFace;

};

struct sw_FaceIndex
{
	int	p1;
	int p2;
	int p3;
	
	int	normal;
	
	int	p1p2AdjFaceIndex;
	int	p2p3AdjFaceIndex;
	int	p3p1AdjFaceIndex;
	
	bool faceVisible;
	
	bool p1p2Visible;
	bool p2p3Visible;
	bool p3p1Visible;	
};


///绘制模型存放数据位置<---------应该加入一个模型名称---------->
// struct vgModel
// {
// 	int m_numOfObj;
// 
// 	vgObjectStruct *pObj;
// 
// 	long *ObjPosIndex;
// };

#endif