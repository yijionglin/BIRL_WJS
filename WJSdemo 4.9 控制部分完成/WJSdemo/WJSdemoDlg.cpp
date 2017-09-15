
// WJSdemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WJSdemo.h"
#include "WJSdemoDlg.h"
#include "afxdialogex.h"
#include "io.h" 
#include "fcntl.h"
#include"stdio.h"
#include"afxdlgs.h"//for open file
#include "iostream"
#include "fstream"
#include "string"
#include "sstream"
#include "ostream"
#include "ChildDlgB1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
using namespace std;

////开始模式：绘图，手动，自动，自动不开刀
//StartMode WJAutoStartMode = m_VisualSimulation;
////全局变量，停止信号
//struct StopSignal stopsignal = {false,false,false};
////全局变量，暂停或继续信号。暂时将不开刀运行和开刀运行的暂停信号合并，因为其程序为同一个
//struct PauseSignal pausesignal = { true, true, true };

//struct RunModeInf{
//	StartMode RunMode;
//	bool StopRunning;
//	bool ContinueRunning;
//
//};
struct RunModeInf CNCRunModeInf = { m_non, true, true };
//全局变量，动静态画图判断量
bool m_staticdraw = true;
bool m_FirstLeadingLineLengthSetting = true;

//全局变量，设置切割引导线类型
struct CoordinateStruct BaseframeData = {0,0,0};
struct CoordinateStruct BaseframeOrigin = { 0, 0, 0 };
struct CoordinateStruct CNC_Pos_Origin = { 0, 0, 0 };
struct CoordinateStruct CNC_Pos = { 0, 0, 0 };

//切割引导线类型
m_LeadLineType LeadlineType=LL_NoType;
//全局变量，设置切割引导线长度
double m_LeadingLineLength = 0;

//全局，DO信息
struct DOStates CNC_DO_States = { false, false, false, false, false,false };
enum DO_Num CNC_DO_Num = { DO64 };


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWJSdemoDlg 对话框



CWJSdemoDlg::CWJSdemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWJSdemoDlg::IDD, pParent)//构造函数
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWJSdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_taba);
	DDX_Control(pDX, IDC_TAB2, m_tabb);
}

BEGIN_MESSAGE_MAP(CWJSdemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CWJSdemoDlg::OnTcnSelchangeTab1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CWJSdemoDlg::OnTcnSelchangeTab2)
	ON_MESSAGE(PROCEEDDXFDATA, CWJSdemoDlg::ProceedDXFData)
	ON_MESSAGE(UPDATE_CNCPOSITION, CWJSdemoDlg::GetCNCPositionData)

	ON_BN_CLICKED(IDC_PAUSEORCONTINUE, &CWJSdemoDlg::OnBnClickedPauseorcontinue)
	ON_BN_CLICKED(IDC_STOP, &CWJSdemoDlg::OnBnClickedStop)
//	ON_BN_CLICKED(IDC_RADIO2, &CWJSdemoDlg::OnBnClickedRadio2)
ON_BN_CLICKED(IDC_BTN_GASVALVE, &CWJSdemoDlg::OnBnClickedBtnGasvalve)
ON_BN_CLICKED(IDC_BTN_WATERVALVE, &CWJSdemoDlg::OnBnClickedBtnWatervalve)
ON_BN_CLICKED(IDC_BTN_WATERPUMP, &CWJSdemoDlg::OnBnClickedBtnWaterpump)
ON_BN_CLICKED(IDC_BTN_SANDVALVE, &CWJSdemoDlg::OnBnClickedBtnSandvalve)
ON_BN_CLICKED(IDC_BTN_MAINPUMP, &CWJSdemoDlg::OnBnClickedBtnMainpump)
ON_MESSAGE(GETDOSTATES, CWJSdemoDlg::getDOStatesFunc)//获取DO口信息
ON_MESSAGE(UPDATE_BASE_ORIGIN, CWJSdemoDlg::updateBaseOriginFunc)
END_MESSAGE_MAP()


// CWJSdemoDlg 消息处理程序

