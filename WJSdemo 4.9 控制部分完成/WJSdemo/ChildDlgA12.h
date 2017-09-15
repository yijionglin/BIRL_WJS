#pragma once
#include "Resource.h"

// CChildDlgA12 对话框

class CChildDlgA12 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlgA12)

public:
	CChildDlgA12(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlgA12();

// 对话框数据
	enum { IDD = IDD_CHILDDLG_A11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
