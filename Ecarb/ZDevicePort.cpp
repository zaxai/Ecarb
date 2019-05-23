#include "StdAfx.h"
#include "ZDevicePort.h"


ZDevicePort::ZDevicePort()
	: m_nOnceBytesSend(10240)
	, m_nOnceBytesRecv(10240)
	, m_nTimeOutSend(3000)
	, m_nTimeOutRecv(3000)
	, m_bIsRunSend(false)
	, m_bIsRunRecv(false)
	, m_strPort(_T(""))
	, m_p_promSendWait(nullptr)
	, m_p_promRecvWait(nullptr)
{
}


ZDevicePort::ZDevicePort(ZDevicePort && zdp)
	: ZSerialPort(std::move(zdp))
	, m_nOnceBytesSend(zdp.m_nOnceBytesSend)
	, m_nOnceBytesRecv(zdp.m_nOnceBytesRecv)
	, m_nTimeOutSend(zdp.m_nTimeOutSend)
	, m_nTimeOutRecv(zdp.m_nTimeOutRecv)
	, m_bIsRunSend(zdp.m_bIsRunSend)
	, m_bIsRunRecv(zdp.m_bIsRunRecv)
	, m_strPort(zdp.m_strPort)
	, m_threadSend(std::move(zdp.m_threadSend))
	, m_threadRecv(std::move(zdp.m_threadRecv))
	, m_p_promSendWait(zdp.m_p_promSendWait)
	, m_p_promRecvWait(zdp.m_p_promRecvWait)
{
	zdp.m_p_promSendWait = nullptr;
	zdp.m_p_promRecvWait = nullptr;
}


ZDevicePort & ZDevicePort::operator=(ZDevicePort && zdp)
{
	if (this != &zdp)
	{
		StopComm();
		m_strPort.Empty();
		ZSerialPort::operator=(std::move(zdp));
		m_nOnceBytesSend = zdp.m_nOnceBytesSend;
		m_nOnceBytesRecv = zdp.m_nOnceBytesRecv;
		m_nTimeOutSend = zdp.m_nTimeOutSend;
		m_nTimeOutRecv = zdp.m_nTimeOutRecv;
		m_bIsRunSend = zdp.m_bIsRunSend;
		m_bIsRunRecv = zdp.m_bIsRunRecv;
		m_strPort = zdp.m_strPort;
		m_threadSend = std::move(zdp.m_threadSend);
		m_threadRecv = std::move(zdp.m_threadRecv);
		m_p_promSendWait = zdp.m_p_promSendWait;
		m_p_promRecvWait = zdp.m_p_promRecvWait;
		zdp.m_p_promSendWait = nullptr;
		zdp.m_p_promRecvWait = nullptr;
	}
	return *this;
}


ZDevicePort::~ZDevicePort()
{
	StopComm();
	m_strPort.Empty();
}


bool ZDevicePort::OpenPort(const CString & strPort)
{
	bool bRtn=ZSerialPort::OpenPort(strPort);
	if(bRtn)
	{
		SetPortTimeOut(MAXDWORD,0,0,0,0);
		SetBufferSize(m_nOnceBytesSend*2,m_nOnceBytesRecv*2);
		m_strPort= strPort;
	}
	return bRtn;
}


void ZDevicePort::ClosePort()
{
	StopComm();
	m_strPort.Empty();
	ZSerialPort::ClosePort();
}


