
// TestMFC.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TestMFC.h"
#include "TestMFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFCApp

BEGIN_MESSAGE_MAP(CTestMFCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestMFCApp construction

CTestMFCApp::CTestMFCApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTestMFCApp object

CTestMFCApp theApp;


// CTestMFCApp initialization

BOOL CTestMFCApp::InitInstance()
{

	CTestMFCDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
	DWORD dwError = GetLastError();
	return FALSE;
}

