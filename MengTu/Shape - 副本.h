#pragma once
class CShape
{
public:
    CShape(void);
    ~CShape(void);

    virtual void Draw(CDC *pDC);
    void OnMouseDrawBegin(CPoint pt);
    // 用鼠标绘制中
    void OnMouseDrawing(CPoint pt);
    void OnMouseDrawEnd(CPoint pt);
protected:
    CRect m_rc;
    CDC *m_pDc;
};

class CRectangle : CShape{
public:
    CRectangle();
    ~CRectangle();

    virtual void Draw(CDC *pDC);
    CBrush m_brh;
};

