// ChildDlgA1.cpp : 实现文件
//

#include "stdafx.h"
#include "WJSdemo.h"
#include "ChildDlgA1.h"
#include "afxdialogex.h"
#include "WJSdemoDlg.h"


// CChildDlgA1 对话框

IMPLEMENT_DYNAMIC(CChildDlgA1, CDialogEx)

CChildDlgA1::CChildDlgA1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlgA1::IDD, pParent)
{

}

CChildDlgA1::~CChildDlgA1()
{
}

void CChildDlgA1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_taba1);
}


BEGIN_MESSAGE_MAP(CChildDlgA1, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CChildDlgA1::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CChildDlgA1 消息处理程序
BOOL CChildDlgA1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。


	// TODO:  在此添加额外的初始化代码
	CRect tabRect;   // 标签控件客户区的位置和大小   
	SetWindowText(_T("位置"));

	m_taba1.InsertItem(0, _T("常规"));  // 插入A组第一个标签“常规”  
	m_taba1.InsertItem(1, _T("程序"));  // 插入A组第二个标签“程序”   
	m_taba1.InsertItem(2, _T("信息"));  // 插入A组第二个标签“信息”  
	m_taba1.InsertItem(3, _T("设定"));  // 插入A组第四个标签“设定”   

	m_ChildDlgA11.Create(IDD_CHILDDLG_A11, &m_taba1);    // 创建A组第一个标签页，m_ChildDlgA1为对话框的类对象，通过该对象可以调用Create函数来创建标签页，第一个参数为对话框ID，第二个为Tab控件的关联变量。
	m_ChildDlgA12.Create(IDD_CHILDDLG_A12, &m_taba1); // 创建A组第二个标签页  
	m_ChildDlgA13.Create(IDD_CHILDDLG_A13, &m_taba1); // 创建A组第三个标签页  
	m_ChildDlgA14.Create(IDD_CHILDDLG_A14, &m_taba1); // 创建A组第四个标签页  


	m_taba1.GetClientRect(&tabRect); // 获取标签控件客户区大小Rect，并传到tabRect中。
	// 调整tabRect，使其覆盖范围适合放置标签页   
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	// 根据调整好的tabRect放置m_ChildDlgA1子对话框，并设置为显示   
	m_ChildDlgA11.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	// m_ChildDlgA2子对话框置为隐藏   
	m_ChildDlgA12.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	//m_ChildDlgA3子对话框设置为隐藏   
	m_ChildDlgA13.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	//m_ChildDlgA4子对话框设置为隐藏   
	m_ChildDlgA14.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CChildDlgA1::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here   
	*pResult = 0;
	CRect tabRect;    // 标签控件客户区的Rect   

	// 获取标签控件客户区Rect，并对其调整，以适合放置标签页   
	m_taba1.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	switch (m_taba1.GetCurSel())//根据TabControl标签控件的当前选择来显示标签页
	{
		// 如果标签控件当前选择标签为“位置”，则显示m_ChildDlgA1对话框，隐藏其余对话框   
	case 0:
	{m_ChildDlgA11.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	m_ChildDlgA12.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_ChildDlgA13.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_ChildDlgA14.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	FromHandle(hWnd)->SendMessage(TABA1GLFLUSH, 0, 0);//

	break; }
	case 1:
		m_ChildDlgA11.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA12.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_ChildDlgA13.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA14.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 2:
		m_ChildDlgA11.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA12.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA13.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_ChildDlgA14.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 3:
		m_ChildDlgA11.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA12.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA13.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA14.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		break;
	default:
		break;
	}
}
