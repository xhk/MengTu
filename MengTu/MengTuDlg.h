
// MengTuDlg.h : 头文件
//

#pragma once

#include "NotifyStatic.h"
#include "afxwin.h"
#include "CaptureDlg.h"

#define MSGID_WAKEUP (WM_USER+1002) // 唤醒

// CMengTuDlg 对话框
class CMengTuDlg : public CDialogEx
{
// 构造
public:
	CMengTuDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CMengTuDlg(){
        if ( m_pCapWnd ){
            delete m_pCapWnd;
        }
    }
// 对话框数据
	enum { IDD = IDD_MENGTU_DIALOG };

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
    afx_msg LRESULT OnMsgStaticNotify(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnMsgWakeup(WPARAM wp,LPARAM lp);
	DECLARE_MESSAGE_MAP()

    void OnStaticLButtonDown(int nCtrlID);
    void OnStaticLButtonUp(int nCtrlID);
    void Scan();
private:
    CNotifyStatic m_ntStatic;
    HCURSOR m_hChangeWndSize;
    HCURSOR m_hRestoreCursor;
    CWnd *m_dstWnd;
    CCaptureDlg *m_pCapWnd;
    HBITMAP CaptureWindow( HWND hWnd,bool bClipborad=false );
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
    afx_msg void OnBnClickedCheckCapture();
    afx_msg void OnEnChangeEdit1();
    afx_msg void OnEnChangeEdit2();
    afx_msg void OnBnClickedCheckRestor();
    void DoCapture();
    CNotifyStatic m_findStatic;
};
