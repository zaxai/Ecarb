#pragma once


// CDashboardStatic
#include "CDataItem.h"
#include "ZUtil.h"

class CDashboardStatic : public CStatic
{
	DECLARE_DYNAMIC(CDashboardStatic)

public:
	CDashboardStatic(const CDataItem * p_dataitem);
	virtual ~CDashboardStatic();

protected:
	DECLARE_MESSAGE_MAP()
private:
	const CDataItem * m_p_dataitem;
public:
	afx_msg void OnPaint();
	void DrawDashboard(CDC & dc);
	Gdiplus::PointF GetPointOnCircle(Gdiplus::REAL fAngle, Gdiplus::PointF ptCenter, Gdiplus::REAL fRadius);
	Gdiplus::REAL GetRadian(Gdiplus::REAL fAngle);
};


