#include "StdAfx.h"
#include "Pk_MainOutPut.h"
#include "IGenerateText.h"
#include "ParagraphMacroHeader.h"
//#include "Param_MainParam.h"
//#include "StructBaseInfo.h"
#include "GReport.h"

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <atlimage.h>
#include "ConfigGen.h"
#include "GPara.h"
#include "SectProperty.h"
#include "Tools.h"
#include "GlobalResult.h"
#include "Force_combine.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Loaddef.h"

const TCHAR* CPk_MainOutPut::pathCalcTemp = _T("CalcTemp");

using namespace sd;


TextTool CPk_MainOutPut::stt;
///////////////////////////////////////////////////////
CPk_MainOutPut::CPk_MainOutPut(void)
{
	 
}

CPk_MainOutPut::~CPk_MainOutPut(void)
{
}


int CPk_MainOutPut::m_doctype = FILE_WORD;
int CPk_MainOutPut::m_drawHoriVeri = DRAWPAGE_NULL;


bool CPk_MainOutPut::initReportTool()
{
	GReport::init();  //加载计算书dll
	ClearCalcTemp();  //工作目录 CalcTemp文件夹内容清理
	prepareT();
	return true;
}
const  TCHAR* CPk_MainOutPut::configXmlPrefix = _T("MjCalcReport");
CString CPk_MainOutPut::configXml = _T("");
TCHAR* CPk_MainOutPut::wordFileName = _T("CalculationSheet");
TCHAR* CPk_MainOutPut::CalculationReportXml = _T("CalculationReport.xml");
bool CPk_MainOutPut::dlgConfig()
{

	if(GReport::g_pReport)
	{
		CString pathWork;
		{
			::GetCurrentDirectory(MAX_PATH,pathWork.GetBuffer(MAX_PATH));
			pathWork.ReleaseBuffer(-1);
			pathWork+= _T("\\");
		}

		CString strConfig = configXml;
		GReport::GetReprotXmlFullPathname(strConfig);
		GReport::g_pReport->setConfigXml(strConfig);
		GReport::g_pReport->registeReportCallback(outputRoutine);
		GReport::g_pReport->registeGraphicCallback(drawingRoutine);

		//GReport::g_pReport->registeOnClickItemCallback(clickIconRoutine);

		SdReportSetupOption eSetupOpt;
		eSetupOpt=sdReportSetupOpt_All;

		SdReportBuildOption eBuildOpt;
		//eBuildOpt=sdReportBuildOpt_Default;
		eBuildOpt = (SdReportBuildOption)( sdReportBuildOpt_TitleAlignLeft | sdReportBuildOpt_Pdf_VectorGraphic);
		CString pathOutFile = pathWork + wordFileName;

		/*GReport::g_pReport->setupOption(eSetupOpt,eBuildOpt,pathOutFile);*/
		GReport::g_pReport->buildReport(eBuildOpt);

		CString strFi = wordFileName ;
		strFi+= ".docx";

		DeleteFile(strFi);
		rename("计算书.docx",strFi);
		strFi = pathWork + strFi;

		ShellExecute(NULL,"open",strFi,NULL,NULL,SW_NORMAL);

	}

	return true;
}

bool CPk_MainOutPut::prepareT()
{
	CopyToCalcTemp(_T("Fiber_NM_y.t"),_T("Fiber_NM_y.t"));
	CopyToCalcTemp(_T("Fiber_NM_z.t"),_T("Fiber_NM_z.t"));
	CopyToCalcTemp(_T("PDM_NM_y.t"),_T("PDM_NM_y.t"));
	CopyToCalcTemp(_T("PDM_NM_z.t"),_T("PDM_NM_z.t"));
	CopyToCalcTemp(_T("Fiber_NM_y_nominal.t"),_T("Fiber_NM_y_nominal.t"));
	CopyToCalcTemp(_T("Fiber_NM_z_nominal.t"),_T("Fiber_NM_z_nominal.t"));
	CopyToCalcTemp(_T("PDM_NM_y_design.t"),_T("PDM_NM_y_design.t"));
	CopyToCalcTemp(_T("PDM_NM_z_design.t"),_T("PDM_NM_z_design.t"));
	CopyToCalcTemp(_T("temptemp.t"),_T("temptemp.t"));
	return true;
}
bool CPk_MainOutPut::outputRoutine( sd::IGenerateText* pGenerateText,const sd::REPORT_ITEM& mReportItem)
{
	m_doctype =  GReport::g_pReport->getCurrentFileType();

	{
		PARAGRAPH_FORMAT ParaghFormat;
		PPARAGRAPH_FORMAT tF=&ParaghFormat;
		ParaghFormat.topSpacing=1;
		ParaghFormat.bottomSpacing=1;
		ParaghFormat.previousSpacing=2;
		ParaghFormat.align=sdAlignLeft;

		TEXT_FORMAT tf;
		tf.font = mReportItem.fontContent;

		stt.Set(pGenerateText,&ParaghFormat);
		stt.SetTEXT_FORMAT(tf);
		stt.readJson();
	}


	// 	{
	// 		DesnPara *pDesnPara =  GPara::m_DesnPara;
	// 		SectDescribe * pParaSectIn =GPara::m_Sect_in;
	// 		if (pParaSectIn==NULL||pDesnPara==NULL)
	// 		{
	// 			return true;
	// 		}
	// 	}

	switch(mReportItem.key)
	{
		
	case STS_MEGA_GENERAL_PARAMETERS:
		CPk_MainOutPut::GENERAL_PARAMETERS(pGenerateText,mReportItem);
		break;
	case STS_MEGA_CODES:
		CPk_MainOutPut::CODE_STANDARD(pGenerateText,mReportItem);
		break;
	case STS_MEGA_CROSS_SECTION:
		CPk_MainOutPut::CROSS_SECTION(pGenerateText,mReportItem);
		break;
	case STS_MEGA_Material_properties:
		CPk_MainOutPut::Material_properties(pGenerateText,mReportItem);
		break;
	case STS_MEGA_Creep_coefficient_concrete:
		CPk_MainOutPut::Creep_coefficient_concrete(pGenerateText,mReportItem);
		break;
	case STS_MEGA_Load_cases:
		CPk_MainOutPut::Load_cases(pGenerateText,mReportItem);
		break;
	case STS_MEGA_Calculation_parameters:
		CPk_MainOutPut::Calculation_parameters(pGenerateText,mReportItem);
		break;
	case STS_MEGA_Loadinfo:
		CPk_MainOutPut::Load_case(pGenerateText,mReportItem);
		break;
	case STS_MEGA_Load_combinations:
		CPk_MainOutPut::Load_combinations(pGenerateText,mReportItem);
		break;
	case STS_MEGA_ULTIMATE_LIMIT_STATES:
		ULTIMATE_LIMIT_STATES(pGenerateText,mReportItem);
		break;
	case STS_MEGA_RESISTANCE_OF_THE_CROSS_SECTION_AXIAL:
		RESISTANCE_OF_THE_CROSS_SECTION_AXIAL(pGenerateText,mReportItem);
		break;
	case STS_MEGA_RESISTANCE_OF_THE_CROSS_SECTION_TO_SHEAR_RESISTANCE:
		RESISTANCE_OF_THE_CROSS_SECTION_TO_SHEAR_RESISTANCE(pGenerateText,mReportItem);
		break;
	case STS_MEGA_RESISTANCE_OF_THE_CROSS_SECTION_TO_COMBINED_ACTIONS_M_N:
		RESISTANCE_OF_THE_CROSS_SECTION_TO_COMBINED_ACTIONS_M_N(pGenerateText,mReportItem);
		break;
	case STS_MEGA_RESISTANCE_OF_THE_CROSS_SECTION_TO_COMBINED_ACTIONS_M_N2:
		RESISTANCE_OF_THE_CROSS_SECTION_TO_COMBINED_ACTIONS_M_N(pGenerateText,mReportItem);
		break;
	case STS_MEGA_BUCKLING_RESISTANCE_TO_AXIAL_FORCE:
		BUCKLING_RESISTANCE_TO_AXIAL_FORCE(pGenerateText,mReportItem);
		break;
	case STS_MEGA_AMPLIFICATION_OF_DESIGN_MOMENT_OF_COMBINED_ACTION_M_N_CONSIDERING:
		AMPLIFICATION_OF_DESIGN_MOMENT_OF_COMBINED_ACTION_M_N_CONSIDERING(pGenerateText,mReportItem);
		break;
	case STS_MEGA_INTERFACE_SHEAR_CONNECTION:
		INTERFACE_SHEAR_CONNECTION(pGenerateText,mReportItem);
		break;
	case STS_MEGA_SYNTHESIS_OF_VERIFICATIONS:
		SYNTHESIS_OF_VERIFICATIONS(pGenerateText,mReportItem);
		break;
	case STS_MEGA_SYNTHESIS_OF_VERIFICATIONS2:
		SYNTHESIS_OF_VERIFICATIONS_CUSTOM(pGenerateText,mReportItem);
		break;

	case STS_MEGA_SERVICEABILITY_LIMIT_STATES:
//		SERVICEABILITY_LIMIT_STATES(pGenerateText,mReportItem);
		break;
	case STS_MEGA_INTERNAL_FORCES:
//		INTERNAL_FORCES(pGenerateText,mReportItem);
		break;
	case STS_MEGA_CRACK_CHECK:
		CRACK_CHECK(pGenerateText,mReportItem);
		break;
	case STS_MEGA_MEMBER_CHECK1:
		MEMBER_CHECK1(pGenerateText,mReportItem);
		break;
	case STS_MEGA_MEMBER_CHECK2:
		//MEMBER_CHECK2(pGenerateText,mReportItem);
		break;
	case STS_MEGA_MEMBER_CHECK3:
		//MEMBER_CHECK3(pGenerateText,mReportItem);
		break;
	case STS_MEGA_MEMBER_CHECK4:
		break;
		
	default:
		return false;
	}

	return true;
}

 

bool CPk_MainOutPut::GENERAL_PARAMETERS(sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem )
{

	DesnPara*  pPara =GPara::m_DesnPara;
	if (pPara==NULL)
	{
		return false;
	}
 
	stt.BeginP();
	stt.Txt(101,10);stt.Txt(101,20);
	stt.EndP();

	stt.BeginP();
	stt.Txt(101,40); stt.m(pPara->Lens);
	stt.EndP();

 
	stt.BeginP();
	if(pPara->Restrict==3)
		stt.Txt(101,50);
	else if(pPara->Restrict==4)
		stt.Txt(101,60);
	stt.EndP();
 
// 	stt.BeginP();
// 	stt.Txt(101,70);
// 	stt.EndP();

	stt.BeginP();
	stt.Txt(101,80);stt.TxSub("ρ","c","=");stt.kg$m(pPara->m_Conp,1);
	stt.EndP();
	 

	stt.BeginP();
	stt.Txt(101,100);stt.TxSub("ρ","s","="); stt.kg$m(pPara->m_Density,1);
	stt.EndP();
	 
	stt.BeginP();
	stt.Txt(101,110);stt.Txt("g = "); stt.m$s2(pPara->m_Gravity);
	stt.EndP();



	{

// 		jpgname = "ProBengou.jpg";	
// 		DrawPicJPG(pGenerateText,mReportItem,jpgname,PATH_WORKDIR);
// 
// 		jpgname = "ConBengou.jpg";	
// 		DrawPicJPG(pGenerateText,mReportItem,jpgname,PATH_WORKDIR);
	}


	return true;
}

