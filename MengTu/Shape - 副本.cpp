#include "StdAfx.h"
#include "Shape.h"


CShape::CShape(void)
{
    
}


CShape::~CShape(void)
{
}

void CShape::OnMouseDrawing( CPoint pt )
{
    m_rc.top  = pt.y;
    m_rc.left = pt.x;
}

void CShape::OnMouseDrawBegin( CPoint pt )
{
    m_rc.bottom = pt.y;
    m_rc.right  = pt.x;
}

void CShape::OnMouseDrawEnd( CPoint pt )
{
    m_rc.bottom = pt.y;
    m_rc.right  = pt.x;
}

void CRectangle::Draw(CDC *pDC)
{
    
    pDC->FrameRect(&m_rc,&m_brh);
}

CRectangle::CRectangle()
{
    m_brh.CreateSolidBrush(RGB(255,0,0));
}

CRectangle::~CRectangle()
{
    m_brh.DeleteObject();
}
