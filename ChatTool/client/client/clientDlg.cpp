// clientDlg.cpp: 实现文件

#include "stdafx.h"
#include "RecFrame.h"
#include "client.h"
#include "clientDlg.h"
#include "afxdialogex.h"
#include "cSendmessage.h"
#include "Register.h"
#include "send.h"
#include "commDef.h"
#include "mainBoard.h"
#include "savemessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern string g_mode;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
CString username;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	CIPAddressCtrl m_ip;
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_ip);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS, &CAboutDlg::OnIpnFieldchangedIpaddress1)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CclientDlg 对话框
CCLIENTDLG::CCLIENTDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, m_usrnameLogin(_T(""))
	, m_psswrdLogin(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCLIENTDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOGIN_USR_NM, m_usrnameLogin);
	DDV_MaxChars(pDX, m_usrnameLogin, 10);
	DDX_Text(pDX, IDC_EDIT_LOGIN_PSSWD, m_psswrdLogin);
	DDV_MaxChars(pDX, m_psswrdLogin, 15);
}

BEGIN_MESSAGE_MAP(CCLIENTDLG, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL, &CCLIENTDLG::OnClose)
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCLIENTDLG::On_bn_clicked_log_in)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN_REG, &CCLIENTDLG::On_bn_clicked_button_sign_in)
	ON_EN_CHANGE(IDC_EDIT_LOGIN_USR_NM, &CCLIENTDLG::On_en_ehange_work_num)
	ON_EN_CHANGE(IDC_EDIT_LOGIN_PSSWD, &CCLIENTDLG::On_en_change_password)
END_MESSAGE_MAP()

void CCLIENTDLG::OnClose() {
	exit(0);
}

// CclientDlg 消息处理程序
BOOL CCLIENTDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCLIENTDLG::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CCLIENTDLG::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCLIENTDLG::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCLIENTDLG::On_bn_clicked_log_in()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);     //用于将屏幕上控件中的数据交换到变量中
	if (m_usrnameLogin.GetLength()!=0 && m_psswrdLogin.GetLength() != 0)
	{
	  snd_to_srv(CString(LOGIN_SIGN) + m_usrnameLogin + DECOLLATOR + m_psswrdLogin);
		
		Sleep(1000);
		if (g_mode==LOGIN_FAILURE)
		{
			AfxMessageBox(_T("密码或用户名错误！！！")); 
		}
		else if(g_mode==LOGIN_SUCCESS)
		{
			username = m_usrnameLogin;
			this->ShowWindow(SW_HIDE);
			/*cSendmessage a;
			a.DoModal();*/
			C_MAIN_BAORD  a;
			a.DoModal(); 
			g_mode = " ";
            this->ShowWindow(SW_SHOW);
			OnOK();
		}
	}
	else
	{
		AfxMessageBox(_T("密码或用户名为空！！！"));
	}
	//OnOK();
}

void CCLIENTDLG::On_bn_clicked_button_sign_in()
{
	// TODO: 在此添加控件通知处理程序代码
	//INT_PTR nRess;
	this->ShowWindow(SW_HIDE);
	C_REGISTER regDlg;
	//clientStartup();
	regDlg.DoModal();
	CDialogEx::OnOK();
}

void CCLIENTDLG::On_en_ehange_work_num()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	// TODO:  在此添加控件通知处理程序代码
}

void CCLIENTDLG::On_en_change_password()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	// TODO:  在此添加控件通知处理程序代码
}

void CAboutDlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	*pResult = 0;
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
