
// glDialogDlg.h : 头文件
//

#pragma once

#include "glWnd.h"

// CglDialogDlg 对话框
class CglDialogDlg : public CDialogEx
{
// 构造
public:
	CglDialogDlg(CWnd* pParent = NULL);	// 标准构造函数

	glWnd	m_Display;

// 对话框数据
	enum { IDD = IDD_GLDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	std::string getFileFromDialog( std::string ext );	// 获取文件路径

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadModel();
};
