#pragma once
#include "ZDevicePort.h"
class ZModbus:public ZDevicePort::ProtocolCheck
{
public:
	enum ERRORINFO
	{
		ERROR_OK = 0,//成功
		ERROR_FUNCCODE,//功能码错误
		ERROR_PROTOCOL//协议检查错误
	};
	enum FIXEDINDEX
	{
		INDEX_DEVICEADDR=0,
		INDEX_FUNCCODE,
		INDEX_AUTOENABLE,
		INDEX_REGSTARTADDR,
		INDEX_REGNUM
	};
private:
	CString m_strDeviceAddr;
	CString m_strFuncCode;
	CString m_strAutoEnable;
	CString m_strRegStartAddr;
	CString m_strRegNum;
	CString m_strTgtData;
	CString m_strCheckCode;
public:
	ZModbus();
	virtual ~ZModbus();
	virtual bool CheckProtocol(const CString & str);
	bool CStringToBYTEArrBy2(const CString & str, BYTE * p_c, int nMaxByteLen);
	CString ReverseCStringBy2(CString str);
	UINT16 GetCRC16(BYTE * p_c, unsigned int nByteLen);
	CString GetCRC16(const CString & str);
	int DataEncode(CString & strData);//发送帧组帧
	int DataDecode(CString strData, std::vector<CString> & vec_strFixedData, std::vector<CString> & vec_strValue);//数据帧解帧
	void Init(const CString & strDeviceAddr, const CString & strFuncCode, const CString & strAutoEnable, const CString & strRegStartAddr, const CString & strRegNum, const CString & strTgtData);
};

