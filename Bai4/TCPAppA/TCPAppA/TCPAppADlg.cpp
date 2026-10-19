
// TCPAppADlg.cpp : implementation file
//

#include "pch.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <thread>
#include <atomic>
#include <vector>
#include <string>
#include <cstdint>
#include "framework.h"
#include "TCPAppA.h"
#include "TCPAppADlg.h"
#include "afxdialogex.h"
#include <atlconv.h>
#pragma comment(lib, "Ws2_32.lib")
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


// CTCPAppADlg dialog



CTCPAppADlg::CTCPAppADlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCPAPPA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPAppADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTCPAppADlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CTCPAppADlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CTCPAppADlg::OnBnClickedButtonSend)
	ON_MESSAGE(WM_CHAT_TEXT, &CTCPAppADlg::OnChatText)
	ON_MESSAGE(WM_CHAT_STATUS, &CTCPAppADlg::OnChatStatus)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTCPAppADlg message handlers

void AppendChat(CWnd* pDlg, const CString& str)
{
	CWnd* pWnd = pDlg->GetDlgItem(IDC_LIST_CHAT);
	if (pWnd != nullptr)
	{
		TCHAR className[64] = { 0 };
		::GetClassName(pWnd->GetSafeHwnd(), className, 64);
		if (_tcsicmp(className, _T("Edit")) == 0)
		{
			CEdit* pEdit = (CEdit*)pWnd;
			int len = pEdit->GetWindowTextLength();
			pEdit->SetSel(len, len);
			CString line = str + _T("\r\n");
			pEdit->ReplaceSel(line);
		}
		else
		{
			((CListBox*)pWnd)->AddString(str);
		}
	}
}

BOOL CTCPAppADlg::OnInitDialog()
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
	WSADATA wsaData;

	int result = WSAStartup(
		MAKEWORD(2, 2),
		&wsaData
	);

	if (result != 0)
	{
		AfxMessageBox(_T("Khong khoi tao duoc Winsock!"));
	}
	else
	{
		m_isServer = true;
		SetDlgItemText(IDC_EDIT_PORT, _T("5000"));
		SetDlgItemText(IDC_EDIT_IP, _T("0.0.0.0 (Server)"));
		AppendChat(this, _T("[Thong bao] TCPAppA la Server. Hay bam Connect de bat dau lang nghe."));
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTCPAppADlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTCPAppADlg::OnPaint()
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
HCURSOR CTCPAppADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool SendAll(SOCKET s, const char* data, int length)
{
	int sent = 0;

	while (sent < length)
	{
		int n = send(
			s,
			data + sent,
			length - sent,
			0
		);

		if (n == SOCKET_ERROR || n == 0)
			return false;

		sent += n;
	}

	return true;
}

bool RecvAll(SOCKET s, char* data, int length)
{
	int received = 0;

	while (received < length)
	{
		int n = recv(
			s,
			data + received,
			length - received,
			0
		);

		if (n <= 0)
			return false;

		received += n;
	}

	return true;
}

bool SendMessageTCP(
	SOCKET s,
	const std::string& message)
{
	if (message.size() > 1024 * 1024)
		return false; // Gioi han 1 MB

	uint32_t length =
		htonl(static_cast<uint32_t>(message.size()));

	if (!SendAll(
		s,
		reinterpret_cast<const char*>(&length),
		sizeof(length)))
	{
		return false;
	}

	if (!message.empty())
	{
		if (!SendAll(
			s,
			message.data(),
			static_cast<int>(message.size())))
		{
			return false;
		}
	}

	return true;
}

bool RecvMessageTCP(
	SOCKET s,
	std::string& message)
{
	uint32_t netLength = 0;

	if (!RecvAll(
		s,
		reinterpret_cast<char*>(&netLength),
		sizeof(netLength)))
	{
		return false;
	}

	uint32_t length = ntohl(netLength);

	if (length > 1024 * 1024)
		return false;

	message.resize(length);

	if (length > 0)
	{
		if (!RecvAll(
			s,
			&message[0],
			static_cast<int>(length)))
		{
			return false;
		}
	}

	return true;
}

void CTCPAppADlg::StartServer()
{
	m_isServer = true;

	SOCKET serverSocket = socket(
		AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP
	);

	if (serverSocket == INVALID_SOCKET)
	{
		PostMessage(
			WM_CHAT_STATUS, 0,
			(LPARAM)new CString(_T("Tao socket that bai"))
		);
		return;
	}

	m_listenSocket = serverSocket;

	CString portText;
	GetDlgItemText(IDC_EDIT_PORT, portText);
	int port = _ttoi(portText);
	if (port <= 0 || port > 65535)
		port = 5000;

	sockaddr_in serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(
		serverSocket,
		(sockaddr*)&serverAddr,
		sizeof(serverAddr)) == SOCKET_ERROR)
	{
		PostMessage(
			WM_CHAT_STATUS, 0,
			(LPARAM)new CString(_T("Bind that bai"))
		);

		closesocket(serverSocket);
		m_listenSocket = INVALID_SOCKET;
		return;
	}

	if (listen(serverSocket, 1) == SOCKET_ERROR)
	{
		PostMessage(
			WM_CHAT_STATUS, 0,
			(LPARAM)new CString(_T("Listen that bai"))
		);

		closesocket(serverSocket);
		m_listenSocket = INVALID_SOCKET;
		return;
	}

	CString msg;
	msg.Format(_T("Server dang cho Client ket noi tai cong %d..."), port);
	PostMessage(
		WM_CHAT_STATUS, 0,
		(LPARAM)new CString(msg)
	);

	sockaddr_in clientAddr{};
	int addrLen = sizeof(clientAddr);

	SOCKET clientSocket = accept(
		serverSocket,
		(sockaddr*)&clientAddr,
		&addrLen
	);

	if (clientSocket == INVALID_SOCKET)
	{
		PostMessage(
			WM_CHAT_STATUS, 0,
			(LPARAM)new CString(_T("Accept that bai"))
		);
		return;
	}

	m_socket = clientSocket;

	PostMessage(
		WM_CHAT_STATUS, 0,
		(LPARAM)new CString(_T("Client da ket noi!"))
	);

	ReceiveLoop(clientSocket);
}

