#pragma once
class HighResolutionTimeCounter
{
private:
	static LARGE_INTEGER m_freq;
	static LARGE_INTEGER m_timeStart;
	static HighResolutionTimeCounter m_hightime;
	HighResolutionTimeCounter();
public:
	virtual ~HighResolutionTimeCounter();
	static LONGLONG NowOfMicrosecond();
	static LONGLONG NowOfMillisecond();
};

