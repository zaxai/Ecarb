#include "stdafx.h"
#include "HighResolutionTimeCounter.h"

LARGE_INTEGER HighResolutionTimeCounter::m_freq;
LARGE_INTEGER HighResolutionTimeCounter::m_timeStart;
HighResolutionTimeCounter HighResolutionTimeCounter::m_hightime;

HighResolutionTimeCounter::HighResolutionTimeCounter()
{
	QueryPerformanceFrequency(&m_freq);
	QueryPerformanceCounter(&m_timeStart);
}


HighResolutionTimeCounter::~HighResolutionTimeCounter()
{
}


LONGLONG HighResolutionTimeCounter::NowOfMicrosecond()
{
	LARGE_INTEGER timeEnd, timeSpan;
	QueryPerformanceCounter(&timeEnd);
	timeSpan.QuadPart = timeEnd.QuadPart - m_timeStart.QuadPart;
	return timeSpan.QuadPart/ m_freq.QuadPart;
}


LONGLONG HighResolutionTimeCounter::NowOfMillisecond()
{
	LARGE_INTEGER timeEnd, timeSpan;
	QueryPerformanceCounter(&timeEnd);
	timeSpan.QuadPart = timeEnd.QuadPart - m_timeStart.QuadPart;
	return timeSpan.QuadPart * 1000 / m_freq.QuadPart;
}