void CTCPAppADlg::ReceiveLoop(SOCKET s)
{
	while (m_running)
	{
		std::string message;

		if (!RecvMessageTCP(s, message))
			break;

		CStringA temp(message.c_str());

		CString* text = new CString(_T("[Peer] "));
		*text += CString(temp);

		if (!PostMessage(
			WM_CHAT_TEXT, 0, (LPARAM)text))
		{
			delete text;
		}
	}

	PostMessage(
		WM_CHAT_STATUS, 0,
		(LPARAM)new CString(_T("Ket noi da dong"))
	);
	m_running = false;
	m_socket = INVALID_SOCKET;
}
LRESULT CTCPAppADlg::OnChatText(
	WPARAM, LPARAM lParam)
{
	CString* text = (CString*)lParam;

	if (text != nullptr)
	{
		AppendChat(this, *text);
		delete text;
	}

	return 0;
}

LRESULT CTCPAppADlg::OnChatStatus(
	WPARAM, LPARAM lParam)
{
	CString* text = (CString*)lParam;

	if (text != nullptr)
	{
		AppendChat(this, *text);
		delete text;
	}

	return 0;
}
void CTCPAppADlg::OnBnClickedButtonConnect()
{
	if (m_networkThread.joinable())
	{
		if (m_running)
		{
			AfxMessageBox(_T("Luong mang dang chay."));
			return;
		}
		m_networkThread.join();
	}

	m_running = true;

	if (m_isServer)
	{
		m_networkThread = std::thread(
			[this]()
			{
				StartServer();
			}
		);
	}
	else
	{
		CString ipText;
		CString portText;

		GetDlgItemText(IDC_EDIT_IP, ipText);
		GetDlgItemText(IDC_EDIT_PORT, portText);

		int port = _ttoi(portText);

		if (port < 1 || port > 65535)
		{
			m_running = false;
			AfxMessageBox(_T("Cong khong hop le."));
			return;
		}

		CT2A ipA(ipText, CP_UTF8);
		std::string ip(ipA);

		m_networkThread = std::thread(
			[this, ip, port]()
			{
				ConnectToServer(ip, port);
			}
		);
	}
}

void CTCPAppADlg::OnBnClickedButtonSend()
{
	if (m_socket == INVALID_SOCKET)
	{
		AfxMessageBox(_T("Chua co ket noi TCP."));
		return;
	}

	CString text;
	GetDlgItemText(IDC_EDIT_MESSAGE, text);

	if (text.IsEmpty())
	{
		AfxMessageBox(_T("Vui long nhap tin nhan truoc khi gui!"));
		return;
	}

	CStringA textA(text);
	std::string message(textA.GetString());

	if (!SendMessageTCP(m_socket, message))
	{
		AfxMessageBox(_T("Gui tin nhan that bai."));
		return;
	}

	AppendChat(this, _T("[Me] ") + text);

	SetDlgItemText(IDC_EDIT_MESSAGE, _T(""));
}
void CTCPAppADlg::OnDestroy()
{
	m_running = false;

	if (m_socket != INVALID_SOCKET)
		shutdown(m_socket, SD_BOTH);

	if (m_listenSocket != INVALID_SOCKET)
		closesocket(m_listenSocket);

	if (m_socket != INVALID_SOCKET)
		closesocket(m_socket);

	if (m_networkThread.joinable())
		m_networkThread.join();

	m_socket = INVALID_SOCKET;
	m_listenSocket = INVALID_SOCKET;

	WSACleanup();

	CDialogEx::OnDestroy();
}
void CTCPAppADlg::ConnectToServer(
	const std::string& ip, int port)
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s == INVALID_SOCKET)
	{
		AfxMessageBox(_T("Tao socket that bai!"));
		return;
	}

	// 2. Thiet lap dia chi Server
	sockaddr_in serverAddr = {};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	if (inet_pton(AF_INET, ip.c_str(),
		&serverAddr.sin_addr) != 1)
	{
		AfxMessageBox(_T("Dia chi IP khong hop le!"));
		closesocket(s);
		return;
	}

	// 3. Ket noi den Server
	if (connect(s, (sockaddr*)&serverAddr,
		sizeof(serverAddr)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("Ket noi Server that bai!"));
		closesocket(s);
		return;
	}

	// 4. Luu socket ket noi
	m_socket = s;

	AfxMessageBox(_T("Ket noi Server thanh cong!"));

	// 5. Nhan tin nhan trong luong mang
	ReceiveLoop(s);
}