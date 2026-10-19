
// TCPAppBDlg.h : header file
//
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <atomic>
#include <string>
#pragma once
#define WM_CHAT_TEXT   (WM_APP + 1)
#define WM_CHAT_STATUS (WM_APP + 2)

// CTCPAppBDlg dialog
class CTCPAppBDlg : public CDialogEx
{
// Construction
public:
	CTCPAppBDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCPAPPB_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	SOCKET m_socket = INVALID_SOCKET;
	SOCKET m_listenSocket = INVALID_SOCKET;
	std::atomic<bool> m_running{ false };
	std::thread m_networkThread;
	bool m_isServer = false;

	afx_msg LRESULT OnChatText(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnChatStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	void ReceiveLoop(SOCKET s);
	void StartServer();
	void ConnectToServer(const std::string& ip, int port);
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonSend();
};
