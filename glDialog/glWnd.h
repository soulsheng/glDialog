#pragma once

#include <gl/GL.h>

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

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};