bool CPk_MainOutPut::CROSS_SECTION(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{

	DesnPara *pDesnPara =  GPara::m_DesnPara;
	SectDescribe * pParaSectIn =GPara::m_Sect_in;
	if (pParaSectIn==NULL||pDesnPara==NULL)
	{
		return false;
	}

	SectProperty m_sect;
	m_sect.SectGeoInfo();
	m_sect.SectCal();


	

	 
	stt.BeginP();
	stt.Txt(102,10);//"◆ Concrete section’s properties are:"
	stt.EndP();
 
	stt.BeginP();
	stt.TxSub("h","1"," = ");stt.mm(pParaSectIn->Sect_h);stt.SpTab______();
	stt.TxSub("h","2"," = ");stt.mm(pParaSectIn->Sect_h);
	stt.EndP();
	
	
	stt.BeginP();
	stt.TxSub("C","x"," = ");;stt.mm(pParaSectIn->HSect_x); stt.SpTab______();
	stt.TxSub("C","y"," = ");;stt.mm(pParaSectIn->HSect_y);
	stt.EndP();


	stt.BeginP();
	stt.TxSub("A","g"," = ");stt.mm(m_sect.Ag);
	stt.EndP();


	stt.BeginP();
	stt.Txt(102,20);//"Steel profile properties are:"
	stt.EndP();
 

	stt.BeginP();
	stt.Txt(103,11);stt.Txt(pParaSectIn->HSect.NAME);
	stt.EndP();

	stt.BeginP();
	stt.Txt("b = ");stt.mm(pParaSectIn->HSect.B1);stt.SpTab______();
	stt.Txt("d = ");stt.mm(pParaSectIn->HSect.H1);
	stt.EndP();


	stt.BeginP();
	stt.TxSub("t","w"," = ");stt.mm(pParaSectIn->HSect.RJ);stt.SpTab______();
	stt.TxSub("t","f"," = ");stt.mm(pParaSectIn->HSect.RI);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Z","sx"," = ");stt.mm(m_sect.Zsx);stt.SpTab______();
	stt.TxSub("Z","sy"," = ");stt.mm(m_sect.Zsy);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("I","HDx"," = ");stt.mm4(m_sect.Isx);stt.SpTab______();
	stt.TxSub("I","HDy"," = ");stt.mm4(m_sect.Isy);
	stt.EndP();


	stt.BeginP();
	//	stt.TxSub("A","a"," = "); stt.mm2(m_sect.As);stt.SpTab______();
	stt.TxSub("A","s"," = ");stt.mm2(m_sect.As);
	stt.EndP();


	stt.BeginP();
	stt.TxSub("d","sx"," = "); stt.mm(pParaSectIn->HSect_x);
	stt.mm();

	stt.SpTab______();
	stt.TxSub("d","sy"," = ");stt.mm(pParaSectIn->HSect_y);
	
	stt.EndP();


	///////////////////////////////////////////////////////
	stt.BeginP();
	stt.Txt(102,40);//"◆ Reinforcement properties are:"
	stt.EndP();

	stt.BeginP();
	stt.Txt(102,50);//Total number of vertical rebars is
	// numbers
	stt.TxV(224);
	stt.Txt(". ");
	stt.EndP();

	stt.BeginP();
	stt.Txt(102,60);
	stt.TxSub("d","b"," = ");stt.mm(pParaSectIn->Bar_d);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("A","sri"," = ");stt.mm2(m_sect.Asr);
	stt.EndP();

// 	stt.BeginP();
// 	stt.Txt(102,90);
// 	// rou
// 	stt.TxV(0.01);
// 	stt.EndP();

	// 	stt.BeginP();
	// 	stt.Txt(102,50);	//"spacing of the transverse reinforcement:"
	// 	stt.mm(1010.f); 
	// 	stt.EndP();
 
	return true;
}
bool CPk_MainOutPut::Material_properties(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	//	return false;
	DesnPara *pDesnPara =  GPara::m_DesnPara;
	SectDescribe * pParaSectIn =GPara::m_Sect_in;

	if (pParaSectIn==NULL||pDesnPara==NULL)
	{
		return false;
	}

	SectProperty m_sect;
	m_sect.SectGeoInfo();
	m_sect.SectCal();


	if (pParaSectIn==NULL||pDesnPara==NULL)
	{
		return false;
	}


	CString oneStr;

	stt.BeginP();
	stt.Txt(103,10);//"◆ Steel properties"
	stt.EndP();

	//钢材类型
	CString str1,str2;
	stt.BeginP();
	if(pDesnPara->ProType==0){
		str1.Format("EN10025");
		if(pDesnPara->ProLevel==0)
			str2.Format("S355JR/JO/J2/K2");
		else if(pDesnPara->ProLevel==1)
			str2.Format("S460JR/JO/J2/K2");
		else if(pDesnPara->ProLevel==2)
			str2.Format("S500JO");
		else if(pDesnPara->ProLevel==3)
			str2.Format("S355M/ML");
		else if(pDesnPara->ProLevel==4)
			str2.Format("S420M/ML");
		else if(pDesnPara->ProLevel==5)
			str2.Format("S460M/ML");
		else
			str2.Format("S500M/ML");
	}
	else if(pDesnPara->ProType==1){
		str1.Format("ETA-10/0156");
		if(pDesnPara->ProLevel==0)
			str2.Format("Histar355");
		else
			str2.Format("Histar460");
	}
	else if(pDesnPara->ProType==2){
		str1.Format("ASTM");
		if(pDesnPara->ProLevel==0)
			str2.Format("A913Gr50");
		else if(pDesnPara->ProLevel==1)
			str2.Format("A913Gr65");
		else if(pDesnPara->ProLevel==2)
			str2.Format("A913Gr70");
		else
			str2.Format("A913Gr80");
	}
	else if(pDesnPara->ProType==3){
		str1.Format("Chinese");
		if(pDesnPara->ProLevel==0)
			str2.Format("Q235");
		else if(pDesnPara->ProLevel==1)
			str2.Format("Q345");
		else if(pDesnPara->ProLevel==2)
			str2.Format("Q345GJ");
		else if(pDesnPara->ProLevel==3)
			str2.Format("Q390");
		else if(pDesnPara->ProLevel==4)
			str2.Format("Q420");
	}
	str1 = str1+" "+str2;
	stt.Txt(str1);
	//	stt.Txt(103,20);//ASTM A913- 11 Grade 65
	stt.EndP();
	//end

	stt.BeginP();
	float fy1 = Tools::GetProFy(pDesnPara->ProType,pParaSectIn->HSect.RI,pDesnPara->ProLevel);
	float fy2 = Tools::GetProFy(pDesnPara->ProType,pParaSectIn->HSect.RJ,pDesnPara->ProLevel);
	stt.TxSub("f","y(flange)"," = ");stt.MPa(fy1);stt.SpTab______();
	stt.TxSub("f","y(web)"," = ");stt.MPa(fy2);stt.SpTab______();	
	stt.TxSub("E","s"," = ");stt.MPa(m_sect.Es);
	stt.EndP();

	//ZHLEI 2018/03 :构图纸
	if(GPara::ifExpertMode){
		if(GPara::m_DesnPara->ProType==4)
			DrawPicJPG(pGenerateText,mReportItem,"ProBengou.jpg",PATH_WORKDIR,100);
	}


	///////////////////////////////////////////////////////
	stt.BeginP();
	stt.Txt(103,40);//◆  Concrete properties"
	stt.EndP();

	stt.BeginP();
	stt.Txt(103,50); stt.C(GPara::m_Sect_in->Concrete_mat); //Concrete class
	stt.EndP();

	stt.BeginP();
	if(GPara::m_DesnPara->CodeType==1){	//欧洲规范
		int fck = GPara::m_Sect_in->Concrete_mat;
		float fcm,Ecm,fctm;
		Tools::GetConPara_EURO(fck,fcm,Ecm,fctm);
		stt.TxSub("f","ck"," = ");stt.MPa(fck);stt.SpTab______();
		stt.TxSub("f","cm"," = ");stt.MPa(fcm);stt.SpTab______();
		stt.TxSub("E","cm"," = ");stt.MPa(Ecm);stt.SpTab______();
		stt.TxSub("f","ctm"," = ");stt.MPa(fctm,2);stt.SpTab______();
	}
	else if(GPara::m_DesnPara->CodeType==2){	//美国规范

	}
	else if(GPara::m_DesnPara->CodeType==3){		//中国规范

	}
	stt.EndP();

	stt.BeginP();
	CString sc[6]={"S1","S2","S3","S4","S5","S6"};
	stt.Txt(103,60); stt.Txt(sc[GPara::m_DesnPara->Struc_Class]); //Structural class :
	stt.EndP();

	stt.BeginP();
	CString ec[11]={"X0","XC1","XC2","XC3","XC4","XD1","XD2","XD3","XS1","XS2","XS3"};
	stt.Txt(103,70); stt.Txt(ec[GPara::m_DesnPara->Exp_Class]); //"Exposure class :"
	stt.EndP();
	 
	stt.BeginP();
	stt.Txt(103,80);  //"Nominal cover : 
	stt.EndP();

	stt.BeginP();
	stt.TxSub("C","s,y"," = ");stt.mm(GPara::m_Sect_in->Con_coverd_x);stt.SpTab______();
	stt.TxSub("C","s,z"," = ");stt.mm(GPara::m_Sect_in->Con_coverd_y);
	stt.EndP();

	if(GPara::ifExpertMode){
		if(GPara::m_DesnPara->con_model==3)
			DrawPicJPG(pGenerateText,mReportItem,"CBengou.jpg",PATH_WORKDIR,100);
	}
	///////////////////////////////////////////////////////
	stt.BeginP();
	stt.Txt(103,100);//"◆\tReinforcement properties"
	stt.EndP();

	stt.BeginP();
	stt.TxSub("f","s"," = ");stt.MPa(GPara::m_Sect_in->Bar_mat);stt.SpTab______();
	stt.TxSub("E","sr"," = ");stt.MPa(m_sect.Es);
	stt.EndP();

	return true;
}
bool CPk_MainOutPut::Creep_coefficient_concrete(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	DesnPara *pDesnPara =  GPara::m_DesnPara;
	SectDescribe * pParaSectIn =GPara::m_Sect_in;
	if (pParaSectIn==NULL||pDesnPara==NULL)
	{
		return false;
	}

	SectProperty m_sect;
	m_sect.SectGeoInfo();
	m_sect.SectCal();

	stt.BeginP();
	stt.Txt(104,10);"Concrete cross-section area:";
	stt.TxSub("A","c"," = ");stt.mm2(m_sect.Ac);
	stt.EndP();

	stt.BeginP();
	stt.Txt(104,30);//Perimeter of the member in contact with the atmosphere:
	stt.Txt("u = ");stt.mm(GlobalResult::con_u);
	stt.EndP();

	stt.BeginP();
	stt.Txt(104,50);stt.TxSub("h","0","=");stt.mm(GlobalResult::con_h0);//"Notional size of the member : "
	stt.EndP();

	stt.BeginP();
	stt.Txt(104,70);stt.TxSub("t","0","=");stt.days(GPara::m_DesnPara->day_t0);//"Age of concrete at loading:  "
	stt.EndP();

	stt.BeginP();
	stt.Txt(104,90);stt.Txt("RH=");stt.TxV(GPara::m_DesnPara->RH,1);stt.Txt("%");//"Relative humidity of the ambient environment:   "
	stt.EndP();

	stt.BeginP();
	stt.Txt(104,110);stt.TxSub("φ","0"," = ");stt.TxV(GlobalResult::fai0,3);//"Notional creep coefficient:"
	stt.EndP();

	stt.BeginP();
	stt.Txt(104,130);//"Coefficient to describe the development of creep with time after loading:
	stt.TxSub("β","c"); stt.TxSub("(t,t", "0" , ")=" ); stt.TxV(1.0,1);
	stt.EndP();

	stt.BeginP();
	stt.Txt(104,150);
	stt.TxSub("φ(t,t", "0" , ")=" );stt.TxV(GlobalResult::fai0,3);
	stt.EndP();

	return true;
}
bool CPk_MainOutPut::Load_cases(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	DesnPara *pDesnPara =  GPara::m_DesnPara;
	SectDescribe * pParaSectIn =GPara::m_Sect_in;
	if (pParaSectIn==NULL||pDesnPara==NULL)
	{
		return false;
	}


	int nCaseCount = pDesnPara->nLoads_y;


	CString strLoadCasePre;
	strLoadCasePre = "◆  Load case";

	for(int i=0;i<nCaseCount;i++)
	{
		if (i==0)
		{
			stt.BeginP();
			stt.Txt(strLoadCasePre);
			CString loadname;
			loadname.Format(_T("1: Permanent loads (%s)"),GPara::m_DesnPara->notation[0]);
			stt.Txt(loadname);
			stt.EndP();
		}
		else
		{
			stt.BeginP();
			stt.Txt(strLoadCasePre);
			stt.TxV(i+1);
			CString loadname;
			loadname.Format(_T(": Live loads (%s) "),GPara::m_DesnPara->notation[i]);
			stt.Txt(loadname);
			stt.EndP();
		}
		
		LoadCaseTable(pGenerateText,mReportItem,i);
	}
	


	return true;
}
bool CPk_MainOutPut::Calculation_parameters(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	DesnPara *pDesnPara =  GPara::m_DesnPara;
	SectDescribe * pParaSectIn =GPara::m_Sect_in;
	if (pParaSectIn==NULL||pDesnPara==NULL)
	{
		return false;
	}

	stt.BeginP();
	stt.Txt(106,10);stt.TxSub("γ","G,sup"," = ");stt.TxV(GPara::m_DesnPara->rGsup,2);stt.Txt(",");stt.TxSub("γ","G,inf"," = ");stt.TxV(GPara::m_DesnPara->rGinf,2);
	if(GPara::m_DesnPara->CodeType==3){	//中国规范
		stt.Txt(",");stt.TxSub("γ","G,inf2"," = ");stt.TxV(GPara::m_DesnPara->rGinf2,2);
	}
	stt.Txt(",");stt.TxSub("γ","Q1"," = ");stt.TxV(GPara::m_DesnPara->rQ1,2);stt.Txt(",");stt.TxSub("γ","Q2"," = ");stt.TxV(GPara::m_DesnPara->rQ2,2);
	stt.EndP();
	//Partial factors on loads : 

	stt.BeginP();
	stt.Txt(106,30);stt.TxSub("γ","M0"," = ");stt.TxV(GPara::m_DesnPara->gama0,2);stt.Txt(",");stt.TxSub("γ","M1"," = ");stt.TxV(GPara::m_DesnPara->gama1,2);
	stt.EndP();

	stt.BeginP();
	stt.Txt(106,40);stt.TxSub("γ","M0"," ");
	stt.EndP();

	stt.BeginP();
	stt.Txt(106,41);stt.TxSub("γ","M1"," ");
	stt.EndP();

	stt.BeginP();
	stt.Txt(106,50);stt.TxSub("γ","s"," = ");
	// γs
	stt.TxV(GPara::m_DesnPara->rs,2); stt.Txt(", ");
	stt.TxSub("γ","c"," = ");
	// γc
	stt.TxV(GPara::m_DesnPara->rc,2); stt.Txt(", ");
// 	stt.TxSub("γ","a"," = ");
// 	// γa
// 	stt.TxV(GPara::m_DesnPara->gama1,2); 
	stt.EndP();

	stt.BeginP();
	stt.Txt(106,70);
	if(GPara::m_DesnPara->ifFiber)
		stt.Txt("Fiber Model");
	else
		stt.Txt("PDM calculation");
	stt.EndP();

	stt.BeginP();
	stt.Txt(106,90);
	stt.EndP();

	stt.BeginP();
	stt.Txt(106,100);
	stt.EndP();

	return true;
}

bool CPk_MainOutPut::Load_combinations(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	//后面慢慢写
	LoadCombTable(pGenerateText,mReportItem);

	stt.BeginP();
	stt.Txt(106,110);
	stt.EndP();
	return true;
}

bool CPk_MainOutPut::INTERNAL_FORCES(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	// 第2章 内力


	return true;
}

bool CPk_MainOutPut::ULTIMATE_LIMIT_STATES(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	return true;
}
bool CPk_MainOutPut::RESISTANCE_OF_THE_CROSS_SECTION_AXIAL(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	stt.BeginP();
	stt.TxSub("N","pl.Rd"," = ");stt.kN(GPara::NplRd*0.001);
	stt.EndP();
	return true;
}
bool CPk_MainOutPut::RESISTANCE_OF_THE_CROSS_SECTION_TO_SHEAR_RESISTANCE(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	stt.BeginP();
	stt.TxSub("V","pl.Rd,y"," = ");stt.kN(GPara::VplRd_y*0.001);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("V","pl.Rd,z"," = ");stt.kN(GPara::VplRd_z*0.001);
	stt.EndP();
	return true;
}
bool CPk_MainOutPut::RESISTANCE_OF_THE_CROSS_SECTION_TO_COMBINED_ACTIONS_M_N(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	DesnPara *pDesnPara =  GPara::m_DesnPara;
	SectDescribe * pParaSectIn =GPara::m_Sect_in;
	if (pParaSectIn==NULL||pDesnPara==NULL)
	{
		return false;
	}

	SectProperty m_sect;
	m_sect.SectGeoInfo();
	m_sect.SectCal();

	stt.BeginP();
	stt.Txt(303,10);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("    ·  A","S1");
	stt.Txt(303,30);
	stt.EndP();

	// 	stt.BeginP();
	// 	stt.Txt(303,50);
	// 	stt.EndP();

	stt.BeginP();
	stt.TxSub("A","S1","=");stt.mm2(m_sect.As1);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("h","S1","=");stt.mm(m_sect.hs1);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("b","S1","=");stt.mm(m_sect.bs1);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("d","1y","=");stt.mm(m_sect.d1y);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("d","2y","=");stt.mm(m_sect.d2y);
	stt.EndP();

	// 	stt.BeginP();
	// 	stt.TxSub("d","s1y","=");stt.mm(m_sect.d);
	// 	stt.EndP();

	stt.BeginP();
	stt.TxSub("Z","sr1x","=");stt.mm3(m_sect.Zsr1x);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("l","sr1x","=");stt.mm4(m_sect.Isr1x);
	stt.EndP();

	///////////////////////////////////////////////////////

	stt.BeginP();
	stt.TxSub("    ·  A","S2");
	stt.Txt(303,30);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("A","S2","=");stt.mm2(m_sect.As2);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("h","S2","=");stt.mm(m_sect.hs2);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("b","S2","=");stt.mm(m_sect.bs2);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("d","1x","=");stt.mm(m_sect.d1x);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("d","2x","=");stt.mm(m_sect.d2x);
	stt.EndP();

	// 	stt.BeginP();
	// 	stt.TxSub("d","s1x","=");stt.mm(1010);
	// 	stt.EndP();

	 
	stt.BeginP();
	stt.TxSub("Z","sr1y","=");stt.mm3(m_sect.Zsr1y);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("l","sr1y","=");stt.mm4(m_sect.Isr1y);
	stt.EndP();

	///////////////////////////////////////////////////////
	stt.BeginP();
	stt.Txt(303,70); //	Definition of plates equivalent to steel profiles
	stt.EndP();

	// 	stt.BeginP();
	// 	stt.Txt(303,90); 
	// 	stt.EndP();

	stt.BeginP();
	stt.TxUp("d","*","=");stt.mm(m_sect.hh);
	stt.EndP();

	stt.BeginP();
	stt.TxUp("b","*","=");stt.mm(m_sect.bb);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Z","sx","=");stt.mm3(m_sect.Zsx,0,8);
	stt.EndP();
	stt.BeginP();
	stt.TxSub("Z","sy","=");stt.mm3(m_sect.Zsy,0,5);
	stt.EndP();
	stt.BeginP();
	stt.TxSub("I","sx","=");stt.mm4(m_sect.Isx,0,10);
	stt.EndP();
	stt.BeginP();
	stt.TxSub("I","sy","=");stt.mm4(m_sect.Isy,0,10);
	stt.EndP();

	///////////////////////////////////////////////////////
	stt.BeginP();//Evaluation of the plastic resistance to axial force
	stt.Txt(303,110);stt.TxSub("N","pl.Rd"," and ");stt.TxSub("N","pm.Rd"); 
	stt.EndP();

	stt.BeginP();
	stt.TxSub("N","pl.Rd"," = ");stt.kN(GlobalResult::NplRd*0.001);
	stt.EndP();
	stt.BeginP();
	stt.TxSub("N","pm.Rd"," = ");stt.kN(GlobalResult::NpmRd*0.001);
	stt.EndP();

	///////////////////////////////////////////////////////
	stt.BeginP();//Evaluation of the maximum moment resistance
	stt.Txt(303,130);stt.TxSub("M","max.Rd");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("M","max.Rd_y"," = ");stt.kNm(GlobalResult::MmaxRd_y*1e-6);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("M","max.Rd_z"," = ");stt.kNm(GlobalResult::MmaxRd_z*1e-6);
	stt.EndP();

	///////////////////////////////////////////////////////
	stt.BeginP();//Evaluation of the maximum moment resistance
	stt.Txt(303,150);stt.TxSub("M","pl.Rd");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("M","pl.Rd_y"," = ");stt.kNm(GlobalResult::MplRd_y*1e-6);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("M","pl.Rd_z"," = ");stt.kNm(GlobalResult::MplRd_z*1e-6);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("h","nx"," = ");stt.mm(GPara::hnx);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("h","ny"," = ");stt.mm(GPara::hny);
	stt.EndP();

	///////////////////////////////////////////////////////
	//Interaction curve of bending moment and axial force
	stt.BeginP(); 
	stt.Txt(303,170);
	stt.EndP();

	stt.BeginP(); 
	stt.Txt(303,190);
	stt.EndP();

	stt.BeginP(); 
	stt.Txt(303,200);
	stt.EndP();

	stt.BeginP(); 
	stt.Txt(303,210);//A – pure axial capacity point
	stt.EndP();

	stt.BeginP();
	stt.TxSub("N","A"," = ");stt.TxSub("N","pl.Rd"," = ");stt.kN(GlobalResult::NplRd*0.001);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("M","A"," = ");stt.kNm(0);
	stt.EndP();

	stt.BeginP(); 
	stt.Txt(303,230);//b – pure flexural bending point
	stt.EndP();

	stt.BeginP();
	stt.TxSub("N","B"," = ");stt.kN(0);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("M","B"," = ");stt.TxSub("M","pl.Rd_y"," = ");stt.kNm(GlobalResult::MplRd_y*1e-6);
	stt.EndP();


	stt.BeginP(); 
	stt.Txt(303,250);// C – point with bending moment equal to the pure bend
	stt.EndP();

	stt.BeginP();
	stt.TxSub("N","C"," = ");stt.TxSub("N","pm.Rd"," = ");stt.kN(GlobalResult::NpmRd*0.001);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("M","C"," = ");stt.TxSub("M","pl.Rd_y"," = ");stt.kNm(GlobalResult::MplRd_y*1e-6);
	stt.EndP();

	stt.BeginP(); 
	stt.Txt(303,270);// C – point with bending moment equal to the pure bend
	stt.EndP();

	stt.BeginP();
	stt.TxSub("N","D"," = ");stt.TxV(0.5,1),stt.TxSub("N","pm.Rd"," = ");stt.kN(0.5*GlobalResult::NpmRd*0.001);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("M","D"," = ");stt.TxSub("M","max.Rd_y"," = ");stt.kNm(GlobalResult::MmaxRd_y*1e-6);
	stt.EndP();

	//另一个方向
	stt.BeginP(); 
	stt.Txt(303,290);
	stt.EndP();

	stt.BeginP(); 
	stt.Txt(303,210);//A – pure axial capacity point
	stt.EndP();

	stt.BeginP();
	stt.TxSub("N","A"," = ");stt.TxSub("N","pl.Rd"," = ");stt.kN(GlobalResult::NplRd*0.001);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("M","A"," = ");stt.kNm(0);
	stt.EndP();

	stt.BeginP(); 
	stt.Txt(303,230);//b – pure flexural bending point
	stt.EndP();

	stt.BeginP();
	stt.TxSub("N","B"," = ");stt.kN(0);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("M","B"," = ");stt.TxSub("M","pl.Rd_z"," = ");stt.kNm(GlobalResult::MplRd_z*1e-6);
	stt.EndP();


	stt.BeginP(); 
	stt.Txt(303,250);// C – point with bending moment equal to the pure bend
	stt.EndP();

	stt.BeginP();
	stt.TxSub("N","C"," = ");stt.TxSub("N","pm.Rd"," = ");stt.kN(GlobalResult::NpmRd*0.001);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("M","C"," = ");stt.TxSub("M","pl.Rd_z"," = ");stt.kNm(GlobalResult::MplRd_z*1e-6);
	stt.EndP();

	stt.BeginP(); 
	stt.Txt(303,270);// C – point with bending moment equal to the pure bend
	stt.EndP();

	stt.BeginP();
	stt.TxSub("N","D"," = ");stt.TxV(0.5,1),stt.TxSub("N","pm.Rd"," = ");stt.kN(0.5*GlobalResult::NpmRd*0.001);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("M","D"," = ");stt.TxSub("M","max.Rd_z"," = ");stt.kNm(GlobalResult::MmaxRd_z*1e-6);
	stt.EndP();
	return true;
}

bool CPk_MainOutPut::RESISTANCE_OF_THE_CROSS_SECTION_TO_COMBINED_ACTIONS_M_N2(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	stt.BeginP(); 
	stt.Txt(303,330);
	stt.EndP();

	return true;
}

bool CPk_MainOutPut::BUCKLING_RESISTANCE_TO_AXIAL_FORCE(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	stt.BeginP(); 
	stt.Txt(304,10);//Effective stiffness evaluation",
	stt.EndP();

// 	stt.BeginP(); 
// 	stt.Txt(304,30); stt.TxV(1.0,1);
// 	stt.EndP();

	stt.BeginP(); 
	stt.TxSub("E","ceff","="); stt.MPa(GlobalResult::Eceff);
	stt.EndP();

	stt.BeginP(); 
	double EI = GlobalResult::EIeff_y*1.e-6;
	stt.TxSub("(EI)","eff_y","="); stt.kNm2(EI,0,8);
	stt.EndP();

	stt.BeginP(); 
	EI = GlobalResult::EIeff_z*1.e-6;
	stt.TxSub("(EI)","eff_z","="); stt.kNm2(EI,0,8);
	stt.EndP();

	stt.BeginP(); 
	stt.TxSub("N","cr_y","="); stt.kN(GlobalResult::Ncr_y*1.e-3);
	stt.EndP();

	stt.BeginP(); 
	stt.TxSub("N","cr_z","="); stt.kN(GlobalResult::Ncr_z*1.e-3);
	stt.EndP();

	///////////////////////////////////////////////////////
	stt.BeginP(); //Evaluation of the reduced axial force parameter  X
	stt.Txt(304,50); 
	stt.EndP();

	// 	stt.BeginP(); 
	// 	stt.TxSub("N","pl.Rk","="); stt.kN(50);
	// 	stt.EndP();
	// 


	stt.BeginP(); 
	stt.Txt(304,70); //Buckling curve about the major axis: b;
	stt.EndP();

	stt.BeginP(); 
	stt.Txt("α=");stt.TxV(0.34,2); //stt.Txt(304,130);
	stt.EndP();

	stt.BeginP(); 
	stt.TxSub("λ","relative","=");
	// λ
	stt.TxV(GlobalResult::ll_y,4);
	stt.EndP();

	stt.BeginP(); 
	stt.Txt("χ=");stt.TxV(GlobalResult::Lx_y,4);  
	stt.EndP();

	stt.BeginP(); 
	stt.TxSub("N","pl.b.Rd-y","=");stt.TxSub("χ·N","pl.Rd","=");
	// N","pl.b.Rd
	double totalN = GPara::ks_y*GPara::NplRd*0.001;
	stt.kN(totalN);  
	stt.EndP();

	stt.BeginP(); 
	stt.EndP();

	stt.BeginP(); 
	stt.Txt(304,90); //Buckling curve about the minor axis: c;
	stt.EndP();

	stt.BeginP(); 
	stt.Txt("α=");stt.TxV(0.49,2); //stt.Txt(304,130);
	stt.EndP();

	stt.BeginP(); 
	stt.TxSub("λ","relative","=");
	// λ
	stt.TxV(GlobalResult::ll_z,4);
	stt.EndP();

	stt.BeginP(); 
	stt.Txt("χ=");stt.TxV(GlobalResult::Lx_z,4);  
	stt.EndP();
 
	stt.BeginP(); 
	stt.TxSub("N","pl.b.Rd-z","=");stt.TxSub("χ·N","pl.Rd","=");
	// N","pl.b.Rd
	totalN = GPara::ks_z*GPara::NplRd*0.01;
	stt.kN(totalN);  
	stt.EndP();

	// 	stt.BeginP(); 
	// 	stt.Txt(304,110); //Major axis:
	// 	stt.EndP();



	// 	stt.BeginP(); 
	// 	stt.Txt("Φ=");stt.TxV(0.34);  
	// 	stt.EndP();



	// 	stt.BeginP(); 
	// 	stt.TxSub("N","pl.b.Rd","=");stt.TxSub("χ×N","pl.Rd","=");stt.kN(0.34);  
	// 	stt.EndP();
	// 
	// 	stt.BeginP(); 
	// 	stt.TxSub("N","pm.b.Rd","=");stt.TxSub("χ×N","pm.Rd","=");stt.kN(0.34);  
	// 	stt.EndP();
	// 
	// 	stt.BeginP(); 
	// 	stt.Txt(304,150); //Minor axis:
	// 	stt.EndP();
	return true; 
}
bool CPk_MainOutPut::AMPLIFICATION_OF_DESIGN_MOMENT_OF_COMBINED_ACTION_M_N_CONSIDERING(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	DrawingFiberPDM_Points(pGenerateText,mReportItem);

	stt.BeginP(); 
	stt.Txt(305,5);
	stt.EndP();

// 	stt.BeginP(); 
// 	stt.Txt("β=");stt.TxV(GlobalResult::beta,2); 
// 	stt.EndP();

	stt.BeginP(); 
	stt.TxSub("(EI)","eff,II_y","=");stt.kNm2(GlobalResult::EIeff1_y*1.e-6,0,8); 
	stt.EndP();

	stt.BeginP(); 
	stt.TxSub("N","cr,eff_y","=");stt.kN(GlobalResult::Ncreff_y*0.001); 
	stt.EndP();

// 	stt.BeginP();
// 	stt.TxSub("k","y","=");stt.TxV(GPara::kk_y[0]);
// 	stt.EndP();
	//这里应该输出弯矩表格


	stt.BeginP(); 
	stt.Txt(305,7);
	stt.EndP();

// 	stt.BeginP(); 
// 	stt.Txt("β=");stt.TxV(GlobalResult::beta,2); 
// 	stt.EndP();

	stt.BeginP(); 
	stt.TxSub("(EI)","eff,II_z","=");stt.kNm2(GlobalResult::EIeff1_z*1.e-6,0,8); 
	stt.EndP();

	stt.BeginP(); 
	stt.TxSub("N","cr,eff_z","=");stt.kN(GlobalResult::Ncreff_z*0.001); 
	stt.EndP();

// 	stt.BeginP();
// 	stt.TxSub("k","z","=");stt.TxV(GPara::kk_z[0]);
// 	stt.EndP();
	//这里应该输出弯矩表格
	for(int i=0;i<GPara::m_DesnPara->n_Combines;i++){
		MTable(pGenerateText,mReportItem,i,1);
		MTable(pGenerateText,mReportItem,i,2);
	}
	return true;
}

void CPk_MainOutPut::DrawingFiberPDM_Points(sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem )
{
	CStringArray filenames,titleNames;


	filenames.Add(_T("Fiber_NM_y_Point.t"));titleNames.Add( _T("Fiber_NM_y_Point"));
	filenames.Add(_T("Fiber_NM_z_Point.t"));titleNames.Add( _T("Fiber_NM_z_Point"));
	filenames.Add(_T("PDM_NM_y_Point.t"));titleNames.Add( _T("PDM_NM_y_Point"));
	filenames.Add(_T("PDM_NM_z_Point.t"));titleNames.Add( _T("PDM_NM_z_Point"));


	GRAPHIC_FORMAT GFormat;

	GFormat.height = 0;
	GFormat.width = 0;
	GFormat.align = sdAlignCenter;
	GFormat.titleFormat.align = GFormat.align;

	sd::GRAPHIC_ITEM  mGraphItem;
	mGraphItem.fontGraphicTitle = mReportItem.fontGraphicTitle;

	CString strTitle;
	CString strFile;
	int nWid = 150;
	bool resetIndex = true;

	for (int i=0;i<filenames.GetSize();i++)
	{
		strFile =  filenames[i];
		strTitle = titleNames[i];
		GenDrawingName(mGraphItem,strTitle,resetIndex);
		if (resetIndex)
		{
			resetIndex = false;
		}

		GFormat.titleFormat.setTitle(strTitle);
		int oldv = m_drawHoriVeri;
		m_drawHoriVeri = DRAWPAGE_VERI;
		DrawingFunc(pGenerateText,mGraphItem,strFile,&GFormat,nWid);
		m_drawHoriVeri = oldv;
	}

}


bool CPk_MainOutPut::INTERFACE_SHEAR_CONNECTION(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	if(GPara::m_DesnPara->shearmethod==0){
	stt.BeginP();
	stt.Txt(306,10);
	stt.EndP();


	stt.BeginP();
	stt.Txt(306,20); /************major axis*************/
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,30);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,50);
	stt.EndP();

	// Figure 20 //
	//强轴
	CString jpgname;
	jpgname.Format(_T("%db-y.jpg"),GPara::m_Sect_in->m_kind);		
	DrawPicJPG(pGenerateText,mReportItem,jpgname);

	stt.BeginP();
	stt.TxSub("b","c3","=");stt.mm(GlobalResult::BeamRst_y.bc1);
	stt.TxSub("b","s","=");stt.mm(GlobalResult::BeamRst_y.bs);
	stt.TxSub("b","c4","=");stt.mm(GlobalResult::BeamRst_y.bc2);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,60); stt.TxSub("b","c3",":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("EI","eff,bc3","="); stt.TxSub("E","sr","·"); stt.TxSub("I","sr,bc3","+"); 
	stt.TxSub("K","e","·"); stt.TxSub("E","c,eff","·"); stt.TxSub("I","c,bc3","=");
	// EI,eff,bc3
//	stt.Nmm2(GlobalResult::BeamRst_y.EIeffbc1);
	stt.kNm2(GlobalResult::BeamRst_y.EIeffbc1*1.e-6,0,6);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,60); stt.TxSub("b","c4",":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("EI","eff,bc4","="); stt.TxSub("E","sr","·"); stt.TxSub("I","sr,bc4","+"); 
	stt.TxSub("K","e","·"); stt.TxSub("E","c,eff","·"); stt.TxSub("I","c,bc4","=");
	// EI,eff,bc4
//	stt.Nmm2(GlobalResult::BeamRst_y.EIeffbc2);
	stt.kNm2(GlobalResult::BeamRst_y.EIeffbc2*1.e-6,0,6);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,60); stt.TxSub("b","s",":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("EI","eff,bs","="); stt.TxSub("E","s","·"); stt.TxSub("I","s,bs","+"); stt.TxSub("E","sr","·"); stt.TxSub("I","sr,bs","+"); 
	stt.TxSub("K","e","·"); stt.TxSub("E","c,eff","·"); stt.TxSub("I","c,bs","=");
	// EI,eff,bs
//	stt.Nmm2(GlobalResult::BeamRst_y.EIeffbs);
	stt.kNm2(GlobalResult::BeamRst_y.EIeffbs*1.e-6,0,6);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("EI","eff","=");
	// EI,eff
//	stt.Nmm2(GlobalResult::BeamRst_y.EIeff);
	stt.kNm2(GlobalResult::BeamRst_y.EIeff*1.e-6,0,6);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,65);
	stt.EndP();

