#include "stdafx.h"
#include "WJSOpenGL.h"
#include <stdio.h>  
#include<iso646.h>
#include "stdafx.h"
#include "afxdialogex.h"
#include <atlstr.h>  
#include <string.h>  
#include <iostream>  
#include <GL/glut.h>
#include <math.h> 
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <windows.h>  
#include "WJSdemoDlg.h"

#define MAX_CHAR 128  // 申请MAX_CHAR个连续的显示列表编号，用于字符显示
const float Pi = 3.1415926536;

using namespace std;

CWJSOpenGL::CWJSOpenGL()
{
}


CWJSOpenGL::~CWJSOpenGL()
{
}


void CWJSOpenGL::InitOpenGL(CWnd* getPictureCtrlItem)

{//glClear(GL_COLOR_BUFFER_BIT);
	PaintWnd = getPictureCtrlItem;
	hDC = ::GetDC(PaintWnd->GetSafeHwnd()); // HDC为设备描述表句柄。
	//WPC = NULL;
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR); //初始化像素格式结构
	pixelDesc.nVersion = 1;
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_STEREO_DONTCARE;
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	//pWnd = GetDlgItem(IDC_STATIC);
	//在Windows环境中，句柄是用来标识被应用程序所建立或使用的对象的唯一整数

	if (!(pixelformat = ChoosePixelFormat(hDC, &pixelDesc)));//为设备描述表选择最匹配的像素格式 ，返回其索引。m_GLPixelIndex作为该像素表格的索引（选择符）  
	//https://msdn.microsoft.com/en-us/library/dd318284(VS.85).aspx
	//Return Value :If the function succeeds, the return value is a pixel format index (one-based) 
	//that is the closest match to the given pixel format descriptor.
	//If the function fails, the return value is zero.To get extended error information, call GetLastError.
	{
		int a = 1;//MessageBox(_T("ChoosePixelFormat failed!"));
	}
	if (!SetPixelFormat(hDC, pixelformat, &pixelDesc))
	{
		int a = 1;//MessageBox(_T("SetPixelFormat failed!"));
	}
	if (!(m_hGLRC = wglCreateContext(hDC)))      //函数创建一个新的OpenGL渲染描述表RC,此描述表必须适用于绘制到由hdc返回的设备。
		//这个渲染描述表将有和设备上下文(dc)一样的像素格式。
		//hdc用于函数将要创建的适合的OpenGL渲染描述表的设备上下文(dc)句柄.
		//Return Values如果函数调用成功, 返回有效的OpenGL渲染描述表句柄.否则返回NULL.
	{
		int a = 1;//MessageBox(_T("CreateContext failed!"));
	}
	if (!wglMakeCurrent(hDC, m_hGLRC))
	{
		int a = 1;//MessageBox(_T("MakeCurrent failed!"));
	}

}

