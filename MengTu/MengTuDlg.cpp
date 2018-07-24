
// MengTuDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MengTu.h"
#include "MengTuDlg.h"
#include "afxdialogex.h"
#include "CaptureDlg.h"
#include "Config.h"
#include <Psapi.h>


#pragma comment(lib,"Psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_HOTKEY_TOGGLESHOW 1 // 隐藏和显示
#define ID_HOTKEY_CAPTURE    2 // 截图
#define ID_HOTKEY_CAPTURE2    3 // 截图
#define ID_HOTKEY_TEST       4  // 测试


//////////////////////////////////////////////////////////////////////////
// 截窗口的屏幕
HBITMAP CMengTuDlg::CaptureWindow( HWND hWnd,bool bClipborad )
{
    RECT rc;
    ::GetWindowRect(hWnd,&rc);
    HDC hSrcDc = ::GetWindowDC(hWnd);
    HDC hMemDc = CreateCompatibleDC(hSrcDc);
    HBITMAP hBmp,hOldBmp;
    hBmp = CreateCompatibleBitmap(hSrcDc,rc.right-rc.left,rc.bottom-rc.top);

    hOldBmp = (HBITMAP)SelectObject(hMemDc,hBmp);

    BitBlt(hMemDc,0,0,rc.right-rc.left,rc.bottom-rc.top,hSrcDc,0,0,SRCCOPY);
    hBmp = (HBITMAP)SelectObject(hMemDc,hOldBmp); // hBmp的句柄已经知道了 这句没必要。

    DeleteDC(hMemDc);

    if(bClipborad)
    {

        if (OpenClipboard()) 
        {
            //清空剪贴板
            EmptyClipboard();
            //把屏幕内容粘贴到剪贴板上,
            //hBitmap 为刚才的屏幕位图句柄
            SetClipboardData(CF_BITMAP, hBmp);
            //关闭剪贴板
            CloseClipboard();
        }
    }

    return hBmp;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMengTuDlg 对话框




CMengTuDlg::CMengTuDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMengTuDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pCapWnd = NULL;
}

void CMengTuDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_NOTIFY, m_ntStatic);
    DDX_Control(pDX, IDC_STATIC_NOTIFY_FINDPATH, m_findStatic);
}

BEGIN_MESSAGE_MAP(CMengTuDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(MSGID_STATIC_NOTIFY,OnMsgStaticNotify)
    ON_MESSAGE(MSGID_WAKEUP,OnMsgWakeup)
	ON_WM_LBUTTONDOWN()
    ON_WM_TIMER()
    ON_WM_HOTKEY()
    ON_BN_CLICKED(IDC_CHECK_CAPTURE, &CMengTuDlg::OnBnClickedCheckCapture)
    ON_EN_CHANGE(IDC_EDIT1, &CMengTuDlg::OnEnChangeEdit1)
    ON_EN_CHANGE(IDC_EDIT2, &CMengTuDlg::OnEnChangeEdit2)
    ON_BN_CLICKED(IDC_CHECK_RESTOR, &CMengTuDlg::OnBnClickedCheckRestor)

END_MESSAGE_MAP()


// CMengTuDlg 消息处理程序

BOOL CMengTuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

    // 注册专用窗口类
    WNDCLASSEX wndClass = {0};
    wndClass.cbSize = sizeof(wndClass);
    wndClass.hbrBackground = NULL;
    wndClass.lpszClassName = _T("capture_window_class");
    wndClass.hInstance = NULL;
    wndClass.hCursor   = NULL; //为NULL，程序才可用SetCursor设置图标，否则鼠标移动的时候，会自动还原的
    wndClass.lpfnWndProc = ::DefWindowProc;
    wndClass.lpszMenuName = NULL;
    wndClass.style = CS_HREDRAW | CS_VREDRAW|CS_DBLCLKS ; //CS_DBLCLKS对双击进行响应

    RegisterClassEx(&wndClass);

    m_hChangeWndSize = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE (IDC_CURSOR1)); 
    SetDlgItemInt(IDC_EDIT1,800);
    SetDlgItemInt(IDC_EDIT2,600);

    RegisterHotKey(m_hWnd,ID_HOTKEY_TOGGLESHOW,MOD_CONTROL|MOD_ALT,'Z');
    RegisterHotKey(m_hWnd,ID_HOTKEY_CAPTURE,MOD_CONTROL|MOD_ALT,'X');
    RegisterHotKey(m_hWnd,ID_HOTKEY_CAPTURE2,MOD_CONTROL|MOD_SHIFT,'P');
	RegisterHotKey(m_hWnd,ID_HOTKEY_TEST,MOD_CONTROL|MOD_SHIFT,'T');
    CConfig & cfg = CConfig::Single();
    SetDlgItemInt(IDC_EDIT1,cfg.width);
    SetDlgItemInt(IDC_EDIT2,cfg.height);
    
    CButton *pBtnRestore = (CButton*)GetDlgItem(IDC_CHECK_RESTOR);
    if( cfg.capture ){
        CButton *pBtnCapture = (CButton*)GetDlgItem(IDC_CHECK_CAPTURE);
        pBtnCapture->SetCheck(BST_CHECKED);
        pBtnRestore->EnableWindow(TRUE);
    }
    else{
        pBtnRestore->EnableWindow(FALSE);
    }

    if ( cfg.restore ){
        pBtnRestore->SetCheck(TRUE);
    }

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMengTuDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMengTuDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMengTuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CMengTuDlg::OnMsgStaticNotify( WPARAM wp,LPARAM lp )
{
    MSG *msg = (MSG*)wp;
    int nCtrlID = (int)lp;
    switch(msg->message){
    case WM_LBUTTONDOWN:
        OnStaticLButtonDown(nCtrlID);
        break;
    case WM_LBUTTONUP:
        OnStaticLButtonUp(nCtrlID);
        break;
    }
    return TRUE;
}

