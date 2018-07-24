#include "StdAfx.h"
#include "Config.h"


CConfig::CConfig(void)
{
}


CConfig::~CConfig(void)
{
}

void CConfig::Load()
{
    CString iniFile;
    GetModuleFileName(NULL,iniFile.GetBuffer(MAX_PATH),MAX_PATH);
    iniFile.ReleaseBuffer();
    iniFile.Replace(_T(".exe"),_T(".ini"));
    m_iniFile = iniFile;
    width  = GetPrivateProfileInt(_T("Mengtu"),_T("width"),800,iniFile);
    height = GetPrivateProfileInt(_T("Mengtu"),_T("height"),600,iniFile);

    capture = GetPrivateProfileInt(_T("Mengtu"),_T("capture"),0,iniFile)== 1? true:false;
    restore = GetPrivateProfileInt(_T("Mengtu"),_T("capture"),0,iniFile)== 1? true:false;

	GetPrivateProfileString(_T("Mengtu"),_T("watermask"),_T(""),waterMask.GetBuffer(256),256,iniFile); 
	waterMask.ReleaseBuffer();

}

void CConfig::SetDefWidth( int width )
{
    Set(_T("width"),width);
}

void CConfig::Set( CString key,int val )
{
    CString str;
    str.Format(_T("%d"),val);
    WritePrivateProfileString(_T("Mentu"),key,str,m_iniFile);
}

void CConfig::SetDefHeight( int height )
{
    Set(_T("height"),height);
}

void CConfig::SetCapture( bool capture )
{
    Set(_T("capture"),capture?1:0);
}

void CConfig::SetRestore( bool restore )
{
    Set(_T("restore"),restore?1:0);
}

CConfig g_cfg;
CConfig & CConfig::Single()
{
    return g_cfg;
}
