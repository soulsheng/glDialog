
#ifndef CAMERANODE_H
#define CAMERANODE_H

struct cameranode
{
	float	g_eye[3];		//	�ӵ�λ��
	float	g_dir[3];		//	���߷���
	float	g_up[3];		//	ͷ������
	cameranode();
	void	look();

	float		g_Angle;		//����ת
	float		g_speed;		//�ٶ�
};


#endif // CAMERANODE_H