#pragma once
class CConfig
{
public:
    CConfig(void);
    ~CConfig(void);

    static CConfig & Single();
    void Load();

    void SetDefWidth( int width );
    void SetDefHeight( int height );
    void SetCapture(bool capture);
    void SetRestore(bool restore);

    int width;
    int height;
    bool capture;
    bool restore;
	CString waterMask; // Ë®Òø

private:
    void Set(CString key,int val);
    CString m_iniFile;
};

