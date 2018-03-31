#pragma once
#include "IGenerateText.h"
#include "ICalculationReport.h"
//#include "Param_MainParam.h"
#include "ConfigGen.h"
#include "Loaddef.h"
//#include "StructBaseInfo.h"



class TextTool
{
public:
	static const CString Eng;
public:
	TextTool();
	~TextTool();
public:
	bool readJson();
public:
	void Set(sd::IGenerateText* pT1,sd::PARAGRAPH_FORMAT* pF1);
	void SetGenerateText(sd::IGenerateText* pT1);
	void SetTEXT_FORMAT(sd::TEXT_FORMAT tf1);
	void SetPARAGRAPH_FORMAT(sd::PARAGRAPH_FORMAT* pF1);
	void BeginP();
	void EndP();
	
	CString format(double,int npre);
	void Txt(CString str1);
	void Txt(int nC,int nAt);



	void TxSub(CString str1);
	void TxUp(CString str1);

	void TxSub(CString str1,CString up,CString sign = "");
	void TxUp(CString str1,CString sub,CString sign = "");

	void TxUpSub(CString str1,CString up,CString sub,CString sign = "");

	void TxV(double f,int nPre = 0);
	void TxV(int  n);

	void m();
	void m(double ff,int nPre = 0);
	void m(double ff,int nPre ,int _n10n);

	void mm3();
	void mm3(double ff,int nPre = 0);
	void mm3(double ff,int nPre ,int _n10n);
	void mm2();
	void mm2(double ff,int nPre = 0);

	void m$s2();
	void m$s2(double ff,int nPre = 0);

	void mm();
	void mm(double ff,int nPre = 0);

	void mm4();
	void mm4(double ff,int nPre = 0 ,int _n10n=4);

	void kg$m();
	void kg$m(double ff,int nPre = 0);

	void MPa();
	void MPa(double ff,int nPre = 0);

	void C(int CXX);

	void days(int ndy);

	void kN();
	void kN(double ff,int nPre = 0);
	void kN(double ff,int nPre,int _n10n=4);

	void N();
	void N(double ff,int nPre = 0);

	void kNm();
	void kNm(double ff,int nPre = 0);

	void Nmm2();
	void Nmm2(double ff,int nPre = 0);

	void kNm2();
	void kNm2(double ff,int nPre = 0);
	void kNm2(double ff,int nPre,int _n10n=4);

	void N$mm();
	void N$mm(double ff,int nPre = 0);

	void kN$m();
	void kN$m(double ff,int nPre = 0);


	void N$m();
	void N$m(double ff,int nPre = 0);

	void studs$1m();
	void studs$1m(double ff,int nPre = 0);

	void SpTab______(int n=2);
private:
	Json::Value jsDocs;
	Json::Value jsLang;
	Json::Value jsC;
	Json::Value jsAt;
private:
	sd::IGenerateText* pT;
	sd::PARAGRAPH_FORMAT* pF;
private:
	sd::TEXT_FORMAT tf;
	sd::TEXT_FORMAT tfsub;
	sd::TEXT_FORMAT tfup; 
	sd::TEXT_FORMAT tfUpSub;
};



















class CPk_MainOutPut
{

	enum 
	{
		DRAWPAGE_NULL = 0,
		DRAWPAGE_HORI = 1,
		DRAWPAGE_VERI = 2
	};

	enum
	{
		PATH_EXE_FOLDER = 0,
		PATH_WORKDIR = 1
	};
private:
	static TextTool stt;
public:
	static const  TCHAR* configXmlPrefix;
	static CString configXml;
	static TCHAR* CalculationReportXml;
	static TCHAR* wordFileName;
	static const TCHAR* pathCalcTemp;
	static int m_doctype;
	static int m_drawHoriVeri;
	 
public:
	struct CombStress
	{
		int id;
		double M1,N1,V1,M2,N2,V2;
	};

	struct BeamM
	{
		CString wanju1;
		CString wanju2;
		double MDown[7],MUp[7];
		double M1,M2,M3,M4,M5,M6,M7;
	};
	struct BeamNao
	{
		CString naodu;
		double nao[7];
	};

	struct WindColumnStree
	{
		int id;
		double My,N,V;
	};

	//static int s_PicIndex; 
public:
	CPk_MainOutPut(void);
	~CPk_MainOutPut(void);
	
