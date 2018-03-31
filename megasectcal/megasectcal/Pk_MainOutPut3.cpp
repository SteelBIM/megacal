#include "stdafx.h"
#include "Pk_MainOutPut.h"
#include "IGenerateText.h"
#include "GReport.h"
#include <atlimage.h>
#include <winerror.h>
#include "CfgSvr.h"
using namespace sd;



bool CPk_MainOutPut::Drawing_Fiber_1(sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config)
{
	CStringArray filenames,titleNames;

	filenames.Add(_T("Fiber_NM_y.t")); titleNames.Add(_T("Fiber_NM_y"));
	filenames.Add(_T("Fiber_NM_z.t")); titleNames.Add(_T("Fiber_NM_z"));	  

	GRAPHIC_FORMAT GFormat;

	GFormat.height = 0;
	GFormat.width = 0;
	GFormat.align = sdAlignCenter;
	GFormat.titleFormat.align = GFormat.align;

	CString strTitle;
	CString strFile;
	int nWid = mReportItem.pageWidth-config.graphLayOut.MarginL-config.graphLayOut.MarginR;
	nWid=150;
	for (int i=0;i<filenames.GetSize();i++)
	{
		strFile =  filenames[i];
		strTitle = titleNames[i];
		GenDrawingName(mReportItem,strTitle  );
		GFormat.height = 0;
		GFormat.width = 0;
		GFormat.titleFormat.setTitle(strTitle);
		DrawingFunc(pGenerateText,mReportItem,strFile,&GFormat,nWid);
	}

	return true;

}
bool CPk_MainOutPut::Drawing_PDM_1(sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config)
{
	CStringArray filenames,titleNames;

	filenames.Add(_T("PDM_NM_y.t")); titleNames.Add(_T("Major Axis"));
	filenames.Add(_T("PDM_NM_z.t")); titleNames.Add(_T("Minor Axis"));	  

	GRAPHIC_FORMAT GFormat;

	GFormat.height = 0;
	GFormat.width = 0;
	GFormat.align = sdAlignCenter;
	GFormat.titleFormat.align = GFormat.align;

	CString strTitle;
	CString strFile;
	int nWid = mReportItem.pageWidth-config.graphLayOut.MarginL-config.graphLayOut.MarginR;
	nWid=150;
	for (int i=0;i<filenames.GetSize();i++)
	{
		strFile =  filenames[i];
		strTitle = titleNames[i];
		GenDrawingName(mReportItem,strTitle  );

		GFormat.titleFormat.setTitle(strTitle);
		DrawingFunc(pGenerateText,mReportItem,strFile,&GFormat,nWid);
	}

	return true;
}

bool CPk_MainOutPut::Drawing_Fiber_2(sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config)
{
	CStringArray filenames,titleNames;

	filenames.Add(_T("Fiber_NM_y_nominal.t")); titleNames.Add(_T("Major Axis(without length effect)"));
	filenames.Add(_T("Fiber_NM_z_nominal.t")); titleNames.Add(_T("Minor Axis(without length effect)"));	  

	GRAPHIC_FORMAT GFormat;

	GFormat.height = 0;
	GFormat.width = 0;
	GFormat.align = sdAlignCenter;
	GFormat.titleFormat.align = GFormat.align;

	CString strTitle;
	CString strFile;
	int nWid = mReportItem.pageWidth-config.graphLayOut.MarginL-config.graphLayOut.MarginR;
	nWid=150;
	for (int i=0;i<filenames.GetSize();i++)
	{
		strFile =  filenames[i];
		strTitle = titleNames[i];
		GenDrawingName(mReportItem,strTitle  );
		GFormat.height = 0;
		GFormat.width = 0;
		GFormat.titleFormat.setTitle(strTitle);
		DrawingFunc(pGenerateText,mReportItem,strFile,&GFormat,nWid);
	}

	return true;

}
bool CPk_MainOutPut::Drawing_PDM_2(sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config)
{
	CStringArray filenames,titleNames;

	filenames.Add(_T("PDM_NM_y_design.t")); titleNames.Add(_T("Major Axis(without length effect)"));
	filenames.Add(_T("PDM_NM_z_design.t")); titleNames.Add(_T("Minor Axis(without length effect)"));	  

	GRAPHIC_FORMAT GFormat;

	GFormat.height = 0;
	GFormat.width = 0;
	GFormat.align = sdAlignCenter;
	GFormat.titleFormat.align = GFormat.align;

	CString strTitle;
	CString strFile;
	int nWid = mReportItem.pageWidth-config.graphLayOut.MarginL-config.graphLayOut.MarginR;
	nWid=150;
	for (int i=0;i<filenames.GetSize();i++)
	{
		strFile =  filenames[i];
		strTitle = titleNames[i];
		GenDrawingName(mReportItem,strTitle  );

		GFormat.titleFormat.setTitle(strTitle);
		DrawingFunc(pGenerateText,mReportItem,strFile,&GFormat,nWid);
	}

	return true;
}

