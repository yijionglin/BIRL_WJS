// ChildDlgB3.cpp : 实现文件
//

#include "stdafx.h"
#include "WJSdemo.h"
#include "ChildDlgB3.h"
#include "afxdialogex.h"
#include "WJSdemoDlg.h"


// CChildDlgB3 对话框
struct ManualMode CNCmanualmode = { sd, true, 0, 0, AN_X };
float speed1 = 0;
float speed2 = 0;
int cali_step;
bool b_setBaseFrame = false;
bool b_CutColor = false;

IMPLEMENT_DYNAMIC(CChildDlgB3, CDialogEx)

CChildDlgB3::CChildDlgB3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlgB3::IDD, pParent)
{

}

CChildDlgB3::~CChildDlgB3()
{
}

void CChildDlgB3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RD_JOG_CONTINUE, m_RBGroup1);
	DDX_Control(pDX, IDC_RD_JOG_SPEED_0_1, m_RBGroup2);
	DDX_Control(pDX, IDC_RD_JOG_DISTANCE_0_1, m_RBGroup3);
}


BEGIN_MESSAGE_MAP(CChildDlgB3, CDialogEx)
	ON_BN_CLICKED(IDC_JOGXP, &CChildDlgB3::OnBnClickedJogxp)
	ON_BN_CLICKED(IDC_RD_JOG_SPEED_CUSTOMIZED, &CChildDlgB3::OnBnClickedRdJogSpeedCustomized)
	ON_BN_CLICKED(IDC_BT_CONFIRM_JOGSPEED, &CChildDlgB3::OnBnClickedBtConfirmJogspeed)
	ON_BN_CLICKED(IDC_RD_JOG_SPEED_1, &CChildDlgB3::OnBnClickedRdJogSpeed1)
	ON_BN_CLICKED(IDC_RD_JOG_SPEED_0_1, &CChildDlgB3::OnBnClickedRdJogSpeed01)
	ON_BN_CLICKED(IDC_RD_JOG_SPEED_10, &CChildDlgB3::OnBnClickedRdJogSpeed10)
	ON_BN_CLICKED(IDC_RD_JOG_DISTANCE_0_1, &CChildDlgB3::OnBnClickedRdJogDistance01)
	ON_BN_CLICKED(IDC_RD_JOG_DISTANCE_1, &CChildDlgB3::OnBnClickedRdJogDistance1)
	ON_BN_CLICKED(IDC_RD_JOG_DISTANCE_10, &CChildDlgB3::OnBnClickedRdJogDistance10)
	ON_BN_CLICKED(IDC_RD_JOG_DISTANCE_CUSTOMIZED, &CChildDlgB3::OnBnClickedRdJogDistanceCustomized)
	ON_BN_CLICKED(IDC_BT_CONFIRM_JOGDISTANCE, &CChildDlgB3::OnBnClickedBtConfirmJogdistance)
	ON_MESSAGE(CNCCONNECT, CChildDlgB3::CNCConnectFunc)//连接
	ON_MESSAGE(SETDOSTATES, CChildDlgB3::setDOFunc)//设置DO
	ON_MESSAGE(SEND_AUTO_START_MESSAGE,CChildDlgB3::GetAutoStartMessage)
	
	ON_MESSAGE(PAUSEORCONTINUE, CChildDlgB3::PauseandContinueVisualSimulation)

	ON_BN_CLICKED(IDC_RD_JOG_CONTINUE, &CChildDlgB3::OnBnClickedRdJogContinue)
	ON_BN_CLICKED(IDC_RD_JOG_STEP, &CChildDlgB3::OnBnClickedRdJogStep)
	ON_BN_CLICKED(IDC_RD_JOG_INCHING, &CChildDlgB3::OnBnClickedRdJogInching)
	ON_BN_CLICKED(IDC_BUTTON11, &CChildDlgB3::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BT_X_HOME, &CChildDlgB3::OnBnClickedBtXHome)
	ON_BN_CLICKED(IDC_BT_Y_HOME, &CChildDlgB3::OnBnClickedBtYHome)
	ON_BN_CLICKED(IDC_BT_Z_HOME, &CChildDlgB3::OnBnClickedBtZHome)
	ON_BN_CLICKED(IDC_BUTTON1, &CChildDlgB3::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON12, &CChildDlgB3::OnBnClickedButton12)
END_MESSAGE_MAP()


// CChildDlgB3 消息处理程序
BOOL CChildDlgB3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(_T("手动"));
	cali_step = 1;
	return TRUE;
}


void CChildDlgB3::OnBnClickedJogxp()
{
	// TODO:  在此添加控件通知处理程序代码
}


///////////////////////////////////Radio选择框///////////////////////////////////////
void CChildDlgB3::OnBnClickedRdJogContinue()
{
	CNCmanualmode.jogmode = jogcontinue;
}


void CChildDlgB3::OnBnClickedRdJogStep()
{

	CNCmanualmode.jogmode = jogstep;
}


void CChildDlgB3::OnBnClickedRdJogInching()
{
	CNCmanualmode.jogmode = joginching;
}





void CChildDlgB3::OnBnClickedRdJogSpeed01()
{
	CNCmanualmode.jogSpeed = 1;
	GetDlgItem(IDC_EDIT_JOGSPEED_INPUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_CONFIRM_JOGSPEED)->EnableWindow(FALSE);
}