void CMengTuDlg::OnStaticLButtonDown(int nCtrlID)
{
    m_hRestoreCursor = GetCursor();
    SetCursor(m_hChangeWndSize);
    SetTimer(1,500,NULL);
}

void CMengTuDlg::OnStaticLButtonUp(int nCtrlID)
{
    KillTimer(1);
    SetCursor(m_hRestoreCursor);
    if ( nCtrlID == IDC_STATIC_NOTIFY ){
        int width  = GetDlgItemInt(IDC_EDIT1);
        int height = GetDlgItemInt(IDC_EDIT2);
        CRect rcRestore;
        m_dstWnd->GetWindowRect(rcRestore);
        m_dstWnd->SetWindowPos(NULL,0,0,width,height,SWP_NOMOVE);

        CButton *pCheckCapture = (CButton*)GetDlgItem(IDC_CHECK_CAPTURE);
        if (pCheckCapture->GetCheck()==BST_CHECKED){
            HBITMAP hBmp = CaptureWindow(m_dstWnd->GetSafeHwnd(),true);
            DeleteObject(hBmp);
        }

        CButton *pCheckRestore = (CButton*)GetDlgItem(IDC_CHECK_RESTOR);
        if ( pCheckRestore->GetCheck()==BST_CHECKED ){
            m_dstWnd->SetWindowPos(NULL,0,0,rcRestore.Width(),rcRestore.Height(),SWP_NOMOVE);
        }
    }
    else if ( nCtrlID == IDC_STATIC_NOTIFY_FINDPATH ){
		CString strText;
		DWORD dwErr = 0;
		m_dstWnd->GetWindowText(strText);
		dwErr = GetLastError();
        DWORD dwProcessID = 0;
        ::GetWindowThreadProcessId(m_dstWnd->GetSafeHwnd(),&dwProcessID);
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessID);
        TCHAR moduleFile[MAX_PATH] = {0};
        DWORD dwRet = GetModuleFileNameEx(hProcess,NULL,moduleFile,MAX_PATH);
        CloseHandle(hProcess);
        SetDlgItemText(IDC_EDIT_MODULEFILE,moduleFile);
		if ( m_dstWnd->IsWindowVisible() ){
			MessageBox(_T("visible"));
		}
    }
    
}



void CMengTuDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialogEx::OnLButtonDown(nFlags, point);
}

