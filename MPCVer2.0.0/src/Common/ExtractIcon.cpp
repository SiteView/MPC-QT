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
#include <winsock2.h>
#include <Windows.h>
#include <tchar.h>
#include <stdlib.h>

#define IMAGE_SIZEOF_NT_OPTIONAL32_HEADER    224

#define IMAGE_SIZEOF_NT_OPTIONAL64_HEADER    240

#include "extracticon.h"

class SmartHANDLE
{
public:
    SmartHANDLE( HANDLE h )
    {
        m_h = h;
    }
    ~SmartHANDLE( void )
    {
        if( m_h != INVALID_HANDLE_VALUE ) ::CloseHandle( m_h );
    }
    operator HANDLE( void )
    {
        return m_h;
    }
    HANDLE operator = ( HANDLE h )
    {
        if( m_h != INVALID_HANDLE_VALUE )
            ::CloseHandle( m_h );
        m_h = h;
        return m_h;
    }
private:
    HANDLE m_h;
};

class SmartFileMapping
{
public:
    SmartFileMapping( void* v )
    {
        m_v = v;
    }
    ~SmartFileMapping( void )
    {
        if( m_v != NULL ) ::UnmapViewOfFile( m_v );
    }
    operator LPVOID( void )
    {
        return m_v;
    }
private:
    void* m_v;
};

// I don't need this double '*' but just to remenber it's changing the string
int _ExpandEnvironmentStrings(  wchar_t* pText )
{
    wchar_t* p1 = _tcschr( pText, _T('%') );  // TODO: have modify by shu-yuan _tcschr to
    if( p1 == NULL )
        return -1;

    wchar_t* p2 = _tcschr( p1 +1, _T('%') );
    if( p2 == NULL )
        return -1;

    wchar_t provi[MAX_PATH +1];
    provi[0] = TCHAR(0);
    TCHAR env[MAX_PATH +1];
    p1 = pText;
    wchar_t* d = provi;
    while( *p1 != wchar_t(0) )
    {
        if( *p1 != _T('%') )
        {
            *d++ = *p1++;
            continue;
        }

// find %
        p2 = _tcschr( p1 +1, _T('%') );
        if( p2 == NULL )
            return -1;

        lstrcpyn( env, p1 +1, int(p2 - p1) );
        wchar_t newEnv[MAX_PATH +1];
        DWORD res = ::GetEnvironmentVariable( env, newEnv, MAX_PATH );
        if( res > 0 )
        {
            lstrcpy( d, newEnv );
            d += lstrlen( newEnv );
        }
        p1 = p2 +1;
    }
    *d = wchar_t(0);

    if( lstrlen( provi ) != 0 )
    {
        lstrcpy( pText, provi );
    }

    return 0;
}

ExtractIcons::ExtractIcons(void)
{
}

ExtractIcons::~ExtractIcons(void)
{
}


#define MAGIC_ICON       0
#define MAGIC_ICO1       1
#define MAGIC_CUR        2
#define MAGIC_BMP        ((WORD)'B'+((WORD)'M'<<8))

#define MAGIC_ANI1       ((WORD)'R'+((WORD)'I'<<8))
#define MAGIC_ANI2       ((WORD)'F'+((WORD)'F'<<8))
#define MAGIC_ANI3       ((WORD)'A'+((WORD)'C'<<8))
#define MAGIC_ANI4       ((WORD)'O'+((WORD)'N'<<8))

#define VER30            0x00030000


