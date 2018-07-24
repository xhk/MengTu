#pragma once
#include "Shape.h"
#include "MyCtrl.h"
#include <vector>
using namespace  std;
// CCaptureDlg 对话框

class CCaptureDlg : public CWnd
{
	DECLARE_DYNAMIC(CCaptureDlg)

public:
	CCaptureDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCaptureDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CAPTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
    void OnCancel();
    void OnOK();
    HBITMAP CopyScreenToBitmap(LPRECT lpRect,BOOL bSave);

    CBitmap *m_pBitmap;
    CBitmap *m_pBitmapRestore;
    //int m_xScreen;
    //int m_yScreen;
    CRect m_windowSelectedRect;   // 通过窗口选择的区域
	CRect m_mouseSelectedRect;    // 通过鼠标选择的区域
    CPoint m_movePoint;
	CPoint m_ptDragStart;		  // 开始拖动的坐标 
    CDC m_dcBg;
    CDC m_dcLight; // 用来储存截图和画图的内容
    CBitmap * m_pBmpLight;

    // 区域选择模式
    enum SelectMode{
        WINDOW, // 窗口选择,默认
        MOUSE,  // 鼠标拖动选择
    };

	inline CRect & GetSelectedRect();

    enum SelectStatus{
        //UN_SELECT,
        SELECTING,
        SELECTED,
        DRAW_SHAPE, //绘制形状
    };

// 	enum DrawStatus{
// 		draw_begin,
// 		drawing,
// 		draw_end
// 	};

    int m_select_mode;
    int m_select_status;
	int m_drawStatus;
    bool m_bDrawingShape;
    
    // 8个热点区域
    CRect m_rcHot[8];
    
    int m_nDragHotChangeIndex; //拖动热点改变大小 
    HCURSOR m_hArrow;
    HCURSOR m_hCross;
    HCURSOR m_hHand;
    HCURSOR m_hSize;
    HCURSOR m_hTopBottom; //竖直
    HCURSOR m_hLeftRight; //水平
    HCURSOR m_hLeftTop;
    HCURSOR m_hRightTop;



 

    FunctionBar m_fbar;
	
    vector<CShape*> m_shapeList;
    CShape *m_pDrawingShape;    //正在绘制的形状
    int m_selectedShapeType;    //选中的形状类型
    CShape *Fac_CreateShape(int type);
    CFont m_waterMarkFont;
public:
    //virtual BOOL OnInitDialog();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    int InHotRect(CPoint pt); // 返回所在热点的索引，不在热点上则返回-1
    void OnFunctionSavePic();
    void OnFunctionOk();
    void OnFunctionCancel();

    void OnDrawShape(int type);

	void OnDrawShapeBegin();
	void OnDrawShapeEnd();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void OnDrag(CPoint lastPonit,CPoint curPoint);
	void OnMouseOver(CPoint lastPonit,CPoint curPoint);

    int getBorderWidth(int type);

    CBitmap *genBitmap();
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};
