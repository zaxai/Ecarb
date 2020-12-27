// CRecordStatic.cpp: 实现文件
//

#include "stdafx.h"
#include "Ecarb.h"
#include "CRecordStatic.h"


// CRecordStatic

IMPLEMENT_DYNAMIC(CRecordStatic, CStatic)

CRecordStatic::CRecordStatic()
	: m_nSelItem(-1)
	, m_nClickRecordDataID(-1)
	, m_dwTimeOrigin(0)
	, m_dwMaxSpan(10000)
	, m_bIsDragging(false)
{

}

CRecordStatic::~CRecordStatic()
{
}


BEGIN_MESSAGE_MAP(CRecordStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CRecordStatic 消息处理程序




void CRecordStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CStatic::OnPaint()
	CRect rc;
	GetClientRect(rc);
	CDC dcMem;
	CBitmap bmp;
	dcMem.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	DrawRecord(dcMem);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(p_bmpOld);
}


void CRecordStatic::DrawRecord(CDC & dc)
{
	CRect rcBack;
	GetClientRect(rcBack);
	Gdiplus::Graphics graphics(dc);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//抗锯齿
	Gdiplus::SolidBrush brushBlack(Gdiplus::Color(0, 0, 0)), brushWhite(Gdiplus::Color(255, 255, 255));
	Gdiplus::Pen penBlack(Gdiplus::Color(0, 0, 0), 2.0f);
	Gdiplus::Pen penGrayDot(Gdiplus::Color(128, 128, 128), 1.0f);
	penGrayDot.SetDashStyle(Gdiplus::DashStyleDashDot);
	Gdiplus::RectF rcBk((Gdiplus::REAL)rcBack.left, (Gdiplus::REAL)rcBack.top, (Gdiplus::REAL)rcBack.Width(), (Gdiplus::REAL)rcBack.Height());
	graphics.FillRectangle(&brushWhite, rcBk);//填充背景白色
	Gdiplus::REAL fOriginOffset = 50.0f;
	m_ptOrigin= Gdiplus::PointF(rcBk.X + fOriginOffset, rcBk.Y + rcBk.Height - fOriginOffset);
	m_ptX = Gdiplus::PointF(rcBk.X + rcBk.Width - fOriginOffset, rcBk.Y + rcBk.Height - fOriginOffset);
	m_ptY = Gdiplus::PointF(rcBk.X + fOriginOffset, rcBk.Y + fOriginOffset);
	Gdiplus::PointF ptAxisX(rcBk.X + rcBk.Width - fOriginOffset / 2.0f, rcBk.Y + rcBk.Height - fOriginOffset), ptAxisY(rcBk.X + fOriginOffset, rcBk.Y + fOriginOffset / 2.0f);
	penBlack.SetEndCap(Gdiplus::LineCapArrowAnchor);
	graphics.DrawLine(&penBlack, m_ptOrigin, ptAxisX);//黑色X轴
	graphics.DrawLine(&penBlack, m_ptOrigin, ptAxisY);//黑色Y轴
	if (m_vec_rditem.size())
	{
		Gdiplus::FontFamily fontfamily(_T("Arial"));
		Gdiplus::Font font(&fontfamily, 8.0f);
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		format.SetFormatFlags(format.GetFormatFlags() | Gdiplus::StringFormatFlagsNoWrap);
		Gdiplus::RectF rcName(rcBk.X, rcBk.Y, rcBk.Width, fOriginOffset);
		graphics.DrawString(m_vec_rditem[m_nSelItem].GetDataItemName() + _T("(") + m_vec_rditem[m_nSelItem].GetUnit() + _T(")"), -1, &font, rcName, &format, &brushBlack);//黑色名称+单位
		Gdiplus::RectF rcTime(rcBk.X, rcBk.Y + rcBk.Height - fOriginOffset, rcBk.Width, fOriginOffset);
		graphics.DrawString(_T("Time(s)"), -1, &font, rcTime, &format, &brushBlack);//黑色时间+单位
		const int nDivide = 10;
		Gdiplus::PointF sz_ptDivideStartY[nDivide + 1], sz_ptDivideEndY[nDivide + 1];
		for (int i = 0; i < nDivide + 1; ++i)
		{
			sz_ptDivideStartY[i].X = m_ptOrigin.X;
			sz_ptDivideStartY[i].Y = Gdiplus::REAL(m_ptOrigin.Y - (double)i / (double)nDivide *(m_ptOrigin.Y - m_ptY.Y));
			sz_ptDivideEndY[i].X = m_ptX.X;
			sz_ptDivideEndY[i].Y = sz_ptDivideStartY[i].Y;
			if (sz_ptDivideStartY[i].Y < m_ptOrigin.Y&&sz_ptDivideStartY[i].Y > m_ptY.Y - 0.001f)
				graphics.DrawLine(&penGrayDot, sz_ptDivideStartY[i], sz_ptDivideEndY[i]);//灰色等分Y轴
		}
		Gdiplus::REAL fDivideSizeY = Gdiplus::REAL((m_vec_rditem[m_nSelItem].GetMaxData() - m_vec_rditem[m_nSelItem].GetMinData()) / (double)nDivide);
		CString sz_strTextY[nDivide + 1];
		Gdiplus::RectF sz_rcTextY[nDivide + 1];
		for (int i = 0; i < nDivide + 1; ++i)
		{
			sz_rcTextY[i] = Gdiplus::RectF(sz_ptDivideStartY[i].X - fOriginOffset, sz_ptDivideStartY[i].Y - 10.0f, fOriginOffset, 20.0f);
			sz_strTextY[i] = ZUtil::GetDoubleString(m_vec_rditem[m_nSelItem].GetMinData() + i * fDivideSizeY, m_vec_rditem[m_nSelItem].GetDecimalDigits());
			graphics.DrawString(sz_strTextY[i], -1, &font, sz_rcTextY[i], &format, &brushBlack);//黑色等分Y值
		}
		const DWORD dwSecond = 1000;
		Gdiplus::REAL fDivideSizeX = Gdiplus::REAL(double(m_dwMaxSpan) / double(nDivide));
		Gdiplus::PointF sz_ptDivideStartX[nDivide + 1], sz_ptDivideEndX[nDivide + 1];
		for (int i = 0; i < nDivide + 1; ++i)
		{
			sz_ptDivideStartX[i].X = Gdiplus::REAL(m_ptOrigin.X + (i * fDivideSizeX - m_dwTimeOrigin % (m_dwMaxSpan / nDivide)) / (double)m_dwMaxSpan*(m_ptX.X - m_ptOrigin.X));
			sz_ptDivideStartX[i].Y = m_ptOrigin.Y;
			sz_ptDivideEndX[i].X = sz_ptDivideStartX[i].X;
			sz_ptDivideEndX[i].Y = m_ptY.Y;
			if (sz_ptDivideStartX[i].X > m_ptOrigin.X&&sz_ptDivideStartX[i].X < m_ptX.X + 0.001f)
				graphics.DrawLine(&penGrayDot, sz_ptDivideStartX[i], sz_ptDivideEndX[i]);//灰色等分X轴
		}
		CString sz_strTextX[nDivide + 1];
		Gdiplus::RectF sz_rcTextX[nDivide + 1];
		for (int i = 0; i < nDivide + 1; ++i)
		{
			sz_rcTextX[i] = Gdiplus::RectF(sz_ptDivideStartX[i].X - fOriginOffset / 2.0f, sz_ptDivideStartX[i].Y, fOriginOffset, 20.0f);
			sz_strTextX[i].Format(_T("%.03f"), double(m_dwTimeOrigin / (m_dwMaxSpan / nDivide)+i)* m_dwMaxSpan/nDivide/ (double)dwSecond);
			if (sz_ptDivideStartX[i].X > m_ptOrigin.X - 0.001f&&sz_ptDivideStartX[i].X < m_ptX.X + 0.001f)
				graphics.DrawString(sz_strTextX[i], -1, &font, sz_rcTextX[i], &format, &brushBlack);//黑色等分X值
		}
		m_vec2_rcDataRange.clear();
		m_vec_deque_rddata.clear();
		m_vec2_rcDataRange.resize(m_vec_rditem.size());
		m_vec_deque_rddata.resize(m_vec_rditem.size());
		auto it_crItem = m_vec_crItem.begin();
		auto it_fWidth = m_vec_fWidth.begin();
		auto it_rcDataRange = m_vec2_rcDataRange.begin();
		auto it_deque_rddata = m_vec_deque_rddata.begin();
		for (CRecordItem & rditem: m_vec_rditem)
		{
			std::vector<CRecordData> & vec_rddata = rditem.GetRecordData();
			std::deque<CRecordData> & deque_rddata = *it_deque_rddata;
			for (CRecordData & rddata : vec_rddata)
			{
				if (rddata.GetTgtTime() > m_dwTimeOrigin&&rddata.GetTgtTime() < m_dwTimeOrigin + m_dwMaxSpan)
					deque_rddata.push_back(rddata);
			}
			int nSize = vec_rddata.size();
			if (deque_rddata.empty())
			{
				if (!vec_rddata.empty() && vec_rddata.front().GetTgtTime() < m_dwTimeOrigin&&vec_rddata.back().GetTgtTime() > m_dwTimeOrigin + m_dwMaxSpan)
				{
					for (int i = 0; i < nSize; ++i)
					{
						if (vec_rddata[i].GetTgtTime() > m_dwTimeOrigin + m_dwMaxSpan)
						{
							deque_rddata.push_back(vec_rddata[i - 1]);
							deque_rddata.push_back(vec_rddata[i]);
							break;
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < nSize; ++i)
				{
					if (vec_rddata[i].GetTgtTime() == deque_rddata.front().GetTgtTime())
					{
						if (i > 0)
							deque_rddata.push_front(vec_rddata[i - 1]);
					}
					if (vec_rddata[i].GetTgtTime() == deque_rddata.back().GetTgtTime())
					{
						if (i + 1 < nSize)
							deque_rddata.push_back(vec_rddata[i + 1]);
						break;
					}
				}
			}
			nSize = deque_rddata.size();
			if (nSize > 0)
			{
				Gdiplus::PointF * p_ptData = new Gdiplus::PointF[nSize];
				Gdiplus::RectF * p_rcDataDot = new Gdiplus::RectF[nSize];
				Gdiplus::RectF rcDataRange;
				Gdiplus::REAL fDotRadius = *it_fWidth, fRangeRadius = *it_fWidth+3.0f;
				CString strTextMouseOnDot, strTextMouseClick;
				Gdiplus::PointF ptTextMouseOnDot, ptTextMouseClick;
				bool bMouseOnDot = false, bMouseClick = false;
				for (int i = 0; i < nSize; ++i)
				{
					p_ptData[i].X = Gdiplus::REAL(m_ptOrigin.X + (double(deque_rddata[i].GetTgtTime()) - (double)m_dwTimeOrigin) / (double)m_dwMaxSpan*(m_ptX.X - m_ptOrigin.X));
					p_ptData[i].Y = Gdiplus::REAL(m_ptOrigin.Y - (deque_rddata[i].GetTgtData() - rditem.GetMinData()) / (rditem.GetMaxData() - rditem.GetMinData())*(m_ptOrigin.Y - m_ptY.Y));
					p_rcDataDot[i] = Gdiplus::RectF(p_ptData[i].X- fDotRadius, p_ptData[i].Y- fDotRadius,2.0f*fDotRadius,2.0f*fDotRadius);
					rcDataRange = Gdiplus::RectF(p_ptData[i].X - fRangeRadius, p_ptData[i].Y - fRangeRadius, 2.0f*fRangeRadius, 2.0f*fRangeRadius);
					it_rcDataRange->push_back(rcDataRange);
					if (rcDataRange.Contains(m_ptMouseMove))
					{
						if (p_ptData[i].X >= m_ptOrigin.X&&p_ptData[i].X <= m_ptX.X)
						{
							Gdiplus::Pen penCross(*it_crItem, 1.0f);
							penCross.SetDashStyle(Gdiplus::DashStyleDashDot);
							graphics.DrawLine(&penCross, Gdiplus::PointF(p_ptData[i].X, m_ptOrigin.Y), Gdiplus::PointF(p_ptData[i].X, m_ptY.Y));//十字竖线
							graphics.DrawLine(&penCross, Gdiplus::PointF(m_ptOrigin.X, p_ptData[i].Y), Gdiplus::PointF(m_ptX.X, p_ptData[i].Y));//十字横线
							CString strTgtData = ZUtil::GetDoubleString(deque_rddata[i].GetTgtData(), rditem.GetDecimalDigits());
							strTextMouseOnDot.Format(_T("(%.03f,%s)"), double(deque_rddata[i].GetTgtTime()) / (double)dwSecond, strTgtData);
							ptTextMouseOnDot = Gdiplus::PointF(p_ptData[i].X + 10.0f, p_ptData[i].Y - 20.0f);
							bMouseOnDot = true;
						}
					}
					if (m_nClickRecordDataID == deque_rddata[i].GetID())
					{
						if (p_ptData[i].X >= m_ptOrigin.X&&p_ptData[i].X <= m_ptX.X)
						{
							Gdiplus::Pen penCross(*it_crItem, 1.0f);
							penCross.SetDashStyle(Gdiplus::DashStyleDashDot);
							graphics.DrawLine(&penCross, Gdiplus::PointF(p_ptData[i].X, m_ptOrigin.Y), Gdiplus::PointF(p_ptData[i].X, m_ptY.Y));//十字竖线
							graphics.DrawLine(&penCross, Gdiplus::PointF(m_ptOrigin.X, p_ptData[i].Y), Gdiplus::PointF(m_ptX.X, p_ptData[i].Y));//十字横线
							CString strTgtData = ZUtil::GetDoubleString(deque_rddata[i].GetTgtData(), rditem.GetDecimalDigits());
							strTextMouseClick.Format(_T("(%.03f,%s)"), double(deque_rddata[i].GetTgtTime()) / (double)dwSecond, strTgtData);
							ptTextMouseClick = Gdiplus::PointF(p_ptData[i].X + 10.0f, p_ptData[i].Y - 20.0f);
							bMouseClick = true;
						}
					}
				}
				int nIndexStart = 0, nIndexEnd = nSize;
				if (nSize > 1 && p_ptData[0].X < m_ptOrigin.X)
				{
					p_ptData[0].Y = p_ptData[1].Y - (p_ptData[1].X - m_ptOrigin.X) / (p_ptData[1].X - p_ptData[0].X)*(p_ptData[1].Y - p_ptData[0].Y);
					p_ptData[0].X = m_ptOrigin.X;
					++nIndexStart;
				}
				if (nSize > 1 && p_ptData[nSize - 1].X > m_ptX.X)
				{
					p_ptData[nSize - 1].Y = p_ptData[nSize - 2].Y - (p_ptData[nSize - 2].X - m_ptX.X) / (p_ptData[nSize - 2].X - p_ptData[nSize - 1].X)*(p_ptData[nSize - 2].Y - p_ptData[nSize - 1].Y);
					p_ptData[nSize - 1].X = m_ptX.X;
					--nIndexEnd;
				}
				Gdiplus::Pen penWave(*it_crItem, *it_fWidth);
				graphics.DrawLines(&penWave, p_ptData, nSize);//实时曲线
				for (int i = nIndexStart; i < nIndexEnd; ++i)
					graphics.FillEllipse(&brushBlack, p_rcDataDot[i]);//黑色采样点
				if(bMouseOnDot)
					graphics.DrawString(strTextMouseOnDot, -1, &font, ptTextMouseOnDot, &brushBlack);//黑色值
				if (bMouseClick)
					graphics.DrawString(strTextMouseClick, -1, &font, ptTextMouseClick, &brushBlack);//黑色值
				delete[] p_ptData;
				delete[] p_rcDataDot;
			}
			++it_crItem;
			++it_fWidth;
			++it_rcDataRange;
			++it_deque_rddata;
		}
	}
}


void CRecordStatic::SetRecordItem(const std::vector<CRecordItem> & vec_rditem, const std::vector<Gdiplus::Color> & vec_crItem, const std::vector<Gdiplus::REAL> & vec_fWidth, BOOL nRepaint)
{
	m_vec_rditem = vec_rditem;
	m_vec_crItem = vec_crItem;
	m_vec_fWidth = vec_fWidth;
	if (m_vec_rditem.size())
		m_nSelItem = 0;
	if (nRepaint)
		Invalidate();
}


void CRecordStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetFocus();
	Gdiplus::PointF ptMouseClick = Gdiplus::PointF((Gdiplus::REAL)point.x, (Gdiplus::REAL)point.y);
	bool bMatch = false;
	int i = 0, j = 0;
	for (std::vector<Gdiplus::RectF> & vec_rc : m_vec2_rcDataRange)
	{
		j = 0;
		for (Gdiplus::RectF & rc : vec_rc)
		{
			if (rc.Contains(ptMouseClick))
			{
				m_nClickRecordDataID = m_vec_deque_rddata[i][j].GetID();
				bMatch = true;
				break;
			}
			++j;
		}
		if (bMatch)
			break;
		++i;
	}
	Invalidate();
	CStatic::OnLButtonDown(nFlags, point);
}


void CRecordStatic::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_ptDragStart = point;
	m_bIsDragging = true;
	CStatic::OnRButtonDown(nFlags, point);
}