void* ExtractIcons::_RelativeVirtualAddresstoPtr( IMAGE_DOS_HEADER* pDosHeader, DWORD rva )
{
    IMAGE_NT_HEADERS32* pPE = (IMAGE_NT_HEADERS32*)((BYTE*)pDosHeader + pDosHeader->e_lfanew);

//  scan the section table looking for the RVA
    IMAGE_SECTION_HEADER* pSection = IMAGE_FIRST_SECTION( pPE );

    for( int i = 0; i < pPE->FileHeader.NumberOfSections; i++ )
    {
        DWORD size = pSection[i].Misc.VirtualSize ?
                     pSection[i].Misc.VirtualSize : pSection[i].SizeOfRawData;

        if( rva >= pSection[i].VirtualAddress &&
                rva <  pSection[i].VirtualAddress + size)
        {
            return (LPBYTE)pDosHeader + pSection[i].PointerToRawData + (rva - pSection[i].VirtualAddress);
        }
    }

    return NULL;
}

void* ExtractIcons::_GetResourceTable( IMAGE_DOS_HEADER* pDosHeader )
{
    IMAGE_NT_HEADERS32* pPE = (IMAGE_NT_HEADERS32*)((BYTE*)pDosHeader + pDosHeader->e_lfanew);

    if( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
        return NULL;
    if( pPE->Signature != IMAGE_NT_SIGNATURE )
        return NULL;
    if( pPE->FileHeader.SizeOfOptionalHeader < IMAGE_SIZEOF_NT_OPTIONAL32_HEADER )
        return NULL;

// The DataDirectory is an array of 16 structures.
// Each array entry has a predefined meaning for what it refers to.
    return _RelativeVirtualAddresstoPtr( pDosHeader,
                                         pPE->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress );
}

IMAGE_RESOURCE_DIRECTORY_ENTRY* ExtractIcons::_FindResourceBase( void* prt, int resType, int* pCount )
{
    IMAGE_RESOURCE_DIRECTORY* pDir = (IMAGE_RESOURCE_DIRECTORY*)prt;

    *pCount = 0;

// first find the type always a ID so ignore strings totaly
    int count  = pDir->NumberOfIdEntries + pDir->NumberOfNamedEntries;
    IMAGE_RESOURCE_DIRECTORY_ENTRY* pRes;
    pRes = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir+1);

    int i;
    for( i = 0; i < count; i++ )
    {
        if( pRes[i].Name == (DWORD)resType )
            break;
    }

    if( i == count )             // did not find the type
        return NULL;

    pDir = (IMAGE_RESOURCE_DIRECTORY*)((LPBYTE)prt +
                                       (pRes[i].OffsetToData & ~IMAGE_RESOURCE_DATA_IS_DIRECTORY));

    count  = pDir->NumberOfIdEntries + pDir->NumberOfNamedEntries;
    *pCount = count;
    pRes = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir+1);
    return pRes;
}

int ExtractIcons::_FindResourceCount( void* prt, int resType )
{
    int count;
    IMAGE_RESOURCE_DIRECTORY_ENTRY* pRes;
    pRes = _FindResourceBase( prt, resType, &count );
    return count;
}

void* ExtractIcons::_FindResource( IMAGE_DOS_HEADER* pDosHeader, void* prt, int resIndex, int resType, DWORD* pcbSize )
{
    int count;
    IMAGE_RESOURCE_DIRECTORY_ENTRY* pRes;
    pRes = _FindResourceBase( prt, resType, &count );

    int index = 0;
// find the actual resource
//      id (resIndex < 0)
//      ordinal (resIndex >= 0)
    if( resIndex < 0 )
    {
        for( index = 0; index < count; index++ )
            if( pRes[index].Name == (DWORD)(-resIndex) )
                break;
    }
    else
    {
        index = resIndex;
    }

    if( index >= count )
        return NULL; // index out of range

// another Directory
// probably language take the first one
    if( pRes[index].OffsetToData & IMAGE_RESOURCE_DATA_IS_DIRECTORY)
    {
        IMAGE_RESOURCE_DIRECTORY* pDir;
        pDir = (IMAGE_RESOURCE_DIRECTORY*)((LPBYTE)prt + (pRes[index].OffsetToData & ~IMAGE_RESOURCE_DATA_IS_DIRECTORY) );
        pRes = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir+1);
        index = 0;
    }

