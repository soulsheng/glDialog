#pragma once

#include <gl/GL.h>
#include "vgPrerequisites.h"

#include "vgObjNode.h"

#include "tvgArchiveManager.h"
#include "tvgTextureManager.h"

#include "glScene.h"

using namespace vgFoundationSystem;

// glWnd

class glWnd : public CWnd
{
	DECLARE_DYNAMIC(glWnd)

public:
	glWnd();
	virtual ~glWnd();

	// add member function and variable
	void cachePath( std::string filename );
	void OpenIOI( std::string filename );
	void LoadModel( std::string filename );
	void cleanup();
	void initialize();

	int MySetPixelFormat(HDC hdc);
	void DrawColorBox(void);
	HDC hdc;
	HGLRC hglrc;
	GLfloat step,s;


	float	g_eye[3];		//	视点位置
	float	g_dir[3];		//	视线方向
	float	g_up[3];		//	头顶法向

	TextureManager	*m_pTextureManager;
	ArchiveManager *m_pArchiveManger;

	glScene	m_scene;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();

	afx_msg void OnSize(UINT nType, int cx, int cy);
};


