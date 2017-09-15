// ChildDlgA11.cpp : 实现文件
//

#include "stdafx.h"
#include "WJSdemo.h"
#include "ChildDlgA11.h"
#include "afxdialogex.h"
#include "WJSdemoDlg.h"
#include <GL/glut.h>
#include "io.h" 
#include <fstream>//for open file

// CChildDlgA11 对话框
using namespace std;
IMPLEMENT_DYNAMIC(CChildDlgA11, CDialogEx)
//bool CChildDlgA11::m_staticdraw = true;//判断是静态绘图还是动态绘图
 bool CChildDlgA11::m_redraw = false;//判断是否第一次点击动态绘图，还没用到
 
CChildDlgA11::CChildDlgA11(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlgA11::IDD, pParent)
{

}

CChildDlgA11::~CChildDlgA11()
{
}

void CChildDlgA11::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDlgA11, CDialogEx)
	ON_MESSAGE(GET_DXFDATA, CChildDlgA11::recieveDXFProcDataFromMainDlg)
	ON_MESSAGE(TABA1GLFLUSH, CChildDlgA11::TabA1glFlush)
	ON_MESSAGE(CLEARDXFDATA, CChildDlgA11::ClearDXFData)
	ON_MESSAGE(VISUALSIMULATION, CChildDlgA11::GetVisualSimulationMsg)
	ON_MESSAGE(STOPSIGNAL1, CChildDlgA11::StopandRedrawStaticPaint)
	ON_MESSAGE(PAUSEORCONTINUE, CChildDlgA11::PauseandContinueVisualSimulation)
	ON_MESSAGE(UPDATENONCUTTINGSTROCKRATE, CChildDlgA11::UpdateNonCuttingRateFunc)
	ON_MESSAGE(UPDATEFEEDRATE, CChildDlgA11::UpdateFeedRateFunc)
	ON_MESSAGE(BEFORE_AUTORUN_REDRAW, CChildDlgA11::AutoModeRedrawStaticFunc)
	//ON_MESSAGE(CNC_DRAW, CChildDlgA11::CNCDraw)

	

END_MESSAGE_MAP()


// CChildDlgA11 消息处理程序
BOOL CChildDlgA11::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(_T("常规"));
	m_endDynamicDraw = true;
	OpenGLDraw.InitOpenGL(GetDlgItem(IDC_OPENGL));
	DrawX0 = 0;
	DrawY0 = 0;
	pCNCDrawThread = AfxBeginThread(CNCDrawThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	m_VisualDynamicDrawThreadList.SetAt(pCNCDrawThread->m_hThread, pCNCDrawThread);
	pCNCDrawThread->ResumeThread();
	return 0;
	return TRUE;
}

LRESULT CChildDlgA11::recieveDXFProcDataFromMainDlg(WPARAM wparam, LPARAM lparam)
{
	m_staticdraw = true;//静态画图标志位

	//以下变量不会因为在画图过程中遍历所有封闭环或图元时改变。
	m_A11_IfDataDisposed = ((DXFProcData*)wparam)->m_IfDataDisposed;
	m_A11_pNestrsltdtND = ((DXFProcData*)wparam)->m_pNestrsltdtND;
	m_A11_pBatchHead = ((DXFProcData*)wparam)->m_pBatchHead;
	m_A11_pGeomclsHead = ((DXFProcData*)wparam)->m_pGeomclsHead;
	m_A11_NewDxf = ((DXFProcData*)wparam)->m_NewDxf;
	A11_BatchHEAD_ID = ((DXFProcData*)wparam)->BatchHEAD_ID;
	A11_NestResult_ID = ((DXFProcData*)wparam)->NestResult_ID;
	A11_GEOMCLOSE_ID = ((DXFProcData*)wparam)->GEOMCLOSE_ID;
	
	//以下为AfxBeginThread的参数说明
	//参数1  线程的入口函数, 声明一定要如下: UINT MyThreadFunction(LPVOID pParam);
	//参数2 传递入线程的参数, 注意它的类型为:LPVOID, 所以我们可以传递一个结构体入线程.
	//参数3、4、5分别指定线程的优先级、堆栈大小、创建标识、安全属性，含义同用户界面线程。
	//pStaticDrawThread = AfxBeginThread(StaticDrawThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	//m_StaticDrawThreadList.SetAt(pStaticDrawThread->m_hThread, pStaticDrawThread);
	//pStaticDrawThread->ResumeThread();
	m_A11_firstdraw = true;
	ReadDXFandDrawFunc();
	return 0;

}



