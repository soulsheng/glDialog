
// glDialogDlg.h : ͷ�ļ�
//

#pragma once

#include "glWnd.h"

// CglDialogDlg �Ի���
class CglDialogDlg : public CDialogEx
{
// ����
public:
	CglDialogDlg(CWnd* pParent = NULL);	// ��׼���캯��

	glWnd	m_Display;

// �Ի�������
	enum { IDD = IDD_GLDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadModel();
};
