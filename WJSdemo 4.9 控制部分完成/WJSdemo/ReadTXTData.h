#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include<iso646.h>
class CReadTXTData :public CDialogEx{
private:
	char* cwd;
public:
	const int getMaxRow(char* cwd);
	bool getTxtData(float** b, const int maxrow, const int maxcol, char* cwd); //读取文本中的二维数组

};