UINT CChildDlgA11::StaticDrawThread(LPVOID lParam)//不合理，动态绘图时切换后会卡死（切换tab的时候恢复OpenGL要用到这个函数（多线程））
{
	//m_staticdraw = true;
	CChildDlgA11* pDlg = (CChildDlgA11*)lParam;//强制转换类型，将LPVOID装换为COpenGLDlgDlg,才可以用得到pDlg->OnBnClickedButton9();因为调用Button9的对象属于COpenGLDlgDlg类
	pDlg->ReadDXFandDrawFunc();//调用绘图函数，即调用类方法成员。因为该函数为静态函数，因此不能用非静态数据成员，所以必须另外开一个非太静态函数成员来运行非静态数据成员
	return 0;
}

int CChildDlgA11::ReadDXFandDrawFunc()//动静态画图的函数都将在此函数中的A11_ReadCloseHeadData函数调用
{
	////////////////////////////////////以下为将处理好的数据读取出来//////////////////////
	//GeomCloseHEAD*Hkidtemp;
	//GeomEleNode*tempnode;
	bool m_ifHvkidClose;
	//m_A11_NewDxf = true;//这是一张新图纸，第一次进入循环

	//初始化OpenGL
	OpenGLDraw.ClearOpenGL();//清空//当进行在另一个线程的绘图时，这里的清空函数无法情况其他线程的绘图（只能通过主线程情况）
	OpenGLDraw.InitOpenGL(GetDlgItem(IDC_OPENGL));//就算将此行放到OnInitDlg中，也不能在此省略，因为动态绘图时是另一个线程调用，需要另外初始化。
	OpenGLDraw.Reshape();//如果没有此函数，则会出现奇怪现象，视野会改变

	if (m_staticdraw){ 
		OpenGLDraw.InitDraw();//配置背景

	}


	//ofstream outfile("I:\\MATLAB\\DXF\\过渡线01.txt");
	if (m_A11_IfDataDisposed)//数据处理完了，保存才有意义
	{
		//m_pNestrsltdtND = m_GeomForCut.ChangeSencondCH2FH(m_pNestrsltdtND);
		A11_pTheFirstLevelCloseHead = m_A11_pNestrsltdtND->FirstGeomClose;//第一个封闭环F结点

		while (A11_pTheFirstLevelCloseHead)//全部遍历
		{

			//要判断是否有子封闭环， 
			m_ifHvkidClose = m_A11_GeomForCut.IfIncludeKidClose(A11_pTheFirstLevelCloseHead);
			if (m_ifHvkidClose)
			{
				//既然有子封闭环，那么就在子封闭环里把整一个封闭嵌套的所有数据读完
				//包括了父封闭环的数据

				if (A11_ReadKidCloseHeadData(A11_pTheFirstLevelCloseHead) == -1){

					return -1;
				}
					
			}
			else//如果没有子封闭环的话
			{
				if (A11_ReadCloseHeadData(A11_pTheFirstLevelCloseHead) == -1){
					return -1;

				}
			}
			A11_pTheFirstLevelCloseHead = A11_pTheFirstLevelCloseHead->nextGeomcloseNode;
		}
	}
	else
	{
		AfxMessageBox((_T("请等待数据处理完毕！"), MB_YESNO) == IDYES);
	}

	if (m_staticdraw == false){
		m_endDynamicDraw = true;
		CNCRunModeInf.StopRunning = true;
		m_staticdraw = true;
	}
	glFlush();

}


