
// ThreadTestDlg.h : 头文件
//

#pragma once
#include<iostream>
using namespace std;

// CThreadTestDlg 对话框
class CThreadTestDlg : public CDialogEx
{
// 构造
public:
	CThreadTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THREADTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	HANDLE hTest;
	HANDLE hTestEvent;
	static DWORD WINAPI	 TestThreadProc(LPVOID pParam);
	DWORD WINAPI TestThreadContent(LPVOID pParam);

	HANDLE hTest2;
	HANDLE hTestEvent2;
	static DWORD WINAPI	 TestThreadProc2(LPVOID pParam);
	DWORD WINAPI TestThreadContent2(LPVOID pParam);

	HANDLE hThread[3];
	HANDLE hThreadEvent[3];
	static DWORD WINAPI	 TestMultipleThreadProc(LPVOID pParam);
	DWORD WINAPI TestMultipleThreadContent(LPVOID pParam);



// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
