
// ChatAppADlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ChatAppA.h"
#include "ChatAppADlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_PIPE_MESSAGE (WM_USER + 1)
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


// CChatAppADlg dialog



CChatAppADlg::CChatAppADlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATAPPA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatAppADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_CHAT, m_editChat);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_editMessage);
}

BEGIN_MESSAGE_MAP(CChatAppADlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_PIPE_MESSAGE, &CChatAppADlg::OnPipeMessage)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatAppADlg::OnBnClickedButtonSend)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CChatAppADlg message handlers

BOOL CChatAppADlg::OnInitDialog()
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
	m_hPipe = CreateNamedPipe(
		_T("\\\\.\\pipe\\MFCChatPipe"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE |
		PIPE_READMODE_MESSAGE |
		PIPE_WAIT,
		1,
		1024,
		1024,
		0,
		NULL
	);

	if (m_hPipe == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("Không tạo được Named Pipe!"));
		return TRUE;
	}

	AfxBeginThread(
		[](LPVOID pParam) -> UINT
		{
			CChatAppADlg* pDlg = (CChatAppADlg*)pParam;

			BOOL connected = ConnectNamedPipe(
				pDlg->m_hPipe,
				NULL
			);
			if (!connected)
			{
				if (GetLastError() != ERROR_PIPE_CONNECTED)
					return 1;
			}
			WCHAR buffer[512];

			while (true)
			{
				DWORD bytesAvailable = 0;
				BOOL peekResult = PeekNamedPipe(
					pDlg->m_hPipe,
					NULL,
					0,
					NULL,
					&bytesAvailable,
					NULL
				);
				if (!peekResult)
				{
					break;
				}
				if (bytesAvailable > 0)
				{
					DWORD bytesRead = 0;
					BOOL result = ReadFile(
						pDlg->m_hPipe,
						buffer,
						sizeof(buffer) - sizeof(WCHAR),
						&bytesRead,
						NULL
					);

					if (!result || bytesRead == 0)
						break;

					buffer[bytesRead / sizeof(WCHAR)] = L'\0';

					CString* pMessage = new CString((buffer));

					pDlg->PostMessage(
						WM_PIPE_MESSAGE,
						0,
						(LPARAM)pMessage
					);
				}
				else
				{
					Sleep(100);
				}
			}
			return 0;
		},
		this
	);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatAppADlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatAppADlg::OnPaint()
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
HCURSOR CChatAppADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CChatAppADlg::OnPipeMessage(
	WPARAM wParam,
	LPARAM lParam)
{
	CString* pMessage =
		(CString*)lParam;

	if (pMessage != NULL)
	{
		CString chat;

		m_editChat.GetWindowText(chat);

		chat += _T("B: ");
		chat += *pMessage;
		chat += _T("\r\n");

		m_editChat.SetWindowText(chat);

		delete pMessage;
	}

	return 0;
}

void CChatAppADlg::OnBnClickedButtonSend()
{
	CString message;

	m_editMessage.GetWindowText(message);

	if (message.IsEmpty())
	{
		return;
	}
	DWORD bytesWritten = 0;

	BOOL result = WriteFile(
		m_hPipe,
		message.GetString(),
		(message.GetLength() + 1) * sizeof(WCHAR),
		&bytesWritten,
		NULL
	);

	if (!result)
	{
		AfxMessageBox(_T("Gửi tin nhắn thất bại!"));
		return;
	}
	CString chat;

	m_editChat.GetWindowText(chat);

	chat += _T("A: ");
	chat += message;
	chat += _T("\r\n");

	m_editChat.SetWindowText(chat);

	m_editMessage.SetWindowText(_T(""));
}
void CChatAppADlg::OnDestroy()
{

	if (m_hPipe != INVALID_HANDLE_VALUE)
	{
		DisconnectNamedPipe(m_hPipe);
		CloseHandle(m_hPipe);

		m_hPipe = INVALID_HANDLE_VALUE;
	}
	CDialogEx::OnDestroy();
}