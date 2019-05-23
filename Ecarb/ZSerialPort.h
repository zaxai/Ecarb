/*
** Version  1.0.0.9
** Date     2019.05.16
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com All Rights Reserved
*/
#pragma once
class ZSerialPort
{
private:
	HANDLE m_hCom;
	OVERLAPPED m_olSend;
	OVERLAPPED m_olRecv;
	ZSerialPort(const ZSerialPort & zsp) = delete;
	ZSerialPort & operator = (const ZSerialPort & zsp) = delete;
public:
	enum ERRORINFO
	{
		ERROR_OK = 0,//成功
		ERROR_INVALIDPORT,//无效端口
		ERROR_PORTSTATE,//操作端口DCB出错
		ERROR_BUFFERSIZE,//操作缓冲区出错
		ERROR_TIMEOUTS,//操作超时出错
		ERROR_PURGE,//清空缓冲区出错
		ERROR_SEND,//发送出错
		ERROR_RECV,//接收出错
		ERROR_OTHER,//其他出错
		ERROR_BUFFERSTATE,//查看缓冲区状态出错
		ERROR_SEND_TIMEOUT,//发送超时
		ERROR_SEND_STOP,//发送停止
		ERROR_RECV_TIMEOUT,//接收超时
		ERROR_RECV_STOP//接收停止
	};
	ZSerialPort(void);
	ZSerialPort(ZSerialPort && zsp);
	ZSerialPort & operator=(ZSerialPort && zsp);
	virtual ~ZSerialPort(void);
	virtual bool OpenPort(CString strPort);//使用异步串口
	virtual void ClosePort(void);
	int GetPortState(DCB & dcb);
	int SetPortState(DCB & dcb);
	int SetPortState( DWORD dwBaudRate, BYTE cParity, BYTE cByteSize, BYTE cStopBits);
	int SetPortState(const CString & strBaudRate,const CString & strParity,const CString & strByteSize,const CString & strStopBits);
	int SetBufferSize( DWORD dwInBuffer,  DWORD dwOutBuffer);
	int GetPortTimeOut(COMMTIMEOUTS  & timeout);
	int SetPortTimeOut(COMMTIMEOUTS  & timeout);
	int SetPortTimeOut( DWORD dwReadIntTmout, DWORD dwReadTolTmoutMul, DWORD dwReadTolTmoutCst, DWORD dwWriteTolTmoutMul, DWORD dwWriteTolTmoutCst);
	int SetPortTimeOut(const CString & strReadIntTmout,const CString & strReadTolTmoutMul,const CString & strReadTolTmoutCst,const CString & strWriteTolTmoutMul,const CString & strWriteTolTmoutCst);
	int PurgePort(void);
	int SendToPort(const BYTE * p_cSendData,DWORD dwSendLen,DWORD * p_dwActSendLen=nullptr);
	int SendToPort( CString strSendData);
	bool CStringToByteArrBy2(const CString & str,BYTE * p_cArray,int nMaxOutBuffer);
	int RecvFromPort(BYTE * p_cRecvData,int nMaxOutBuffer,DWORD * p_dwActRecvLen = nullptr);
	int RecvFromPort(CString & strRecvData,int nMaxOutBuffer);
	void ByteArrToCStringAsHex(const BYTE * p_cArray,DWORD dwArrayLen,CString & str);
	int ClearPortError(DWORD & dwInQue,DWORD & dwOutQue, DWORD & dwErrors);
	DWORD GetErrorInfo(void);
};

