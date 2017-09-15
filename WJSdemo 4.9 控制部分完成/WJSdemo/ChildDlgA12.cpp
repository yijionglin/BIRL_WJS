// ChildDlgA12.cpp : 实现文件
//

#include "stdafx.h"
#include "WJSdemo.h"
#include "ChildDlgA12.h"
#include "afxdialogex.h"


// CChildDlgA12 对话框

IMPLEMENT_DYNAMIC(CChildDlgA12, CDialogEx)

CChildDlgA12::CChildDlgA12(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlgA12::IDD, pParent)
{

}

CChildDlgA12::~CChildDlgA12()
{
}

void CChildDlgA12::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDlgA12, CDialogEx)
END_MESSAGE_MAP()


// CChildDlgA12 消息处理程序
