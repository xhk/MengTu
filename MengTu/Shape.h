#pragma once
#include <vector>
using namespace std;


class CShapeProperty{
	int m_width;
	COLORREF m_color;
};

class CShape
{
public:
	CShape(int nBorderWidth);
	~CShape(void);

	virtual void Draw(CDC* pDC) = 0;

	virtual void OnMouseDrawBegin(CPoint pt);
	virtual void OnMouseDrawing(CPoint pt);
	virtual void OnMouseDrawEnd(CPoint pt);

protected :
	CRect m_rc;
	CPen m_borderPen;
};

class CRectangle : public CShape{
public:
	CRectangle(int nBorderWidth)
    : CShape(nBorderWidth)
    {
		m_brsh.CreateSolidBrush(RGB(255,0,0));
		
	}
	~CRectangle(){
		m_brsh.DeleteObject();
	}
	void Draw(CDC* pDC);
private:
	CBrush m_brsh;
	
};

class CEllispe : public CShape{
public:
	CEllispe(int nBorderWidth)
    :CShape(nBorderWidth)
    {
        
    }
	~CEllispe(){}
	void Draw(CDC* pDC);
	
private:
    //CBrush m_brsh;
};

class CArrow : public CShape{
public:
	CArrow(int nBorderWidth)
        :CShape(nBorderWidth)
    {}
	~CArrow(){}

	void Draw(CDC *pDC);
private:
	void LineArrow(CPoint p1,CPoint p2,CDC *pDC);
};

class CBrushPen : public CShape{
public:
    CBrushPen(int nBorderWidth):CShape(nBorderWidth){}
	void Draw(CDC *pDC);	

	virtual void OnMouseDrawBegin(CPoint pt);
	virtual void OnMouseDrawing(CPoint pt);
	virtual void OnMouseDrawEnd(CPoint pt);
private:
	vector<CPoint> m_points;
};