#pragma once

#include <gl/GL.h>
#include "oceanFFT/COceanSim.h"

// glWnd

class glWnd : public CWnd
{
	DECLARE_DYNAMIC(glWnd)

public:
	glWnd();
	virtual ~glWnd();

	// add member function and variable
	int MySetPixelFormat(HDC hdc);
	void DrawColorBox(void);
	HDC hdc;
	HGLRC hglrc;
	GLfloat step,s;

	COceanSim	m_ocean;

	float	g_eye[3];		//	�ӵ�λ��
	float	g_look[3];		//	����λ��
	float	g_up[3];		//	ͷ������

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};


