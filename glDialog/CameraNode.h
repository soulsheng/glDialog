
#pragma once

#include "BoundingBox.h"

struct cameranode
{
	float	g_eye[3];		//	�ӵ�λ��
	float	g_dir[3];		//	���߷���
	float	g_up[3];		//	ͷ������
	cameranode();
	void	look();
	void	reset();
	void	setSpeed(float speed);
	float		g_Angle;		//����ת
	float		g_speed;		//�ٶ�
	const float		g_acc;			//���ٶ�

	void OnMouseMove(int nFlags, int positionX, int positionY );
	void OnMouseWheel( short zDelta );

	BoundingBox*	m_pBox;
	void	setBox( BoundingBox* pBox )
	{
		m_pBox = pBox;
	}
	void lookBox();
	void enableLookBox();
	bool bLookBox;
};

