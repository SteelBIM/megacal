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
 	//XML�ĵ���ȡ
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
 			//����������û�����
 			bRetSelfControl = false;
 			return bRetSelfControl; //�����û�����ֱ�ӷ���
 		}
 		
 	}
 	else
 	{
 		bRetSelfControl = true;
 	}
 
 	//���ݲ�ͬ�½�������ͼ��
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
// 	strTitle.Format(_T("ͼ%d-%d %s"),nCh+1, ++PicIndex, DrawName);
// 	DrawName = strTitle;
// 	return true;
// }
// bool CPk_MainOutPut::DrawingView( sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config)
// {
// 
// 	CStringArray filenames,titleNames;
// 	filenames.Add(_T("�ռܼ�ͼ.T"));  titleNames.Add(_T("�ռܼ�ͼ"));//KLM-CAL.T
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
// 	filenames.Add(_T("���ؼ�ͼ.T"));  titleNames.Add(_T("���ؼ�ͼ"));//D-L-CAL.T
// 	if(StruCalcInfo().IsHasDiaoGua())
// 	{
// 		filenames.Add(_T("���Һ��ؼ�ͼ.T"));   titleNames.Add(_T("���Һ��ؼ�ͼ"));//DG-D-L-CAL.T
// 	}
// 
// 
// 	if (1 == g_Param_MainParam.KLL )
// 	{
// 		filenames.Add(_T("���ؼ�ͼ.T")); titleNames.Add(_T("���ؼ�ͼ"));//L-L-CAL.T
// 	}
// 	else if (g_Param_MainParam.KLL >1)
// 	{
// 		CString strfile;
// 		CString strtitle;
// 		for (int i=0;i<g_Param_MainParam.KLL;i++)
// 		{
// 			strfile.Format(_T("��%d�黥�����ؼ�ͼ.T"),i+1);//L-L%d-CAL.T
// 			if (0 == i)
// 			{
// 				strtitle = _T("���ؼ�ͼ");
// 			}
// 			else
// 			{
// 				strtitle.Format(_T("��%d�黥�����ؼ�ͼ"),i);//
// 			}
// 			filenames.Add(strfile); titleNames.Add(strtitle);
// 		}
// 
// 	}
// 
// 	//�Զ�����ؼ�ͼ
// 	{
// 		int UserZiDingYi = CmdDataMgr::Get().NZiDingYi();
// 		const TCHAR * formatTitle = "%s���ؼ�ͼ";
// 		const TCHAR * formatFname = "%s���ؼ�ͼ.T";
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
// 		filenames.Add(_T("���1��ͼ.T"));titleNames.Add(_T("���1��ͼ"));//L-W-1-CAL.T
// 		filenames.Add(_T("�ҷ�1��ͼ.T"));titleNames.Add(_T("�ҷ�1��ͼ"));//R-W-1-CAL.T
// 	}
// 	
// 	if (StruCalcInfo().nWindP > 1)
// 	{
// 		filenames.Add(_T("���2��ͼ.T"));titleNames.Add(_T("���2��ͼ"));//L-W-2-CAL.T
// 		filenames.Add(_T("�ҷ�2��ͼ.T"));titleNames.Add(_T("�ҷ�2��ͼ"));//R-W-2-CAL.T
// 	}
// 	
// 	if (StruCalcInfo().nWindP > 2)
// 	{
// 		filenames.Add(_T("���3��ͼ.T"));titleNames.Add(_T("���3��ͼ"));//L-W-3-CAL.T
// 		filenames.Add(_T("�ҷ�3��ͼ.T"));titleNames.Add(_T("�ҷ�3��ͼ"));//R-W-3-CAL.T
// 	}
// 	if (StruCalcInfo().nDiaoChe>0)
// 	{
// 		filenames.Add(_T("�������ؼ�ͼ.T"));titleNames.Add(_T("�������ؼ�ͼ"));//C-H-CAL
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
// 	filenames.Add(_T("�������ͼ.T"));titleNames.Add( _T("�������ͼ"));//D-M.T
// 	filenames.Add(_T("���ؼ���ͼ.T"));titleNames.Add( _T("���ؼ���ͼ"));//D-V.T
// 	filenames.Add(_T("��������ͼ.T"));titleNames.Add( _T("��������ͼ"));//D-N.T
// 	
// 	filenames.Add(_T("�������ͼ.T"));titleNames.Add( _T("�������ͼ"));//L-M.T
// 	filenames.Add(_T("���ؼ���ͼ.T"));titleNames.Add( _T("���ؼ���ͼ"));//L-V.T
// 	filenames.Add(_T("��������ͼ.T"));titleNames.Add( _T("��������ͼ"));//L-N.T
// 	
// 	if (StruCalcInfo().IsHasDiaoGua())
// 	{
// 		filenames.Add(_T("���Һ������ͼ.T"));titleNames.Add( _T("���Һ������ͼ"));//DG-M.T
// 		filenames.Add(_T("���Һ�������ͼ.T"));titleNames.Add( _T("���Һ�������ͼ"));//DG-N.T
// 		filenames.Add(_T("���Һ��ؼ���ͼ.T"));titleNames.Add( _T("���Һ��ؼ���ͼ"));//DG-V.T
// 	}
// 
// 	if (StruCalcInfo().nWindP > 0)
// 	{
// 		filenames.Add(_T("���1���ͼ.T"));titleNames.Add( _T("���1���ͼ"));//WL.T
// 		filenames.Add(_T("�ҷ�1���ͼ.T"));titleNames.Add( _T("�ҷ�1���ͼ"));//WR.T
// 	}
// 	if (StruCalcInfo().nWindP > 1)
// 	{
// 		filenames.Add(_T("���2���ͼ.T"));titleNames.Add( _T("���2���ͼ"));//WL2.T
// 		filenames.Add(_T("�ҷ�2���ͼ.T"));titleNames.Add( _T("�ҷ�2���ͼ"));//WR2.T
// 	}
// 	if (StruCalcInfo().nWindP > 2)
// 	{
// 		filenames.Add(_T("���3���ͼ.T"));titleNames.Add( _T("���3���ͼ"));//WL3.T
// 		filenames.Add(_T("�ҷ�3���ͼ.T"));titleNames.Add( _T("�ҷ�3���ͼ"));//WR3.T
// 	}
// 
// 	filenames.Add(_T("��������ͼ.T"));titleNames.Add( _T("��������ͼ"));//EL.T
// 	filenames.Add(_T("�ҵ������ͼ.T"));titleNames.Add( _T("�ҵ������ͼ"));//ER.T
// 
// 
// 
// 
// 	//�Զ�����ؼ���ͼ
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
// 	filenames.Add(_T("��ذ���ͼ.T")); titleNames.Add(_T("��ذ���ͼ"));	 //M.T
// 	filenames.Add(_T("��������ͼ.T")); titleNames.Add(_T("��������ͼ"));	 //Q.T
// 	filenames.Add(_T("��������ͼ.T")); titleNames.Add(_T("��������ͼ"));	 //N.T
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
// 	filenames.Add(_T("Ӧ����ͼ.T")); titleNames.Add(_T("Ӧ����ͼ"));	 //N.T
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
// 	filenames.Add(_T("����λ��ͼ.T"));  titleNames.Add(_T("����λ��ͼ"));//DISP-D.T
// 	if (StruCalcInfo().IsHasDiaoGua())
// 	{
// 			filenames.Add(_T("���Һ���λ��ͼ.T"));  titleNames.Add(_T("���Һ���λ��ͼ"));//DISP-D.T
// 	}
// 	filenames.Add(_T("����λ��ͼ.T"));  titleNames.Add(_T("����λ��ͼ"));//DISP-L.T
// 
// 	if (StruCalcInfo().nWindP > 0)
// 	{
// 		filenames.Add(_T("���1λ��ͼ.T")); titleNames.Add(_T("���1λ��ͼ"));//DISP-WL.T
// 		filenames.Add(_T("�ҷ�1λ��ͼ.T")); titleNames.Add(_T("�ҷ�1λ��ͼ"));//DISP-WR.T
// 	}
// 	if (StruCalcInfo().nWindP > 1)
// 	{
// 		filenames.Add(_T("���2λ��ͼ.T")); titleNames.Add(_T("���2λ��ͼ"));//DISP-WL2.T
// 		filenames.Add(_T("�ҷ�2λ��ͼ.T")); titleNames.Add(_T("�ҷ�2λ��ͼ"));//DISP-WR2.T
// 	}
// 	if (StruCalcInfo().nWindP > 2)
// 	{
// 		filenames.Add(_T("���3λ��ͼ.T")); titleNames.Add(_T("���3λ��ͼ"));//DISP-WL3.T
// 		filenames.Add(_T("�ҷ�3λ��ͼ.T")); titleNames.Add(_T("�ҷ�3λ��ͼ"));//DISP-WR3.T
// 	}
// 
// 	if (StruCalcInfo().bHasEarthLR)
// 	{
// 		filenames.Add(_T("�����λ��ͼ.T")); titleNames.Add(_T("�����λ��ͼ"));//DISP-EL.T
// 		filenames.Add(_T("�ҵ���λ��ͼ.T")); titleNames.Add(_T("�ҵ���λ��ͼ"));//DISP-ER.T
// 	}
// 	
// 	filenames.Add(_T("��+��λ��ͼ.T")); titleNames.Add(_T("��+��λ��ͼ"));//DISP-DL.T
// 
// 	if (StruCalcInfo().nDiaoChe>0)
// 	{
// 		filenames.Add(_T("����ɲ��λ��ͼ.T")); titleNames.Add(_T("����ɲ��λ��ͼ"));//DISP-TD.T
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
