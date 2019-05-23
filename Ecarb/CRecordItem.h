#pragma once
#include "CRecordData.h"
#include <vector>
class CRecordItem
{
private:
	int m_nID;
	int m_nDataItemID;
	int m_nDataItemType;
	CString m_strDataItemName;
	CString m_strUnit;
	CString m_strRegStartAddr;
	int m_nRegNum;
	int m_nDecimalDigits;
	double m_dMultiplier;
	double m_dConstant;
	double m_dMaxData;
	double m_dMinData;
	int m_nRecordID;
	std::vector<CRecordData> m_vec_rddata;
public:
	CRecordItem();
	CRecordItem(int nID, int nDataItemID, int nDataItemType, const CString & strDataItemName, const CString & strUnit, const CString & strRegStartAddr, int nRegNum, int nDecimalDigits, double dMultiplier, double dConstant, double dMaxData, double dMinData, int nRecordID, const std::vector<CRecordData> & vec_rddata);
	virtual ~CRecordItem();
	int GetID() const;
	int GetDataItemID() const;
	int GetDataItemType() const;
	CString GetDataItemName() const;
	CString GetUnit() const;
	CString GetRegStartAddr() const;
	int GetRegNum() const;
	int GetDecimalDigits() const;
	double GetMultiplier() const;
	double GetConstant() const;
	double GetMaxData() const;
	double GetMinData() const;
	int GetRecordID() const;
	std::vector<CRecordData> & GetRecordData();
};

