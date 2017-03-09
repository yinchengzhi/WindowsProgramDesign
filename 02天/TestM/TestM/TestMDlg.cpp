
// TestMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestM.h"
#include "TestMDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMDlg dialog



CTestMDlg::CTestMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//消息映射机制:消息的种类和消息对应的窗口ID关联到一个类成员函数上

BEGIN_MESSAGE_MAP(CTestMDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD, &CTestMDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DEC, &CTestMDlg::OnBnClickedDec)
END_MESSAGE_MAP()


// CTestMDlg message handlers

BOOL CTestMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CTestMDlg::OnBnClickedAdd()
{
	int nLeft = GetDlgItemInt(IDC_LEFT1, NULL, TRUE);
	int nRight = GetDlgItemInt(IDC_RIGHT1, NULL, TRUE);
	SetDlgItemInt(IDC_RESULT1, nLeft + nRight, TRUE);
	// TODO: Add your control notification handler code here
}


void CTestMDlg::OnBnClickedDec()
{
	// TODO: Add your control notification handler code here
}
