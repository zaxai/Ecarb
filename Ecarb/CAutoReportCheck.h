#pragma once
#include "ZDevicePort.h"
class CAutoReportCheck:public ZDevicePort::ProtocolCheck
{
private:
	int m_nLength;
public:
	CAutoReportCheck();
	virtual ~CAutoReportCheck();
	void SetLength(int nLength);
	bool CheckProtocol(const CString & str);
};