// 	stt.BeginP();
// 	stt.TxSub("V","Ed,bc3","=");
// 	// ("V","Ed,bc3","=");
// 	stt.kN(GlobalResult::BeamRst_y.Vedbc1);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("V","Ed,bc4","=");
// 	// ("V","Ed,bc4","=");
// 	stt.kN(GlobalResult::BeamRst_y.Vedbc2);
// 	stt.EndP();

	stt.BeginP();
	stt.TxSub("V","Ed,bs","=");
	// ("V","Ed,bs","=");
	stt.kN(GlobalResult::BeamRst_y.Vedbs);
	stt.EndP();


	stt.BeginP();
	stt.Txt(306,67); stt.TxSub("b","s"); stt.Txt(".");
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,68);
	stt.EndP();

	// Figure 21 //

	stt.TxSub("I*","c","=");
	// I*c
	stt.mm4(GlobalResult::BeamRst_y.Ic);

	stt.BeginP();
	stt.Txt(306,70);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("S","CC1"),stt.Txt(306,90);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("S","CC1","=");
	// Scc1
	stt.mm3(GlobalResult::BeamRst_y.Scc1);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,130);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("V","Ed,CC1","=");stt.TxSub("V","Ed,bs","·");stt.TxSub("S","CC1","/");stt.TxUpSub("I","*","c","=");
	stt.N$mm(GlobalResult::BeamRst_y.VedCC1);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,150); stt.TxSub("V","Ed,CC1","=");stt.kN$m(GlobalResult::BeamRst_y.VedCC1);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,170);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("S","CC2"),stt.Txt(306,175);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("S","CC2","=");
	// Scc2
	stt.mm3(GlobalResult::BeamRst_y.Scc2);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,180);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("V","Ed,CC2","=");stt.TxSub("V","Ed,bs","·");stt.TxSub("S","CC2","/");stt.TxUpSub("I","*","c","=");
	stt.N$mm(GlobalResult::BeamRst_y.VedCC2,1);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,150); stt.TxSub("V","Ed,CC2","=");stt.kN$m(GlobalResult::BeamRst_y.VedCC2,1);
	stt.EndP();


	stt.BeginP();
	stt.Txt(306,190);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,210);
	stt.EndP();

	stt.BeginP();
	stt.Txt("d=");stt.mm(GPara::m_DesnPara->stud_d);
	stt.EndP();
	stt.BeginP();
	stt.TxSub("h","SC","=");stt.mm(GPara::m_DesnPara->stud_len);
	stt.EndP();
	stt.BeginP();
	stt.TxSub("f","u","=");
	// fu
	stt.MPa(GPara::m_DesnPara->stud_fu);
	stt.EndP();
	stt.BeginP();
	stt.TxSub("P","rk","=");stt.kN(GlobalResult::Vstud);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,230);//For 1 m column length, the necessary amount of shear studs at each flange interface is:
	stt.EndP();

	stt.BeginP();
	stt.TxSub("n","studs_CC1","=");stt.TxSub("V","Ed_CC1","·1m/");stt.TxSub("P","Rk=");
	stt.studs$1m(GlobalResult::BeamRst_y.nstud_cc1);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("n","studs_CC2","=");stt.TxSub("V","Ed_CC2","·1m/");stt.TxSub("P","Rk=");
	stt.studs$1m(GlobalResult::BeamRst_y.nstud_cc2);
	stt.EndP();




	stt.BeginP();
	stt.Txt(306,240); /************minor axis*************/
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,30);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,50);
	stt.EndP();

	// Figure 22 //
	jpgname.Format(_T("%d-z.jpg"),GPara::m_Sect_in->m_kind);		
	DrawPicJPG(pGenerateText,mReportItem,jpgname);

	stt.BeginP();
	stt.TxSub("b","c1","=");stt.mm(GlobalResult::BeamRst_z.bc1);
	stt.TxSub("b","s2","=");stt.mm(GlobalResult::BeamRst_z.bs);
	stt.TxSub("b","c2","=");stt.mm(GlobalResult::BeamRst_z.bc2);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,60); stt.TxSub("b","c1",":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("EI","eff,bc1","="); stt.TxSub("E","sr","·"); stt.TxSub("I","sr,bc1","+"); 
	stt.TxSub("K","e","·"); stt.TxSub("E","c,eff","·"); stt.TxSub("I","c,bc1","=");
	// EI,eff,bc1
	stt.Nmm2(GlobalResult::BeamRst_z.EIeffbc1);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,60); stt.TxSub("b","c2",":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("EI","eff,bc2","="); stt.TxSub("E","sr","·"); stt.TxSub("I","sr,bc2","+"); 
	stt.TxSub("K","e","·"); stt.TxSub("E","c,eff","·"); stt.TxSub("I","c,bc2","=");
	// EI,eff,bc2
	stt.Nmm2(GlobalResult::BeamRst_z.EIeffbc2);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,60); stt.TxSub("b","s2",":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("EI","eff,bs2","="); stt.TxSub("E","s","·"); stt.TxSub("I","s,bs2","+"); stt.TxSub("E","sr","·"); stt.TxSub("I","sr,bs2","+"); 
	stt.TxSub("K","e","·"); stt.TxSub("E","c,eff","·"); stt.TxSub("I","c,bs2","=");
	// EI,eff,bs2
	stt.Nmm2(GlobalResult::BeamRst_z.EIeffbs);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("EI","eff","=");
	// EI,eff
	stt.Nmm2(GlobalResult::BeamRst_z.EIeff);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,65);
	stt.EndP();