bool CPk_MainOutPut::Drawing_Sect(sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config)
{
	CStringArray filenames,titleNames;

	filenames.Add(_T("temptemp.t")); titleNames.Add(_T("Section"));
//	filenames.Add(_T("PDM_NM_z.t")); titleNames.Add(_T("Minor Axis"));	  

	GRAPHIC_FORMAT GFormat;

	GFormat.height = 0;
	GFormat.width = 0;
	GFormat.align = sdAlignCenter;
	GFormat.titleFormat.align = GFormat.align;

	CString strTitle;
	CString strFile;
	int nWid = mReportItem.pageWidth-config.graphLayOut.MarginL-config.graphLayOut.MarginR;
	nWid=150;
	for (int i=0;i<filenames.GetSize();i++)
	{
		strFile =  filenames[i];
		strTitle = titleNames[i];
		GenDrawingName(mReportItem,strTitle  );

		GFormat.titleFormat.setTitle(strTitle);
		DrawingFunc(pGenerateText,mReportItem,strFile,&GFormat,nWid);
	}

	return true;
}



bool CPk_MainOutPut::GenDrawingName(const sd::GRAPHIC_ITEM& mReportItem,CString& DrawName,bool resetIndex)
{
	static int PicIndex = 0;
	if (resetIndex)
	{
		PicIndex = 0;
	}
	int nCh = mReportItem.chapterSection.level_1;
	CString strTitle ;
	strTitle.Format(_T("Figure %d-%d %s"),nCh+1, ++PicIndex, DrawName);
	DrawName = strTitle;
	return true;
}


void CPk_MainOutPut::DrawingFuncEmbed(sd::IGenerateText* pGenerateText, 
	const sd::GRAPHIC_ITEM& mReportItem,CString fileName,
	PGRAPHIC_FORMAT pFormat,int wid)
{

	fileName.Trim();
	CString strfolder;
	CString strT,strTrans;
	GetCurrentDirectory(MAX_PATH,strfolder.GetBuffer(MAX_PATH));
	strfolder.ReleaseBuffer(-1);
	strfolder+="\\";
	strfolder += pathCalcTemp;
	strfolder+="\\";



	strT = strfolder + fileName;
	strTrans = fileName;
	strTrans.Delete(strTrans.GetLength()-2,2);
	if(FILE_PDF == m_doctype)
	{
		strTrans = strfolder + strTrans + _T(".jpg");
	}
	else
	{
		strTrans = strfolder + strTrans + _T(".emf");
	}


	TEXT_FORMAT titleFormat;
	titleFormat.font = mReportItem.fontGraphicTitle;
	titleFormat.bold = false;
	titleFormat.align = SdAlign(sdAlignHoriMid|sdAlignBottom);

	CFile file;
	if(file.Open(strT,CFile::modeRead))
	{
		file.Close();
		bool nRetTrans = false;
		if(FILE_PDF == m_doctype)
		{
			nRetTrans = GReport::g_pReport->transformTtoJpg(strT,strTrans);
		}
		else
		{
			nRetTrans = GReport::g_pReport->transformTtoEmf(strT,strTrans);
		}
		if(true == nRetTrans)
		{
			if(pFormat->height<=0 || pFormat->width<=0)
			{
				CImage im;
				if(SUCCEEDED(im.Load(strTrans)))
				{
					double sc = im.GetWidth() / (wid * 1.0) ;
					if (fabs(sc) < 0.01 )  
					{
						pFormat->width = im.GetWidth();
						pFormat->height = im.GetHeight();
					}
					else
					{
						int wi = im.GetWidth();
						int hi = im.GetHeight();

						if (DRAWPAGE_HORI == m_drawHoriVeri&&FILE_PDF ==  m_doctype)
						{
							CConfigGenCalc conf;
							conf.readFromFile(CPk_MainOutPut::CalculationReportXml);
							int hi2 = mReportItem.pageHeight - conf.graphLayOut.MarginT-conf.graphLayOut.MarginB;
							pFormat->height = hi2*0.75;

							double sc2 = im.GetHeight() / (hi2 * 1.0) ;
							pFormat->width = int(wi/sc2*1.0);
							pFormat->align = sdAlignHoriMid;
							pGenerateText->embedGraphi(strTrans);
							return;
						}
						else
						{
							pFormat->width = wid  ;
							pFormat->height = int(hi/sc*1.0);
						}


					}
				}
			}

			pGenerateText->embedGraphi(strTrans);

		}
	}


}

