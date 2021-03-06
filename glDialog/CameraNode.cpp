#include "stdafx.h"
#include "CameraNode.h"
#include <fstream>

#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define CAMERA_CONFIG_ONCE	1
#define PI	3.1415927

cameranode::cameranode()
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
	g_speed = 2.0f;


#if CAMERA_CONFIG_ONCE
	// set camera property by configuration file
	std::ifstream file;
	file.open("camera.txt");
	file	>> g_eye[0] >> g_eye[1] >> g_eye[2]
			>> g_Angle >> g_dir[1] ;
	file.close();
#endif

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
	
	if (KEY_DOWN(VK_PRIOR) && (g_dir[1] <= 100))	g_dir[1] += 0.02f;//抬头
	if (KEY_DOWN(VK_NEXT) && (g_dir[1] >= -100))	g_dir[1] -= 0.02f;//低头

	g_dir[0] = cos(PI*g_Angle/180.0f);
	g_dir[2] = sin(PI*g_Angle/180.0f);

	gluLookAt(
		g_eye[0],	g_eye[1],	g_eye[2],
		g_eye[0] + g_dir[0],	g_eye[1] + g_dir[1],	g_eye[2] + g_dir[2],
		g_up[0],	g_up[1],	g_up[2]);

}