void CMengTuDlg::Scan()
{
    Sleep(400); //设置闪烁时间间隔

    POINT pnt;

    RECT rc;

    HWND DeskHwnd = ::GetDesktopWindow(); //取得桌面句柄

    HDC DeskDC = ::GetWindowDC(DeskHwnd); //取得桌面设备场景

    int oldRop2 = SetROP2(DeskDC, R2_NOTXORPEN);

    ::GetCursorPos(&pnt); //取得鼠标坐标

    m_dstWnd = WindowFromPoint(pnt) ; //取得鼠标指针处窗口句柄

    m_dstWnd->GetWindowRect(&rc); //获得窗口矩形

    //if( rc.left < 0 ) rc.left = 0;
	//
    //if (rc.top < 0 ) rc.top = 0;

    HPEN newPen = ::CreatePen(0, 3, 0); //建立新画笔,载入DeskDC

    HGDIOBJ oldPen = ::SelectObject(DeskDC, newPen);

    ::Rectangle(DeskDC, rc.left, rc.top, rc.right, rc.bottom); //在窗口周围显示闪烁矩形

    Sleep(400); //设置闪烁时间间隔

    ::Rectangle( DeskDC, rc.left, rc.top, rc.right, rc.bottom);

    ::SetROP2(DeskDC, oldRop2);

    ::SelectObject( DeskDC, oldPen);

    ::DeleteObject(newPen);

    ::ReleaseDC( DeskHwnd, DeskDC);

    DeskDC = NULL;

}

void CMengTuDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if ( 1 == nIDEvent ){
        Scan();
    }
    CDialogEx::OnTimer(nIDEvent);
}


void CMengTuDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    switch ( nHotKeyId ){
    case ID_HOTKEY_CAPTURE:
    case ID_HOTKEY_CAPTURE2:
        DoCapture();
        break;
    case ID_HOTKEY_TOGGLESHOW:
        if ( IsWindowVisible() ){
            ShowWindow(SW_HIDE);
        }
        else{
            ShowWindow(SW_SHOW);
        }
        break;
	case ID_HOTKEY_TEST:
		{

		}
		break;
    }
  
    CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CMengTuDlg::OnBnClickedCheckCapture()
{
    // TODO: 在此添加控件通知处理程序代码
    CButton *pCheckCapture = (CButton*)GetDlgItem(IDC_CHECK_CAPTURE);
    CButton *pCheckRestore = (CButton*)GetDlgItem(IDC_CHECK_RESTOR);
    
    if (pCheckCapture->GetCheck()==BST_CHECKED){
         pCheckRestore->EnableWindow(TRUE);
         CConfig::Single().SetCapture(true);
    }
    else{
        pCheckRestore->EnableWindow(FALSE);
        CConfig::Single().SetCapture(false);

    }
}


void CMengTuDlg::OnEnChangeEdit1()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    CConfig::Single().SetDefWidth(GetDlgItemInt(IDC_EDIT1));
}


void CMengTuDlg::OnEnChangeEdit2()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    CConfig::Single().SetDefWidth(GetDlgItemInt(IDC_EDIT2));
}


void CMengTuDlg::OnBnClickedCheckRestor()
{
    // TODO: 在此添加控件通知处理程序代码
    CButton *pCheckRestore = (CButton*)GetDlgItem(IDC_CHECK_RESTOR);

    CConfig::Single().SetRestore(pCheckRestore->GetCheck()==BST_CHECKED);
}

void CMengTuDlg::DoCapture()
{
    DWORD dwFlags=WS_EX_TOOLWINDOW;
//#ifndef _DEBUG
    dwFlags |= WS_EX_TOPMOST;
//#endif

    if ( m_pCapWnd ){
        if ( !::IsWindow(m_pCapWnd->m_hWnd) ){
            delete m_pCapWnd;
            m_pCapWnd = new CCaptureDlg(this);
            if (  !m_pCapWnd->CreateEx(dwFlags,_T("capture_window_class"),NULL,WS_VISIBLE|WS_POPUPWINDOW,CRect(0,0,100,100),GetDesktopWindow(),0)){
                MessageBox(_T("窗口失败"));
            }
        }
    }
    else {
        m_pCapWnd = new CCaptureDlg(this);
        if (  !m_pCapWnd->CreateEx(dwFlags,_T("capture_window_class"),NULL,WS_VISIBLE|WS_POPUPWINDOW,CRect(0,0,100,100),GetDesktopWindow(),0)){
            MessageBox(_T("窗口失败"));
        }
    }
}

LRESULT CMengTuDlg::OnMsgWakeup( WPARAM wp,LPARAM lp )
{
	ShowWindow(SW_SHOW);
	SetForegroundWindow();

	return TRUE;
}
