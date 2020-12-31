
// RemoteToolDlg.h: 头文件
//

#pragma once

// 引入boost库
#include <fstream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#define BOOST_DATE_TIME_SOURCE

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;

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
	// 系统托盘类
	NOTIFYICONDATA NotifyIcon;
protected:
	// 响应鼠标的操作
	afx_msg LRESULT OnSystemtray(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
public:
	void LogInit();
	src::logger lg;
};