BOOL CWJSdemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	

	SetWindowText(_T("超高压水射流切割智能控制软件"));
	CRect tabaRect, tabbRect;   // 标签控件客户区的位置和大小   

	m_taba.InsertItem(0, _T("位置"));  // 插入A组第一个标签“位置”  
	m_taba.InsertItem(1, _T("程序"));  // 插入A组第二个标签“程序”   
	m_taba.InsertItem(2, _T("参数"));  // 插入A组第二个标签“参数”  
	m_taba.InsertItem(3, _T("诊断"));  // 插入A组第四个标签“诊断” 

	m_tabb.InsertItem(0, _T("文件处理"));  // 插入B组第一个标签“位置”  
	m_tabb.InsertItem(1, _T("自动"));  // 插入B组第二个标签“程序”   
	m_tabb.InsertItem(2, _T("手动"));  // 插入B组第二个标签“参数”  
	m_tabb.InsertItem(3, _T("连接"));  // 插入B组第二个标签“参数”  

	m_ChildDlgA1.Create(IDD_CHILDDLG_A1, &m_taba);    // 创建A组第一个标签页，m_ChildDlgA1为对话框的类对象，通过该对象可以调用Create函数来创建标签页，第一个参数为对话框ID，第二个为Tab控件的关联变量。
	m_ChildDlgA2.Create(IDD_CHILDDLG_A2, &m_taba); // 创建A组第二个标签页  
	m_ChildDlgA3.Create(IDD_CHILDDLG_A3, &m_taba); // 创建A组第三个标签页  
	m_ChildDlgA4.Create(IDD_CHILDDLG_A4, &m_taba); // 创建A组第四个标签页  
	m_ChildDlgB1.Create(IDD_CHILDDLG_B1, &m_tabb);    // 创建B组第一个标签页，m_ChildDlgB1为对话框的类对象，通过该对象可以调用Create函数来创建标签页，第一个参数为对话框ID，第二个为Tab控件的关联变量。
	m_ChildDlgB2.Create(IDD_CHILDDLG_B2, &m_tabb); // 创建B组第二个标签页  
	m_ChildDlgB3.Create(IDD_CHILDDLG_B3, &m_tabb); // 创建B组第三个标签页  
	m_ChildDlgB4.Create(IDD_CHILDDLG_B4, &m_tabb); // 创建B组第三个标签页  


	m_taba.GetClientRect(&tabaRect); // 获取标签控件客户区大小Rect，并传到tabRect中。
	// 调整tabRect，使其覆盖范围适合放置标签页   
	tabaRect.left += 1;
	tabaRect.right -= 1;
	tabaRect.top += 25;
	tabaRect.bottom -= 1;
	// 根据调整好的 tabaRect放置m_ChildDlgA1子对话框，并设置为显示   
	m_ChildDlgA1.SetWindowPos(NULL, tabaRect.left, tabaRect.top, tabaRect.Width(), tabaRect.Height(), SWP_SHOWWINDOW);
	// m_ChildDlgA2子对话框置为隐藏   
	m_ChildDlgA2.SetWindowPos(NULL,  tabaRect.left,  tabaRect.top,  tabaRect.Width(),  tabaRect.Height(), SWP_HIDEWINDOW);
	//m_ChildDlgA3子对话框设置为隐藏   
	m_ChildDlgA3.SetWindowPos(NULL,  tabaRect.left,  tabaRect.top,  tabaRect.Width(),  tabaRect.Height(), SWP_HIDEWINDOW);
	//m_ChildDlgA4子对话框设置为隐藏   
	m_ChildDlgA4.SetWindowPos(NULL, tabaRect.left, tabaRect.top, tabaRect.Width(), tabaRect.Height(), SWP_HIDEWINDOW);

	m_tabb.GetClientRect(&tabbRect);
	 tabbRect.left += 1;
	 tabbRect.right -= 1;
	 tabbRect.top += 25;
	 tabbRect.bottom -= 1;
	m_ChildDlgB1.SetWindowPos(NULL,  tabbRect.left,  tabbRect.top,  tabbRect.Width(),  tabbRect.Height(), SWP_SHOWWINDOW);
	// m_ChildDlgA2子对话框置为隐藏   
	m_ChildDlgB2.SetWindowPos(NULL,  tabbRect.left,  tabbRect.top,  tabbRect.Width(),  tabbRect.Height(), SWP_HIDEWINDOW);
	//m_ChildDlgA3子对话框设置为隐藏   
	m_ChildDlgB3.SetWindowPos(NULL,  tabbRect.left,  tabbRect.top,  tabbRect.Width(),  tabbRect.Height(), SWP_HIDEWINDOW);
	//m_ChildDlgA3子对话框设置为隐藏   
	m_ChildDlgB4.SetWindowPos(NULL,  tabbRect.left,  tabbRect.top,  tabbRect.Width(),  tabbRect.Height(), SWP_HIDEWINDOW);
	///////////以下对DXF处理过程的相应变量作初始化///////////////////
	BatchHEAD_ID = 0;//初始化时候，还没有批次
	NestResult_ID = 0;//初始化，还没有图纸导入
	GEOMCLOSE_ID = 0;//初始化，还没有封闭环
	m_pBatchHead = NULL;//未指向任何地方
	m_pNestrsltdtND = NULL;//每一个切割平面图（800*800）的信息。包括了所有封闭环
	m_pGeomclsHead = NULL;
	m_IfDataDisposed = false;
	m_NewDxf = true;//新图纸

	HBITMAP hBmp1 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_STOP));
	((CButton *)GetDlgItem(IDC_STOP))->SetBitmap(hBmp1);

	HBITMAP hBmp2 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_PAUSE));
	((CButton *)GetDlgItem(IDC_PAUSEORCONTINUE))->SetBitmap(hBmp2);

	HBITMAP hBmp3 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_WATERPUMP));
	((CButton *)GetDlgItem(IDC_BTN_WATERPUMP))->SetBitmap(hBmp3);

	HBITMAP hBmp4 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_SANDVALVE));
	((CButton *)GetDlgItem(IDC_BTN_SANDVALVE))->SetBitmap(hBmp4);

	HBITMAP hBmp5 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_MAINPUMP));
	((CButton *)GetDlgItem(IDC_BTN_MAINPUMP))->SetBitmap(hBmp5);

	HBITMAP hBmp6 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_GASVALVE));
	((CButton *)GetDlgItem(IDC_BTN_GASVALVE))->SetBitmap(hBmp6);

	HBITMAP hBmp7 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_WATERVALVE));
	((CButton *)GetDlgItem(IDC_BTN_WATERVALVE))->SetBitmap(hBmp7);



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWJSdemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWJSdemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWJSdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWJSdemoDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here   
	*pResult = 0;
	CRect tabRect;    // 标签控件客户区的Rect   

	// 获取标签控件客户区Rect，并对其调整，以适合放置标签页   
	m_taba.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	switch (m_taba.GetCurSel())//根据TabControl标签控件的当前选择来显示标签页
	{
		// 如果标签控件当前选择标签为“位置”，则显示m_ChildDlgA1对话框，隐藏其余对话框   
	case 0:
	{m_ChildDlgA1.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_ChildDlgA2.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA3.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA4.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
		hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		FromHandle(hWnd)->SendMessage(TABA1GLFLUSH,0, 0);//
	}
		break;
	case 1:
		m_ChildDlgA1.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA2.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_ChildDlgA3.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA4.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 2:
		m_ChildDlgA1.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA2.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA3.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_ChildDlgA4.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 3:
		m_ChildDlgA1.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA2.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA3.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgA4.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		break;
	default:
		break;
	}
}


void CWJSdemoDlg::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here   
	*pResult = 0;
	CRect tabRect;    // 标签控件客户区的Rect   

	// 获取标签控件客户区Rect，并对其调整，以适合放置标签页   
	m_tabb.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	switch (m_tabb.GetCurSel())//根据TabControl标签控件的当前选择来显示标签页
	{
		// 如果标签控件当前选择标签为“位置”，则显示m_ChildDlgB1对话框，隐藏其余对话框   
	case 0:
		m_ChildDlgB1.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_ChildDlgB2.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgB3.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgB4.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);

		break;
	case 1:
		m_ChildDlgB1.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgB2.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_ChildDlgB3.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgB4.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);

		break;
	case 2:
		m_ChildDlgB1.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgB2.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgB3.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_ChildDlgB4.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 3:
		m_ChildDlgB1.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgB2.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgB3.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_ChildDlgB4.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);

		break;
	default:
		break;
	}
}

