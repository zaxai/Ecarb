#pragma once


// CStatusLEDStatic
#include "CDataItem.h"
#include "ZUtil.h"

class CStatusLEDStatic : public CStatic
{
	DECLARE_DYNAMIC(CStatusLEDStatic)

public:
	CStatusLEDStatic(const CDataItem * p_dataitem);
	virtual ~CStatusLEDStatic();

protected:
	DECLARE_MESSAGE_MAP()
private:
	const CDataItem * m_p_dataitem;
public:
	afx_msg void OnPaint();
	void DrawStatusLED(CDC & dc);
};


