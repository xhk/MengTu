#include "StdAfx.h"
#include "ImgTrans.h"
//#include <GdiPlus.h>
//#include "GdiplusImaging.h"
//#include <GdiplusImageCodec.h>

using namespace Gdiplus;

CImgTrans::CImgTrans(void)
{
}


CImgTrans::~CImgTrans(void)
{
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)     
{     
    UINT num = 0;                     // number of image encoders     
    UINT size = 0;                   // size of the image encoder array in bytes     
    ImageCodecInfo* pImageCodecInfo = NULL;     
    GetImageEncodersSize(&num, &size);     
    if(size == 0)     
        return -1;     //   Failure     
      
    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));     
    if(pImageCodecInfo == NULL)     
        return -1;     //   Failure     
      
    GetImageEncoders(num, size, pImageCodecInfo);     
    for(UINT j = 0; j < num; ++j)     
    {     
        if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )     
        {     
            *pClsid = pImageCodecInfo[j].Clsid;     
            free(pImageCodecInfo);     
            return j;     //   Success     
        }             
    }     
    free(pImageCodecInfo);     
    return -1;     //   Failure     
}  
  
  
/****************BMPתJPG*********�÷�ʾ��************************** 
 
Bitmap newbitmap(L"d:\\d.bmp");//����BMP 
const unsigned short *pFileName=L"d:\\new.jpg";//����·�� 
SaveFile(&newbitmap,pFileName ); 
 
************************************************************/  
  
void SaveFile(Bitmap* pImage, const wchar_t* pFileName)//  
{  
    EncoderParameters encoderParameters;  
    CLSID jpgClsid;   
GetEncoderClsid(L"image/jpeg", &jpgClsid);  
    encoderParameters.Count = 1;  
    encoderParameters.Parameter[0].Guid = EncoderQuality;  
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;  
    encoderParameters.Parameter[0].NumberOfValues = 1;  
  
    // Save the image as a JPEG with quality level 100.  
    ULONG             quality;  
    quality = 100;  
    encoderParameters.Parameter[0].Value = &quality;  
    Status status = pImage->Save(pFileName, &jpgClsid, &encoderParameters);  
    if (status != Ok)   
    {  
        wprintf(L"%d Attempt to save %s failed.\n", status, pFileName);  
    }  
}  
  
  
// ����ǰ��Ļ�����ΪjpgͼƬ         
// ����   xs = ͼ��x���?   ys = ͼ��y���С,   quality = jpegͼ������         
void SaveCurScreenJpg(LPCWSTR   pszFileName,   int   xs,   int   ys,   int   quality)     
{     
    HWND hwnd = ::GetDesktopWindow();     
    HDC hdc = GetWindowDC(NULL);     
    int x = GetDeviceCaps(hdc, HORZRES);     
    int y = GetDeviceCaps(hdc, VERTRES);     
    HBITMAP hbmp = ::CreateCompatibleBitmap(hdc, x, y), hold;     
    HDC hmemdc = ::CreateCompatibleDC(hdc);     
    hold = (HBITMAP)::SelectObject(hmemdc,   hbmp);     
    BitBlt(hmemdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);     
    SelectObject(hmemdc, hold);     
      
    Bitmap bit(xs, ys), bit2(hbmp, NULL);     
    Graphics g(&bit);     
    g.ScaleTransform((float)xs/x,   (float)ys/y);     
    g.DrawImage(&bit2,   0,   0);     
      
    CLSID                           encoderClsid;     
    EncoderParameters   encoderParameters;     
      
    encoderParameters.Count   =   1;     
    encoderParameters.Parameter[0].Guid   =   EncoderQuality;     
    encoderParameters.Parameter[0].Type   =   EncoderParameterValueTypeLong;     
    encoderParameters.Parameter[0].NumberOfValues   =   1;     
    encoderParameters.Parameter[0].Value   =   &quality;     
      
    GetEncoderClsid(L"image/jpeg",   &encoderClsid);     
    bit.Save(pszFileName,   &encoderClsid,   &encoderParameters);     
     
    ::DeleteObject(hbmp);     
    ::DeleteObject(hmemdc);     
    return;     
}    

void SaveBmpToJpg(CString file,CBitmap *bmp){
    Bitmap b((HBITMAP)bmp->m_hObject,NULL);
    CLSID                           encoderClsid;     
    EncoderParameters   encoderParameters;     

    int quality = 100;
    encoderParameters.Count   =   1;     
    encoderParameters.Parameter[0].Guid   =   EncoderQuality;     
    encoderParameters.Parameter[0].Type   =   EncoderParameterValueTypeLong;     
    encoderParameters.Parameter[0].NumberOfValues   =   1;     
    encoderParameters.Parameter[0].Value   =   &quality;     

    GetEncoderClsid(L"image/jpeg",   &encoderClsid);     
    b.Save(file,   &encoderClsid,   &encoderParameters);
}

 
//FileName������bmp��dib��png��gif��jpeg/jpg��tiff��emf���ļ����ļ���   
HBITMAP   ReturnHBITMAP(CString   FileName)
{   
          Bitmap   tempBmp(FileName.AllocSysString())   ;   
        Color       backColor;         
        HBITMAP       HBitmap;     
        tempBmp.GetHBITMAP(backColor,&HBitmap);   
        return   HBitmap;  
  
}  