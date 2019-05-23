#pragma once
class CDataItem
{
public:
	enum ITEMTYPE
	{
		TYPE_CALIBRATE = 0,
		TYPE_MONITOR
	};
private:
	int m_nID;
	int m_nItemType;
	CString m_strName;
	CString m_strUnit;
	CString m_strRegStartAddr;
	int m_nRegNum;
	int m_nDecimalDigits;
	double m_dMultiplier;
	double m_dConstant;
	double m_dTgtData;
	double m_dMaxData;
	double m_dMinData;
public:
	CDataItem();
	CDataItem(int nID, int nItemType, const CString & strName, const CString & strUnit, const CString & strRegStartAddr, int nRegNum, int nDecimalDigits,double dMultiplier,double dConstant, double dTgtData, double dMaxData, double dMinData);
	virtual ~CDataItem();
	bool operator==(const CDataItem & dataitem);
	int GetID() const;
	int GetItemType() const;
	CString GetName() const;
	CString GetUnit() const;
	CString GetRegStartAddr() const;
	int GetRegNum() const;
	int GetDecimalDigits() const;
	double GetMultiplier() const;
	double GetConstant() const;
	double GetTgtData() const;
	double GetMaxData() const;
	double GetMinData() const;
	void SetID(int nID);
	void SetItemType(int nItemType);
	void SetName(const CString & strName);
	void SetUnit(const CString & strUnit);
	void SetRegStartAddr(const CString & strRegStartAddr);
	void SetRegNum(int nRegNum);
	void SetDecimalDigits(int nDecimalDigits);
	void SetMultiplier(double dMultiplier);
	void SetConstant(double dConstant);
	void SetTgtData(double dTgtData);
	void SetMaxData(double dMaxData);
	void SetMinData(double dMinData);
};

