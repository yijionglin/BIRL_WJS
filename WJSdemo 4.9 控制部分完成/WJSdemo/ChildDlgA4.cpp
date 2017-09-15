// ChildDlgA4.cpp : 实现文件
//

#include "stdafx.h"
#include "WJSdemo.h"
#include "ChildDlgA4.h"
#include "afxdialogex.h"


// CChildDlgA4 对话框

IMPLEMENT_DYNAMIC(CChildDlgA4, CDialogEx)

CChildDlgA4::CChildDlgA4(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlgA4::IDD, pParent)
{

}

CChildDlgA4::~CChildDlgA4()
{
}

void CChildDlgA4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDlgA4, CDialogEx)
END_MESSAGE_MAP()


// CChildDlgA4 消息处理程序
