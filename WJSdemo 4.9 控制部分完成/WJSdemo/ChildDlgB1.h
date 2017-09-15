#pragma once
#include "Resource.h"
#include "afxwin.h"

// CChildDlgB1 对话框

class CChildDlgB1 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlgB1)

public:
	CChildDlgB1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlgB1();

// 对话框数据
	enum { IDD = IDD_CHILDDLG_B1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenfileStaticdraw();
	afx_msg void OnBnClickedCleardxfdata();
	CEdit m_EdiitFilePath;

	//获取Dlg中打开DXF文件的路径并显示于编辑框
	afx_msg LRESULT GetFilePathAndShow(WPARAM wparam, LPARAM lparam);

	//输入的引导线长度
//	double m_leadline_length;
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtConfiguration();
	afx_msg void OnBnClickedBtnContourextraction();
};
