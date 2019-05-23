#pragma once


// CWaveStatic
#include "CDataItem.h"
#include "ZUtil.h"
#include <deque>

class CWaveStatic : public CStatic
{
	DECLARE_DYNAMIC(CWaveStatic)

public:
	CWaveStatic(const CDataItem * p_dataitem);
	virtual ~CWaveStatic();

protected:
	DECLARE_MESSAGE_MAP()
private:
	const CDataItem * m_p_dataitem;
	DWORD m_dwTimeStart;
	DWORD m_dwTimeCount;
	std::deque<double> m_deque_dTgtData;
	std::deque<DWORD> m_deque_dwTgtTime;
public:
	afx_msg void OnPaint();
	void DrawWave(CDC & dc);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void StartComm(DWORD dwTimeStart);
	void StopComm();
	void AddTgtData(double dTgtData,DWORD dwTgtTime,BOOL nRepaint = TRUE);
};


