#pragma once

#include <gl/GL.h>
#include "vgPrerequisites.h"

#include "vgObjNode.h"

using namespace vgFoundationSystem;

// glWnd

class glWnd : public CWnd
{
	DECLARE_DYNAMIC(glWnd)

public:
	glWnd();
	virtual ~glWnd();

	// add member function and variable
	void LoadModel( std::string filename );
	void AddObject( vgObject* pObject );
	void renderObject( );
	void cleanup();

	int MySetPixelFormat(HDC hdc);
	void DrawColorBox(void);
	HDC hdc;
	HGLRC hglrc;
	GLfloat step,s;


	float	g_eye[3];		//	视点位置
	float	g_look[3];		//	焦点位置
	float	g_up[3];		//	头顶法向

	std::vector<vgObject*>	m_objects;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};


