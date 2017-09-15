#pragma once
#include "GeomForCut.h"
#include "WJSOpenGL.h"
#include "Resource.h"

// CChildDlgA11 对话框

class CChildDlgA11 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlgA11)

public:
	CChildDlgA11(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlgA11();
	afx_msg LRESULT recieveDXFProcDataFromMainDlg(WPARAM wparam, LPARAM lparam);//获得一个生产批次的头节点信息
	static UINT StaticDrawThread(LPVOID lParam);//读取头节点数据并静态画图（全局预览）
	static UINT DynamicDrawThread(LPVOID lParam);//读取头节点数据并动态画图（可视化模拟）
	static UINT CNCDrawThread(LPVOID lParam);//读取头节点数据并动态画图（可视化模拟）
	void BeginCNCDraw();
	
	int ReadDXFandDrawFunc();
	//void DynamicDrawFunc();

	CMapPtrToPtr m_StaticDrawThreadList, m_VisualDynamicDrawThreadList, m_CNCDrawThreadList;//将线程句柄，线程对象添加到m_StaticDrawThreadList中，目的是防止线程未结束时，终止进程出现内存泄露。
	afx_msg LRESULT TabA1glFlush(WPARAM wparam, LPARAM lparam);//切换Tab的时候通过glFlsuh来显示OpenGl
	afx_msg LRESULT ClearDXFData(WPARAM wparam, LPARAM lparam);//B1Dlg发送过来的清除DXF数据命令。
	afx_msg LRESULT GetVisualSimulationMsg(WPARAM wparam, LPARAM lparam);//可视化仿真动态绘图消息函数
	afx_msg LRESULT StopandRedrawStaticPaint(WPARAM wparam, LPARAM lparam);//停止可视化仿真动态绘图的 消息函数
	afx_msg LRESULT PauseandContinueVisualSimulation(WPARAM wparam, LPARAM lparam);//停止可视化仿真动态绘图的 消息函数
	afx_msg LRESULT UpdateFeedRateFunc(WPARAM wparam, LPARAM lparam);//通过滑条发送信息函数改变进给速度
	afx_msg LRESULT UpdateNonCuttingRateFunc(WPARAM wparam, LPARAM lparam);//通过滑条发送信息函数改变空行程速度
	afx_msg LRESULT CNCDraw(WPARAM wparam, LPARAM lparam);//通过滑条发送信息函数改变空行程速度
	afx_msg LRESULT AutoModeRedrawStaticFunc(WPARAM W, LPARAM L);//每次启动三个自动模式前，先清空画板和重画静态图

	
	bool m_endDynamicDraw;//判断可视化模拟是否结束
	double m_drawspeed1, m_drawspeed2;//speed1用于空行程，speed2用于进给
	static bool m_redraw;
	CWinThread* pStaticDrawThread,*pVisualDynamicDrawThread,*pCNCDrawThread;
// 对话框数据
	enum { IDD = IDD_CHILDDLG_A11 };

	 //bool m_staticdraw=true;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:	
	BatchHEAD* m_A11_pBatchHead;//接受来自主对话框处理好后一个生产批次的数据的头节点
	bool m_A11_IfDataDisposed;//判断数据是否处理完成
	GeomCloseHEAD*A11_pTheFirstLevelCloseHead;
	int A11_BatchHEAD_ID;//初始化时候，还没有批次
	int A11_NestResult_ID;//初始化，还没有图纸导入
	int A11_GEOMCLOSE_ID;//初始化，还没有封闭环
	GeomCloseHEAD* m_A11_pGeomclsHead;
	NestResultDataNode*m_A11_pNestrsltdtND;//指向封闭环双向链表的头结点

	int A11_ReadKidCloseHeadData(GeomCloseHEAD*pTemp);
	int A11_ReadCloseHeadData(GeomCloseHEAD*Htemp);

	//这个类主要是实现把之前的封闭环之间串起来切割使用。
	//实现，将封闭环之间排序，对圆这种特殊图形要另外处理，对于圆弧要转换成原来的数据形式
	//包含封闭图元之间的过渡直线
	//弄个单向链表来存储着一张排样dxf的数据
	GeomForCut m_A11_GeomForCut;

	bool m_A11_firstdraw;//当第一次进入读取数据时候x0_tran，y0_tran应该为0.0，和每打开一张图片时候，都应该为0.0;(代替了原本的m_A11_NewDxf)
	bool m_A11_NewDxf;
	//一些数据
	double x0, x1, y0, y1, Arccent_x, Arccent_y, r, temp, Angle_start, Angle_end, Angle_cut_start;
	double x0_tran, y0_tran, x1_tran, y1_tran;
	double Angle_add;//增量角度

	CWJSOpenGL OpenGLDraw;//实例化OpenGL并用于绘图

	DXFProcData* GetDXFProcMsgFromMainDlg;//接受住Dlg处理好DXF图后的信息
	double DrawX0, DrawY0, DrawX1, DrawY1;

};
