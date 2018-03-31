#include "stdafx.h"
#include "Pk_MainOutPut.h"
#include "IGenerateText.h"
#include "ConfigGen.h"
#include "ParagraphMacroHeader.h"
#include "GReport.h"
//#include "StructBaseInfo.h"
//#include "..\OptLib\CmdDataMgr.h"
using namespace sd; 

// 
 bool CPk_MainOutPut::drawingRoutine( sd::IGenerateText* pGenerateText,
 	const sd::GRAPHIC_ITEM& mReportItem)
 {
 	//XMLŒƒµµ∂¡»°
 	const TCHAR  Hori[] = _T("Hori");
 	const TCHAR  Veri[] = _T("Veri");
 	CConfigGenCalc conf;
 	conf.readFromFile(CPk_MainOutPut::CalculationReportXml);
 	m_drawHoriVeri = 
 		conf.graphLayOut.Direction.CompareNoCase(Hori)==0?DRAWPAGE_HORI:DRAWPAGE_VERI;
 	bool bRetSelfControl = false;
 	if(DRAWPAGE_HORI ==  m_drawHoriVeri) 
 	{
 		if (FILE_PDF == GReport::g_pReport->getCurrentFileType())
 		{
 			bRetSelfControl = true;
 		}
 		else //FILE_WORD
 		{
 			//∫·∞Ê «“¿¥””√ªßøÿ÷∆
 			bRetSelfControl = false;
 			return bRetSelfControl; //“¿¥””√ªßøÿ÷∆÷±Ω”∑µªÿ
 		}
 		
 	}
 	else
 	{
 		bRetSelfControl = true;
 	}
 
 	//∏˘æ›≤ªÕ¨’¬Ω⁄¿¥ªÊ÷∆Õº–Œ
 	switch(mReportItem.key)
 	{
 	case STS_STRUCT_FIBER_1:
 		CPk_MainOutPut::Drawing_Fiber_1(pGenerateText,mReportItem,conf);
		break;
	case STS_STRUCT_PDM_1:
		CPk_MainOutPut::Drawing_PDM_1(pGenerateText,mReportItem,conf);
 		break;
	case STS_STRUCT_FIBER_2:
		CPk_MainOutPut::Drawing_Fiber_2(pGenerateText,mReportItem,conf);
		break;
	case STS_STRUCT_PDM_2:
		CPk_MainOutPut::Drawing_PDM_2(pGenerateText,mReportItem,conf);
		break;
	case STS_STRUCT_SECTION:
		CPk_MainOutPut::Drawing_Sect(pGenerateText,mReportItem,conf);
		break;
 	}
 
 	return true;
 
 }
