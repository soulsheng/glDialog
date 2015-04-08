
#pragma once


struct cameranode
{
	float	g_eye[3];		//	视点位置
	float	g_dir[3];		//	视线方向
	float	g_up[3];		//	头顶法向
	cameranode();
	void	look();
	void	reset();
	float		g_Angle;		//左右转
	float		g_speed;		//速度
	const float		g_acc;			//加速度
};

