
// MengTuDlg.cpp : ʵ���ļ�
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

#define ID_HOTKEY_TOGGLESHOW 1 // ���غ���ʾ
#define ID_HOTKEY_CAPTURE    2 // ��ͼ
#define ID_HOTKEY_CAPTURE2    3 // ��ͼ
#define ID_HOTKEY_TEST       4  // ����


//////////////////////////////////////////////////////////////////////////
// �ش��ڵ���Ļ
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
    hBmp = (HBITMAP)SelectObject(hMemDc,hOldBmp); // hBmp�ľ���Ѿ�֪���� ���û��Ҫ��

    DeleteDC(hMemDc);

    if(bClipborad)
    {

        if (OpenClipboard()) 
        {
            //��ռ�����
            EmptyClipboard();
            //����Ļ����ճ������������,
            //hBitmap Ϊ�ղŵ���Ļλͼ���
            SetClipboardData(CF_BITMAP, hBmp);
            //�رռ�����
            CloseClipboard();
        }
    }

    return hBmp;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMengTuDlg �Ի���




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


// CMengTuDlg ��Ϣ�������

BOOL CMengTuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

    // ע��ר�ô�����
    WNDCLASSEX wndClass = {0};
    wndClass.cbSize = sizeof(wndClass);
    wndClass.hbrBackground = NULL;
    wndClass.lpszClassName = _T("capture_window_class");
    wndClass.hInstance = NULL;
    wndClass.hCursor   = NULL; //ΪNULL������ſ���SetCursor����ͼ�꣬��������ƶ���ʱ�򣬻��Զ���ԭ��
    wndClass.lpfnWndProc = ::DefWindowProc;
    wndClass.lpszMenuName = NULL;
    wndClass.style = CS_HREDRAW | CS_VREDRAW|CS_DBLCLKS ; //CS_DBLCLKS��˫��������Ӧ

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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMengTuDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CDialogEx::OnLButtonDown(nFlags, point);
}

void CMengTuDlg::Scan()
{
    Sleep(400); //������˸ʱ����

    POINT pnt;

    RECT rc;

    HWND DeskHwnd = ::GetDesktopWindow(); //ȡ��������

    HDC DeskDC = ::GetWindowDC(DeskHwnd); //ȡ�������豸����

    int oldRop2 = SetROP2(DeskDC, R2_NOTXORPEN);

    ::GetCursorPos(&pnt); //ȡ���������

    m_dstWnd = WindowFromPoint(pnt) ; //ȡ�����ָ�봦���ھ��

    m_dstWnd->GetWindowRect(&rc); //��ô��ھ���

    //if( rc.left < 0 ) rc.left = 0;
	//
    //if (rc.top < 0 ) rc.top = 0;

    HPEN newPen = ::CreatePen(0, 3, 0); //�����»���,����DeskDC

    HGDIOBJ oldPen = ::SelectObject(DeskDC, newPen);

    ::Rectangle(DeskDC, rc.left, rc.top, rc.right, rc.bottom); //�ڴ�����Χ��ʾ��˸����

    Sleep(400); //������˸ʱ����

    ::Rectangle( DeskDC, rc.left, rc.top, rc.right, rc.bottom);

    ::SetROP2(DeskDC, oldRop2);

    ::SelectObject( DeskDC, oldPen);

    ::DeleteObject(newPen);

    ::ReleaseDC( DeskHwnd, DeskDC);

    DeskDC = NULL;

}

void CMengTuDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if ( 1 == nIDEvent ){
        Scan();
    }
    CDialogEx::OnTimer(nIDEvent);
}


void CMengTuDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CConfig::Single().SetDefWidth(GetDlgItemInt(IDC_EDIT1));
}


void CMengTuDlg::OnEnChangeEdit2()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CConfig::Single().SetDefWidth(GetDlgItemInt(IDC_EDIT2));
}


void CMengTuDlg::OnBnClickedCheckRestor()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
                MessageBox(_T("����ʧ��"));
            }
        }
    }
    else {
        m_pCapWnd = new CCaptureDlg(this);
        if (  !m_pCapWnd->CreateEx(dwFlags,_T("capture_window_class"),NULL,WS_VISIBLE|WS_POPUPWINDOW,CRect(0,0,100,100),GetDesktopWindow(),0)){
            MessageBox(_T("����ʧ��"));
        }
    }
}

LRESULT CMengTuDlg::OnMsgWakeup( WPARAM wp,LPARAM lp )
{
	ShowWindow(SW_SHOW);
	SetForegroundWindow();

	return TRUE;
}
