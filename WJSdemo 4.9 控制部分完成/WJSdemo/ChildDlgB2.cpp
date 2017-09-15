// ChildDlgB2.cpp : 实现文件
//

#include "stdafx.h"
#include "WJSdemo.h"
#include "ChildDlgB2.h"
#include "afxdialogex.h"
#include "WJSdemoDlg.h"


// CChildDlgB2 对话框

IMPLEMENT_DYNAMIC(CChildDlgB2, CDialogEx)

CChildDlgB2::CChildDlgB2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlgB2::IDD, pParent)
	, m_speed1(0)
	, m_speed2(0)
{

}

CChildDlgB2::~CChildDlgB2()
{
}

void CChildDlgB2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITSPEED1, m_speed1);
	DDX_Text(pDX, IDC_EDITSPEED2, m_speed2);
//	DDX_Control(pDX, IDC_SLIDER1, m_sliderNonCuttingRate);
	//  DDX_Control(pDX, IDC_SLIDER_FEEDRATE, m_slidernoncuttingrate);
	DDX_Control(pDX, IDC_SLIDER_NONCUTTINGRATE, m_SliderNonCuttingRate);
	DDX_Control(pDX, IDC_SLIDER_FEEDRATE, m_SliderFeedRate);
}


BEGIN_MESSAGE_MAP(CChildDlgB2, CDialogEx)
	ON_BN_CLICKED(IDC_VISUALSIMULATION_BTN, &CChildDlgB2::OnBnClickedVisualsimulationBtn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_NONCUTTINGRATE, &CChildDlgB2::OnNMCustomdrawSliderNoncuttingrate)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FEEDRATE, &CChildDlgB2::OnNMCustomdrawSliderFeedrate)

	
	ON_BN_CLICKED(IDC_BTN_DYNAMICSIMULATION, &CChildDlgB2::OnBnClickedBtnDynamicsimulation)
	ON_BN_CLICKED(IDC_BTN_PROCESS, &CChildDlgB2::OnBnClickedBtnProcess)
END_MESSAGE_MAP()


// CChildDlgB2 消息处理程序

BOOL CChildDlgB2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_SliderFeedRate.SetRange(1, 120);//设置slider的范围

	m_SliderFeedRate.SetTicFreq(5);//设置slider的刻度间隔，这里设置为1个单位一个刻度

	m_SliderFeedRate.SetPos(100);//设置slider的值，这里把初始值设为100

	m_SliderNonCuttingRate.SetRange(1, 120);//设置slider的范围

	m_SliderNonCuttingRate.SetTicFreq(5);//设置slider的刻度间隔，这里设置为1个单位一个刻度

	m_SliderNonCuttingRate.SetPos(100);//设置slider的值，这里把初始值设为100

	HBITMAP hBmp1 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_VISUALIZEDSIMULATION));
	((CButton *)GetDlgItem(IDC_VISUALSIMULATION_BTN))->SetBitmap(hBmp1);

	HBITMAP hBmp2 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_DYNAMICSIMULATION));
	((CButton *)GetDlgItem(IDC_BTN_DYNAMICSIMULATION))->SetBitmap(hBmp2);

	HBITMAP hBmp3 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_PROCESS));
	((CButton *)GetDlgItem(IDC_BTN_PROCESS))->SetBitmap(hBmp3);

	SetWindowText(_T("自动"));


	return TRUE;
}






void CChildDlgB2::OnNMCustomdrawSliderNoncuttingrate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int pos = m_SliderNonCuttingRate.GetPos();//获取slider的当前位置

	CString str;//定义一个字符串变量

	str.Format(_T("%d"), pos);//把slider的当前位置pos转为字符，存放于str中。

	SetDlgItemText(IDC_STATIC_NONCUTTINGRATE, str);//设置ID为IDC_STATIC_B_status的文本框的内容
	float* speed = new float;
	
	UpdateData(TRUE);
	*speed = (float)60000 / (pos * m_speed1 / 100);

	UpdateData(FALSE);	
	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	FromHandle(hWnd)->SendMessage(UPDATENONCUTTINGSTROCKRATE, (WPARAM)(speed), 0);
	delete speed;
	*pResult = 0;
}


void CChildDlgB2::OnNMCustomdrawSliderFeedrate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int pos =m_SliderFeedRate.GetPos();//获取slider的当前位置

	CString str;//定义一个字符串变量

	str.Format(_T("%d"), pos);//把slider的当前位置pos转为字符，存放于str中。

	SetDlgItemText(IDC_STATIC_FEEDRATE, str);//设置ID为IDC_STATIC_B_status的文本框的内容
	float* speed = new float;

	UpdateData(TRUE);
	*speed = (float)60000 / (pos * m_speed2 / 100);

	UpdateData(FALSE);
	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	FromHandle(hWnd)->SendMessage(UPDATEFEEDRATE, (WPARAM)(speed), 0);
	delete speed;
	*pResult = 0;
}