void CRecordStatic::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_ptDragEnd = point;
	SetOriginTimeByDrag();
	m_bIsDragging = false;
	CStatic::OnRButtonUp(nFlags, point);
}


void CRecordStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tm;
	tm.cbSize = sizeof(TRACKMOUSEEVENT);
	tm.dwFlags = TME_LEAVE;
	tm.dwHoverTime = 500;
	tm.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tm);
	if (m_bIsDragging)
	{
		m_ptDragEnd = point;
		SetOriginTimeByDrag();
		m_ptDragStart = point;
	}
	m_ptMouseMove = Gdiplus::PointF((Gdiplus::REAL)point.x, (Gdiplus::REAL)point.y);
	bool bMatch = false;
	int i = 0;
	for (std::vector<Gdiplus::RectF> & vec_rc : m_vec2_rcDataRange)
	{
		for (Gdiplus::RectF & rc : vec_rc)
		{
			if (rc.Contains(m_ptMouseMove))
			{
				m_nSelItem = i;
				bMatch = true;
				break;
			}
		}
		if (bMatch)
			break;
		++i;
	}
	Invalidate();
	CStatic::OnMouseMove(nFlags, point);
}


void CRecordStatic::SetOriginTimeByDrag()
{
	long l = long((double)m_dwTimeOrigin- double(m_ptDragEnd.x - m_ptDragStart.x)*(double)m_dwMaxSpan / (m_ptX.X - m_ptOrigin.X));
	m_dwTimeOrigin = l < 0 ? 0 : (DWORD)l;
	Invalidate();
}