LRESULT CWJSdemoDlg::ProceedDXFData(WPARAM wparam, LPARAM lparam)  //消息函数，对DXF进行处理并得到BatchHead用来遍历
{
	BatchHEAD_ID++;//启动生产，那么产品批次就是一次
	m_pBatchHead = m_GeomForCut.CreatBatchHEAD(BatchHEAD_ID);//输入批次进行初始化调用


	//打开图纸导入图纸结点
	m_NewDxf = true;//新图纸
	NestResult_ID++;//排样结果图纸编号
	m_pNestrsltdtND = m_GeomForCut.CreatNestResultNode(NestResult_ID);//在初始化时候记录这是打开的第几张图纸
	m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestrsltdtND);//把新生成的结点挂在生产批次指向的图纸双向链表中
	//生成一个只存放圆的排样结果图结点
	m_pNestNode_forCircle = m_GeomForCut.CreatNestResultNode(NestResult_ID);//输入上一张图纸的ID，因为这里只是先划一块区域来挂圆，ID不需要改变
	//把只存放圆的排样结果图结点挂到生产批次结点上
	m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestNode_forCircle);//新的图纸双向链表结点挂在同一生产批次F头结点上

	//创建记录图元结点的F头结点
	GEOMCLOSE_ID++;
	m_pGeomclsHead = m_GeomClose.CreatGeomCloseHEAD(GEOMCLOSE_ID);
	//把封闭环结点挂到排样结果图结点上，即将同一排样图中的所有图元串成一个链表
	m_pNestrsltdtND = m_GeomForCut.AddGeomCloseHeadNode(m_pNestrsltdtND, m_pGeomclsHead);//输入指向封闭环结点的排样dxf结果图结点，和封闭环结点，把封闭环结点挂到排样dxf结果图上，这里已经包含了三层结构

	//
	OPENFILENAME ofn = { 0 };//作为打开文件的特定风格
	//bOpenFileDialog
	//	TRUE则显示打开文件对话框，FALSE则显示保存文件对话框。
	//	lpszDefExt
	//	指定默认的文件扩展名。
	//	lpszFileName
	//	指定默认的文件名。
	//	dwFlags
	//	指明一些特定风格。
	//	lpszFilter
	//	是最重要的一个参数，它指明可供选择的文件类型和相应的扩展名。
	//	pParentWnd
	//	为父窗口指针。
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
	CFileDialog m_dlg(TRUE, NULL, NULL, ofn.Flags, TEXT("DXF(*.*)|*.*|"), NULL);
	//以后可以做一个批量打开DXF文件，进行自动加工

	/*CFileDialog m_dlg(true);
	m_dlg.m_ofn.nMaxFile = 511;
	m_dlg.m_ofn.lpstrFilter = "DXF Files(*.dxf)\0*.dxf\0All Files(*.*)\0*.*\0\0";
	m_dlg.m_ofn.lpstrTitle = "Open DXF Files";
	CString m_filename;
	*/

	if (m_dlg.DoModal() == IDOK)
	{
		path = m_dlg.GetPathName();
		switchkeyword(path);
		//以上已经把排样结果DXF里面的数据全部读取了，接下来要对数据进行：封闭环分开挂到不同的封闭环头结点上
		AdjustGeomCloseNode(m_pNestrsltdtND);
		//将处理好的DXF数据传递到A11 Dlg中进行读取和画图
		DXFProcData* SendDXFProcMsgtoA111 = (DXFProcData *)malloc(sizeof(DXFProcData));

		SendDXFProcMsgtoA111->BatchHEAD_ID = BatchHEAD_ID;
		SendDXFProcMsgtoA111->GEOMCLOSE_ID = GEOMCLOSE_ID;
		SendDXFProcMsgtoA111->NestResult_ID = NestResult_ID;
		SendDXFProcMsgtoA111->m_IfDataDisposed = m_IfDataDisposed;
		SendDXFProcMsgtoA111->m_NewDxf = m_NewDxf;
		SendDXFProcMsgtoA111->m_pBatchHead = m_pBatchHead;
		SendDXFProcMsgtoA111->m_pNestrsltdtND = m_pNestrsltdtND;
		SendDXFProcMsgtoA111->m_pGeomclsHead = m_pGeomclsHead;

		HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
		hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		FromHandle(hWnd)->SendMessage(GET_DXFDATA, (WPARAM)(SendDXFProcMsgtoA111), 0);//
		free(SendDXFProcMsgtoA111);


		int len = WideCharToMultiByte(CP_ACP, 0, m_dlg.GetPathName(), -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
		char* cwd = new char[len + 1];
		WideCharToMultiByte(CP_ACP, 0, m_dlg.GetPathName(), -1, cwd, len, NULL, NULL);

		HWND hWnd2 = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
		hWnd2 = ::FindWindowEx(FromHandle(hWnd2)->GetDlgItem(IDC_TAB2)->m_hWnd, NULL, NULL, _T("文件处理"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		FromHandle(hWnd2)->SendMessage(GET_FILEPATH, (WPARAM)(cwd), 0);//


		return 0;
	}

	else{
		return -1;
	}

}



//按照打开的文件名路径去搜索LINE ARC CIRCLE
void CWJSdemoDlg::switchkeyword(CString path)
{
	//bool a=false;
	CStdioFile m_dxfofnestresult(path, CFile::modeRead);//输入文件路径，只读
	//a=m_dxfofnestresult.ReadString(m_readgeomele);//每次读一行，读取一次之后，指向下一行，放回bool
	while (m_dxfofnestresult.ReadString(m_readgeomele))//如果不是空文件（//m_readgeomele存储读取到的dxf的数据）
		//while (a)
	{
		//如果是LINE ARC CIRCLE这三个关键字之间的一个
		//a = m_dxfofnestresult.ReadString(m_readgeomele);
		int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
		m_readgeomeleUnicode = new char[len + 1];
		WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
		while ((strcmp(m_readgeomeleUnicode, "LINE") == 0) || (strcmp(m_readgeomeleUnicode, "ARC") == 0) || (strcmp(m_readgeomeleUnicode, "CIRCLE") == 0))
		{
			if (strcmp(m_readgeomeleUnicode, "LINE") == 0) m_typegeomele = 0;
			if (strcmp(m_readgeomeleUnicode, "ARC") == 0) m_typegeomele = 1;
			if (strcmp(m_readgeomeleUnicode, "CIRCLE") == 0) m_typegeomele = 2;
			switch (m_typegeomele)
			{
			case LINE://查到的关键字为LINE的时候，这事只是读到了LINE，但要往下走
			{m_dxfofnestresult.ReadString(m_readgeomele);//如果这里不读下一行，那么还是LINE，那么就进不去下面的while，且一直都是LINE,还会在上一个while形成死循环。
			int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
			m_readgeomeleUnicode = new char[len + 1];
			WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
			//每进入一次LINE之后除非遇到了下一个LINE，ARC,CIRCLE否则不能让它跳出这个LINE循环
			//从外面往里只有不是LINE ,ARC,CIRCLE，ENDSEC才能进入，从里则遇到LINE ,ARC,CIRCLE，ENDSEC才能出来（把LINE ,ARC,CIRCLE，ENDSEC）排除出来
			while ((strcmp(m_readgeomeleUnicode, "LINE") != 0) && (strcmp(m_readgeomeleUnicode, "ARC") != 0) && (strcmp(m_readgeomeleUnicode, "CIRCLE") != 0) && (strcmp(m_readgeomeleUnicode, "ENDSEC") != 0))//真是加多了一个(strcmp(m_readgeomele, "ENDSEC") != 0)这个while才能跳出去。
			{
				m_dxfofnestresult.ReadString(m_readgeomele);//将DXF里面的字符串信息读入m_readgeomele
				int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
				m_readgeomeleUnicode = new char[len + 1];
				WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
				symbol = atoi(m_readgeomeleUnicode);//将m_readgeomele里面的字符串转为整形数值symbol
				m_line = AcceptDxfLineData(symbol, m_readgeomele, m_dxfofnestresult);//输入symbol，输出源图元的起止坐标，
			}//离开这里的时候已经循环完一个LINE了
			//结果却是当到了这里的时候m_line为0了，之前的值全部没有保存起来，而且一次次被覆盖，要想个办法把中间的参数保存起来。
			m_geomstanddata = m_geomele.ReadLineData(m_line);//输入源图元的起止坐标，输出一个标准的图元的数据格式
			//创立记录基本图元的双向链表,把获得的数据保存起来
			m_pGeomEleND = m_GeomClose.CreatGeomEleNode(m_geomstanddata);//输入上面获得的数据，创立一个结点保存数据
			m_pGeomclsHead = m_GeomClose.InsertGeomEleNode(m_pGeomclsHead, m_pGeomEleND, m_geomstanddata);//把创立的基本图元结点挂到由m_pGeomclsHead指向的双向链表中。
			break;//跳出cade LINE的事件
			}
			case ARC:
			{m_dxfofnestresult.ReadString(m_readgeomele);
			int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
			m_readgeomeleUnicode = new char[len + 1];
			WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
			while ((strcmp(m_readgeomeleUnicode, "LINE") != 0) && (strcmp(m_readgeomeleUnicode, "ARC") != 0) && (strcmp(m_readgeomeleUnicode, "CIRCLE") != 0) && (strcmp(m_readgeomeleUnicode, "ENDSEC") != 0))//真是加多了一个(strcmp(m_readgeomele, "ENDSEC") != 0)这个while才能跳出去。
			{
				m_dxfofnestresult.ReadString(m_readgeomele);//将DXF里面的字符串信息读入m_readgeomele
				int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
				m_readgeomeleUnicode = new char[len + 1];
				WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
				symbol = atoi(m_readgeomeleUnicode);//将m_readgeomele里面的字符串转为整形数值symbol
				m_arc = AcceptDxfArcData(symbol, m_readgeomele, m_dxfofnestresult);//输入symbol，输出源图元的起止坐标，
			}//离开这里的时候已经循环完一个LINE了
			//结果却是当到了这里的时候m_line为0了，之前的值全部没有保存起来，而且一次次被覆盖，要想个办法把中间的参数保存起来。
			m_geomstanddata = m_geomele.ReadArcData(m_arc);//输入源图元的起止坐标，输出一个标准的图元的数据格式
			//创立记录基本图元的双向链表,把获得的数据保存起来
			m_pGeomEleND = m_GeomClose.CreatGeomEleNode(m_geomstanddata);//输入上面获得的数据，创立一个结点保存数据
			m_pGeomclsHead = m_GeomClose.InsertGeomEleNode(m_pGeomclsHead, m_pGeomEleND, m_geomstanddata);//把创立的基本图元结点挂到由m_pGeomclsHead指向的双向链表中。
			break; }
			case CIRCLE:
			{m_dxfofnestresult.ReadString(m_readgeomele);
			int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
			m_readgeomeleUnicode = new char[len + 1];
			WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
			while ((strcmp(m_readgeomeleUnicode, "LINE") != 0) && (strcmp(m_readgeomeleUnicode, "ARC") != 0) && (strcmp(m_readgeomeleUnicode, "CIRCLE") != 0) && (strcmp(m_readgeomeleUnicode, "ENDSEC") != 0))
			{
				m_dxfofnestresult.ReadString(m_readgeomele);//加不加这行代码取决于读取的数据结构模式，要看能不能加
				int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
				m_readgeomeleUnicode = new char[len + 1];
				WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
				symbol = atoi(m_readgeomeleUnicode);
				m_circle = AcceptDxfCircleData(symbol, m_readgeomele, m_dxfofnestresult);
			}
			m_geomstanddata = m_geomele.ReadCircleData(m_circle);
			//创立记录基本图元的双向链表,把获得的数据保存起来
			//圆的特殊性是因为它本身就是一个封闭环,先让它挂到另一张排样结果图中，读取完数据之后再挂回来
			GEOMCLOSE_ID++;//自己就可以增加一个封闭环
			//////////////////////////////////////////////////////////////////////////////////
			////生成一个排样结果图结点
			//m_pNestNode_forCircle = m_GeomForCut.CreatNestResultNode(NestResult_ID);//输入上一张图纸的ID，因为这里只是先划一块区域来挂圆，ID不需要改变
			////把图结点挂到生产批次结点上
			//m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestNode_forCircle);//新的图纸双向链表结点挂在同一生产批次F头结点上
			////生成一个封闭环结点
			////////////////////////////////////////////////////////////////////////////////////
			m_pGeomclsHead_forCircle = m_GeomClose.CreatGeomCloseHEAD(GEOMCLOSE_ID);
			//把封闭环结点挂到排样结果图结点上
			m_pNestNode_forCircle = m_GeomForCut.AddGeomCloseHeadNode(m_pNestNode_forCircle, m_pGeomclsHead_forCircle);//输入指向封闭环结点的排样dxf结果图结点，和封闭环结点，把封闭环结点挂到排样dxf结果图上，这里已经包含了三层结构
			//生成一个数据结点
			m_pGeomEleND = m_GeomClose.CreatGeomEleNode(m_geomstanddata);//输入上面获得的数据，创立一个结点保存数据
			//把数据结点挂到封闭环结点上
			m_pGeomclsHead_forCircle = m_GeomClose.InsertGeomEleNode(m_pGeomclsHead_forCircle, m_pGeomEleND, m_geomstanddata);//把创立的基本图元结点挂到由m_pGeomclsHead指向的双向链表中。
			break; }
			default:break;//跳出switch (m_typegeomele)事件
			}
			//下面也不应该break跳出，否则，再也进不来了。while是会自循环的，每一次都会去判断下（）里的值。
			//break;//当进来一个关键字之后读完一遍了，则跳出第二个while即判断三个关键字的那个while从第一个while那里去开始第二轮的循环。也可以用m_dxfofnestresult.ReadString(m_readgeomele);让它内部循环但这样就永远跳不出循环了。
		}
		//break;//不应该跳出，否则只是一次就结束了
	}//没有跳出第一个循环的语句，所以一直在内循环指导文件末尾。
}



bool CWJSdemoDlg::AdjustGeomCloseNode(NestResultDataNode*head)
{
	//以上已经把排样结果DXF里面的数据全部读取了，接下来要对数据进行：封闭环分开挂到不同的封闭环头结点上
	int m_GeomCloseID;//处理了的封闭环的个数
	GeomEleNode*temp;
	GeomCloseHEAD *m_FindTheEndClosed, *m_CircleFirstClosed = NULL;//抽出*Htemp,
	int m_MaxNumOfGeomClose, m_MAXNumOfGeomClose;//保存封闭环总数



	temp = head->FirstGeomClose->FirstGeomele;//最原始链表的第一个数据结点
	while (temp->nextGeomeleNode)//找到最原始链表的最后一个数据结点
		temp = temp->nextGeomeleNode;
	m_MaxNumOfGeomClose = temp->m_NumGeomCloseID;//保存封闭环总数（除了圆之外最大的封闭环数）
	GeomCloseHEAD*m_NoIntactGeomCloseHead = NULL;
	for (m_GeomCloseID = 1; m_GeomCloseID <= m_MaxNumOfGeomClose; m_GeomCloseID++)//保证全部结点已经被处理一遍，且给以跳出的条件
	{
		m_pDiffGeomclsDataNode = m_GeomForCut.FindDiffGeomCloseNode(m_pNestrsltdtND);
		m_GeomForCut.InsertGeomCloseHEAD(m_pNestrsltdtND, m_pDiffGeomclsDataNode);//将一个双向链表上的不同封闭环挂到（插入）到不同的双向链表中去
	}
	//对已经分开不同封闭环的数据结点分析其封闭环完整性
	for (m_GeomCloseID = 1; m_GeomCloseID <= m_MaxNumOfGeomClose; m_GeomCloseID++)//保证全部结点已经被处理一遍，且给以跳出的条件
	{
		m_NoIntactGeomCloseHead = m_GeomForCut.JudgeGeomCloseIntact(m_pNestrsltdtND);//判断封闭环是否完整
		m_pNestrsltdtND = m_GeomForCut.Find_Change_GeomCloseHEAD(m_pNestrsltdtND, m_NoIntactGeomCloseHead);//输入不完整的封闭环结点，寻找配对的封闭环结点，并内部做调整
	}
	//再将封闭环清理一遍，将不同的分出去
	for (m_GeomCloseID = 1; m_GeomCloseID <= m_MaxNumOfGeomClose; m_GeomCloseID++)//保证全部结点已经被处理一遍，且给以跳出的条件
	{
		m_pDiffGeomclsDataNode = m_GeomForCut.FindDiffGeomCloseNode(m_pNestrsltdtND);
		m_GeomForCut.InsertGeomCloseHEAD(m_pNestrsltdtND, m_pDiffGeomclsDataNode);
	}
	//对已经分开不同封闭环的数据结点分析其封闭环完整性
	//以上应该是已经保证了封闭环挂在不同的封闭环F头结点指向的双向链表上，并且每一个封闭环结点内的数据结点都是完整的
	//以下还有封闭环之间的排序问题，封闭环之间的过渡线问题，圆往回挂的问题。
	//封闭环内的数据结点处理问题
	//要把圆加入到排样图里面
	if ((m_pNestrsltdtND->nextNestResultDataNode->FirstGeomClose))//如果有圆的排样图
	{
		m_CircleFirstClosed = m_pNestrsltdtND->nextNestResultDataNode->FirstGeomClose;//全部是圆的那个封闭环双向链表的第一个封闭环头结点
		m_FindTheEndClosed = m_pNestrsltdtND->FirstGeomClose;

		while (m_FindTheEndClosed->nextGeomcloseNode)//找到最后一个直线圆弧组成的封闭环
		{
			m_FindTheEndClosed = m_FindTheEndClosed->nextGeomcloseNode;
		}
		m_FindTheEndClosed->nextGeomcloseNode = m_CircleFirstClosed;
		m_CircleFirstClosed->prevGeomcloseNode = m_FindTheEndClosed;
		while (m_CircleFirstClosed->nextGeomcloseNode)//找到圆封闭环的最后一个封闭环
		{
			m_CircleFirstClosed = m_CircleFirstClosed->nextGeomcloseNode;

		}
		m_MAXNumOfGeomClose = m_MaxNumOfGeomClose + m_CircleFirstClosed->GEOMCLOSE_ID;
		m_FindTheEndClosed->GEOMCLOSE_ID = m_MAXNumOfGeomClose;//这样就可以把所有的封闭环数传到最后一个圆封闭环数据里
		//对包含有圆的再处理一遍
		for (m_GeomCloseID = 1; m_GeomCloseID <= m_MAXNumOfGeomClose; m_GeomCloseID++)//保证全部结点已经被处理一遍，且给以跳出的条件
		{
			m_pDiffGeomclsDataNode = m_GeomForCut.FindDiffGeomCloseNode(m_pNestrsltdtND);
			m_GeomForCut.InsertGeomCloseHEAD(m_pNestrsltdtND, m_pDiffGeomclsDataNode);
		}
		m_pNestrsltdtND->nextNestResultDataNode->FirstGeomClose = NULL;//既然圆已经从这个链表头里面退出来了，那么就该让其下面变为表。
	}
	//板材外轮廓不进入规划阶段，所以，这里对于切割引刀线而言是奇外偶里。
	head = m_GeomForCut.ChangeSencondCH2FH(head);//这里先要把第一层板材的去掉，但同时应该是要把第一层板材的数据保存出来的。
	//以上以及将所有的封闭环处理好了。
	//TSP两步法的代码，以下三行
	//m_GeomForCut.ChangeEleNodeOfGeomClosed_origin(m_pNestrsltdtND);//可惜这代码没有起到任何效果，原因之一可能是封闭环本身就没有分清楚，其二是处理的算法有问题。
	//m_GeomForCut.ChangClosedNodeOfNRDXF(m_pNestrsltdtND);
	//m_GeomForCut.ChangeEleNodeOfGeomClosed_order(m_pNestrsltdtND);

	//m_GeomForCut.ChangeEleNode_Avoid_Impact(m_pNestrsltdtND);
	////////////////另一种处理方式//////////////////////////////////
	////先贪婪算法将所有的封闭环按给定初始顺序
	m_GeomForCut.BaseTS_GR_ForCutPathPlan(head);//对于没有嵌套的平面切割图形，dtsp法就用这个。

	//对于有嵌套的封闭环，用以下三个函数（除了ACO那个）
	//划分出不同的封闭环层次
	m_GeomForCut.Find_AdjustNestCloseHead(head);//嵌套封闭环的嵌套识别工作，就这行代码
	//////用蚁群算法对第一层封闭环进行路径规划与优化
	//////////m_GeomForCut.Base_ACO_ForFirstCHead(head);//未成功，但这是另外一篇EI的工作
	m_GeomForCut.BaseTS_GR_ForCutPathPlan(head);
	//  ////根据贪婪算法调整后的第一层封闭环，调整每个封闭环群里面的子封闭环顺序，并调整相应的父封闭环打孔点
	m_GeomForCut.BaseTS_GR_ForKidCHead(head);
	//
	//添加切割引刀线
	//先写入封闭环的奇偶性
	m_GeomForCut.JudgeClosedHead_Odd_even(head);
	//在封闭环奇偶性的确定里封闭环奇偶性后，写入切割引刀线
	m_GeomForCut.Add_CutGuideLine(head);
	//写完切割 引导线之后要进行切割引刀线的判断
	m_GeomForCut.CheckCutGuideLINE(head);
	//head = m_GeomForCut.ChangeSencondCH2FH(head);//这里先要把第一层板材的去掉，但同时应该是要把第一层板材的数据保存出来的。

	m_IfDataDisposed = true;
	return m_IfDataDisposed;
}

//输入dxf里面的符号，输出dxf里面直线的起止点
GLINE CWJSdemoDlg::AcceptDxfLineData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult)//前面加上了CNestDxfDataForCutDlg是使得下面一些m_line可以被使用
{
	//GLINE m_line = {0.0,0.0,0.0,0.0};//每一次进来都初始化一次，初始化不应该在这里

	switch (symbol)//只进来一次
	{
	case 10://起始点x
	{m_dxfofnestresult.ReadString(m_readgeomele);
	int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_readgeomeleUnicode = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
	m_line.x0 = atof(m_readgeomeleUnicode);
	break; }
	case 20://起始点y
	{m_dxfofnestresult.ReadString(m_readgeomele);
	int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_readgeomeleUnicode = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
	m_line.y0 = atof(m_readgeomeleUnicode);
	break; }
	case 11://终止点x
	{m_dxfofnestresult.ReadString(m_readgeomele);
	int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_readgeomeleUnicode = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
	m_line.x1 = atof(m_readgeomeleUnicode);
	break; }
	case 21://终止点y
	{m_dxfofnestresult.ReadString(m_readgeomele);
	int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_readgeomeleUnicode = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
	m_line.y1 = atof(m_readgeomeleUnicode);
	break; }
	default://如果出现了其他情况
		break;
	}
	return m_line;
}
GARC CWJSdemoDlg::AcceptDxfArcData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult)
{

	switch (symbol)
	{
	case 10://圆心x
	{m_dxfofnestresult.ReadString(m_readgeomele);
	int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_readgeomeleUnicode = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
	m_arc.Arccent_x = atof(m_readgeomeleUnicode);
	break; }
	case 20://圆心y
	{m_dxfofnestresult.ReadString(m_readgeomele);
	int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_readgeomeleUnicode = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
	m_arc.Arccent_y = atof(m_readgeomeleUnicode);
	break; }
	case 40://半径r
	{m_dxfofnestresult.ReadString(m_readgeomele);
	int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_readgeomeleUnicode = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
	m_arc.m_Arc_r = atof(m_readgeomeleUnicode);
	break; }
	case 50://起始角
	{m_dxfofnestresult.ReadString(m_readgeomele);
	int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_readgeomeleUnicode = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
	m_arc.m_ArcAngle_start = atof(m_readgeomeleUnicode);
	break; }
	case 51://终止角
	{m_dxfofnestresult.ReadString(m_readgeomele);
	int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_readgeomeleUnicode = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
	m_arc.m_ArcAngle_end = atof(m_readgeomeleUnicode);
	break; }
	default:
		break;
	}
	return m_arc;
}
GCIRCLE CWJSdemoDlg::AcceptDxfCircleData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult)
{
	switch (symbol)
	{
	case 10://圆心x
	{m_dxfofnestresult.ReadString(m_readgeomele);
	int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_readgeomeleUnicode = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
	m_circle.m_Circent_x = atof(m_readgeomeleUnicode);
	break; }
	case 20://圆心y
	{m_dxfofnestresult.ReadString(m_readgeomele);
	int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_readgeomeleUnicode = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
	m_circle.m_Circent_y = atof(m_readgeomeleUnicode);
	break; }
	case 40://半径r
	{m_dxfofnestresult.ReadString(m_readgeomele);
	int len = WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
	m_readgeomeleUnicode = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_readgeomele, -1, m_readgeomeleUnicode, len, NULL, NULL);
	m_circle.m_Circle_r = atof(m_readgeomeleUnicode);
	break; }
	default:
		break;
	}
	return m_circle;
}


