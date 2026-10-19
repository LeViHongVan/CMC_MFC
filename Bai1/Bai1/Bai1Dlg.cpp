
// Bai1Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Bai1.h"
#include "Bai1Dlg.h"
#include "afxdialogex.h"
#include <shlobj.h>

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


// CBai1Dlg dialog



CBai1Dlg::CBai1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BAI1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBai1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBai1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, &CBai1Dlg::OnBnClickedButtonFolder)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CBai1Dlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CBai1Dlg::OnBnClickedButtonLoad)
END_MESSAGE_MAP()


// CBai1Dlg message handlers

BOOL CBai1Dlg::OnInitDialog()
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

void CBai1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBai1Dlg::OnPaint()
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
HCURSOR CBai1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBai1Dlg::OnBnClickedButtonFolder()
{
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = _T("Chọn thư mục lưu file");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != NULL)
	{
		TCHAR path[MAX_PATH];
		if (SHGetPathFromIDList(pidl, path))
		{
			CString folderPath(path);
			CString filePath = folderPath + _T("\\data.txt");
			SetDlgItemText(IDC_EDITPATH, filePath);
		}
		CoTaskMemFree(pidl);
	}
}

BOOL check(HANDLE hFile)
{
	if (hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("Không thể mở file!"));
		return FALSE;
	}
	return TRUE;
}

void CBai1Dlg::OnBnClickedButtonSave()
{
	CString content;
	CString filePath;

	GetDlgItemText(IDC_EDIT_CONTENT, content);
	GetDlgItemText(IDC_EDITPATH, filePath);

	if (filePath.IsEmpty())
	{
		AfxMessageBox(_T("Vui lòng chọn folder lưu file!"));
		return;
	}

	HANDLE hFile = CreateFile(
		filePath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (!check(hFile))
	{
		return;
	}

	DWORD bytesWritten = 0;

	BOOL result = WriteFile(
		hFile,
		content.GetString(),
		content.GetLength() * sizeof(TCHAR),
		&bytesWritten,
		NULL
	);

	if (!result)
	{
		AfxMessageBox(_T("Ghi file thất bại!"));
		CloseHandle(hFile);
		return;
	}

	CloseHandle(hFile);

	AfxMessageBox(_T("Lưu file thành công!"));
}


void CBai1Dlg::OnBnClickedButtonLoad()
{
	CString filePath;

	GetDlgItemText(IDC_EDITPATH, filePath);

	if (filePath.IsEmpty())
	{
		AfxMessageBox(_T("Vui lòng chọn file cần load!"));
		return; 
	}
	
	HANDLE hFile = CreateFile(
		filePath,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (!check(hFile))
	{
		return;
	}

	DWORD fileSize = GetFileSize(hFile, NULL);

	if (fileSize == INVALID_FILE_SIZE)
	{
		AfxMessageBox(_T("Không thể lấy kích thước file!"));
		CloseHandle(hFile);
		return;
	}

	if (fileSize == 0)
	{
		SetDlgItemText(IDC_EDIT_CONTENT, _T(""));
		CloseHandle(hFile);
		AfxMessageBox(_T("Load file thành công!"));
		return;
	}

	LPVOID buffer = HeapAlloc(
		GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		fileSize + sizeof(TCHAR)
	);

	if (buffer == NULL)
	{
		AfxMessageBox(_T("Không thể cấp phát bộ nhớ!"));
		CloseHandle(hFile);
		return;
	}

	DWORD bytesRead = 0;

	BOOL result = ReadFile(
		hFile,
		buffer,
		fileSize,
		&bytesRead,
		NULL
	);

	if (!result)
	{
		AfxMessageBox(_T("Đọc file thất bại!"));
		HeapFree(GetProcessHeap(), 0, buffer);
		CloseHandle(hFile);

		return;
	}

	CloseHandle(hFile);

	CString content(
		static_cast<LPCTSTR>(buffer),
		bytesRead / sizeof(TCHAR)
	);

	SetDlgItemText(IDC_EDIT_CONTENT, content);

	HeapFree(GetProcessHeap(), 0, buffer);
	AfxMessageBox(_T("Load file thành công!"));
}
