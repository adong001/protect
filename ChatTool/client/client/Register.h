#pragma once
#include "afxwin.h"


// Register 对话框

class C_REGISTER: public CDialogEx
{
	DECLARE_DYNAMIC(C_REGISTER)

public:
	C_REGISTER(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~C_REGISTER();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString p_paramname;
	CString m_paramname;
	CString m_csRegWorkId;
	CString m_csRegPsswd;
	CString m_csRegPsswdCfm;
	afx_msg void on_bn_clicked_buttonreturn();
	afx_msg void on_bn_clicked_buttonconfirm();
	afx_msg void on_en_change_edit_username();
	afx_msg void on_en_change_editpassword();
	afx_msg void on_en_change_editconfirmpassword();	
	afx_msg void on_cbn_selchange_combo1_apartment();
	afx_msg void on_cbn_setfocus_combo1();
	afx_msg void on_cbn_selchange_combo_post();
	afx_msg void on_cbn_setfocus_combo_post();
	CEdit * view(CString prama, int num, CEdit * pBoxOne);
	CComboBox m_combo;
	CComboBox p_combo;
	CString username;
	afx_msg void on_en_change_edit2_resign();
private:
	CString error_output;
};
