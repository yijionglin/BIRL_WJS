#pragma once
#define GET_DXFDATA WM_USER+1000//在A11获取一个生产批次的头节点，用于绘图
#define PROCEEDDXFDATA WM_USER+1001//B1向主对话框发送处理DXF信息，最后得到路径规划好的四向链表的头节点
#define TABA1GLFLUSH WM_USER+1002//向A1和A11框传送glFlush命令
#define CLEARDXFDATA WM_USER+1003//清除DXF数据，并清屏
#define VISUALSIMULATION WM_USER+1004//B2向A11发送可视化模拟信号
#define PAUSEORCONTINUE WM_USER+1005//主Dlg向A11发送继续、暂停按钮
#define STOPSIGNAL1 WM_USER+1006//主Dlg向A11发送停止命令,STOPSIGNAL1用于可视化模拟，STOPSIGNAL2用于动态模拟，STOPSIGNAL3用于实际加工
#define UPDATEFEEDRATE WM_USER+1007//更新进给倍率
#define UPDATENONCUTTINGSTROCKRATE WM_USER+1008//更新空行程倍率
#define GET_FILEPATH WM_USER+1009//获取文件路径
#define UPDATE_CNCPOSITION WM_USER+1010//机床反馈的坐标
#define CNCCONNECT WM_USER+1011//连接信息，B4发至B3
#define CNCCONNECTRESULT WM_USER+1012//连接信息反馈，B3发至B4
#define SETDOSTATES WM_USER+1013//发送IO设置信号
#define GETDOSTATES WM_USER+1014//反馈DO设置信号
#define UPDATE_BASE_ORIGIN WM_USER+1015//更新工件原点坐标
#define SEND_AUTO_START_MESSAGE WM_USER+1016//发送不开刀加工或开到加工信息
#define CNC_DRAW WM_USER+1017//发送机床反馈的数据来动态绘图的函数
#define BEFORE_AUTORUN_REDRAW WM_USER+1018//每次启动三个自动模式前，先清空画板和重画静态图A11Dlg
#define UPDATE_VISUAL_POSITION WM_USER+1019//更新可视化模拟的坐标

// CWJSdemoDlg 对话框
struct SpeedStruct{
	float speed1;//空切速度
	float speed2;//实切速度
};
enum DO_Num{ 
	DO64=64, 
	DO65=65, 
	DO66=66, 
	DO67=67, 
	DO68=68, 
	DO69=69 };

struct DOStates{
	bool DO64;
	bool DO65;
	bool DO66;
	bool DO67;
	bool DO68;
	bool DO69;

};

typedef struct CoordinateStruct{
	float x;//X轴坐标
	float y;//Y轴坐标
	float z;//Z轴坐标
}CoordinateStruct;


struct StopSignal{
	bool m_visualsimulation;//停止可视化仿真
	bool m_dynamicsimulation;//停止动态仿真
	bool m_manufacturing;//停止加工
};

struct PauseSignal{
	bool m_visualsimulation;//停止可视化仿真
	bool m_dynamicsimulation;//停止动态仿真
	bool m_manufacturing;//停止加工
};

enum StartMode{
	m_non,
	m_VisualSimulation,//软件工作模式
	m_AutoWithoutCut,
	m_AutoCutting,
	m_Manulmode
};


enum m_LeadLineType{ LL_NoType, LL_Line, LL_Arc };

struct RunModeInf{
	StartMode RunMode;
	bool StopRunning;
	bool ContinueRunning;
	
};

enum JogMode{
	sd,
	joginching,
	jogcontinue,
	jogstep,
	joghome

};
enum AxisNumber{

	AN_X, AN_Y, AN_Z

};
struct ManualMode{
	JogMode jogmode;
	bool jogPositive;
	double jogSpeed;
	double jogstepDistance;
	AxisNumber axisnum;
};
extern ManualMode CNCmanualmode;
//extern SpeedStruct* getCNCSpeed;
extern float speed1;
extern float speed2;
extern bool b_setBaseFrame;
extern bool b_CutColor;