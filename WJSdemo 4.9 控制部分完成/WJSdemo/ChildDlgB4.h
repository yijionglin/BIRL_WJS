#pragma once
#include "Resource.h"
#include "afxwin.h"

// CChildDlgB4 对话框

class CChildDlgB4 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlgB4)

public:
	CChildDlgB4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlgB4();

// 对话框数据
	enum { IDD = IDD_CHILDDLG_B4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDisconnect();
	CListBox m_connectlistbox;
	afx_msg void OnBnClickedConnect();
	afx_msg LRESULT CNCConnectResultFunc(WPARAM wparam, LPARAM lparam);

//	char* m_ConnectAdressInput;
	CString m_ConnectAdressInput;
	char* m_ConnectAdress;
};