void CChildDlgB3::OnBnClickedRdJogSpeed1()
{
	CNCmanualmode.jogSpeed = 3;

	GetDlgItem(IDC_EDIT_JOGSPEED_INPUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_CONFIRM_JOGSPEED)->EnableWindow(FALSE);
}




void CChildDlgB3::OnBnClickedRdJogSpeed10()
{
	CNCmanualmode.jogSpeed = 5;

	GetDlgItem(IDC_EDIT_JOGSPEED_INPUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_CONFIRM_JOGSPEED)->EnableWindow(FALSE);
}

void CChildDlgB3::OnBnClickedRdJogSpeedCustomized()
{
	GetDlgItem(IDC_EDIT_JOGSPEED_INPUT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BT_CONFIRM_JOGSPEED)->EnableWindow(TRUE);

}

void CChildDlgB3::OnBnClickedBtConfirmJogspeed()//默认最大速度为1000MM/S
{
	CString str;
	GetDlgItem(IDC_EDIT_JOGSPEED_INPUT)->GetWindowText(str);
		double jogV_input= _ttof(str);
		double checkjogV=jogV_input / 10;
		if (checkjogV > 15){
			MessageBox(_T("步进速度不能大于150 mm/s（9000 mm/min）"));
		}
		else{
			CNCmanualmode.jogSpeed = jogV_input / 10;
		}

}
void CChildDlgB3::OnBnClickedRdJogDistance01()
{
	CNCmanualmode.jogstepDistance = 0.1;
	GetDlgItem(IDC_EDIT_JOGDISTANCE_INPUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_CONFIRM_JOGDISTANCE)->EnableWindow(FALSE);
}


void CChildDlgB3::OnBnClickedRdJogDistance1()
{
	CNCmanualmode.jogstepDistance = 1;

	GetDlgItem(IDC_EDIT_JOGDISTANCE_INPUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_CONFIRM_JOGDISTANCE)->EnableWindow(FALSE);
}


void CChildDlgB3::OnBnClickedRdJogDistance10()
{
	CNCmanualmode.jogstepDistance = 10;
	GetDlgItem(IDC_EDIT_JOGDISTANCE_INPUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_CONFIRM_JOGDISTANCE)->EnableWindow(FALSE);
}


void CChildDlgB3::OnBnClickedRdJogDistanceCustomized()
{


	GetDlgItem(IDC_EDIT_JOGDISTANCE_INPUT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BT_CONFIRM_JOGDISTANCE)->EnableWindow(TRUE);
}


void CChildDlgB3::OnBnClickedBtConfirmJogdistance()
{
	CString str;
	GetDlgItem(IDC_EDIT_JOGDISTANCE_INPUT)->GetWindowText(str);
	double jogDis_input = _ttof(str);

	
	if (jogDis_input > 800){
		MessageBox(_T("单次步进距离不能大于800mm"));
	}
	else{
		if (jogDis_input > 500)
			MessageBox(_T("单次步进距离大于500mm，请小心超出运动范围极限"));

	CNCmanualmode.jogstepDistance = jogDis_input;

	}
}
///////////////////////////////////Radio选择框，以上///////////////////////////////////////

LRESULT CChildDlgB3::CNCConnectFunc(WPARAM wparam, LPARAM lparam)//连接控制器的消息函数（非多线程
{
	char *ConnectAdress = (char*)wparam;
	CString conadr(ConnectAdress);
	CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	edit1->SetWindowText(conadr);

	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB2)->m_hWnd, NULL, NULL, _T("连接"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	if (*ConnectAdress == NULL){
	if (!CNCWaterjet.connect("192.168.3.99")){//默认地址

		FromHandle(hWnd)->SendMessage(CNCCONNECTRESULT, 0, 0);//		CNCWaterjet.enableScriptLoading(false);
		CNCWaterjet.enableScriptLoading(false);
		CNCWaterjet.setCurrentRobot(0);
		CNCWaterjet.setSpeed(10, (BUnits)0);
		CNCWaterjet.setTableFrameNo(1);
		CNCWaterjet.servoOn(true);

	}
	else {
		FromHandle(hWnd)->SendMessage(CNCCONNECTRESULT, 1, 0);//		CNCWaterjet.enableScriptLoading(false);
	}
	}
	else{
		if (!CNCWaterjet.connect(ConnectAdress)){//默认地址

			FromHandle(hWnd)->SendMessage(CNCCONNECTRESULT, 0, 0);//		CNCWaterjet.enableScriptLoading(false);
			CNCWaterjet.enableScriptLoading(false);
			CNCWaterjet.setCurrentRobot(0);
			CNCWaterjet.setSpeed(10, (BUnits)0);
			CNCWaterjet.setTableFrameNo(1);
			CNCWaterjet.servoOn(true);

		}
		else {
			FromHandle(hWnd)->SendMessage(CNCCONNECTRESULT, 1, 0);//		CNCWaterjet.enableScriptLoading(false);
		}
	}
	return 0;

}


UINT CChildDlgB3::JogStepThread(LPVOID lpParam)
{
	CChildDlgB3* p = (CChildDlgB3*)lpParam;
	p->JogStepCommandFunc();
	return 0;
}


