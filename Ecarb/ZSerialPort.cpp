#include "StdAfx.h"
#include "ZSerialPort.h"


ZSerialPort::ZSerialPort(void)
	: m_hCom(INVALID_HANDLE_VALUE)
{
	memset(&m_olSend, 0, sizeof(OVERLAPPED));
	memset(&m_olRecv, 0, sizeof(OVERLAPPED));
	if (m_olSend.hEvent == nullptr)
		m_olSend.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (m_olRecv.hEvent == nullptr)
		m_olRecv.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
}


ZSerialPort::ZSerialPort(ZSerialPort && zsp)
	: m_hCom(zsp.m_hCom)
{
	m_olSend = zsp.m_olSend;
	m_olRecv = zsp.m_olRecv;
	zsp.m_hCom = INVALID_HANDLE_VALUE;
	zsp.m_olSend.hEvent = nullptr;
	zsp.m_olRecv.hEvent = nullptr;
}


ZSerialPort & ZSerialPort::operator=(ZSerialPort && zsp)
{
	if (this != &zsp)
	{
		ClosePort();
		if (m_olSend.hEvent)
			CloseHandle(m_olSend.hEvent);
		if (m_olRecv.hEvent)
			CloseHandle(m_olRecv.hEvent);
		m_hCom = zsp.m_hCom;
		m_olSend = zsp.m_olSend;
		m_olRecv = zsp.m_olRecv;
		zsp.m_hCom = INVALID_HANDLE_VALUE;
		zsp.m_olSend.hEvent = nullptr;
		zsp.m_olRecv.hEvent = nullptr;
	}
	return *this;
}


ZSerialPort::~ZSerialPort(void)
{
	ClosePort();
	if (m_olSend.hEvent)
		CloseHandle(m_olSend.hEvent);
	if (m_olRecv.hEvent)
		CloseHandle(m_olRecv.hEvent);
}


bool ZSerialPort::OpenPort(CString strPort)
{
	ClosePort();
	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		if (strPort.Find(_T("\\\\.\\")) < 0)
			strPort = _T("\\\\.\\") + strPort;
		DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;
		m_hCom = CreateFile(strPort, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, dwFlagsAndAttributes, nullptr);
		if (m_hCom != INVALID_HANDLE_VALUE)
			return true;
	}
	return false;
}


void ZSerialPort::ClosePort(void)
{
	if(m_hCom!= INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCom);
		m_hCom= INVALID_HANDLE_VALUE;
	}
}


int ZSerialPort::GetPortState(DCB & dcb)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=GetCommState(m_hCom,&dcb);
	if(!bRtn)
		return ERROR_PORTSTATE;
	return ERROR_OK;
}


int ZSerialPort::SetPortState(DCB & dcb)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=SetCommState(m_hCom,&dcb);
	if(!bRtn)  
		return ERROR_PORTSTATE;
	return ERROR_OK;
}


int ZSerialPort::SetPortState(DWORD dwBaudRate, BYTE cParity, BYTE cByteSize, BYTE cStopBits)
{
	DCB dcb;
	int nRtn=GetPortState(dcb);
	if(nRtn)
		return nRtn;
	dcb.BaudRate=dwBaudRate;
	dcb.Parity=cParity;
	dcb.ByteSize=cByteSize;
	dcb.StopBits=cStopBits;
	return SetPortState(dcb);
}


int ZSerialPort::SetPortState(const CString & strBaudRate, const CString & strParity, const CString & strByteSize, const CString & strStopBits)
{
	DWORD dwBaudRate;
	BYTE cBytesize,cParity,cStopBits;
	dwBaudRate=_ttol(strBaudRate);
	cBytesize=_ttoi(strByteSize);
	if(strParity==_T("None"))
		cParity =NOPARITY ;
	else if(strParity==_T("Even"))
		cParity =EVENPARITY;
	else if(strParity==_T("Odd"))
		cParity =ODDPARITY;
	else if(strParity==_T("Mark"))
		cParity =MARKPARITY;
	else
		cParity =SPACEPARITY;
	if(strStopBits==_T("1"))
		cStopBits =ONESTOPBIT  ;
	else if(strStopBits==_T("1.5"))
		cStopBits =ONE5STOPBITS;
	else
		cStopBits =TWOSTOPBITS;
	return SetPortState(dwBaudRate, cParity,cBytesize, cStopBits);
}


int ZSerialPort::SetBufferSize(DWORD dwInBuffer, DWORD dwOutBuffer)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=SetupComm(m_hCom,dwInBuffer,dwOutBuffer);
	if(!bRtn)
		return ERROR_BUFFERSIZE;
	return ERROR_OK;
}


int ZSerialPort::GetPortTimeOut(COMMTIMEOUTS  & timeout)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=GetCommTimeouts(m_hCom,&timeout);
	if(!bRtn)
		return ERROR_TIMEOUTS;
	return ERROR_OK;
}


int ZSerialPort::SetPortTimeOut(COMMTIMEOUTS  & timeout)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=SetCommTimeouts(m_hCom,&timeout);
	if(!bRtn)
		return ERROR_TIMEOUTS;
	return ERROR_OK;
}


int ZSerialPort::SetPortTimeOut(DWORD dwReadIntTmout, DWORD dwReadTolTmoutMul, DWORD dwReadTolTmoutCst, DWORD dwWriteTolTmoutMul, DWORD dwWriteTolTmoutCst)
{
	COMMTIMEOUTS TimeOut;  
	TimeOut.ReadIntervalTimeout=dwReadIntTmout;  
	TimeOut.ReadTotalTimeoutMultiplier=dwReadTolTmoutMul;  
	TimeOut.ReadTotalTimeoutConstant=dwReadTolTmoutCst;  
	TimeOut.WriteTotalTimeoutMultiplier=dwWriteTolTmoutMul;  
	TimeOut.WriteTotalTimeoutConstant=dwWriteTolTmoutCst;  
	return SetPortTimeOut(TimeOut); 
}


