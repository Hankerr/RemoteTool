
// RemoteToolDlg.h: 头文件
//

#pragma once


// CRemoteToolDlg 对话框
class CRemoteToolDlg : public CDialogEx
{
#define WM_SYSTEMTRAY WM_USER+5

// 构造
public:
	CRemoteToolDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMOTETOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnStop();
	// 远程工具运行状态
	CString m_ExeStatus;
	BOOL bExeStart;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	CString app;
	CString arg;
	DWORD dwExitCode;
	afx_msg void OnClose();
private:
	NOTIFYICONDATA NotifyIcon;  // 系统托盘类
protected:
	afx_msg LRESULT OnSystemtray(WPARAM wParam, LPARAM lParam); // 响应鼠标的操作
public:
	afx_msg void OnDestroy();
};
