
// MengTuDlg.h : ͷ�ļ�
//

#pragma once

#include "NotifyStatic.h"
#include "afxwin.h"
#include "CaptureDlg.h"

#define MSGID_WAKEUP (WM_USER+1002) // ����

// CMengTuDlg �Ի���
class CMengTuDlg : public CDialogEx
{
// ����
public:
	CMengTuDlg(CWnd* pParent = NULL);	// ��׼���캯��
    ~CMengTuDlg(){
        if ( m_pCapWnd ){
            delete m_pCapWnd;
        }
    }
// �Ի�������
	enum { IDD = IDD_MENGTU_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
