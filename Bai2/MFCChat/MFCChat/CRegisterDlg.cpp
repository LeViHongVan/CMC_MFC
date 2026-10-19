// CRegisterDlg.cpp : implementation file
//

#include "pch.h"
#include "MFCChat.h"
#include "CRegisterDlg.h"
#include "afxdialogex.h"


// CRegisterDlg dialog

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGISTER_DIALOG, pParent)
{

}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER_OK, &CRegisterDlg::OnBnClickedButtonRegisterOk)
END_MESSAGE_MAP()


// CRegisterDlg message handlers


void CRegisterDlg::OnBnClickedButtonRegisterOk()
{
	CString username;
	CString password;
	CString confirm;

	GetDlgItemText(
		IDC_EDIT_REG_USERNAME,
		username
	);

	GetDlgItemText(
		IDC_EDIT_REG_PASSWORD,
		password
	);

	GetDlgItemText(
		IDC_EDIT_REG_CONFIRM,
		confirm
	);

	if (username.IsEmpty() || password.IsEmpty() || confirm.IsEmpty())
	{
		AfxMessageBox(_T("Vui lòng nhập đầy đủ thông tin!"));
		return;
	}
	if (password != confirm)
	{
		AfxMessageBox(_T("Mật khẩu xác nhận không đúng!"));
		return;
	}

	//m_username = username;
	//m_password = password;

	AfxMessageBox(_T("Đăng ký thành công!"));

	CDialogEx::OnOK();
}