//读取封闭环内部子封闭环的数据
//这个代码是把这个大封闭环的数据从里到外全部读取出来
int CChildDlgA11::A11_ReadKidCloseHeadData(GeomCloseHEAD*pTemp)
{
	GeomCloseHEAD*Htemp, *Hkidtemp, *HFattemp, *pfadtemph;
	GeomCloseHEAD*NextCHtemp;//下一个封闭环头结点
	bool m_ifHvkidClose;
	bool m_ifotherHvkidClose;
	//既然pTemp是有子封闭环的，那么就得先读子封闭环的数据
	Hkidtemp = pTemp->FirstInsideGCloseNode;
	//这个while是要找到最深的一层子封闭环
	while (Hkidtemp->FirstInsideGCloseNode)//找到最深一层；
		Hkidtemp = Hkidtemp->FirstInsideGCloseNode;
	Htemp = Hkidtemp;
	while (Htemp)//把这一层给遍历一遍
	{

		if (Htemp->FirstInsideGCloseNode)//如果兄弟节点有子节点,那么在这个代码就会把子封闭环和父封闭环一起读完
		{

			A11_ReadKidCloseHeadData(Htemp);
		}
		else
		{
			if(A11_ReadCloseHeadData(Htemp))
				return -1;
		}

		Htemp = Htemp->nextGeomcloseNode;
	}


	//上一个循环是已经把最深一层子封闭处理完，那么现在要进行上一层子封闭环
	HFattemp = Hkidtemp->prevOutsideGCloseNode;
	while (HFattemp != pTemp)//当还不是最先的父节点的时候
	{
		if (!(HFattemp))//当HFattemp为NULL 的时候
		{
			break;
		}
		if (A11_ReadCloseHeadData(HFattemp)==-1)
			return -1;

		NextCHtemp = HFattemp->nextGeomcloseNode;//第一个兄弟封闭环结点
		while (NextCHtemp)//全部遍历
		{
			//要判断是否有子封闭环， 
			m_ifotherHvkidClose = m_A11_GeomForCut.IfIncludeKidClose(NextCHtemp);
			if (m_ifotherHvkidClose)
			{
				//既然有子封闭环，那么就在子封闭环里把整一个封闭嵌套的所有数据读完
				//包括了父封闭环的数据
				A11_ReadKidCloseHeadData(NextCHtemp);
			}
			else//如果没有子封闭环的话
			{
				if (A11_ReadCloseHeadData(NextCHtemp)==-1)
					return -1;
			}
			NextCHtemp = NextCHtemp->nextGeomcloseNode;
		}
		HFattemp = HFattemp->prevOutsideGCloseNode;
	}
	//以上已经把除了第一层的父节点之外的所以子节点的数据都处理完了，余下要处理的是第一层的父节点
	if (A11_ReadCloseHeadData(pTemp)==-1)
		return -1;
	//glFlush();

	return 0;
}

