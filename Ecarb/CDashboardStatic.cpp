// CDashboardStatic.cpp: 实现文件
//

#include "stdafx.h"
#include "CDashboardStatic.h"


// CDashboardStatic

IMPLEMENT_DYNAMIC(CDashboardStatic, CStatic)

CDashboardStatic::CDashboardStatic(const CDataItem * p_dataitem)
	: m_p_dataitem(p_dataitem)
{

}

CDashboardStatic::~CDashboardStatic()
{
}


BEGIN_MESSAGE_MAP(CDashboardStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CDashboardStatic 消息处理程序




void CDashboardStatic::OnPaint()
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
	DrawDashboard(dcMem);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(p_bmpOld);
}


void CDashboardStatic::DrawDashboard(CDC & dc)
{
	CRect rcBack,rcCircle;
	GetClientRect(rcBack);
	rcCircle = rcBack;
	rcCircle.DeflateRect(10,10);
	Gdiplus::Graphics graphics(dc);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//抗锯齿
	Gdiplus::SolidBrush brushBlack(Gdiplus::Color(0, 0, 0)), brushGreen(Gdiplus::Color(0, 255, 0)), brushRed(Gdiplus::Color(255, 0, 0));
	Gdiplus::SolidBrush brushWhite(Gdiplus::Color(255, 255, 255)), brushYellow(Gdiplus::Color(255, 255, 0));
	Gdiplus::Pen penGreen(Gdiplus::Color(0, 255, 0),2.0f);
	Gdiplus::RectF rcBk((Gdiplus::REAL)rcBack.left, (Gdiplus::REAL)rcBack.top, (Gdiplus::REAL)rcBack.Width(), (Gdiplus::REAL)rcBack.Height());
	Gdiplus::RectF rcCir((Gdiplus::REAL)rcCircle.left, (Gdiplus::REAL)rcCircle.top, (Gdiplus::REAL)rcCircle.Width(), (Gdiplus::REAL)rcCircle.Height());
	graphics.FillRectangle(&brushBlack,rcBk);//填充背景黑色
	graphics.DrawEllipse(&penGreen, rcCir);//绿色圆形
	Gdiplus::PointF ptCenter(rcCir.X+ rcCir.Width/2.0f, rcCir.Y+ rcCir.Height/2.0f);
	Gdiplus::REAL fRadiusStart, fRadiusStartHalf, fRadiusEnd,fRadiusText;
	fRadiusEnd = rcCir.Width/ 2.0f;
	fRadiusStart = fRadiusEnd - 10.0f;
	fRadiusStartHalf = fRadiusEnd - 5.0f;
	fRadiusText = fRadiusEnd - 25.0f;
	const int nDivide = 8;
	Gdiplus::REAL sz_fAngle[nDivide + 1] = { 210.0f,180.0f,150.0f,120.0f,90.0f,60.0f,30.0f,0.0f,-30.0f };
	Gdiplus::REAL sz_fAngleHalf[nDivide] = { 195.0f,165.0f,135.0f,105.0f,75.0f,45.0f,15.0f,-15.0f };
	Gdiplus::PointF sz_ptStart[nDivide + 1], sz_ptEnd[nDivide + 1], sz_ptStartHalf[nDivide], sz_ptEndHalf[nDivide], sz_ptText[nDivide + 1];
	for (int i = 0; i < nDivide + 1; ++i)
	{
		sz_ptStart[i] = GetPointOnCircle(sz_fAngle[i], ptCenter, fRadiusStart);
		sz_ptEnd[i] = GetPointOnCircle(sz_fAngle[i], ptCenter, fRadiusEnd);
		graphics.DrawLine(&penGreen, sz_ptStart[i], sz_ptEnd[i]);//绿色等分线
	}
	for (int i = 0; i < nDivide; ++i)
	{
		sz_ptStartHalf[i] = GetPointOnCircle(sz_fAngleHalf[i], ptCenter, fRadiusStartHalf);
		sz_ptEndHalf[i] = GetPointOnCircle(sz_fAngleHalf[i], ptCenter, fRadiusEnd);
		graphics.DrawLine(&penGreen, sz_ptStartHalf[i], sz_ptEndHalf[i]);//绿色半等分线
	}
	Gdiplus::REAL fDivideSize = Gdiplus::REAL((m_p_dataitem->GetMaxData() - m_p_dataitem->GetMinData()) / double(nDivide));
	CString sz_strText[nDivide + 1];
	Gdiplus::RectF sz_rcText[nDivide + 1];
	Gdiplus::FontFamily fontfamily(_T("Arial"));
	Gdiplus::Font font(&fontfamily,8.0f);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	format.SetFormatFlags(format.GetFormatFlags() | Gdiplus::StringFormatFlagsNoWrap);
	for (int i = 0; i < nDivide + 1; ++i)
	{
		sz_ptText[i] = GetPointOnCircle(sz_fAngle[i], ptCenter, fRadiusText);
		sz_rcText[i] = Gdiplus::RectF(sz_ptText[i].X - 25.0f, sz_ptText[i].Y - 10.0f, 50.0f, 20.0f);
		sz_strText[i] = ZUtil::GetDoubleString(m_p_dataitem->GetMinData() + i * fDivideSize, m_p_dataitem->GetDecimalDigits());
		graphics.DrawString(sz_strText[i],-1,&font, sz_rcText[i],&format,&brushWhite);//白色等分值
	}
	Gdiplus::RectF rcName(ptCenter.X - rcCir.Width / 2.0f, ptCenter.Y - 30.0f, rcCir.Width, 20.0f);
	graphics.DrawString(m_p_dataitem->GetName(), -1, &font, rcName, &format, &brushGreen);//绿色名称
	Gdiplus::RectF rcValue(ptCenter.X - rcCir.Width / 2.0f, ptCenter.Y + 15.0f, rcCir.Width, 25.0f);
	Gdiplus::Font fontValue(&fontfamily, 10.0f);
	CString strValue = ZUtil::GetDoubleString(m_p_dataitem->GetTgtData(), m_p_dataitem->GetDecimalDigits()) +_T(" ")+ m_p_dataitem->GetUnit();
	graphics.DrawString(strValue, -1, &fontValue, rcValue, &format, &brushYellow);//黄色实时值
	Gdiplus::PointF ptPointer,sz_ptSide[2];
	Gdiplus::REAL fTgtData,fAnglePointer,sz_fAngleSide[2], fRadiusSide = 3.0f;
	fTgtData = Gdiplus::REAL(m_p_dataitem->GetTgtData() > m_p_dataitem->GetMaxData() ? m_p_dataitem->GetMaxData() : m_p_dataitem->GetTgtData());
	fTgtData = Gdiplus::REAL(fTgtData < m_p_dataitem->GetMinData() ? m_p_dataitem->GetMinData() : fTgtData);
	fAnglePointer = Gdiplus::REAL(210.0- (fTgtData - m_p_dataitem->GetMinData()) *240.0 / (m_p_dataitem->GetMaxData() - m_p_dataitem->GetMinData()));
	sz_fAngleSide[0] = fAnglePointer - 90.0f;
	sz_fAngleSide[1] = fAnglePointer + 90.0f;
	ptPointer = GetPointOnCircle(fAnglePointer, ptCenter, fRadiusStart);
	sz_ptSide[0] = GetPointOnCircle(sz_fAngleSide[0], ptCenter, fRadiusSide);
	sz_ptSide[1] = GetPointOnCircle(sz_fAngleSide[1], ptCenter, fRadiusSide);
	Gdiplus::PointF sz_ptTriangle[3] = { ptPointer ,sz_ptSide[0] ,sz_ptSide[1] };
	graphics.FillPolygon(&brushRed, sz_ptTriangle,3);//红色指针三角形区域
	Gdiplus::RectF rcSemiCircle(ptCenter.X - fRadiusSide, ptCenter.Y - fRadiusSide, 2.0f*fRadiusSide, 2.0f*fRadiusSide);
	graphics.FillPie(&brushRed, rcSemiCircle, 360.f-(Gdiplus::REAL)sz_fAngleSide[0],180.0f);//红色指针半圆区域
	Gdiplus::REAL fRadiusCenter = 1.5f;
	Gdiplus::RectF rcCenterCircle(ptCenter.X - fRadiusCenter, ptCenter.Y - fRadiusCenter, 2.0f*fRadiusCenter, 2.0f*fRadiusCenter);
	graphics.FillEllipse(&brushBlack, rcCenterCircle);//指针黑色固定轴
}


Gdiplus::PointF CDashboardStatic::GetPointOnCircle(Gdiplus::REAL fAngle, Gdiplus::PointF ptCenter, Gdiplus::REAL fRadius)
{
	Gdiplus::PointF pt;
	pt.Y = Gdiplus::REAL(ptCenter.Y - fRadius * sin(GetRadian(fAngle)));
	pt.X = Gdiplus::REAL(ptCenter.X + fRadius * cos(GetRadian(fAngle)));
	return pt;
}


Gdiplus::REAL CDashboardStatic::GetRadian(Gdiplus::REAL fAngle)
{
	const Gdiplus::REAL fPI = Gdiplus::REAL(atan(1.0)*4.0);
	return fPI * fAngle / 180.0f;
}