void CChildDlgB3::JogStepCommandFunc(){//步进
	CNCWaterjet.setSpeed(CNCmanualmode.jogSpeed, (BUnits)0);
	if ( CNCmanualmode.axisnum == AN_X && CNCmanualmode.jogPositive == true){
		CNCWaterjet.jogRobotXaxis(CNCmanualmode.jogstepDistance, false);
	}
	else if (CNCmanualmode.axisnum == AN_X && CNCmanualmode.jogPositive == false){
		CNCWaterjet.jogRobotXaxis(-CNCmanualmode.jogstepDistance, false);
	}
	else if (CNCmanualmode.axisnum == AN_Y && CNCmanualmode.jogPositive == true){
		CNCWaterjet.jogRobotYaxis(CNCmanualmode.jogstepDistance, false);
	}
	else if (CNCmanualmode.axisnum == AN_Y && CNCmanualmode.jogPositive == false){
		CNCWaterjet.jogRobotYaxis(-CNCmanualmode.jogstepDistance, false);
	}
	else if (CNCmanualmode.axisnum == AN_Z && CNCmanualmode.jogPositive == true){
		CNCWaterjet.jogRobotZaxis(CNCmanualmode.jogstepDistance, false); 
	}
	else if (CNCmanualmode.axisnum == AN_Z && CNCmanualmode.jogPositive == false){
		CNCWaterjet.jogRobotZaxis(-CNCmanualmode.jogstepDistance, false);
	}
	while (CNCWaterjet.isJogging()){
		SendCNCPos();
		Sleep(100);
	};
	CNCWaterjet.endRobotJog();
	SendCNCPos();

}


void CChildDlgB3::SendCNCPos(){

	EPos *s_UpdataCart = (EPos *)malloc(sizeof(EPos));

	//注意 A对应Y轴  B对应X轴
	s_UpdataCart->y = CNCWaterjet.cncPos.A;
	s_UpdataCart->x = CNCWaterjet.cncPos.B;
	s_UpdataCart->z = CNCWaterjet.cncPos.z;
	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	//HWND hWnd = ::FindWindowEx(m_hWnd, NULL, NULL, _T("多线程绘图/传递头节点"));//最后一项为NULL可以，或为父框内的Dlg的标题，this->GetParent()->m_hWnd为父框手柄
	//hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	//hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	FromHandle(hWnd)->SendMessage(UPDATE_CNCPOSITION, (WPARAM)(s_UpdataCart), 0);
	//free(WPC);
	s_UpdataCart = NULL;


}