int ZSerialPort::SetPortTimeOut(const CString & strReadIntTmout, const CString & strReadTolTmoutMul, const CString & strReadTolTmoutCst, const CString & strWriteTolTmoutMul, const CString & strWriteTolTmoutCst)
{
	DWORD dwReadIntTmout,dwReadTolTmoutMul,dwReadTolTmoutCst,dwWriteTolTmoutMul,dwWriteTolTmoutCst;
	dwReadIntTmout=_ttol(strReadIntTmout);
	dwReadTolTmoutMul=_ttol(strReadTolTmoutMul);
	dwReadTolTmoutCst=_ttol(strReadTolTmoutCst);
	dwWriteTolTmoutMul=_ttol(strWriteTolTmoutMul);
	dwWriteTolTmoutCst=_ttol(strWriteTolTmoutCst);
	return SetPortTimeOut(dwReadIntTmout,dwReadTolTmoutMul,dwReadTolTmoutCst,dwWriteTolTmoutMul,dwWriteTolTmoutCst); 
}


int ZSerialPort::PurgePort(void)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=PurgeComm(m_hCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);  
	if(!bRtn)
		return ERROR_PURGE;
	return ERROR_OK;
}


int ZSerialPort::SendToPort(const BYTE * p_cSendData, DWORD dwSendLen, DWORD * p_dwActSendLen)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=WriteFile(m_hCom, p_cSendData,dwSendLen,nullptr,&m_olSend);
	if(!bRtn)
	{
		if(GetLastError()!=ERROR_IO_PENDING)
			return ERROR_SEND;
	}
	DWORD dwActSendLen = 0;
	if (!GetOverlappedResult(m_hCom, &m_olSend, &dwActSendLen, TRUE))
		return ERROR_SEND;
	if(p_dwActSendLen)
		*p_dwActSendLen =dwActSendLen;
	return ERROR_OK;
}


int ZSerialPort::SendToPort( CString strSendData)
{
	strSendData.Remove(_T(' '));
	int nMaxLen=strSendData.GetLength();
	if(nMaxLen==0)
		return ERROR_OK;
	BYTE *p_cSendData=new BYTE[nMaxLen];
	if(p_cSendData ==nullptr)
		return ERROR_OTHER;
	if(!CStringToByteArrBy2(strSendData, p_cSendData, nMaxLen))
	{
		delete []p_cSendData;
		return ERROR_OTHER;
	}
	DWORD dwActSendLen = 0, dwSendLen = strSendData.GetLength() / 2;
	int nRtn = SendToPort(p_cSendData, dwSendLen, &dwActSendLen);
	delete []p_cSendData;
	if (!nRtn&&dwActSendLen != dwSendLen)
		return ERROR_SEND;
	return nRtn;
}


bool ZSerialPort::CStringToByteArrBy2(const CString & str,BYTE *p_cArray,int nMaxOutBuffer)
{
	CString strByte;
	int i=0,j=0,nLen;
	nLen= str.GetLength()/2;
	if(nLen>= nMaxOutBuffer)
		return false;
	for(;i<nLen;++i,j=j+2)
	{
		strByte= str.Mid(j,2);
		p_cArray[i]=BYTE(_tcstol(strByte,nullptr ,16));
	}
	p_cArray[nLen]='\0';
	return true;
}


int ZSerialPort::RecvFromPort(BYTE * p_cRecvData,int nMaxOutBuffer,DWORD * p_dwActRecvLen)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=ReadFile(m_hCom, p_cRecvData, nMaxOutBuffer,nullptr,&m_olRecv);
	if(!bRtn)
	{
		if(GetLastError()!=ERROR_IO_PENDING)
			return ERROR_RECV;
	}
	DWORD dwActRecvLen = 0;
	if (!GetOverlappedResult(m_hCom, &m_olRecv, &dwActRecvLen, TRUE))
		return ERROR_RECV;
	if (p_dwActRecvLen)
		*p_dwActRecvLen = dwActRecvLen;
	return ERROR_OK;
}


int ZSerialPort::RecvFromPort(CString & strRecvData,int nMaxOutBuffer)
{
	BYTE *p_cOutArray=new BYTE[nMaxOutBuffer];
	if(p_cOutArray ==nullptr)
		return ERROR_OTHER;
	DWORD dwActRecvLen =0;
	int nRtn=RecvFromPort(p_cOutArray, nMaxOutBuffer,&dwActRecvLen);
	if(nRtn)
	{
		delete []p_cOutArray;
		return nRtn;
	}
	ByteArrToCStringAsHex(p_cOutArray, dwActRecvLen, strRecvData);
	delete []p_cOutArray;
	return ERROR_OK;
}


void ZSerialPort::ByteArrToCStringAsHex(const BYTE * p_cArray,DWORD dwArrayLen,CString & str)
{
	str.Empty();
	CString strByte;
	for(DWORD i=0;i< dwArrayLen;++i)
	{
		strByte.Format(_T("%02X"), p_cArray[i]);
		str +=strByte;
	}
}


int ZSerialPort::ClearPortError(DWORD & dwInQue,DWORD & dwOutQue, DWORD & dwErrors)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	COMSTAT ComStat; 
	BOOL bRtn=ClearCommError(m_hCom,&dwErrors,&ComStat);
	if(!bRtn)
		return ERROR_BUFFERSTATE;
	dwInQue = ComStat.cbInQue;
	dwOutQue = ComStat.cbOutQue;
	return ERROR_OK;
}


DWORD ZSerialPort::GetErrorInfo(void)
{
	return GetLastError();
}