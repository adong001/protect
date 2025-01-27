// cSendmessage.cpp: 实现文件

/*
created by 许鹏飞、孙颖
此程序用于在文本框显示己方发送的消息
3个变量用来存放己方g_strSrvIP，系统时间和刚刚发送出去的消息
组合后形成一个新的字符串m_show，就可以及时更新文本框消息
*/
#include "stdafx.h"
#include "C_transform_S.h"
#include "send.h"
#include "client.h"
#include "cSendmessage.h"
#include "afxdialogex.h"
#include "cSendmessage.h"
#include <ctime>
#include "winsock.h"
#include "afx.h"
#include "winbase.h"
#include "winsock.h"
#include "afx.h"
#include <iostream>
#include <atlstr.h>
#include "windows.h"
#include "Register.h"
#include "clientDlg.h"
#include "savemessage.h"
#include "commDef.h"
#include "receive.h"
#include "mainBoard.h"

using  namespace std;
extern string g_message[2];
//string giveServer;
extern bool g_bIsRcvMsg;
extern string g_mode;
string path = "";
																	 
IMPLEMENT_DYNAMIC(C_SEND_MESSAGE, CDialogEx)

C_SEND_MESSAGE::C_SEND_MESSAGE(CWnd* pParent )
	: CDialogEx(IDD_DIALOG_CHAT_MAIN, pParent)
{
	is_recv=true;
}

C_SEND_MESSAGE::~C_SEND_MESSAGE()
{
}

void C_SEND_MESSAGE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_message);
	DDX_Text(pDX, IDC_EDIT_IP, m_oppositeName);
	DDX_Text(pDX, IDC_EDIT_SHOW, m_show);
}

BEGIN_MESSAGE_MAP(C_SEND_MESSAGE, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &C_SEND_MESSAGE::OnBnClickedButtonsend)
	ON_EN_CHANGE(IDC_EDIT_SHOW, &C_SEND_MESSAGE::OnEnChangeEditshow)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_IP, &C_SEND_MESSAGE::On_en_change_edit_ip)
	ON_EN_CHANGE(IDC_EDIT_MESSAGE, &C_SEND_MESSAGE::On_en_change_edit_message)
	ON_BN_CLICKED(IDC_BUTTON_Message, &C_SEND_MESSAGE::OnBnClickedButtonMessage)
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


extern CString m_frndNm;
// cSendmessage 消息处理程序
//发送按钮
void C_SEND_MESSAGE::OnBnClickedButtonsend()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取本机时间
	//string str;
	CTime time;
	CString timeStr;
	time = CTime::GetCurrentTime();             //获取系统当前时间
	timeStr = time.Format(_T("%Y-%m-%d %H:%M:%S"));
	//获取本机g_strSrvIP
	WORD wVersionRequested;
	WSADATA wsaData;
	char cHostNm[255];
	CString csHostIP;
	PHOSTENT oHostInfo;
	wVersionRequested = MAKEWORD(2, 0);
	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if (gethostname(cHostNm, sizeof(cHostNm)) == 0)
		{
			if ((oHostInfo = gethostbyname(cHostNm)) != NULL)
			{
				csHostIP = inet_ntoa(*(struct in_addr *)*oHostInfo->h_addr_list);
			}
		}
		WSACleanup();
	}
	//更新文本框
	UpdateData(TRUE);
	m_oppositeName = m_frndNm;
	if (m_message.GetLength() != 0&&m_oppositeName.GetLength()!=0)
	{
		m_show += *"\n" + timeStr  + *"\r\n";
		m_show += *"\n" + m_message + *"\r\n";
		string saveMessage = CT2A((timeStr + CString(" ") + CString("本机") + CString(" To ") + m_oppositeName + CString(" : ") + m_message + CString("\n\r")).GetBuffer());
		
		C_SAVE_MESSAGE::saveMessageToFile(saveMessage, m_oppositeName);

		snd_to_srv(CString(SEND_MESSAGE_SIGN) + CString(DECOLLATOR) + m_oppositeName + CString(DECOLLATOR) + m_message);
	}
	
	UpdateData(FALSE);  //用于将数据在屏幕中对应控件中显示出来。
	//((CEdit*)GetDlgItem(IDC_EDIT_SHOW))->SetSel(m_show.GetLength(), m_show.GetLength());
	SetDlgItemText(IDC_EDIT_MESSAGE, _T(""));   //函数的功能：设置对话框中控件的文本和标题。
	UpdateWindow();
}

//每隔一秒更新文本框消息
void C_SEND_MESSAGE::OnEnChangeEditshow()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	// TODO:  在此添加控件通知处理程序代码

	//TIMER1表示定时器的ID，1000表示没1000ms也就是1s调用一次处理函数，
	//最后一个参数是处理的函数，如果填NULL表示，使用系统默认的，默认处理函数是On Timer（）那个函数，
	//SetTimer(TIMER1, 1000, NULL);   
	
	//OnTimer(1);
}

//设置定时器
void C_SEND_MESSAGE::OnTimer(UINT_PTR nIDEvent)//接受到消息时更新在对话框
{
	if (nIDEvent == 1)
	{
		if (C_SEND_MESSAGE::is_recv != g_bIsRcvMsg)
		{
			UpdateData(TRUE);
			if (g_message[0] == SEND_MESSAGE_SIGN)
			{
				/*for (int i = 0; i < 2; i++)
				{
					m_show += *"\n" + toCString(g_message[i]) + *"\r\n";
				}*/

				analyse_received_msg(g_message[1]);//拆出时间
				string time = g_message[0];
				analyse_received_msg(g_message[1]);//拆出用户名
				string name = g_message[0];
				string oppositeName = CT2A(m_frndNm.GetBuffer());//获得当前聊天用户名
				C_MAIN_BAORD::set_zero_zero(name);
				if (name == oppositeName)
				{
					analyse_received_msg(g_message[1]);//拆出信息
					string Message = time + " " + oppositeName + " To " + name + " : " + g_message[1] + "\n\r";
					C_SAVE_MESSAGE::saveMessageToFile(Message, toCString(name));//存进本地
					Message = *"\n" + time + " " + name + "" + *"\r\n";
					Message += *"\n" + g_message[1] + *"\r\n";
					m_show += toCString(Message);
					
				}
			}
			
            UpdateData(FALSE);
			
            ((CEdit*)GetDlgItem(IDC_EDIT_SHOW))->SetSel(m_show.GetLength(), m_show.GetLength());
			is_recv = g_bIsRcvMsg;
			UpdateWindow();
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void C_SEND_MESSAGE::On_en_change_edit_ip()
{
	
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	// TODO:  在此添加控件通知处理程序代码

}

void C_SEND_MESSAGE::On_en_change_edit_message()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	// TODO:  在此添加控件通知处理程序代码
	//((CEdit*)GetDlgItem(IDC_EDIT_SHOW))->SetSel(m_show.GetLength(), m_show.GetLength());
}


BOOL C_SEND_MESSAGE::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	this->SetWindowText(this->m_frndNm);

	SetTimer(TIMER1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void C_SEND_MESSAGE::OnBnClickedButtonMessage()
{
	// TODO: 在此添加控件通知处理程序代码
	C_SAVE_MESSAGE::getMessagePath(path);
	C_SAVE_MESSAGE::getSaveFileName(path, m_oppositeName);
	ShellExecute(NULL, _T("open"), C_SAVE_MESSAGE::StringToLPCWSTR(path), NULL, NULL, SW_SHOWNORMAL); // 打开C:\Test.txt 文件
}



void C_SEND_MESSAGE::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
}


int C_SEND_MESSAGE::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}
