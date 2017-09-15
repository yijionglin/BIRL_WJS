#pragma once
#include "afxwin.h"


// CConfiguration 对话框
class CConfiguration : public CDialogEx
{
	DECLARE_DYNAMIC(CConfiguration)

public:
	CConfiguration(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfiguration();

// 对话框数据
	enum { IDD = IDD_CONFIG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	double m_LeadLineLength_input;
	afx_msg void OnBnClickedSetConfiguration();
	CButton m_LeadLineSetting;
	virtual BOOL OnInitDialog();
	static  UINT SetWaterJetConfigThread(LPVOID lParam);//参数设置多线程
	void SetWaterJetConfig();
};
