#pragma once
#include "Resource.h"

// CChildDlgA3 对话框

class CChildDlgA3 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlgA3)

public:
	CChildDlgA3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlgA3();

// 对话框数据
	enum { IDD = IDD_CHILDDLG_A3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
