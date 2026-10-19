// MainDlg.cpp : implementation file
//

#include "pch.h"
#include "MFCChat.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include <list>


// MainDlg dialog

IMPLEMENT_DYNAMIC(MainDlg, CDialogEx)

MainDlg::MainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_DIALOG, pParent)
{

}

MainDlg::~MainDlg()
{
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE_FRIENDS, m_treeFriends);
	DDX_Control(pDX, IDC_LIST_USERS, m_listUsers);
}


BEGIN_MESSAGE_MAP(MainDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_SEND, &MainDlg::OnBnClickedButtonSend)
    ON_NOTIFY(NM_CLICK, IDC_LIST_USERS, &MainDlg::OnNMClickListUsers)
    //ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FRIENDS, &MainDlg::OnTvnSelchangedTreeFriends)
    //ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_USERS, &MainDlg::OnLvnItemchangedListUsers)
END_MESSAGE_MAP()

BOOL MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HTREEITEM hFriends = m_treeFriends.InsertItem(_T("Bạn bè"));

    m_treeFriends.InsertItem(
        _T("An"),
        hFriends
    );

    m_treeFriends.InsertItem(
        _T("Bình"),
        hFriends
    );

    m_treeFriends.InsertItem(
        _T("Chi"),
        hFriends
    );

    m_treeFriends.Expand(
        hFriends,
        TVE_EXPAND
    );

    m_listUsers.SetExtendedStyle(
        m_listUsers.GetExtendedStyle()
        | LVS_EX_FULLROWSELECT
        | LVS_EX_GRIDLINES
    );

    m_listUsers.InsertColumn(
        0,
        _T("Username"),
        LVCFMT_LEFT,
        120
    );

    m_listUsers.InsertColumn(
        1,
        _T("Status"),
        LVCFMT_LEFT,
        100
    );

    int index = m_listUsers.InsertItem(
        0,
        _T("An")
    );

    m_listUsers.SetItemText(
        index,
        1,
        _T("Online")
    );

    index = m_listUsers.InsertItem(
        1,
        _T("Bình")
    );

    m_listUsers.SetItemText(
        index,
        1,
        _T("Offline")
    );

    index = m_listUsers.InsertItem(
        2,
        _T("Chi")
    );

    m_listUsers.SetItemText(
        index,
        1,
        _T("Online")
    );

    /*CString chat;

    chat += _T("An: Hello!\r\n");
    chat += _T("Bạn: Hello An!\r\n");
    chat += _T("An: Khỏe khum?\r\n");
    chat += _T("Bạn: Bình thường\r\n");*/

    m_chatAn =
        _T("An: Hello!\r\n")
        _T("Bạn: Chào An!\r\n")
        _T("An: Khỏe khum?\r\n")
        _T("Bạn: Bình thường?\r\n");

    m_chatBinh =
        _T("Bình: Làm bài chưa?\r\n")
        _T("Bạn: Sao t phải làm?\r\n");

    m_chatChi =
        _T("Chi: Mai có học không?\r\n")
        _T("Bạn: Mai là ai?\r\n");

    /*SetDlgItemText(
        IDC_EDIT_CHAT,
        chat
    );*/

    return TRUE;
}
// MainDlg message handlers


void MainDlg::OnBnClickedButtonSend()
{
    CString message;

    GetDlgItemText(
        IDC_EDIT_MESSAGE,
        message
    );

    if (message.IsEmpty())
    {
        return;
    }

    CString newMessage;
    newMessage = _T("Bạn: ");
    newMessage += message;
    newMessage += _T("\r\n");

    if (m_currentUser == _T("An"))
    {
        m_chatAn += newMessage;

        SetDlgItemText(
            IDC_EDIT_CHAT,
            m_chatAn
        );
    }
    else if (m_currentUser == _T("Bình"))
    {
        m_chatBinh += newMessage;

        SetDlgItemText(
            IDC_EDIT_CHAT,
            m_chatBinh
        );
    }

    else if (m_currentUser == _T("Chi"))
    {
        m_chatChi += newMessage;

        SetDlgItemText(
            IDC_EDIT_CHAT,
            m_chatChi
        );
    }

    SetDlgItemText(
        IDC_EDIT_MESSAGE,
        _T("")
    );
}

void MainDlg::OnNMClickListUsers(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate =
        reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    int selectedIndex =
        pNMItemActivate->iItem;

    if (selectedIndex >= 0)
    {
        CString username =
            m_listUsers.GetItemText(
                selectedIndex,
                0
            );
        m_currentUser = username;
       
        if (username == _T("An"))
        {
            SetDlgItemText(
                IDC_EDIT_CHAT,
                m_chatAn
            );
        }
        else if (username == _T("Bình"))
        {
            SetDlgItemText(
                IDC_EDIT_CHAT,
                m_chatBinh
            );
        }
        else if (username == _T("Chi"))
        {
            SetDlgItemText(
                IDC_EDIT_CHAT,
                m_chatChi
            );
        }
    }
        *pResult = 0;
}

