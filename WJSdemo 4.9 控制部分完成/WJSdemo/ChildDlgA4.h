#pragma once
#include "Resource.h"

// CChildDlgA4 对话框

class CChildDlgA4 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlgA4)

public:
	CChildDlgA4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlgA4();

// 对话框数据
	enum { IDD = IDD_CHILDDLG_A4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
