// AES.h: interface for the AES class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AES_H__24984881_87F5_447A_838A_57873C600813__INCLUDED_)
#define AFX_AES_H__24984881_87F5_447A_838A_57873C600813__INCLUDED_

#include <string.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <QString>
class AES

{

public:
    AES(QString key);
    AES(unsigned char* key);

    virtual ~AES();
    //加密整段文字
    void Bm53Cipher(char *input,char *output);
    void Bm53InvCipher(char *inut, char *output);
    void InvCipher(char *inut, char *output);
    void OnAesUncrypt(const QString &sInBuffer, QString &sOutUncryptData);
    void OnAesEncrypt(const QString &sInBuffer, QString &sOutUncryptData);

private:
    unsigned char* Cipher(unsigned char* input);
    unsigned char* InvCipher(unsigned char* input);
    void* Cipher(void* input, int length=0);
    void* InvCipher(void* input, int length);
    void Cipher(char *input, char *output);
    unsigned char Sbox[256];
    unsigned char InvSbox[256];
    unsigned char w[11][4][4];
    void KeyExpansion(unsigned char* key, unsigned char w[][4][4]);
    unsigned char FFmul(unsigned char a, unsigned char b);
    void SubBytes(unsigned char state[][4]);
    void ShiftRows(unsigned char state[][4]);
    void MixColumns(unsigned char state[][4]);
    void AddRoundKey(unsigned char state[][4], unsigned char k[][4]);
    void InvSubBytes(unsigned char state[][4]);
    void InvShiftRows(unsigned char state[][4]);
    void InvMixColumns(unsigned char state[][4]);
    int strToHex(const char *ch, char *hex);
    int hexToStr(const char *hex, char *ch);
    int ascillToValue(const char ch);
    char valueToHexCh(const int value);
    int getUCharLen(const unsigned char *uch);
    int strToUChar(const char *ch, unsigned char *uch);
    int ucharToStr(const unsigned char *uch, char *ch);
    int ucharToHex(const unsigned char *uch, char *hex);
    int hexToUChar(const char *hex, unsigned char *uch);
};


#endif // !defined(AFX_AES_H__24984881_87F5_447A_838A_57873C600813__INCLUDED_)
