#pragma once


// CNotifyStatic


#define MSGID_STATIC_NOTIFY (WM_USER+1001) // ֪ͨ��Ϣ wParam��MSGָ��Ϊ�� lParam �ſؼ��Լ���ID

class CNotifyStatic : public CStatic
{
	DECLARE_DYNAMIC(CNotifyStatic)

public:
	CNotifyStatic();
	virtual ~CNotifyStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


