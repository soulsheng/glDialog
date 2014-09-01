#pragma once

#include <gl/GL.h>
#include "CameraNode.h"

// glWnd

class glWndBase : public CWnd
{
	DECLARE_DYNAMIC(glWndBase)

public:
	glWndBase();
	virtual ~glWndBase();

	virtual	void initialize()	= 0;
	virtual	void render()		= 0;

	// add member function and variable
	void cleanup();
	void initializeGL();

	int MySetPixelFormat(HDC hdc);
	void DrawColorBox(void);
	HDC hdc;
	HGLRC hglrc;
	GLfloat step,s;

	cameranode	m_camera;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();

	afx_msg void OnSize(UINT nType, int cx, int cy);
};