void CWJSdemoDlg::OnBnClickedPauseorcontinue()
{
	if (m_staticdraw != true){//如果进行动态绘图。有三种情况
		if (CNCRunModeInf.RunMode == m_VisualSimulation){//第一种，可视化
			if (CNCRunModeInf.ContinueRunning == true){

				HBITMAP hBmpContinue = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_CONTINUE));
				((CButton *)GetDlgItem(IDC_PAUSEORCONTINUE))->SetBitmap(hBmpContinue);
			}
			else{
				HBITMAP hBmpPause = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_PAUSE));
				((CButton *)GetDlgItem(IDC_PAUSEORCONTINUE))->SetBitmap(hBmpPause);
			}

			HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
			hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
			hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
			FromHandle(hWnd)->SendMessage(PAUSEORCONTINUE, 0, 0);//发送停止信息，用于重绘静态图，清空动态图
		}

		else if (CNCRunModeInf.RunMode == m_AutoWithoutCut){
			if (CNCRunModeInf.ContinueRunning == true){

				HBITMAP hBmpContinue = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_CONTINUE));
				((CButton *)GetDlgItem(IDC_PAUSEORCONTINUE))->SetBitmap(hBmpContinue);
			}
			else{
				HBITMAP hBmpPause = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_PAUSE));
				((CButton *)GetDlgItem(IDC_PAUSEORCONTINUE))->SetBitmap(hBmpPause);
			}

			HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
			hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB2)->m_hWnd, NULL, NULL, _T("手动"));//先在父窗口手柄下获得Tab控件，再获得子窗口
			FromHandle(hWnd)->SendMessage(PAUSEORCONTINUE, 0, 0);//发送停止信息，用于重绘静态图，清空动态图
		}
		else if (CNCRunModeInf.RunMode == m_AutoCutting){
			if (CNCRunModeInf.ContinueRunning == true){

				HBITMAP hBmpContinue = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_CONTINUE));
				((CButton *)GetDlgItem(IDC_PAUSEORCONTINUE))->SetBitmap(hBmpContinue);
			}
			else{
				HBITMAP hBmpPause = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_PAUSE));
				((CButton *)GetDlgItem(IDC_PAUSEORCONTINUE))->SetBitmap(hBmpPause);
			}

			HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
			hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB2)->m_hWnd, NULL, NULL, _T("手动"));//先在父窗口手柄下获得Tab控件，再获得子窗口
			FromHandle(hWnd)->SendMessage(PAUSEORCONTINUE,0, 0);//发送停止信息，用于重绘静态图，清空动态图
		}
	}
	else{
		HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
		hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		FromHandle(hWnd)->SendMessage(PAUSEORCONTINUE, 0, 0);//发送停止信息，用于重绘静态图，清空动态图
	}
}


