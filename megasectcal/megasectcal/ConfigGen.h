#pragma once

#include "stdafx.h"
#include "./json/json.h"
#include "XmlInfo/InfoItem.h"
 

struct ConfigDrawT
{
	CString filename;
	double bottom,left,right,top;
	CString caption;
};

class PrintPageSize
{
public:
	enum 
	{
		None = -1,
		A0 = 0, 
		A1 = 1,
		A2 = 2,
		A3 = 3,
		A4 = 4,
	    A5 = 5
	};
public:
	static bool GetSizeA(int Type,bool bVertical, int& Width , int& Height);
	static bool GetSizeA(CString Type,bool bVertical, int& Width , int& Height);

};
struct LayoutSub
{
	CString Page;
	CString Direction;
	int MarginL,MarginR,MarginT,MarginB;
	int ColNum;
};

class CConfigGenCalc :public CInfoBase
{

	DECLARE_DYNAMIC(CConfigGenCalc)

	friend class CInfoBase;
public:
	static const TCHAR* XML_FILE_NAME ;
	static const TCHAR* CalculationReportXml ;
public:
	slim::XmlDocument m_doc;
public:
	CConfigGenCalc(void);
	~CConfigGenCalc(void);
public:
	bool DeleteCalculationReportXml();

	bool saveToFile(CString& strFileName);
	bool readFromFile(CString strFileName);
	void init();
	void clear();
// 	void addViewT( CString filename ,CString caption );
// 	void addLoadsT(CString filename ,CString caption);
// 	void addStressT(CString filename ,CString caption);
// 	void addStressScaleT(CString filename ,CString caption);
	void addFiberNM_T(CString filename ,CString caption);
	void addPDMNM_T(CString filename ,CString caption);
	void addFiberNM_T2(CString filename ,CString caption);
	void addPDMNM_T2(CString filename ,CString caption);
	void addSection_T(CString filename ,CString caption);
//	void addDispT(CString filename ,CString caption);
	void setPathT(CString str = _T("CalcTemp\\"));

public:
	LayoutSub textLayOut;
	LayoutSub graphLayOut;

private:
	//写入函数
	void WriteToReportOption();

	void WriteToCover(slim::XmlNode* ndReportOption);
	void WriteToFont(slim::XmlNode *ndFont);
	void WriteToVersion(slim::XmlNode *ndVersion);
	void WriteToGeneral1(slim::XmlNode *ndGeneral1);
	void WriteToGeneral2(slim::XmlNode *ndGeneral2);

	void WriteToHeaderFooter(slim::XmlNode *ndHeaderFooter);

	void WriteToLayout(slim::XmlNode *ndLayout);
	void WriteToTable(slim::XmlNode *ndTable);
	void WriteToNumStyle(slim::XmlNode *ndNumStyle);

	void WriteToAllItems(slim::XmlNode *ndAllItems);
	void WriteToSelectedItems(slim::XmlNode *ndSelectedItems);
	void WriteToSetup(slim::XmlNode *ndSetup);

	CString Option(Json::Value&  val,int idx,CString pathfile,double bottom,double left,double right,double top,CString drawname="",
		bool forceDrawNum=true,bool forceFontHeight=false,bool forceFontZoomout=false,bool foreFontZoomoutMult=1,int minFontSize=1);
	void addT(CArray<ConfigDrawT>& arr, CString filename ,
		CString caption );

	void addT0001(CArray<ConfigDrawT>& arr, CString filename ,
		CString caption );
	//读取函数
	bool ReadFromReportOption();
	bool ReadFromLayout(slim::XmlNode *ndLayout);

private:
	int m_ver;
	CString m_Title;
	CString m_ProjNum;
	CString m_ProjName;
	CString m_Computer;
	CString m_MajorOfficer;
	CString m_Date         ;
	CString m_Checker	   ;
	CString m_Company	   ;
	CString m_Sketch	   ;
	CString m_PageHeader;
//	CArray<ConfigDrawT> T_View;
// 	CArray<ConfigDrawT> T_Loads;
// 	CArray<ConfigDrawT> T_Stress;
// 	CArray<ConfigDrawT> T_StressScale;
// 	CArray<ConfigDrawT> T_Disp;
	CArray<ConfigDrawT> T_FiberNM;
	CArray<ConfigDrawT> T_PDMNM;
	CArray<ConfigDrawT> T_FiberNM_2;
	CArray<ConfigDrawT> T_PDMNM_2;
	CArray<ConfigDrawT> T_Section;
	CString m_strPathT;
};







class CConfigGenJC :public CInfoBase
{

	DECLARE_DYNAMIC(CConfigGenJC)

	friend class CInfoBase;
public:
	static const TCHAR* XML_FILE_NAME ;
public:
	slim::XmlDocument doc;
public:
	CConfigGenJC(void);
	~CConfigGenJC(void);
public:
	bool saveTo(CString strFileName);
	void init();
	void clear();
 
private:
	void ReportOption();

	void Cover(slim::XmlNode* ndReportOption);
	void Font(slim::XmlNode *ndFont);
	void Version(slim::XmlNode *ndVersion);
	void General1(slim::XmlNode *ndGeneral1);
	void General2(slim::XmlNode *ndGeneral2);

	void HeaderFooter(slim::XmlNode *ndHeaderFooter);

	void Layout(slim::XmlNode *ndLayout);
	void Table(slim::XmlNode *ndTable);
	void NumStyle(slim::XmlNode *ndNumStyle);

	void AllItems(slim::XmlNode *ndAllItems);
	void SelectedItems(slim::XmlNode *ndSelectedItems);
	void Setup(slim::XmlNode *ndSetup);

	CString Option(Json::Value&  val,int idx,CString pathfile,double bottom,double left,double right,double top,CString drawname="",
		bool forceDrawNum=true,bool forceFontHeight=false,bool forceFontZoomout=false,bool foreFontZoomoutMult=1,int minFontSize=1);
 
private:
	int m_ver;
	CString m_Title;
	CString m_ProjNum;
	CString m_ProjName;
	CString m_Computer;
	CString m_MajorOfficer;
	CString m_Date         ;
	CString m_Checker	   ;
	CString m_Company	   ;
	CString m_Sketch	   ;
	CString m_PageHeader;

	CArray<ConfigDrawT> T_Loads;
	CArray<ConfigDrawT> T_Stress;
	CArray<ConfigDrawT> T_Disp;

	CString m_strPathT;
};



