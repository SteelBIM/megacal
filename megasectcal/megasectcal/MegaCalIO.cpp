#include "StdAfx.h"
#include "MegaCalIO.h"
#include "AISC.h"
#include "EUROCode.h"
#include "ChineseCode.h"
#include "InterfaceCheck.h"



extern "C" __declspec( dllexport ) void MegaDrawTest_( ){
	 
}

//extern "C" __declspec( dllexport ) void MegaCheckIO_(SectDescribe *pSect,DesnPara *pDesnPara){
//	MegaCalIO::MegaCheckIO(pSect,pDesnPara);
//
//}

MegaCalIO::MegaCalIO(void)
{
}


MegaCalIO::~MegaCalIO(void)
{
}

void MegaCalIO::InitMegaCheckIO(SectDescribe *pSect,DesnPara *pDesnPara)
{
	//初始化
	GPara::m_Sect_in = new SectDescribe;
	GPara::m_DesnPara = new DesnPara;
	memcpy(GPara::m_Sect_in,pSect,sizeof(SectDescribe));
	memcpy(GPara::m_DesnPara,pDesnPara,sizeof(DesnPara));
	GPara::SetDesnContent();		//根据情况确定计算条件
}

void MegaCalIO::ExitMega()
{
	//清理指针
	if(GPara::m_Sect_in != NULL)
		delete[] GPara::m_Sect_in;
	if(GPara::m_DesnPara !=NULL)
	delete[] GPara::m_DesnPara;
}

void MegaCalIO::MegaDesn(DesnPara *pDesnPara,double M[][FirbePoints],double N[][FirbePoints])
{
	GPara::m_DesnPara->BarQ = pDesnPara->BarQ;	//初始化一下钢筋应力
	GPara::m_DesnPara->Fiberpoints_y = pDesnPara->Fiberpoints_y;
	GPara::m_DesnPara->Fiberpoints_z = pDesnPara->Fiberpoints_z;
	GPara::m_DesnPara->Fiberpoints_y_k = pDesnPara->Fiberpoints_y_k;
	GPara::m_DesnPara->Fiberpoints_z_k = pDesnPara->Fiberpoints_z_k;
	memcpy(GPara::M_Curve,M,4*FirbePoints*sizeof(double));
	memcpy(GPara::N_Curve,N,4*FirbePoints*sizeof(double));
	if(GPara::m_DesnPara->CodeType==1){	//欧洲规范
		//Colmforce.Euro_combine();
		//		Colmforce.HeZai_combine();
		EUROCode m_Desn;
		if(!pDesnPara->ifCustomSection){
			if(pDesnPara->ifFiber){
				m_Desn.JudgeSect_Fiber(N,M);
				m_Desn.ReduceNMCurve(GPara::N_Curve,GPara::M_Curve);
			}
			else{
				m_Desn.SectDesn();
//				m_Desn.ReduceNMCurve(GPara::N_Curve,GPara::M_Curve);
			}
		}
		else{
			//曲线折减并构件验算
			m_Desn.JudgeSect_Fiber(N,M);
		}
	}
	else if(GPara::m_DesnPara->CodeType==2){	//美国规范
		//Colmforce.AISC_combine();
		//		Colmforce.HeZai_combine();
		AISC m_Desn;
		if(!pDesnPara->ifCustomSection){
			m_Desn.SectDesn();
			m_Desn.ReduceNMCurve(GPara::N_Curve,GPara::M_Curve);
		}
		else{

		}
	}
	else if(GPara::m_DesnPara->CodeType==3){	//中国规范
		//Colmforce.Chinese_combine();
		//		Colmforce.HeZai_combine();
		ChineseCode m_Desn;
		if(!pDesnPara->ifCustomSection){
			m_Desn.SectDesn();
			m_Desn.ReduceNMCurve(GPara::N_Curve,GPara::M_Curve);
		}
		else{

		}
	}
	//界面剪力
	if(!pDesnPara->ifCustomSection){
		InterfaceCheck studcheck;
		studcheck.InterfaceDesn();
	}

}

void MegaCalIO::MegaCheckIO(SectDescribe *pSect,DesnPara *pDesnPara)
{
	//初始化
	GPara::m_Sect_in = new SectDescribe;
	GPara::m_DesnPara = new DesnPara;
	memcpy(GPara::m_Sect_in,pSect,sizeof(SectDescribe));
	memcpy(GPara::m_DesnPara,pDesnPara,sizeof(DesnPara));
	GPara::SetDesnContent();		//根据情况确定计算条件
	//根据规范来处理

	if(pDesnPara->CodeType==1){	//欧洲规范
		//Colmforce.Euro_combine();
//		Colmforce.HeZai_combine();
		EUROCode m_Desn;
		m_Desn.SectDesn();
		
	}
	else if(pDesnPara->CodeType==2){	//美国规范
		//Colmforce.AISC_combine();
//		Colmforce.HeZai_combine();
	}
	else if(pDesnPara->CodeType==3){	//中国规范
		//Colmforce.Chinese_combine();
//		Colmforce.HeZai_combine();
	}
	//界面剪力
	InterfaceCheck studcheck;
	studcheck.InterfaceDesn();
}

void MegaCalIO::DrawNMPoints()
{

}