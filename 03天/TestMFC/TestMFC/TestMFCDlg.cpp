
// TestMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestMFC.h"
#include "TestMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFCDlg dialog



CTestMFCDlg::CTestMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTestMFCDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CTestMFCDlg::OnBnClickedOk2)
	ON_BN_CLICKED(IDOK3, &CTestMFCDlg::OnBnClickedOk3)
	ON_BN_CLICKED(IDOK4, &CTestMFCDlg::OnBnClickedOk4)
	ON_BN_CLICKED(IDOK5, &CTestMFCDlg::OnBnClickedOk5)
END_MESSAGE_MAP()


// CTestMFCDlg message handlers

BOOL CTestMFCDlg::OnInitDialog()
{
	//WM_INITDIALOG
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

void CTestMFCDlg::OnPaint()
{
	//WM_Paint
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
HCURSOR CTestMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*

	char s[200];
	GetDlgItemText(IDC_LEFT1, s, sizeof(s));
	
	char s[200];
	sprintf(s, "%lf", fLeft + fRight);
*/

//	CDialogEx::OnOK();
void CTestMFCDlg::OnBnClickedOk()
{
	CString str;
	GetDlgItemText(IDC_LEFT1, str);
	double fLeft = atof(str);
	GetDlgItemText(IDC_RIGHT1, str);
	double fRight = atof(str);
	str.Format("%lf", fLeft + fRight);

	SetDlgItemText(IDC_RESULT1, str);

}


void CTestMFCDlg::OnBnClickedOk2()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_LEFT2, str);
	double fLeft = atof(str);
	GetDlgItemText(IDC_RIGHT2, str);
	double fRight = atof(str);
	str.Format("%lf", fLeft - fRight);
	SetDlgItemText(IDC_RESULT2, str);
}


void CTestMFCDlg::OnBnClickedOk3()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_LEFT3, str);
	double fLeft = atof(str);
	GetDlgItemText(IDC_RIGHT3, str);
	double fRight = atof(str);
	str.Format("%lf", fLeft * fRight);
	SetDlgItemText(IDC_RESULT3, str);
}


void CTestMFCDlg::OnBnClickedOk4()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_LEFT4, str);
	double fLeft = atof(str);
	GetDlgItemText(IDC_RIGHT4, str);
	double fRight = atof(str);
	if (fRight == 0)
		SetDlgItemText(IDC_RESULT4, "除数不能为0!");
	else {
		str.Format("%lf", fLeft / fRight);
		SetDlgItemText(IDC_RESULT4, str);
	}
}


void CTestMFCDlg::OnBnClickedOk5()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_LEFT5, str);
	int fLeft = atoi(str);
	GetDlgItemText(IDC_RIGHT5, str);
	int fRight = atoi(str);
	if (fRight == 0)
		SetDlgItemText(IDC_RESULT5, "除数不能为0!");
	else {
		str.Format("%d", fLeft % fRight);
		SetDlgItemText(IDC_RESULT5, str);
	}
}
