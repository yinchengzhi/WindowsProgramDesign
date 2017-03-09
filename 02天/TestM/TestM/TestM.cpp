
// TestM.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TestM.h"
#include "TestMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMApp

BEGIN_MESSAGE_MAP(CTestMApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestMApp construction

CTestMApp::CTestMApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTestMApp object

CTestMApp theApp;


// CTestMApp initialization

BOOL CTestMApp::InitInstance()
{
	
	CTestMDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
	
	return FALSE;
}

