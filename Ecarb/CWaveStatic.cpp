// CWaveStatic.cpp: 实现文件
//

#include "stdafx.h"
#include "CWaveStatic.h"
#include "HighResolutionTimeCounter.h"

// CWaveStatic
const int nTimerComm = 0;
const int nTimerSpan = 100;

IMPLEMENT_DYNAMIC(CWaveStatic, CStatic)

CWaveStatic::CWaveStatic(const CDataItem * p_dataitem)
	: m_p_dataitem(p_dataitem)
	, m_dwTimeStart(0)
	, m_dwTimeCount(0)
{

}

CWaveStatic::~CWaveStatic()
{
}


BEGIN_MESSAGE_MAP(CWaveStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CWaveStatic 消息处理程序




void CWaveStatic::OnPaint()
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
	DrawWave(dcMem);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(p_bmpOld);
}


void CWaveStatic::DrawWave(CDC & dc)
{
	CRect rcBack;
	GetClientRect(rcBack);
	Gdiplus::Graphics graphics(dc);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//抗锯齿
	Gdiplus::SolidBrush brushBlack(Gdiplus::Color(0, 0, 0)), brushWhite(Gdiplus::Color(255, 255, 255));
	Gdiplus::Pen penBlack(Gdiplus::Color(0, 0, 0), 2.0f), penGray(Gdiplus::Color(128, 128, 128), 1.0f), penRed(Gdiplus::Color(255, 0, 0), 1.0f);
	Gdiplus::RectF rcBk((Gdiplus::REAL)rcBack.left, (Gdiplus::REAL)rcBack.top, (Gdiplus::REAL)rcBack.Width(), (Gdiplus::REAL)rcBack.Height());
	graphics.FillRectangle(&brushWhite, rcBk);//填充背景白色
	Gdiplus::REAL fOriginOffset = 50.0f;
	Gdiplus::PointF ptOrigin(rcBk.X + fOriginOffset, rcBk.Y + rcBk.Height - fOriginOffset);
	Gdiplus::PointF ptX(rcBk.X + rcBk.Width - fOriginOffset, rcBk.Y + rcBk.Height - fOriginOffset), ptY(rcBk.X + fOriginOffset, rcBk.Y + fOriginOffset);
	Gdiplus::PointF ptAxisX(rcBk.X + rcBk.Width - fOriginOffset / 2.0f, rcBk.Y + rcBk.Height - fOriginOffset), ptAxisY(rcBk.X + fOriginOffset, rcBk.Y + fOriginOffset / 2.0f);
	penBlack.SetEndCap(Gdiplus::LineCapArrowAnchor);
	graphics.DrawLine(&penBlack, ptOrigin, ptAxisX);//黑色X轴
	graphics.DrawLine(&penBlack, ptOrigin, ptAxisY);//黑色Y轴
	Gdiplus::FontFamily fontfamily(_T("Arial"));
	Gdiplus::Font font(&fontfamily, 8.0f);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	format.SetFormatFlags(format.GetFormatFlags() | Gdiplus::StringFormatFlagsNoWrap);
	Gdiplus::RectF rcName(rcBk.X, rcBk.Y, rcBk.Width, fOriginOffset);
	graphics.DrawString(m_p_dataitem->GetName()+_T("(")+ m_p_dataitem->GetUnit()+ _T(")"), -1, &font, rcName, &format, &brushBlack);//黑色名称+单位
	Gdiplus::RectF rcTime(rcBk.X, rcBk.Y+rcBk.Height- fOriginOffset, rcBk.Width, fOriginOffset);
	graphics.DrawString(_T("Time(s)"), -1, &font, rcTime, &format, &brushBlack);//黑色时间+单位
	const int nDivide = 10;
	Gdiplus::PointF sz_ptDivideStartY[nDivide + 1], sz_ptDivideEndY[nDivide + 1];
	penGray.SetDashStyle(Gdiplus::DashStyleDashDot);
	for (int i = 0; i < nDivide + 1; ++i)
	{
		sz_ptDivideStartY[i].X = ptOrigin.X;
		sz_ptDivideStartY[i].Y = Gdiplus::REAL(ptOrigin.Y - (double)i / (double)nDivide *(ptOrigin.Y - ptY.Y));
		sz_ptDivideEndY[i].X = ptX.X;
		sz_ptDivideEndY[i].Y = sz_ptDivideStartY[i].Y;
		if (sz_ptDivideStartY[i].Y < ptOrigin.Y&&sz_ptDivideStartY[i].Y > ptY.Y - 0.001f)
			graphics.DrawLine(&penGray, sz_ptDivideStartY[i], sz_ptDivideEndY[i]);//灰色等分Y轴
	}
	Gdiplus::REAL fDivideSizeY = Gdiplus::REAL((m_p_dataitem->GetMaxData() - m_p_dataitem->GetMinData()) / (double)nDivide);
	CString sz_strTextY[nDivide + 1];
	Gdiplus::RectF sz_rcTextY[nDivide + 1];
	for (int i = 0; i < nDivide + 1; ++i)
	{
		sz_rcTextY[i] = Gdiplus::RectF(sz_ptDivideStartY[i].X- fOriginOffset, sz_ptDivideStartY[i].Y-10.0f, fOriginOffset,20.0f);
		sz_strTextY[i] = ZUtil::GetDoubleString(m_p_dataitem->GetMinData() + i * fDivideSizeY, m_p_dataitem->GetDecimalDigits());
		graphics.DrawString(sz_strTextY[i], -1, &font, sz_rcTextY[i], &format, &brushBlack);//黑色等分Y值
	}
	const DWORD dwMaxSpan = 10000;
	Gdiplus::REAL fDivideSizeX = Gdiplus::REAL(double(dwMaxSpan) / double(nDivide));
	Gdiplus::PointF sz_ptDivideStartX[nDivide + 1], sz_ptDivideEndX[nDivide + 1];
	for (int i = 0; i < nDivide + 1; ++i)
	{
		if (m_dwTimeCount < dwMaxSpan)
			sz_ptDivideStartX[i].X = Gdiplus::REAL(ptOrigin.X +  i * fDivideSizeX / (double)dwMaxSpan*(ptX.X - ptOrigin.X));
		else
			sz_ptDivideStartX[i].X = Gdiplus::REAL(ptOrigin.X + (i * fDivideSizeX -m_dwTimeCount % (dwMaxSpan / nDivide)) / (double)dwMaxSpan*(ptX.X - ptOrigin.X));
		sz_ptDivideStartX[i].Y = ptOrigin.Y;
		sz_ptDivideEndX[i].X = sz_ptDivideStartX[i].X;
		sz_ptDivideEndX[i].Y = ptY.Y;
		if (sz_ptDivideStartX[i].X > ptOrigin.X&&sz_ptDivideStartX[i].X < ptX.X + 0.001f)
			graphics.DrawLine(&penGray, sz_ptDivideStartX[i], sz_ptDivideEndX[i]);//灰色等分X轴
	}
	CString sz_strTextX[nDivide + 1];
	Gdiplus::RectF sz_rcTextX[nDivide + 1];
	for (int i = 0; i < nDivide + 1; ++i)
	{
		sz_rcTextX[i] = Gdiplus::RectF(sz_ptDivideStartX[i].X - fOriginOffset/2.0f, sz_ptDivideStartX[i].Y, fOriginOffset, 20.0f);
		if (m_dwTimeCount < dwMaxSpan)
			sz_strTextX[i].Format(_T("%d"), i);
		else
			sz_strTextX[i].Format(_T("%d"), (m_dwTimeCount - dwMaxSpan) / (dwMaxSpan / nDivide) + i);
		if (sz_ptDivideStartX[i].X > ptOrigin.X - 0.001f&&sz_ptDivideStartX[i].X < ptX.X + 0.001f)
			graphics.DrawString(sz_strTextX[i], -1, &font, sz_rcTextX[i], &format, &brushBlack);//黑色等分X值
	}
	while (m_deque_dwTgtTime.size() && m_dwTimeCount > dwMaxSpan)
	{
		if (m_deque_dwTgtTime.size() > 1 && m_deque_dwTgtTime[1] <= m_dwTimeCount - dwMaxSpan)
		{
			m_deque_dTgtData.pop_front();
			m_deque_dwTgtTime.pop_front();
		}
		else if (m_deque_dwTgtTime.size() == 1 && m_deque_dwTgtTime[0] < m_dwTimeCount - dwMaxSpan)
		{
			m_deque_dTgtData.pop_front();
			m_deque_dwTgtTime.pop_front();
		}
		else
			break;
	}
	int nSize = m_deque_dTgtData.size();
	if (nSize > 1)
	{
		Gdiplus::PointF * p_ptData = new Gdiplus::PointF[nSize];
		for (int i = 0; i < nSize; ++i)
		{
			if (m_dwTimeCount < dwMaxSpan)
				p_ptData[i].X = Gdiplus::REAL(ptOrigin.X + double(m_deque_dwTgtTime[i]) / (double)dwMaxSpan*(ptX.X - ptOrigin.X));
			else
				p_ptData[i].X = Gdiplus::REAL(ptOrigin.X + (double(m_deque_dwTgtTime[i]) - double(m_dwTimeCount - dwMaxSpan)) / (double)dwMaxSpan*(ptX.X - ptOrigin.X));
			p_ptData[i].Y = Gdiplus::REAL(ptOrigin.Y - (m_deque_dTgtData[i] - m_p_dataitem->GetMinData()) / (m_p_dataitem->GetMaxData() - m_p_dataitem->GetMinData())*(ptOrigin.Y - ptY.Y));
		}
		if (p_ptData[0].X < ptOrigin.X)
		{
			p_ptData[0].Y = p_ptData[1].Y - (p_ptData[1].X - ptOrigin.X) / (p_ptData[1].X - p_ptData[0].X)*(p_ptData[1].Y - p_ptData[0].Y);
			p_ptData[0].X = ptOrigin.X;
		}
		graphics.DrawLines(&penRed, p_ptData, nSize);//红色实时曲线
		delete[] p_ptData;
	}
}


void CWaveStatic::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case nTimerComm:
	{
		m_dwTimeCount = DWORD(HighResolutionTimeCounter::NowOfMillisecond()) - m_dwTimeStart;
		Invalidate();
	}
	break;
	}
	CStatic::OnTimer(nIDEvent);
}


void CWaveStatic::StartComm(DWORD dwTimeStart)
{
	m_dwTimeStart = dwTimeStart;
	m_dwTimeCount = 0;
	m_deque_dTgtData.clear();
	m_deque_dwTgtTime.clear();
	SetTimer(nTimerComm, nTimerSpan, nullptr);
}


void CWaveStatic::StopComm()
{
	KillTimer(nTimerComm);
}


void CWaveStatic::AddTgtData(double dTgtData, DWORD dwTgtTime, BOOL nRepaint)
{
	m_deque_dTgtData.push_back(dTgtData);
	m_deque_dwTgtTime.push_back(dwTgtTime);
	m_dwTimeCount =DWORD( HighResolutionTimeCounter::NowOfMillisecond()) - m_dwTimeStart;
	if(nRepaint)
		Invalidate();
}