void CPk_MainOutPut::DrawingFunc(sd::IGenerateText* pGenerateText, 
	const sd::GRAPHIC_ITEM& mReportItem,CString fileName,
	PGRAPHIC_FORMAT pFormat,int wid,int nPreEmptyLine)
{
	
	fileName.Trim();
	CString strfolder;
	CString strT,strTrans;
	GetCurrentDirectory(MAX_PATH,strfolder.GetBuffer(MAX_PATH));
	strfolder.ReleaseBuffer(-1);
	strfolder+="\\";
 	strfolder += pathCalcTemp;
 	strfolder+="\\";

	

	strT = strfolder + fileName;
	strTrans = fileName;
	strTrans.Delete(strTrans.GetLength()-2,2);
	if(FILE_PDF == m_doctype)
	{
		strTrans = strfolder + strTrans + _T(".jpg");
	}
	else
	{
		strTrans = strfolder + strTrans + _T(".emf");
	}

	
	TEXT_FORMAT titleFormat;
	titleFormat.font = mReportItem.fontGraphicTitle;
	titleFormat.bold = false;
	titleFormat.align = SdAlign(sdAlignHoriMid|sdAlignBottom);
	
	CFile file;
	if(file.Open(strT,CFile::modeRead))
	{
		file.Close();
		bool nRetTrans = false;
		if(FILE_PDF == m_doctype)
		{
			nRetTrans = GReport::g_pReport->transformTtoJpg(strT,strTrans);
		}
		else
		{
			nRetTrans = GReport::g_pReport->transformTtoEmf(strT,strTrans);
		}
		if(true == nRetTrans)
		{

			if(nPreEmptyLine > 0 )
			{
				PARAGRAPH_FORMAT m_ParaghFormat;
				m_ParaghFormat.topSpacing=1;
				m_ParaghFormat.bottomSpacing=1;
				m_ParaghFormat.previousSpacing=2;
				m_ParaghFormat.align=sdAlignLeft;

				pGenerateText->beginParagraph(&m_ParaghFormat);
				for (int i=0;i<nPreEmptyLine;i++)
				{
					pGenerateText->appendText(_T("  "),&titleFormat);
				}
				pGenerateText->endParagraph();
			}

			
			if(pFormat->height<=0 || pFormat->width<=0)
			{
				CImage im;
				if(SUCCEEDED(im.Load(strTrans)))
				{
					double sc = im.GetWidth() / (wid * 1.0) ;
					if (fabs(sc) < 0.01 )  
					{
						pFormat->width = im.GetWidth();
						pFormat->height = im.GetHeight();
					}
					else
					{
						int wi = im.GetWidth();
						int hi = im.GetHeight();
						
						if (DRAWPAGE_HORI == m_drawHoriVeri&&FILE_PDF ==  m_doctype)
						{
							CConfigGenCalc conf;
							conf.readFromFile(CPk_MainOutPut::CalculationReportXml);
							int hi2 = mReportItem.pageHeight - conf.graphLayOut.MarginT-conf.graphLayOut.MarginB;
							pFormat->height = hi2*0.75;

							double sc2 = im.GetHeight() / (hi2 * 1.0) ;
							pFormat->width = int(wi/sc2*1.0);
							pFormat->align = sdAlignHoriMid;
							pGenerateText->insertGraphi(strTrans,pFormat);
							return;
						}
						else
						{
							pFormat->width = wid  ;
							pFormat->height = int(hi/sc*1.0);
						}
		 	
						
					}
				}
			}
 	
			pGenerateText->insertGraphi(strTrans,pFormat);
			
		}
	}
	
	
}

void CPk_MainOutPut::DrawPicJPG(sd::IGenerateText* pGenerateText, 
	const sd::REPORT_ITEM& mReportItem,CString filename,int PathType,int wid  )
{
	filename.Trim();
	CString strfolder;
	CString strT;
	if (PATH_EXE_FOLDER==PathType)
	{
		CfgSvrUser::GetExePath(strfolder);
	}
	else if(PATH_WORKDIR==PathType)
	{
		CfgSvrUser::GetCurrentDirectory(strfolder);
	}
	
	CfgSvrUser::FixPathStr(strfolder);

	filename = strfolder + filename;

	GRAPHIC_FORMAT picFormat;
	PGRAPHIC_FORMAT pFormat = &picFormat;


	CImage im;
	if(SUCCEEDED(im.Load(filename)))
	{
		double sc = im.GetWidth() / (wid * 1.0) ;
		if (fabs(sc) < 0.01 )  
		{
			pFormat->width = im.GetWidth();
			pFormat->height = im.GetHeight();
		}
		else
		{
			int wi = im.GetWidth();
			int hi = im.GetHeight();


			pFormat->width = wid  ;
			pFormat->height = int(hi/sc*1.0);
		}

		pFormat->align = sdAlignHoriMid;

		pGenerateText->insertGraphi(filename,pFormat);
	}
	
}