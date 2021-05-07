
// ThreadTestDlg.h : ͷ�ļ�
//

#pragma once
#include<iostream>
using namespace std;

// CThreadTestDlg �Ի���
class CThreadTestDlg : public CDialogEx
{
// ����
public:
	CThreadTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THREADTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

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



// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
