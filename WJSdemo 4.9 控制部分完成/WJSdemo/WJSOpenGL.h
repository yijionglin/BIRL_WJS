#pragma once
#include "afxdialogex.h"

class CWJSOpenGL :
	public CDialogEx
{
protected:
	HDC hDC;//画图控件的DC
	HGLRC m_hGLRC;//OpenGL渲染描述表RC
	int pixelformat;//匹配后的最佳像素表格
	PIXELFORMATDESCRIPTOR pixelDesc;//像素表格描述
	CWnd* PaintWnd;
	//CoordinateStruct* WPC;//传递工件系的坐标

public:
	CWJSOpenGL();
	~CWJSOpenGL();
	void InitOpenGL(CWnd* getPictureCtrlItem);//初始化OpenGL
	//PIXELFORMATDESCRIPTOR pixelDesc;//像素表格描述
	//int pixelformat;//匹配后的最佳像素表格
	//CWnd* PaintWnd;//画图控件的ID
	//HDC hDC;//画图控件的DC
	//HGLRC m_hGLRC;//OpenGL渲染描述表RC
	int m_GLPixelIndex;//m_GLPixelIndex作为该像素表格的索引（选择符）
	void Reshape( );//调整画图控件的视野和大小等
	void DrawOpenGL();
	void drawString(const char* str); //屏幕显示字体  
	void InitDraw();//初始化画板，绘坐标轴
	//void DynamicDraw();//动态绘图
	void selectFont(int size, int charset, const char* face); //字体选择函数的声明，字体大小、字符集、字体名称，可查书看参数定义
	const int getMaxRow(char* cwd1); //此函数返回值为number，为txt文件中的总行数
	bool getTxtData(float** b, const int maxrow, const int maxcol, char* cwd1); //读取文本中的二维数组
	void StaticDrawArc(float b1, float b2, float b3, float b4, float b5);//静态圆弧绘图命令（进给行程）
	void StaticDrawDottedLine(float b1, float b2, float b3, float b4); //静态直线绘图命令（空行程）
	void StaticDrawLine(float b1, float b2, float b3, float b4); //静态直线绘图命令（进给行程）
	void StaticDrawArrow(float b1, float b2, float b3, float b4);//静态箭头绘图命令（空行程）

	int DynamicDrawArc(float b1, float b2, float b3, float b4, float b5, double &speed);//静态圆弧绘图命令（进给行程）
	int DynamicDrawDottedLine(float b1, float b2, float b3, float b4, double &speed); //静态直线绘图命令（空行程）
	int DynamicDrawLine(float b1, float b2, float b3, float b4, double &speed); //静态直线绘图命令（进给行程）

	void ClearOpenGL();

	void SendWPC(int type,int i, float xb,float x,float y, float z,float b3,float b4);//将可视化模拟信息发送到MainDlg


};

