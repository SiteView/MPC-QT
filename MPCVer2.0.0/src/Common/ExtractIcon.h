/*
********************************************************************************************
Discription: 图标资源输出.

Written By:		周志光
Date:			2013-08-05
Version:		1.0
Mail:			zhouklansman@gmail.com

Modified by:
Modified Date:
Version:
Discription:
********************************************************************************************
*/

#pragma once

BOOL  SaveIcon(HICON Icon, LPCWSTR FileName, int nBit);

class ExtractIcons
{
public:
    ExtractIcons(void);
    ~ExtractIcons(void);
    static UINT Get( LPCTSTR pFileName, int iconIndex,
                     int cxIcon, int cyIcon,
                     HICON* phicon, UINT* piconid,
                     UINT maxIcons, UINT flags /* LR_COLOR ( for full color )*/ );
private:
    struct NEWHEADER
    {
        WORD    Reserved;
        WORD    ResType;
        WORD    ResCount;
    };

    enum
    {
        COM_FILE = 1,
        BAT_FILE,
        CMD_FILE,
        PIF_FILE,
        LNK_FILE,
        ICO_FILE,
        EXE_FILE,
    };

    static UINT _ExtractIcons( LPCTSTR pFileName, int iconIndex,
                               int cxIcon, int cyIcon,
                               HICON* phicon, UINT* piconid,
                               UINT maxIcons, UINT flags );

    static UINT _ExtractFromExe( HANDLE hFile, int iconIndex,
                                 int cxIconSize, int cyIconSize,
                                 HICON *phicon, UINT *piconid,
                                 UINT maxIcons, UINT flags );
    static UINT _ExtractFromBMP( LPCTSTR pFileName, int iconIndex,
                                 int cxIcon, int cyIcon,
                                 HICON* phicon, UINT flags );
    static UINT _ExtractFromICO( LPCTSTR pFileName, int iconIndex,
                                 int cxIcon, int cyIcon,
                                 HICON* phicon, UINT flags );

    static void* _FindResource( IMAGE_DOS_HEADER* pDosHeader, void* prt, int resIndex, int resType, DWORD* pcbSize );
    static int _FindResourceCount( void* prt, int resType );
    static IMAGE_RESOURCE_DIRECTORY_ENTRY* _FindResourceBase( void* prt, int resType, int* pCount );
    static void* _GetResourceTable( IMAGE_DOS_HEADER* pDosHeader );
    static void* _RelativeVirtualAddresstoPtr( IMAGE_DOS_HEADER* pDosHeader, DWORD rva );
    static int _HasExtension( LPCTSTR pPath );
};