void CWJSdemoDlg::OnBnClickedStop()
{
	if (CNCRunModeInf.RunMode == m_VisualSimulation){
	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	FromHandle(hWnd)->SendMessage(STOPSIGNAL1, 0, 0);//发送停止信息，用于重绘静态图，清空动态图
	}

	else {
		HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
		hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
		FromHandle(hWnd)->SendMessage(STOPSIGNAL1, 0, 0);//发送停止信息，用于重绘静态图，清空动态图
	}


	//stopsignal.m_manufacturing
}

LRESULT CWJSdemoDlg::GetCNCPositionData(WPARAM wparam, LPARAM lparam)  //消息函数，接受来自A11框的坐标信息
{

		EPos * receiveCNC_Pos = (EPos*)wparam;

	CString CX, CY, CZ, SBZ, SBX, SBY;//创建C字符串型变量CX,CY

	CX.Format(_T("%5.1f"), receiveCNC_Pos->x);//调用CString的Format的方法将接受到的坐标信息传入CX
	CEdit* edit2 = (CEdit*)GetDlgItem(IDC_EDIT_MCX);//获得编辑框1的句柄并强制转换为CEdit类型
	edit2->SetWindowText(CX);//调用CEdit中的SetWindowText方法设置编辑框中的内容

	CY.Format(_T("%5.1f"), receiveCNC_Pos->y);
	CEdit* edit3 = (CEdit*)GetDlgItem(IDC_EDIT_MCY);
	edit3->SetWindowText(CY);

	CZ.Format(_T("%5.1f"), receiveCNC_Pos->z);
	CEdit* edit6 = (CEdit*)GetDlgItem(IDC_EDIT_MCZ);
	edit6->SetWindowText(CZ);

	BaseframeData.x = receiveCNC_Pos->x - BaseframeOrigin.x;
	BaseframeData.y = receiveCNC_Pos->y - BaseframeOrigin.y;
	BaseframeData.z = receiveCNC_Pos->z - BaseframeOrigin.z;

	SBX.Format(_T("%5.1f"), BaseframeData.x);//调用CString的Format的方法将接受到的坐标信息传入CX
	CEdit* edit4 = (CEdit*)GetDlgItem(IDC_EDIT_WPCX);//获得编辑框1的句柄并强制转换为CEdit类型
	edit4->SetWindowText(SBX);//调用CEdit中的SetWindowText方法设置编辑框中的内容

	SBY.Format(_T("%5.1f"), BaseframeData.y);
	CEdit* edit5 = (CEdit*)GetDlgItem(IDC_EDIT_WPCY);
	edit5->SetWindowText(SBY);

	SBZ.Format(_T("%5.1f"), BaseframeData.z);
	CEdit* edit7 = (CEdit*)GetDlgItem(IDC_EDIT_WPCZ);
	edit7->SetWindowText(SBZ);

	//HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	//hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	//hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	//FromHandle(hWnd)->SendMessage(CNC_DRAW, 0, 0);//发送停止信息，用于重绘静态图，清空动态图
	return 0;
	
}

