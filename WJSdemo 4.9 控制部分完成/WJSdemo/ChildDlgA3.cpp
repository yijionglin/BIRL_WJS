// ChildDlgA3.cpp : 实现文件
//

#include "stdafx.h"
#include "WJSdemo.h"
#include "ChildDlgA3.h"
#include "afxdialogex.h"


// CChildDlgA3 对话框

IMPLEMENT_DYNAMIC(CChildDlgA3, CDialogEx)

CChildDlgA3::CChildDlgA3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlgA3::IDD, pParent)
{

}

CChildDlgA3::~CChildDlgA3()
{
}

void CChildDlgA3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDlgA3, CDialogEx)
END_MESSAGE_MAP()


// CChildDlgA3 消息处理程序
