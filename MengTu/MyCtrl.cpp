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
	Pen newPen(m_clrBorder,m_nBorderWidth);//画笔，最后一个参数，画笔大小
	Status ret = graphics.DrawRectangle(&newPen,m_rc.left,m_rc.top,m_rc.Width(),m_rc.Height());//画一个矩形
	TRACE("status:%d\n",ret);
}

void CMyCtrl::OnMouseOver( CPoint pt )
{
	// 激活的控件
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

	// 没有激活的子控件
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

	// 鼠标按下和弹起都在本控件区域内才算单击
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