void CRecordStatic::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bIsDragging)
		m_bIsDragging = false;
	CStatic::OnMouseLeave();
}


BOOL CRecordStatic::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint ptCur(pt);
	ScreenToClient(&ptCur);
	DWORD dwTime = GetPointTime(ptCur);
	if (zDelta > 0)
	{
		if (m_dwMaxSpan > 1250)
		{
			m_dwMaxSpan /= 2;
			SetOriginTimeByPointTime(ptCur, dwTime);
			Invalidate();
		}
	}
	else
	{
		if (m_dwMaxSpan < 160000)
		{
			m_dwMaxSpan *= 2;
			SetOriginTimeByPointTime(ptCur, dwTime);
			Invalidate();
		}
	}
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}


DWORD CRecordStatic::GetPointTime(CPoint pt)
{
	return m_dwTimeOrigin + DWORD(((Gdiplus::REAL)pt.x - m_ptOrigin.X) / (m_ptX.X - m_ptOrigin.X)*m_dwMaxSpan);
}


void CRecordStatic::SetOriginTimeByPointTime(CPoint pt,DWORD dwTime)
{
	long l=long((double)dwTime - ((Gdiplus::REAL)pt.x - m_ptOrigin.X) / (m_ptX.X - m_ptOrigin.X)*m_dwMaxSpan);
	m_dwTimeOrigin = l < 0 ? 0 : (DWORD)l;
}


