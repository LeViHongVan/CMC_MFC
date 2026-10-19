
// ChatAppBDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ChatAppB.h"
#include "ChatAppBDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatAppBDlg dialog



CChatAppBDlg::CChatAppBDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATAPPB_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatAppBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_editMessage);
	DDX_Control(pDX, IDC_EDIT_CHAT, m_editChat);
}

BEGIN_MESSAGE_MAP(CChatAppBDlg, CDialogEx)
	ON_WM_COPYDATA()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatAppBDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CChatAppBDlg message handlers

BOOL CChatAppBDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatAppBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatAppBDlg::OnPaint()
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
HCURSOR CChatAppBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatAppBDlg::OnBnClickedButtonSend()
{
	CString message;

	m_editMessage.GetWindowText(message);

	if (message.IsEmpty())
		return;

	HWND hWndA = ::FindWindow(NULL, _T("ChatAppA"));

	if (hWndA == NULL)
	{
		AfxMessageBox(_T("Không tìm thấy ChatAppA!"));
		return;
	}

	COPYDATASTRUCT cds;

	cds.dwData = 1;
	cds.cbData = (message.GetLength() + 1) * sizeof(TCHAR);
	cds.lpData = (void*)message.GetString();

	::SendMessage(
		hWndA,
		WM_COPYDATA,
		(WPARAM)m_hWnd,
		(LPARAM)&cds
	);

	CString chat;

	m_editChat.GetWindowText(chat);

	chat += _T("B: ");
	chat += message;
	chat += _T("\r\n");

	m_editChat.SetWindowText(chat);

	m_editMessage.SetWindowText(_T(""));
}

BOOL CChatAppBDlg::OnCopyData(
	CWnd* pWnd,
	COPYDATASTRUCT* pCopyDataStruct)
{
	if (pCopyDataStruct == NULL)
		return FALSE;

	CString message((LPCTSTR)pCopyDataStruct->lpData);

	CString chat;
	m_editChat.GetWindowText(chat);

	chat += _T("A: ");
	chat += message;
	chat += _T("\r\n");

	m_editChat.SetWindowText(chat);

	return TRUE;
}