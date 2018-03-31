#include "StdAfx.h"
#include "GReport.h"
#include "ICalculationReport.h"
#include "CfgSvr.h"
#include <io.h>
#include <tchar.h>
#include <afxstr.h>
using namespace sd;

TCHAR * GReport::PATH_CONFIG = _T("CalcReportConfig ");
#ifdef _DEBUG
	TCHAR * GReport::PATH_CALCREPROT_DLL = _T("CalculateReportD");
#else
	TCHAR * GReport::PATH_CALCREPROT_DLL = _T("CalculateReport");
#endif

GReport::GReport(void)
{
}


GReport::~GReport(void)
{
}

sd::ICalculationReport * GReport::g_pReport = NULL;
HMODULE GReport::m_hReportHnadle = NULL;

bool GReport::init()
{
	CString sDllFile;
#ifdef _DEBUG
#ifdef WIN32
	sDllFile="CalculationReport32D.dll";
#else //WIN64
	sDllFile="CalculationReport64D.dll";
#endif
#else
#ifdef WIN32
	sDllFile="CalculationReport32.dll";
#else //WIN64
	sDllFile="CalculationReport64.dll";
#endif
#endif

	//本地优先 ,之后才是PM下的
	CString testFile1 = sDllFile;
	CString testFile2 = sDllFile;
	CString path1,path2;
	path1 = GetCalculateReportLibPath(testFile1,RelativePath);
	path2 = GetCalculateReportLibPath(testFile2,PmSub);
	
	if(m_hReportHnadle == NULL)
	{
		CString strOriWorkDir1;
 		::GetCurrentDirectory(MAX_PATH,strOriWorkDir1.GetBuffer(MAX_PATH));
		::SetCurrentDirectoryA(path1);

		sDllFile = path1 + sDllFile;
		m_hReportHnadle=::LoadLibraryA(testFile1);

 		::SetCurrentDirectoryA(strOriWorkDir1);

		if (m_hReportHnadle == NULL)
		{
			CString strOriWorkDir2;
			::GetCurrentDirectory(MAX_PATH,strOriWorkDir2.GetBuffer(MAX_PATH));
			strOriWorkDir2.ReleaseBuffer(-1);
			::SetCurrentDirectoryA(path2);
				m_hReportHnadle=::LoadLibraryA(testFile2);
			::SetCurrentDirectoryA(strOriWorkDir2);
		}
	}
	if (m_hReportHnadle == NULL)
	{
#ifdef _DEBUG
		CString strHit;
		strHit.Format(_T("无法找到\"计算书中间件\",请检查 <PM>或<调试目录>下是否有\"%s\"文件夹"),
			GReport::PATH_CALCREPROT_DLL);
		AfxMessageBox(strHit);
#else
		CString strHit;
		strHit.Format(_T("无法找到\"计算书中间件\",请重新安装程序 \n位置:\"%s\"文件夹"),testFile2);
		AfxMessageBox(strHit);
#endif // _DEBUG
		
		return false;
	}


	if(g_pReport == NULL)
	{
		typedef ICalculationReport* (_cdecl * GetCalculationReportFun)();
		GetCalculationReportFun pGetCalculationFun
			=(GetCalculationReportFun)GetProcAddress(m_hReportHnadle,
			"getCalculationReport");
		if(pGetCalculationFun)
			g_pReport=pGetCalculationFun();
	}

	return true;  
}


CString GReport::GetCalculateReportLibPath(CString &file,int nType /*0=相对路径 1 Pm下的路径*/)
{
	CString dllPath;
	if (RelativePath == nType)
	{
		TCHAR szDllPath[MAX_PATH];
		GetModuleFileName(AfxGetStaticModuleState()->m_hCurrentInstanceHandle,szDllPath,MAX_PATH);
		dllPath = szDllPath;
		dllPath = dllPath.Left(dllPath.ReverseFind(_T('\\')));
	}
	else
	{
		CString strPm;
		if(CfgSvrUser::GetPmPath(strPm))
		{
			dllPath = strPm;
		}

	}
	CfgSvrUser::FixPathStr(dllPath);

	dllPath += PATH_CALCREPROT_DLL;
	CfgSvrUser::FixPathStr(dllPath);

	file = dllPath + file;
	return dllPath;
}

CString GReport::GetReprotXmlFullPathname(CString& strXmlFileName)
{
	CString pathWork;
	{
		::GetCurrentDirectory(MAX_PATH,pathWork.GetBuffer(MAX_PATH));
		pathWork.ReleaseBuffer(-1);
		pathWork+= _T("\\");
	}
	strXmlFileName = pathWork + strXmlFileName;
	return   strXmlFileName;

}

CString GReport::GetConfigPath()
{
	TCHAR szDllPath[MAX_PATH];
	GetModuleFileName(AfxGetStaticModuleState()->m_hCurrentInstanceHandle,szDllPath,MAX_PATH);
	CString dllPath = szDllPath;
	dllPath = dllPath.Left(dllPath.ReverseFind(_T('\\')));

	CfgSvrUser::FixPathStr(dllPath);
	dllPath += PATH_CONFIG;
	
	CfgSvrUser::FixPathStr(dllPath);
	return dllPath;
}


