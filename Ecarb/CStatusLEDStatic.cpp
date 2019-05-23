// CStatusLEDStatic.cpp: 实现文件
//

#include "stdafx.h"
#include "CStatusLEDStatic.h"


// CStatusLEDStatic

IMPLEMENT_DYNAMIC(CStatusLEDStatic, CStatic)

CStatusLEDStatic::CStatusLEDStatic(const CDataItem * p_dataitem)
	: m_p_dataitem(p_dataitem)
{

}

CStatusLEDStatic::~CStatusLEDStatic()
{
}


BEGIN_MESSAGE_MAP(CStatusLEDStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CStatusLEDStatic 消息处理程序




void CStatusLEDStatic::OnPaint()
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
	DrawStatusLED(dcMem);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(p_bmpOld);
}


void CStatusLEDStatic::DrawStatusLED(CDC & dc)
{
	CRect rcBack, rcCircle;
	GetClientRect(rcBack);
	rcCircle = rcBack;
	rcCircle.DeflateRect(50, 50);
	Gdiplus::Graphics graphics(dc);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//抗锯齿
	Gdiplus::SolidBrush brushBlack(Gdiplus::Color(0, 0, 0)), brushWhite(Gdiplus::Color(255, 255, 255));
	Gdiplus::RectF rcBk((Gdiplus::REAL)rcBack.left, (Gdiplus::REAL)rcBack.top, (Gdiplus::REAL)rcBack.Width(), (Gdiplus::REAL)rcBack.Height());
	Gdiplus::RectF rcCir((Gdiplus::REAL)rcCircle.left, (Gdiplus::REAL)rcCircle.top, (Gdiplus::REAL)rcCircle.Width(), (Gdiplus::REAL)rcCircle.Height());
	graphics.FillRectangle(&brushBlack, rcBk);//填充背景黑色
	Gdiplus::PointF ptCenter(rcCir.X + rcCir.Width / 2.0f, rcCir.Y + rcCir.Height / 2.0f);
	Gdiplus::FontFamily fontfamily(_T("Arial"));
	Gdiplus::Font font(&fontfamily, 8.0f);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	format.SetFormatFlags(format.GetFormatFlags() | Gdiplus::StringFormatFlagsNoWrap);
	Gdiplus::RectF rcName(rcBk.X, rcBk.Y, rcBk.X + rcBk.Width / 2.0f, 50.0f);
	graphics.DrawString(m_p_dataitem->GetName(), -1, &font, rcName, &format, &brushWhite);//白色名称
	Gdiplus::RectF rcValue(rcBk.X + rcBk.Width / 2.0f, rcBk.Y, rcBk.Width / 2.0f, 50.0f);
	Gdiplus::Font fontValue(&fontfamily, 10.0f);
	CString strValue = ZUtil::GetDoubleString(m_p_dataitem->GetTgtData(), m_p_dataitem->GetDecimalDigits()) + _T(" ") + m_p_dataitem->GetUnit();
	graphics.DrawString(strValue, -1, &fontValue, rcValue, &format, &brushWhite);//白色实时值
	Gdiplus::Color cr;
	if (m_p_dataitem->GetTgtData() < m_p_dataitem->GetMinData())
		cr.SetFromCOLORREF(RGB(128, 128, 128));
	else if(m_p_dataitem->GetTgtData() > m_p_dataitem->GetMaxData())
		cr.SetFromCOLORREF(RGB(255, 0, 0));
	else
		cr.SetFromCOLORREF(RGB(0, 0, 255));
	Gdiplus::SolidBrush brushLED(cr);
	graphics.FillEllipse(&brushLED, rcCir);//状态灯
}