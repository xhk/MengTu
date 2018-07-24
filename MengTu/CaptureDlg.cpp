// CaptureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MengTu.h"
#include "CaptureDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <WindowsX.h>
#include "ImgTrans.h"
#include "Config.h"

using namespace std;
vector<CRect> g_wndrect_list;

//
// �����Ƿ��ڵ�
bool IsCoveredByOtherWindow(HWND hWnd)
{
 RECT rcTarget; 
 ::GetWindowRect(hWnd, &rcTarget); 

 bool isChild = (WS_CHILD == (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD)); 

 if (::GetDesktopWindow() == hWnd)
  hWnd = ::GetWindow(::GetTopWindow(hWnd), GW_HWNDLAST);

 do{ 
  HWND hCurWnd = hWnd; 

  while(NULL != (hWnd = ::GetNextWindow(hWnd, GW_HWNDPREV))){ 
   if (::IsWindowVisible(hWnd)){ 
    RECT rcWnd; 
    ::GetWindowRect(hWnd, &rcWnd); 

    if(!((rcWnd.right < rcTarget.left) || (rcWnd.left > rcTarget.right) || 
     (rcWnd.bottom < rcTarget.top) || (rcWnd.top > rcTarget.bottom))){ 
     return true; 
    } 
   } 
  } 

  if(isChild){ 
   hWnd = ::GetParent(hCurWnd); 
   isChild = hWnd ? (WS_CHILD == (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD)) : false; 
  } 
  else 
   break; 
 }while(true); 

 return false;
}


    //�ص�����  
BOOL CALLBACK MyEnumWindowsProc(HWND hwnd,LPARAM lParam)  
{  
	CCaptureDlg *pWnd = (CCaptureDlg*)lParam;
    if (::IsWindow(hwnd) && ::IsWindowVisible(hwnd))  
    {  
		if ( /*!IsCoveredByOtherWindow(hwnd)*/1 ){
			CRect tempRect;  
			::GetWindowRect(hwnd,&tempRect);

			// ת��Ϊ��ͼ���ڵ�����
			pWnd->ScreenToClient(&tempRect);
			g_wndrect_list.push_back(tempRect);  
			::EnumChildWindows(hwnd,(WNDENUMPROC)MyEnumWindowsProc,0);
		}

        //::EnumChildWindows(hwnd,(WNDENUMPROC)MyEnumWindowsProc,0);  
    }  
    return true;  
} 

CRect g_rcDisplay; // �ܹ�������ʾ������


//////////////////////////////////////////////////////////////////////////
// �õ���С�ľ���
inline CRect get_min_rect(CWnd *pWnd,CPoint pt,CRect rcMax){
    
    auto it = g_wndrect_list.begin();
    auto ie = g_wndrect_list.end();
//     pt.x = 200;
//     pt.y = 200;
    //auto ir = it;
    /// �Ƚϵ��ıߵľ���֮�ͣ���С�ļ�Ϊ��С����

	CRect rc=rcMax;
    int dis = rcMax.Width()+rcMax.Height();
    for ( ;it!=ie;++it ){
        if (/* it->PtInRect(pt)*/
			pt.x>=it->left && pt.x<= it->right
			&& pt.y>=it->top && pt.y<=it->bottom
			){
            int d = it->Width()+it->Height();
            if (d<dis){
                dis = d;
                rc = *it;
            }
        }
    }

	return rc;
}


BOOL CALLBACK MonitorEnumProc(
  _In_  HMONITOR hMonitor,
  _In_  HDC hdcMonitor,
  _In_  LPRECT lprcMonitor,
  _In_  LPARAM dwData
  )
{
	MONITORINFOEX mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor,&mi);
	//TRACE(_T("%s\n"),mi.szDevice);

	// �󲢼�
	if ( mi.rcMonitor.left < g_rcDisplay.left ) {
		g_rcDisplay.left = mi.rcMonitor.left;
	}

	if ( mi.rcMonitor.top < g_rcDisplay.top ){
		g_rcDisplay.top = mi.rcMonitor.top;
	}

	if ( mi.rcMonitor.right > g_rcDisplay.right ){
		g_rcDisplay.right = mi.rcMonitor.right;
	}

	if ( mi.rcMonitor.bottom > g_rcDisplay.bottom ){
		g_rcDisplay.bottom = mi.rcMonitor.bottom;
	}

	return TRUE;
}