// 	stt.BeginP();
// 	stt.TxSub("V","Ed,bc1","=");
// 	// ("V","Ed,bc1","=");
// 	stt.kN(GlobalResult::BeamRst_z.Vedbc1);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("V","Ed,bc2","=");
// 	// ("V","Ed,bc2","=");
// 	stt.kN(GlobalResult::BeamRst_z.Vedbc2);
// 	stt.EndP();

	stt.BeginP();
	stt.TxSub("V","Ed,bs2","=");
	// ("V","Ed,bs2","=");
	stt.kN(GlobalResult::BeamRst_z.Vedbs);
	stt.EndP();


	stt.BeginP();
	stt.Txt(306,67); stt.TxSub("b","s2"); stt.Txt(".");
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,68);
	stt.EndP();

	// Figure 23 //

	stt.TxSub("I*","c","=");
	// I*c
	stt.mm4(GlobalResult::BeamRst_z.Ic);

	stt.BeginP();
	stt.Txt(306,70);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("S","CC1"),stt.Txt(306,90);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("S","CC1","=");
	// Scc1
	stt.mm3(GlobalResult::BeamRst_z.Scc1);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,130);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("V","Ed,CC1","=");stt.TxSub("V","Ed,bs","·");stt.TxSub("S","CC1","/");stt.TxUpSub("I","*","c","=");
	stt.N$mm(GlobalResult::BeamRst_z.VedCC1);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,150); stt.TxSub("V","Ed,CC1","=");stt.kN$m(GlobalResult::BeamRst_z.VedCC1);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,170);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("S","CC2"),stt.Txt(306,175);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("S","CC2","=");
	// Scc2
	stt.mm3(GlobalResult::BeamRst_z.Scc2);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,180);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("V","Ed,CC2","=");stt.TxSub("V","Ed,bs","·");stt.TxSub("S","CC2","/");stt.TxUpSub("I","*","c","=");
	stt.N$mm(GlobalResult::BeamRst_z.VedCC2);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,150); stt.TxSub("V","Ed,CC2","=");stt.kN$m(GlobalResult::BeamRst_z.VedCC2);
	stt.EndP();


	stt.BeginP();
	stt.Txt(306,190);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,210);
	stt.EndP();

	stt.BeginP();
	stt.Txt("d=");stt.mm(GPara::m_DesnPara->stud_d);
	stt.EndP();
	stt.BeginP();
	stt.TxSub("h","SC","=");stt.mm(GPara::m_DesnPara->stud_len);
	stt.EndP();
	stt.BeginP();
	stt.TxSub("f","u","=");
	// fu
	stt.MPa(GPara::m_DesnPara->stud_fu);
	stt.EndP();
	stt.BeginP();
	stt.TxSub("P","rk","=");stt.kN(GlobalResult::Vstud);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,230);//For 1 m column length, the necessary amount of shear studs at each flange interface is:
	stt.EndP();

	stt.BeginP();
	stt.TxSub("n","studs_CC1","=");stt.TxSub("V","Ed_CC1","·1m/");stt.TxSub("P","Rk=");
	stt.studs$1m(GlobalResult::BeamRst_z.nstud_cc1);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("n","studs_CC2","=");stt.TxSub("V","Ed_CC2","·1m/");stt.TxSub("P","Rk=");
	stt.studs$1m(GlobalResult::BeamRst_z.nstud_cc2);
	stt.EndP();

	}	//end of beam theory
	//开关控制
	else{
 	stt.BeginP();
 	stt.Txt(306,250);//(2)	Adapted method-truss model method
 	stt.EndP();
 
 
 	stt.BeginP();
 	stt.Txt(306,20); /************major axis*************/
 	stt.EndP();
 
 
 	stt.BeginP();
 	stt.Txt(306,270);//The definition of the used symbols and subsections are defined in FIgure
 	stt.EndP();
 
	CString jpgname;
	jpgname.Format(_T("%d-y.jpg"),GPara::m_Sect_in->m_kind);		
	DrawPicJPG(pGenerateText,mReportItem,jpgname);

 	// Figure 24 //
 
//  	stt.BeginP();
//  	stt.Txt(306,290);
//  	stt.EndP();
	int side = 1;
	TrussModelTable(pGenerateText,mReportItem,side);
	stt.BeginP();
	stt.Txt(306,740);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,741);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,742);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,743);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,750);
	stt.EndP();
	if(GPara::m_Sect_in->m_kind==6){
		stt.BeginP();
		stt.Txt(306,370);
		stt.EndP();
	}

 	stt.BeginP();
 	stt.Txt(306,240); /************minor axis*************/
 	stt.EndP();
 
 
 	stt.BeginP();
 	stt.Txt(306,270);//The definition of the used symbols and subsections are defined in FIgure
 	stt.EndP();

	jpgname;
	jpgname.Format(_T("%d-z.jpg"),GPara::m_Sect_in->m_kind);		
	DrawPicJPG(pGenerateText,mReportItem,jpgname);
 	// Figure 25 //
 
//  	stt.BeginP();
//  	stt.Txt(306,290);
//  	stt.EndP();
	side = 2;
	TrussModelTable(pGenerateText,mReportItem,side);
	stt.BeginP();
	stt.Txt(306,740);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,741);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,742);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,743);
	stt.EndP();

	stt.BeginP();
	stt.Txt(306,750);
	stt.EndP();
	if(GPara::m_Sect_in->m_kind==7){
		stt.BeginP();
		stt.Txt(306,370);
		stt.EndP();
	}
// 	stt.BeginP();
// 	stt.TxSub("ρ","swy","=");stt.TxSub("A","sw","/");stt.Txt("bs=");stt.TxV(GPara::m_Sect_in->Tran_ratio_x,1);stt.Txt("%");
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("ρ","swz","=");stt.TxSub("A","sw","/");stt.Txt("bs=");stt.TxV(GPara::m_Sect_in->Tran_ratio_y,1);stt.Txt("%");
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,310); stt.TxSub("V","c,i",""); stt.Txt(306,330);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,350);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt("s=");stt.mm(GPara::m_Sect_in->Bar_interval);
// 	stt.EndP();
// 
// // 	stt.BeginP();
// // 	stt.TxSub("E","s","=");stt.MPa(200000);
// // 	stt.EndP();
// 
// // 	stt.BeginP();
// // 	stt.TxSub("A","sw","=");stt.mm2(976);
// // 	stt.EndP();
// // 
// // 	stt.BeginP();
// // 	stt.TxSub("E","c","=");stt.MPa(37278);stt.Txt(306,370);
// // 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("b","w","=");stt.TxSub("b","c","=");stt.mm(GlobalResult::Truss_y.bc);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,390);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("S","RC,bc","=");stt.N$mm(GlobalResult::Truss_y.Srcbc);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,410);//b)	Section bs
// 	stt.EndP();
// 
// // 	stt.BeginP();
// // 	stt.Txt("s=");stt.mm(100);
// // 	stt.EndP();
// 
// // 	stt.BeginP();
// // 	stt.TxSub("E","s","=");stt.MPa(200000);
// // 	stt.EndP();
// 
// // 	stt.BeginP();
// // 	stt.TxSub("A","sw","=");stt.mm2(976);
// // 	stt.EndP();
// 
// // 	stt.BeginP();
// // 	stt.TxSub("E","c","=");stt.MPa(37278);stt.Txt(306,370);
// // 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("b","w","=");stt.TxSub("b","s","=");stt.mm(GlobalResult::Truss_y.bs);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,450);//Because there is no middle steel profile in this type of section, the factor   could approximately equal to 1.0.
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("S","RC,bs","=");stt.N$mm(GlobalResult::Truss_y.Srcbs);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt("N=");stt.TxV(GlobalResult::Truss_y.nn,2);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt("G=");stt.MPa(GlobalResult::Truss_y.Ge);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("A","v","=");stt.mm2(GlobalResult::Truss_y.Av);stt.Txt(306,470);// (Only including the area of web plate)
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt("z=");
// 	// z
// 	stt.mm(GlobalResult::Truss_y.Zz);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("S","SP,bs","=");
// 	// ("S","SP,bs","=")
// 	stt.N$mm(GlobalResult::Truss_y.Sspbs);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,490); stt.TxSub("V","Ed"," :");
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("V","c,bs","=");
// 	// ("V","c,bs","=");
// 	stt.N(GlobalResult::Truss_y.Vcbs);
// 	stt.EndP();
// 
// 
// 	stt.BeginP();
// 	stt.Txt(306,510); stt.TxSub("V","l,a",""); stt.Txt(306,530);
// 	stt.EndP();
// 
// 
// 	stt.BeginP();
// 	stt.TxSub("V","l,a","="); stt.TxSub("V","c,bs","=");
// 	// ("V","c,bs","=");
// 	stt.N(GlobalResult::Truss_y.Vcbs);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,550); stt.TxSub("τ","l,a","=");
// 	// ("τ","l,a","=");
// 	stt.N$m(306.550,3);
// 	stt.EndP();
// 
// 
// 	stt.BeginP();
// 	stt.Txt(306,570); 
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,590); stt.TxSub("V","l,a","≤"); stt.TxSub("V","l,Rd",".");
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,610); stt.TxSub("V","Rd,friction",""); stt.Txt(306,630);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("V","Rd,bond","=");
// 	// ("V","Rd,bond","=");
// 	stt.kN(4138.2);
// 	stt.EndP();
// 
// 	double VRd_bond = 1.0f;
// 	double Vl_a = 2.0f;
// 	double Vremain = 3;
// 	if (VRd_bond>Vl_a)
// 	{
// 		stt.BeginP();
// 		stt.Txt(306,650); stt.TxSub("V","Rd,bond",">"); stt.TxSub("V","l,a",""); 	stt.Txt(306,670); 
// 		stt.EndP();
// 
// 		stt.BeginP();
// 		stt.Txt(306,690);
// 		stt.EndP();
// 
// 		// 忽略掉粘结力计算
// 		Vremain=Vl_a;
// 	}
// 	else
// 	{
// 		// 减掉粘结力计算
// 		Vremain=Vl_a-VRd_bond;
// 	}
// 
// 	stt.BeginP();
// 	stt.TxSub("P","Rk","=");
// 	// ("P","Rk","=");
// 	stt.kN(141.37);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("n","studs","="); stt.TxSub("V","Rd,connectors","·"); stt.m(1.0); stt.Txt("/("); stt.TxSub("P","Rk","·z)=");
// 	// Vremain
// 	stt.TxV(141.37);
// 	stt.Txt("/2 ·1m/("); stt.TxSub("P","Rk","·z)=");
// 	// nstuds
// 	stt.TxV(7);
// 	stt.Txt(" studs/1m "); stt.Txt(306,710); 
// 	stt.EndP();
// 
// 
// 
// 
// 	stt.BeginP();
// 	stt.Txt(306,240); /************minor axis*************/
// 	stt.EndP();
// 
// 
// 	stt.BeginP();
// 	stt.Txt(306,270);//The definition of the used symbols and subsections are defined in FIgure
// 	stt.EndP();
// 
// 	// Figure 25 //
// 
// 	stt.BeginP();
// 	stt.Txt(306,290);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("ρ","sw","=");stt.TxSub("A","sw","/");stt.Txt("bs=");stt.TxV(GPara::m_Sect_in->Transversal_ratio,1);stt.Txt("%");
// 	stt.EndP();
// 
// 
// 	stt.BeginP();
// 	stt.Txt(306,310); stt.TxSub("V","c,i",""); stt.Txt(306,330);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,350);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt("s=");stt.mm(100);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("E","s","=");stt.MPa(200000);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("A","sw","=");stt.mm2(976);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("E","c","=");stt.MPa(37278);stt.Txt(306,370);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("b","w","=");stt.TxSub("b","c","=");stt.mm(976);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,390);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("S","RC,bc","=");stt.N$mm(1762843.5);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,410);//b)	Section bs
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt("s=");stt.mm(100);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("E","s","=");stt.MPa(200000);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("A","sw","=");stt.mm2(976);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("E","c","=");stt.MPa(37278);stt.Txt(306,370);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("b","w","=");stt.TxSub("b","c","=");stt.mm(1048);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,450);//Because there is no middle steel profile in this type of section, the factor   could approximately equal to 1.0.
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("S","RC,bs","=");stt.N$mm(1892889.4);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt("N=");stt.TxV(2);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt("G=");stt.MPa(80769);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("A","v","=");stt.mm2(3200);stt.Txt(306,470);// (Only including the area of web plate)
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt("z=");
// 	// z
// 	stt.mm(1900);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("S","SP,bs","=");
// 	// ("S","SP,bs","=")
// 	stt.N$mm(2720640);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,490); stt.TxSub("V","Ed"," :");
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("V","c,bs","=");
// 	// ("V","c,bs","=");
// 	stt.N(3444779.27);
// 	stt.EndP();
// 
// 
// 	stt.BeginP();
// 	stt.Txt(306,510); stt.TxSub("V","l,a",""); stt.Txt(306,530);
// 	stt.EndP();
// 
// 
// 	stt.BeginP();
// 	stt.TxSub("V","l,a","="); stt.TxSub("V","c,bs","=");
// 	// ("V","c,bs","=");
// 	stt.N(3444779.27);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,550); stt.TxSub("τ","l,a","=");
// 	// ("τ","l,a","=");
// 	stt.N$m(306.550,3);
// 	stt.EndP();
// 
// 
// 	stt.BeginP();
// 	stt.Txt(306,570); 
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,590); stt.TxSub("V","l,a","≤"); stt.TxSub("V","l,Rd",".");
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(306,610); stt.TxSub("V","Rd,friction",""); stt.Txt(306,630);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("V","Rd,bond","=");
// 	// ("V","Rd,bond","=");
// 	stt.kN(4138.2);
// 	stt.EndP();
// 
// 	//double VRd_bond = 3.0;
// 	//double Vl_a = 2.0;
// 	if (VRd_bond>Vl_a)
// 	{
// 		stt.BeginP();
// 		stt.Txt(306,650); stt.TxSub("V","Rd,bond",">"); stt.TxSub("V","l,a",""); 	stt.Txt(306,670); 
// 		stt.EndP();
// 
// 		stt.BeginP();
// 		stt.Txt(306,690);
// 		stt.EndP();
// 
// 		// 忽略掉粘结力计算
// 		Vremain=Vl_a;
// 	}
// 	else
// 	{
// 		// 减掉粘结力计算
// 		Vremain=Vl_a-VRd_bond;
// 	}
// 
// 	stt.BeginP();
// 	stt.TxSub("P","Rk","=");
// 	// ("P","Rk","=");
// 	stt.kN(141.37);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("n","studs","="); stt.TxSub("V","Rd,connectors","·"); stt.m(1); stt.Txt("/("); stt.TxSub("P","Rk","·z)=");
// 	// Vremain
// 	stt.TxV(141.37);
// 	stt.Txt("/2 ·1m/("); stt.TxSub("P","Rk","·z)=");
// 	// nstuds
// 	stt.TxV(7);
// 	stt.Txt(" studs/1m "); stt.Txt(306,710); 
// 	stt.EndP();
	}	//end of truss model

	return true;
}

