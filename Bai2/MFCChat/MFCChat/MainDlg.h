#pragma once

// MainDlg dialog

class MainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MainDlg)

public:
	MainDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MainDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CTreeCtrl m_treeFriends;
	CListCtrl m_listUsers;
	CString m_chatAn;
	CString m_chatBinh;
	CString m_chatChi;
	CString m_currentUser;
public:
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnNMClickListUsers(
		NMHDR* pNMHDR,
		LRESULT* pResult
	);
	afx_msg void OnTvnSelchangedTreeFriends(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedListUsers(NMHDR* pNMHDR, LRESULT* pResult);
};