// Oooooooooooo no !! another Directory !!!
    if( pRes[index].OffsetToData & IMAGE_RESOURCE_DATA_IS_DIRECTORY )
        return NULL;

    IMAGE_RESOURCE_DATA_ENTRY* pEntry;
    pEntry = (IMAGE_RESOURCE_DATA_ENTRY*)((LPBYTE)prt + pRes[index].OffsetToData);

// all OffsetToData fields except the final one are relative to
// the start of the section.  the final one is a virtual address
// we need to go back to the header and get the virtual address
// of the resource section to do this right.
    *pcbSize = pEntry->Size;
    return _RelativeVirtualAddresstoPtr( pDosHeader, pEntry->OffsetToData );
}

UINT ExtractIcons::_ExtractFromExe( HANDLE hFile, int iconIndex,
                                    int cxIconSize, int cyIconSize,
                                    HICON *phicon, UINT *piconid,
                                    UINT maxIcons, UINT flags )
{
    DWORD fileLen = GetFileSize( hFile, NULL );
    SmartHANDLE hFileMap( CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, 0, NULL ) );
    if( hFileMap == NULL )
        return 0;

    SmartFileMapping pFile( MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0) );
    if( pFile == NULL )
        return 0;

    IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)(void*)pFile;
    if( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
        return 0;
    if( pDosHeader->e_lfanew <= 0 )
        return 0;
    if( DWORD(pDosHeader->e_lfanew) >= fileLen )
        return 0;

    void* pRes = _GetResourceTable( pDosHeader );
    if( pRes == NULL)
        return 0; // cant find the resource

    DWORD cbSize = 0;

    if( phicon == NULL )
    {
//  we want the count
        int count = _FindResourceCount( pRes, (int)RT_GROUP_ICON );
        return count;
    }

    UINT res = 0;
    while( res < maxIcons )
    {
//  find the icon dir for this icon.
        NEWHEADER* pIconDir = (NEWHEADER*)_FindResource( pDosHeader, pRes, iconIndex, (int)RT_GROUP_ICON, &cbSize );
        if( pIconDir == NULL )
            return res;

        if( pIconDir->Reserved != 0 || pIconDir->ResType != RES_ICON ) // 1 == iconType
        {
            return res;
        }

        int idIcon = LookupIconIdFromDirectoryEx( (LPBYTE)pIconDir, TRUE,
                     cxIconSize, cyIconSize, flags );
        void* pIcon = _FindResource( pDosHeader, pRes, -idIcon, (int)RT_ICON, &cbSize );
        if( pIcon == NULL )
            return res;

        if( (((LPBITMAPINFOHEADER)pIcon)->biSize != sizeof(BITMAPINFOHEADER)) &&
                (((LPBITMAPINFOHEADER)pIcon)->biSize != sizeof(BITMAPCOREHEADER)) )
        {
            return res;
        }

        if( piconid )
            piconid[res] = idIcon;

        phicon[res] = CreateIconFromResourceEx( (LPBYTE)pIcon, cbSize,
                                                TRUE, VER30, cxIconSize, cyIconSize, flags );

        res++;
        iconIndex++;       // next icon index
    }

    return res;
}