bool CPk_MainOutPut::SYNTHESIS_OF_VERIFICATIONS(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	float comb_pos;

	stt.BeginP();
	stt.Txt(307,10);
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.Nmax_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.Nmax_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","N","="); stt.TxSub("N","Ed","/"); stt.TxSub("N","pl,Rd","=");
	// ("Γ","N","="); 
	stt.TxV(GlobalResult::AixalForce_Check.floatVal[0],2);
	// 判断
	double RN = GlobalResult::AixalForce_Check.floatVal[0];
	if (RN<=1.0)
	{
		stt.Txt("≤1.0");
	}
	else
	{
		stt.Txt(">1.0");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();


	stt.BeginP();
	stt.Txt(307,70);
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.Vymax_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.Vymax_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","Vy","="); stt.TxSub("V","Ed","/"); stt.TxSub("V","pl,Rd","=");
	// ("Γ","Vy","="); 
	stt.TxV(GlobalResult::ShearForcevy_Check.floatVal[0],2);
	// 判断
	double RVy = GlobalResult::ShearForcevy_Check.floatVal[0];
	if (RVy<=1.0)
	{
		stt.Txt("≤1.0");
	}
	else
	{
		stt.Txt(">1.0");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.Vzmax_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.Vzmax_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","Vz","="); stt.TxSub("V","Ed","/"); stt.TxSub("V","pl,Rd","=");
	// ("Γ","Vz","="); 
	stt.TxV(GlobalResult::ShearForcevz_Check.floatVal[0],2);
	// 判断
	double RVz = GlobalResult::ShearForcevz_Check.floatVal[0];
	if (RVz<=1.0)
	{
		stt.Txt("≤1.0");
	}
	else
	{
		stt.Txt(">1.0");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();


	stt.BeginP();
	stt.Txt(307,90);
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.Myz_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.Myz_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","My,N","="); stt.TxSub("M","y,Ed","/"); stt.TxSub("M","y,pl,N,Rd","=");
	// ("Γ","My,N","="); 
	stt.TxV(GlobalResult::My_Check.floatVal[0],2);
	// 判断
	double RmyN = GlobalResult::My_Check.floatVal[0];
	if (RmyN<=0.8)//ΓMyN<=0.8
	{
		stt.Txt("≤0.8");
	}
	else
	{
		stt.Txt(">0.8");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.Myz_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.Myz_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","Mz,N","="); stt.TxSub("M","z,Ed","/"); stt.TxSub("M","z,pl,N,Rd","=");
	// ("Γ","Mz,N","="); 
	stt.TxV(GlobalResult::Mz_Check.floatVal[0],2);
	// 判断
	double RMzN = GlobalResult::Mz_Check.floatVal[0];
	if (RMzN<=0.8)
	{
		stt.Txt("≤0.8");
	}
	else
	{
		stt.Txt(">0.8");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,110);
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.Myz_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.Myz_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","Myz,N","="); stt.TxSub("M","y,Ed","/"); stt.TxSub("M","y,pl,N,Rd","+"); stt.TxSub("M","z,Ed","/"); stt.TxSub("M","z,pl,N,Rd","=");
	// ("Γ","Myz,N","="); 
	stt.TxV(GlobalResult::MNV_Check.floatVal[0],2);
	// 判断
	double RMyzN = GlobalResult::MNV_Check.floatVal[0];
	if (RMyzN<=1.0)//ΓMyzN<=1.0
	{
		stt.Txt("≤1.0");
	}
	else
	{
		stt.Txt(">1.0");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();


	stt.BeginP();
	stt.Txt(307,130);
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	stt.EndP();

	// 组合号
	stt.BeginP();
	stt.TxV(GlobalResult::loadcomb.Nmax_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.Nmax_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","by","="); stt.TxSub("N","Ed","/"); stt.TxSub("χN","pl,Rd","=");
	// ("Γ","by","="); 
	stt.TxV(GlobalResult::BucklingNby_Check.floatVal[0],2);
	// 判断
	double Rby = GlobalResult::BucklingNby_Check.floatVal[0];
	if (Rby<=1.0)//Γby<=1.0
	{
		stt.Txt("≤1.0");
	}
	else
	{
		stt.Txt(">1.0");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","bz","="); stt.TxSub("N","Ed","/"); stt.TxSub("χN","pl,Rd","=");
	// ("Γ","bz","="); 
	stt.TxV(GlobalResult::BucklingNbz_Check.floatVal[0],2);
	// 判断
	double Rbz = GlobalResult::BucklingNbz_Check.floatVal[0];
	if (Rbz <=1.0)//Γbz<=1.0
	{
		stt.Txt("≤1.0");
	}
	else
	{
		stt.Txt(">1.0");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();


	stt.BeginP();
	stt.Txt(307,150);
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.bMyz_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.bMyz_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","My,N","="); stt.TxSub("M","y,Ed","/"); stt.TxSub("M","y,pl,N,Rd","=");
	// ("Γ","My,N","="); 
	stt.TxV(GlobalResult::bMy_Check.floatVal[0],2);
	// 判断
	RmyN = GlobalResult::bMy_Check.floatVal[0];
	if (RmyN<=0.8)//ΓMyN<=0.8
	{
		stt.Txt("≤0.8");
	}
	else
	{
		stt.Txt(">0.8");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.bMyz_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.bMyz_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","Mz,N","="); stt.TxSub("M","z,Ed","/"); stt.TxSub("M","z,pl,N,Rd","=");
	// ("Γ","Mz,N","="); 
	stt.TxV(GlobalResult::bMz_Check.floatVal[0],2);
	// 判断
	RMzN = GlobalResult::bMz_Check.floatVal[0];
	if (RMzN<=0.8)
	{
		stt.Txt("≤0.8");
	}
	else
	{
		stt.Txt(">0.8");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,110);
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.bMyz_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.bMyz_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","Myz,N","="); stt.TxSub("M","y,Ed","/"); stt.TxSub("M","y,pl,N,Rd","+"); stt.TxSub("M","z,Ed","/"); stt.TxSub("M","z,pl,N,Rd","=");
	// ("Γ","Myz,N","="); 
	stt.TxV(GlobalResult::bMNV_Check.floatVal[0],2);
	// 判断
	RMyzN = GlobalResult::bMNV_Check.floatVal[0];
	if (RMyzN<=1.0)//ΓMyzN<=1.0
	{
		stt.Txt("≤1.0");
	}
	else
	{
		stt.Txt(">1.0");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();
	return true;
}

bool CPk_MainOutPut::SYNTHESIS_OF_VERIFICATIONS_CUSTOM(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	float comb_pos;

	//轴压稳定
	stt.BeginP();
	stt.Txt(307,130);
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	stt.EndP();

	// 组合号
	stt.BeginP();
	stt.TxV(GlobalResult::loadcomb.Nmax_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.Nmax_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","by","="); stt.TxSub("N","Ed","/"); stt.TxSub("χN","pl,Rd","=");
	// ("Γ","by","="); 
	stt.TxV(GlobalResult::BucklingNby_Check.floatVal[0],2);
	// 判断
	double Rby = GlobalResult::BucklingNby_Check.floatVal[0];
	if (Rby<=1.0)//Γby<=1.0
	{
		stt.Txt("≤1.0");
	}
	else
	{
		stt.Txt(">1.0");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","bz","="); stt.TxSub("N","Ed","/"); stt.TxSub("χN","pl,Rd","=");
	// ("Γ","bz","="); 
	stt.TxV(GlobalResult::BucklingNbz_Check.floatVal[0],2);
	// 判断
	double Rbz = GlobalResult::BucklingNbz_Check.floatVal[0];
	if (Rbz <=1.0)//Γbz<=1.0
	{
		stt.Txt("≤1.0");
	}
	else
	{
		stt.Txt(">1.0");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

	//强度计算
	stt.BeginP();
	stt.Txt(307,210);
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.Myz_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.Myz_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","My,N","="); stt.TxSub("M","y,Ed","/"); stt.TxSub("M","y,pl,N,Rd","=");
	// ("Γ","My,N","="); 
	stt.TxV(GlobalResult::My_Check.floatVal[0],2);
	// 判断
	double RmyN = GlobalResult::My_Check.floatVal[0];
	if (RmyN<=0.8)//ΓMyN<=0.8
	{
		stt.Txt("≤0.8");
	}
	else
	{
		stt.Txt(">0.8");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.Myz_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.Myz_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","Mz,N","="); stt.TxSub("M","z,Ed","/"); stt.TxSub("M","z,pl,N,Rd","=");
	// ("Γ","Mz,N","="); 
	stt.TxV(GlobalResult::Mz_Check.floatVal[0],2);
	// 判断
	double RMzN = GlobalResult::Mz_Check.floatVal[0];
	if (RMzN<=0.8)
	{
		stt.Txt("≤0.8");
	}
	else
	{
		stt.Txt(">0.8");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,110);
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.Myz_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.Myz_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","Myz,N","="); stt.TxSub("M","y,Ed","/"); stt.TxSub("M","y,pl,N,Rd","+"); stt.TxSub("M","z,Ed","/"); stt.TxSub("M","z,pl,N,Rd","=");
	// ("Γ","Myz,N","="); 
	stt.TxV(GlobalResult::MNV_Check.floatVal[0],2);
	// 判断
	double RMyzN = GlobalResult::MNV_Check.floatVal[0];
	if (RMyzN<=1.0)//ΓMyzN<=1.0
	{
		stt.Txt("≤1.0");
	}
	else
	{
		stt.Txt(">1.0");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

		
	//考虑二阶效应的弯矩
	stt.BeginP();
	stt.Txt(307,230);
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.bMyz_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.bMyz_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","My,N","="); stt.TxSub("M","y,Ed","/"); stt.TxSub("M","y,pl,N,Rd","=");
	// ("Γ","My,N","="); 
	stt.TxV(GlobalResult::bMy_Check.floatVal[0],2);
	// 判断
	RmyN = GlobalResult::bMy_Check.floatVal[0];
	if (RmyN<=0.8)//ΓMyN<=0.8
	{
		stt.Txt("≤0.8");
	}
	else
	{
		stt.Txt(">0.8");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.bMyz_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.bMyz_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","Mz,N","="); stt.TxSub("M","z,Ed","/"); stt.TxSub("M","z,pl,N,Rd","=");
	// ("Γ","Mz,N","="); 
	stt.TxV(GlobalResult::bMz_Check.floatVal[0],2);
	// 判断
	RMzN = GlobalResult::bMz_Check.floatVal[0];
	if (RMzN<=0.8)
	{
		stt.Txt("≤0.8");
	}
	else
	{
		stt.Txt(">0.8");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,110);
	stt.EndP();

	stt.BeginP();
	stt.Txt(307,30);
	// 组合号
	stt.TxV(GlobalResult::loadcomb.bMyz_comb);
	stt.Txt(307,50);
	//截面位置
	comb_pos = Tools::GetPos(GlobalResult::loadcomb.bMyz_pos);
	stt.m(comb_pos);
	stt.Txt(":");
	stt.EndP();

	stt.BeginP();
	stt.TxSub("Γ","Myz,N","="); stt.TxSub("M","y,Ed","/"); stt.TxSub("M","y,pl,N,Rd","+"); stt.TxSub("M","z,Ed","/"); stt.TxSub("M","z,pl,N,Rd","=");
	// ("Γ","Myz,N","="); 
	stt.TxV(GlobalResult::bMNV_Check.floatVal[0],2);
	// 判断
	RMyzN = GlobalResult::bMNV_Check.floatVal[0];
	if (RMyzN<=1.0)//ΓMyzN<=1.0
	{
		stt.Txt("≤1.0");
	}
	else
	{
		stt.Txt(">1.0");
		stt.SpTab______(); stt.Txt(307,170);
	}
	stt.EndP();
	return true;

}
// 	// 组合号
// 	stt.Txt("01");
// 	stt.Txt(307,50);
// 	//截面位置
// 	stt.m(0);
// 	stt.Txt(":");
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("Γ","b,My,N","="); stt.TxSub("M","y,Ed","/"); stt.TxSub("M","by,pl,N,Rd","=");
// 	// ("Γ","b,My,N","="); 
// 	stt.TxV(0.6818);
// 	// 判断
// 	double RbmyN = 0.1;
// 	if (RbmyN <=0.8)//ΓbMyN<=0.8
// 	{
// 		stt.Txt("≤0.8");
// 	}
// 	else
// 	{
// 		stt.Txt(">0.8");
// 		stt.SpTab______(); stt.Txt(307,170);
// 	}
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(307,30);
// 	// 组合号
// 	stt.Txt("01");
// 	stt.Txt(307,50);
// 	//截面位置
// 	stt.m(0);
// 	stt.Txt(":");
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("Γ","b,Mz,N","="); stt.TxSub("M","z,Ed","/"); stt.TxSub("M","bz,pl,N,Rd","=");
// 	// ("Γ","b,Mz,N","="); 
// 	stt.Txt(0);
// 	// 判断
// 	//double RMzN;
// 	if (RMzN<=0.8)
// 	{
// 		stt.Txt("≤0.8");
// 	}
// 	else
// 	{
// 		stt.Txt(">0.8");
// 		stt.SpTab______(); stt.Txt(307,170);
// 	}
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(307,110);
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.Txt(307,30);
// 	// 组合号
// 	stt.Txt("01");
// 	stt.Txt(307,50);
// 	//截面位置
// 	stt.m(0);
// 	stt.Txt(":");
// 	stt.EndP();
// 
// 	stt.BeginP();
// 	stt.TxSub("Γ","b,Myz,N","="); stt.TxSub("M","y,Ed","/"); stt.TxSub("M","by,pl,N,Rd","+"); stt.TxSub("M","z,Ed","/"); stt.TxSub("M","bz,pl,N,Rd","=");
// 	// ("Γ","b,Myz,N","="); 
// 	stt.Txt(0);
// 	// 判断
// 	//double RMyzN = 0.5;
// 	if (RMyzN<=1.0)
// 	{
// 		stt.Txt("≤1.0");
// 	}
// 	else
// 	{
// 		stt.Txt(">1.0");
// 		stt.SpTab______(); stt.Txt(307,170);
// 	}
// 	stt.EndP();



bool CPk_MainOutPut::SERVICEABILITY_LIMIT_STATES(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	// 第四章 SLS
	return true;
}

bool CPk_MainOutPut::CRACK_CHECK(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	// 4.1计算裂缝
	// 注意：只计算fully 非圆形截面！
	int cros_kind = GPara::m_Sect_in->m_kind;
	if(cros_kind!=2 && cros_kind!=5 && cros_kind!=8){	//判断是否可以计算裂缝
	stt.BeginP();
	stt.Txt(401,10);
	stt.EndP();

	stt.BeginP();
	stt.Txt(401,30);
	stt.EndP();

	stt.BeginP();
	stt.Txt(401,40);
	// 求出最大钢筋应力 所在组合号
	stt.TxV(1);
	stt.Txt(401,50);
	// 求出最大钢筋应力截面所在位置x
	stt.mm(0);
	stt.Txt(401,70);
	stt.EndP();

	stt.BeginP();
	stt.Txt(401,90); stt.TxSub("σ","s"," = ");
	//用纤维模型求出钢筋应力
	stt.MPa(GPara::m_DesnPara->BarQ);
	stt.EndP();

	stt.BeginP();
	stt.Txt(401,110);
	stt.EndP();

//	stt.BeginP();
//	stt.EndP();

// 	stt.BeginP();
// 	stt.TxSub("f","ct,eff_y"," = "); stt.TxSub("f","ctm"," = ");
// 	// fctm
// 	stt.MPa(4.07);
// 	stt.EndP();

	stt.BeginP();
	stt.TxSub("α","e"," = ");
	// αe
//	stt.Txt("11.40");
	stt.TxV(GlobalResult::Crack_Desn.ae);
	stt.EndP();

	stt.BeginP();
	stt.Txt(401,290);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("h","c,ef"," = ");
	stt.Txt(401,150);
	// hc,ef
	stt.mm(GlobalResult::Crack_Desn.hcef_y);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("A","c,eft"," = ");
	// Ac.eft
	stt.mm2(GlobalResult::Crack_Desn.Aceff_y);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("ρ","p,eff"," = ");
	// ρp.eff
	stt.TxV(GlobalResult::Crack_Desn.peff_y);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("ε","sm",""); stt.Txt("-"); stt.TxSub("ε","cm","=");
	// εsm-εcm
//	stt.Txt("0.000851");
	CString temp1;
	temp1.Format(_T("%.5f"),GlobalResult::Crack_Desn.em_y);
	stt.Txt(temp1);
//	stt.TxV(GlobalResult::Crack_Desn.em_y,);
	stt.EndP();

	stt.BeginP();
	stt.EndP();

	// 求:
	double space = GPara::m_Sect_in->Bar_interval; //The spacing of the bonded reinforcement
	double c= GPara::m_Sect_in->Con_coverd_x;
	double fai= GPara::m_Sect_in->Bar_d;

	if (space<=5*(c+fai/2))
	{
		stt.BeginP();
		stt.Txt(401,170); stt.TxV(space); stt.Txt(401,190); stt.mm(5*(c+fai/2));
		stt.EndP();

		stt.BeginP();
		stt.TxSub("k","1","="); stt.Txt("0.8.");
		stt.EndP();

		stt.BeginP();
		stt.TxSub("k","2","="); stt.Txt("0.5.");
		stt.EndP();

		stt.BeginP();
		stt.TxSub("k","3","="); stt.Txt("3.4.");
		stt.EndP();

		stt.BeginP();
		stt.TxSub("k","4","="); stt.Txt("0.425.");
		stt.EndP();

		stt.BeginP();
		stt.TxSub("s","r,max","=");
		// sr,max
		stt.mm(GlobalResult::Crack_Desn.srmax_y);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(401,170); stt.TxV(space); stt.Txt(401,210); stt.mm(5*(c+fai/2));
		stt.EndP();

		stt.BeginP();
		stt.TxSub("s","r,max","="); stt.Txt(401,230);
		// sr,max
		stt.mm(GlobalResult::Crack_Desn.srmax_y);
		stt.EndP();
	}

	stt.BeginP();
	stt.TxSub("w","k","=");
	// wk
	temp1.Format(_T("%.3f mm"),GlobalResult::Crack_Check1.CalVal);
	stt.Txt(temp1);
	stt.EndP();

	stt.BeginP();
	stt.Txt(401,250);
	stt.EndP();

	stt.BeginP();
	stt.Txt(401,270);
	// 环境等级
	if(GPara::m_DesnPara->Exp_Class==0)
		stt.Txt("X0,XC1");
	else if(GPara::m_DesnPara->Exp_Class==1)
		stt.Txt("XC2,XC3,XC4/XD1,XD2,XS1,XS2,XS3");
	stt.Txt(", "); stt.TxSub("w","max","=");
	// wmax
	temp1.Format(_T("%.3f mm"),GlobalResult::Crack_Check1.PermitVal_upper);
	stt.Txt(temp1); stt.Txt(".");
	stt.EndP();

	stt.BeginP();
	stt.Txt("∴ "); stt.TxSub("w","k");

	// 判断大小
//	double wk = 1.0;
//	double wmax = 2.0;
	if (GlobalResult::Crack_Check1.iOK)
		stt.Txt("≤");
	else
		stt.Txt(">");

	stt.TxSub("w","max");
	stt.EndP();

	//另一个方向
	stt.BeginP();
	stt.Txt(401,310);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("h","c,ef"," = ");
	stt.Txt(401,150);
	// hc,ef
	stt.mm(GlobalResult::Crack_Desn.hcef_z);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("A","c,eft"," = ");
	// Ac.eft
	stt.mm2(GlobalResult::Crack_Desn.Aceff_z);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("ρ","p,eff"," = ");
	// ρp.eff
	stt.TxV(GlobalResult::Crack_Desn.peff_z);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("ε","sm",""); stt.Txt("-"); stt.TxSub("ε","cm","=");
	// εsm-εcm
	//	stt.Txt("0.000851");
	temp1.Format(_T("%.5f"),GlobalResult::Crack_Desn.em_z);
	stt.Txt(temp1);
	stt.EndP();

	stt.BeginP();
	stt.EndP();

	// 求:
//	double space = GPara::m_Sect_in->Bar_interval; //The spacing of the bonded reinforcement
	double cover= GPara::m_Sect_in->Con_coverd_y;
//	double fai= GPara::m_Sect_in->Bar_d;

	if (space<=5*(c+fai/2))
	{
		stt.BeginP();
		stt.Txt(401,170); stt.TxV(space); stt.Txt(401,190); stt.mm(5*(c+fai/2));
		stt.EndP();

		stt.BeginP();
		stt.TxSub("k","1","="); stt.Txt("0.8.");
		stt.EndP();

		stt.BeginP();
		stt.TxSub("k","2","="); stt.Txt("0.5.");
		stt.EndP();

		stt.BeginP();
		stt.TxSub("k","3","="); stt.Txt("3.4.");
		stt.EndP();

		stt.BeginP();
		stt.TxSub("k","4","="); stt.Txt("0.425.");
		stt.EndP();

		stt.BeginP();
		stt.TxSub("s","r,max","=");
		// sr,max
		stt.mm(GlobalResult::Crack_Desn.srmax_z);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(401,170); stt.TxV(space); stt.Txt(401,210); stt.mm(5*(c+fai/2));
		stt.EndP();

		stt.BeginP();
		stt.TxSub("s","r,max","="); stt.Txt(401,230);
		// sr,max
		stt.mm(GlobalResult::Crack_Desn.srmax_z);
		stt.EndP();
	}

	stt.BeginP();
	stt.TxSub("w","k","=");
	// wk
	temp1.Format(_T("%.3f mm"),GlobalResult::Crack_Check2.CalVal);
	stt.Txt(temp1);
	stt.EndP();

	stt.BeginP();
	stt.Txt(401,250);
	stt.EndP();

	stt.BeginP();
	stt.Txt(401,270);
	// 环境等级
	if(GPara::m_DesnPara->Exp_Class==0)
		stt.Txt("X0,XC1");
	else if(GPara::m_DesnPara->Exp_Class==1)
		stt.Txt("XC2,XC3,XC4/XD1,XD2,XS1,XS2,XS3");
	stt.Txt(", "); stt.TxSub("w","max","=");
	// wmax
	temp1.Format(_T("%.3f mm"),GlobalResult::Crack_Check2.PermitVal_upper);
	stt.Txt(temp1); stt.Txt(".");
	stt.EndP();

	stt.BeginP();
	stt.Txt("∴ "); stt.TxSub("w","k");

	// 判断大小
	//	double wk = 1.0;
	//	double wmax = 2.0;
	if (GlobalResult::Crack_Check2.iOK)
		stt.Txt("≤");
	else
		stt.Txt(">");

	stt.TxSub("w","max");
	stt.EndP();
	}

	return true;
}

bool CPk_MainOutPut::MEMBER_CHECK1(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	//第5章，构件检查
	//适用于fully && 非圆形 截面。
	stt.BeginP();
	stt.Txt(501,10);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,30);
	stt.EndP();

	// cx
	double cx = GPara::m_Sect_in->Con_coverd_x;
	stt.BeginP();
	stt.TxSub("c","y","=");
	
	stt.mm(cx);
	stt.SpTab______(); stt.Txt(501,40);
	// 范围
	stt.mm(GlobalResult::Con_CoverCheck_y.PermitVal_lower);
	stt.Txt("~");
	stt.mm(GlobalResult::Con_CoverCheck_y.PermitVal_upper);
	stt.EndP();

//	double minCx=1;
//	double maxCx =2;
	//  
	if (GlobalResult::Con_CoverCheck_y.iOK)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	//z 侧
	double cy = GPara::m_Sect_in->Con_coverd_y;
	stt.BeginP();
	stt.TxSub("c","y","=");
	// cy
	stt.mm(cy);
	stt.SpTab______(); stt.Txt(501,40);
	// 范围
	stt.mm(GlobalResult::Con_CoverCheck_z.PermitVal_lower);
	stt.Txt("~");
	stt.mm(GlobalResult::Con_CoverCheck_z.PermitVal_upper);
	stt.EndP();

	//  
//	double minCy=0.1,maxCy = 1.0;
//	double cy = 0.5;
	if (GlobalResult::Con_CoverCheck_z.iOK)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,90);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,110);
	// fc
	stt.MPa(50);
	stt.SpTab______(); stt.Txt(501,40); stt.Txt(501,130); 
	stt.EndP();

	double fc=GPara::m_Sect_in->Concrete_mat;
	// 判断强度
	double minFc = 20,maxFc=50;
	if (minFc<=fc && fc<=maxFc)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,150);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("h","1",""); stt.Txt("/"); stt.TxSub("h","2","=");
	// h1/h2
	CString temp1;
	temp1.Format(_T("%2.f"),GlobalResult::Con_HBCheck.CalVal);
	stt.Txt(temp1);
//	stt.TxV(GlobalResult::Con_HBCheck.CalVal);
	stt.SpTab______(); stt.Txt(501,40); stt.Txt("0.2~5.0. "); 
	stt.EndP();

	// 判断长宽比
	double r=GlobalResult::Con_HBCheck.CalVal;
// 	double minR = 0.2;
// 	double maxR = 5;
	if (GlobalResult::Con_HBCheck.iOK)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,170);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,190);
	stt.EndP();

	stt.BeginP();
	stt.Txt("δ=");
	// δ
	temp1.Format(_T("%.3f"),GlobalResult::Pro_RatioCheck.CalVal);
	stt.Txt(temp1);
//	stt.TxV(GlobalResult::Pro_RatioCheck.CalVal);
	stt.SpTab______(); stt.Txt(501,40); stt.Txt("0.2~0.9. "); 
	stt.EndP();

	// 判断含钢率
	double delta = GlobalResult::Pro_RatioCheck.CalVal;
	double minDelta = 0.2;
	double maxDelta = 0.9;
	if (GlobalResult::Pro_RatioCheck.iOK)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,210);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("f","y","=");
	// fy
	stt.MPa(GPara::m_Sect_in->Bar_mat);
	stt.SpTab______(); stt.Txt(501,40); stt.Txt(501,230); 
	stt.EndP();

	// 判断强度
	double fy = GPara::m_Sect_in->Bar_mat;
	double minFy=235,maxFy = 460;
	if (minFy<=fy && fy<=maxFy)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,250);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,270);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("A","sr","/"); stt.TxSub("A","c","=");
	// Asr/Ac
	temp1.Format(_T("%.2f"),GlobalResult::Bar_RatioCheck.CalVal);
	stt.Txt(temp1);
	stt.Txt("%");
	stt.SpTab______(); stt.Txt(501,40); stt.Txt("0.30%~6.00% . "); 
	stt.EndP();

	// 判断配筋率
	double rou = GlobalResult::Bar_RatioCheck.CalVal;
	double minRou = 0.3;
	double maxRou = 6;
	if (GlobalResult::Bar_RatioCheck.iOK)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,290);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,310);
	stt.EndP();

	// 判断角点钢筋
	bool allExist = true;
	if (allExist==true)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,330);
	stt.EndP();

	//纵筋间距
	stt.BeginP();
	stt.Txt(501,350);
	// 间距
	stt.mm(GlobalResult::Bar_SpaceCheck.CalVal);
	stt.SpTab______(); stt.Txt(501,370);
	// 最小值
	stt.mm(GlobalResult::Bar_SpaceCheck.PermitVal_lower); stt.Txt(" .");
	stt.EndP();

	// 判断钢筋间距
	double clsp = GlobalResult::Bar_SpaceCheck.CalVal;
	double minClsp = GlobalResult::Bar_SpaceCheck.PermitVal_lower;
	if (GlobalResult::Bar_SpaceCheck.iOK)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,390);
	stt.EndP();

	//最小直径
	double ds = GlobalResult::Bar_BarDCheck.CalVal;
	stt.BeginP();
	stt.TxSub("d","s","=");
	// ds
	stt.mm(ds);
	stt.SpTab______(); stt.Txt(501,370); stt.mm(GlobalResult::Bar_BarDCheck.PermitVal_lower);
	stt.EndP();

	// 判断钢筋直径
	if (GlobalResult::Bar_BarDCheck.iOK)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,410);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,430);
	stt.EndP();

	stt.BeginP();
	stt.Txt("λy=");
	// λ
	temp1.Format(_T("%.2f"),GlobalResult::ll_y);
	stt.Txt(temp1);

//	stt.TxV(GlobalResult::ll_y);
	stt.SpTab______(); stt.Txt(501,450); stt.Txt("2 . ");
	stt.EndP();

	// 判断λ
	double lamd = GlobalResult::ll_y;
	if (lamd<=2)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt("λz=");
	// λ
	temp1.Format(_T("%.2f"),GlobalResult::ll_z);
	stt.Txt(temp1);
	stt.SpTab______(); stt.Txt(501,450); stt.Txt("2 . ");
	stt.EndP();
	lamd = GlobalResult::ll_z;
	if (lamd<=2)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	//增加15%偏心输出
	stt.BeginP();
	stt.Txt("Eccentricity along y-y axis:");
	temp1.Format(_T("%.3f"),GlobalResult::MN15y_Check.CalVal*100);
	stt.Txt(temp1);stt.Txt("%");
	stt.EndP();

	stt.BeginP();
	stt.Txt("Eccentricity along z-z axis:");
	temp1.Format(_T("%.3f"),GlobalResult::MN15z_Check.CalVal*100);
	stt.Txt(temp1);stt.Txt("%");
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,570);
	stt.EndP();





	return true;
}

