#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "ReadTXTData.h"

// CChildDlgB2 对话框
struct Speed{
	double m_noncutspeed;
	double m_cutspeed;
};
class CChildDlgB2 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlgB2)

public:
	CChildDlgB2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlgB2();

// 对话框数据
	enum { IDD = IDD_CHILDDLG_B2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedVisualsimulationBtn();
	//Speed1 空切速度
	double m_speed1;
	//Speed2 实切速度
	double m_speed2;
//	CSliderCtrl m_sliderNonCuttingRate;
//	CSliderCtrl m_slidernoncuttingrate;
	afx_msg void OnNMCustomdrawSliderNoncuttingrate(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SliderNonCuttingRate;
	CSliderCtrl m_SliderFeedRate;
	afx_msg void OnNMCustomdrawSliderFeedrate(NMHDR *pNMHDR, LRESULT *pResult);
	CReadTXTData ReadTXTData;
	Speed CNCSpeed;
	afx_msg void OnBnClickedBtnDynamicsimulation();
	bool B2ReadTXTData(float **b, char* cwd);
	void StartErrorWarning();
	afx_msg void OnBnClickedBtnProcess();
};
