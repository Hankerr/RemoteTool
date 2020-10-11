
// RemoteToolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RemoteTool.h"
#include "RemoteToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CRemoteToolDlg 对话框



CRemoteToolDlg::CRemoteToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REMOTETOOL_DIALOG, pParent)
	, m_ExeStatus(_T("")), bExeStart(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_STATUS, m_ExeStatus);
}

BEGIN_MESSAGE_MAP(CRemoteToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CRemoteToolDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_STOP, &CRemoteToolDlg::OnBnClickedBtnStop)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_SYSTEMTRAY, &CRemoteToolDlg::OnSystemtray)
END_MESSAGE_MAP()


// CRemoteToolDlg 消息处理程序

BOOL CRemoteToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_BTN_OPEN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

	//设置系统托盘
	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	NotifyIcon.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon.hIcon = m_hIcon;  // 上面那句也可以
	NotifyIcon.hWnd = m_hWnd;
	lstrcpy(NotifyIcon.szTip, _T("运行中..."));
	NotifyIcon.uCallbackMessage = WM_SYSTEMTRAY;
	NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);   // 添加系统托盘

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRemoteToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRemoteToolDlg::OnPaint()
{
	if (IsIconic()) {
		CPaintDC dc(this); // 用于绘制的设备上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialogEx::OnPaint();
		CPaintDC   dc(this);
		CRect   rect;
		GetClientRect(&rect);    //获取对话框长宽      
		CDC   dcBmp;             //定义并创建一个内存设备环境
		dcBmp.CreateCompatibleDC(&dc);             //创建兼容性DC
		CBitmap   bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP1);    //载入资源中图片
		BITMAP   m_bitmap;                         //图片变量               
		bmpBackground.GetBitmap(&m_bitmap);       //将图片载入位图中
		//将位图选入临时内存设备环境
		CBitmap  *pbmpOld = dcBmp.SelectObject(&bmpBackground);
		//调用函数显示图片StretchBlt显示形状可变
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcBmp, 0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRemoteToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRemoteToolDlg::OnBnClickedBtnOpen()
{
	si = { 0 };
	pi = { 0 };
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESHOWWINDOW;

	//TCHAR app[] = TEXT(".\\ExePath\\frp_0.31.2_windows_amd64\\frpc.exe");
	arg = _T(".\\ExePath\\frp_0.31.2_windows_amd64\\frpc.exe -c .\\ExePath\\frp_0.31.2_windows_amd64\\frpc.ini");

	bExeStart = CreateProcess(NULL, arg.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	arg.ReleaseBuffer();
	if (bExeStart)
	{
		m_ExeStatus.Empty();
		m_ExeStatus = _T("程序运行中...");
		SetDlgItemText(IDC_EDT_STATUS, m_ExeStatus);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_OPEN)->EnableWindow(FALSE);
	}
	else {
		AfxMessageBox(_T("程序启动失败,请重新启动!!!"));
	}
}


void CRemoteToolDlg::OnBnClickedBtnStop()
{
	//系统通知窗口
	this->ShowWindow(HIDE_WINDOW);

	GetDlgItem(IDC_BTN_OPEN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
}


void CRemoteToolDlg::OnClose()
{
	// 关闭程序调用frpc结束
	if (bExeStart) {
		BOOL bret = TerminateProcess(pi.hProcess, 0);
	}
	CDialogEx::OnClose();
}


LRESULT CRemoteToolDlg::OnSystemtray(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case WM_RBUTTONUP:// 右键起来时弹出菜单
	{
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);// 得到鼠标位置
		CMenu menu;
		menu.CreatePopupMenu();// 声明一个弹出式菜单
		menu.AppendMenu(MF_STRING, WM_DESTROY, _T("关闭"));
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		HMENU hmenu = menu.Detach();
		menu.DestroyMenu();
		delete lpoint;
		GetDlgItem(IDC_BTN_OPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	}
	break;
	case WM_LBUTTONDBLCLK:// 双击左键的处理
	{
		GetDlgItem(IDC_BTN_OPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
		this->ShowWindow(SW_SHOWNORMAL);// 显示主窗口
	}
	break;
	}
	return 0;
}

void CRemoteToolDlg::OnDestroy()
{
	Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);//消除托盘图标
	if (bExeStart) {
		BOOL bret = TerminateProcess(pi.hProcess, 0);
	}

	CDialogEx::OnDestroy();
}
