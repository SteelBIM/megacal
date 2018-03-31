#include "StdAfx.h"
#include "MegaCalCmd.h"

#include "DesignParameter.h"
#include "PredefSectDescrib.h"

#include "MegaCalIO.h"
#include "MegaDrawingImp.h"

#include "Force_combine.h "
#include "InterfaceHeader_CPP.h "

#include "Tools.h"
#include "Force_combine.h"


//ZHLEI 2017/11 :ÃüÁîÈë¿Ú

extern "C" __declspec( dllexport ) 
	void MegaCheckIO_(SectDescribe *pSect,DesnPara *pDesnPara)
{
	//MegaCalIO::MegaCheckIO(pSect,pDesnPara);
	MegaCalIO::InitMegaCheckIO(pSect,pDesnPara);
	//MegaCalIO::MegaDesn();
}

extern "C" __declspec( dllexport ) 
	void ExitMega_()
{
	MegaCalIO::ExitMega();
}


extern "C" __declspec( dllexport ) 
	void MegaDraw(int nLoadCase, int nType)
{
	//MegaCalIO::MegaCheckIO(pSect,pDesnPara);
	CMegaDrawingImp::Get().DrawCol(nLoadCase,nType);
}


extern "C" __declspec( dllexport ) 
	void MegaReport()
{
	CalcOutWord();
}

extern "C" __declspec( dllexport ) 
	void GetProPara(int ProType,float TT,int ProNo,int code,float &fy,int &ProE)
{
	fy = Tools::GetProFy(ProType,TT,ProNo);
	ProE = Tools::GetProE(code);
}

extern "C" __declspec( dllexport ) 
	void GetBarE(int code,int &BarE)
{
	BarE = Tools::GetBarE(code);
}

extern "C" __declspec( dllexport)
	void MegaDesn_(DesnPara *pDesnPara,double M[][FirbePoints],double N[][FirbePoints])
{
	MegaCalIO::MegaDesn(pDesnPara,M,N);
}

extern "C" __declspec( dllexport ) 
	void GetSLSCombine(double &My,double &Mz,double &MMix,double &N)
{
	Force_combine Colmforce;
	Colmforce.GetForce();
	Colmforce.BiaoZhun_combine();
	My = Colmforce.My_max_bz;
	Mz = Colmforce.Mz_max_bz;
	N  = Colmforce.N_max_bz;
	MMix = Colmforce.Mmix_max_bz;
}

extern "C" __declspec( dllexport )
	float GetProFy_Cal(int ProType,float TT,int ProNo)
{	
	return Tools::GetProFy(ProType,TT,ProNo);
}

extern "C" __declspec( dllexport )
	float GetProFy_Cal2(CString ProName,float TT,int Type)
{	
	return Tools::GetProFy(ProName,TT,Type);
}

extern "C" __declspec( dllexport )
	void DrawNMPoints(float ratio,double Iin[],int yz)
{
	Tools::DrawNMPoint_forfiber(ratio,Iin,yz);
}

extern "C" __declspec( dllexport )
	void SetCalEI(double EIin[])
{
	GPara::SetEI_Fiber(EIin);
}

extern "C" __declspec( dllexport )
	void GetTipsStr(float& XCRS,float& YCRS,char* STR)
{
	Tools::GetStr(XCRS,YCRS,STR);
}

extern "C" __declspec( dllexport )
	void SetIfExpertMode(int expert)
{
	GPara::ifExpertMode=expert;
}