#pragma once

#include "Resource.h"
#include "CNCInterface.hpp"
#include "ReadTXTData.h"
#include "GlobalSetting.h"
#include "afxwin.h"
using namespace CNCControl;




// CChildDlgB3 对话框
class CChildDlgB3 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlgB3)

public:
	CChildDlgB3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlgB3();

// 对话框数据
	enum { IDD = IDD_CHILDDLG_B3 };
private:
	CNCInterface  CNCWaterjet = CNCInterface("./lib");//不能用隐式声明CNCInterface CNCWaterjet("./lib");
	//SpeedStruct* getCNCSpeed;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	char* m_cwd;
	bool m_HaveSetManualMode;
	int m_TurnOnDO_Sequency;
	//关闭砂阀水阀高压气阀
	void TurnOffDO_66_67_68();
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedJogxp();

	afx_msg void OnBnClickedRdJogSpeed1();
	afx_msg void OnBnClickedRdJogSpeed01();
	afx_msg void OnBnClickedRdJogSpeed10();
	afx_msg void OnBnClickedRdJogSpeedCustomized();
	afx_msg void OnBnClickedBtConfirmJogspeed();

	afx_msg void OnBnClickedRdJogDistance01();
	afx_msg void OnBnClickedRdJogDistance1();
	afx_msg void OnBnClickedRdJogDistance10();
	afx_msg void OnBnClickedRdJogDistanceCustomized();
	afx_msg void OnBnClickedBtConfirmJogdistance();
	static  UINT ConectThreadFunction(LPVOID lParam);//连接多线程
	static  UINT AutoMotionCommandThread(LPVOID lParam);//自动加工多线程
	static  UINT HomeJointCommandThread(LPVOID lParam);//回零多线程
	static  UINT JogStepThread(LPVOID lParam);//步进多线程多线程

	void OpenTXTFileAndGetData(int maxcol, float** b);
	void DeleteTXTData(int maxrow, float** b);
	void SetToManualMode();
	void ConnectFunc();
	void MotionCommandFunc(LPVOID lpParam);
	void HomeJointCommandFunc();
	void JogStepCommandFunc();
	void setDOFunc();
	void SendCNCPos();
	void TurnOnDO_Order(int DOSequency);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CReadTXTData CNCReadTxtData;
	afx_msg LRESULT CNCConnectFunc(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT setDOFunc(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT GetAutoStartMessage(WPARAM w, LPARAM y);
	afx_msg LRESULT PauseandContinueVisualSimulation(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT GetPauseOrContinueMessage(WPARAM wparam, LPARAM lparam);
	afx_msg void OnBnClickedRdJogContinue();
	afx_msg void OnBnClickedRdJogStep();
	afx_msg void OnBnClickedRdJogInching();
	afx_msg void OnBnClickedButton11();


	afx_msg void OnBnClickedBtXHome();
	afx_msg void OnBnClickedBtYHome();
	afx_msg void OnBnClickedBtZHome();

	afx_msg void OnBnClickedButton1();
	CButton m_RBGroup1;
	CButton m_RBGroup2;
	CButton m_RBGroup3;
	afx_msg void OnBnClickedButton12();

};