void CChildDlgB2::OnBnClickedVisualsimulationBtn()
{
	//////////////////先判断是否运行，如果运行则不能进行下面，提示先停止或等待结束
	//////////////////加入确定对话框，确定后执行以下

	if (CNCRunModeInf.StopRunning == true){
		DeleteFile(_T("123嵌套封闭环.txt"));

		CNCRunModeInf = { m_VisualSimulation, false, true };

		SpeedStruct* sendspeed = (SpeedStruct *)malloc(sizeof(SpeedStruct));
		UpdateData(TRUE);
		sendspeed->speed1 = (float)60000 / m_speed1;
		sendspeed->speed2 = (float)60000 / m_speed2;
		UpdateData(FALSE);
		//CNCRunModeInf.StopRunning = false;//如果没有这一行，当在没有启动可视化仿真前，若连续两次点击停止按钮，也必须点击两次可视化仿真才可
		//以进行动态画图。因为未进入动态画图而停止后，stopsignal.m_visualsimulation的值不会变为false。
		HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
		hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		FromHandle(hWnd)->SendMessage(VISUALSIMULATION, (WPARAM)(sendspeed), 0);
		free(sendspeed);
	}
	else StartErrorWarning();
}



void CChildDlgB2::OnBnClickedBtnDynamicsimulation()
{
	//////////////////先判断是否运行，如果运行则不能进行下面，提示先停止或等待结束
	//////////////////加入确定对话框，确定后执行以下

	if (CNCRunModeInf.StopRunning == true){
		DeleteFile(_T("123嵌套封闭环.txt"));
	CNCRunModeInf = { m_AutoWithoutCut, false, true };
	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	FromHandle(hWnd)->SendMessage(BEFORE_AUTORUN_REDRAW, 0,0);
	//UpdateData(TRUE);//设置原始速度为5000，空切速度3000，进给速度800
	//speed1 = m_speed1 / (0.5 * 60 * 1000);// m_feedspeed/(setspeed * 1000mm * 60 sec)= m_v1 
	//speed2 = m_speed2 / (0.5 * 60 * 1000);
	//UpdateData(FALSE);
	CString str1,str2;
	GetDlgItemText(IDC_EDITSPEED1, str1);//这种方式防止在多线程（或者是PrestranslateMessage）状况下，文本框输入的数据丢失，变为0。Updatedata不好用
	GetDlgItemText(IDC_EDITSPEED2, str2);
	speed1 = _ttof(str1) / (0.5 * 60 * 1000);
	speed2 = _ttof(str2) / (0.5 * 60 * 1000);


	if ((speed1 == 0) or(speed2 == 0)){
		MessageBox(_T("请输入速度"));
		CNCRunModeInf.StopRunning = true;
		return;
	}
	if (speed1 > 0.166 or speed2 > 0.166){
		MessageBox(_T("速度不能大于5000mm/min"));
		CNCRunModeInf.StopRunning = true;
		return;
	}
	else{

		HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
		hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB2)->m_hWnd, NULL, NULL, _T("手动"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		FromHandle(hWnd)->SendMessage(SEND_AUTO_START_MESSAGE, 0, 0);
	}
	}
	else StartErrorWarning();
}



void CChildDlgB2::OnBnClickedBtnProcess()
{
	//////////////////先判断是否运行，如果运行则不能进行下面，提示先停止或等待结束
	//////////////////加入确定对话框，确定后执行以下
	//stopsignal = { false, false, false };
	//全局变量，暂停或继续信号。暂时将不开刀运行和开刀运行的暂停信号合并，因为其程序为同一个
	//pausesignal = { true, true, true };
	if (CNCRunModeInf.StopRunning == true){
		DeleteFile(_T("123嵌套封闭环.txt"));
	CNCRunModeInf = { m_AutoCutting, false, true };
	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	FromHandle(hWnd)->SendMessage(BEFORE_AUTORUN_REDRAW, 0, 0);
	//UpdateData(TRUE);//设置原始速度为5000，空切速度3000，进给速度800
	//speed1 = m_speed1 / (0.5 * 60 * 1000);// m_feedspeed/(setspeed * 1000mm * 60 sec)= m_v1 
	//speed2 = m_speed2 / (0.5 * 60 * 1000);
	//UpdateData(FALSE);
	CString str1, str2;
	GetDlgItemText(IDC_EDITSPEED1, str1);//这种方式防止在多线程（或者是PrestranslateMessage）状况下，文本框输入的数据丢失，变为0。Updatedata不好用
	GetDlgItemText(IDC_EDITSPEED2, str2);
	speed1 = _ttof(str1) / (0.5 * 60 * 1000);
	speed2 = _ttof(str2) / (0.5 * 60 * 1000);


	if ((speed1 == 0) or(speed2 == 0)){
		MessageBox(_T("请输入速度"));
		CNCRunModeInf.StopRunning = true;
		return;
	}
	if (speed1 > 0.166 or speed2 > 0.166){
		MessageBox(_T("速度不能大于5000mm/min"));
		CNCRunModeInf.StopRunning = true;
		return;
	}
	else{

		HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
		hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB2)->m_hWnd, NULL, NULL, _T("手动"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		FromHandle(hWnd)->SendMessage(SEND_AUTO_START_MESSAGE, 0, 0);
	}
	}
	else StartErrorWarning();
}

void CChildDlgB2::StartErrorWarning(){
	switch (CNCRunModeInf.RunMode){
	case m_VisualSimulation:
		MessageBox(_T("正在进行可视化模拟程序，请先停止或待其介绍再执行其他自动化程序"));
		break;
	case m_AutoCutting:
		MessageBox(_T("正在进行实际加工程序，请先停止或待其介绍再执行其他自动化程序"));
		break;
	case m_AutoWithoutCut:
		MessageBox(_T("正在进行动态模拟程序，请先停止或待其介绍再执行其他自动化程序"));
		break;
	default:
		break;

	}
}