UINT ExtractIcons::_ExtractFromBMP( LPCTSTR pFileName, int iconIndex,
                                    int cxIcon, int cyIcon,
                                    HICON* phicon, UINT flags )
{
    if( iconIndex >= 1 )
        return 0;

    flags |= LR_LOADFROMFILE;
    HBITMAP hbm = (HBITMAP)LoadImage( NULL, pFileName, IMAGE_BITMAP,
                                      cxIcon, cyIcon, flags );
    if( hbm == NULL )
        return 0;

    if( phicon == NULL )
    {
        DeleteObject(hbm);
        return 1;
    }

    HBITMAP hbmMask = CreateBitmap( cxIcon, cyIcon, 1, 1, NULL );

    HDC hdc = CreateCompatibleDC(NULL);
    SelectObject( hdc, hbm );

    HDC hdcMask = CreateCompatibleDC(NULL);
    SelectObject(hdcMask, hbmMask);

    SetBkColor( hdc, GetPixel(hdc, 0, 0) );
// this ROP Code will leave bits in the destination bitmap the same color if the
// corresponding source bitmap's bit are black.
// all other bits in the destination (where source bits are not black)
// are turned to black.
#define DSTERASE 0x00220326 // dest = dest & (~src) :
    BitBlt( hdcMask, 0, 0, cxIcon, cyIcon, hdc, 0, 0, SRCCOPY );
    BitBlt( hdc, 0, 0, cxIcon, cyIcon, hdcMask, 0, 0, DSTERASE );

    ICONINFO ii;
    ii.fIcon    = TRUE;
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmColor = hbm;
    ii.hbmMask  = hbmMask;
    HICON hicon = CreateIconIndirect( &ii );

    DeleteObject(hdc);
    DeleteObject(hbm);
    DeleteObject(hdcMask);
    DeleteObject(hbmMask);

    *phicon = hicon;
    return 1;
}

UINT ExtractIcons::_ExtractFromICO( LPCTSTR pFileName, int iconIndex,
                                    int cxIcon, int cyIcon,
                                    HICON* phicon, UINT flags )
{
    if( iconIndex >= 1 )
        return 0;

    flags |= LR_LOADFROMFILE;
    HICON hicon = (HICON)LoadImage( NULL, pFileName, IMAGE_ICON, cxIcon, cyIcon, flags );
    if( hicon == NULL )
        return 0;

//  do we just want a count?
    if( phicon == NULL )
        DestroyIcon( hicon );
    else
        *phicon = hicon;

    return 1;
}

