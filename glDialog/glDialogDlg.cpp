
// glDialogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "glDialog.h"
#include "glDialogDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CglDialogDlg �Ի���




CglDialogDlg::CglDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CglDialogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CglDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CglDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LoadModel, &CglDialogDlg::OnBnClickedLoadModel)
	ON_BN_CLICKED(IDC_BTN_OPENIOI, &CglDialogDlg::OnBnClickedBtnOpenIOI)
END_MESSAGE_MAP()


// CglDialogDlg ��Ϣ�������

BOOL CglDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_Display.Create( NULL,   //ȱʡ�Ĵ���
		NULL,   //�޴�������
		WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE,
		// ���崰�ڷ��
		CRect(0, 0, 256, 256),   // ���ڵĴ�С
		this,   // ָ����ǰ�Ի���Ϊ�丸����ָ��
		0); 

	m_Display.initialize();

	m_Display.OpenIOI( "./0411.ioi" ) ;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CglDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CglDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CglDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


std::string CglDialogDlg::getFileFromDialog( std::string ext )
{
	std::string DefExt;

	std::string filepath;


	DefExt = ext + "�ļ�(*." + ext + ")|*." + ext + "|)";
	// DefExt.Format("%s","mod�ļ�(*.mod)|*.mod|");

	///CFileDialog��һ��������Ϊtrue��Ϊ���ļ��Ի���,�öλ���mfc
	CFileDialog dlgmodelFile(true,
		ext.c_str(),NULL,
		OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_NONETWORKBUTTON,
		DefExt.c_str() );

	dlgmodelFile.m_ofn.lpstrTitle="��";

	if(dlgmodelFile.DoModal()==IDOK)
	{
		filepath = dlgmodelFile.GetPathName();
	}

	return filepath;
}
void CglDialogDlg::OnBnClickedLoadModel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	std::string filepath = getFileFromDialog( "mod" );

	if (filepath.empty())
	{
		return;
	}

	m_Display.LoadModel( filepath ) ;
}


void CglDialogDlg::OnBnClickedBtnOpenIOI()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	std::string filepath = getFileFromDialog( "ioi" );

	if (filepath.empty())
	{
		return;
	}

	m_Display.OpenIOI( filepath ) ;
}
