
// ThreadTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ThreadTest.h"
#include "ThreadTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CThreadTestDlg 对话框
struct ThreadInfo
{
	HANDLE hEvent;
	CString strPicPath;
	CString strIpsource;
	BOOL bcoustomState;
	int faceId;
	CThreadTestDlg* pThis;
	CString strBase64;
};
ThreadInfo g_pthInfo;

CThreadTestDlg::CThreadTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_THREADTEST_DIALOG, pParent)
{
	hTest = NULL;

	/*	
	1 null  SECURITY_ATTRIBUTES结构指针，可为NULL 
	2 manual 手动 manualReset 手动复位
	3 initStatus 复位状态为FALSE，事件阻塞
	4 null 事件的名称
	*/
	hTestEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	hThread[3] = NULL;
	for (int i = 0; i<3; i++)
	{
		hThreadEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);//设置成自复位类型，否则，似乎会调用三次
	}

	hTest2 = NULL;
	hTestEvent2 = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThreadTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CThreadTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CThreadTestDlg 消息处理程序

BOOL CThreadTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	DWORD dwThreadId = 0;
	//1 单线程
	//传入this指针，this,传入回调函数 TestThreadProc
	hTest = CreateThread(NULL, 0, TestThreadProc, this, 0, &dwThreadId);
	//2 多线程
	for (int i = 0; i < 3; i++)
	{
		hThread[i] = CreateThread(NULL, 0, TestMultipleThreadProc, this, 0, &dwThreadId);
	}
	//3 单线程 传参改变
	g_pthInfo.pThis = this;
	g_pthInfo.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hTest2 = CreateThread(NULL, 0, TestThreadProc2, &g_pthInfo, 0, &dwThreadId);

	//SetEvent(hTestEvent);
	SetEvent(hThreadEvent[2]);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CThreadTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CThreadTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


DWORD CThreadTestDlg::TestThreadProc(LPVOID pParam)
{
	CThreadTestDlg *pDlg = (CThreadTestDlg *)pParam;
	return pDlg->TestThreadContent(pParam);
}

DWORD CThreadTestDlg::TestThreadContent(LPVOID pParam)
{
	while (FALSE == g_bExit)
	{
		DWORD dwResult = WaitForSingleObject(hTestEvent, INFINITE); //等待一个进程结束
																	//DWORD dwResult = WaitForSingleObject(hTestEvent, 5000); //等待5秒，无论收到与否，都进入WAIT_TIMEOUT
		switch (dwResult)
		{
		case WAIT_OBJECT_0:
			// hProcess所代表的进程在5秒内结束
			AfxMessageBox("2");
			break;
		case WAIT_TIMEOUT:
			// 等待时间超过5秒
			AfxMessageBox("1");
			break;
		case WAIT_FAILED:
			// 函数调用失败，比如传递了一个无效的句柄,
			//hTestEvent 被赋值为null,然后setEvent(hTestEvent),触发WaitForSingleObject,会进入WAIT_FAILED
			AfxMessageBox("3");
			break;
		}
		ResetEvent(hTestEvent);
	}
	return TRUE;
}

DWORD CThreadTestDlg::TestMultipleThreadProc(LPVOID pParam)
{
	CThreadTestDlg *pDlg = (CThreadTestDlg *)pParam;
	return pDlg->TestMultipleThreadContent(pParam);
}

DWORD CThreadTestDlg::TestMultipleThreadContent(LPVOID pParam)
{
	DWORD dw = WaitForMultipleObjects(3, hThreadEvent, FALSE, INFINITE); //等待3个进程结束

	switch (dw)
	{
	case WAIT_FAILED:
		// 函数呼叫失败
		break;

	case WAIT_TIMEOUT:
		// 超时
		break;

	case WAIT_OBJECT_0 + 0:
		// h[0]（hProcess1）所代表的进程结束
		AfxMessageBox("t1");
		break;

	case WAIT_OBJECT_0 + 1:
		// h[1]（hProcess2）所代表的进程结束
		AfxMessageBox("t2");
		break;

	case WAIT_OBJECT_0 + 2:
		// h[2]（hProcess3）所代表的进程结束
		AfxMessageBox("t3");

		break;
	}
	return TRUE;
}

DWORD CThreadTestDlg::TestThreadProc2(LPVOID pParam)
{
	ThreadInfo *pDlg = (ThreadInfo *)pParam;
	return pDlg->pThis->TestThreadContent2(pParam);
}

DWORD CThreadTestDlg::TestThreadContent2(LPVOID pParam)
{
	ThreadInfo*pthInfo = (ThreadInfo*)pParam;

	while (FALSE == g_bExit)
	{
		DWORD dwResult = WaitForSingleObject(pthInfo->hEvent, INFINITE); //等待一个进程结束
																		 //DWORD dwResult = WaitForSingleObject(hTestEvent, 5000); //等待5秒，无论收到与否，都进入WAIT_TIMEOUT
		switch (dwResult)
		{
		case WAIT_OBJECT_0:
			// hProcess所代表的进程在5秒内结束
			//AfxMessageBox("2");
			break;

		case WAIT_TIMEOUT:
			// 等待时间超过5秒
			AfxMessageBox("1");
			break;

		case WAIT_FAILED:
			// 函数调用失败，比如传递了一个无效的句柄,
			//hTestEvent 被赋值为null,然后setEvent(hTestEvent),触发WaitForSingleObject,会进入WAIT_FAILED
			AfxMessageBox("3");
			break;
		}
		ResetEvent(hTestEvent);
	}
	return TRUE;
}
