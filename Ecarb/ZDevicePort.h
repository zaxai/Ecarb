/*
** Version  1.0.0.13
** Date     2019.05.22
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com All Rights Reserved
*/
#pragma once
#include "ZSerialPort.h"
#include <thread>
#include <future>

class ZDevicePort :public ZSerialPort
{
public:
	class ProtocolCheck
	{
	public:
		virtual bool CheckProtocol(const CString & str) = 0;//接收数据判断协议
	};
	class CommCallBack
	{
	public:
		virtual void OnSendResult(int nResult,CString strSendData) = 0;//发送结果回调
		virtual void OnRecvResult(int nResult,CString strRecvData) = 0;//接收结果回调
	};
private:
	unsigned int m_nOnceBytesSend;
	unsigned int m_nOnceBytesRecv;
	unsigned int m_nTimeOutSend;
	unsigned int m_nTimeOutRecv;
	bool m_bIsRunSend;
	bool m_bIsRunRecv;
	CString m_strPort;
	std::thread m_threadSend;
	std::thread m_threadRecv;
	std::promise<void> * m_p_promSendWait;
	std::promise<void> * m_p_promRecvWait;
	int SendThread(const CString & strSendData, CommCallBack * p_callback, std::promise<void> prom_Start, std::future<void> future_Wait);
	int RecvThread(ProtocolCheck * p_pcheck, CommCallBack * p_callback, std::promise<void> prom_Start, std::future<void> future_Wait);
public:
	ZDevicePort();
	ZDevicePort(ZDevicePort && zdp);
	ZDevicePort & operator=(ZDevicePort && zdp);
	virtual ~ZDevicePort();
	virtual bool OpenPort(const CString & strPort);
	virtual void ClosePort();
	bool Send(const CString & strSendData, CommCallBack * p_callback);
	int Send(const CString & strSendData);
	bool Recv(ProtocolCheck * p_pcheck, CommCallBack * p_callback);
	int Recv(ProtocolCheck * p_pcheck, CString & strRecvData);
	void SetOnceBytesSend( unsigned int nBytes);
	void SetOnceBytesRecv( unsigned int nBytes);
	void SetTimeOutSend( unsigned int nTimeOut);
	void SetTimeOutRecv( unsigned int nTimeOut);
	unsigned int GetOnceBytesSend();
	unsigned int GetOnceBytesRecv();
	unsigned int GetTimeOutSend();
	unsigned int GetTimeOutRecv();
	void WaitSendComplete();
	void WaitRecvComplete();
	void StopSend();
	void StopRecv();
	void StopComm();
	CString GetPort();
};

