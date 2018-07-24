#pragma once
#include "Shape.h"
#include "MyCtrl.h"
#include <vector>
using namespace  std;
// CCaptureDlg �Ի���

class CCaptureDlg : public CWnd
{
	DECLARE_DYNAMIC(CCaptureDlg)

public:
	CCaptureDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCaptureDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CAPTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
    void OnCancel();
    void OnOK();
    HBITMAP CopyScreenToBitmap(LPRECT lpRect,BOOL bSave);

    CBitmap *m_pBitmap;
    CBitmap *m_pBitmapRestore;
    //int m_xScreen;
    //int m_yScreen;
    CRect m_windowSelectedRect;   // ͨ������ѡ�������
	CRect m_mouseSelectedRect;    // ͨ�����ѡ�������
    CPoint m_movePoint;
	CPoint m_ptDragStart;		  // ��ʼ�϶������� 
    CDC m_dcBg;
    CDC m_dcLight; // ���������ͼ�ͻ�ͼ������
    CBitmap * m_pBmpLight;

    // ����ѡ��ģʽ
    enum SelectMode{
        WINDOW, // ����ѡ��,Ĭ��
        MOUSE,  // ����϶�ѡ��
    };

	inline CRect & GetSelectedRect();

    enum SelectStatus{
        //UN_SELECT,
        SELECTING,
        SELECTED,
        DRAW_SHAPE, //������״
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
    
    // 8���ȵ�����
    CRect m_rcHot[8];
    
    int m_nDragHotChangeIndex; //�϶��ȵ�ı��С 
    HCURSOR m_hArrow;
    HCURSOR m_hCross;
    HCURSOR m_hHand;
    HCURSOR m_hSize;
    HCURSOR m_hTopBottom; //��ֱ
    HCURSOR m_hLeftRight; //ˮƽ
    HCURSOR m_hLeftTop;
    HCURSOR m_hRightTop;



 

    FunctionBar m_fbar;
	
    vector<CShape*> m_shapeList;
    CShape *m_pDrawingShape;    //���ڻ��Ƶ���״
    int m_selectedShapeType;    //ѡ�е���״����
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
    int InHotRect(CPoint pt); // ���������ȵ�������������ȵ����򷵻�-1
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
