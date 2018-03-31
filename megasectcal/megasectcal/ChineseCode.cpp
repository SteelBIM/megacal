#include "StdAfx.h"
#include "ChineseCode.h"

#include "GPara.h"
#include "Tools.h"
#include "SectProperty.h"
ChineseCode::ChineseCode(void)
{

}


ChineseCode::~ChineseCode(void)
{
}

bool ChineseCode::CalNpl()
{
	SectProperty m_sect;
	m_sect.SectGeoInfo();
	m_sect.SectCal();
	int Ec = Tools::GetConEcm(GPara::m_Sect_in->Concrete_mat);
	int Es = Tools::GetProE(3);
	float EE = (Ec*m_sect.Icx+Es*m_sect.Isx)/(Ec*m_sect.Ac+Es*m_sect.As);
	float ii = sqrt(EE);
	Ll = GPara::m_DesnPara->Lens*1000/ii;
	float Fai = GetFai(Ll);
	float fyd;		//钢材屈服强度
	float fsd;		//钢筋屈服强度
	float fcd;		//混凝土屈服强度
	fyd = m_sect.fy/1.11;
	fsd = m_sect.fs/1.1;
	fcd = m_sect.fc/1.4;
	double Pn = 0.9*Fai*(m_sect.fc*m_sect.Ac+m_sect.fs*m_sect.Asr+m_sect.fy*m_sect.As);		//标准值稳定承载力
	double Npl = 0.9*Fai*(fcd*m_sect.Ac+fsd*m_sect.Asr+fyd*m_sect.As);
	GPara::NplRd = Npl;
	GPara::Pn_y = Pn;
	GPara::Pn_z = Pn;
	Nu = m_sect.fc*m_sect.Ac+m_sect.fy*m_sect.As;
// 	//二阶效应放大
// 	double Nmax = GPara::N_max;
// 	double Mmin;
// 	float Amp = 1.f;
// 	float ea = max(20,GPara::m_Sect_in->Sect_h/30);
// 	bool checkPdelta=true;
// 	if(GPara::M12 >0.9)
// 		checkPdelta = false;
// 	if(Nmax/Nu >0.9)
// 		checkPdelta = false;
// 	if(Ll>32-12*GPara::M12)
// 		checkPdelta = false;
// 	int h0 = GPara::m_Sect_in->Sect_h - 2*GPara::m_Sect_in->Concrete_coverd;
// 	int h = GPara::m_Sect_in->Sect_h;
// 	if(checkPdelta){	//进行二阶放大
// 		float Cm = 0.7+0.3*GPara::M12;
// 		float kc = 0.5*m_sect.fc*m_sect.Ag/Nmax;
// 		float temp2 = (GPara::m_DesnPara->Lens*1000/h)*(GPara::m_DesnPara->Lens/h);
// 		float nns = 1+h0*temp2*kc/(1300*(Mmin/Nmax+ea));
// 		Amp = Cm*nns;
// 	}
//	float Mpl = Nmax*ea;
	return true;
}


bool ChineseCode::SectDesn()
{
	CalNpl();
// 	if(GPara::PDM_switch)
	NMCurve_PDM();
	MemberCheck();
	CrackCheck();
	//WriteRst();
	return true;
}

bool ChineseCode::CalVpl()
{
	//中国规范需要输入剪跨比
	float ll;		//剪跨比
	float con_ftk,con_ft;
	float bar_fyv;
	int b = GPara::m_Sect_in->Sect_b;
	int h = GPara::m_Sect_in->Sect_h;
	int cover_b = GPara::m_Sect_in->Con_coverd_x;
	int cover_h = GPara::m_Sect_in->Con_coverd_y;
	float Asv_b = GPara::m_Sect_in->Tran_ratio_x*b*h;
	float Asv_h = GPara::m_Sect_in->Tran_ratio_y*b*h;
	Tools::GetConFtk_Chi(GPara::m_Sect_in->Concrete_mat,con_ftk);
	con_ft = con_ftk/1.4;
	bar_fyv = Tools::GetBarFyv(GPara::m_DesnPara->CodeType);
	float Vp_x = 1.75*con_ft*b*(h-cover_h)/(ll+1)+bar_fyv*Asv_b*(h-cover_h)/100+0.07*GPara::N_max;
	float Vp_y = 1.75*con_ft*h*(b-cover_b)/(ll+1)+bar_fyv*Asv_b*(h-cover_h)/100+0.07*GPara::N_max;
	GPara::VplRd_y = Vp_x;
	GPara::VplRd_z = Vp_y;
	return true;
}