void CWJSOpenGL::InitDraw(){
	Reshape();//调整窗口
	for (int l = 0; l < 9; l++) //坐标刻度
	{
		glColor3f(0, 1, 0);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex2d(-10, l * 100);
		glVertex2d(0, l * 100);
		glVertex2d(l * 100, -10);
		glVertex2d(l * 100, 0);
		glEnd();
		glFlush();
	}
	for (int l = 0; l < 9; l++) //坐标刻度
	{
		glColor3f(0, 1, 0);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex2d(-10, l * 100);
		glVertex2d(0, l * 100);
		glVertex2d(l * 100, -10);
		glVertex2d(l * 100, 0);
		glEnd();

	}

	selectFont(26, ANSI_CHARSET, "Comic Sans MS"); //字体大小、字符集、字体名称，
	glColor3f(0.0f, 1.0f, 0.0f);     //设置字体颜色  
	glRasterPos2i(-30, 850);  //起始位置  
	drawString("Y");   //输出的字符串  
	glColor3f(0.0f, 1.0f, 0.0f);     //设置字体颜色  
	glRasterPos2i(850, -35);  //起始位置  
	drawString("X");   //输出的字符串  

	glColor3f(0.0f, 1.0f, 0.0f);     //设置字体颜色  
	glRasterPos2i(-25, -20);  //起始位置  
	drawString("0");   //输出的字符串  

	glRasterPos2i(-55, 95);  //起始位置  
	drawString("100");   //输出的字符串  

	glRasterPos2i(-55, 195);  //起始位置  
	drawString("200");   //输出的字符串  

	glRasterPos2i(-55, 295);  //起始位置  
	drawString("300");   //输出的字符串  

	glRasterPos2i(-55, 395);  //起始位置  
	drawString("400");   //输出的字符串  

	glRasterPos2i(-55, 495);  //起始位置  
	drawString("500");   //输出的字符串  

	glRasterPos2i(-55, 595);  //起始位置  
	drawString("600");   //输出的字符串  

	glRasterPos2i(-55, 695);  //起始位置  
	drawString("700");   //输出的字符串  

	glRasterPos2i(-55, 795);  //起始位置  
	drawString("800");   //输出的字符串  

	glRasterPos2i(80, -35);  //起始位置  
	drawString("100");   //输出的字符串  

	glRasterPos2i(180, -35);  //起始位置  
	drawString("200");   //输出的字符串  

	glRasterPos2i(280, -35);  //起始位置  
	drawString("300");   //输出的字符串  

	glRasterPos2i(380, -35);  //起始位置  
	drawString("400");   //输出的字符串  

	glRasterPos2i(480, -35);  //起始位置  
	drawString("500");   //输出的字符串  

	glRasterPos2i(580, -35);  //起始位置  
	drawString("600");   //输出的字符串  

	glRasterPos2i(680, -35);  //起始位置  
	drawString("700");   //输出的字符串  

	glRasterPos2i(780, -35);  //起始位置  
	drawString("800");   //输出的字符串  

	glColor3f(0, 1, 0);//以下为坐标轴箭头
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2d(0, 0);
	glVertex2d(0, 850);
	glVertex2d(0, 0);
	glVertex2d(850, 0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(-6, 835);
	glVertex2d(6, 835);
	glVertex2d(0, 850);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(835, -6);
	glVertex2d(835, 6);
	glVertex2d(850, 0);
	glEnd();
	glFlush();


	//为了方便，将当前工作地址用cwd1来代替，这样方便后续的编程
	//将文件数据读入二维数组中
	//const int maxrow = getMaxRow(cwd);//实现对矩阵的各行取最大值，对角线取最大值，并用函数返回指针值
	//const int maxcol = 6;

	//float** b = new float*[maxrow];   //对于生成二维及更高维的数组，应使用多维指针。
	//以二维指针为例
	//int **p=new int* [row]; //row是二维数组的行，*p是指向一个指针数组的指针
	//for(int i=0; i<row; i++)
	//p[i]=new int [col]; //col是二维数组的列，p是指向一个int数组的指针
	//for (int i = 0; i<maxrow; i++){
	//	b[i] = new float[maxcol];
	//}

	//这个for语句是用new申请完数组加的一段初始化，若无，则无数据的数组元素位会显示乱码
	//for (int i = 0; i < maxrow; i++){
	//	for (int j = 0; j < maxcol; j++)
	//		b[i][j] = 0;
	//}
	//开始读取txt数据到二维数组b
	//getTxtData(b, maxrow, maxcol, cwd);

	//for (int n = 0; n<maxrow; n++)
	//{

	//	glPointSize(5);
	//	若为空行程，虚线段表示
	//	if (b[n][0] == 4){

	//		StaticDrawDottedLine(b[n][1], b[n][2], b[n][3], b[n][4]);//静态虚直线绘图命令（空行程）
	//		StaticDrawArrow(b[n][1], b[n][2], b[n][3], b[n][4]);//静态箭头绘图命令（空行程）


	//	}

	//	若为直线段
	//	else if (b[n][0] == 1)
	//	{
	//		StaticDrawLine(b[n][1], b[n][2], b[n][3], b[n][4]);//静态直线绘图命令（进给行程）

	//	}

	//	若为圆弧段
	//	else if (b[n][0] == 2)
	//	{

	//		StaticDrawArc(b[n][1], b[n][2], b[n][3], b[n][4], b[n][5]);//静态圆弧绘图命令（进给行程）



	//	}


	//}
	//for (int i = 0; i < maxrow; i++){ delete[]b[i]; }
	//delete[]b;
	//glFlush();
}

void CWJSOpenGL::Reshape(){
	{
		//获取窗口大小
		CRect rect;
		PaintWnd->GetWindowRect(rect);//GetDlgItem返回窗口中指定参数ID的子元素的句柄，可以通过返回的句柄对窗口内的子元素进行操作。
		//根据窗口大小，调整视口大小
		glViewport(0, 0, rect.Width(), rect.Height());// 这里要设置opengl在窗口的绘图区域，指定视口大小与对话框客户区大小一致
		glMatrixMode(GL_PROJECTION); 	// 一般只在窗口reshape时才重新设定下投影矩阵和模型矩阵
		glLoadIdentity();               //先初始化窗口，然后重置投影矩阵，最后回到模型矩阵

		//正射投影	 	
		if (rect.Width() <= rect.Height())
			gluOrtho2D(-70.0, 870.0, -80.0*(GLfloat)rect.Height() / (GLfloat)rect.Width(),
			880.0*(GLfloat)rect.Height() / (GLfloat)rect.Width());
		else
			gluOrtho2D(-70.0*(GLfloat)rect.Width() / (GLfloat)rect.Height(),
			870.0*(GLfloat)rect.Width() / (GLfloat)rect.Height(), -80.0, 880.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//glFlush();

	}

}

void CWJSOpenGL::drawString(const char* str) //屏幕显示字体  
{
	static int isFirstCall = 1;
	static GLuint lists;

	//if (isFirstCall) {
	//isFirstCall = 0;//并不需要这个，否则清屏后画图显示不出字体
	// 申请MAX_CHAR个连续的显示列表编号  
	lists = glGenLists(MAX_CHAR);
	// 把每个字符的绘制命令都装到对应的显示列表中  
	wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	//}
	// 调用每个字符对应的显示列表，绘制每个字符  
	for (; *str != '\0'; ++str) {
		glCallList(lists + *str);
	}
}


void CWJSOpenGL::selectFont(int size, int charset, const char* face) //字体选择函数的声明，字体大小、字符集、字体名称，可查书看参数定义
{

	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);

}


bool CWJSOpenGL::getTxtData(float** b, const int maxrow, const int maxcol, char* cwd1){ //读取文本中的二维数组
	//由于传入的实参是指针，因此赋值是直接到相应的内存地址，而不是简单的复制，因此调用该命令后，b的值仍然有效。
	ifstream txtFile(cwd1);//以输入方式打开文件，格式为ifstream file("地址")。char* cwd1 = "新建文本文档2.txt"
	if (!txtFile)return false;

	string buffer;       //访问字符操作string可以按数组方式，以下标来访问。还可以用at()函数访问指定的字符
	for (int i = 0; i<maxrow; i++){
		getline(txtFile, buffer);//getline（istream &in, string &s）从输入流读入一行到string s
		istringstream is(buffer);
		for (int j = 0; j<maxcol; j++){
			is >> b[i][j];

		}
	}
	txtFile.close();
	return true;
}


const int CWJSOpenGL::getMaxRow(char* cwd1){  //此函数返回值为number，为txt文件中的总行数
	ifstream txtFile(cwd1);//从当前文件地址中输入文件流到txrFile中
	if (!txtFile.is_open())//用来检查文件是否被打开：is_open方法。成功打开，其值为1
	{
		cout << "Error opening file"; exit(1);//退出该函数
	}
	string buffer; //访问字符操作string可以按数组方式，以下标来访问。还可以用at()函数访问指定的字符
	int number = 0;
	while (!txtFile.eof())//eof函数 ，它是ifstream 从类 ios 中继承过来的，当到达文件末尾时返回true 。(end of file）
	{
		getline(txtFile, buffer);//这里的getline通过和eof一起作用，作为一个最大行数的终止条件
		//（getline是从txtFile里面得到一行字符并输入到buffer中）
		number++;
	}
	return number;//若txt文档中最后一行为空行，则返回值为当前读取到的行数减一，
	//因为数组中的排序是从0而不是1开始，需要减一。
}

void CWJSOpenGL::StaticDrawArc(float b1, float b2, float b3, float b4, float b5) //静态圆弧绘图命令
{

	int arc = fabs(b2) / 180 * Pi*b3; //该段弧长值，fabs取绝对值
	float * xarc = new float[arc + 1]; //数组的个数为（arc+1）-1=arc个
	float * yarc = new float[arc + 1];
	glColor3f(1, 1, 0);
	glLineWidth(2);              //动态圆弧线粗         

	for (int arci = 1; arci <= arc; arci++) //因为这里arci最大值为arc,所以要用到arc个元素。
		//因此上面申请动态数组的时候，[]中必须为arc+1,这样申请到的元素总数为arc个才够用
	{


		xarc[0] = b4 + b3 * cos(b1 * Pi / 180);
		yarc[0] = b5 + b3 * sin(b1 * Pi / 180);
		xarc[arci] = b4 + b3 * cos((b1 + (b2)* arci / arc)*Pi / 180);
		yarc[arci] = b5 + b3 * sin((b1 + (b2)* arci / arc)*Pi / 180);
		glBegin(GL_LINES);
		glVertex2d(xarc[arci - 1], yarc[arci - 1]);
		glVertex2d(xarc[arci], yarc[arci]);
		glEnd();


	}

	delete[]xarc;
	delete[]yarc;
	//glFlush();

}

void CWJSOpenGL::StaticDrawLine(float b1, float b2, float b3, float b4) //静态直线绘图命令（进给行程）
{
	glColor3f(1, 1, 0);
	glLineWidth(2);
	glBegin(GL_LINES);//在下面2个点之间画一条虚线
	glVertex2d(b1, b2);
	glVertex2d(b3, b4);
	glEnd();
	//glFlush();

}



void CWJSOpenGL::StaticDrawDottedLine(float b1, float b2, float b3, float b4) //静态虚线绘图命令（空行程）
{
	glLineWidth(2);             //静态虚线线粗
	glColor3f(1, 1, 1);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(2, 0x0F0F);//设置虚线的样式  
	glBegin(GL_LINES);//在下面2个点之间画一条虚线
	glVertex2d(b1, b2);
	glVertex2d(b3, b4);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	//glFlush();

}

void CWJSOpenGL::StaticDrawArrow(float b1, float b2, float b3, float b4) //静态箭头绘图命令（空行程）x0 y0 x1 y1
{
	float*a = new float[5];

	if (b4< b2 && b3 <b1 or b4 > b2 && b3 < b1) //若线段为第三象限
	{
		a[4] = atan((b4 - b2) / (b3 - b1));//虚线的倾斜角度
		a[1] = b4 + 20 * sin(a[4] - 15 * Pi / 180);  //左边箭头点
		a[0] = b3 + 20 * cos(a[4] - 15 * Pi / 180);
		glColor3f(1, 0, 1);
		glLineWidth(4);
		glBegin(GL_LINES);
		glVertex2d(b3, b4);
		glVertex2d(a[0], a[1]);
		glEnd();
		a[3] = b4 + 20 * sin(a[4] + 15 * Pi / 180);  //右边箭头点
		a[2] = b3 + 20 * cos(a[4] + 15 * Pi / 180);
		glColor3f(1, 0, 1);
		glBegin(GL_POLYGON);
		glVertex2d(b3, b4);
		glVertex2d(a[2], a[3]);
		glVertex2d(a[2], a[3]);
		glVertex2d(a[0], a[1]);
		glVertex2d(b3, b4);
		glVertex2d(a[0], a[1]);
		glEnd();
	}
	else
	{
		a[4] = atan((b4 - b2) / (b3 - b1));//虚线的倾斜角度
		a[1] = b4 - 20 * sin(a[4] - 15 * Pi / 180);  //左边箭头点
		a[0] = b3 - 20 * cos(a[4] - 15 * Pi / 180);
		glColor3f(1, 0, 1);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex2d(b3, b4);
		glVertex2d(a[0], a[1]);
		glEnd();
		a[3] = b4 - 20 * sin(a[4] + 15 * Pi / 180);  //右边箭头点
		a[2] = b3 - 20 * cos(a[4] + 15 * Pi / 180);
		glColor3f(1, 0, 1);
		glLineWidth(2);
		glBegin(GL_POLYGON);
		glVertex2d(b3, b4);
		glVertex2d(a[2], a[3]);
		glVertex2d(a[2], a[3]);
		glVertex2d(a[0], a[1]);
		glVertex2d(b3, b4);
		glVertex2d(a[0], a[1]);
		glEnd();
	}
	delete[]a;
	//glFlush();

}

void CWJSOpenGL::ClearOpenGL() //静态箭头绘图命令（空行程）
{
	glClear(GL_COLOR_BUFFER_BIT);

	glFlush();
}

int CWJSOpenGL::DynamicDrawDottedLine(float b1, float b2, float b3, float b4, double &speed){ //静态直线绘图命令（空行程）
	glLineWidth(2);
	glColor3f(0, 0, 1);                 //动态空行程的线粗

	if (b1 != b3)										//如果斜率存在
	{
		GLfloat k = (b4 - b2) / (b3 - b1);					//直线斜率 y=kx+c
		GLfloat c = (b3 * b2 - b1 * b4) / (b3 - b1);			//直线常数
		float zjb1 = b3 - b1;           //直角边x
		float zjb2 = b4 - b2;            //直角边y
		float xb = sqrt(zjb1*zjb1 + zjb2*zjb2);    //斜边

		//假定以A为原点建立二维坐标系，则下边4个if分别对应于：第一象限，第二象限，第三象限，第四象限


		//float cta = atan(zjb2 / zjb1);             //倾斜角
		for (int i = 0; i <= (xb + 1); i++)			 	//若xb不+1，则因为for的判断语句中i为整型，缺失1
		{
			glEnable(GL_LINE_STIPPLE);
			glBegin(GL_LINES);
			glVertex2f(b1, b2);
			glVertex2f(b1 + i*zjb1 / xb, k*(b1 + i*zjb1 / xb) + c);//每一步斜边走1个单位长度，对应于横坐标和纵坐标分别为 1*cosa, 1*sina，其中cosa = zjb1/xb ,sina=zjb2/xb
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			Sleep(speed);
			glFlush();							//立刻开始处理保存在缓冲区中的OpenGL函数调用
			///////////////////////////以下为传递坐标信息
			float x = b1 + i*zjb1 / xb;
			float y = k*(b1 + i*zjb1 / xb) + c;        //工件坐标更新
			//HWND hWnd = ::FindWindow(NULL, _T("Waterjet-Software"));//获得父窗口手柄
			//FromHandle(hWnd)->SendMessage(UPDATEWPC, (WPARAM)(WPC), 0);
			////free(WPC);
			//WPC = NULL;
			SendWPC(1,i, xb, x, y, 0, b3, b4);
			if (CNCRunModeInf.StopRunning)
			{
				return -1;
				break;
			}
		}
		//if (stopsignal.m_visualsimulation)
		//{
		//	stopsignal.m_visualsimulation = false;
		//	return -1;
		//}


	}
	else
	{
		if (b2<b4)									//沿y轴正方向画线
		{
			for (int i = 0; i <= b4 - b2; i++)				//特别注意循环条件，既要避免死循环又要避免少循环
			{
				glBegin(GL_LINES);
				glVertex2f(b1, b2);
				glVertex2f(b1, b2 + i);
				glEnd();
				Sleep(speed);
				glFlush();							//立刻开始处理保存在缓冲区中的OpenGL函数调用
				//////////////////////////以下为传递坐标信息
				float x = b1;
				float y = b2+i;        //工件坐标更新
				//HWND hWnd = ::FindWindow(NULL, _T("Waterjet-Software"));//获得父窗口手柄
				//FromHandle(hWnd)->SendMessage(UPDATEWPC, (WPARAM)(WPC), 0);
				////free(WPC);
				//WPC = NULL;
				SendWPC(1,i, b4 - b2, x, y, 0, b3, b4);
				if (CNCRunModeInf.StopRunning)
				{
					return -1;
					break;
				}
			}
			//if (stopsignal.m_visualsimulation)
			//{
			//	stopsignal.m_visualsimulation = false;
			//	return -1;
			//}
		}
		else										//沿y轴负方向画线
		{
			for (int i = 0; i >= b4- b2; i--)				//特别注意循环条件，既要避免死循环又要避免少循环
			{
				glBegin(GL_LINES);
				glVertex2f(b1, b2);
				glVertex2f(b1, b2 + i);
				glEnd();
				Sleep(speed);
				glFlush();							//立刻开始处理保存在缓冲区中的OpenGL函数调用

				//////////////////////////以下为传递坐标信息
				float x = b1;
				float y = b2 + i;        //工件坐标更新
				//HWND hWnd = ::FindWindow(NULL, _T("Waterjet-Software"));//获得父窗口手柄
				//FromHandle(hWnd)->SendMessage(UPDATEWPC, (WPARAM)(WPC), 0);
				////free(WPC);
				//WPC = NULL;
				SendWPC(2,i, b4 - b2, x, y, 0, b3, b4);
				if (CNCRunModeInf.StopRunning)
				{
					return -1;
					break;
				}
			}
			//if (stopsignal.m_visualsimulation)
			//{
			//	stopsignal.m_visualsimulation = false;
			//	return -1;
			//}
		}
	}
	return 0;
}
int CWJSOpenGL::DynamicDrawLine(float b1, float b2, float b3, float b4, double &speed){ //静态直线绘图命令（进给行程）
	GLfloat k = (b4 - b2) / (b3 - b1);					//直线斜率 y=kx+c
	GLfloat c = (b3 * b2 - b1 * b4) / (b3 - b1);			//直线常数
	float zjb1 = b3 - b1;           //直角边x
	float zjb2 = b4 - b2;            //直角边y
	float xb = sqrt(zjb1*zjb1 + zjb2*zjb2);    //斜边

	glLineWidth(2);                         //动态直线的线粗
	glColor3f(1, 0, 0);

	if (b1 != b3)										//如果斜率存在
	{
		GLfloat k = (b4 - b2) / (b3 - b1);					//直线斜率 y=kx+c
		GLfloat c = (b3 * b2 - b1 * b4) / (b3 - b1);			//直线常数
		float zjb1 = b3 - b1;           //直角边x
		float zjb2 = b4 - b2;            //直角边y
		float xb = sqrt(zjb1*zjb1 + zjb2*zjb2);    //斜边

		//假定以A为原点建立二维坐标系，则下边4个if分别对应于：第一象限，第二象限，第三象限，第四象限


		//float cta = atan(zjb2 / zjb1);             //倾斜角
		for (int i = 0; i <= (xb + 1); i++)			 	//若xb不+1，则因为for的判断语句中i为整型，缺失1
		{
			glBegin(GL_LINES);
			glVertex2f(b1, b2);
			glVertex2f(b1 + i*zjb1 / xb, k*(b1 + i*zjb1 / xb) + c);//每一步斜边走1个单位长度，对应于横坐标和纵坐标分别为 1*cosa, 1*sina，其中cosa = zjb1/xb ,sina=zjb2/xb
			glEnd();
			Sleep(speed);
			glFlush();						//立刻开始处理保存在缓冲区中的OpenGL函数调用
			//CoordinateStruct* WPC = (CoordinateStruct *)malloc(sizeof(CoordinateStruct)); //工件坐标系 Workpiece Coordinate
			float x = b1 + i*zjb1 / xb;
			float y = k*(b1 + i*zjb1 / xb) + c;        //工件坐标更新
			//HWND hWnd = ::FindWindow(NULL, _T("Waterjet-Software"));//获得父窗口手柄
			//FromHandle(hWnd)->SendMessage(UPDATEWPC, (WPARAM)(WPC), 0);
			////free(WPC);
			//WPC = NULL;
			SendWPC(1,i, xb, x, y, 0, b3, b4);

			if (CNCRunModeInf.StopRunning)
			{
				return -1;
				break;
			}

		}

		//if (stopsignal.m_visualsimulation)
		//{
		//	stopsignal.m_visualsimulation = false;
		//	return -1;
		//}
	}
	else
	{
		if (b2<b4)									//沿y轴正方向画线
		{
			for (int i = 0; i <= b4 - b2; i++)				//特别注意循环条件，既要避免死循环又要避免少循环
			{
				glBegin(GL_LINES);
				glVertex2f(b1, b2);
				glVertex2f(b1, b2 + i);
				glEnd();
				Sleep(speed);
				glFlush();							//立刻开始处理保存在缓冲区中的OpenGL函数调用
				//CoordinateStruct* WPC = (CoordinateStruct *)malloc(sizeof(CoordinateStruct)); //工件坐标系 Workpiece Coordinate
				//WPC->x = b[n][1];
				//WPC->y = b[n][2] + i;        //工件坐标更新
				//HWND hWnd = ::FindWindow(NULL, _T("Waterjet-Software"));//获得父窗口手柄
				//FromHandle(hWnd)->SendMessage(UPDATEWPC, (WPARAM)(WPC), 0);
				////free(WPC);
				//WPC = NULL;
				float x = b1 ;
				float y = b2+i;        //工件坐标更新
				//HWND hWnd = ::FindWindow(NULL, _T("Waterjet-Software"));//获得父窗口手柄
				//FromHandle(hWnd)->SendMessage(UPDATEWPC, (WPARAM)(WPC), 0);
				////free(WPC);
				//WPC = NULL;
				SendWPC(1,i, b4 - b2, x, y, 0, b3, b4);
				if (CNCRunModeInf.StopRunning)
				{
					return -1;
					break;
				}

			}
			//if (stopsignal.m_visualsimulation)
			//{
			//	stopsignal.m_visualsimulation = false;
			//	return -1;
			//}

		}
		else										//沿y轴负方向画线
		{
			for (int i = 0; i >= b4 - b2; i--)				//特别注意循环条件，既要避免死循环又要避免少循环
			{
				glBegin(GL_LINES);
				glVertex2f(b1, b2);
				glVertex2f(b1, b2 + i);
				glEnd();
				Sleep(speed);
				glFlush();							//立刻开始处理保存在缓冲区中的OpenGL函数调用
				float x = b1 ;
				float y =b2+i;        //工件坐标更新
				//HWND hWnd = ::FindWindow(NULL, _T("Waterjet-Software"));//获得父窗口手柄
				//FromHandle(hWnd)->SendMessage(UPDATEWPC, (WPARAM)(WPC), 0);
				////free(WPC);
				//WPC = NULL;
				SendWPC(2,i, b4 - b2, x, y, 0, b3, b4);
				if (CNCRunModeInf.StopRunning)
				{
					return -1;
					break;
				}
			}
			//if (stopsignal.m_visualsimulation)
			//{
			//	stopsignal.m_visualsimulation = false;
			//	return -1;
			//}
		}
	}

}

int CWJSOpenGL::DynamicDrawArc(float b1, float b2, float b3, float b4,float b5, double &speed){ //静态直线绘图命令（进给行程）
	int arc = fabs(b2) / 180 * Pi*b3; //该段弧长值，fabs取绝对值
	float * xarc = new float[arc + 1]; //数组的个数为（arc+1）-1=arc个
	float * yarc = new float[arc + 1];

	glLineWidth(2);              //动态圆弧线粗         
	glColor3f(1, 0, 0);

	for (int arci = 1; arci <= arc; arci++) //因为这里arci最大值为arc,所以要用到arc个元素。
		//因此上面申请动态数组的时候，[]中必须为arc+1,这样申请到的元素总数为arc个才够用
	{


		xarc[0] = b4 + b3 * cos(b1* Pi / 180);
		yarc[0] = b5 + b3 * sin(b1 * Pi / 180);
		xarc[arci] = b4 + b3 * cos((b1 + (b2)  * arci / arc)*Pi / 180);
		yarc[arci] = b5 + b3 * sin((b1 + (b2) * arci / arc)*Pi / 180);
		glBegin(GL_LINES);
		glVertex2d(xarc[arci - 1], yarc[arci - 1]);
		glVertex2d(xarc[arci], yarc[arci]);
		glEnd();
		Sleep(speed);
		glFlush();
		//CoordinateStruct* WPC = (CoordinateStruct *)malloc(sizeof(CoordinateStruct)); //工件坐标系 Workpiece Coordinate
		//WPC->x = xarc[arci];
		//WPC->y = yarc[arci];        //工件坐标更新
		float x = xarc[arci];
		float y = yarc[arci];        //工件坐标更新
		//HWND hWnd = ::FindWindow(NULL, _T("Waterjet-Software"));//获得父窗口手柄
		//FromHandle(hWnd)->SendMessage(UPDATEWPC, (WPARAM)(WPC), 0);
		////free(WPC);
		//WPC = NULL;
		SendWPC(3, 0, 0, x, y, 0,0, 0);
		if (CNCRunModeInf.StopRunning)
		{
			return -1;
			break;
		}

	}
	//if (stopsignal.m_visualsimulation)
	//{
	//	stopsignal.m_visualsimulation = false;
	//	return -1;
	//}
	delete[]xarc;
	delete[]yarc;
	return 0;
}
//type 1 是一开始i<斜边的直线段，2是一开始i>斜边的直线段,分为type1,2是为了好处理当一条线段结束时其对应显示的坐标为其末端（否则会因为四舍五入导致有小数点）。
void CWJSOpenGL::SendWPC(int type,int i, float xb,float x,float y, float z,float b3,float b4){
	CoordinateStruct* WPC = (CoordinateStruct *)malloc(sizeof(CoordinateStruct)); //工件坐标系 Workpiece Coordinate
	if (type == 1){
	if (i < xb)
	{
		WPC->x =x;
		WPC->y =y;        //工件坐标更新
	}//每一步斜边走1个单位长度，对应于横坐标和纵坐标分别为 1*cosa, 1*sina，其中cosa = zjb1/xb ,sina=zjb2/xb
	else{//若到了最后一小段，则不计算，以线段末端的坐标显示
		WPC->x = b3;
		WPC->y = b4;        //工件坐标更新
	}
	}
	else if(type ==2 ){
		if (i > xb)
		{
			WPC->x = x;
			WPC->y = y;        //工件坐标更新
		}//每一步斜边走1个单位长度，对应于横坐标和纵坐标分别为 1*cosa, 1*sina，其中cosa = zjb1/xb ,sina=zjb2/xb
		else{//若到了最后一小段，则不计算，以线段末端的坐标显示
			WPC->x = b3;
			WPC->y = b4;        //工件坐标更新
		}
	}
	else if (type == 3){

			WPC->x = x;
			WPC->y = y;        //工件坐标更新

	}

	HWND hWnd = ::FindWindow(NULL, _T("超高压水射流切割智能控制软件"));//获得父窗口手柄
	//HWND hWnd = ::FindWindowEx(m_hWnd, NULL, NULL, _T("多线程绘图/传递头节点"));//最后一项为NULL可以，或为父框内的Dlg的标题，this->GetParent()->m_hWnd为父框手柄
	//hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("位置"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	//hWnd = ::FindWindowEx(FromHandle(hWnd)->GetDlgItem(IDC_TAB1)->m_hWnd, NULL, NULL, _T("常规"));//先在父窗口手柄下获得Tab控件，再获得子窗口
	FromHandle(hWnd)->SendMessage(UPDATE_VISUAL_POSITION, (WPARAM)(WPC), 0);
	//free(WPC);
	WPC = NULL;
}