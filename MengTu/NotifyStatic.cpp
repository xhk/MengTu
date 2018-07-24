// NotifyStatic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MengTu.h"
#include "NotifyStatic.h"
#include "windowsx.h"

// CNotifyStatic

IMPLEMENT_DYNAMIC(CNotifyStatic, CStatic)

CNotifyStatic::CNotifyStatic()
{

}

CNotifyStatic::~CNotifyStatic()
{
}


BEGIN_MESSAGE_MAP(CNotifyStatic, CStatic)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CNotifyStatic ��Ϣ�������




void CNotifyStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    SetCapture();
    MSG msg = {0};
    msg.message = WM_LBUTTONDOWN;
    GetParent()->SendMessage(MSGID_STATIC_NOTIFY,(WPARAM)&msg,(LPARAM)GetDlgCtrlID());
    CStatic::OnLButtonDown(nFlags, point);
}


void CNotifyStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    ReleaseCapture();
    MSG msg = {0};
    msg.message = WM_LBUTTONUP;
    GetParent()->SendMessage(MSGID_STATIC_NOTIFY,(WPARAM)&msg,(LPARAM)GetDlgCtrlID());
    CStatic::OnLButtonUp(nFlags, point);
}

