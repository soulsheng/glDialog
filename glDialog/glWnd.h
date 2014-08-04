#pragma once

#include <gl/GL.h>
#include "CameraNode.h"

// glWnd

class glWnd : public CWnd
{
	DECLARE_DYNAMIC(glWnd)

public:
	glWnd();
	virtual ~glWnd();

	// add member function and variable
	void cleanup();
	void initialize();
	int MySetPixelFormat(HDC hdc);
	void DrawColorBox(void);
	HDC hdc;
	HGLRC hglrc;
	GLfloat step,s;

	cameranode		m_camera;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


