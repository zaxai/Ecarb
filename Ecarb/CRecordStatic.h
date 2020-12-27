#pragma once


// CRecordStatic
#include "CRecordItem.h"

class CRecordStatic : public CStatic
{
	DECLARE_DYNAMIC(CRecordStatic)

public:
	CRecordStatic();
	virtual ~CRecordStatic();

protected:
	DECLARE_MESSAGE_MAP()
private:
	std::vector<CRecordItem> m_vec_rditem;
	std::vector<Gdiplus::Color> m_vec_crItem;
	std::vector<Gdiplus::REAL> m_vec_fWidth;
	int m_nSelItem;
	Gdiplus::PointF m_ptMouseMove;
	int m_nClickRecordDataID;
	std::vector<std::vector<Gdiplus::RectF>> m_vec2_rcDataRange;
	std::vector<std::deque<CRecordData>> m_vec_deque_rddata;
	DWORD m_dwTimeOrigin;
	DWORD m_dwMaxSpan;
	Gdiplus::PointF m_ptOrigin;
	Gdiplus::PointF m_ptX;
	Gdiplus::PointF m_ptY;
	CPoint m_ptDragStart;
	CPoint m_ptDragEnd;
	bool m_bIsDragging;
	void SetOriginTimeByDrag();
	DWORD GetPointTime(CPoint pt);
	void SetOriginTimeByPointTime(CPoint pt,DWORD dwTime);
public:
	afx_msg void OnPaint();
	void DrawRecord(CDC & dc);
	void SetRecordItem(const std::vector<CRecordItem> & vec_rditem, const std::vector<Gdiplus::Color> & vec_crItem, const std::vector<Gdiplus::REAL> & vec_fWidth,BOOL nRepaint = TRUE);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


