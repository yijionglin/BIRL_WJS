// ChildDlgA2.cpp : 实现文件
//

#include "stdafx.h"
#include "WJSdemo.h"
#include "ChildDlgA2.h"
#include "afxdialogex.h"


// CChildDlgA2 对话框

IMPLEMENT_DYNAMIC(CChildDlgA2, CDialogEx)

CChildDlgA2::CChildDlgA2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlgA2::IDD, pParent)
{

}

CChildDlgA2::~CChildDlgA2()
{
}

void CChildDlgA2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDlgA2, CDialogEx)
END_MESSAGE_MAP()


// CChildDlgA2 消息处理程序
