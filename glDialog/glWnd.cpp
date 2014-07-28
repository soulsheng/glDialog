// glWnd.cpp : 实现文件
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



// glWnd 消息处理程序




int glWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}


void glWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
}