//读单一封闭环的数据
int CChildDlgA11::A11_ReadCloseHeadData(GeomCloseHEAD*Htemp)
{
	GeomEleNode*tempnode;
	GeomEleNode*PLastNode = NULL;//保存最后一个封闭环的最后一个图元节点
	int typegeomele;
	ofstream outfile("123嵌套封闭环.txt", ios_base::out | ios_base::app);//打开并追加

	tempnode = Htemp->FirstGeomele;//封闭环里的第一个数据结点
	x1_tran = tempnode->m_GeomStandData.GeoEle_start_x0;
	y1_tran = tempnode->m_GeomStandData.GeoEle_start_y0;
	typegeomele = 4;//空跑直线
	if (m_A11_firstdraw == true)//表明这是新图片
	{
		x0_tran = 0.0;
		y0_tran = 0.0;
	}
	if (m_staticdraw)//判断是否静态绘图
	{
		OpenGLDraw.StaticDrawDottedLine(x0_tran, y0_tran, x1_tran, y1_tran);//静态圆弧绘图命令（空行程）
		outfile << typegeomele << "    " << x0_tran << "    " << y0_tran << "    " << x1_tran << "    " << y1_tran << endl;

	}
	else{
		if(OpenGLDraw.DynamicDrawDottedLine(x0_tran, y0_tran, x1_tran, y1_tran, m_drawspeed1)==-1)//DynamicDraw返回-1时，说明stop
			return -1;
	}
	OpenGLDraw.StaticDrawArrow(x0_tran, y0_tran, x1_tran, y1_tran);//静态圆弧绘图命令（进给行程）

	m_A11_firstdraw = false;//新图纸;//进入一次之后便不再是新图片
	while (tempnode)//全部遍历完
	{
		switch (tempnode->m_GeomStandData.m_typegeomele)
		{
		case 1://直线

			typegeomele = 1;//实切直线
			x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
			x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
			y0 = tempnode->m_GeomStandData.GeoEle_start_y0;
			y1 = tempnode->m_GeomStandData.GeoEle_start_y1;
			if (m_staticdraw){
				OpenGLDraw.StaticDrawLine(x0, y0, x1, y1);//
			outfile << typegeomele << "    " << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;
			}
			else{
				if (OpenGLDraw.DynamicDrawLine(x0, y0, x1, y1, m_drawspeed2)==-1)//DynamicDraw返回-1时，说明stop
				return -1;
			}
				

			break;
		case 2://圆弧，要按照太极控制卡的API输出相应的数据
			typegeomele = 2;//实切圆弧
			Angle_start = tempnode->m_GeomStandData.m_arc.m_ArcAngle_start;
			Angle_end = tempnode->m_GeomStandData.m_arc.m_ArcAngle_end;
			r = tempnode->m_GeomStandData.m_arc.m_Arc_r;
			Arccent_x = tempnode->m_GeomStandData.m_arc.Arccent_x;
			Arccent_y = tempnode->m_GeomStandData.m_arc.Arccent_y;
			if (tempnode->m_GeomStandData.m_IsTranData)//如果将圆弧的起止角度
			{
				Angle_cut_start = Angle_end;
				if (Angle_start < Angle_end)//起始角小于终止角
				{
					Angle_add = Angle_end - Angle_start;
					Angle_add = -Angle_add;//调换了位置则是负的增量角度
				}
				else//起始角大于终止角
				{
					Angle_add = 360 - Angle_start + Angle_end;
					Angle_add = -Angle_add;//调换了位置则是负的增量角度
				}
			}
			else//如果没有调换圆弧的起止角度
			{
				Angle_cut_start = Angle_start;
				if (Angle_start < Angle_end)//起始角小于终止角
				{
					Angle_add = Angle_end - Angle_start;
				}
				else//起始角大于终止角
				{
					Angle_add = 360 - Angle_start + Angle_end;
				}
			}
			if (m_staticdraw){
				outfile << typegeomele << "    " << Angle_cut_start << "    " << Angle_add << "    " << r << "    " << Arccent_x << "    " << Arccent_y << endl;
				OpenGLDraw.StaticDrawArc(Angle_cut_start, Angle_add, r, Arccent_x, Arccent_y);//静态圆弧绘图命令（进给行程）

			}
			else{
				if (OpenGLDraw.DynamicDrawArc(Angle_cut_start, Angle_add, r, Arccent_x, Arccent_y, m_drawspeed2) == -1)//DynamicDraw返回-1时，说明stop
					return -1;
			
			}

			break;
		case 3:
			typegeomele = 2;//实切圆弧
			Angle_start = tempnode->m_GeomStandData.m_circle.m_Circle_Start_Angle;
			Angle_add = 360;
			r = tempnode->m_GeomStandData.m_circle.m_Circle_r;
			Arccent_x = tempnode->m_GeomStandData.m_circle.m_Circent_x;
			Arccent_y = tempnode->m_GeomStandData.m_circle.m_Circent_y;
			//静态圆弧绘图命令（进给行程）
			if (m_staticdraw){
				outfile << typegeomele << "    " << Angle_start << "    " << Angle_add << "    " << r << "    " << Arccent_x << "    " << Arccent_y << endl;
				OpenGLDraw.StaticDrawArc(Angle_start, Angle_add, r, Arccent_x, Arccent_y);//静态圆弧绘图命令（进给行程）

			}
			else{
				if (OpenGLDraw.DynamicDrawArc(Angle_start, Angle_add, r, Arccent_x, Arccent_y, m_drawspeed2) == -1)//DynamicDraw返回-1时，说明stop
					return -1;
			}
			break;
		case 6:
			////typegeomele = 6;//切割引刀线
			//typegeomele = 1;//实切直线
			//x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
			//x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
			//y0 = tempnode->m_GeomStandData.GeoEle_start_y0;
			//y1 = tempnode->m_GeomStandData.GeoEle_start_y1;
			//outfile << typegeomele << "    " << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;
			//break;
			if (LeadlineType == LL_Line){
			typegeomele = 1;//实切直线
			x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
			x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
			y0 = tempnode->m_GeomStandData.GeoEle_start_y0;
			y1 = tempnode->m_GeomStandData.GeoEle_start_y1;
			if (m_staticdraw){
				OpenGLDraw.StaticDrawLine(x0, y0, x1, y1);//静态圆弧绘图命令（进给行程）
				outfile << typegeomele << "    " << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;
			}
			else{
				if (OpenGLDraw.DynamicDrawLine(x0, y0, x1, y1, m_drawspeed2) == -1)//DynamicDraw返回-1时，说明stop
					return -1;
			}
			}

			break;
		default:
			break;
		}
		if (NULL == tempnode->nextGeomeleNode)//如果是最后一个,那么要把他的值保留下来
		{
			PLastNode = tempnode;
			x0_tran = tempnode->m_GeomStandData.GeoEle_start_x1;
			y0_tran = tempnode->m_GeomStandData.GeoEle_start_y1;
		}


		tempnode = tempnode->nextGeomeleNode;

	}
	//pTheFirstLevelCloseHead是全局变量，下面代码是表示:
	//如果第一层的封闭环遍历完了，那么就该让水刀回到机床原点。
	if (NULL == A11_pTheFirstLevelCloseHead->nextGeomcloseNode)
	{
		GeomEleNode*temp;
		temp = A11_pTheFirstLevelCloseHead->FirstGeomele;
		while (temp->nextGeomeleNode)
			temp = temp->nextGeomeleNode;
		if (temp == PLastNode)
		{
			x1 = 0.0;
			y1 = 0.0;
			typegeomele = 4;//空跑直线
			if (m_staticdraw)
			{
				OpenGLDraw.StaticDrawDottedLine(x0_tran, y0_tran, x1, y1);//静态圆弧绘图命令（空行程）
				outfile << typegeomele << "    " << x0_tran << "    " << y0_tran << "    " << x1 << "    " << y1 << endl;

			}
			
			else{
				if (OpenGLDraw.DynamicDrawDottedLine(x0_tran, y0_tran, x1, y1, m_drawspeed1) == -1)//DynamicDraw返回-1时，说明stop
					return -1;
			}
			OpenGLDraw.StaticDrawArrow(x0_tran, y0_tran, x1, y1);//静态圆弧绘图命令（进给行程）
		}

	}
	return 0;
}

