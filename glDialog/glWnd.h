#pragma once

#include <gl/GL.h>
#include "vgPrerequisites.h"

#include "vgObjNode.h"

#include "tvgArchiveManager.h"
#include "tvgTextureManager.h"

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
	void initialize();

	int MySetPixelFormat(HDC hdc);
	void DrawColorBox(void);
	HDC hdc;
	HGLRC hglrc;
	GLfloat step,s;


	float	g_eye[3];		//	�ӵ�λ��
	float	g_look[3];		//	����λ��
	float	g_up[3];		//	ͷ������

	std::vector<vgObject*>	m_objects;

	TextureManager	*m_pTextureManager;
	ArchiveManager *m_pArchiveManger;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();

	afx_msg void OnSize(UINT nType, int cx, int cy);
};


