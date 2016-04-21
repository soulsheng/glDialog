#include "stdafx.h"
#include "CameraNode.h"
#include <fstream>
#include <iostream>

#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define CAMERA_CONFIG_ONCE	1
#define PI	3.1415927

cameranode::cameranode()
	:g_acc(1.01f)
{
	// 初始位置和视线
	reset();
}

void cameranode::look()
{
	if( bLookBox )
	{
		lookBox();
		return;
	}

	if (KEY_DOWN(VK_SHIFT) && (g_speed <= 10000))    g_speed   =g_speed*g_acc;//按SHIFT时的加速
	if (KEY_DOWN(VK_CONTROL))  g_speed   =g_speed/g_acc;//按CONTROL时的减速
	if (KEY_DOWN(VK_LEFT))     g_Angle-=g_speed;//左转
	if (KEY_DOWN(VK_RIGHT))    g_Angle+=g_speed;//右转

	if (KEY_DOWN(VK_TAB))		reset();

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
	
	if (KEY_DOWN(VK_PRIOR) && (g_dir[1] <= 100))	g_dir[1] += g_speed*0.005f;//抬头
	if (KEY_DOWN(VK_NEXT) && (g_dir[1] >= -100))	g_dir[1] -= g_speed*0.005f;//低头

	g_dir[0] = cos(PI*g_Angle/180.0f);
	g_dir[2] = sin(PI*g_Angle/180.0f);

	gluLookAt(
		g_eye[0],	g_eye[1],	g_eye[2],
		g_eye[0] + g_dir[0],	g_eye[1] + g_dir[1],	g_eye[2] + g_dir[2],
		g_up[0],	g_up[1],	g_up[2]);

	if( KEY_DOWN('P') )
	{
		std::ofstream file;
		file.open("cameraPointCloud.txt");
		if (!file)
		{
			std::cout << "can not open file: cameraPointCloud.txt" << std::endl;
		}
		file << g_eye[0] << std::endl << g_eye[1]<< std::endl << g_eye[2]<< std::endl
			 << g_Angle<< std::endl << g_dir[1] ;
		file.close();
	}

	if( KEY_DOWN('B') )
	{
		bLookBox = true;
	}
}

void cameranode::lookBox()
{
	Point3* pPoints = m_pBox->getPoints() ;

	Point3& pMin = pPoints[0];
	Point3& pMax = pPoints[1];

	Point3& pMid = ( pMax + pMin ) * 0.5f;
	Point3& pSize = pMax - pMin;

	static int nAngle = 0;
	if ( bLookBox && nAngle < 720 )
	{
		nAngle ++;

		gluLookAt(
			pMid.x + pSize.x*cos(PI*nAngle/360.0f),	fabsf(pMax.y) * 4,	pMid.z + pSize.x*sin(PI*nAngle/360.0f),
			pMid.x,	pMid.y,	pMid.z,
			g_up[0],	g_up[1],	g_up[2]);

	}
	else
	{
		bLookBox = false;
		nAngle = 0;
	}
}

void cameranode::reset()
{
	//	视点位置
	g_eye[0] = 0.0f;	
	g_eye[1] = 0.0f;	
	g_eye[2] = 10.0f;	

	//	视线方向
	g_dir[0] = 0.0f;	
	g_dir[1] = 0.0f;	
	g_dir[2] = -1.0f;	// 默认朝屏幕里面看

	//	头顶法向
	g_up[0] = 0.0f;		
	g_up[1] = 1.0f;		// 头顶向上，不歪
	g_up[2] = 0.0f;	

	g_Angle = -90.0f;
	g_speed = 0.2f;

	bLookBox = false;

#if CAMERA_CONFIG_ONCE
	// set camera property by configuration file
	std::ifstream file;
	file.open("cameraPointCloud.txt");
	file	>> g_eye[0] >> g_eye[1] >> g_eye[2]
	>> g_Angle >> g_dir[1] ;
	file.close();
#endif
}

void cameranode::setSpeed( float speed )
{
	g_speed = speed;
}

void cameranode::OnMouseMove( int nFlags, int positionX, int positionY )
{
	static int s_lastPostionX;
	static int s_lastPostionY;
	static bool first = true;

	if ( !first && (nFlags & MK_LBUTTON) )
	{
		float offsetx = positionX - s_lastPostionX; 
		float offsety = positionY - s_lastPostionY;

		if ( fabs(offsetx) > fabs(offsety) && fabs(offsetx) != 0 )
		{
			//changeHeading(offsetx);
			g_Angle -= g_speed * offsetx * 5;
		}
		else if ( offsety != 0)
		{
			//changePitch(offsety);
			g_dir[1] += g_speed * offsety ;
		}
	}
	else if ( !first && (nFlags & MK_RBUTTON) )
	{
		float offsetx = positionX - s_lastPostionX; 
		float offsety = positionY - s_lastPostionY;

		if ( offsety != 0)
		{
			g_eye[0]+=g_dir[0]*g_speed * offsety * 5;
			g_eye[2]+=g_dir[2]*g_speed * offsety * 5;
		}
	}


	first = false;
	s_lastPostionX = positionX;
	s_lastPostionY = positionY;
}

void cameranode::OnMouseWheel( short zDelta )
{
	g_eye[0]+=g_dir[0]*g_speed * zDelta * 5;
	g_eye[2]+=g_dir[2]*g_speed * zDelta * 5;
}

void cameranode::enableLookBox()
{
	bLookBox = true;
}