void ChineseCode::MemberCheck()
{
	SectProperty m_sect;
	m_sect.SectGeoInfo();
	//保护层厚度
	int cover = GPara::m_Sect_in->Concrete_coverd;
	if(cover>200)
		Con_CoverCheck1.iOK=true;
	else
		Con_CoverCheck1.iOK=false;
	Con_CoverCheck1.PermitVal_lower=200;
	Con_CoverCheck1.CalVal = cover;
	if(cover>GPara::m_Sect_in->Bar_d)
		Con_CoverCheck2.iOK=true;
	else
		Con_CoverCheck2.iOK=false;
	Con_CoverCheck2.PermitVal_lower=GPara::m_Sect_in->Bar_d;
	Con_CoverCheck2.CalVal = cover;
	int minc;
	int ep_class = GPara::m_DesnPara->Exp_Class;
	if(ep_class==1)
		minc = 20;
	else if(ep_class==2)
		minc = 25;
	else if(ep_class==3)
		minc = 35;
	else if(ep_class==4)
		minc = 40;
	else if(ep_class==5)
		minc = 50;
	if(cover>minc)
		Con_CoverCheck3.iOK=true;
	else
		Con_CoverCheck3.iOK=false;
	Con_CoverCheck3.PermitVal_lower=minc;
	Con_CoverCheck3.CalVal=cover;

	//含钢率
	float delta = m_sect.As/m_sect.Ac;
	Pro_RatioCheck.PermitVal_lower = 0.04;
	Pro_RatioCheck.PermitVal_upper = 0.15;
	Pro_RatioCheck.CalVal = delta;
	Tools::CompareVal(Pro_RatioCheck);
	//高厚比
	int btlim,htlim;
	int Hsecth = GPara::m_Sect_in->HSect.H1;
	int Hsectb = GPara::m_Sect_in->HSect.B1;
	float Tf = GPara::m_Sect_in->HSect.RI;
	float Tw = GPara::m_Sect_in->HSect.RJ;
	Tools::Getbthtlim_chi(btlim,htlim);
	float bt = (Hsectb-Tw)*0.5/Tf;
	float ht = (Hsecth-2*Tf)/Tw;
	if(bt>btlim)
		Pro_btCheck.iOK=false;
	else 
		Pro_btCheck.iOK=true;
	Pro_btCheck.CalVal = bt;
	Pro_btCheck.PermitVal_upper = btlim;

	if(ht>htlim)
		Pro_htCheck.iOK=false;
	else
		Pro_htCheck.iOK=true;
	Pro_htCheck.CalVal=ht;
	Pro_htCheck.PermitVal_upper=htlim;

	//纵筋率
	float bar_ratio = m_sect.Asr*100/m_sect.Ac;
	Bar_RatioCheck.CalVal = bar_ratio;
	Bar_RatioCheck.PermitVal_lower = 0.8;
	Bar_RatioCheck.PermitVal_upper = 3.31;
	Tools::CompareVal(Bar_RatioCheck);

	Bar_SideRatioCheck.CalVal = bar_ratio*0.25;
	Bar_SideRatioCheck.PermitVal_lower = 0.2;
	Bar_SideRatioCheck.PermitVal_upper = 0.83;
	//间距
	Bar_SpaceCheck_x.CalVal = m_sect.bar_inv_x;
	Bar_SpaceCheck_x.PermitVal_lower = 50;
	Bar_SpaceCheck_x.PermitVal_upper = 250;
	Tools::CompareVal(Bar_SpaceCheck_x);
	Bar_SpaceCheck_y.CalVal = m_sect.bar_inv_y;
	Bar_SpaceCheck_y.PermitVal_lower = 50;
	Bar_SpaceCheck_y.PermitVal_upper = 250;
	Tools::CompareVal(Bar_SpaceCheck_y);
	//最小直径
	if(GPara::m_Sect_in->Bar_d<16)
		Bar_BarDCheck.iOK=false;
	else 
		Bar_BarDCheck.iOK=true;
	Bar_BarDCheck.CalVal = GPara::m_Sect_in->Bar_d;
	Bar_BarDCheck.PermitVal_lower = 16;
}

void ChineseCode::CrackCheck()
{
	SectProperty m_sect;
	m_sect.SectGeoInfo();
	//环境等级控制
	int h0 = GPara::m_Sect_in->Sect_h - 2*GPara::m_Sect_in->Concrete_coverd;
	float Wlim;
	if(GPara::m_DesnPara->Exp_Class==1)
		Wlim = 0.3;
	else
		Wlim = 0.2;
	float Wmax;
	float N,M;
	float e0 = M/N;
	if(e0/h0>0.55){	//进行裂缝验算
		int Es = Tools::GetBarE(3);
		float qs = GPara::Qs_Euro;
		int deq = GPara::m_Sect_in->Bar_d;
		float pte = m_sect.As*2/m_sect.Ac;
		if(pte<0.01)
			pte = 0.01;
		float ftk;
		Tools::GetConFtk_Chi(GPara::m_Sect_in->Concrete_mat,ftk);
		float fai = 1.1-0.65*ftk/(pte*qs);
		if(fai<0.2)
			fai = 0.2;
		else if(fai>1.0)
			fai = 1.0;
		int cs = GPara::m_Sect_in->Concrete_coverd;
		if(cs<20)
			cs = 20;
		else if(cs>65)
			cs = 65;
		 Wmax = 1.9*fai*qs*(1.9*cs+0.08*deq/pte)/Es;
		if(Wmax>Wlim)
			Crack_Check.iOK=false;
		else
			Crack_Check.iOK=true;
	}
	else{
		Crack_Check.iOK = true;
		Wmax = 0;
	}
	Crack_Check.CalVal = Wmax;
	Crack_Check.PermitVal_upper = Wlim;
}

