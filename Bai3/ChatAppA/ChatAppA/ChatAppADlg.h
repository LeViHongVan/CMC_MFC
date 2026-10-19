
// ChatAppADlg.h : header file
//

#pragma once


// CChatAppADlg dialog
class CChatAppADlg : public CDialogEx
{
// Construction
public:
	CChatAppADlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATAPPA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	afx_msg BOOL OnCopyData(
		CWnd* pWnd,
		COPYDATASTRUCT* pCopyDataStruct
	);
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit2();
	CEdit m_editMessage;
	CEdit m_editChat;
	afx_msg void OnBnClickedButtonSend();
};
