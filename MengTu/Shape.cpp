#include "StdAfx.h"
#include "Shape.h"
#include <math.h>

CShape::CShape(int nBorderWidth)
{
	m_borderPen.CreatePen(PS_SOLID|PS_ENDCAP_SQUARE,nBorderWidth,RGB(255,0,0));
}


CShape::~CShape(void)
{
		m_borderPen.DeleteObject();
}

void CShape::OnMouseDrawBegin( CPoint pt )
{
	m_rc.top = pt.y;
	m_rc.left = pt.x;
	m_rc.right = pt.x;
	m_rc.bottom = pt.y;
}

void CShape::OnMouseDrawing( CPoint pt )
{
	m_rc.right = pt.x;
	m_rc.bottom = pt.y;
}

void CShape::OnMouseDrawEnd( CPoint pt )
{
	m_rc.right = pt.x;
	m_rc.bottom = pt.y;
}

void CRectangle::Draw( CDC* pDC )
{
	CGdiObject *pOld = pDC->SelectObject(&m_borderPen);

	pDC->MoveTo(m_rc.TopLeft());
	pDC->LineTo(m_rc.right,m_rc.top);
	pDC->LineTo(m_rc.right,m_rc.bottom);
	pDC->LineTo(m_rc.left,m_rc.bottom);
	pDC->LineTo(m_rc.left,m_rc.top);

	pDC->SelectObject(pOld);
}

void CEllispe::Draw( CDC* pDC )
{
    // 画刷为空，可画出空心圆
	CGdiObject *pOld = pDC->SelectStockObject(NULL_BRUSH);
	pDC->SelectObject(&m_borderPen);
    pDC->Ellipse(&m_rc);
	pDC->SelectObject(pOld);
	
}

void CArrow::Draw( CDC *pDC )
{
// 	pDC->MoveTo(m_rc.TopLeft());
// 	pDC->LineTo(m_rc.BottomRight());
// 
// 	pDC->Polygon( &m_rc.BottomRight(), 3 );
	CGdiObject *pOld = pDC->SelectObject(&m_borderPen);
	LineArrow(m_rc.TopLeft(),m_rc.BottomRight(),pDC);
	pDC->SelectObject(pOld);	
}

void CArrow::LineArrow(CPoint m_One,CPoint m_Two,CDC *pdc)
{
	double slopy , cosy , siny;
	double Par = 10.0;  //length of Arrow (>)
	slopy = atan2( (double)( m_One.y - m_Two.y ),(double)( m_One.x - m_Two.x ) );
	cosy = cos( slopy );
	siny = sin( slopy ); //need math.h for these functions

	//draw a line between the 2 endpoint
	pdc->MoveTo( m_One );
	pdc->LineTo( m_Two );
  
	//here is the tough part - actually drawing the arrows
	//a total of 6 lines drawn to make the arrow shape
// 	pdc->MoveTo( m_One);
// 	pdc->LineTo( m_One.x + int( - Par * cosy - ( Par / 2.0 * siny ) ),
// 	m_One.y + int( - Par * siny + ( Par / 2.0 * cosy ) ) );
// 	pdc->LineTo( m_One.x + int( - Par * cosy + ( Par / 2.0 * siny ) ),
// 	m_One.y - int( Par / 2.0 * cosy + Par * siny ) );
// 	pdc->LineTo( m_One );
	/*/-------------similarly the the other end-------------/*/
	pdc->MoveTo( m_Two );
	pdc->LineTo( m_Two.x + int( Par * cosy - ( Par / 2.0 * siny ) ),
	m_Two.y + int( Par * siny + ( Par / 2.0 * cosy ) ) );
	pdc->LineTo( m_Two.x + int( Par * cosy + Par / 2.0 * siny ),
	m_Two.y - int( Par / 2.0 * cosy - Par * siny ) );
	pdc->LineTo( m_Two );
}

void CBrushPen::Draw( CDC *pDC )
{
	if ( m_points.size() < 2 ){ return ; }
	auto it = m_points.begin();
	auto ite = m_points.end();

	CGdiObject *pOld = pDC->SelectObject(&m_borderPen);
	pDC->MoveTo(*it);
	for ( ;it!=ite;++it ){
		pDC->LineTo(*it);
	}
	pDC->SelectObject(pOld);
}

void CBrushPen::OnMouseDrawBegin( CPoint pt )
{
	m_points.push_back(pt);
}

void CBrushPen::OnMouseDrawing( CPoint pt )
{
	m_points.push_back(pt);
}

void CBrushPen::OnMouseDrawEnd( CPoint pt )
{
	m_points.push_back(pt);
}