// bool CPk_MainOutPut::GenDrawingName(const sd::GRAPHIC_ITEM& mReportItem,CString& DrawName,bool resetIndex)
// {
// 	static int PicIndex = 0;
// 	if (resetIndex)
// 	{
// 		PicIndex = 0;
// 	}
// 	int nCh = mReportItem.chapterSection.level_1;
// 	CString strTitle ;
// 	strTitle.Format(_T("Õº%d-%d %s"),nCh+1, ++PicIndex, DrawName);
// 	DrawName = strTitle;
// 	return true;
// }
// bool CPk_MainOutPut::DrawingView( sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config)
// {
// 
// 	CStringArray filenames,titleNames;
// 	filenames.Add(_T("∏’º‹ºÚÕº.T"));  titleNames.Add(_T("∏’º‹ºÚÕº"));//KLM-CAL.T
// 
// 
// 	GRAPHIC_FORMAT GFormat;
// 
// 	GFormat.height = 0;
// 	GFormat.width = 0;
// 	GFormat.align = sdAlignCenter;
// 	GFormat.titleFormat.align = GFormat.align;
// 
// 	CString strTitle;
// 	CString strFile;
// 
// 	int nWid = mReportItem.pageWidth-config.graphLayOut.MarginL-config.graphLayOut.MarginR;
// 	bool resetIndex = true;
// 	for (int i=0;i<filenames.GetSize();i++)
// 	{
// 		strFile =  filenames[i];
// 		strTitle = titleNames[i];
// 		GenDrawingName(mReportItem,strTitle,resetIndex);
// 		if (resetIndex == true)
// 		{
// 			resetIndex = false;
// 		}
// 		GFormat.titleFormat.setTitle(strTitle);
// 		DrawingFunc(pGenerateText,mReportItem,strFile,&GFormat,nWid);
// 	}
// 
// 	return true;
// }
// 
// bool CPk_MainOutPut::DrawingLoad( sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config)
// {
// 
// 	CStringArray filenames,titleNames;
// 	 
// 	filenames.Add(_T("∫„‘ÿºÚÕº.T"));  titleNames.Add(_T("∫„‘ÿºÚÕº"));//D-L-CAL.T
// 	if(StruCalcInfo().IsHasDiaoGua())
// 	{
// 		filenames.Add(_T("µıπ“∫„‘ÿºÚÕº.T"));   titleNames.Add(_T("µıπ“∫„‘ÿºÚÕº"));//DG-D-L-CAL.T
// 	}
// 
// 
// 	if (1 == g_Param_MainParam.KLL )
// 	{
// 		filenames.Add(_T("ªÓ‘ÿºÚÕº.T")); titleNames.Add(_T("ªÓ‘ÿºÚÕº"));//L-L-CAL.T
// 	}
// 	else if (g_Param_MainParam.KLL >1)
// 	{
// 		CString strfile;
// 		CString strtitle;
// 		for (int i=0;i<g_Param_MainParam.KLL;i++)
// 		{
// 			strfile.Format(_T("µ⁄%d◊Èª•≥‚ªÓ∫…‘ÿºÚÕº.T"),i+1);//L-L%d-CAL.T
// 			if (0 == i)
// 			{
// 				strtitle = _T("ªÓ‘ÿºÚÕº");
// 			}
// 			else
// 			{
// 				strtitle.Format(_T("µ⁄%d◊Èª•≥‚ªÓ∫…‘ÿºÚÕº"),i);//
// 			}
// 			filenames.Add(strfile); titleNames.Add(strtitle);
// 		}
// 
// 	}
// 
// 	//◊‘∂®“Â∫…‘ÿºÚÕº
// 	{
// 		int UserZiDingYi = CmdDataMgr::Get().NZiDingYi();
// 		const TCHAR * formatTitle = "%s∫…‘ÿºÚÕº";
// 		const TCHAR * formatFname = "%s∫…‘ÿºÚÕº.T";
// 		CString strUserTitle;
// 		CString strUserFname;
// 		if (UserZiDingYi>0)
// 		{
// 			for (int i=0;i<UserZiDingYi;i++)
// 			{
// 				strUserTitle.Format(formatTitle,CmdDataMgr::Get().NameZiDingYi(i));
// 				strUserFname.Format(formatFname,CmdDataMgr::Get().NameZiDingYi(i));
// 				filenames.Add(strUserFname); titleNames.Add(strUserTitle);
// 			}
// 		}
// 	}
// 
// 	if (StruCalcInfo().nWindP > 0)
// 	{
// 		filenames.Add(_T("◊Û∑Á1ºÚÕº.T"));titleNames.Add(_T("◊Û∑Á1ºÚÕº"));//L-W-1-CAL.T
// 		filenames.Add(_T("”“∑Á1ºÚÕº.T"));titleNames.Add(_T("”“∑Á1ºÚÕº"));//R-W-1-CAL.T
// 	}
// 	
// 	if (StruCalcInfo().nWindP > 1)
// 	{
// 		filenames.Add(_T("◊Û∑Á2ºÚÕº.T"));titleNames.Add(_T("◊Û∑Á2ºÚÕº"));//L-W-2-CAL.T
// 		filenames.Add(_T("”“∑Á2ºÚÕº.T"));titleNames.Add(_T("”“∑Á2ºÚÕº"));//R-W-2-CAL.T
// 	}
// 	
// 	if (StruCalcInfo().nWindP > 2)
// 	{
// 		filenames.Add(_T("◊Û∑Á3ºÚÕº.T"));titleNames.Add(_T("◊Û∑Á3ºÚÕº"));//L-W-3-CAL.T
// 		filenames.Add(_T("”“∑Á3ºÚÕº.T"));titleNames.Add(_T("”“∑Á3ºÚÕº"));//R-W-3-CAL.T
// 	}
// 	if (StruCalcInfo().nDiaoChe>0)
// 	{
// 		filenames.Add(_T("µı≥µ∫…‘ÿºÚÕº.T"));titleNames.Add(_T("µı≥µ∫…‘ÿºÚÕº"));//C-H-CAL
// 	}
// 	
// 	
// 	GRAPHIC_FORMAT GFormat;
// 
// 	GFormat.height = 0;
// 	GFormat.width = 0;
// 	GFormat.align = sdAlignCenter;
// 	GFormat.titleFormat.align = GFormat.align;
// 
// 	CString strTitle;
// 	CString strFile;
// 
// 	int nWid = mReportItem.pageWidth-config.graphLayOut.MarginL-config.graphLayOut.MarginR;
// 	bool resetIndex = true;
// 	for (int i=0;i<filenames.GetSize();i++)
// 	{
// 		strFile =  filenames[i];
// 		strTitle = titleNames[i];
// 		GenDrawingName(mReportItem,strTitle,resetIndex);
// 		if (resetIndex == true)
// 		{
// 			resetIndex = false;
// 		}
// 		GFormat.titleFormat.setTitle(strTitle);
// 		DrawingFunc(pGenerateText,mReportItem,strFile,&GFormat,nWid);
// 	}
// 
// 	return true;
// 
// 
// }
// bool CPk_MainOutPut::DrawingStress( sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config)
// {
// 	CStringArray filenames,titleNames;
// 
// 	filenames.Add(_T("∫„‘ÿÕ‰æÿÕº.T"));titleNames.Add( _T("∫„‘ÿÕ‰æÿÕº"));//D-M.T
// 	filenames.Add(_T("∫„‘ÿºÙ¡¶Õº.T"));titleNames.Add( _T("∫„‘ÿºÙ¡¶Õº"));//D-V.T
// 	filenames.Add(_T("∫„‘ÿ÷·¡¶Õº.T"));titleNames.Add( _T("∫„‘ÿ÷·¡¶Õº"));//D-N.T
// 	
// 	filenames.Add(_T("ªÓ‘ÿÕ‰æÿÕº.T"));titleNames.Add( _T("ªÓ‘ÿÕ‰æÿÕº"));//L-M.T
// 	filenames.Add(_T("ªÓ‘ÿºÙ¡¶Õº.T"));titleNames.Add( _T("ªÓ‘ÿºÙ¡¶Õº"));//L-V.T
// 	filenames.Add(_T("ªÓ‘ÿ÷·¡¶Õº.T"));titleNames.Add( _T("ªÓ‘ÿ÷·¡¶Õº"));//L-N.T
// 	
// 	if (StruCalcInfo().IsHasDiaoGua())
// 	{
// 		filenames.Add(_T("µıπ“∫„‘ÿÕ‰æÿÕº.T"));titleNames.Add( _T("µıπ“∫„‘ÿÕ‰æÿÕº"));//DG-M.T
// 		filenames.Add(_T("µıπ“∫„‘ÿ÷·¡¶Õº.T"));titleNames.Add( _T("µıπ“∫„‘ÿ÷·¡¶Õº"));//DG-N.T
// 		filenames.Add(_T("µıπ“∫„‘ÿºÙ¡¶Õº.T"));titleNames.Add( _T("µıπ“∫„‘ÿºÙ¡¶Õº"));//DG-V.T
// 	}
// 
// 	if (StruCalcInfo().nWindP > 0)
// 	{
// 		filenames.Add(_T("◊Û∑Á1Õ‰æÿÕº.T"));titleNames.Add( _T("◊Û∑Á1Õ‰æÿÕº"));//WL.T
// 		filenames.Add(_T("”“∑Á1Õ‰æÿÕº.T"));titleNames.Add( _T("”“∑Á1Õ‰æÿÕº"));//WR.T
// 	}
// 	if (StruCalcInfo().nWindP > 1)
// 	{
// 		filenames.Add(_T("◊Û∑Á2Õ‰æÿÕº.T"));titleNames.Add( _T("◊Û∑Á2Õ‰æÿÕº"));//WL2.T
// 		filenames.Add(_T("”“∑Á2Õ‰æÿÕº.T"));titleNames.Add( _T("”“∑Á2Õ‰æÿÕº"));//WR2.T
// 	}
// 	if (StruCalcInfo().nWindP > 2)
// 	{
// 		filenames.Add(_T("◊Û∑Á3Õ‰æÿÕº.T"));titleNames.Add( _T("◊Û∑Á3Õ‰æÿÕº"));//WL3.T
// 		filenames.Add(_T("”“∑Á3Õ‰æÿÕº.T"));titleNames.Add( _T("”“∑Á3Õ‰æÿÕº"));//WR3.T
// 	}
// 
// 	filenames.Add(_T("◊Ûµÿ’Õ‰æÿÕº.T"));titleNames.Add( _T("◊Ûµÿ’Õ‰æÿÕº"));//EL.T
// 	filenames.Add(_T("”“µÿ’Õ‰æÿÕº.T"));titleNames.Add( _T("”“µÿ’Õ‰æÿÕº"));//ER.T
// 
// 
// 
// 
// 	//◊‘∂®“ÂÕ‰æÿºÙ¡¶Õº
// 	{
// 		int UserZiDingYi = CmdDataMgr::Get().NZiDingYi();
// 	
// 		CString strUserFname;
// 		if (UserZiDingYi>0)
// 		{
// 			for (int i=0;i<CmdDataMgr::Get().NameZiDingYiNeiLiTu().GetCount();i++)
// 			{
// 				strUserFname =  CmdDataMgr::Get().NameZiDingYiNeiLiTu(i);
// 				filenames.Add(strUserFname); titleNames.Add(strUserFname);
// 			}
// 		}
// 	}
// 
// 
// 	filenames.Add(_T("Õ‰æÿ∞¸¬ÁÕº.T")); titleNames.Add(_T("Õ‰æÿ∞¸¬ÁÕº"));	 //M.T
// 	filenames.Add(_T("ºÙ¡¶∞¸¬ÁÕº.T")); titleNames.Add(_T("ºÙ¡¶∞¸¬ÁÕº"));	 //Q.T
// 	filenames.Add(_T("÷·¡¶∞¸¬ÁÕº.T")); titleNames.Add(_T("÷·¡¶∞¸¬ÁÕº"));	 //N.T
// 
// 
// 
// 
// 	 
// 
// 	GRAPHIC_FORMAT GFormat;
// 
// 	GFormat.height = 0;
// 	GFormat.width = 0;
// 	GFormat.align = sdAlignCenter;
// 	GFormat.titleFormat.align = GFormat.align;
// 
// 	CString strTitle;
// 	CString strFile;
// 	int nWid = mReportItem.pageWidth-config.graphLayOut.MarginL-config.graphLayOut.MarginR;
//  
// 	for (int i=0;i<filenames.GetSize();i++)
// 	{
// 		strFile =  filenames[i];
// 		strTitle = titleNames[i];
// 		GenDrawingName(mReportItem,strTitle  );
// 		 
// 		GFormat.titleFormat.setTitle(strTitle);
// 		DrawingFunc(pGenerateText,mReportItem,strFile,&GFormat,nWid);
// 	}
// 	
// 	return true;
// }
// bool CPk_MainOutPut::DrawingStressScale(sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config)
// {
// 	CStringArray filenames,titleNames;
// 
// 	filenames.Add(_T("”¶¡¶±»Õº.T")); titleNames.Add(_T("”¶¡¶±»Õº"));	 //N.T
// 
// 	GRAPHIC_FORMAT GFormat;
// 
// 	GFormat.height = 0;
// 	GFormat.width = 0;
// 	GFormat.align = sdAlignCenter;
// 	GFormat.titleFormat.align = GFormat.align;
// 
// 	CString strTitle;
// 	CString strFile;
// 	int nWid = mReportItem.pageWidth-config.graphLayOut.MarginL-config.graphLayOut.MarginR;
// 
// 	for (int i=0;i<filenames.GetSize();i++)
// 	{
// 		strFile =  filenames[i];
// 		strTitle = titleNames[i];
// 		GenDrawingName(mReportItem,strTitle  );
// 
// 		GFormat.titleFormat.setTitle(strTitle);
// 		DrawingFunc(pGenerateText,mReportItem,strFile,&GFormat,nWid);
// 	}
// 
// 	return true;
// 
// }
// 
// bool CPk_MainOutPut::DrawingDISP( sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config)
// {
// 	CStringArray filenames,titleNames;
// 	
// 	filenames.Add(_T("∫„‘ÿŒª“∆Õº.T"));  titleNames.Add(_T("∫„‘ÿŒª“∆Õº"));//DISP-D.T
// 	if (StruCalcInfo().IsHasDiaoGua())
// 	{
// 			filenames.Add(_T("µıπ“∫„‘ÿŒª“∆Õº.T"));  titleNames.Add(_T("µıπ“∫„‘ÿŒª“∆Õº"));//DISP-D.T
// 	}
// 	filenames.Add(_T("ªÓ‘ÿŒª“∆Õº.T"));  titleNames.Add(_T("ªÓ‘ÿŒª“∆Õº"));//DISP-L.T
// 
// 	if (StruCalcInfo().nWindP > 0)
// 	{
// 		filenames.Add(_T("◊Û∑Á1Œª“∆Õº.T")); titleNames.Add(_T("◊Û∑Á1Œª“∆Õº"));//DISP-WL.T
// 		filenames.Add(_T("”“∑Á1Œª“∆Õº.T")); titleNames.Add(_T("”“∑Á1Œª“∆Õº"));//DISP-WR.T
// 	}
// 	if (StruCalcInfo().nWindP > 1)
// 	{
// 		filenames.Add(_T("◊Û∑Á2Œª“∆Õº.T")); titleNames.Add(_T("◊Û∑Á2Œª“∆Õº"));//DISP-WL2.T
// 		filenames.Add(_T("”“∑Á2Œª“∆Õº.T")); titleNames.Add(_T("”“∑Á2Œª“∆Õº"));//DISP-WR2.T
// 	}
// 	if (StruCalcInfo().nWindP > 2)
// 	{
// 		filenames.Add(_T("◊Û∑Á3Œª“∆Õº.T")); titleNames.Add(_T("◊Û∑Á3Œª“∆Õº"));//DISP-WL3.T
// 		filenames.Add(_T("”“∑Á3Œª“∆Õº.T")); titleNames.Add(_T("”“∑Á3Œª“∆Õº"));//DISP-WR3.T
// 	}
// 
// 	if (StruCalcInfo().bHasEarthLR)
// 	{
// 		filenames.Add(_T("◊Ûµÿ’Œª“∆Õº.T")); titleNames.Add(_T("◊Ûµÿ’Œª“∆Õº"));//DISP-EL.T
// 		filenames.Add(_T("”“µÿ’Œª“∆Õº.T")); titleNames.Add(_T("”“µÿ’Œª“∆Õº"));//DISP-ER.T
// 	}
// 	
// 	filenames.Add(_T("∫„+ªÓŒª“∆Õº.T")); titleNames.Add(_T("∫„+ªÓŒª“∆Õº"));//DISP-DL.T
// 
// 	if (StruCalcInfo().nDiaoChe>0)
// 	{
// 		filenames.Add(_T("µı≥µ…≤≥µŒª“∆Õº.T")); titleNames.Add(_T("µı≥µ…≤≥µŒª“∆Õº"));//DISP-TD.T
// 	}
// 
// 	GRAPHIC_FORMAT GFormat;
// 
// 	GFormat.height = 0;
// 	GFormat.width = 0;
// 	GFormat.align = sdAlignCenter;
// 	GFormat.titleFormat.align = GFormat.align;
// 
// 	CString strTitle;
// 	CString strFile;
// 	int nWid = mReportItem.pageWidth-config.graphLayOut.MarginL-config.graphLayOut.MarginR;
//  
// 	for (int i=0;i<filenames.GetSize();i++)
// 	{
// 		strFile =  filenames[i];
// 		strTitle = titleNames[i];
// 		GenDrawingName(mReportItem,strTitle);
// 	 
// 		GFormat.titleFormat.setTitle(strTitle);
// 		DrawingFunc(pGenerateText,mReportItem,strFile,&GFormat,nWid);
// 	}
// 	return true;
// }