bool ChineseCode::JudgeSect()
{
	//轴力承载
	double Ned = GPara::N_max;
	float Rbz = Ned/GPara::NplRd;
	//MNV组合下的弯矩
	double Mnplrd;
	Mnplrd = Tools::GetMFromCurve(Ned,1);
	float Rmy = GPara::My_max/Mnplrd;
	Mnplrd = Tools::GetMFromCurve(Ned,2);
	float Rmz = GPara::Mz_max/Mnplrd;

	return true;
}

float ChineseCode::GetFai(float Li)
{
	float Fai;
	if(Li<=28)
		Fai=1.0;
	else if(Li>28 && Li<=35)
		Fai = 0.98;
	else if(Li>35 && Li<=42)
		Fai = 0.95;
	else if(Li>42 && Li<=48)
		Fai = 0.92;
	else if(Li>48 && Li<=55)
		Fai = 0.87;
	else if(Li>55 && Li<=62)
		Fai = 0.81;
	else if(Li>62 && Li<=69)
		Fai = 0.75;
	else if(Li>69 && Li<=76)
		Fai = 0.70;
	else if(Li>76 && Li<=83)
		Fai = 0.65;
	else if(Li>83 && Li<=90)
		Fai = 0.60;
	else if(Li>90 && Li<=97)
		Fai = 0.56;
	else if(Li>97 && Li<=104)
		Fai = 0.52;
	return Fai;
}

float ChineseCode::AmplfyM(double &M1,double &M2)
{
	//二阶效应放大
	SectProperty m_sect;
	m_sect.SectGeoInfo();
	m_sect.SectCal();
	double Nmax = GPara::N_max;
	double Mmin = min(fabs(M1),fabs(M2));
	double Mmax = max(fabs(M1),fabs(M2));
	float Amp = 1.f;
	float ea = max(20,GPara::m_Sect_in->Sect_h/30);
	bool checkPdelta=true;
	if(Mmin/Mmax >0.9)
		checkPdelta = false;
	if(Nmax/Nu >0.9)
		checkPdelta = false;
	if(Ll>32-12*Mmin/Mmax)
		checkPdelta = false;
	int h0 = GPara::m_Sect_in->Sect_h - 2*GPara::m_Sect_in->Concrete_coverd;
	int h = GPara::m_Sect_in->Sect_h;
	if(checkPdelta){	//进行二阶放大
		float Cm = 0.7+0.3*Mmin/Mmax;
		float kc = 0.5*m_sect.fc*m_sect.Ag/Nmax;
		float temp2 = (GPara::m_DesnPara->Lens*1000*GPara::m_DesnPara->uL/h)*(GPara::m_DesnPara->Lens*1000*GPara::m_DesnPara->uL/h);
		float nns = 1+h0*temp2*kc/(1300*(Mmin/Nmax+ea));
		Amp = Cm*nns;
	}
	return Amp;
}

void ChineseCode::Line1(double N_in[FirbePoints],double N_out[FirbePoints],int yz,int nCase)
{
	float ks;
	if(yz==1)
		ks = fabs(GPara::Pn_y/N_in[0]);
	else if(yz==2)
		ks = fabs(GPara::Pn_z/N_in[0]);

	for(int i=0;i<10;i++){
		N_out[i] = N_in[i]*ks;
	}
}

void ChineseCode::ReduceNMCurve(double N[][FirbePoints],double M[][FirbePoints])
{
	//前两组是y,后两组是z
	double N_draw[3][FirbePoints],M_draw[3][FirbePoints];
	//y方向初始化
	memcpy(N_draw[0],N[0],FirbePoints*sizeof(double));
	memcpy(M_draw[0],M[0],FirbePoints*sizeof(double));
	memcpy(M_draw[1],M[0],FirbePoints*sizeof(double));
	memcpy(M_draw[2],M[1],FirbePoints*sizeof(double));
	double Ntemp[FirbePoints];
	int tempcase = 0;
	for(int i=0;i<2;i++){
		memcpy(Ntemp,N[i],FirbePoints*sizeof(double));
		Line1(N[i],Ntemp,1,tempcase);
		memcpy(N_draw[i+1],Ntemp,FirbePoints*sizeof(double));
	}
	DrawFiberCurve(N_draw,M_draw,12);
	DrawFiberCurve(N_draw,M_draw,13);
	//z方向初始化
	memcpy(N_draw[0],N[2],FirbePoints*sizeof(double));
	memcpy(M_draw[0],M[2],FirbePoints*sizeof(double));
	memcpy(M_draw[1],M[2],FirbePoints*sizeof(double));
	memcpy(M_draw[2],M[3],FirbePoints*sizeof(double));
	for(int i=2;i<4;i++){
		memcpy(Ntemp,N[i],10*sizeof(double));
		Line1(N[i],Ntemp,2,tempcase);
		memcpy(N_draw[i-1],Ntemp,FirbePoints*sizeof(double));
	}
	DrawFiberCurve(N_draw,M_draw,22);
	DrawFiberCurve(N_draw,M_draw,23);

}