BOOL CChildDlgB3::PreTranslateMessage(MSG* pMsg){


	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_JOGXP)->m_hWnd &&  CNCmanualmode.jogmode == jogcontinue){//GetDlgItem获得的是指针，然后再找到其句柄
		//SetToManualMode();

		CNCWaterjet.setSpeed(CNCmanualmode.jogSpeed, (BUnits)0);

		//CNCWaterjet.servoOn(true);
		CNCWaterjet.jogRobotXaxis(1, true);
		//while (CNCWaterjet.isJogging());
		//SendCNCPos();

	}
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_JOGYP)->m_hWnd && CNCmanualmode.jogmode == jogcontinue){//GetDlgItem获得的是指针，然后再找到其句柄
		//SetToManualMode();

		CNCWaterjet.setSpeed(CNCmanualmode.jogSpeed, (BUnits)0);

		//CNCWaterjet.servoOn(true);
		CNCWaterjet.jogRobotYaxis(CNCmanualmode.jogSpeed, true);
		//while (CNCWaterjet.isJogging());

	}
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_JOGZP)->m_hWnd && CNCmanualmode.jogmode == jogcontinue){//GetDlgItem获得的是指针，然后再找到其句柄
		//SetToManualMode();

		CNCWaterjet.setSpeed(CNCmanualmode.jogSpeed, (BUnits)0);

		CNCWaterjet.jogRobotZaxis(CNCmanualmode.jogSpeed, true);

	}

	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_JOGXN)->m_hWnd &&  CNCmanualmode.jogmode == jogcontinue){//GetDlgItem获得的是指针，然后再找到其句柄
		//SetToManualMode();

		CNCWaterjet.setSpeed(CNCmanualmode.jogSpeed, (BUnits)0);

		CNCWaterjet.jogRobotXaxis(-1, true);

	}
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_JOGYN)->m_hWnd && CNCmanualmode.jogmode == jogcontinue){//GetDlgItem获得的是指针，然后再找到其句柄
		//SetToManualMode();

		CNCWaterjet.setSpeed(CNCmanualmode.jogSpeed, (BUnits)0);

		CNCWaterjet.jogRobotYaxis(-CNCmanualmode.jogSpeed, true);

	}
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_JOGZN)->m_hWnd && CNCmanualmode.jogmode == jogcontinue){//GetDlgItem获得的是指针，然后再找到其句柄
		//SetToManualMode();

		//if (CNCmanualmode.jogSpeed == 1)
		//	CNCWaterjet.setSpeed(0.1, (BUnits)0);
		//if (CNCmanualmode.jogSpeed == 10)
		//	CNCWaterjet.setSpeed(1, (BUnits)0);
		//if (CNCmanualmode.jogSpeed == 100)
		//	CNCWaterjet.setSpeed(10, (BUnits)0);
		CNCWaterjet.setSpeed(CNCmanualmode.jogSpeed, (BUnits)0);
		CNCWaterjet.jogRobotZaxis(-CNCmanualmode.jogSpeed, true);

	}
	//按钮松开后，取消继续模式
	if (pMsg->message == WM_LBUTTONUP && (
		(pMsg->hwnd == GetDlgItem(IDC_JOGXP)->m_hWnd)
		or(pMsg->hwnd == GetDlgItem(IDC_JOGXN)->m_hWnd)
		or(pMsg->hwnd == GetDlgItem(IDC_JOGYN)->m_hWnd)
		or(pMsg->hwnd == GetDlgItem(IDC_JOGYP)->m_hWnd)
		) && CNCmanualmode.jogmode == jogcontinue){
		//while (CNCWaterjet.isJogging());
		CNCWaterjet.endRobotJog();
		//CNCWaterjet.servoOn(false);
		CNCWaterjet.servoOn(true);
	};

	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_JOGXP)->m_hWnd && CNCmanualmode.jogmode == jogstep){//GetDlgItem获得的是指针，然后再找到其句柄


		if (!CNCmanualmode.jogstepDistance == 0){
			//SetToManualMode();
			CNCmanualmode.axisnum = AN_X;
			CNCmanualmode.jogPositive = true;
			CWinThread* pThread = AfxBeginThread(JogStepThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			pThread->ResumeThread();

		}




	}
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_JOGXN)->m_hWnd && CNCmanualmode.jogmode == jogstep){//GetDlgItem获得的是指针，然后再找到其句柄


		if (!CNCmanualmode.jogstepDistance == 0){
			//SetToManualMode();
			CNCmanualmode.axisnum = AN_X;
			CNCmanualmode.jogPositive = false;
			CWinThread* pThread = AfxBeginThread(JogStepThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			pThread->ResumeThread();
		}



	}
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_JOGYP)->m_hWnd && CNCmanualmode.jogmode == jogstep){//GetDlgItem获得的是指针，然后再找到其句柄


		if (!CNCmanualmode.jogstepDistance == 0){
			//SetToManualMode();
			CNCmanualmode.axisnum = AN_Y;
			CNCmanualmode.jogPositive = true;
			CWinThread* pThread = AfxBeginThread(JogStepThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			pThread->ResumeThread();

		}



	}
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_JOGYN)->m_hWnd && CNCmanualmode.jogmode == jogstep){//GetDlgItem获得的是指针，然后再找到其句柄


		if (!CNCmanualmode.jogstepDistance == 0){
			//SetToManualMode();
			CNCmanualmode.axisnum = AN_Y;
			CNCmanualmode.jogPositive = false;
			CWinThread* pThread = AfxBeginThread(JogStepThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			pThread->ResumeThread();

		}



	}
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_JOGZP)->m_hWnd && CNCmanualmode.jogmode == jogstep){//GetDlgItem获得的是指针，然后再找到其句柄


		if (!CNCmanualmode.jogstepDistance == 0){
			//SetToManualMode();
			CNCmanualmode.axisnum = AN_Z;
			CNCmanualmode.jogPositive = true;
			CWinThread* pThread = AfxBeginThread(JogStepThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			pThread->ResumeThread();
		}



	}
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_JOGZN)->m_hWnd && CNCmanualmode.jogmode == jogstep){//GetDlgItem获得的是指针，然后再找到其句柄


		if (!CNCmanualmode.jogstepDistance == 0){
			//SetToManualMode();
			CNCmanualmode.axisnum = AN_Z;
			CNCmanualmode.jogPositive = false;
			CWinThread* pThread = AfxBeginThread(JogStepThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			pThread->ResumeThread();

		}

	}
	return CDialog::PreTranslateMessage(pMsg);
}



void CChildDlgB3::OnBnClickedButton11()//Calibrate 工件系标定
{
	b_setBaseFrame = true;
	if (cali_step == 1){
		cali_step = 2;
		SetDlgItemText(IDC_EDIT_CALIBRATION, _T("请选择工件系原点"));
	}
	else if (cali_step == 2){
		CNCWaterjet.calibTable(1);
		cali_step = 3;
		BaseframeOrigin.y = CNCWaterjet.cncPos.A;//CNC的原始坐标是乱的
		BaseframeOrigin.x = CNCWaterjet.cncPos.B;
		BaseframeOrigin.z = CNCWaterjet.cncPos.z;
		SetDlgItemText(IDC_EDIT_CALIBRATION, _T("已标定原点，请移动第二点到X轴正方向上"));
		HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
		FromHandle(hWnd)->SendMessage(UPDATE_BASE_ORIGIN, 0, 0);
	}
	else if (cali_step == 3){
		CNCWaterjet.calibTable(2);
		SetDlgItemText(IDC_EDIT_CALIBRATION, _T("已标定第二点，请移动第三点到Y轴正方向上"));
		cali_step = 4;
	}
	else if (cali_step == 4){
		if (!CNCWaterjet.calibTable(3)){
			SetDlgItemText(IDC_EDIT_CALIBRATION, _T("三点标定完成"));

		}
		else{
			SetDlgItemText(IDC_EDIT_CALIBRATION, _T("三点标定失败，请重新标定"));

		}
		cali_step = 1;
	}
}