int ZDevicePort::SendThread(const CString & strSendData, CommCallBack * p_callback,std::promise<void> prom_Start,  std::future<void> future_Wait)
{
	m_bIsRunSend = true;
	prom_Start.set_value();
	int nRtn= ERROR_OK,i=0;
	unsigned int nStart,nSpan;
	DWORD dwIn = 0, dwOut = 0, dwErrors = 0;
	nStart=GetTickCount();
	CString strSend= strSendData.Mid(i,m_nOnceBytesSend*2);
	while(m_bIsRunSend)
	{
		ClearPortError(dwIn, dwOut, dwErrors);
		if (!dwErrors&&dwOut)
			future_Wait.wait_for(std::chrono::milliseconds(1));
		else
		{
			nRtn=SendToPort(strSend);
			if(nRtn)
				break;
			else
			{
				i+=m_nOnceBytesSend*2;
				strSend= strSendData.Mid(i,m_nOnceBytesSend*2);
				if(strSend.IsEmpty())
					break;
			}
		}
		nSpan=unsigned int(GetTickCount())-nStart;
		if(nSpan>=m_nTimeOutSend)
		{
			nRtn= ERROR_SEND_TIMEOUT;
			break;
		}
	}
	if(!m_bIsRunSend)
		nRtn= ERROR_SEND_STOP;
	if (p_callback)
		p_callback->OnSendResult(nRtn, strSendData);
	m_bIsRunSend = false;
	return nRtn;
}


bool ZDevicePort::Send(const CString & strSendData, CommCallBack * p_callback)
{
	if (!m_bIsRunSend)
	{
		WaitSendComplete();
		m_p_promSendWait = new std::promise<void>;
		std::promise<void> prom_Start;
		std::future<void> f = prom_Start.get_future();
		m_threadSend = std::thread(&ZDevicePort::SendThread, this, strSendData, p_callback, std::move(prom_Start), std::move(m_p_promSendWait->get_future()));
		f.wait();
		return true;
	}
	return false;
}


int ZDevicePort::Send(const CString & strSendData)
{
	int nRtn = ERROR_OK, i = 0;
	unsigned int nStart, nSpan;
	DWORD dwIn = 0, dwOut = 0, dwErrors = 0;
	nStart = GetTickCount();
	CString strSend = strSendData.Mid(i, m_nOnceBytesSend * 2);
	while (m_bIsRunSend)
	{
		ClearPortError(dwIn, dwOut, dwErrors);
		if (!dwErrors&&dwOut)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		else
		{
			nRtn =SendToPort(strSend);
			if (nRtn)
				break;
			else
			{
				i += m_nOnceBytesSend * 2;
				strSend = strSendData.Mid(i, m_nOnceBytesSend * 2);
				if (strSend.IsEmpty())
					break;
			}
		}
		nSpan = unsigned int(GetTickCount()) - nStart;
		if (nSpan >= m_nTimeOutSend)
		{
			nRtn = ERROR_SEND_TIMEOUT;
			break;
		}
	}
	if (!m_bIsRunSend)
		nRtn = ERROR_SEND_STOP;
	return nRtn;
}


int ZDevicePort::RecvThread(ProtocolCheck * p_pcheck, CommCallBack * p_callback, std::promise<void> prom_Start, std::future<void> future_Wait)
{
	m_bIsRunRecv = true;
	prom_Start.set_value();
	CString strRecvData;
	CString strRecvOnce;
	int nRtn= ERROR_OK;
	unsigned int nStart,nSpan;
	DWORD dwIn = 0, dwOut = 0, dwErrors = 0;
	nStart=GetTickCount();
	while(m_bIsRunRecv)
	{
		ClearPortError(dwIn, dwOut, dwErrors);
		if (!dwErrors&&dwIn)
		{
			nRtn = ZSerialPort::RecvFromPort(strRecvOnce, m_nOnceBytesRecv);
			if (nRtn)
				break;
			else
			{
				strRecvData += strRecvOnce;
				if (p_pcheck)
				{
					if (p_pcheck->CheckProtocol(strRecvData))
						break;
				}
			}
		}
		else
			future_Wait.wait_for(std::chrono::milliseconds(1));
		nSpan=unsigned int(GetTickCount())-nStart;
		if(nSpan>=m_nTimeOutRecv)
		{
			nRtn= ERROR_RECV_TIMEOUT;
			break;
		}
	}
	if(!m_bIsRunRecv)
		nRtn= ERROR_RECV_STOP;
	if (p_callback)
		p_callback->OnRecvResult(nRtn, strRecvData);
	m_bIsRunRecv = false;
	return nRtn;
}