LRESULT CWJSdemoDlg::GetVisualizedPositionData(WPARAM wparam, LPARAM lparam)  //消息函数，接受来自A11框的坐标信息
{
	CoordinateStruct* receiveWPC = (CoordinateStruct*)wparam;

	CString CX, CY;//创建C字符串型变量CX,CY
	CX.Format(_T("%5.2f"), receiveWPC->x);//调用CString的Format的方法将接受到的坐标信息传入CX
	CEdit* edit2 = (CEdit*)GetDlgItem(IDC_EDIT_WPCX);//获得编辑框1的句柄并强制转换为CEdit类型
	edit2->SetWindowText(CX);//调用CEdit中的SetWindowText方法设置编辑框中的内容
	CY.Format(_T("%5.2f"), receiveWPC->y);
	CEdit* edit3 = (CEdit*)GetDlgItem(IDC_EDIT_WPCY);
	edit3->SetWindowText(CY);
	//free(receiveWPC);
	return 0;
}
void CWJSdemoDlg::OnBnClickedBtnMainpump()//主轴泵
{
	if (CNC_DO_States.DO69 == false)
		CNC_DO_States.DO69 = true;
	else 
		CNC_DO_States.DO69 = false;

	SendDOMessage(DO69, CNC_DO_States.DO69);


}


