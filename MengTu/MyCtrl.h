#pragma once
#include "resource.h"
#include <vector>
using namespace std;
class CMyCtrl
{
public:
	CMyCtrl(void);
	~CMyCtrl(void);

	virtual void OnMouseMove(CPoint pt);
	virtual void OnMouseOver(CPoint pt);
	virtual void OnMouseOut(CPoint pt);
	virtual void OnMouseUp(CPoint pt);
	virtual void OnMouseDown(CPoint pt);
	virtual void Draw(CDC *pDC);

	//
	// �϶�
	virtual void OnDrag( CPoint ptStart, CPoint ptDraging );

	//
	// ����
	virtual void OnClick( CPoint pt );

	// 
	// �ƶ�
	void Move( int x, int y );

	//
	// �������ڿؼ�������
	bool IsPointOn( CPoint pt );

	void SetRect( int x, int y, int cx, int cy );
	void SetBorderWidth( int nWidth) { m_nBorderWidth = nWidth;}
	void SetBorderColor(COLORREF clr) { m_clrBorder=clr; }
	void SetColor(COLORREF clr) { m_clrBg = clr; }
protected:
	CRect m_rc; // �ؼ�����
	int m_id;   // �ؼ�id
	vector<CMyCtrl*> m_children; // �ӿؼ��б�
	CMyCtrl *m_pActiveChild;
private:
	CPoint m_ptMouseDown;   // ��갴�µ�����
	int m_nBorderWidth;		// �߿���
	COLORREF m_clrBorder;   // �߿���ɫ
	COLORREF m_clrBg;       // ����ɫ
};







    // ������
    
    enum FunctionID{
        fi_rect,
        fi_ellipse,
        fi_arrow,
        fi_brush,
        fi_font,
        fi_cancel,
        fi_save,
        fi_exit,
        fi_ok
    };

    struct CtrlStatus{
        static const int normal = 0;
        static const int mouse_over = 1;   
        static const int chose = 2;        //ѡ��
    };

			// �������
	struct  PropertyPanel  
	{
		struct LineWidthCtrl 
		{
			CRect m_rc;
			int m_width;
			int m_status;
			LineWidthCtrl(){
				m_status = CtrlStatus::normal;
			}
		};
		     // ��ȣ���ϸ��
		COLORREF color;  // ��ɫ

		bool is_show;
		bool m_bEnable;
		CRect m_rc;
		CBrush m_brsh;
		CBrush m_selectBrsh;
		static const int width  = 100;  
		static const int height = 30;
		LineWidthCtrl m_lwCtrl[3];
		PropertyPanel(){
			is_show = false;
			m_rc.SetRect(0,0,100,30);
			m_brsh.CreateSolidBrush(RGB(10,128,128));
			m_selectBrsh.CreateSolidBrush(RGB(255,128,128));
			for ( int i=0;i<3;++i ){
				m_lwCtrl[i].m_width = i+1;
			}
			
		}

		~PropertyPanel(){
			m_brsh.DeleteObject();
			m_selectBrsh.DeleteObject();
		}

        int getBorderWidth(){
            for ( int i=0;i<3;++i ){
                if ( m_lwCtrl[i].m_status == CtrlStatus::chose ){
                    return m_lwCtrl[i].m_width;
                }
            }
            return 1;
        }

		void OnMouseOver(CPoint pt){
			for ( int i=0;i<3;++i ){
				if ( m_lwCtrl[i].m_rc.PtInRect(pt) ){
					if ( m_lwCtrl[i].m_status == CtrlStatus::normal ){
						m_lwCtrl[i].m_status = CtrlStatus::mouse_over;
					}
				}
				else{
					if( m_lwCtrl[i].m_status == CtrlStatus::mouse_over ){
						m_lwCtrl[i].m_status = CtrlStatus::normal;
					}
				}
			}
		}

		void OnLButtonUp(CPoint pt){
			for ( int i=0;i<3;++i ){
				if ( m_lwCtrl[i].m_rc.PtInRect(pt) ){
					m_lwCtrl[i].m_status = CtrlStatus::chose;
				}
				else{
					m_lwCtrl[i].m_status = CtrlStatus::normal;
				}
			}
		}

		void Draw(CDC *pDC){
			pDC->FrameRect(&m_rc,&m_brsh);
			CRect rc = m_rc;
			rc.DeflateRect(1,1);
			pDC->FillSolidRect(rc,RGB(180,241,255));

			int x = 5;
			for ( int i=0;i<3;++i ){
				m_lwCtrl[i].m_rc.left = m_rc.left+x;
				m_lwCtrl[i].m_rc.top  = m_rc.top+4;
				m_lwCtrl[i].m_rc.right = m_lwCtrl[i].m_rc.left+22;
				m_lwCtrl[i].m_rc.bottom = m_lwCtrl[i].m_rc.top+22;
				
				if ( m_lwCtrl[i].m_status == CtrlStatus::normal ){
					pDC->FrameRect(&m_lwCtrl[i].m_rc,&m_brsh);
				}
				else{
					pDC->FrameRect(&m_lwCtrl[i].m_rc,&m_selectBrsh);					
				}
				CRect rc;
				rc.left = (m_lwCtrl[i].m_rc.left + m_lwCtrl[i].m_rc.right)/2-m_lwCtrl[i].m_width;
				rc.top  = (m_lwCtrl[i].m_rc.top  + m_lwCtrl[i].m_rc.bottom)/2-m_lwCtrl[i].m_width;
				rc.right = rc.left + 2*(m_lwCtrl[i].m_width);
				rc.bottom = rc.top + 2*(m_lwCtrl[i].m_width);
				CGdiObject *pOld = pDC->SelectStockObject(BLACK_BRUSH);
                pDC->SelectStockObject(BLACK_PEN);
				pDC->Ellipse(rc);
				pDC->SelectObject(pOld);

				x += 22+5;
			}
		}

		void AdjustRect(CRect functionCtrlRect,int xScreen,int yScreen){
			if ( functionCtrlRect.top-30>0 ){
				m_rc.top  = functionCtrlRect.top-30;
				m_rc.left = functionCtrlRect.left;
				goto _result;
			}

			if ( functionCtrlRect.bottom+30<yScreen ){
				m_rc.top  = functionCtrlRect.bottom+1;
				m_rc.left = functionCtrlRect.left;
				goto _result;
			}

			m_rc.top  = functionCtrlRect.bottom+1;
			m_rc.left = functionCtrlRect.left;

_result:;
			m_rc.bottom = m_rc.top+height;
			m_rc.right  = m_rc.left+width;
		}

	};

    struct FunctionCtrl{
        //bool bUse;
        int status;
        CBitmap bmp;
		BITMAP m_bmpSize;
		CBitmap m_bgBmp;
		BITMAP m_bgBmpInfo;
        int x,y;
        int width;
        int height;
        int fun_id;

		FunctionCtrl(){
			status = CtrlStatus::normal;
			m_bgBmp.LoadBitmap(IDB_BITMAP_SELECTEDBG);
		}

		~FunctionCtrl(){
			m_bgBmp.DeleteObject();
		}

		

		CRect GetRect(){
            CRect rc(x,y,x+width,y+height);
            return rc;
        }

		void Draw(CDC *pDC,CDC *memDC){
			if (status==CtrlStatus::mouse_over || status==CtrlStatus::chose ){
			        // �Ȼ���ѡ�еı���
			        memDC->SelectObject(&m_bgBmp);
			        m_bgBmp.GetBitmap(&m_bgBmpInfo);
			        pDC->BitBlt(x-3,y-3,m_bgBmpInfo.bmWidth,m_bgBmpInfo.bmHeight,memDC,0,0,SRCCOPY);
			    }
			    memDC->SelectObject(&bmp);
			    bmp.GetBitmap(&m_bmpSize);
			    
			    width  = m_bmpSize.bmWidth;
			    height = m_bmpSize.bmHeight;

			    pDC->BitBlt(x,y,m_bmpSize.bmWidth,m_bmpSize.bmHeight,memDC,0,0,SRCCOPY);
		}

    };

   struct FunctionBar{
        static const int width=392;
        static const int height=28;
        int m_x,m_y;
        CBitmap bgBmp;
        int ctrl_count;
		int m_xScreen,m_yScreen;
		CRect m_rcSelected;
		
        FunctionCtrl ctrl[32];
		PropertyPanel propPanel[32]; // ÿ���ؼ������Լ����������
		int m_activeIndex;
		FunctionBar(){
			m_x = 0;m_y=0;
			m_activeIndex = -1;
		}

		PropertyPanel *getPropPanel(){
			for ( int i=0;i<ctrl_count;++i){
				if ( propPanel[i].m_bEnable && propPanel[i].is_show ){
					//TRACE("prop id %d\n",i);
					return &propPanel[i];
				}
			}
			return nullptr;
		}

		void SetXyScreen(int xScreen,int yScreen){
			m_xScreen = xScreen;
			m_yScreen = yScreen;
		}

		void SetSelectedRect(CRect rc){
			m_rcSelected = rc;
		}

        // ����ѡȡ���������Լ�������
        void ClacXyBySelectRect(CRect rc,int xScreen,int yScreen){
            // ������ռ��Ƿ�
			if ( rc.top-30 > 0 ){
				m_x = rc.left;
				m_y = rc.top-30;
				return ;
			}

			// �·�
			if ( rc.bottom + 30<yScreen ){
				m_x = rc.left;
				m_y = rc.bottom + 2;
				return ;
			}
			
			// �ҷ�
			if ( rc.right + 395<xScreen ){
				m_x = rc.right + 2;
				m_y = rc.top;
				return ;
			}

			// ��
			if ( rc.left-395 >0 ){
				m_x = rc.left -395;
				m_y = rc.top;
				return ;
			}

			// ������
			m_x = rc.left;
			m_y = 1;
        }

        CRect GetRect(){
            CRect rc(m_x,m_y,m_x+width,m_y+height);
            return rc;
        }

		void Reset(){
			for ( int i=0;i<ctrl_count;++i ){
				ctrl[i].status = CtrlStatus::normal;
			}
		}

		void Draw(CDC *pDC){
			ClacXyBySelectRect(m_rcSelected,m_xScreen,m_yScreen);

			CDC fbarDc;
			fbarDc.CreateCompatibleDC(pDC);
			CGdiObject *fbarOld = fbarDc.SelectObject(&bgBmp);
			BITMAP bmpSize;
			bgBmp.GetBitmap(&bmpSize);
			pDC->StretchBlt(m_x,m_y,width,height,&fbarDc,0,0,bmpSize.bmWidth,bmpSize.bmHeight,SRCCOPY);

			int x=m_x+10;
			int y=m_y+6;
			for(int i=0;i<ctrl_count;++i){
			    ctrl[i].x = x;
			    ctrl[i].y = y;
			    
				ctrl[i].Draw(pDC,&fbarDc);
				
			    x += 5+22;

				if ( propPanel[i].is_show && propPanel[i].m_bEnable ){
					propPanel[i].AdjustRect(GetRect(),m_xScreen,m_yScreen);
					propPanel[i].Draw(pDC);
				}
			}

			fbarDc.DeleteDC();
		}
    };

