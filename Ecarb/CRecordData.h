#pragma once
class CRecordData
{
private:
	int m_nID;
	double m_dTgtData;
	DWORD m_dwTgtTime;
	int m_nRecordItemID;
public:
	CRecordData();
	CRecordData(int nID, double dTgtData, DWORD dwTgtTime, int m_nRecordItemID);
	virtual ~CRecordData();
	int GetID() const;
	double GetTgtData() const;
	DWORD GetTgtTime() const;
	int GetRecordItemID() const;
};

