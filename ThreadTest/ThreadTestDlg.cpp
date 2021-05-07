
// ThreadTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ThreadTest.h"
#include "ThreadTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CThreadTestDlg �Ի���
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
	1 null  SECURITY_ATTRIBUTES�ṹָ�룬��ΪNULL 
	2 manual �ֶ� manualReset �ֶ���λ
	3 initStatus ��λ״̬ΪFALSE���¼�����
	4 null �¼�������
	*/
	hTestEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	hThread[3] = NULL;
	for (int i = 0; i<3; i++)
	{
		hThreadEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);//���ó��Ը�λ���ͣ������ƺ����������
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


// CThreadTestDlg ��Ϣ�������

BOOL CThreadTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	DWORD dwThreadId = 0;
	//1 ���߳�
	//����thisָ�룬this,����ص����� TestThreadProc
	hTest = CreateThread(NULL, 0, TestThreadProc, this, 0, &dwThreadId);
	//2 ���߳�
	for (int i = 0; i < 3; i++)
	{
		hThread[i] = CreateThread(NULL, 0, TestMultipleThreadProc, this, 0, &dwThreadId);
	}
	//3 ���߳� ���θı�
	g_pthInfo.pThis = this;
	g_pthInfo.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hTest2 = CreateThread(NULL, 0, TestThreadProc2, &g_pthInfo, 0, &dwThreadId);

	//SetEvent(hTestEvent);
	SetEvent(hThreadEvent[2]);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CThreadTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		DWORD dwResult = WaitForSingleObject(hTestEvent, INFINITE); //�ȴ�һ�����̽���
																	//DWORD dwResult = WaitForSingleObject(hTestEvent, 5000); //�ȴ�5�룬�����յ���񣬶�����WAIT_TIMEOUT
		switch (dwResult)
		{
		case WAIT_OBJECT_0:
			// hProcess������Ľ�����5���ڽ���
			AfxMessageBox("2");
			break;
		case WAIT_TIMEOUT:
			// �ȴ�ʱ�䳬��5��
			AfxMessageBox("1");
			break;
		case WAIT_FAILED:
			// ��������ʧ�ܣ����紫����һ����Ч�ľ��,
			//hTestEvent ����ֵΪnull,Ȼ��setEvent(hTestEvent),����WaitForSingleObject,�����WAIT_FAILED
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
	DWORD dw = WaitForMultipleObjects(3, hThreadEvent, FALSE, INFINITE); //�ȴ�3�����̽���

	switch (dw)
	{
	case WAIT_FAILED:
		// ��������ʧ��
		break;

	case WAIT_TIMEOUT:
		// ��ʱ
		break;

	case WAIT_OBJECT_0 + 0:
		// h[0]��hProcess1��������Ľ��̽���
		AfxMessageBox("t1");
		break;

	case WAIT_OBJECT_0 + 1:
		// h[1]��hProcess2��������Ľ��̽���
		AfxMessageBox("t2");
		break;

	case WAIT_OBJECT_0 + 2:
		// h[2]��hProcess3��������Ľ��̽���
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
		DWORD dwResult = WaitForSingleObject(pthInfo->hEvent, INFINITE); //�ȴ�һ�����̽���
																		 //DWORD dwResult = WaitForSingleObject(hTestEvent, 5000); //�ȴ�5�룬�����յ���񣬶�����WAIT_TIMEOUT
		switch (dwResult)
		{
		case WAIT_OBJECT_0:
			// hProcess������Ľ�����5���ڽ���
			//AfxMessageBox("2");
			break;

		case WAIT_TIMEOUT:
			// �ȴ�ʱ�䳬��5��
			AfxMessageBox("1");
			break;

		case WAIT_FAILED:
			// ��������ʧ�ܣ����紫����һ����Ч�ľ��,
			//hTestEvent ����ֵΪnull,Ȼ��setEvent(hTestEvent),����WaitForSingleObject,�����WAIT_FAILED
			AfxMessageBox("3");
			break;
		}
		ResetEvent(hTestEvent);
	}
	return TRUE;
}
