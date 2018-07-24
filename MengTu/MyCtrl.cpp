#include "StdAfx.h"
#include "MyCtrl.h"
#include <GdiPlus.h>
using namespace Gdiplus;


CMyCtrl::CMyCtrl(void)
{
	m_pActiveChild = NULL;
	m_nBorderWidth = 1;
}


CMyCtrl::~CMyCtrl(void)
{
}

void CMyCtrl::Draw( CDC *pDC )
{
// 	CBrush brush;
// 	brush.CreateSolidBrush(m_clrBg);
// 	pDC->FillRect(m_rc,&brush);
// 	brush.DeleteObject();
	Graphics graphics(pDC->GetSafeHdc());
	Pen newPen(m_clrBorder,m_nBorderWidth);//���ʣ����һ�����������ʴ�С
	Status ret = graphics.DrawRectangle(&newPen,m_rc.left,m_rc.top,m_rc.Width(),m_rc.Height());//��һ������
	TRACE("status:%d\n",ret);
}

void CMyCtrl::OnMouseOver( CPoint pt )
{
	// ����Ŀؼ�
	if ( m_pActiveChild ){
		if ( m_pActiveChild->IsPointOn(pt) ){
			m_pActiveChild->OnMouseOver(pt);
			return ;
		}		
		else{
			m_pActiveChild->OnMouseOut(pt);
			m_pActiveChild = NULL;
		}
	}

	// û�м�����ӿؼ�
	auto it = m_children.begin();
	for ( ;it!=m_children.end();++it ){
		if ( (*it)->IsPointOn(pt) ){
			m_pActiveChild = (*it);
			(*it)->OnMouseOver(pt);
			break;
		}
	}
}

void CMyCtrl::OnMouseUp( CPoint pt )
{
	auto it = m_children.begin();
	for ( ;it!=m_children.end();++it ){
		if ( (*it)->IsPointOn(pt) ){
			(*it)->OnMouseUp(pt);
			return;
		}
	}

	// ��갴�º͵����ڱ��ؼ������ڲ��㵥��
	if ( m_rc.PtInRect(m_ptMouseDown) && m_rc.PtInRect(pt) ){
		OnClick(pt);
	}
}

void CMyCtrl::OnMouseDown( CPoint pt )
{
	m_ptMouseDown = pt;

	auto it = m_children.begin();
	for ( ;it!=m_children.end();++it ){
		if ( (*it)->IsPointOn(pt) ){
			(*it)->OnMouseUp(pt);
			return;
		}
	}
}

void CMyCtrl::OnClick( CPoint pt )
{

}

void CMyCtrl::Move( int x, int y )
{
	m_rc.MoveToXY(x,y);
}

void CMyCtrl::OnMouseOut( CPoint pt )
{

}

void CMyCtrl::OnMouseMove( CPoint pt )
{

}

void CMyCtrl::SetRect( int x, int y, int cx, int cy )
{
	m_rc.left = x;
	m_rc.top  = y;
	m_rc.right = x+cx;
	m_rc.bottom = y+cy;
}

void CMyCtrl::OnDrag( CPoint ptStart, CPoint ptDraging )
{

}

bool CMyCtrl::IsPointOn( CPoint pt )
{
	return m_rc.PtInRect(pt)==TRUE;
}
