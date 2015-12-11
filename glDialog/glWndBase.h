#pragma once

#include <gl/GL.h>
#include "CameraNode.h"
#include "glFrameBuffer.h"

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
	FrameBuffer *m_pFrameBuffer;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();

	void renderAll();
	void screenShot();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