bool CPk_MainOutPut::MEMBER_CHECK2(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	//第5章，构件检查
	//适用于fully && 圆形 截面。
	stt.BeginP();
	stt.Txt(501,10);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,30);
	stt.EndP();

	// c
	double c = 70.1;
	stt.BeginP();
	stt.Txt("c=");stt.TxV(c);
	

	stt.mm(86);
	stt.SpTab______(); stt.Txt(501,40);
	// 范围
	stt.mm(40);
	stt.Txt("~");
	stt.mm(921);
	stt.EndP();

	// 
	double minC= 1;
	double maxC = 20;
	if (minC<=c && c<=maxC)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,90);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,110);
	// fc
	stt.MPa(50);
	stt.SpTab______(); stt.Txt(501,40); stt.Txt(501,130); 
	stt.EndP();

	// 判断强度
	double minFc = 0.1,maxFc=1.0;
	double fc = 1;
	if (minFc<=fc && fc<=maxFc)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,170);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,190);
	stt.EndP();

	stt.BeginP();
	stt.Txt("δ=");
	// δ
	stt.TxV(0.45);
	stt.SpTab______(); stt.Txt(501,40); stt.Txt("0.2~0.9. "); 
	stt.EndP();

	// 判断含钢率
	double minDelta=0.1,maxDelta=1;
	double delta = 0.5;
	if (minDelta<=delta && delta<=maxDelta)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,210);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("f","y","=");
	// fy
	stt.MPa(450);
	stt.SpTab______(); stt.Txt(501,40); stt.Txt(501,230); 
	stt.EndP();

	// 判断强度
	double fy = 100;
	double minFy = 10,maxFy = 200;
	if (minFy<=fy && fy<=maxFy)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,250);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,270);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("A","sr","/"); stt.TxSub("A","c","=");
	// Asr/Ac
	stt.Txt("3.31%");
	stt.SpTab______(); stt.Txt(501,40); stt.Txt("0.30%~6.00% . "); 
	stt.EndP();

	// 判断配筋率
	double rou = 1;
	double minRou=0.1,maxRou=1.0;
	if (minRou<=rou && rou<=maxRou)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}



	stt.BeginP();
	stt.Txt(501,470);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,490);
	// 间距
	stt.mm(100);
	stt.SpTab______(); stt.Txt(501,370);
	// 最小值
	stt.mm(40); stt.Txt(" .");
	stt.EndP();

	// 判断钢筋间距
	double clsp=2;
	double minClsp=1;
	if (minClsp<=clsp)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,510);
	stt.EndP();

	double ds = 10;
	stt.BeginP();
	stt.TxSub("d","s","="); stt.TxV(ds);
	// ds
	stt.mm(40);
	stt.SpTab______(); stt.Txt(501,370); stt.mm(8);
	stt.EndP();

	// 判断钢筋直径
	
	if (8<=ds)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,410);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,430);
	stt.EndP();

	stt.BeginP();
	stt.Txt("λ=");
	// λ
	stt.TxV(0.311);
	stt.SpTab______(); stt.Txt(501,450); stt.Txt("2 . ");
	stt.EndP();

	// 判断λ
	double lamd = 1;
	if (lamd<=2)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	return true;
}

bool CPk_MainOutPut::MEMBER_CHECK3(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	//第5章，构件检查
	//适用于partially 截面。
	stt.BeginP();
	stt.Txt(501,10);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,30);
	stt.EndP();

	stt.BeginP();
	stt.Txt("c=");
	// c
	stt.mm(86);
	stt.SpTab______(); stt.Txt(501,370);
	// cnom
	stt.mm(40);
	stt.EndP();

	//  
	double c = 0.1;
	double minC = 0.01;
	if (minC<=c)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.TxSub("c","y","=");
	// cy
	stt.mm(86);
	stt.SpTab______(); stt.Txt(501,40);
	// 范围
	stt.mm(50);
	stt.Txt("~");
	stt.mm(1228.8);
	stt.EndP();

	//  
	double cy = 0.5;
	double minCy=0.1,maxCy=1;
	if (minCy<=cy && cy<=maxCy)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,90);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,110);
	// fc
	stt.MPa(50);
	stt.SpTab______(); stt.Txt(501,40); stt.Txt(501,130); 
	stt.EndP();

	// 判断强度
	double fc = 1.5;
	double minFc=1,maxFc = 2;
	if (minFc<=fc && maxFc<=maxFc)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,150);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("h","1",""); stt.Txt("/"); stt.TxSub("h","2","=");
	// h1/h2
	stt.TxV(1);
	stt.SpTab______(); stt.Txt(501,40); stt.Txt("0.2~5.0. "); 
	stt.EndP();

	// 判断长宽比
	double r = 1;
	double minR = 0.5;
	double maxR = 2.0;
	if (minR<=r && r<=maxR)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,170);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,190);
	stt.EndP();

	stt.BeginP();
	stt.Txt("δ=");
	// δ
	stt.TxV(0.45);
	stt.SpTab______(); stt.Txt(501,40); stt.Txt("0.2~0.9. "); 
	stt.EndP();

	// 判断含钢率
	double delta = 1;
	double minDelta = 0.5;
	double maxDelta = 2;
	if (minDelta<=delta && delta<=maxDelta)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,210);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("f","y","=");
	// fy
	stt.MPa(450);
	stt.SpTab______(); stt.Txt(501,40); stt.Txt(501,230); 
	stt.EndP();

	// 判断强度
	double fy=100;
	double minFy = 50,maxFy = 200;
	if (minFy<=fy && fy<=maxFy)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,530);
	stt.EndP();

	stt.BeginP();
	stt.Txt("b/"); stt.TxSub("t","f","=");
	// b/tf
	stt.TxV(22);
	stt.SpTab______(); stt.Txt(501,450); 
	//限值
	stt.TxV(44); stt.Txt(" . ");
	stt.EndP();

	// 判断宽厚比
	double bt = 0.1;
	double maxBt = 1;
	if (bt<=maxBt)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}


	stt.BeginP();
	stt.Txt(501,250);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,270);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("A","sr","/"); stt.TxSub("A","c","=");
	// Asr/Ac
	stt.Txt("3.31%");
	stt.SpTab______(); stt.Txt(501,40); stt.Txt("0.30%~6.00% . "); 
	stt.EndP();

	// 判断配筋率
	double rou = 2;
	double minRou = 1;
	double maxRou = 3;
	if (minRou<=rou && rou<=maxRou)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}


	stt.BeginP();
	stt.Txt(501,550);
	stt.EndP();

	stt.BeginP();
	stt.TxSub("d","s","=");
	// ds
	stt.mm(40);
	stt.SpTab______(); stt.Txt(501,370); stt.mm(8);
	stt.EndP();

	// 判断钢筋直径
	double ds = 10;
	if (8<=ds)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}

	stt.BeginP();
	stt.Txt(501,410);
	stt.EndP();

	stt.BeginP();
	stt.Txt(501,430);
	stt.EndP();

	stt.BeginP();
	stt.Txt("λ=");
	// λ
	stt.TxV(0.311);
	stt.SpTab______(); stt.Txt(501,450); stt.Txt("2 . ");
	stt.EndP();

	// 判断λ
	double lamd = 1;
	if (lamd<=2)
	{
		stt.BeginP();
		stt.Txt(501,50);
		stt.EndP();
	}
	else
	{
		stt.BeginP();
		stt.Txt(501,70);
		stt.EndP();
	}


	return true;
}