	static bool initReportTool();
	static bool dlgConfig();


	static bool prepareT();

	static bool ProgressPrepair(int np);

	static bool outputRoutine(sd::IGenerateText* ,const sd::REPORT_ITEM&);

	static bool GENERAL_PARAMETERS(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool CODE_STANDARD(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool CROSS_SECTION(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool Material_properties(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool Creep_coefficient_concrete(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool Load_cases(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool Load_case(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool Load_case_one(sd::IGenerateText* ,const sd::REPORT_ITEM&, const mega_loads& ldCase);
	static bool Calculation_parameters(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool Load_combinations(sd::IGenerateText* ,const sd::REPORT_ITEM&);

	static bool ULTIMATE_LIMIT_STATES(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool RESISTANCE_OF_THE_CROSS_SECTION_AXIAL(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool RESISTANCE_OF_THE_CROSS_SECTION_TO_SHEAR_RESISTANCE(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool RESISTANCE_OF_THE_CROSS_SECTION_TO_COMBINED_ACTIONS_M_N(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool RESISTANCE_OF_THE_CROSS_SECTION_TO_COMBINED_ACTIONS_M_N2(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool BUCKLING_RESISTANCE_TO_AXIAL_FORCE(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool AMPLIFICATION_OF_DESIGN_MOMENT_OF_COMBINED_ACTION_M_N_CONSIDERING(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool INTERFACE_SHEAR_CONNECTION(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool SYNTHESIS_OF_VERIFICATIONS(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static bool SYNTHESIS_OF_VERIFICATIONS_CUSTOM(sd::IGenerateText* ,const sd::REPORT_ITEM&);
	static void DrawingFiberPDM_Points(sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem );

	///////////////////////////////////////////////////////
	static void  DrawingFunc(sd::IGenerateText* pGenerateText, 
		const sd::GRAPHIC_ITEM& mReportItem,CString fileName,
		sd::PGRAPHIC_FORMAT pFormat,int wid = 110,int nPreEmptyLine = 1);

	static void DrawingFuncEmbed(sd::IGenerateText* pGenerateText, 
		const sd::GRAPHIC_ITEM& mReportItem,CString fileName,
		sd::PGRAPHIC_FORMAT pFormat,int wid = 75);

	static bool drawingRoutine( sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem);
	static bool GenDrawingName(const sd::GRAPHIC_ITEM& mReportItem,CString& DrawName,bool resetIndex=false);



	static bool ClearCal_T();
	static bool ClearCalcTemp();
	static bool CopyInCalcTemp(CString fileName,CString newName );
	static bool CopyToCalcTemp(CString fileName,CString newName,bool delori=false);
	static bool subt(sd::IGenerateText* pT,const sd::REPORT_ITEM& mReportItem,CString str1);
	static bool upt(sd::IGenerateText* pT,const sd::REPORT_ITEM& mReportItem,CString str1);


	//Αυ
	static bool SERVICEABILITY_LIMIT_STATES(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem);
	static bool INTERNAL_FORCES(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem);
	static bool CRACK_CHECK(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem);
	static bool MEMBER_CHECK1(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem);
	static bool MEMBER_CHECK2(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem);
	static bool MEMBER_CHECK3(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem);
	static bool MEMBER_CHECK4(sd::IGenerateText* pGenerateText ,const sd::REPORT_ITEM& mReportItem);
	static void MTable( sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem,int currentcomb,const int yz);
	static void TrussModelTable(sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem,int side);
	static void LoadCaseTable(sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem,int caseno);
	static void LoadCombTable(sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem);
	static void DrawPicJPG(sd::IGenerateText* pGenerateText, const sd::REPORT_ITEM& mReportItem,CString filename,int PathType=PATH_EXE_FOLDER,int wid=150 );


	static bool Drawing_Fiber_1(sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config);
	static bool Drawing_PDM_1(sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config);
	static bool Drawing_Fiber_2(sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config);
	static bool Drawing_PDM_2(sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config);
	static bool Drawing_Sect(sd::IGenerateText* pGenerateText, const sd::GRAPHIC_ITEM& mReportItem,CConfigGenCalc& config);


 

	//static bool clickIconRoutine( NODE_KEY key,OPTION_TYPE eType,SD_POINT pt);

	
};



 



 



