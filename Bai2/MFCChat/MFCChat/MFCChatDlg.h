
// MFCChatDlg.h : header file
//

#pragma once


// CMFCChatDlg dialog
class CMFCChatDlg : public CDialogEx
{
// Construction
public:
	CMFCChatDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCCHAT_DIALOG };
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
	afx_msg void OnEnChangeEditUsername();
	afx_msg void OnBnClickedButtonRegister();
	afx_msg void OnBnClickedButtonLogin();
};