// CCaptureDlg �Ի���
IMPLEMENT_DYNAMIC(CCaptureDlg, CWnd)
CCaptureDlg::CCaptureDlg(CWnd* pParent /*=NULL*/)
	/*: CWnd(CCaptureDlg::IDD, pParent)*/
{
	EnumDisplayMonitors(NULL,NULL,MonitorEnumProc,NULL);
    //��ȡ��Ļ�ֱ���
    //m_xScreen = GetSystemMetrics(SM_CXSCREEN);
    //m_yScreen = GetSystemMetrics(SM_CYSCREEN);

    //��ȡ��Ļ��λͼ��
    CRect rect = g_rcDisplay;
    m_pBitmap=CBitmap::FromHandle(CopyScreenToBitmap(&rect,FALSE));
    m_pBitmapRestore = CBitmap::FromHandle(CopyScreenToBitmap(&rect,FALSE));
    m_pBmpLight = NULL;
    
    m_select_mode = WINDOW;
    m_select_status = SELECTING;
    m_nDragHotChangeIndex = -1;
    m_fbar.bgBmp.LoadBitmap(IDB_BITMAP_FBAR_BG);
    

    int i;
    i=0;m_fbar.ctrl[i].bmp.LoadBitmap(IDB_BITMAP_RECT);    m_fbar.ctrl[i].width=18;m_fbar.ctrl[i].height=16;m_fbar.ctrl[i].fun_id=fi_rect   ;
    i++;m_fbar.ctrl[i].bmp.LoadBitmap(IDB_BITMAP_ELLIPSE); m_fbar.ctrl[i].width=18;m_fbar.ctrl[i].height=16;m_fbar.ctrl[i].fun_id=fi_ellipse;
    i++;m_fbar.ctrl[i].bmp.LoadBitmap(IDB_BITMAP_ARROW);   m_fbar.ctrl[i].width=18;m_fbar.ctrl[i].height=16;m_fbar.ctrl[i].fun_id=fi_arrow  ;
    i++;m_fbar.ctrl[i].bmp.LoadBitmap(IDB_BITMAP_BRUSH);   m_fbar.ctrl[i].width=18;m_fbar.ctrl[i].height=16;m_fbar.ctrl[i].fun_id=fi_brush  ;
    i++;m_fbar.ctrl[i].bmp.LoadBitmap(IDB_BITMAP_FONT);    m_fbar.ctrl[i].width=18;m_fbar.ctrl[i].height=16;m_fbar.ctrl[i].fun_id=fi_font   ;
    i++;m_fbar.ctrl[i].bmp.LoadBitmap(IDB_BITMAP_CANCEL);  m_fbar.ctrl[i].width=18;m_fbar.ctrl[i].height=16;m_fbar.ctrl[i].fun_id=fi_cancel ;
    i++;m_fbar.ctrl[i].bmp.LoadBitmap(IDB_BITMAP_SAVE);    m_fbar.ctrl[i].width=18;m_fbar.ctrl[i].height=16;m_fbar.ctrl[i].fun_id=fi_save   ;
    i++;m_fbar.ctrl[i].bmp.LoadBitmap(IDB_BITMAP_EXIT);    m_fbar.ctrl[i].width=18;m_fbar.ctrl[i].height=16;m_fbar.ctrl[i].fun_id=fi_exit   ;
    i++;m_fbar.ctrl[i].bmp.LoadBitmap(IDB_BITMAP_OK);      m_fbar.ctrl[i].width=18;m_fbar.ctrl[i].height=16;m_fbar.ctrl[i].fun_id=fi_ok     ;

    m_fbar.ctrl_count = i+1;
    //m_fbar.ctrl[0].bUse = true;
	for ( int i=0;i<m_fbar.ctrl_count;++i ){
		m_fbar.propPanel[i].m_bEnable = i<4;
	}

	m_fbar.SetXyScreen(g_rcDisplay.Width(),g_rcDisplay.Height());

    m_pDrawingShape = NULL;
	//m_drawStatus=draw_begin;
    m_selectedShapeType = -1;
    m_bDrawingShape = false;

}

CCaptureDlg::~CCaptureDlg()
{
	auto it = m_shapeList.begin();
	for ( ;it!=m_shapeList.end();++it ){
		delete (*it);
	}
    m_waterMarkFont.DeleteObject();
}

void CCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCaptureDlg, CWnd)
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_CREATE()
    ON_WM_KEYUP()
END_MESSAGE_MAP()


// CCaptureDlg ��Ϣ�������

//*********************���ӵĺ���**********************************************************
//������Ļ,��δ������������� �Ǻ�
HBITMAP CCaptureDlg::CopyScreenToBitmap(LPRECT lpRect,BOOL bSave)
//lpRect ����ѡ������
{
	HDC       hScrDC, hMemDC;      
	// ��Ļ���ڴ��豸������
	HBITMAP    hBitmap, hOldBitmap;   
	// λͼ���
	int       nX, nY, nX2, nY2;      
	// ѡ����������
	int       nWidth, nHeight;
	
	// ȷ��ѡ������Ϊ�վ���
	if (IsRectEmpty(lpRect))
		return NULL;
	//Ϊ��Ļ�����豸������
	hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScrDC);
	// ���ѡ����������
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	//ȷ��ѡ�������ǿɼ���
// 	if (nX < 0)
// 		nX = 0;
// 	if (nY < 0)
// 		nY = 0;
// 	if (nX2 > m_xScreen)
// 		nX2 = m_xScreen;
// 	if (nY2 > m_yScreen)
// 		nY2 = m_yScreen;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// ����һ������Ļ�豸��������ݵ�λͼ
	hBitmap = CreateCompatibleBitmap
		(hScrDC, nWidth, nHeight);
	// ����λͼѡ���ڴ��豸��������
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
	if(bSave)
	{
		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(CDC::FromHandle(hMemDC));
		dcCompatible.SelectObject(m_pBitmap);
        
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			dcCompatible, nX, nY, SRCCOPY);

	}
	else
	{
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			hScrDC, nX, nY, SRCCOPY);
	}

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//�õ���Ļλͼ�ľ��
	//��� 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// ����λͼ���
	if(bSave)
	{
				
		if (OpenClipboard()) 
		{
        //��ռ�����
        EmptyClipboard();
        //����Ļ����ճ������������,
        //hBitmap Ϊ�ղŵ���Ļλͼ���
        SetClipboardData(CF_BITMAP, hBitmap);
        //�رռ�����
        CloseClipboard();
      }
	}
	return hBitmap;
}



// BOOL CCaptureDlg::OnInitDialog()
// {
//     CWnd::OnInitDialog();
// 
//     // TODO:  �ڴ���Ӷ���ĳ�ʼ��
//     //SetWindowPos(&CWnd::wndTopMost,0,0,m_xScreen,m_yScreen,0);
//     
//     //**************************************************************************
// 	//�ѶԻ������ó�ȫ�����㴰��
// 	//SetWindowPos(NULL,0,0,m_xScreen,m_yScreen,SWP_SHOWWINDOW);
//  
//     
//     m_selectedRect.SetRect(0,0,800,600);
//     m_selectedRect.MoveToXY(10,10);
//     
//     CDC *pDc = GetWindowDC();
//     m_dcBg.CreateCompatibleDC(pDc);
//     m_dcLight.CreateCompatibleDC(pDc);
//     //m_dcBg.CreateDiscardableBitmap(pDc,m_xScreen,m_yScreen);
//     //m_dcLight.CreateDiscardableBitmap(pDc,m_xScreen,m_yScreen);
//     // ��ǰ��Ļ��ͼ ��ɫ
//     //BLENDFUNCTION   bf;
//     //bf.AlphaFormat  = AC_SRC_OVER;
//     //bf.BlendFlags   = 0;
//     //bf.BlendOp      = AC_SRC_OVER;
//     //bf.SourceConstantAlpha = 128;
// 
//     m_dcBg.SelectObject(m_pBitmap);
//     m_dcLight.SelectObject(m_pBitmap);
// 
//     m_hArrow = LoadCursor(NULL,IDC_ARROW);
//     m_hCross = LoadCursor(NULL,IDC_CROSS);
//     m_hHand  = LoadCursor(NULL,IDC_HAND);
//     return TRUE;  // return TRUE unless you set the focus to a control
//     // �쳣: OCX ����ҳӦ���� FALSE
// }


BOOL CCaptureDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    


    return TRUE;

//**************************************************************************************

    //return CWnd::OnEraseBkgnd(pDC);
}


void CCaptureDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    OnFunctionOk();
    //OnOK();
    ///CWnd::OnLButtonDblClk(nFlags, point);
}



void CCaptureDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()

        //**************************************************************************************
	//�������������ȫ���Ի��򱳾�
	CRect rect;
	GetClientRect(&rect);
    CDC *pDC = GetWindowDC();
    
	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);

	///CGdiObject *pMemOldObj = dcCompatible.SelectObject(m_pBitmap);
    CBitmap bmpBg;
    bmpBg.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
    dcCompatible.SelectObject(&bmpBg);
	
    //CBrush brs;
    //brs.CreateSolidBrush(RGB(240,20,240));
    ///dcCompatible.SelectObject(&brs);
    //pDC->FillRect(rect,&brs);
	//pDC->SelectObject(&brs);
    //pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,MERGECOPY);
    //m_dcBg.PatBlt(0, 0, rect.Width(),rect.Height(), BLACKNESS);
    BLENDFUNCTION blend;
    blend.BlendOp = AC_SRC_OVER;  
    blend.BlendFlags = 0;
    blend.AlphaFormat = 0;
    blend.SourceConstantAlpha = 128;

    dcCompatible.AlphaBlend(0, 0, rect.Width(), rect.Height(), &m_dcBg, 0, 0, rect.Width(), rect.Height(), blend);
    //dcCompatible.SelectObject(pMemOldObj);
    //dcCompatible.PatBlt(0, 0, rect.Width(),rect.Height(), WHITENESS);
    // ����ѡ�б߿�
     int border_width = 3;
     int def_rc = 2;
     if ( m_select_status>=SELECTED || m_select_mode==MOUSE ){
         border_width = 1;
         def_rc = 1;
     }
     CPen pen;
     pen.CreatePen(PS_SOLID,border_width,RGB(0,255,0));
     CGdiObject *pOldObj = dcCompatible.SelectObject(&pen);
     CRect rcSelected = GetSelectedRect();
     rcSelected.DeflateRect(def_rc,def_rc,def_rc,def_rc);
     dcCompatible.MoveTo(rcSelected.TopLeft());
     dcCompatible.LineTo(rcSelected.right,rcSelected.top);
     //dcCompatible.MoveTo(rcSelected.right,rcSelected.top);
     dcCompatible.LineTo(rcSelected.right,rcSelected.bottom);
     //dcCompatible.MoveTo(rcSelected.right,rcSelected.bottom);
     dcCompatible.LineTo(rcSelected.left,rcSelected.bottom);
     //dcCompatible.MoveTo(rcSelected.left,rcSelected.bottom);
     dcCompatible.LineTo(rcSelected.TopLeft());
     pen.DeleteObject();
     rcSelected.DeflateRect(1,1,1,1);
 
     dcCompatible.BitBlt(rcSelected.left,rcSelected.top,rcSelected.Width()+1,rcSelected.Height()+1,&m_dcBg,rcSelected.left,rcSelected.top,SRCCOPY);