bool CPk_MainOutPut::CODE_STANDARD(sd::IGenerateText* ,const sd::REPORT_ITEM&)
{
	stt.BeginP();
	stt.Txt(101,120);
	stt.EndP();

	stt.BeginP();
	stt.Txt(101,130);
	stt.EndP();

	stt.BeginP();
	stt.Txt(101,140);
	stt.EndP();

	return true;
}

//给任意截面用的截面检查
bool CPk_MainOutPut::MEMBER_CHECK4(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem)
{
	return true;
}

bool CPk_MainOutPut::ClearCalcTemp()
{
	CString path;
	::GetCurrentDirectory(MAX_PATH,path.GetBuffer(MAX_PATH));
	path.ReleaseBuffer(-1);
	path+= _T("\\");

	CString path2 = path;
	path2 += pathCalcTemp;
	path2 += _T("\\");

	CFileFind tempFind;

	CString strAllfile;
	strAllfile.Format(_T("%s\\*.*"),path2);
	BOOL isFinded = tempFind.FindFile(strAllfile);

	CString strFileDirName2;
	while(isFinded)
	{
		isFinded = tempFind.FindNextFile();
		if (!tempFind.IsDots())
		{
			strFileDirName2 = tempFind.GetFilePath();
			strFileDirName2.MakeUpper();
			if (strFileDirName2.Find("-CAL.T")>=0)
			{
				continue;
			}
			::DeleteFile(strFileDirName2);
		}
	}

	return true;

}

void CPk_MainOutPut::MTable( sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem,int currentcomb,const int yz)
{
	//sd::REPORT_ITEM I2 = mReportItem;
	Force_combine Colmforce;
	Colmforce.GetForce();
	Colmforce.HeZai_combine();

	float kky = GPara::kk_y[currentcomb];
	float kkz = GPara::kk_z[currentcomb];
	double Myed1,Mzed1; //+
	double Myed2,Mzed2;	//-
	CArray<double> M11;
	CArray<double> M12;
	CArray<double> M13;
	CArray<double> M21;
	CArray<double> M22;
	CArray<double> M23;
	CArray<double> VVy;
	CArray<double> VVz;
	CArray<double> rouy;
	CArray<double> rouz;

	for(int j=0;j<Parts;j++){
		Myed1 = kky*Colmforce.My_zuhe[currentcomb][j];
		Myed2 = Colmforce.My_zuhe[currentcomb][j] - (kky-1)*Colmforce.My_zuhe[currentcomb][j];
		Mzed1 = kkz*Colmforce.Mz_zuhe[currentcomb][j];
		Mzed2 = Colmforce.Mz_zuhe[currentcomb][j] - (kkz-1)*Colmforce.Mz_zuhe[currentcomb][j];
		M11.Add(Colmforce.My_zuhe[currentcomb][j]);
		M12.Add(Myed1);
		M13.Add(Myed2);
		M21.Add(Colmforce.Mz_zuhe[currentcomb][j]);
		M22.Add(Mzed1);
		M23.Add(Mzed2);
		VVy.Add(Colmforce.Vy_zuhe[currentcomb][j]);
		VVz.Add(Colmforce.Vy_zuhe[currentcomb][j]);
		rouy.Add(GlobalResult::Vrou_y[currentcomb][j]);
		rouz.Add(GlobalResult::Vrou_z[currentcomb][j]);
	}

	CELL_FORMAT cFormat;
	TABLE_FORMAT tFormat;

	tFormat.borderStyle=mReportItem.tableStyle;
	//表头0行
	tFormat.headerRows=1;
	//表格标题
	TEXT_FORMAT titleTextFormat;
	titleTextFormat.bold=true;
	tFormat.titleFormat.setFormat(titleTextFormat);
	tFormat.titleFormat.align=mReportItem.tableTitleAlign;
	CString title;
	if(yz==1)
		title.Format(_T("ULS%d (y-y axis)"),currentcomb);
	else
		title.Format(_T("ULS%d (z-z axis)"),currentcomb);
	tFormat.titleFormat.setTitle(title);

	tFormat.align = sdAlignCenter; //整个表作为一个整体在纸张中的位置
	int tbRowHeight = 3;
	if(FILE_PDF == m_doctype)
	{
		tbRowHeight = 15;
	}
	//柱号	　　标准截面号	约束信息	截面布置角度
	tFormat.setColNum(8);
	//设置列宽
	tFormat.setColWidth(0,15);
	tFormat.setColWidth(1,15);
	tFormat.setColWidth(2,25);
	tFormat.setColWidth(3,25);
	tFormat.setColWidth(4,25);
	tFormat.setColWidth(5,25);
	tFormat.setColWidth(6,25);
	tFormat.setColWidth(7,25);

	pGenerateText->beginTable(&tFormat);

	cFormat.font = mReportItem.fontTableContent;
	cFormat.align = sdAlignCenter;
	cFormat.colorFill = sdColorBlackLightGray1;
	//第一行
	pGenerateText->beginTableRow(tbRowHeight);
	pGenerateText->insertTableText(0,"No.",&cFormat);
	pGenerateText->insertTableText(1,"x(m)",&cFormat);
	if(yz==1){
		pGenerateText->insertTableText(2,"Vy,Ed(kN)",&cFormat);
		pGenerateText->insertTableText(3,"ρy",&cFormat);
		pGenerateText->insertTableText(4,"My(kN.m)",&cFormat);
		pGenerateText->insertTableText(5,"Myb(+)",&cFormat);
		pGenerateText->insertTableText(6,"Myb(-)",&cFormat);
	}
	else{
		pGenerateText->insertTableText(2,"Vz,Ed(kN)",&cFormat);
		pGenerateText->insertTableText(3,"ρz",&cFormat);
		pGenerateText->insertTableText(4,"Mz(kN.m)",&cFormat);
		pGenerateText->insertTableText(5,"Mzb(+)",&cFormat);
		pGenerateText->insertTableText(6,"Mzb(-)",&cFormat);
	}
	pGenerateText->insertTableText(7,"k",&cFormat);


	pGenerateText->endTableRow();

	cFormat.colorFill = sdColorAuto;
	//第二行开始数据
	CString strNo1,strNo2;
	CString strM11;
	CString strM12;
	CString strM13;
	CString strM21;
	CString strM22;
	CString strM23;
	CString strVVy;
	CString strVVz;
	CString strry;
	CString strrz;
	CString strky;
	CString strkz;
	for (int i=0;i<M11.GetSize();i+=1)
	{
		pGenerateText->beginTableRow(tbRowHeight);

		strNo1.Format(_T("%d"),i+1);
		float Lens = Tools::GetPos(i);
		strNo2.Format(_T("%.1f"),Lens);

		strVVy.Format(_T("%.1f"),VVy[i]*1e-3);
		strry.Format(_T("%.1f"),rouy[i]);
		strM11.Format(_T("%.1f"),M11[i]*1e-6);
		strM12.Format(_T("%.1f"),M12[i]*1e-6);
		strM13.Format(_T("%.1f"),M13[i]*1e-6);

		strVVz.Format(_T("%.1f"),VVz[i]*1e-3);
		strrz.Format(_T("%.1f"),rouz[i]);
		strM21.Format(_T("%.1f"),M21[i]*1e-6);
		strM22.Format(_T("%.1f"),M22[i]*1e-6);
		strM23.Format(_T("%.1f"),M23[i]*1e-6);

		strky.Format(_T("%.1f"),kky);
		strkz.Format(_T("%.1f"),kkz);
		pGenerateText->insertTableText(0,strNo1,&cFormat);
		pGenerateText->insertTableText(1,strNo2,&cFormat);
		if(yz==1){
			pGenerateText->insertTableText(2,strVVy,&cFormat);
			pGenerateText->insertTableText(3,strry,&cFormat);
			pGenerateText->insertTableText(4,strM11,&cFormat);
			pGenerateText->insertTableText(5,strM12,&cFormat);
			pGenerateText->insertTableText(6,strM13,&cFormat);
			pGenerateText->insertTableText(7,strky,&cFormat);
		}
		else{
			pGenerateText->insertTableText(2,strVVz,&cFormat);
			pGenerateText->insertTableText(3,strrz,&cFormat);
			pGenerateText->insertTableText(4,strM21,&cFormat);
			pGenerateText->insertTableText(5,strM22,&cFormat);
			pGenerateText->insertTableText(6,strM23,&cFormat);
			pGenerateText->insertTableText(7,strkz,&cFormat);
		}

		pGenerateText->endTableRow();
	}


	pGenerateText->endTable();

}

//桁架模型时候多个配筋率下的验算结果
void CPk_MainOutPut::TrussModelTable(sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem,int side)
{
	CELL_FORMAT cFormat;
	TABLE_FORMAT tFormat;

	tFormat.borderStyle=mReportItem.tableStyle;
	//表头0行
	tFormat.headerRows=1;
	//表格标题
	TEXT_FORMAT titleTextFormat;
	titleTextFormat.bold=true;
	tFormat.titleFormat.setFormat(titleTextFormat);
	tFormat.titleFormat.align=mReportItem.tableTitleAlign;
	if(side==1)
		tFormat.titleFormat.setTitle("Truss Model aixs-y");
	else
		tFormat.titleFormat.setTitle("Truss Model aixs-z");

	tFormat.align = sdAlignCenter; //整个表作为一个整体在纸张中的位置
	int tbRowHeight = 3;
	if(FILE_PDF == m_doctype)
	{
		tbRowHeight = 15;
	}
	//柱号	　　标准截面号	约束信息	截面布置角度
	tFormat.setColNum(9);
	//设置列宽
	tFormat.setColWidth(0,20);
	tFormat.setColWidth(1,20);
	tFormat.setColWidth(2,20);
	tFormat.setColWidth(3,20);
	tFormat.setColWidth(4,20);
	tFormat.setColWidth(5,20);
	tFormat.setColWidth(6,20);
	tFormat.setColWidth(7,20);
	tFormat.setColWidth(8,20);
//	tFormat.setColWidth(9,20);

	pGenerateText->beginTable(&tFormat);

	cFormat.font = mReportItem.fontTableContent;
	cFormat.align = sdAlignCenter;
	cFormat.colorFill = sdColorBlackLightGray1;
	//第一行
	pGenerateText->beginTableRow(tbRowHeight);
	pGenerateText->insertTableText(0,"ρ",&cFormat);
	pGenerateText->insertTableText(1,"bc(mm)",&cFormat);
//	pGenerateText->insertTableText(2,"SRcbc(mm3)",&cFormat);
	pGenerateText->insertTableText(2,"bs(mm)",&cFormat);
//	pGenerateText->insertTableText(4,"Srcbs(mm3)",&cFormat);
	pGenerateText->insertTableText(3,"Vcbs(kN)",&cFormat);
	pGenerateText->insertTableText(4,"Vrdbond(kN)",&cFormat);
	pGenerateText->insertTableText(5,"Nstud",&cFormat);
	pGenerateText->insertTableText(6,"Vcbc(kN)",&cFormat);
	pGenerateText->insertTableText(7,"Vcbc_r(kN)",&cFormat);
	pGenerateText->insertTableText(8,"Vcbc_Check",&cFormat);
	pGenerateText->endTableRow();

	cFormat.colorFill = sdColorAuto;
	//第二行开始数据
	CString strings[10];
	for (int i=0;i<GlobalResult::Truss_y.GetSize();i+=1)
	{
		pGenerateText->beginTableRow(tbRowHeight);
		if(side==1){
			strings[0].Format(_T("%.2f%%"),GlobalResult::Truss_y[i].rou);
			strings[1].Format(_T("%.1f"),GlobalResult::Truss_y[i].bc);
//			strings[2].Format(_T("%.1f"),GlobalResult::Truss_y[i].Srcbc);
			strings[2].Format(_T("%.1f"),GlobalResult::Truss_y[i].bs);
//			strings[4].Format(_T("%.1f"),GlobalResult::Truss_y[i].Srcbs);
			strings[3].Format(_T("%.1f"),GlobalResult::Truss_y[i].Vcbs*0.001);
			strings[4].Format(_T("%.1f"),GlobalResult::Truss_y[i].Vrdbond*0.001);
			strings[5].Format(_T("%d"),GlobalResult::Truss_y[i].Nstud);
			strings[6].Format(_T("%1.f"),GlobalResult::Truss_y[i].Vcbc*0.001);
			strings[7].Format(_T("%1.f"),GlobalResult::Truss_y[i].Vcbc_r*0.001);
			if(GlobalResult::Truss_y[i].ifVcOk)
				strings[8].Format(_T("OK"));
			else
				strings[8].Format(_T("NotOK"));
		}
		else{
			strings[0].Format(_T("%.2f%%"),GlobalResult::Truss_z[i].rou);
			strings[1].Format(_T("%.1f"),GlobalResult::Truss_z[i].bc);
//			strings[2].Format(_T("%.1f"),GlobalResult::Truss_z[i].Srcbc);
			strings[2].Format(_T("%.1f"),GlobalResult::Truss_z[i].bs);
//			strings[4].Format(_T("%.1f"),GlobalResult::Truss_z[i].Srcbs);
			strings[3].Format(_T("%.1f"),GlobalResult::Truss_z[i].Vcbs*0.001);
			strings[4].Format(_T("%.1f"),GlobalResult::Truss_z[i].Vrdbond*0.001);
			strings[5].Format(_T("%d"),GlobalResult::Truss_z[i].Nstud);
			strings[6].Format(_T("%1.f"),GlobalResult::Truss_z[i].Vcbc*0.001);
			strings[7].Format(_T("%1.f"),GlobalResult::Truss_z[i].Vcbc_r*0.001);
			if(GlobalResult::Truss_z[i].ifVcOk)
				strings[8].Format(_T("OK"));
			else
				strings[8].Format(_T("NotOK"));
		}
		for(int j=0;j<10;j++){
			pGenerateText->insertTableText(j,strings[j],&cFormat);
		}
		pGenerateText->endTableRow();
	}


	pGenerateText->endTable();
}


