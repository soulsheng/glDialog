
#pragma once


struct cameranode
{
	float	g_eye[3];		//	�ӵ�λ��
	float	g_dir[3];		//	���߷���
	float	g_up[3];		//	ͷ������
	cameranode();
	void	look();
	void	reset();
	float		g_Angle;		//����ת
	float		g_speed;		//�ٶ�
	const float		g_acc;			//���ٶ�
};