bool ZDevicePort::Recv(ProtocolCheck * p_pcheck, CommCallBack * p_callback)
{
	if (!m_bIsRunRecv)
	{
		WaitRecvComplete();
		m_p_promRecvWait = new std::promise<void>;
		std::promise<void> prom_Start;
		std::future<void> f = prom_Start.get_future();
		m_threadRecv = std::thread(&ZDevicePort::RecvThread, this, p_pcheck, p_callback, std::move(prom_Start), std::move(m_p_promRecvWait->get_future()));
		f.wait();
		return true;
	}
	return false;
}


int ZDevicePort::Recv(ProtocolCheck * p_pcheck, CString & strRecvData)
{
	strRecvData.Empty();
	CString strRecvOnce;
	int nRtn = ERROR_OK;
	unsigned int nStart, nSpan;
	DWORD dwIn = 0, dwOut = 0, dwErrors = 0;
	nStart = GetTickCount();
	while (m_bIsRunRecv)
	{
		ClearPortError(dwIn, dwOut, dwErrors);
		if (!dwErrors&&dwIn)
		{
			nRtn = ZSerialPort::RecvFromPort(strRecvOnce, m_nOnceBytesRecv);
			if (nRtn)
				break;
			else
			{
				strRecvData += strRecvOnce;
				if (p_pcheck)
				{
					if (p_pcheck->CheckProtocol(strRecvData))
						break;
				}
			}
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		nSpan = unsigned int(GetTickCount()) - nStart;
		if (nSpan >= m_nTimeOutRecv)
		{
			nRtn = ERROR_RECV_TIMEOUT;
			break;
		}
	}
	if (!m_bIsRunRecv)
		nRtn = ERROR_RECV_STOP;
	return nRtn;
}


void ZDevicePort::SetOnceBytesSend( unsigned int nBytes)
{
	m_nOnceBytesSend= nBytes;
	SetBufferSize(m_nOnceBytesSend*2,m_nOnceBytesRecv*2);
}


void ZDevicePort::SetOnceBytesRecv( unsigned int nBytes)
{
	m_nOnceBytesRecv= nBytes;
	SetBufferSize(m_nOnceBytesSend*2,m_nOnceBytesRecv*2);
}


void ZDevicePort::SetTimeOutSend( unsigned int nTimeOut)
{
	m_nTimeOutSend= nTimeOut;
}


void ZDevicePort::SetTimeOutRecv( unsigned int nTimeOut)
{
	m_nTimeOutRecv= nTimeOut;
}


unsigned int  ZDevicePort::GetOnceBytesSend()
{
	return m_nOnceBytesSend;
}


unsigned int  ZDevicePort::GetOnceBytesRecv()
{
	return m_nOnceBytesRecv;
}


unsigned int  ZDevicePort::GetTimeOutSend()
{
	return m_nTimeOutSend;
}


unsigned int  ZDevicePort::GetTimeOutRecv()
{
	return m_nTimeOutRecv;
}


void ZDevicePort::WaitSendComplete()
{
	if (m_p_promSendWait)
		m_p_promSendWait->set_value();
	if (m_threadSend.joinable())
		m_threadSend.join();
	if (m_p_promSendWait)
	{
		delete m_p_promSendWait;
		m_p_promSendWait = nullptr;
	}
}


void ZDevicePort::WaitRecvComplete()
{
	if (m_p_promRecvWait)
		m_p_promRecvWait->set_value();
	if (m_threadRecv.joinable())
		m_threadRecv.join();
	if (m_p_promRecvWait)
	{
		delete m_p_promRecvWait;
		m_p_promRecvWait = nullptr;
	}
}


void ZDevicePort::StopSend()
{
	m_bIsRunSend = false;
	WaitSendComplete();
}


void ZDevicePort::StopRecv()
{
	m_bIsRunRecv = false;
	WaitRecvComplete();
}


void ZDevicePort::StopComm()
{
	StopSend();
	StopRecv();
}


CString ZDevicePort::GetPort()
{
	return m_strPort;
}
