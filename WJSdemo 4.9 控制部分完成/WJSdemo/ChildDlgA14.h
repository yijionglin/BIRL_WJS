#pragma once
#include "Resource.h"

// CChildDlgA14 对话框

class CChildDlgA14 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlgA14)

public:
	CChildDlgA14(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlgA14();

// 对话框数据
	enum { IDD = IDD_CHILDDLG_A14 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