void CWJSdemoDlg::OnBnClickedBtnWaterpump()//水泵
{
	if (CNC_DO_States.DO65 == false)
		CNC_DO_States.DO65 = true;
	else
		CNC_DO_States.DO65 = false;

	SendDOMessage(DO65, CNC_DO_States.DO65);
}



void CWJSdemoDlg::OnBnClickedBtnWatervalve()//水阀
{
	if (CNC_DO_States.DO68 == false)
		CNC_DO_States.DO68 = true;
	else
		CNC_DO_States.DO68 = false;

	SendDOMessage(DO68, CNC_DO_States.DO68);
}


void CWJSdemoDlg::OnBnClickedBtnSandvalve()//砂阀
{
	if (CNC_DO_States.DO67 == false)
		CNC_DO_States.DO67 = true;
	else
		CNC_DO_States.DO67 = false;

	SendDOMessage(DO67, CNC_DO_States.DO67);
}


void CWJSdemoDlg::OnBnClickedBtnGasvalve()//高压气阀
{
	if (CNC_DO_States.DO66 == false)
		CNC_DO_States.DO66 = true;
	else
		CNC_DO_States.DO66 = false;

	SendDOMessage(DO66, CNC_DO_States.DO66);
}
int CWJSdemoDlg::SendDOMessage(DO_Num Do_num, bool Do_states){

	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB2)->m_hWnd, NULL, NULL, _T("手动"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	switch (Do_num){
	case DO69:{
				  FromHandle(hWnd)->SendMessage(SETDOSTATES, Do_num, CNC_DO_States.DO69);
				  break;
	}
	case DO65:{
				  FromHandle(hWnd)->SendMessage(SETDOSTATES, Do_num, CNC_DO_States.DO65);
				  break;
	}
	case DO66:{
				  FromHandle(hWnd)->SendMessage(SETDOSTATES, Do_num, CNC_DO_States.DO66);
				  break;
	}
	case DO67:{
				  FromHandle(hWnd)->SendMessage(SETDOSTATES, Do_num, CNC_DO_States.DO67);
				  break;
	}
	case DO68:{
				  FromHandle(hWnd)->SendMessage(SETDOSTATES, Do_num, CNC_DO_States.DO68);
				  break;
	}
	default:
		break;

	}
	return 0;
}
LRESULT CWJSdemoDlg::getDOStatesFunc(WPARAM w,LPARAM y){
	int DOChannel = (int)w;
	int GetCNC_DO = (int)y;

	switch (w){
	case 69:
	{if (GetCNC_DO == 1){
		HBITMAP hBmp5 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_MAINPUMP_GREEN));
			((CButton *)GetDlgItem(IDC_BTN_MAINPUMP))->SetBitmap(hBmp5);
		}
		else{
			HBITMAP hBmp5 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_MAINPUMP));
			((CButton *)GetDlgItem(IDC_BTN_MAINPUMP))->SetBitmap(hBmp5);
		}

		break;
	}
	case 65:
	{if (GetCNC_DO == 1){
		HBITMAP hBmp5 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_WATERPUMP_GREEN));
		((CButton *)GetDlgItem(IDC_BTN_WATERPUMP))->SetBitmap(hBmp5);
	}
	else{
		HBITMAP hBmp5 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_WATERPUMP));
		((CButton *)GetDlgItem(IDC_BTN_WATERPUMP))->SetBitmap(hBmp5);
	}

	break;
	}	
	case 66:
	{if (GetCNC_DO == 1){
		HBITMAP hBmp5 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_GASVALVE_GREEN));
		((CButton *)GetDlgItem(IDC_BTN_GASVALVE))->SetBitmap(hBmp5);
	}
	else{
		HBITMAP hBmp5 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_GASVALVE));
		((CButton *)GetDlgItem(IDC_BTN_GASVALVE))->SetBitmap(hBmp5);
	}

	break;
	}	
	case 67 :
	{if (GetCNC_DO == 1){
		HBITMAP hBmp5 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_SANDVALVE_GREEN));
		((CButton *)GetDlgItem(IDC_BTN_SANDVALVE))->SetBitmap(hBmp5);
	}
	else{
		HBITMAP hBmp5 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_SANDVALVE));
		((CButton *)GetDlgItem(IDC_BTN_SANDVALVE))->SetBitmap(hBmp5);
	}

	break;
	}	
	
	case 68:
	{if (GetCNC_DO == 1){
		HBITMAP hBmp5 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_WATERVALVE_GREEN));
		((CButton *)GetDlgItem(IDC_BTN_WATERVALVE))->SetBitmap(hBmp5);
	}
	else{
		HBITMAP hBmp5 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_WATERVALVE));
		((CButton *)GetDlgItem(IDC_BTN_WATERVALVE))->SetBitmap(hBmp5);
	}

	break;
	}
	default:
		break;
	}
	return 0;
}

LRESULT CWJSdemoDlg::updateBaseOriginFunc(WPARAM w, LPARAM y){
	CString BOX,BOY,BOZ;//创建C字符串型变量CX,CY

	BOX.Format(_T("%5.1f"), BaseframeOrigin.x);//调用CString的Format的方法将接受到的坐标信息传入CX
	CEdit* edit2 = (CEdit*)GetDlgItem(IDC_EDIT_WPOX);//获得编辑框1的句柄并强制转换为CEdit类型
	edit2->SetWindowText(BOX);//调用CEdit中的SetWindowText方法设置编辑框中的内容

	BOY.Format(_T("%5.1f"), BaseframeOrigin.y);
	CEdit* edit3 = (CEdit*)GetDlgItem(IDC_EDIT_WPOY);
	edit3->SetWindowText(BOY);

	BOZ.Format(_T("%5.1f"), BaseframeOrigin.z);
	CEdit* edit6 = (CEdit*)GetDlgItem(IDC_EDIT_WPOZ);
	edit6->SetWindowText(BOZ);

	return 0;
}
