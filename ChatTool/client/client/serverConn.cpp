// CServerCfg.cpp: 实现文件
//

#include "stdafx.h"
#include "client.h"
#include "serverConn.h"
#include "afxdialogex.h"
#include"cSendmessage.h"
#include"Register.h"
#include"Success.h"
#include<iostream>
using namespace std;

char g_strSrvIP[20];
// CServerCfg 对话框

IMPLEMENT_DYNAMIC(C_SERVER_CFG, CDialogEx)

C_SERVER_CFG::C_SERVER_CFG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SERVER_CFG, pParent)
	, serverIP(_T(""))
{

}

C_SERVER_CFG::~C_SERVER_CFG()
{
}

void C_SERVER_CFG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_ip);
}


BEGIN_MESSAGE_MAP(C_SERVER_CFG, CDialogEx)
	ON_BN_CLICKED(IDCANCEL,&C_SERVER_CFG::OnClose)
	ON_BN_CLICKED(IDOK, &C_SERVER_CFG::On_bn_clicked_ok_ipwindow)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS, &C_SERVER_CFG::On_ipn_fieldchanged_ipaddress)
END_MESSAGE_MAP()


void C_SERVER_CFG::OnClose()
{
	exit(0);
}

// CServerCfg 消息处理程序

void C_SERVER_CFG::On_bn_clicked_ok_ipwindow()
{
	UpdateData(TRUE);  //用于将屏幕上控件中的数据交换到变量中
	
    //读取IP Address的内容.及读取ip地址
	
	DWORD dwIP;
	IN_ADDR ia;
	char* strIP;
	m_ip.GetAddress(dwIP);
	ia.S_un.S_addr = htonl(dwIP);
	strIP = inet_ntoa(ia);

	if (strIP != NULL)  
	{	
        strcpy_s(g_strSrvIP, strIP);
		
        // TODO: 在此添加控件通知处理程序代码
		CDialogEx::OnOK();
		this->ShowWindow(SW_HIDE);
	}
	
}


void C_SERVER_CFG::On_ipn_fieldchanged_ipaddress(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