//输出组合情况
void CPk_MainOutPut::LoadCombTable(sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem)
{
	CELL_FORMAT cFormat;
	TABLE_FORMAT tFormat;

	tFormat.borderStyle=mReportItem.tableStyle;
	//表头0行
	tFormat.headerRows=1;
	//表格标题
	TEXT_FORMAT titleTextFormat;
	titleTextFormat.bold=true;
	tFormat.titleFormat.setFormat(titleTextFormat);
	tFormat.titleFormat.align=mReportItem.tableTitleAlign;
	CString T_Title;
	T_Title.Format(_T("Load Combinations"));
	tFormat.titleFormat.setTitle(T_Title);

	tFormat.align = sdAlignCenter; //整个表作为一个整体在纸张中的位置
	int tbRowHeight = 3;
	if(FILE_PDF == m_doctype)
	{
		tbRowHeight = 15;
	}	

	tFormat.setColNum(5);
	//设置列宽
	tFormat.setColWidth(0,25);
	tFormat.setColWidth(1,30);
	tFormat.setColWidth(2,80);
	tFormat.setColWidth(3,30);
	tFormat.setColWidth(4,30);

	pGenerateText->beginTable(&tFormat);

	cFormat.font = mReportItem.fontTableContent;
	cFormat.align = sdAlignCenter;
	cFormat.colorFill = sdColorBlackLightGray1;

	//第一行
	pGenerateText->beginTableRow(tbRowHeight);
	pGenerateText->insertTableText(0,"No.",&cFormat);
	pGenerateText->insertTableText(1,"Name",&cFormat);
	pGenerateText->insertTableText(2,"Combination",&cFormat);
	pGenerateText->insertTableText(3,"β(y-y)",&cFormat);
	pGenerateText->insertTableText(4,"β(z-z)",&cFormat);
	pGenerateText->endTableRow();

	cFormat.colorFill = sdColorAuto;

	//第二行开始数据
	int nComb = GPara::m_DesnPara->n_Combines;
	CString strNo1;
	CString strName;
	CString strComb;
	CString strbetay,strbetaz;
	CString strtemp;
	for (int i=0;i<nComb;i++)
	{
		pGenerateText->beginTableRow(tbRowHeight);

		strNo1.Format(_T("%d"),i+1);
		strName.Format(_T("ULS%d"),i+1);
		strComb.Format(_T("%g%s"),GPara::m_DesnPara->f[0][i],GPara::m_DesnPara->notation[0]);
		if(GPara::m_DesnPara->f[1][i]>0){
			strtemp.Format(_T("+%g%s"),GPara::m_DesnPara->f[1][i],GPara::m_DesnPara->notation[1]);
			strComb.Append(strtemp);
		}
		if(GPara::m_DesnPara->f[2][i]>0){
			strtemp.Format(_T("+%g%s"),GPara::m_DesnPara->f[2][i],GPara::m_DesnPara->notation[2]);
			strComb.Append(strtemp);
		}
		if(GPara::m_DesnPara->f[3][i]>0){
			strtemp.Format(_T("+%g%s"),GPara::m_DesnPara->f[3][i],GPara::m_DesnPara->notation[3]);
			strComb.Append(strtemp);
		} 
		strbetay.Format(_T("%g"),GPara::beta_y[i]);
		strbetaz.Format(_T("%g"),GPara::beta_z[i]);
		pGenerateText->insertTableText(0,strNo1,&cFormat);
		pGenerateText->insertTableText(1,strName,&cFormat);
		pGenerateText->insertTableText(2,strComb,&cFormat);
		pGenerateText->insertTableText(3,strbetay,&cFormat);
		pGenerateText->insertTableText(4,strbetaz,&cFormat);

		pGenerateText->endTableRow();
	}

	pGenerateText->endTable();
}

void CPk_MainOutPut::LoadCaseTable(sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem,int caseno)
{
	Force_combine Colmforce;
	Colmforce.GetForce();
	Colmforce.HeZai_combine();

	CELL_FORMAT cFormat;
	TABLE_FORMAT tFormat;

	tFormat.borderStyle=mReportItem.tableStyle;
	//表头0行
	tFormat.headerRows=1;
	//表格标题
	TEXT_FORMAT titleTextFormat;
	titleTextFormat.bold=true;
	tFormat.titleFormat.setFormat(titleTextFormat);
	tFormat.titleFormat.align=mReportItem.tableTitleAlign;
	CString T_Title;
	T_Title.Format(_T("Load Case %d"),caseno+1);
	tFormat.titleFormat.setTitle(T_Title);

	tFormat.align = sdAlignCenter; //整个表作为一个整体在纸张中的位置
	int tbRowHeight = 3;
	if(FILE_PDF == m_doctype)
	{
		tbRowHeight = 15;
	}	

	tFormat.setColNum(7);
	//设置列宽
	tFormat.setColWidth(0,25);
	tFormat.setColWidth(1,25);
	tFormat.setColWidth(2,25);
	tFormat.setColWidth(3,25);
	tFormat.setColWidth(4,25);
	tFormat.setColWidth(5,25);
	tFormat.setColWidth(6,25);

	pGenerateText->beginTable(&tFormat);

	cFormat.font = mReportItem.fontTableContent;
	cFormat.align = sdAlignCenter;
	cFormat.colorFill = sdColorBlackLightGray1;
	//第一行
	pGenerateText->beginTableRow(tbRowHeight);
	pGenerateText->insertTableText(0,"No.",&cFormat);
	pGenerateText->insertTableText(1,"x(m)",&cFormat);
	pGenerateText->insertTableText(2,"N(kN)",&cFormat);
	pGenerateText->insertTableText(3,"My(kN.m)",&cFormat);
	pGenerateText->insertTableText(4,"Mz(kN.m)",&cFormat);
	pGenerateText->insertTableText(5,"Vy(kN)",&cFormat);
	pGenerateText->insertTableText(6,"Vz(kN)",&cFormat);
	pGenerateText->endTableRow();

	cFormat.colorFill = sdColorAuto;
	//第二行开始数据
	CString strNo1,strNo2;
	CString strN;
	CString strMy;
	CString strMz;
	CString strVy;
	CString strVz;
	for (int i=0;i<13;i+=1)
	{
		pGenerateText->beginTableRow(tbRowHeight);

		strNo1.Format(_T("%d"),i+1);
		float Lens = Tools::GetPos(i);
		strNo2.Format(_T("%.1f"),Lens);

		strN.Format(_T("%.1f"),Colmforce.N[caseno][i]*1.e-3);
		strMy.Format(_T("%1.f"),Colmforce.My[caseno][i]*1.e-6);
		strMz.Format(_T("%1.f"),Colmforce.Mz[caseno][i]*1.e-6);
		strVy.Format(_T("%1.f"),fabs(Colmforce.Vy[caseno][i]*1.e-3));
		strVz.Format(_T("%1.f"),fabs(Colmforce.Vz[caseno][i]*1.e-3));

		pGenerateText->insertTableText(0,strNo1,&cFormat);
		pGenerateText->insertTableText(1,strNo2,&cFormat);
		pGenerateText->insertTableText(2,strN,&cFormat);
		pGenerateText->insertTableText(3,strMy,&cFormat);
		pGenerateText->insertTableText(4,strMz,&cFormat);
		pGenerateText->insertTableText(5,strVy,&cFormat);
		pGenerateText->insertTableText(6,strVz,&cFormat);
		pGenerateText->endTableRow();
	}

	pGenerateText->endTable();

}
bool CPk_MainOutPut::CopyInCalcTemp(CString fileName,CString newName )
{
	CString path;
	::GetCurrentDirectory(MAX_PATH,path.GetBuffer(MAX_PATH));
	path.ReleaseBuffer(-1);
	path+= _T("\\");

	CString path2 = path;
	path2 += pathCalcTemp;
	path2 += _T("\\");
	CString fi1= path2 + fileName;
	CString fi2 = path2 + newName;
	//先删除旧的
	::DeleteFile(fi2);
	//在准备新的.
	return ::CopyFile(fi1,fi2,TRUE)==TRUE?true:false;

}
bool CPk_MainOutPut::CopyToCalcTemp(CString fileName,CString newName,bool delori)
{

	CString path;
	::GetCurrentDirectory(MAX_PATH,path.GetBuffer(MAX_PATH));
	path.ReleaseBuffer(-1);
	path+= _T("\\");

	CString path2 = path;
	path2 += pathCalcTemp;
	path2 += _T("\\");

	CString fi1 = path + fileName;
	CString fi2;
	if (newName.IsEmpty())
	{
		fi2 = path2 + fileName;
	}
	else
	{
		fi2 = path2 + newName;
	}


	if (_taccess_s(fi1,0)==0)
	{

		if (FALSE == ::PathIsDirectory(path2))
		{
			::CreateDirectory(path2,NULL);
		}
		if (FALSE == ::CopyFileA(fi1,fi2,false))
		{
			return false;
		}
		if (delori)
		{
			DeleteFile(fi1);
		}
	}


	return true;
}



bool CPk_MainOutPut::Load_case_one(sd::IGenerateText* pGenerateText, 
	const sd::REPORT_ITEM& mReportItem,const mega_loads& ldCase)
{
	
	CELL_FORMAT cFormat;
	TABLE_FORMAT tFormat;

	tFormat.borderStyle=mReportItem.tableStyle;
	//表头0行
	tFormat.headerRows=1;
	//表格标题
	TEXT_FORMAT titleTextFormat;
	titleTextFormat.bold=true;
	
	tFormat.titleFormat.setFormat(titleTextFormat);
	tFormat.titleFormat.align=mReportItem.tableTitleAlign;
	CString T_Title;
	T_Title.Format(_T("%s (%s)"),ldCase.Notatio,ldCase.Description);
	tFormat.titleFormat.setTitle(T_Title);

	tFormat.align = sdAlignCenter; //整个表作为一个整体在纸张中的位置
	int tbRowHeight = 3;
	if(FILE_PDF == m_doctype)
	{
		tbRowHeight = 15;
	}	

	tFormat.setColNum(7);
	//设置列宽
	tFormat.setColWidth(0,15);
	tFormat.setColWidth(1,28);
	tFormat.setColWidth(2,28);
	tFormat.setColWidth(3,28);
	tFormat.setColWidth(4,28);
	tFormat.setColWidth(5,28);
	tFormat.setColWidth(6,28);

	pGenerateText->beginTable(&tFormat);

	cFormat.font = mReportItem.fontTableContent;
	cFormat.align = sdAlignCenter;
	cFormat.colorFill = sdColorBlackLightGray1;
	SdColor oriColor = cFormat.colorBack ;
	cFormat.colorBack = sdColorBlackLightGray1;
	//第一行 = 列名字 
	
	pGenerateText->beginTableRow(tbRowHeight);
	pGenerateText->insertTableText(0,"Dir",&cFormat);
	pGenerateText->insertTableText(1,"No.",&cFormat);
	pGenerateText->insertTableText(2,"TYPE",&cFormat);
	pGenerateText->insertTableText(3,"POS1",&cFormat);
	pGenerateText->insertTableText(4,"LD1",&cFormat);
	pGenerateText->insertTableText(5,"POS2",&cFormat);
	pGenerateText->insertTableText(6,"LD2",&cFormat);
	pGenerateText->endTableRow();
	cFormat.colorBack = sdColorAuto;
	cFormat.colorFill = sdColorAuto;
	//第二行开始数据
	CString strDir;
	CString	strNo;
	CString strTYPE;
	CString strPOS1;
	CString strLD1;
	CString strPOS2;
	CString strLD2;

	int nStartLine = 1;//merger 起点行
	int nLines = 0; // merger 行数
	for(int iDir=0;iDir<2;iDir++)
	{
		bool bHasInsertDir = false;
		int iNo = 0;
	
		if (0 == iDir)
		{
			nStartLine = 1;
			nLines = 0;
		}
		else
		{
			nStartLine = nLines+1;
			nLines = 0; 
		}


		///////////////////////////集中荷载////////////////////////////
		{
			const std::vector<ConLoad>* Con;
			if (iDir == 0)
			{
				Con =  &ldCase.ConYY;
			}
			else
			{
				Con =  &ldCase.ConZZ;
			}
			for (int iCon=0;iCon<Con->size();iCon++)
			{
				if (fabs(Con->at(iCon).F)<1e-3)
				{
					continue;
				}

				pGenerateText->beginTableRow(tbRowHeight);
				if (iCon == 0)
				{
					if (iDir == 0)
					{
						strDir = "y-y";
					}
					else
					{
						strDir = "z-z";
					}

					pGenerateText->insertTableText(0,strDir,&cFormat);
					bHasInsertDir = true;
				}
				iNo++;
				strNo.Format(_T("%d"),iNo);
				strTYPE="Con";
				strPOS1.Format(_T("%1.f"),Con->at(iCon).x);
				strLD1.Format(_T("%1.f"),Con->at(iCon).F);

				pGenerateText->insertTableText(1,strNo,&cFormat);
				pGenerateText->insertTableText(2,strTYPE,&cFormat);
				pGenerateText->insertTableText(3,strPOS1,&cFormat);
				pGenerateText->insertTableText(4,strLD1,&cFormat);
				pGenerateText->insertTableText(5,"-",&cFormat);
				pGenerateText->insertTableText(6,"-",&cFormat);
				pGenerateText->endTableRow();
				nLines++;

			}
		}
		
		///////////////////////均布荷载////////////////////////////////
		{
			const std::vector<DisLoad>* Dis;
			if (0 == iDir)
			{
				Dis = &ldCase.DisYY;
			}
			else if(1 == iDir)
			{
				Dis = &ldCase.DisZZ;
			}

			for (int iDis=0;iDis<Dis->size();iDis++)
			{
				if (fabs(Dis->at(iDis).q1)<1e-3&&fabs(Dis->at(iDis).q2)<1e-3)
				{
					continue;
				}

				pGenerateText->beginTableRow(tbRowHeight);
				if (0 == iDis  && false == bHasInsertDir)
				{
					if (0 == iDir)
					{
						strDir = "y-y";
					}
					else
					{
						strDir = "z-z";
					}

					pGenerateText->insertTableText(0,strDir,&cFormat);
					bHasInsertDir = true;

				}
				iNo++;
				strNo.Format(_T("%d"),iNo);
				strTYPE="Dis";
				strPOS1.Format(_T("%0.1f"),Dis->at(iDis).x1);
				strLD1.Format(_T("%0.1f"),Dis->at(iDis).q1);
				strPOS2.Format(_T("%0.1f"),Dis->at(iDis).x2);
				strLD2.Format(_T("%0.1f"),Dis->at(iDis).q2);

				pGenerateText->insertTableText(1,strNo,&cFormat);
				pGenerateText->insertTableText(2,strTYPE,&cFormat);
				pGenerateText->insertTableText(3,strPOS1,&cFormat);
				pGenerateText->insertTableText(4,strLD1,&cFormat);
				pGenerateText->insertTableText(5,strPOS2,&cFormat);
				pGenerateText->insertTableText(6,strLD2,&cFormat);
				pGenerateText->endTableRow();
				nLines++;
			}
		}
		
		///////////////////////端部荷载////////////////////////////////
		{
			const EndLoad* EndLd;
			if (0 == iDir)
			{
				EndLd = &ldCase.EndmYY;
			}
			else if(1 == iDir)
			{
				EndLd = &ldCase.EndmZZ;
			}

			if(fabs(EndLd->Bottom)>1e-3||fabs(EndLd->Top)>1e-3)
			{
				pGenerateText->beginTableRow(tbRowHeight);

				if(false == bHasInsertDir)
				{
					if (0 == iDir)
					{
						strDir = "y-y";
					}
					else
					{
						strDir = "z-z";
					}

					pGenerateText->insertTableText(0,strDir,&cFormat);
					bHasInsertDir = true;
				}

				iNo++;
				strNo.Format(_T("%d"),iNo);
				strTYPE="End";
				//strPOS1.Format(_T("%0.1f"),Dis->GetAt(iDis).x1);
				strLD1.Format(_T("%0.1f"),EndLd->Bottom);
				//strPOS1.Format(_T("%0.1f"),Dis->GetAt(iDis).x2);
				strLD2.Format(_T("%0.1f"),EndLd->Top);

				pGenerateText->insertTableText(1,strNo,&cFormat);
				pGenerateText->insertTableText(2,strTYPE,&cFormat);
				pGenerateText->insertTableText(3,"-",&cFormat);
				pGenerateText->insertTableText(4,strLD1,&cFormat);
				pGenerateText->insertTableText(5,"-",&cFormat);
				pGenerateText->insertTableText(6,strLD2,&cFormat);
				pGenerateText->endTableRow();
				nLines++;
			}
		}
		if (nLines>0)
		{
			pGenerateText->mergeTableCells(sd::TABLE_CELL(nStartLine,0), sd::TABLE_CELL(nStartLine+nLines-1,0));
		}
	
	}//两个方向的循环

	//轴力 不分方向
	{
		const float Anxial = ldCase.AnxialForceYY;
		if (fabs(Anxial)>1e-3)
		{
			strNo = "1";
			pGenerateText->beginTableRow(tbRowHeight);
			strTYPE="Anxial";
			strLD1.Format(_T("%0.1f"),Anxial);
			pGenerateText->insertTableText(0,"-",&cFormat);
			pGenerateText->insertTableText(1,strNo,&cFormat);
			pGenerateText->insertTableText(2,"Anxi",&cFormat);
			pGenerateText->insertTableText(3,"-",&cFormat);
			pGenerateText->insertTableText(4,strLD1,&cFormat);
			pGenerateText->insertTableText(5,"-",&cFormat);
			pGenerateText->insertTableText(6,"-",&cFormat);
			pGenerateText->endTableRow();
		}
	}

	pGenerateText->endTable();

	return true;
}

//荷载信息
bool CPk_MainOutPut::Load_case(sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem)
{
// 	stt.BeginP();
// 	stt.Txt("Load No.1");
// 	stt.EndP();
// 	CString jpgname = "Load.jpg";	
// 	DrawPicJPG(pGenerateText,mReportItem,jpgname,PATH_WORKDIR);
// 	return true;

	for (int i=0;i<Mega_CasesCal::Get().Cases().size();i++)
	{
		mega_loads &ld = Mega_CasesCal::Get().Cases()[i];
		Load_case_one(pGenerateText,mReportItem,ld);
	}

	return true;
	
	
	
	
}



