#pragma once
#include "Resource.h"

// CChildDlgA13 对话框

class CChildDlgA13 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlgA13)

public:
	CChildDlgA13(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlgA13();

// 对话框数据
	enum { IDD = IDD_CHILDDLG_A13 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
