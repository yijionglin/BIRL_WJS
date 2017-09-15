// ChildDlgB1.cpp : 实现文件
//

#include "stdafx.h"
#include "WJSdemo.h"
#include "ChildDlgB1.h"
#include "afxdialogex.h"
#include "WJSdemoDlg.h"
#include "Configuration.h"


// CChildDlgB1 对话框

IMPLEMENT_DYNAMIC(CChildDlgB1, CDialogEx)

CChildDlgB1::CChildDlgB1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlgB1::IDD, pParent)
	//, m_leadline_length(0)
{

}

CChildDlgB1::~CChildDlgB1()
{
}

void CChildDlgB1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_EdiitFilePath);
	//  DDX_Text(pDX, IDC_LEADLINE_LENGTH, m_leadline_length);
}


BEGIN_MESSAGE_MAP(CChildDlgB1, CDialogEx)
	ON_BN_CLICKED(IDC_OPENFILE_STATICDRAW, &CChildDlgB1::OnBnClickedOpenfileStaticdraw)
	ON_BN_CLICKED(IDC_CLEARDXFDATA, &CChildDlgB1::OnBnClickedCleardxfdata)
	ON_MESSAGE(GET_FILEPATH, &CChildDlgB1::GetFilePathAndShow)

	ON_BN_CLICKED(IDC_BT_CONFIGURATION, &CChildDlgB1::OnBnClickedBtConfiguration)
	ON_BN_CLICKED(IDC_BTN_CONTOUREXTRACTION, &CChildDlgB1::OnBnClickedBtnContourextraction)
END_MESSAGE_MAP()


// CChildDlgB1 消息处理程序
BOOL CChildDlgB1::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	HBITMAP hBmp1 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_OPENDXF));
	((CButton *)GetDlgItem(IDC_OPENFILE_STATICDRAW))->SetBitmap(hBmp1);

	HBITMAP hBmp2 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_CLEAR));
	((CButton *)GetDlgItem(IDC_CLEARDXFDATA))->SetBitmap(hBmp2);

	HBITMAP hBmp3 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_STOCKLAYOUT));
	((CButton *)GetDlgItem(IDC_BTN_STOCKLAYOUT))->SetBitmap(hBmp3);

	HBITMAP hBmp4 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_CONTOUREXTRACTION));
	((CButton *)GetDlgItem(IDC_BTN_CONTOUREXTRACTION))->SetBitmap(hBmp4);

	SetWindowText(_T("文件处理"));

	return TRUE;
}

void CChildDlgB1::OnBnClickedOpenfileStaticdraw()
{
	//CWnd* pWnd = GetDlgItem(IDD_NESTDXFDATAFORCUT_DIALOG);
	//HWND hWnd = pWnd->GetSafeHwnd();
	if (CNCRunModeInf.StopRunning == true){
	DeleteFile(_T("123嵌套封闭环.txt"));

	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	//HWND hWnd = ::FindWindowEx(m_hWnd, NULL, NULL, _T("多线程绘图/传递头节点"));//最后一项为NULL可以，或为父框内的Dlg的标题，this->GetParent()->m_hWnd为父框手柄
	//hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	//hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	FromHandle(hWnd)->SendMessage(PROCEEDDXFDATA, 0, 0);
	}
	else 	MessageBox(_T("请先停止目前进行的模式"));

}


void CChildDlgB1::OnBnClickedCleardxfdata()
{
	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	FromHandle(hWnd)->SendMessage(CLEARDXFDATA, 0, 0);
}

LRESULT CChildDlgB1::GetFilePathAndShow(WPARAM wparam, LPARAM lparam)  //消息函数，接受来自主Dlg打开的DXF路径并显示
{
	char *pChar = (char*)wparam;
	int charLen = strlen(pChar); //计算pChar所指向的字符串大小，以字节为单位，一个汉字占两个字节  
	int len = MultiByteToWideChar(CP_ACP, 0, pChar, charLen, NULL, 0); //计算多字节字符的大小，按字符计算  
	wchar_t *pWChar = new wchar_t[len + 1]; //为宽字节字符数申请空间，  
	MultiByteToWideChar(CP_ACP, 0, pChar, charLen, pWChar, len); //多字节编码转换成宽字节编码  
	pWChar[len] = '\0';

	//将wchar_t数组转换为CString  
	CString str;
	str.Append(pWChar);

	delete[] pChar;
	delete[] pWChar;
	SetDlgItemText(IDC_EDIT_FILEPATH, str);
	return 0;
}

//void CChildDlgB1::OnBnClickedButton1()
//{
//	UpdateData(TRUE);
//	m_LeadingLineLength = m_leadline_length;
//	UpdateData(FALSE);
//	if (m_LeadingLineLength == 0){
//		MessageBox(_T("请输入大于零的切割引导线长度"));
//	}
//	else{
//
//	}
//
//}


void CChildDlgB1::OnBnClickedBtConfiguration()
{
	CConfiguration ConfigDlg;
	ConfigDlg.DoModal();
}


void CChildDlgB1::OnBnClickedBtnContourextraction()
{
	// TODO:  在此添加控件通知处理程序代码
}
