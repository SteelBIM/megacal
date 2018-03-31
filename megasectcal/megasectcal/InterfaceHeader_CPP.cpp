#include "stdafx.h"
#include "InterfaceHeader_CPP.h"
#include "Pk_MainOutPut.h"

//#include "Param_MainParam.h"
#include <math.h>
//#include "..\OptLib\CmdDataMgr.h"
//#include "..\OptLib\PluginCmd.h"
#include "ConfigGen.h"


// extern "C" 
// {
//  
// 	void __stdcall REFPRT(char NOTE[], int n2 );
// }

bool ProgressSetT(int np)
{
	CString strProgress;
	strProgress.Format("ÕýÔÚ¼ÆËãÍ¼Ö½±ß½ç %3d%%",np);
	//REFPRT(strProgress.GetBuffer(),strProgress.GetLength());
	strProgress.ReleaseBuffer();

	//PluginCmd::BigProgress(np);

	return true;
}

void SetConfigT(CConfigGenCalc& conf)
{
	conf.setPathT();
	conf.addFiberNM_T(_T("Fiber_NM_y.t"),  _T("Fiber N-M(y)"));
	conf.addFiberNM_T(_T("Fiber_NM_z.t"),  _T("Fiber N-M(z)"));
	conf.addPDMNM_T(_T("PDM_NM_y.t"),  _T("PDM N-M(y)"));
	conf.addPDMNM_T(_T("PDM_NM_z.t"),  _T("PDM N-M(z)"));
	//²»¿¼ÂÇ³¤¶ÈÐ§Ó¦µÄÍ¼
	conf.addFiberNM_T2(_T("Fiber_NM_y_nominal.t"),  _T("Fiber N-M(y)(without length effect)"));
	conf.addFiberNM_T2(_T("Fiber_NM_z_nominal.t"),  _T("Fiber N-M(z)(without length effect)"));
	conf.addPDMNM_T2(_T("PDM_NM_y_design.t"),  _T("PDM N-M(y)(without length effect)"));
	conf.addPDMNM_T2(_T("PDM_NM_z_design.t"),  _T("PDM N-M(z)(without length effect)"));
	conf.addSection_T(_T("temptemp.t"), _T("Section"));
// 	conf.setPathT();
// 
// 	//PluginCmd::ShowBigProgress();
// 
// 	ProgressSetT(0);
// 	 
// 	conf.addViewT(_T("¸Õ¼Ü¼òÍ¼.T"),  _T("¸Õ¼Ü¼òÍ¼"));//KLM-CAL.T
// 
// 	conf.addLoadsT(_T("ºãÔØ¼òÍ¼.T"),  _T("ºãÔØ¼òÍ¼"));//D-L-CAL.T
// 	if(StruCalcInfo().IsHasDiaoGua())
// 	{
// 		conf.addLoadsT(_T("µõ¹ÒºãÔØ¼òÍ¼.T"),  _T("µõ¹ÒºãÔØ¼òÍ¼"));//DG-D-L-CAL.T
// 	}
// 	ProgressSetT(10);
// 	if (1 == g_Param_MainParam.KLL )
// 	{
// 		
// 		conf.addLoadsT(_T("»îÔØ¼òÍ¼.T"), _T("»îÔØ¼òÍ¼"));//L-L-CAL.T
// 	}
// 	else if (g_Param_MainParam.KLL >1)
// 	{
// 		CString strfile;
// 		CString strtitle;
// 		for (int i=0;i<g_Param_MainParam.KLL;i++)
// 		{
// 			//strfile.Format(_T("µÚ%d×é»¥³â»îºÉÔØ¼òÍ¼.T"),i+1);//L-L%d-CAL.T
// 			if (0 == i)
// 			{
// 				strfile = _T("»îÔØ¼òÍ¼.T");
// 				strtitle = _T("»îÔØ¼òÍ¼");
// 			}
// 			else
// 			{
// 				strfile.Format(_T("µÚ%d×é»¥³â»îºÉÔØ¼òÍ¼.T"),i);
// 				strtitle.Format(_T("µÚ%d×é»¥³â»îºÉÔØ¼òÍ¼"),i);
// 			}
// 			conf.addLoadsT(strfile, strtitle);
// 		}
// 		
// 	}
// 
// 	//×Ô¶¨ÒåºÉÔØ¼òÍ¼
// 	//×Ô¶¨ÒåºÉÔØ¼òÍ¼
// 	{
// 		int UserZiDingYi = CmdDataMgr::Get().NZiDingYi();
// 		const TCHAR * formatUserCalcTmpFile = "%sºÉÔØ¼òÍ¼.T";
// 		const TCHAR * formatUserCalcTmpTitle= "%sºÉÔØ¼òÍ¼";
// 		CString strUserLCalcTmpFile;
// 		CString strUserLCalcTmpTitle;
// 		if (UserZiDingYi>0)
// 		{
// 			for (int i=0;i<UserZiDingYi;i++)
// 			{
// 				strUserLCalcTmpFile.Format(formatUserCalcTmpFile,CmdDataMgr::Get().NameZiDingYi(i));
// 				strUserLCalcTmpTitle.Format(formatUserCalcTmpTitle,CmdDataMgr::Get().NameZiDingYi(i));
// 				conf.addLoadsT(strUserLCalcTmpFile, strUserLCalcTmpTitle);
// 			}
// 		}
// 	}
// 	
// 
// 	ProgressSetT(20);
// 
// 	if (StruCalcInfo().nWindP > 0)
// 	{
// 		conf.addLoadsT(_T("×ó·ç1¼òÍ¼.T"), _T("×ó·ç1¼òÍ¼"));//L-W-1-CAL.T
// 		conf.addLoadsT(_T("ÓÒ·ç1¼òÍ¼.T") , _T("ÓÒ·ç1¼òÍ¼"));//R-W-1-CAL.T
// 	}
// 	ProgressSetT(25);
// 	if (StruCalcInfo().nWindP > 1)
// 	{
// 		conf.addLoadsT(_T("×ó·ç2¼òÍ¼.T"), _T("×ó·ç2¼òÍ¼"));//L-W-2-CAL.T
// 		conf.addLoadsT(_T("ÓÒ·ç2¼òÍ¼.T") , _T("ÓÒ·ç2¼òÍ¼"));//R-W-2-CAL.T
// 	}
// 	ProgressSetT(30);
// 	if (StruCalcInfo().nWindP > 2)
// 	{	
// 		conf.addLoadsT(_T("×ó·ç3¼òÍ¼.T"), _T("×ó·ç3¼òÍ¼"));//L-W-3-CAL.T
// 		conf.addLoadsT(_T("ÓÒ·ç3¼òÍ¼.T") , _T("ÓÒ·ç3¼òÍ¼"));//R-W-3-CAL.T
// 	}
// 	ProgressSetT(35);
// 	if (StruCalcInfo().nDiaoChe>0)
// 	{
// 		conf.addLoadsT(_T("µõ³µºÉÔØ¼òÍ¼.T") , _T("µõ³µºÉÔØ¼òÍ¼"));//C-H-CAL.T
// 	}
// 
// 	ProgressSetT(40);
// 	conf.addStressT(_T("ºãÔØÍä¾ØÍ¼.T"), _T("ºãÔØÍä¾ØÍ¼"));//D-M.T
// 	conf.addStressT(_T("ºãÔØ¼ôÁ¦Í¼.T"), _T("ºãÔØ¼ôÁ¦Í¼"));//D-V.T
// 	conf.addStressT(_T("ºãÔØÖáÁ¦Í¼.T"), _T("ºãÔØÖáÁ¦Í¼"));//D-N.T  
// 
// 	ProgressSetT(45);
// 	if(StruCalcInfo().IsHasDiaoGua())
// 	{
// 		conf.addStressT(_T("µõ¹ÒºãÔØÍä¾ØÍ¼.T"), _T("µõ¹ÒºãÔØÍä¾ØÍ¼"));//DG-M.T
// 		conf.addStressT(_T("µõ¹ÒºãÔØÖáÁ¦Í¼.T"), _T("µõ¹ÒºãÔØÖáÁ¦Í¼"));//DG-N.T
// 		conf.addStressT(_T("µõ¹ÒºãÔØ¼ôÁ¦Í¼.T"), _T("µõ¹ÒºãÔØ¼ôÁ¦Í¼"));//DG-V.T
// 	}
// 
// 	ProgressSetT(50);
// 	conf.addStressT(_T("»îÔØÍä¾ØÍ¼.T"), _T("»îÔØÍä¾ØÍ¼"));//L-M.T
// 	conf.addStressT(_T("»îÔØ¼ôÁ¦Í¼.T"), _T("»îÔØ¼ôÁ¦Í¼"));//L-V.T
// 	conf.addStressT(_T("»îÔØÖáÁ¦Í¼.T"), _T("»îÔØÖáÁ¦Í¼"));//L-N.T
// 
// 	ProgressSetT(55);
// 	if (StruCalcInfo().nWindP > 0)
// 	{
// 		conf.addStressT(_T("×ó·ç1Íä¾ØÍ¼.T"), _T("×ó·ç1Íä¾ØÍ¼"));//WL.T
// 		conf.addStressT(_T("ÓÒ·ç1Íä¾ØÍ¼.T"), _T("ÓÒ·ç1Íä¾ØÍ¼"));//WR.T
// 	}
// 	ProgressSetT(60);
// 	if (StruCalcInfo().nWindP > 1)
// 	{
// 		conf.addStressT(_T("×ó·ç2Íä¾ØÍ¼.T"), _T("×ó·ç2Íä¾ØÍ¼"));//WL2.T
// 		conf.addStressT(_T("ÓÒ·ç2Íä¾ØÍ¼.T"), _T("ÓÒ·ç2Íä¾ØÍ¼"));//WR2.T
// 	}
// 	ProgressSetT(75);
// 	if (StruCalcInfo().nWindP > 2)
// 	{
// 		conf.addStressT(_T("×ó·ç3Íä¾ØÍ¼.T"), _T("×ó·ç3Íä¾ØÍ¼"));//WL3.T
// 		conf.addStressT(_T("ÓÒ·ç3Íä¾ØÍ¼"), _T("ÓÒ·ç3Íä¾ØÍ¼"));//WR3.T 
// 	}
// 	ProgressSetT(80);
// 	conf.addStressT(_T("×óµØÕðÍä¾ØÍ¼.T"), _T("×óµØÕðÍä¾ØÍ¼"));//EL.T
// 	conf.addStressT(_T("ÓÒµØÕðÍä¾ØÍ¼.T"), _T("ÓÒµØÕðÍä¾ØÍ¼"));//ER.T
// 
// 	ProgressSetT(82);
// 	{
// 		//×Ô¶¨ÒåÍä¾Ø¼ôÁ¦Í¼
// 		{
// 			int UserZiDingYi = CmdDataMgr::Get().NZiDingYi();
// 
// 			CString strUserFname;
// 			CString strTitle;
// 			if (UserZiDingYi>0)
// 			{
// 				for (int i=0;i<CmdDataMgr::Get().NameZiDingYiNeiLiTu().GetCount();i++)
// 				{
// 					strUserFname =  CmdDataMgr::Get().FiNameZiDingYiNeiLiTu(i);
// 					strTitle = CmdDataMgr::Get().NameZiDingYiNeiLiTu(i);
// 					conf.addStressT(strUserFname,strTitle);
// 				}
// 			}
// 		}
// 	}
// 
// 
// 	ProgressSetT(85);
// 	conf.addStressT(_T("Íä¾Ø°üÂçÍ¼.T"), _T("Íä¾Ø°üÂçÍ¼"));//M.T
// 	conf.addStressT(_T("¼ôÁ¦°üÂçÍ¼.T"), _T("¼ôÁ¦°üÂçÍ¼"));//Q.T
// 	conf.addStressT(_T("ÖáÁ¦°üÂçÍ¼.T"), _T("ÖáÁ¦°üÂçÍ¼"));//N.T
// 
// 	conf.addStressScaleT(_T("Ó¦Á¦±ÈÍ¼.T"), _T("Ó¦Á¦±ÈÍ¼"));//AS.T
// 
// 	ProgressSetT(90);
// 	conf.addDispT(_T("ºãÔØÎ»ÒÆÍ¼.T"), _T("ºãÔØÎ»ÒÆÍ¼"));//DISP-D.T
// 	if (StruCalcInfo().IsHasDiaoGua())
// 	{
// 		conf.addDispT(_T("µõ¹ÒºãÔØÎ»ÒÆÍ¼.T"), _T("µõ¹ÒºãÔØÎ»ÒÆÍ¼"));//DISP-DG.T
// 	}
// 	conf.addDispT(_T("»îÔØÎ»ÒÆÍ¼.T"), _T("»îÔØÎ»ÒÆÍ¼"));//DISP-L.T
// 	if (StruCalcInfo().nWindP > 0)
// 	{
// 		conf.addDispT(_T("×ó·ç1Î»ÒÆÍ¼.T"), _T("×ó·ç1Î»ÒÆÍ¼"));//DISP-WL.T
// 		conf.addDispT(_T("ÓÒ·ç1Î»ÒÆÍ¼.T"), _T("ÓÒ·ç1Î»ÒÆÍ¼"));//DISP-WR.T
// 	}
// 
// 	ProgressSetT(95);
// 	if (StruCalcInfo().nWindP > 1)
// 	{
// 		conf.addDispT(_T("×ó·ç2Î»ÒÆÍ¼.T"), _T("×ó·ç2Î»ÒÆÍ¼"));//DISP-WL2.T
// 		conf.addDispT(_T("ÓÒ·ç2Î»ÒÆÍ¼.T"), _T("ÓÒ·ç2Î»ÒÆÍ¼"));//DISP-WR2.T
// 	}
// 
// 	if (StruCalcInfo().nWindP > 2)
// 	{
// 		conf.addDispT(_T("×ó·ç3Î»ÒÆÍ¼.T"), _T("×ó·ç3Î»ÒÆÍ¼"));//DISP-WL3.T
// 		conf.addDispT(_T("ÓÒ·ç3Î»ÒÆÍ¼.T"), _T("ÓÒ·ç3Î»ÒÆÍ¼"));//DISP-WR3.T
// 	}
// 	ProgressSetT(100);
// 
// 	if (StruCalcInfo().bHasEarthLR)
// 	{
// 		conf.addDispT(_T("×óµØÕðÎ»ÒÆÍ¼.T"), _T("×óµØÕðÎ»ÒÆÍ¼"));//DISP-EL.T
// 		conf.addDispT(_T("ÓÒµØÕðÎ»ÒÆÍ¼.T"), _T("ÓÒµØÕðÎ»ÒÆÍ¼"));//DISP-ER.T
// 	}
// 
// 	if (StruCalcInfo().bHasDiaoGua  )
// 	{
// 		conf.addDispT(_T("ºã+»îÎ»ÒÆÍ¼.T"), _T("ºã+µõ¹Òºã+»îÎ»ÒÆÍ¼"));//DISP-DL.T
// 		// ×¢ÒâÕâÀïÃ»ÓÐ³ö´í,Í¼Ö½¾ÍÊÇÕâ¸ö  ºã¼Ó»îÎ»ÒÆÍ¼.t
// 	}
// 	else
// 	{
// 		conf.addDispT(_T("ºã+»îÎ»ÒÆÍ¼.T"), _T("ºã+»îÎ»ÒÆÍ¼"));//DISP-DL.T
// 	}
// 	 
// 	
// 	if (StruCalcInfo().nDiaoChe >0 )
// 	{
// 		conf.addDispT(_T("µõ³µÉ²³µÎ»ÒÆÍ¼.T"), _T("µõ³µÉ²³µÎ»ÒÆÍ¼"));//DISP-TD.T
// 	}
// 
// 
// 
// 	{ //×Ô¶¨ÒåÎ»ÒÆÍ¼
// 		CString strFi;
// 		CString strTitle;
// 		for (int i=0;i<CmdDataMgr::Get().NameZdySingleDispTu().GetCount();i++)
// 		{
// 			strFi = CmdDataMgr::Get().FiNameZdySingleDispTu(i);
// 			strTitle = CmdDataMgr::Get().NameZdySingleDispTu(i);
// 			conf.addDispT(strFi, strTitle);
// 		}
// 		bool bfirst=true;
// 		for (int i=0;i<CmdDataMgr::Get().NameZdyMultDispTu().GetCount();i++)
// 		{
// 			if (bfirst)
// 			{
// 				bfirst=false;
// 				if(CmdDataMgr::Get().AvoidFirstZdyMultDisp())
// 				{
// 					continue;
// 				}
// 
// 			}
// 			strFi = CmdDataMgr::Get().FiNameZdyMultDispTu(i);
// 			strTitle = CmdDataMgr::Get().NameZdyMultDispTu(i);
// 			conf.addDispT(strFi, strTitle);
// 		}
// 
// 	}
// 
// 	PluginCmd::CloseBigProgress();	
// 
// 
// 	CString str = "Íê³ÉÍ¼Ö½ÇøÓò¼ÆËã";
// 	int n = str.GetLength();
// 	REFPRT(str.GetBuffer(),n);
// 
// 	str = "´ò¿ª\"¼ÆËãÊéÉèÖÃ\"¶Ô»°¿ò...";
// 	n = str.GetLength();
// 	REFPRT(str.GetBuffer(),n);


}
extern void CalcOutWord()
{	
	//ÇåÀí¾ÉÍ¼  + ÇåÀí CalculationReport.xml
	CPk_MainOutPut::initReportTool();//ÒÑ¾­×¼±¸Íê³ÉÁËTÍ¼
	CConfigGenCalc conf;
	conf.DeleteCalculationReportXml();

	//²úÉúÐÂÎÄ¼þ,ºÍTÍ¼
	SetConfigT(conf);
	//CPk_MainOutPut::ClearCal_T(); 
	conf.saveToFile(CPk_MainOutPut::configXml);

	CPk_MainOutPut::dlgConfig();
}

extern void Main_Param()
{

}


//int &Itype, int &NInt, int IPara[], int &NFloat, float RPara[], int &NStr, int &LStr, char *StrPara[], int n1
extern "C" void __stdcall Param_PrjName(char* prjName,int Len)
{
	//g_Param_MainParam.PrjName  = prjName;
}