// 
//     if ( /*m_select_mode==MOUSE || */m_select_status==SELECTED ){
//         // �����϶��ȵ�
//         // ����
//         m_rcHot[0].left   = rcSelected.left-3;
//         m_rcHot[0].top    = rcSelected.top-3;
//         m_rcHot[1].left   = (rcSelected.left+rcSelected.right)/2-3;
//         m_rcHot[1].top    = rcSelected.top-3;
//         m_rcHot[2].left   = rcSelected.right-2;
//         m_rcHot[2].top    = rcSelected.top-3;
//         m_rcHot[3].left   = rcSelected.left-3;
//         m_rcHot[3].top    = (rcSelected.top+rcSelected.bottom)/2+3;
//         m_rcHot[4].left   = rcSelected.right-2;
//         m_rcHot[4].top    = (rcSelected.top+rcSelected.bottom)/2+3;;
//         m_rcHot[5].left   = rcSelected.left-3;
//         m_rcHot[5].top    = rcSelected.bottom-2;
//         m_rcHot[6].left   = (rcSelected.left+rcSelected.right)/2-3;
//         m_rcHot[6].top    = rcSelected.bottom-2;
//         m_rcHot[7].left   = rcSelected.right-2;
//         m_rcHot[7].top    = rcSelected.bottom-2;
// 
//         for ( int i=0;i<8;++i ){
//             m_rcHot[i].right  = m_rcHot[i].left+5;
//             m_rcHot[i].bottom = m_rcHot[i].top + 5;
//             dcCompatible.FillSolidRect(m_rcHot[i],RGB(0,255,0));
//         }
//     }
// 
//     // ������
// 	m_fbar.SetSelectedRect(GetSelectedRect());
//     m_fbar.Draw(&dcCompatible);
// 
//     // ������״
//     auto it = m_shapeList.begin();
//     for ( ;it!=m_shapeList.end();++it ){
//         (*it)->Draw(&dcCompatible);
//     }

    pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);
    dcCompatible.DeleteDC();
    //memSelectDc.DeleteDC();
    //pDC->DeleteDC();
    //pDC->SelectObject(pOldObj);
}


void CCaptureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CRect & selected_rect = GetSelectedRect();
	switch ( m_select_status ){
	case SELECTING:
		m_mouseSelectedRect.SetRect(point,point);
		m_ptDragStart = point;
		break;
	case SELECTED:
		if ( selected_rect.PtInRect(point)){
			if ( InHotRect(point)<0 ){ //�����ȵ������ʱ�򣬲����ƶ�����
			    SetCursor(m_hHand);
			}
		}
		break;
	case DRAW_SHAPE:
		if ( selected_rect.PtInRect(point) && m_selectedShapeType>=0 ){
            m_bDrawingShape = true;
			if ( !m_pDrawingShape ){
				m_pDrawingShape = Fac_CreateShape(m_selectedShapeType);
				m_shapeList.push_back(m_pDrawingShape);
			}

			m_pDrawingShape->OnMouseDrawBegin(point);
		}
		
		break;
	}

    CWnd::OnLButtonDown(nFlags, point);
}


void CCaptureDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    SetCursor(m_hArrow);
    m_nDragHotChangeIndex = -1; //��ֹ�϶��ı��С
	int select_status = m_select_status;
    if (select_status==SELECTING){
        m_select_status = SELECTED;
    }
    else if ( select_status==SELECTED ){
        if (m_fbar.GetRect().PtInRect(point)){
            int fun_id = -1;
			int i=0;
            for ( i=0;i<m_fbar.ctrl_count;++i){
                if ( m_fbar.ctrl[i].GetRect().PtInRect(point) ){
                    m_fbar.ctrl[i].status = CtrlStatus::chose;
                    fun_id = m_fbar.ctrl[i].fun_id;
					m_fbar.propPanel[i].is_show = true;
					m_fbar.m_activeIndex = i;
                }
                else{
                     m_fbar.ctrl[i].status = CtrlStatus::normal;
				     m_fbar.propPanel[i].is_show = false;

                }
            }
			if ( fun_id >= 0){
				//m_drawStatus = draw_begin;
				switch(fun_id){
				case fi_rect:
				case fi_ellipse:
                case fi_arrow:
                case fi_brush:
                case fi_font:
                    OnDrawShape(fun_id);
					
					break; 
				case fi_exit:
					OnCancel();
					break;
				case fi_save:
					OnFunctionSavePic();
					break;
				case fi_ok:
					OnFunctionOk();
					break;
				case fi_cancel:
					OnFunctionCancel();
					break;
				
				}
			}
        }
    }
	else if ( select_status == DRAW_SHAPE ){
        if ( m_bDrawingShape ){
            OnDrawShapeEnd();
            m_bDrawingShape = false;
        }

       if (m_fbar.GetRect().PtInRect(point)){
            int fun_id = -1;
            for ( int i=0;i<m_fbar.ctrl_count;++i){
                 if ( m_fbar.ctrl[i].GetRect().PtInRect(point) ){
                 	m_fbar.ctrl[i].status = CtrlStatus::chose;
                 	fun_id = m_fbar.ctrl[i].fun_id;
					m_fbar.propPanel[i].is_show = true;
					m_fbar.m_activeIndex = i;
                 }
                 else{
                 	 m_fbar.ctrl[i].status = CtrlStatus::normal;
					 m_fbar.propPanel[i].is_show = false;
                 }
            }
            if ( fun_id >= 0 ){
       			switch(fun_id){
				case fi_rect:
				case fi_ellipse:
				case fi_arrow:
				case fi_brush:
				case fi_font:
					OnDrawShape(fun_id);
					break;
       			case fi_exit:
       				OnCancel();
       				break;
       			case fi_save:
       				OnFunctionSavePic();
       				break;
       			case fi_ok:
       				OnFunctionOk();
       				break;
				case fi_cancel:
					OnFunctionCancel();
					break;
       			}
		   }

			
	    }
    
// 	    PropertyPanel *propPanel = m_fbar.getPropPanel();
// 		if ( propPanel ){
// 			propPanel->OnLButtonUp(point);
// 		}

	   if ( m_fbar.m_activeIndex >= 0 ){
		   if ( m_fbar.propPanel[m_fbar.m_activeIndex].m_rc.PtInRect(point)){
				m_fbar.propPanel[m_fbar.m_activeIndex].OnLButtonUp(point);
		   }
	   }
	}
    Invalidate(TRUE);
    CWnd::OnLButtonUp(nFlags, point);
}

void CCaptureDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACE("pt:%d\n",point.x);
	CRect &selected_rect = GetSelectedRect();
    if ( IsLButtonDown() ){
		// ��ʱ�����϶�ģʽ, ��ֹ������Ķ����ʹ��в˵��Ĵ��ڸ��Ž�ͼ
		// �϶���һ��С���� �������϶�ģʽ
		if ( abs(point.x-m_ptDragStart.x) > 5 && abs(point.y-m_ptDragStart.y)>5 ){
			OnDrag(m_movePoint,point);
		}
    }
    else{
        OnMouseOver(m_movePoint,point);
    }
	Invalidate(TRUE);
    m_movePoint = point;
    CWnd::OnMouseMove(nFlags, point);
}


int CCaptureDlg::InHotRect(CPoint pt)
{
    for ( int i=0;i<8;++i ){
        if (m_rcHot[i].PtInRect(pt)){
            return i;
        }
    }
    return -1;
}

inline CString gen_file_name(){
	CTime t = CTime::GetCurrentTime();
	CString str;
	str.Format(
		_T("MT%04d%02d%02d%02d%02d%02d%3d"),
		t.GetYear(),t.GetMonth(),t.GetDay(),
		t.GetHour(),t.GetMinute(),t.GetSecond(),GetTickCount()%1000
		);
	return str;
}