LRESULT CChildDlgA11::TabA1glFlush(WPARAM wparam, LPARAM lparam)//通过重新开一个多线程来解决Tab之后不显示OpenGL，但在绘图的时候不用启用这个函数，因为也有显示。
{
	if (m_A11_IfDataDisposed == false){
		return 0;
	}
	m_A11_firstdraw = true;//重新画图，需要画第一条线
	if (m_staticdraw){//如果不是动态绘图的时候，不用执行
	pStaticDrawThread = AfxBeginThread(StaticDrawThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	m_StaticDrawThreadList.SetAt(pStaticDrawThread->m_hThread, pStaticDrawThread);
	pStaticDrawThread->ResumeThread();
	}

	return 0;
}

LRESULT CChildDlgA11::ClearDXFData(WPARAM wparam, LPARAM lparam)
{
	//OpenGLDraw.InitOpenGL(GetDlgItem(IDC_OPENGL));//清除时，由于这里是在主线程用glFlush，而不是辅助线程，因此需要重新初始化一个新的OpenGL
	OpenGLDraw.ClearOpenGL();
	OpenGLDraw.InitDraw();

	m_staticdraw = true;
	m_A11_IfDataDisposed = false;
	m_A11_pNestrsltdtND = NULL;
	m_A11_pBatchHead = NULL;
	m_A11_pGeomclsHead = NULL;
	m_A11_NewDxf = true;
	A11_BatchHEAD_ID =0;
	A11_NestResult_ID = 0;
	A11_GEOMCLOSE_ID = 0;
	m_A11_firstdraw = true;
	return 0;
}

LRESULT CChildDlgA11::GetVisualSimulationMsg(WPARAM wparam, LPARAM lparam)//可视化仿真
{
	SpeedStruct* receivespeed = (SpeedStruct*)wparam;
	m_drawspeed1 = receivespeed->speed1;
	m_drawspeed2 = receivespeed->speed2;
	if (m_A11_IfDataDisposed == false){
		MessageBox(_T("请先读取DXF文件"));
		return -1;
	}

	if (receivespeed->speed1 <0 || receivespeed->speed2 <0){
		MessageBox(_T("空行程速度和进给速度需均为正数"));
		return -1;
	}
	if ((receivespeed->speed1>1199.99) || (receivespeed->speed2>1199.99)){ //1200对应在界面输入50
		MessageBox(_T("请输入正确的速度：空行程速度和进给速度不能为0，且均需大于50mm/min"));
		return -1;
	}

	if (m_endDynamicDraw == true){

		if (m_A11_IfDataDisposed == true)//开始前，先清空并绘静态图一次
		{
			m_A11_firstdraw = true;//重新绘图，要画第一条线
			OpenGLDraw.ClearOpenGL();
			ReadDXFandDrawFunc();
		}
	m_staticdraw = false;//非静态画图，可视化仿真动态模拟画图的标志位

	m_A11_firstdraw = true;//重新绘图，要画第一条线
	//以下为AfxBeginThread的参数说明
	//参数1  线程的入口函数, 声明一定要如下: UINT MyThreadFunction(LPVOID pParam);
	//参数2 传递入线程的参数, 注意它的类型为:LPVOID, 所以我们可以传递一个结构体入线程.
	//参数3、4、5分别指定线程的优先级、堆栈大小、创建标识、安全属性，含义同用户界面线程。
	pVisualDynamicDrawThread = AfxBeginThread(DynamicDrawThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	m_VisualDynamicDrawThreadList.SetAt(pVisualDynamicDrawThread->m_hThread, pVisualDynamicDrawThread);
	pVisualDynamicDrawThread->ResumeThread();
	m_endDynamicDraw = false;
	}
	else{
		MessageBox(_T("可视化仿真过程正在进行"));
		return 1;
	}

	return 0;

}

UINT CChildDlgA11::DynamicDrawThread(LPVOID lParam)//可视化仿真动态模拟画图函数
{
	CChildDlgA11* pDlg = (CChildDlgA11*)lParam;//强制转换类型，将LPVOID装换为COpenGLDlgDlg,才可以用得到pDlg->OnBnClickedButton9();因为调用Button9的对象属于COpenGLDlgDlg类
	pDlg->ReadDXFandDrawFunc();//调用绘图函数，即调用类方法成员。因为该函数为静态函数，因此不能用非静态数据成员，所以必须另外开一个非太静态函数成员来运行非静态数据成员
	return 0;
}

LRESULT CChildDlgA11::StopandRedrawStaticPaint(WPARAM wparam, LPARAM lparam){

	if (m_staticdraw == true ){
		MessageBox(_T("请先启动可视化仿真/动态模拟/实际加工过程"));
	}
	else{
		CNCRunModeInf.StopRunning = true;//首先改使opengl的dynamic draw函数跳出循环，并返回-1，接着逐层函数跳出循环。
		m_staticdraw = true;//静态画图标志位
		m_A11_firstdraw = true;//重新画图，需要画第一条线
		//OpenGLDraw.InitOpenGL(GetDlgItem(IDC_OPENGL));//调用ClearOpenGL清除时，在主线程用glFlush，而不是辅助线程，因此需要重新初始化一个新的OpenGL
		if (m_A11_IfDataDisposed == true)
		{
			OpenGLDraw.ClearOpenGL();
			OpenGLDraw.InitDraw();
			ReadDXFandDrawFunc();//停止后，重绘静态图
		}

		m_endDynamicDraw = true;
		switch (CNCRunModeInf.RunMode){
		case m_VisualSimulation:
				MessageBox(_T("可视化仿真过程被停止，可选择重新开始"));
				break;
		case m_AutoWithoutCut:
			MessageBox(_T("动态仿真过程被停止，可选择重新开始"));
			break;
		case m_AutoCutting:
			MessageBox(_T("实际加工过程被停止，可选择重新开始"));
			break;
		default:
			break;

		}

	}

	
	return 0;

}

LRESULT CChildDlgA11::PauseandContinueVisualSimulation(WPARAM wparam, LPARAM lparam){

	if (m_staticdraw == true){//如果未进入动态画图，点击无效
		MessageBox(_T("请先启动可视化仿真/动态模拟/实际加工过程"));

	}
	else{

	
	if (CNCRunModeInf.ContinueRunning == true){
		pVisualDynamicDrawThread->SuspendThread();
		CNCRunModeInf.ContinueRunning = false;
		MessageBox(_T("可视化仿真过程暂停"));

	}
	else{
		pVisualDynamicDrawThread->ResumeThread();
		CNCRunModeInf.ContinueRunning = true;
		MessageBox(_T("可视化仿真过程继续"));

	}
	}
	return 0;

}

LRESULT CChildDlgA11::UpdateNonCuttingRateFunc(WPARAM wparam, LPARAM lparam){

	float* speed = (float*)wparam;//获得指针值，而不是指针指向的值。可m_drawspeed1 = *(float*)wparam直接获得指针指向的值，*为解地址符。
	m_drawspeed1 = *speed;
	return 0;

}

LRESULT CChildDlgA11::UpdateFeedRateFunc(WPARAM wparam, LPARAM lparam){

	float* speed = (float*)wparam;
	m_drawspeed2 = *speed;

	return 0;

}

LRESULT CChildDlgA11::CNCDraw(WPARAM wparam, LPARAM lparam){
	pCNCDrawThread = AfxBeginThread(CNCDrawThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	m_VisualDynamicDrawThreadList.SetAt(pCNCDrawThread->m_hThread, pCNCDrawThread);
	pCNCDrawThread->ResumeThread();
	m_endDynamicDraw = false;
	return 0;
}

UINT CChildDlgA11::CNCDrawThread(LPVOID lParam)//可视化仿真动态模拟画图函数
{

	CChildDlgA11* pDlg = (CChildDlgA11*)lParam;//强制转换类型，将LPVOID装换为COpenGLDlgDlg,才可以用得到pDlg->OnBnClickedButton9();因为调用Button9的对象属于COpenGLDlgDlg类
	pDlg->BeginCNCDraw();//调用绘图函数，即调用类方法成员。因为该函数为静态函数，因此不能用非静态数据成员，所以必须另外开一个非太静态函数成员来运行非静态数据成员
	return 0;
}

void CChildDlgA11::BeginCNCDraw( ){
	OpenGLDraw.ClearOpenGL();
	OpenGLDraw.InitOpenGL(GetDlgItem(IDC_OPENGL));//就算将此行放到OnInitDlg中，也不能在此省略，因为动态绘图时是另一个线程调用，需要另外初始化。
	OpenGLDraw.Reshape();
	OpenGLDraw.InitDraw();//初始化画板，绘坐标轴

	while (1){
		if (b_setBaseFrame == true){
			if (b_CutColor == true){
				glColor3f(1, 0, 0);
			}
			else glColor3f(0, 0, 1);

		DrawX1 = BaseframeData.x;
		DrawY1 = BaseframeData.y;
		glLineWidth(2);             //静态虚线线粗
		glBegin(GL_LINES);//在下面2个点之间画一条虚线
		glVertex2d(DrawX0, DrawY0);
		glVertex2d(DrawX1, DrawY1);
		glEnd();
		glFlush();
		DrawX0 = DrawX1;
		DrawY0 = DrawY1;
		Sleep(50);
		}
	}


}

LRESULT CChildDlgA11::AutoModeRedrawStaticFunc(WPARAM W, LPARAM L){
	m_staticdraw = true;
	OpenGLDraw.InitOpenGL(GetDlgItem(IDC_OPENGL));
	OpenGLDraw.ClearOpenGL();
	OpenGLDraw.InitDraw();
	m_A11_firstdraw = true;//重新绘图，要画第一条线
	ReadDXFandDrawFunc();//停止后，重绘静态图
	return 0;
}