#include "stdafx.h"
#include "ReadTXTData.h"

using namespace std;
const int CReadTXTData::getMaxRow(char* cwd){
	ifstream txtFile(cwd);
	if (!txtFile.is_open()){
		MessageBox(_T("未打开DXF文件，或缓存文件失败，需重新打开DXF文件(1)"));
	}
	string buffer;
	int number = 0;
	while (!txtFile.eof()){
		getline(txtFile, buffer);
		number++;

	}
	return number;
}

bool CReadTXTData::getTxtData(float **b, const int maxrow, const int maxcol, char*cwd){
	ifstream txtFile(cwd);
	if (!txtFile.is_open()){
		MessageBox(_T("未打开DXF文件，或缓存文件失败，需重新打开DXF文件(2)"));
	}
	string buffer;
	for (int i = 0; i < maxrow; i++){
		getline(txtFile, buffer);//一次读入一行


		istringstream is(buffer);//将buffer的内容输入到is，遇到空格不停止
		for (int j = 0; j < maxcol; j++){
			is >> b[i][j];//遇到空格结束输出（意味着一次只输出一个string)
		}
	}
	return true;
}