void CCaptureDlg::OnFunctionSavePic()
{

    CFileDialog dlg(
        FALSE,
        _T("jpg"),
        gen_file_name(),
        OFN_NOCHANGEDIR,
        _T("ͼ���ļ� (*.jpg;*.bmp)|*.jpg;*.bmp||"),
        this
        );
    if ( dlg.DoModal() == IDOK ){
        CString fileName = dlg.GetPathName();
//         HBITMAP h = CopyScreenToBitmap(m_selectedRect,FALSE);
//         CBitmap *bmp = CBitmap::FromHandle(h);

        CFile file;
        if ( file.Open(fileName,CFile::modeRead) ){
            CString str;
            str.Format(_T("%s �Ѿ�����,�Ƿ񸲸�"),fileName);

            int nRet = MessageBox(str,_T("����"),MB_YESNO) ;
            if ( nRet == IDYES ){

            }
            else if ( nRet == IDNO ){
                // ������
                OnFunctionSavePic();
                return ;
            }
            else{
                return ;
            }
        }
;
        CBitmap *bmp = genBitmap();
        SaveBmpToJpg(fileName,bmp);
        delete bmp;

        OnOK();
    }
}

CString g_strWaterMark = _T("���ý�ͼ(mengtu.qeephone.com)");
void CCaptureDlg::OnFunctionOk()
{
    CBitmap *bmp = genBitmap();
    if (OpenClipboard()) 
    {
        //��ռ�����
        EmptyClipboard();
        //����Ļ����ճ������������,
        //hBitmap Ϊ�ղŵ���Ļλͼ���
        SetClipboardData(CF_BITMAP, bmp->GetSafeHandle());
        //�رռ�����
        CloseClipboard();
    }
    delete bmp;
    OnOK();
}


BOOL CCaptureDlg::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: �ڴ����ר�ô����/����û���
    //cs.lpszClass = NULL;
    return CWnd::PreCreateWindow(cs);
}

void CCaptureDlg::OnCancel()
{
    PostMessage(WM_CLOSE);
}

void CCaptureDlg::OnOK()
{
    PostMessage(WM_CLOSE);    
}


int CCaptureDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  �ڴ������ר�õĴ�������
       //SetWindowPos(&CWnd::wndTopMost,0,0,m_xScreen,m_yScreen,0);
       
       //**************************************************************************
    //�ѶԻ������ó�ȫ�����㴰��
    SetWindowPos(NULL,g_rcDisplay.left,g_rcDisplay.top,g_rcDisplay.Width(),g_rcDisplay.Height(),SWP_SHOWWINDOW);
    
	// ö�ٴ���
    g_wndrect_list.clear();
    EnumWindows(MyEnumWindowsProc,(LPARAM)this);

    
    m_windowSelectedRect.SetRect(0,0,800,600);
    m_windowSelectedRect.MoveToXY(10,10);
    
    CDC *pDc = GetWindowDC();
    m_dcBg.CreateCompatibleDC(pDc);
    m_dcLight.CreateCompatibleDC(pDc);
    //m_dcBg.CreateDiscardableBitmap(pDc,m_xScreen,m_yScreen);
    //m_dcLight.CreateDiscardableBitmap(pDc,m_xScreen,m_yScreen);
    // ��ǰ��Ļ��ͼ ��ɫ
    //BLENDFUNCTION   bf;
    //bf.AlphaFormat  = AC_SRC_OVER;
    //bf.BlendFlags   = 0;
    //bf.BlendOp      = AC_SRC_OVER;
    //bf.SourceConstantAlpha = 128;
    
    m_dcBg.SelectObject(m_pBitmap);
    m_dcLight.SelectObject(m_pBitmap);
    
    m_hArrow = LoadCursor(NULL,IDC_ARROW);
    m_hCross = LoadCursor(NULL,IDC_CROSS);
    m_hHand  = LoadCursor(NULL,IDC_HAND);
    //IDC_SIZEALL ʮ�ּ�ͷ
    //IDC_SIZENWSE ����-����

    m_hSize       = LoadCursor(NULL,IDC_SIZENESW);
    m_hLeftRight  = LoadCursor(NULL,IDC_SIZEWE);
    m_hTopBottom  = LoadCursor(NULL,IDC_SIZENS);
    m_hLeftTop    = LoadCursor(NULL,IDC_SIZENWSE);
    m_hRightTop   = LoadCursor(NULL,IDC_SIZENESW);

    VERIFY(m_waterMarkFont.CreateFont(
        12,                        // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_NORMAL,                 // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        ANSI_CHARSET,              // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        _T("����")));                 // lpszFacename


    return 0;
}

void CCaptureDlg::OnDrawShapeEnd()
{
	m_pDrawingShape = NULL;
	//m_fbar.Reset(); // ���ù�������״̬
	//m_select_status = DRAW_SHAPE;
//	m_drawStatus = draw_end;
}

void CCaptureDlg::OnDrawShape( int type )
{
    m_select_status = DRAW_SHAPE;
    //m_pDrawingShape = Fac_CreateShape(type);
    m_selectedShapeType = type;
    //m_shapeList.push_back(m_pDrawingShape);
}

int CCaptureDlg::getBorderWidth(int type){
    //if ( m_fbar.m_activeIndex < 0 ){return 1;}
    int i=0;
    for (  i=0;i<m_fbar.ctrl_count;++i ){
        if ( m_fbar.ctrl[i].fun_id==type ){
            return m_fbar.propPanel[i].getBorderWidth();
            break;
        }
    }

    return 1;
}

CShape * CCaptureDlg::Fac_CreateShape( int type )
{
    CShape *shape = NULL;
    int boderWidth = getBorderWidth(type);
    switch(type){
    case fi_rect:
        shape = new CRectangle(boderWidth);
        break;
    case fi_ellipse:
        shape = new CEllispe(boderWidth);
        break;
	case fi_arrow:
		shape = new CArrow(boderWidth);
		break;
	case fi_brush:
		shape = new CBrushPen(boderWidth);
		break;
    }
    return shape;
}

CRect & CCaptureDlg::GetSelectedRect()
{
	if ( m_select_mode == WINDOW ){
		return m_windowSelectedRect;
	}
	return m_mouseSelectedRect;
}

void CCaptureDlg::OnDrag( CPoint lastPonit,CPoint point )
{
	CRect &selected_rect = GetSelectedRect();
	if ( m_select_status == SELECTING ){
			if ( m_select_mode == WINDOW ){
				m_select_mode = MOUSE;
			}
			if ( m_mouseSelectedRect.top != point.y && m_mouseSelectedRect.left != point.x ){
				m_mouseSelectedRect.right  = point.x;
				m_mouseSelectedRect.bottom = point.y;
				
			}
		}
        // ��ѡ�������Сȷ���󣬲��ܹ��ƶ�ѡ������
        else if ( m_select_status == SELECTED ){
            //���������ȵ����棬����Ϊ�˸ı������С
            int nHostIndex = -1;
            if ( (nHostIndex=InHotRect(point))>-1 || m_nDragHotChangeIndex>-1 ){
				CRect old = selected_rect;
                int xinc,yinc;
                if ( nHostIndex==-1 ){
                    nHostIndex = m_nDragHotChangeIndex;
                }
                else{
                    // ���� �϶��ı��С
                    m_nDragHotChangeIndex = nHostIndex;
                }
                switch(nHostIndex){
                case 0:
                    xinc = point.x-m_movePoint.x;
                    yinc = point.y-m_movePoint.y;
                    selected_rect.left += xinc;
                    selected_rect.top  += yinc;
                    break;
                case 7: 
                    // �Խ���
                    xinc = point.x-m_movePoint.x;
                    yinc = point.y-m_movePoint.y;
                    selected_rect.right   += xinc;
                    selected_rect.bottom  += yinc;
                    break;
                case 1:
                    yinc = point.y-m_movePoint.y;
                    selected_rect.top  += yinc;
                    break;
                case 6:
                    //��ֱ
                    yinc = point.y-m_movePoint.y;
                    selected_rect.bottom  += yinc;
                    break;
                case 2:
                    xinc = point.x-m_movePoint.x;
                    yinc = point.y-m_movePoint.y;
                    selected_rect.right += xinc;
                    selected_rect.top   += yinc;
                    break;
                case 5:
                    // ���Խ���
                    xinc = point.x-m_movePoint.x;
                    yinc = point.y-m_movePoint.y;
                    selected_rect.left     += xinc;
                    selected_rect.bottom   += yinc;
                    break;
                case 3:
                    xinc = point.x-m_movePoint.x;
                    selected_rect.left     += xinc;
                    break;
                case 4:
                    // ˮƽ
                    xinc = point.x-m_movePoint.x;
                    selected_rect.right     += xinc;
                    break;
                }
				// ����
				if ( selected_rect.top <0 ||selected_rect.bottom>g_rcDisplay.bottom || selected_rect.left<0 || selected_rect.right>g_rcDisplay.right
					|| selected_rect.top >= selected_rect.bottom || selected_rect.left >= selected_rect.right
					){
					selected_rect = old;
				}
				
            }
            else{
                if ( selected_rect.PtInRect(point) ){
					CRect old = selected_rect;
                    selected_rect.MoveToXY(selected_rect.left+(point.x-m_movePoint.x),selected_rect.top+(point.y-m_movePoint.y));
					// ����
					if ( selected_rect.top <0 ||selected_rect.bottom>g_rcDisplay.bottom || selected_rect.left<0 || selected_rect.right>g_rcDisplay.right){
						selected_rect = old;
					}
				}
            }
        }
        else if ( m_select_status == DRAW_SHAPE ){
				//m_drawStatus = drawing;
				if ( m_pDrawingShape ){
					CPoint pt = point;
					if ( pt.x < selected_rect.left ){ pt.x = selected_rect.left; }
					if ( pt.x > selected_rect.right){ pt.x = selected_rect.right;}
					if ( pt.y < selected_rect.top  ){ pt.y = selected_rect.top; }
					if ( pt.y > selected_rect.bottom){pt.y = selected_rect.bottom;}
					m_pDrawingShape->OnMouseDrawing(pt);
				}
			}
}

void CCaptureDlg::OnMouseOver( CPoint lastPonit,CPoint point )
{
	     if ( m_select_status == SELECTING ){
            CPoint pt = point;
            //ClientToScreen(&pt);
    //         pt.x = 295;
    //         pt.y = 165;
            m_windowSelectedRect = get_min_rect(this,pt,g_rcDisplay);
           // Invalidate(TRUE);
        }
		else if ( m_select_status == SELECTED ){
		    int nHostIndex=-1;
            if (m_fbar.GetRect().PtInRect(point)){
				for ( int i=0;i<m_fbar.ctrl_count;++i){
					if ( m_fbar.ctrl[i].GetRect().PtInRect(point) ){
						m_fbar.ctrl[i].status = CtrlStatus::mouse_over;
					}
					else{
						 m_fbar.ctrl[i].status = CtrlStatus::normal;
					}
				}
			}
            // ���ȵ���
            else if ( (nHostIndex=InHotRect(point))>=-1  ){
                HCURSOR hCursor = NULL;
                switch(nHostIndex){
                case 0:
                case 7:
                    hCursor = m_hLeftTop;
                    break;
                case 1:
                case 6:
                    hCursor = m_hTopBottom;
                    break;
                case 2:
                case 5:
                    hCursor = m_hRightTop;
                    break;
                case 3:
                case 4:
                    hCursor = m_hLeftRight;
                    break;
                default:
                    hCursor = m_hArrow;
                }
                SetCursor(hCursor);
                
            }
			//Invalidate(TRUE);
		}
		else if ( m_select_status == DRAW_SHAPE ){

            if ( GetSelectedRect().PtInRect(point) ){
                SetCursor(m_hCross);
            }
            else{
                SetCursor(m_hArrow);
            }

            if (m_fbar.GetRect().PtInRect(point)){
				for ( int i=0;i<m_fbar.ctrl_count;++i){
					if ( m_fbar.ctrl[i].GetRect().PtInRect(point) ){
                        if ( m_fbar.ctrl[i].status == CtrlStatus::normal ){
						    m_fbar.ctrl[i].status = CtrlStatus::mouse_over;
                        }
					}
					else{
                        if ( m_fbar.ctrl[i].status == CtrlStatus::mouse_over ){
							m_fbar.ctrl[i].status = CtrlStatus::normal;
                        }
					}
				}
				//Invalidate(TRUE);
			}

// 			PropertyPanel *propPanel = m_fbar.getPropPanel();
// 			if ( propPanel ){
// 				propPanel->OnMouseOver(point);
// 			}

            if ( m_fbar.m_activeIndex >= 0 ){
                if ( m_fbar.propPanel[m_fbar.m_activeIndex].m_rc.PtInRect(point)){
                    m_fbar.propPanel[m_fbar.m_activeIndex].OnMouseOver(point);
                }
            }
		}
   
}

CBitmap * CCaptureDlg::genBitmap()
{
    CBitmap *bmpLight = new CBitmap();
    CDC *pDC = GetWindowDC();
    CDC dcLight;
    dcLight.CreateCompatibleDC(pDC);
    CRect & selected_rect = GetSelectedRect();
    bmpLight->CreateDiscardableBitmap(GetWindowDC(),selected_rect.Width(),selected_rect.Height());
    dcLight.SelectObject(bmpLight);
    // ������״
    auto it = m_shapeList.begin();
    for ( ;it!=m_shapeList.end();++it ){
        (*it)->Draw(&m_dcBg);
    }

    dcLight.BitBlt(0,0,selected_rect.Width(),selected_rect.Height(),&m_dcBg,selected_rect.left,selected_rect.top,SRCCOPY);
    dcLight.SetBkMode(TRANSPARENT);
    if ( !CConfig::Single().waterMask.IsEmpty() ){
		CRect rcText;
		dcLight.SelectObject(&m_waterMarkFont);
		dcLight.DrawText(CConfig::Single().waterMask,&rcText,DT_CALCRECT);
		rcText.MoveToXY(selected_rect.Width()-rcText.Width(),selected_rect.Height()-rcText.Height());
		dcLight.DrawText(CConfig::Single().waterMask,&rcText,DT_LEFT|DT_VCENTER);
	}

    dcLight.DeleteDC();
    return bmpLight;
}


void CCaptureDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if ( nChar == VK_RETURN ){
        m_select_status = SELECTED;
        Invalidate(TRUE);
    }
    CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CCaptureDlg::OnFunctionCancel()
{
	if( m_shapeList.empty() ){
		return ;
	}
	vector<CShape*>::iterator it = m_shapeList.end();
	m_shapeList.erase(--it);
}
