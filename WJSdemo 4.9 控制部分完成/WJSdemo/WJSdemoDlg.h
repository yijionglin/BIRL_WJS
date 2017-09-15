
// WJSdemoDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "ChildDlgA1.h"
#include "ChildDlgA2.h"
#include "ChildDlgA3.h"
#include "ChildDlgA4.h"
#include "ChildDlgB1.h"
#include "ChildDlgB2.h"
#include "ChildDlgB3.h"
#include "ChildDlgB4.h"
#include "GeomForCut.h"
#include "Resource.h"
#include "GlobalSetting.h"

////全局变量，停止信号
//extern struct StopSignal stopsignal;
//
////全局变量，暂停或继续信号
//extern struct PauseSignal pausesignal;
//
////进行模式
//extern StartMode WJAutoStartMode;
extern RunModeInf CNCRunModeInf;
//全局变量，动静态画图判断量
extern bool m_staticdraw;
//StopSignal stopsignal;

//全局变量声明，切割引导线长度
extern double m_LeadingLineLength;

extern bool m_FirstLeadingLineLengthSetting;//是否第一次打开设置对话框（用于保存第一次设置后的配置信息）
extern m_LeadLineType LeadlineType;

//工件系坐标数据
extern CoordinateStruct BaseframeData;
extern CoordinateStruct BaseframeOrigin;
//DO口信号
extern DO_Num CNC_DO_Num;
extern DOStates CNC_DO_States;

class CWJSdemoDlg : public CDialogEx
{
// 构造
public:
	CWJSdemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WJSDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_taba; //第一组选项卡，以下用于在标签页中显示对话框
	CChildDlgA1 m_ChildDlgA1;
	CChildDlgA2 m_ChildDlgA2;
	CChildDlgA3 m_ChildDlgA3;
	CChildDlgA4 m_ChildDlgA4;
	CChildDlgB1 m_ChildDlgB1;
	CChildDlgB2 m_ChildDlgB2;
	CChildDlgB3 m_ChildDlgB3;
	CChildDlgB4 m_ChildDlgB4;

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_tabb;
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);

	//打开DXF图并处理出带有路径规划的一整幅嵌套封闭环图。
	afx_msg LRESULT ProceedDXFData(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT GetCNCPositionData(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT GetVisualizedPositionData(WPARAM wparam, LPARAM lparam); //消息函数，接受来自A11框的坐标信息

	///////////////////////////////////////以下为排序封闭环和规划切割路径需要的变量和函数/////////////////////////////////////////////////////////////
	int BatchHEAD_ID;//生产批次
	BatchHEAD* m_pBatchHead;//将其划为全局变量，统一保存起来



	bool m_NewDxf;//当第一次进入读取数据时候x0_tran，y0_tran应该为0.0，和每打开一张图片时候，都应该为0.0;
	int NestResult_ID;//排样结果图纸
	int GEOMCLOSE_ID;//封闭环的个数
	GeomCloseHEAD*m_pGeomclsHead;//指向基本图元双向链表的头结点
	GeomClose m_GeomClose;//因为要运用到图元的操作，实例化这个类
	CString path;//保存打开的文件名
	NestResultDataNode*m_pNestrsltdtND;//指向封闭环双向链表的头结点，用于遍历一个生产批次的所有信息

	//这个类主要是实现把之前的封闭环之间串起来切割使用。
	//实现，将封闭环之间排序，对圆这种特殊图形要另外处理，对于圆弧要转换成原来的数据形式
	//包含封闭图元之间的过渡直线
	//弄个单向链表来存储着一张排样dxf的数据
	GeomForCut m_GeomForCut;

	int SendDOMessage(DO_Num , bool);
	void switchkeyword(CString path);
	CString m_readgeomele;//存储读取到的dxf的数据
	char* m_readgeomeleUnicode;//作为Unicode下，CString转换为char*
	int m_typegeomele = -1;//三种图元类型的标志位
	enum{ LINE, ARC, CIRCLE };//三种类型图元类型枚举变量
	int symbol;//当进入寻找循环的时候，用它来做判断存储
	GLINE m_line;//全局的m_line变量
	GARC m_arc;//全局的m_arc变量
	GCIRCLE m_circle;//全局的m_circle变量
	GeomStandData m_geomstanddata;//实例化一个存储数据结构
	GEOMELE m_geomele;//因为要运用到图元的操作，实例化这个类
	GeomEleNode*m_pGeomEleND;//基本图元双向链表的头结点
	//对于圆而言应该是另外的一条路
	NestResultDataNode*m_pNestNode_forCircle;//生成一个只存放圆的排样结果图结点
	GeomCloseHEAD*m_pGeomclsHead_forCircle;//指向基本图元双向链表的头结点


	bool AdjustGeomCloseNode(NestResultDataNode*head);
	GeomEleNode*m_pDiffGeomclsDataNode;//不同封闭环的数据结点
	bool m_IfDataDisposed;//数据是否已经处理完毕

	GLINE AcceptDxfLineData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//输入DXF文件中的标识输入，输出GLINE的数据
	GARC AcceptDxfArcData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//输入DXF文件中的标识输入，输出GARC的数据
	GCIRCLE AcceptDxfCircleData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//输入DXF文件中的标识输入，输出GCIRCLE的数据





	//CString strFilePath;
	bool m_savefile = true;//当第一次进入ReadCloseHeadData时，打开保存文件对话框，之后每次不用再打开保存文件对话框
	CString strFilePath;//用于保存“txt保存路径”
	///////////////////////////////////////以下为排序封闭环和规划切割路径需要的变量和函数/////////////////////////////////////////////////////////////


	DXFProcData* SendDXFProcMsgtoA11;
	afx_msg void OnBnClickedPauseorcontinue();
	afx_msg void OnBnClickedStop();
//	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedBtnGasvalve();
	afx_msg void OnBnClickedBtnWatervalve();
	afx_msg void OnBnClickedBtnWaterpump();
	afx_msg void OnBnClickedBtnSandvalve();
	afx_msg void OnBnClickedBtnMainpump();
	afx_msg LRESULT getDOStatesFunc(WPARAM w, LPARAM l);//接受DO设置信息
	afx_msg LRESULT updateBaseOriginFunc(WPARAM w, LPARAM y);

};
