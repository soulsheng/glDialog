// glWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "glDialog.h"
#include "glWnd.h"


// glWnd

IMPLEMENT_DYNAMIC(glWnd, CWnd)

glWnd::glWnd()
{

}

glWnd::~glWnd()
{
}


BEGIN_MESSAGE_MAP(glWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// glWnd ��Ϣ�������




int glWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}


void glWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
}
