// Configuration.cpp : 实现文件
//

#include "stdafx.h"
#include "WJSdemo.h"
#include "Configuration.h"
#include "afxdialogex.h"

#include "WJSdemoDlg.h"

// CConfiguration 对话框

IMPLEMENT_DYNAMIC(CConfiguration, CDialogEx)

CConfiguration::CConfiguration(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfiguration::IDD, pParent)
	, m_LeadLineLength_input(0)
{

}

CConfiguration::~CConfiguration()
{
}

void CConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LEADLINE_LENGTH, m_LeadLineLength_input);

	DDX_Control(pDX, IDC_RADIO1, m_LeadLineSetting);

}


BEGIN_MESSAGE_MAP(CConfiguration, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO3, &CConfiguration::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO1, &CConfiguration::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CConfiguration::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_SET_CONFIGURATION, &CConfiguration::OnBnClickedSetConfiguration)
END_MESSAGE_MAP()


// CConfiguration 消息处理程序

BOOL CConfiguration::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(_T("切割参数设置"));

	if (m_FirstLeadingLineLengthSetting == true){
	m_LeadLineSetting.SetCheck(TRUE);
	GetDlgItem(IDC_EDIT_LEADLINE_LENGTH)->EnableWindow(FALSE);
	m_FirstLeadingLineLengthSetting = false;
	return 0;
	}
	else{
		if (LeadlineType == LL_NoType){
			m_LeadLineSetting.SetCheck(TRUE);
			GetDlgItem(IDC_EDIT_LEADLINE_LENGTH)->EnableWindow(FALSE);
			m_FirstLeadingLineLengthSetting = false;
			return 0;
		}
		else if (LeadlineType == LL_Line){
			m_LeadLineSetting.SetCheck(TRUE);
			((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
			((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
			((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
			m_FirstLeadingLineLengthSetting = false;
			UpdateData(TRUE);
			m_LeadLineLength_input = m_LeadingLineLength;
			UpdateData(FALSE);

			return 0;
		}
		else {
			m_LeadLineSetting.SetCheck(TRUE);
			((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
			((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
			((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);
			m_FirstLeadingLineLengthSetting = false;
			return 0;
		}
		}

	}




void CConfiguration::OnBnClickedRadio1()
{
	GetDlgItem(IDC_EDIT_LEADLINE_LENGTH)->EnableWindow(FALSE);
	LeadlineType = LL_NoType;
}


void CConfiguration::OnBnClickedRadio2()
{

	GetDlgItem(IDC_EDIT_LEADLINE_LENGTH)->EnableWindow(TRUE);
	LeadlineType = LL_Line;
}

void CConfiguration::OnBnClickedRadio3()
{
	GetDlgItem(IDC_EDIT_LEADLINE_LENGTH)->EnableWindow(TRUE);
	LeadlineType = LL_Arc;
}


void CConfiguration::OnBnClickedSetConfiguration()
{
	UpdateData(TRUE);
	m_LeadingLineLength = m_LeadLineLength_input;
	UpdateData(FALSE);

	if (LeadlineType == LL_Line){
		if (m_LeadingLineLength > 30 or m_LeadingLineLength <= 0){
		
			MessageBox(_T("设置失败：切割引导线的直线型长度不能大于30mm或小于等于0"));
		
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		m_LeadingLineLength = 0;
		UpdateData(TRUE);
		m_LeadLineLength_input = 0;
		UpdateData(FALSE);
		OnBnClickedRadio1();
	}

	else{
		MessageBox(_T("直线型切割引导线设置成功，请重新打开DXF图"));

	}
	}
	else if (LeadlineType == LL_Arc){
		if (m_LeadingLineLength > 25 or m_LeadingLineLength <= 0){
			MessageBox(_T("设置失败：圆弧线半径不能大于25mm或小于0"));
			((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
			((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
			((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
			m_LeadingLineLength = 0;
			UpdateData(TRUE);
			m_LeadLineLength_input = 0;
			UpdateData(FALSE);
			OnBnClickedRadio1();
		}

		else{
			MessageBox(_T("圆弧型切割引导线设置成功，请重新打开DXF图"));

		}
	}
	else{
		m_LeadingLineLength = 0;
		MessageBox(_T("无引导线设置成功，请重新打开DXF图"));
	}

	//this->ShowWindow( SW_HIDE);
}

UINT CConfiguration::SetWaterJetConfigThread(LPVOID lParam){
	CConfiguration* p = (CConfiguration*)lParam;
		p->SetWaterJetConfig();
		return 0;
		//CWinThread* pThread = AfxBeginThread(SetWaterJetConfigThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		////m_ThreadList.SetAt(pThread->m_hThread, pThread);//当用了CNCInterface.Connect之后，这一行会崩溃，原因是为什么呢？
		//pThread->ResumeThread();
};

void CConfiguration::SetWaterJetConfig(){



}