UINT CChildDlgB3::HomeJointCommandThread(LPVOID lpParam)
{
	CChildDlgB3* p = (CChildDlgB3*)lpParam;
	p->HomeJointCommandFunc();
	return 0;
}

void CChildDlgB3::HomeJointCommandFunc()
{
	CNCWaterjet.setSpeed(CNCmanualmode.jogSpeed, (BUnits)0);

	if (CNCmanualmode.axisnum == AN_X)
		CNCWaterjet.homeJoint((JogJoint)3, 1, 1);
	else if (CNCmanualmode.axisnum == AN_Y)
		CNCWaterjet.homeJoint((JogJoint)2,1, 1);
	else if (CNCmanualmode.axisnum == AN_Z)
		CNCWaterjet.homeJoint((JogJoint)1, 1, 1);
}

void CChildDlgB3::OnBnClickedBtXHome()
{
	CNCWaterjet.setSpeed(1, (BUnits)0);
	SetToManualMode();
	CNCmanualmode.axisnum = AN_X;
	CWinThread* pThread = AfxBeginThread(HomeJointCommandThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	//m_ThreadList.SetAt(pThread->m_hThread, pThread);//当用了CNCInterface.Connect之后，这一行会崩溃，原因是为什么呢？
	pThread->ResumeThread();
}


void CChildDlgB3::OnBnClickedBtYHome()
{
	CNCWaterjet.setSpeed(1, (BUnits)0);
	CNCmanualmode.axisnum = AN_Y;
	CWinThread* pThread = AfxBeginThread(HomeJointCommandThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	//m_ThreadList.SetAt(pThread->m_hThread, pThread);//当用了CNCInterface.Connect之后，这一行会崩溃，原因是为什么呢？
	pThread->ResumeThread();
}


void CChildDlgB3::OnBnClickedBtZHome()
{
	CNCWaterjet.setSpeed(1, (BUnits)0);
	CNCmanualmode.axisnum = AN_Z;
	CWinThread* pThread = AfxBeginThread(HomeJointCommandThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	//m_ThreadList.SetAt(pThread->m_hThread, pThread);//当用了CNCInterface.Connect之后，这一行会崩溃，原因是为什么呢？
	pThread->ResumeThread();
}


LRESULT CChildDlgB3::GetAutoStartMessage(WPARAM w, LPARAM l){
	m_staticdraw = false;

	if ((speed1 == 0) or (speed2 == 0)){
		MessageBox(_T("请输入速度"));
		return -1;
	}
	else{
		CWinThread* pThread = AfxBeginThread(AutoMotionCommandThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		//m_ThreadList.SetAt(pThread->m_hThread, pThread);//当用了CNCInterface.Connect之后，这一行会崩溃，原因是为什么呢？
		pThread->ResumeThread();
		return 0;
	}

}

UINT CChildDlgB3::AutoMotionCommandThread(LPVOID lpParam)
{
	CChildDlgB3* p = (CChildDlgB3*)lpParam;
	p->MotionCommandFunc(lpParam);
	return 0;
}

void CChildDlgB3::MotionCommandFunc(LPVOID lpParam){//开始程序
	m_cwd = "123嵌套封闭环.txt";
	//m_HaveSetManualMode = false;
	const int maxrow = CNCReadTxtData.getMaxRow(m_cwd);//实现对矩阵的各行取最大值，对角线取最大值，并用函数返回指针值
	float** b = new float*[maxrow];   //对于生成二维及更高维的数组，应使用多维指针。
	OpenTXTFileAndGetData(maxrow, b);

	if (CNCRunModeInf.RunMode == m_AutoCutting){
		if (CNC_DO_States.DO69 == false or CNC_DO_States.DO65 == false){
			MessageBox(_T("水泵或主轴泵没开"));
			CNCWaterjet.servoOn(false);
			CNCWaterjet.setMode(0);
			CNCWaterjet.cancel();
			DeleteTXTData(maxrow, b);
			return;
		}
	}

	const float xPii = 3.1415926536f;

	CNCWaterjet.setTableFrameNo(1);

	EPos target;
	target.x = 0;
	target.y = 0;
	target.z = 0;
	target.A = 0;
	target.B = 0;
	target.C = 0;
	target.S = 0;
	target.T = 0;

	EFrameData myframedata;
	myframedata.baseNO = 1;
	myframedata.toolNO = 0;
	myframedata.Torque_state = 0;
	myframedata.Circ_alpha = -1;
	myframedata.IPO_MODE = 3;
	CNCWaterjet.setCurrentFrame(myframedata);
	CNCWaterjet.setTableFrameNo(1);

	EProfile arcprof;
	arcprof.Type = 13; 
	arcprof.MaxVel = 0.4;
	arcprof.MaxAcc = 4;
	arcprof.MaxJerk = 8;
	arcprof.AppDist = 1;
	arcprof.AccTime = 0.1;  //0.1 sec

	CNCWaterjet.servoOn(true);
	CNCWaterjet.setSpeed(50, (BUnits)0);
	CNCWaterjet.line(0, 0, 0, 0.1);//回工件原点
	CNCWaterjet.start();
	while (CNCWaterjet.isExecutingTrajectory()){
		if (CNCRunModeInf.StopRunning == true){
			CNCWaterjet.servoOn(false);
			CNCWaterjet.setMode(0);
			CNCWaterjet.cancel();
			DeleteTXTData(maxrow, b);
			//stopsignal.m_manufacturing = false;
			return;
		}
		SendCNCPos();
		Sleep(50);

	}
	for (int n = 0; n < maxrow; n++)
	{


		if (b[n][0] == 4){
			b_CutColor = false;
			if (CNCRunModeInf.RunMode  == m_AutoCutting){
				TurnOffDO_66_67_68();
			}
			CNCWaterjet.setSpeed(50, (BUnits)0);
			target.x = b[n][3];
			target.y = b[n][4];
			if (CNCWaterjet.line(b[n][3], b[n][4], 0, speed1)){//CNCWaterjet.linesto(target, m_v2 / 60, 1, myframedata)
				MessageBox(_T("空行程运动命令执行失败"));
				//CNCWaterjet.servoOn(false);
				CNCWaterjet.setMode(0);
				CNCWaterjet.cancel();
				TurnOffDO_66_67_68();

				DeleteTXTData(maxrow, b);

				return;
			}
			else
				CNCWaterjet.start();
			while (CNCWaterjet.isExecutingTrajectory()){
				if (CNCRunModeInf.StopRunning == true){
					//CNCWaterjet.servoOn(false);
					CNCWaterjet.setMode(0);
					CNCWaterjet.cancel();
					TurnOffDO_66_67_68();

					DeleteTXTData(maxrow, b);

					//stopsignal.m_manufacturing = false;
					return;
				}
				SendCNCPos();
				Sleep(50);
			}
			
			CNCWaterjet.setSpeed(5, (BUnits)0);
			SendCNCPos();

		}
		else if (b[n][0] == 1){
			b_CutColor = true;

			if (CNCRunModeInf.RunMode  == m_AutoCutting){

				 TurnOnDO_Order( 1);
				
			}
			CNCWaterjet.setSpeed(50, (BUnits)0);
			target.x = b[n][3];
			target.y = b[n][4];
			if (CNCWaterjet.line(b[n][3], b[n][4], 0, speed2)){ //CNCWaterjet.linesto(target, m_v1 / 60, 1, myframedata)
				MessageBox(_T("切割直线运动命令执行失败"));
				TurnOffDO_66_67_68();
				//CNCWaterjet.servoOn(false);
				CNCWaterjet.setMode(0);
				CNCWaterjet.cancel();
				DeleteTXTData(maxrow, b);
				return;
			}
			else
				CNCWaterjet.start();//小心，以前是放在运动命令之后的。
			while (CNCWaterjet.isExecutingTrajectory()){
				if (CNCRunModeInf.StopRunning == true){
					//CNCWaterjet.servoOn(false);
					CNCWaterjet.setMode(0);
					CNCWaterjet.cancel();
					TurnOffDO_66_67_68();
					DeleteTXTData(maxrow, b);
					//stopsignal.m_manufacturing = false;
					return;
				}
				SendCNCPos();
				Sleep(50);
			}
			CNCWaterjet.setSpeed(5, (BUnits)0);
			SendCNCPos();

		}
		else if (b[n][0] == 2){
			b_CutColor = true;

			CNCWaterjet.setSpeed(50, (BUnits)0);
			arcprof.MaxVel = speed2;
			if (CNCRunModeInf.RunMode == m_AutoCutting){
				TurnOnDO_Order(1);
			}
			if (b[n][2] == 360){

				double xm, ym, xe, ye;

				ym = b[n][3] * sin((b[n][1] + b[n][2] / 4)*xPii / 180) + b[n][5];
				xm = b[n][3] * cos((b[n][1] + b[n][2] / 4)*xPii / 180) + b[n][4];
				ye = b[n][3] * sin((b[n][1] + b[n][2] / 2)*xPii / 180) + b[n][5];
				xe = b[n][3] * cos((b[n][1] + b[n][2] / 2)*xPii / 180) + b[n][4];
				EPos arcmid;
				arcmid.x = xm;
				arcmid.y = ym;
				arcmid.z = 0;
				arcmid.A = 0;
				arcmid.B = 0;
				arcmid.C = 0;
				arcmid.S = 0;
				arcmid.T = 0;
				EPos arcend;
				arcend.x = xe;
				arcend.y = ye;
				arcend.z = 0;
				arcend.A = 0;
				arcend.B = 0;
				arcend.C = 0;
				arcend.S = 0;
				arcend.T = 0;

				if (CNCWaterjet.arcto(arcmid, arcend, arcprof, myframedata)){
					MessageBox(_T("前半段整圆运动命令执行失败"));
					TurnOffDO_66_67_68();
					CNCWaterjet.servoOn(false);
					CNCWaterjet.setMode(0);
					CNCWaterjet.cancel();
					DeleteTXTData(maxrow, b);
					return;
				}
				else
					CNCWaterjet.start();
				while (CNCWaterjet.isExecutingTrajectory()){
					if (CNCRunModeInf.StopRunning == true){
						TurnOffDO_66_67_68();
						CNCWaterjet.servoOn(false);
						CNCWaterjet.setMode(0);
						CNCWaterjet.cancel();
						DeleteTXTData(maxrow, b);
						//stopsignal.m_manufacturing = false;
						return;
					}
					SendCNCPos();
					Sleep(50);
				};
				ym = b[n][3] * sin((b[n][1] + 3 * b[n][2] / 4)*xPii / 180) + b[n][5];
				xm = b[n][3] * cos((b[n][1] + 3 * b[n][2] / 4)*xPii / 180) + b[n][4];
				ye = b[n][3] * sin((b[n][1] + b[n][2])*xPii / 180) + b[n][5];
				xe = b[n][3] * cos((b[n][1] + b[n][2])*xPii / 180) + b[n][4];
				arcmid.x = xm;
				arcmid.y = ym;
				arcend.x = xe;
				arcend.y = ye;
				if (CNCWaterjet.arcto(arcmid, arcend, arcprof, myframedata)){
					MessageBox(_T("后半段整圆运动命令执行失败"));
					CNCWaterjet.servoOn(false);
					CNCWaterjet.setMode(0);
					CNCWaterjet.cancel();
					TurnOffDO_66_67_68();
					DeleteTXTData(maxrow, b);
					return;
				}
				else
					CNCWaterjet.start();

				while (CNCWaterjet.isExecutingTrajectory()){
					if (CNCRunModeInf.StopRunning== true){
						CNCWaterjet.servoOn(false);
						CNCWaterjet.setMode(0);
						CNCWaterjet.cancel();
						TurnOffDO_66_67_68();
						DeleteTXTData(maxrow, b);

						//stopsignal.m_manufacturing = false;

						return;
					}
					SendCNCPos();
					Sleep(50);

				};
				SendCNCPos();
			}

			else{
				double xm, ym, xe, ye;
				ym = b[n][3] * sin((b[n][1] + b[n][2] / 2)*xPii / 180) + b[n][5];
				xm = b[n][3] * cos((b[n][1] + b[n][2] / 2)*xPii / 180) + b[n][4];
				ye = b[n][3] * sin((b[n][1] + b[n][2])*xPii / 180) + b[n][5];
				xe = b[n][3] * cos((b[n][1] + b[n][2])*xPii / 180) + b[n][4];

				EPos arcmid;
				arcmid.x = xm;
				arcmid.y = ym;
				arcmid.z = 0;
				arcmid.A = 0;
				arcmid.B = 0;
				arcmid.C = 0;
				arcmid.S = 0;
				arcmid.T = 0;

				EPos arcend;
				arcend.x = xe;
				arcend.y = ye;
				arcend.z = 0;
				arcend.A = 0;
				arcend.B = 0;
				arcend.C = 0;
				arcend.S = 0;
				arcend.T = 0;

				if (CNCWaterjet.arcto(arcmid, arcend, arcprof, myframedata)){//如果调用arcto失败则返回
					MessageBox(_T("圆弧命令执行失败"));
					TurnOffDO_66_67_68();
					CNCWaterjet.servoOn(false);
					CNCWaterjet.setMode(0);
					CNCWaterjet.cancel();


					DeleteTXTData(maxrow, b);

					return;
				}
				else
					CNCWaterjet.start();

				while (CNCWaterjet.isExecutingTrajectory()){
					if (CNCRunModeInf.StopRunning == true){
						TurnOffDO_66_67_68();
						CNCWaterjet.servoOn(false);
						CNCWaterjet.setMode(0);
						CNCWaterjet.cancel();
						DeleteTXTData(maxrow, b);
						//stopsignal.m_manufacturing = false;

						return;
					}
					SendCNCPos();
					Sleep(50);

				}
				SendCNCPos();
			}
			CNCWaterjet.setSpeed(5, (BUnits)0);
		}
	}

	TurnOffDO_66_67_68();

	DeleteTXTData( maxrow,  b);
	CNCWaterjet.servoOn(false);//这两个关闭为正常顺序
	CNCWaterjet.stop();
	CNCWaterjet.setMode(0);
	CNCWaterjet.cancel();
	CNCWaterjet.setSpeed(5, (BUnits)0);
}


LRESULT CChildDlgB3::setDOFunc(WPARAM wparam, LPARAM lparam){
	int DOChannel = (int)wparam;
	bool SetCNC_DO = (bool)lparam;

	if (!CNCWaterjet.setDO(DOChannel, SetCNC_DO)){ //使能DO成功时
		HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
		FromHandle(hWnd)->SendMessage(GETDOSTATES, DOChannel, SetCNC_DO);

	}
	else//使能DO失败时
	{
		switch (DOChannel)
		{
		case DO69://DO64主轴泵
		{
			if (SetCNC_DO == true){
				CNC_DO_States.DO69 = false;
				MessageBox(_T("DO69使能失败"));
			}
			else{
				CNC_DO_States.DO69 = true;
				MessageBox(_T("DO69失能失败"));
			}
			break;
		}
		case DO65://DO65水泵
		{
					  if (SetCNC_DO == true){
						  CNC_DO_States.DO65 = false;
						  MessageBox(_T("DO65使能失败"));
					  }
					  else{
						  CNC_DO_States.DO65 = true;
						  MessageBox(_T("DO65失能失败"));
					  }
					  break;
		}
		case DO66://DO66 高压气
		{
					  if (SetCNC_DO == true){
						  CNC_DO_States.DO66 = false;
						  MessageBox(_T("DO69使能失败"));
					  }
					  else{
						  CNC_DO_States.DO66 = true;
						  MessageBox(_T("DO66失能失败"));
					  }
					  break;
		}
		case DO67://DO64沙阀
		{
					  if (SetCNC_DO == true){
						  CNC_DO_States.DO67 = false;
						  MessageBox(_T("DO67使能失败"));
					  }
					  else{
						  CNC_DO_States.DO67 = true;
						  MessageBox(_T("DO67失能失败"));
					  }
					  break;
		}
		case DO68://DO68水阀
		{
					  if (SetCNC_DO == true){
						  CNC_DO_States.DO68 = false;
						  MessageBox(_T("DO68使能失败"));
					  }
					  else{
						  CNC_DO_States.DO68 = true;
						  MessageBox(_T("DO68失能失败"));
					  }
					  break;
		}

		default:
			break;
	}






	}
	return 0;
}

void CChildDlgB3::TurnOffDO_66_67_68(){

	CNCWaterjet.setDO(68, false);//水阀
	CNCWaterjet.setDO(67, false);//砂阀
	CNCWaterjet.setDO(66, false);//高压气阀
	CNC_DO_States.DO68 = false;
	CNC_DO_States.DO67 = false;
	CNC_DO_States.DO66 = false;

	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	FromHandle(hWnd)->SendMessage(GETDOSTATES, 66, 0);
	FromHandle(hWnd)->SendMessage(GETDOSTATES, 67, 0);
	FromHandle(hWnd)->SendMessage(GETDOSTATES, 68, 0);

}

void CChildDlgB3::TurnOnDO_Order(int Sequency){

	switch(Sequency){

	case 1:
	{
		HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄

		CNCWaterjet.setDO(68, 1);//水阀
		FromHandle(hWnd)->SendMessage(GETDOSTATES, 68, 1);

		CNCWaterjet.setDO(67, 1);//砂阀
		FromHandle(hWnd)->SendMessage(GETDOSTATES, 67, 1);

		CNCWaterjet.setDO(66, 1);//高压气阀
		FromHandle(hWnd)->SendMessage(GETDOSTATES, 66, 1);

		CNC_DO_States.DO68 = true;
		CNC_DO_States.DO67 = true;
		CNC_DO_States.DO66 = true;

		break;
	}
	default:
		break;
	}


}

void CChildDlgB3::OpenTXTFileAndGetData(int maxrow,float** b){

	
	if (m_cwd == NULL){
		MessageBox(_T("m_pcwd=null"));
		return;
	}

	const int maxcol = 6;
	for (int i = 0; i < maxrow; i++){
		b[i] = new float[maxcol];
	}

	//这个for语句是用new申请完数组加的一段初始化，若无，则无数据的数组元素位会显示乱码
	for (int i = 0; i < maxrow; i++){
		for (int j = 0; j < maxcol; j++)
			b[i][j] = 0;
	}
	//开始读取txt数据到二维数组b
	if (!CNCReadTxtData.getTxtData(b, maxrow, maxcol, m_cwd)){
		MessageBox(_T("读取轨迹TXT失败"));
	}

}

void CChildDlgB3::DeleteTXTData(int maxrow, float** b){
	CNCWaterjet.setSpeed(5, (BUnits)0);
	CNCRunModeInf.StopRunning = true;
	m_staticdraw = true;
	for (int i = 0; i < maxrow; i++){ delete[]b[i]; }
	delete[]b;
	b_CutColor = false;

}

LRESULT CChildDlgB3::PauseandContinueVisualSimulation(WPARAM wparam, LPARAM lparam){
	


	if (m_staticdraw == true){//如果未进入动态画图，点击无效
		MessageBox(_T("请先启动可视化仿真/动态模拟/实际加工过程"));

	}
	else{
		if (CNCRunModeInf.RunMode == m_AutoWithoutCut){
			if (CNCRunModeInf.ContinueRunning == true){
			CNCWaterjet.suspend(true);
			CNCRunModeInf.ContinueRunning = false;
			MessageBox(_T("动态模拟过程暂停"));

		}
		else{
			CNCWaterjet.suspend(false);
			CNCRunModeInf.ContinueRunning = true;
			MessageBox(_T("动态模拟过程继续"));

		}
		}
		else{
			
			if (CNCRunModeInf.ContinueRunning == true){
				CNCWaterjet.suspend(true);
				CNCRunModeInf.ContinueRunning = false;
				MessageBox(_T("加工过程暂停"));

			}
			else{
				CNCWaterjet.suspend(false);
				CNCRunModeInf.ContinueRunning = true;
				MessageBox(_T("加工过程继续"));

			}
		}
	}



	return 0;

}

void CChildDlgB3::SetToManualMode(){
	//if (m_HaveSetManualMode == true)
	//{
	//	if (CNCRunModeInf.StopRunning == true)
	//	{
	//		CNCWaterjet.setMode(0);
	//		CNCWaterjet.servoOn(true);
	//		m_HaveSetManualMode = true;
	//	}
	//	else{
	//		MessageBox(_T("当前进行可视化、动态模拟或加工模式，请先停止再进入手动模式"));
	//	}

	//}

}




void CChildDlgB3::OnBnClickedButton1()
{
			//CNCWaterjet.setMode(0);
			CNCWaterjet.servoOn(false);
}


void CChildDlgB3::OnBnClickedButton12()//上伺服下伺服
{
	CNCWaterjet.servoOn(true);
}