UINT ExtractIcons::_ExtractIcons( LPCTSTR pFileName, int iconIndex,
                                  int cxIcon, int cyIcon,
                                  HICON* phicon, UINT* piconid, UINT maxIcons, UINT flags )
{
    SmartHANDLE hFile( INVALID_HANDLE_VALUE );

    if( phicon != NULL )
        *phicon = NULL;
    if( piconid != NULL )
        *piconid = (UINT)-1;

    TCHAR  fileName[MAX_PATH];
    lstrcpy( fileName, pFileName );

    _ExpandEnvironmentStrings( fileName );

    TCHAR  expFileName[MAX_PATH];
    lstrcpy( expFileName, fileName );
    if( SearchPath( NULL, expFileName, NULL, MAX_PATH, fileName, NULL ) == 0 )
        return 0; // error...

    hFile = CreateFile( fileName, GENERIC_READ|FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if( hFile == INVALID_HANDLE_VALUE )
    {
        hFile = CreateFile( fileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

        if( hFile == INVALID_HANDLE_VALUE )
            return 0;
    }
    else
    {
// Restore the Access Date
        FILETIME ftAccess;
        if( GetFileTime( hFile, NULL, &ftAccess, NULL ) )
            SetFileTime( hFile, NULL, &ftAccess, NULL );
    }


    WORD magic[6];
    DWORD read = 0;
    if( ! ReadFile( hFile,&magic, sizeof(magic), &read, NULL ) )
        return 0;
    if( read != sizeof(magic) )
        return 0;

    UINT res = 0;
    switch( magic[0] )
    {
    case IMAGE_DOS_SIGNATURE:
        res = _ExtractFromExe( hFile, iconIndex, cxIcon, cyIcon, phicon, piconid, maxIcons, flags );
        break;

    case MAGIC_ANI1: //  ani cursors are RIFF file of type 'ACON'
        if( magic[1] == MAGIC_ANI2 &&
                magic[4] == MAGIC_ANI3 &&
                magic[5] == MAGIC_ANI4 )
            res = _ExtractFromICO( fileName, iconIndex, cxIcon, cyIcon, phicon, flags );
        break;

    case MAGIC_BMP:
        res = _ExtractFromBMP( fileName, iconIndex, cxIcon, cyIcon, phicon, flags );
        break;

    case MAGIC_ICON:
//  icons and cursors format:
//      reserved       : always zero
//      resourceType   : 1 for icons 2 cor cursor.
//      countIcons       : images count
// we want countIcons >= 1
        if( (magic[1] == MAGIC_ICON || magic[1] == MAGIC_CUR ) &&
                magic[2] >= 1 )
        {
            res = _ExtractFromICO( fileName, iconIndex, cxIcon, cyIcon, phicon, flags );
        }
        break;
    }

    return res;
}

UINT ExtractIcons::Get( LPCTSTR pFileName, int iconIndex,
                        int cxIcon, int cyIcon,
                        HICON* phicon, UINT* piconid,
                        UINT maxIcons, UINT flags )
{
    return _ExtractIcons( pFileName, iconIndex,
                          cxIcon, cyIcon,
                          phicon, piconid,
                          maxIcons, flags );
}

struct TCursorOrIcon
{
    WORD Reserved;
    WORD wType;
    WORD Count;
};

struct TIconRec
{
    BYTE Width;
    BYTE Height;
    WORD Colors;
    WORD Reserved1;
    WORD Reserved2;
    long DIBSize;
    long DIBOffset;
};

static void InitBmpInfoHeader(HBITMAP Bitmap,BITMAPINFOHEADER& BI, int nBit)
{
    int Bytes;
    DIBSECTION DS;

    DS.dsBmih.biSize = 0;
    Bytes = GetObject(Bitmap, sizeof(DS), &DS);
    if (Bytes>=sizeof(DS.dsBm)+sizeof(DS.dsBmih) && DS.dsBmih.biSize>=sizeof(DS.dsBmih))
        memcpy(&BI, &DS.dsBmih, sizeof(BITMAPINFOHEADER));
    else
    {
        memset(&BI, 0, sizeof(BI));
        BI.biSize = sizeof(BI);
        BI.biWidth = DS.dsBm.bmWidth;
        BI.biHeight = DS.dsBm.bmHeight;
        BI.biBitCount = DS.dsBm.bmPlanes
                        * DS.dsBm.bmBitsPixel;
    }
    if (nBit!=0) BI.biBitCount = nBit;
    if (BI.biBitCount <= 8)
        BI.biClrUsed = 1<<BI.biBitCount;
    BI.biPlanes = 1;
    if (BI.biClrImportant > BI.biClrUsed)
        BI.biClrImportant = BI.biClrUsed;
    if (BI.biSizeImage == 0)
        BI.biSizeImage = ((BI.biWidth*BI.biBitCount+31) / 32) * 4 * BI.biHeight;
}

static void GetDIBSizes(HBITMAP Bitmap,DWORD& InfoSize, DWORD& ImageSize,int nBit)
{
    BITMAPINFOHEADER BI;

    InitBmpInfoHeader(Bitmap, BI, nBit);
    InfoSize = sizeof(BITMAPINFOHEADER);
    if (BI.biBitCount > 8)
    {
        if (BI.biCompression&BI_BITFIELDS)
            InfoSize += 12;
    }
    else
        InfoSize += sizeof(RGBQUAD) * (BI.biClrUsed!=0 ? BI.biClrUsed :(1 << BI.biBitCount));
    ImageSize = BI.biSizeImage;
}

static void GetDIB(HBITMAP Bitmap,BITMAPINFO* BmpInfo, void* Bits, int nBit)
{
    HDC DC;
    DC = CreateCompatibleDC(NULL);
    InitBmpInfoHeader(Bitmap, BmpInfo->bmiHeader, nBit);
    GetDIBits(DC, Bitmap, 0, BmpInfo->bmiHeader.biHeight, Bits,BmpInfo, DIB_RGB_COLORS);
    DeleteDC(DC);
}

BOOL  SaveIcon(HICON Icon, LPCWSTR FileName, int nBit)
{
    BOOL ret;
    UINT nColor;
    long Length;
    void *MonoBits;
    void *ColorBits;
    DWORD dwWrite;
    HANDLE hFile;
    TIconRec List;
    ICONINFO IconInfo;
    TCursorOrIcon CI;
    DWORD MonoInfoSize;
    DWORD ColorInfoSize;
    DWORD MonoBitsSize;
    DWORD ColorBitsSize;
    BITMAPINFO *MonoInfo;
    BITMAPINFO *ColorInfo;

    switch(nBit)
    {
    case 0:
    case 1:
    case 4:
    case 8:
    case 16:
    case 24:
    case 32:
        break;
    default:
        return FALSE;
    }
    if (Icon==NULL || !FileName)
        return FALSE;

    hFile = CreateFile(FileName,GENERIC_WRITE, FILE_SHARE_READ,0, CREATE_ALWAYS, 0, 0);
    if (hFile==INVALID_HANDLE_VALUE)
        return false;
    memset(&CI, 0, sizeof(CI));
    memset(&List, 0, sizeof(List));
    GetIconInfo(Icon, &IconInfo);

    GetDIBSizes(IconInfo.hbmMask,MonoInfoSize, MonoBitsSize, 1);
    GetDIBSizes(IconInfo.hbmColor,ColorInfoSize, ColorBitsSize,nBit);
    MonoInfo = (BITMAPINFO*)malloc(MonoInfoSize);
    ColorInfo = (BITMAPINFO*)malloc(ColorInfoSize);
    MonoBits = malloc(MonoBitsSize);
    ColorBits = malloc(ColorBitsSize);
    GetDIB(IconInfo.hbmMask, MonoInfo,MonoBits, 1);
    GetDIB(IconInfo.hbmColor, ColorInfo,ColorBits, nBit);
    CI.wType = 0x10001;
    CI.Count = 1;
    ret &= WriteFile(hFile, &CI, sizeof(CI),&dwWrite, NULL);
    List.Width = ColorInfo->bmiHeader.biWidth;
    List.Height = ColorInfo->bmiHeader.biHeight;
    List.Colors = ColorInfo->bmiHeader.biPlanes * ColorInfo->bmiHeader.biBitCount;
    List.DIBSize = ColorInfoSize + ColorBitsSize + MonoBitsSize;
    List.DIBOffset = sizeof(CI) + sizeof(List);
    ret &= WriteFile(hFile, &List, sizeof(List), &dwWrite, NULL);
    ColorInfo->bmiHeader.biHeight *= 2;
    ret &= WriteFile(hFile, ColorInfo,ColorInfoSize, &dwWrite, NULL);
    ret &= WriteFile(hFile, ColorBits,ColorBitsSize, &dwWrite, NULL);
    ret &= WriteFile(hFile, MonoBits,MonoBitsSize, &dwWrite, NULL);
    free(ColorInfo);
    free(MonoInfo);
    free(ColorBits);
    free(MonoBits);
    DeleteObject(IconInfo.hbmColor);
    DeleteObject(IconInfo.hbmMask);

    CloseHandle(hFile);
    return ret;
}

#if 0
example for this

HICON* pIcons = (HICON*)LocalAlloc( LPTR, 100*sizeof(HICON) );
int cIcons;
if( pIcons != NULL )
{

    cIcons = (int)ExtractIcons::Get( _T("C:\\Program Files\\HaoZip\\HaoZip.exe"), 0, 128, 128, pIcons, NULL, 100, LR_COLOR );

}

SaveIcon(pIcons[0],_T("c:\\tmp.ico"),32);

GlobalFree(pIcons);

#endif
