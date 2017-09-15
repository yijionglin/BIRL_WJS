// ChildDlgB4.cpp : 实现文件
//

#include "stdafx.h"
#include "WJSdemo.h"
#include "ChildDlgB4.h"
#include "afxdialogex.h"
#include "WJSdemoDlg.h"


// CChildDlgB4 对话框

IMPLEMENT_DYNAMIC(CChildDlgB4, CDialogEx)

CChildDlgB4::CChildDlgB4(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlgB4::IDD, pParent)
	//, m_ConnectAdressInput(NULL)
	, m_ConnectAdressInput(_T(""))
{

}

CChildDlgB4::~CChildDlgB4()
{
}

void CChildDlgB4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_connectlistbox);
	DDX_Text(pDX, IDC_EDIT_CONNECTADRESS, m_ConnectAdressInput);
}


BEGIN_MESSAGE_MAP(CChildDlgB4, CDialogEx)
	ON_BN_CLICKED(IDC_DISCONNECT, &CChildDlgB4::OnBnClickedDisconnect)
	ON_BN_CLICKED(IDC_CONNECT, &CChildDlgB4::OnBnClickedConnect)
	ON_MESSAGE(CNCCONNECTRESULT, CChildDlgB4::CNCConnectResultFunc)
END_MESSAGE_MAP()


// CChildDlgB4 消息处理程序

BOOL CChildDlgB4::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HBITMAP hBmp1 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_CONNECT));
	((CButton *)GetDlgItem(IDC_CONNECT))->SetBitmap(hBmp1);

	HBITMAP hBmp2 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_SCAN));
	((CButton *)GetDlgItem(IDC_SCAN))->SetBitmap(hBmp2);

	HBITMAP hBmp3 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_DISCONNECT));
	((CButton *)GetDlgItem(IDC_DISCONNECT))->SetBitmap(hBmp3);

	SetWindowText(_T("连接"));

	GetDlgItem(IDC_EDIT_LOCALIP)->SetWindowText(_T("192.168.3.1"));
	SetDlgItemText(IDC_EDIT_CONNECTSTATE, _T("断开/未连接"));
	//m_connectlistbox.AddString(_T("192.168.3.12"));        // 在列表框结尾添加字符串 
	//m_connectlistbox.AddString(_T("192.168.3.24"));        // 在列表框结尾添加字符串  
	//m_connectlistbox.AddString(_T("192.168.3.30"));        // 在列表框结尾添加字符串   

	return TRUE;
}

void CChildDlgB4::OnBnClickedDisconnect()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CChildDlgB4::OnBnClickedConnect()
{
	SetDlgItemText(IDC_EDIT_CONNECTSTATE, _T("连接中"));
	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB2)->m_hWnd, NULL, NULL, _T("手动"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	UpdateData(TRUE);

	UpdateData(FALSE);
	int len = WideCharToMultiByte(CP_ACP, 0, m_ConnectAdressInput, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_ConnectAdress = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_ConnectAdressInput, -1, m_ConnectAdress, len, NULL, NULL);
	FromHandle(hWnd)->SendMessage(CNCCONNECT, (WPARAM)m_ConnectAdress, 0);//
}

LRESULT CChildDlgB4::CNCConnectResultFunc(WPARAM a, LPARAM b){

	int result = (int)a;
	if (result == 1){
		SetDlgItemText(IDC_EDIT_CONNECTSTATE, _T("连接失败"));

	}
	else{
		SetDlgItemText(IDC_EDIT_CONNECTSTATE, _T("连接成功"));
		SetDlgItemText(IDC_EDIT_CONNECTADRESS, _T("192.168.3.99"));

	}
	return 0;
}