BOOL CRecordStatic::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		UINT nKey = pMsg->wParam;
		if (nKey == VK_LEFT || nKey == VK_RIGHT || nKey == VK_UP || nKey == VK_DOWN)
		{
			switch (nKey)
			{
			case VK_LEFT:
			case VK_RIGHT:
			{
				const int nDivide = 10;
				long l;
				if(nKey== VK_LEFT)
					l = long((double)m_dwTimeOrigin - (double)m_dwMaxSpan/ (double)nDivide);
				else
					l = long((double)m_dwTimeOrigin + (double)m_dwMaxSpan / (double)nDivide);
				m_dwTimeOrigin = l < 0 ? 0 : (DWORD)l;
				Invalidate();
			}
			break;
			case VK_UP:
			case VK_DOWN:
			{
				CPoint ptCur;
				::GetCursorPos(&ptCur);
				ScreenToClient(&ptCur);
				DWORD dwTime = GetPointTime(ptCur);
				if (nKey == VK_UP)
				{
					if (m_dwMaxSpan > 1250)
					{
						m_dwMaxSpan /= 2;
						SetOriginTimeByPointTime(ptCur, dwTime);
						Invalidate();
					}
				}
				else
				{
					if (m_dwMaxSpan < 160000)
					{
						m_dwMaxSpan *= 2;
						SetOriginTimeByPointTime(ptCur, dwTime);
						Invalidate();
					}
				}
			}
			break;
			}
			return TRUE;
		}
	}
	return CStatic::PreTranslateMessage(pMsg);
}
