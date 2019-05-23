/*
** Version  1.0.0.4
** Date     2019.05.01
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#ifndef _ZUTIL_H
#define _ZUTIL_H
#include <lm.h>
#pragma comment(lib, "netapi32.lib")
#include <vector>
#include <future>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

class ZUtil
{
public:
	enum FILEENCODING
	{
		ENCODING_FAIL = 0,
		ENCODING_ANSI,
		ENCODING_UTF8,
		ENCODING_UNICODE,
		ENCODING_UNICODEBIGENDIAN,
	};
public:
	static void StrSplit(CString strSrc, std::vector<CString> & vec_strData, CString strSplit, bool bLastExist = false);
	static void StrSplit(CString strSrc, std::vector<CString> & vec_strData, TCHAR cSplit, bool bLastExist = false);
	static BOOL AtoA(const char * p_cSrc, char * p_cDest, const int & nDesSize);
	static BOOL WtoA(const wchar_t * p_cSrc, char * p_cDest, const int & nDesSize);
	static BOOL TtoA(const TCHAR * p_cSrc, char * p_cDest, const int & nDesSize);
	static BOOL WtoUTF8(const wchar_t * p_cSrc, char * p_cDest, const int & nDesSize);
	static BOOL TtoUTF8(const TCHAR * p_cSrc, char * p_cDest, const int & nDesSize);
	static BOOL AtoW(const char * p_cSrc, wchar_t * p_cDest, const int & nDesSize);
	static BOOL AtoT(const char * p_cSrc, TCHAR * p_cDest, const int & nDesSize);
	static BOOL UTF8toW(const char * p_cSrc, wchar_t * p_cDest, const int & nDesSize);
	static BOOL UTF8toT(const char * p_cSrc, TCHAR * p_cDest, const int & nDesSize);
	static BOOL AtoUTF8(const char * p_cSrc, char * p_cDest, const int & nDesSize);
	static BOOL UTF8toA(const char * p_cSrc, char * p_cDest, const int & nDesSize);
	static BOOL IsStrUTF8(char* p_str, unsigned long long llLength);
	static int GetFileEncoding(const CString & strFilePath);
	static BOOL IsLittle_Endian(void);
	static BOOL ReadFile2CString(const CString & strFilePath, CString & strDest);
	static CString GetExeCatalogPath(void);
	static CString GetExePath(void);
	static void BrowseCurrentDir(const CString & strFolder, std::vector<CString> & vec_strPaths);
	static BOOL IsRunasAdmin(void);
	static CString GetWinVersion(void);
	static std::vector<CString> GetWMIData(const CString & strWQL, const CString & strKey);
	static CString GetDoubleString(double d, int nDecimalDigits);
};
#endif