#pragma once
#include "ICalculationReport.h"
class GReport
{
public:
	GReport(void);
	~GReport(void);
	static TCHAR *PATH_CONFIG;
	static TCHAR *PATH_CALCREPROT_DLL;
	static bool init();
	static HMODULE m_hReportHnadle;
	static sd::ICalculationReport *g_pReport;

	static CString GetConfigPath();
	static CString GetReprotXmlFullPathname(CString& strXmlFileName);
	static CString GetCalculateReportLibPath(CString &file, int nType /*0=���·�� 1 Pm�µ�·��*/);

	enum 
	{
		RelativePath = 0,
		PmSub = 1
	};
};


