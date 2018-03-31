#include "StdAfx.h"
#include "AISC.h"
#include "Tools.h"
#include "MegaCalIO.h"
#include "GlobalResult.h"

AISC::AISC(void)
{
}


AISC::~AISC(void)
{
}

bool AISC::JudgeSect()
{
	//在截面验算时候再考虑折减faic
	Force_combine Colmforce;
	Colmforce.GetForce();
	Colmforce.HeZai_combine();

	//截面轴向承载力
	float Rn = GPara::N_max/GPara::NplRd;
	if(Rn<=1)
		AixalForce_Check.iOK = true;
	else 
		AixalForce_Check.iOK = false;
	AixalForce_Check.CalVal = GPara::N_max;
	AixalForce_Check.PermitVal_upper = GPara::NplRd;
	//截面剪切承载力
	double Ved_z=GPara::Vz_max;
	double Ved_y=GPara::Vy_max;
	float RVz = fabs(Ved_z)/GPara::VplRd_z;
	float Rvy = fabs(Ved_y)/GPara::VplRd_y;
	if(Rvy<=1)
		ShearForcevy_Check.iOK = true;
	else 
		ShearForcevy_Check.iOK = false;
	ShearForcevy_Check.CalVal = Ved_y;
	ShearForcevy_Check.PermitVal_upper = GPara::VplRd_y;

	if(RVz<=1)
		ShearForcevz_Check.iOK = true;
	else
		ShearForcevz_Check.iOK = false;
	ShearForcevz_Check.CalVal = Ved_z;
	ShearForcevz_Check.PermitVal_upper = GPara::VplRd_z;

	//受弯承载力验算
	float RMN=0.f;
	for(int i=0;i<LoadCase;i++){
		for(int j=0;j<Combine;j++){
			double N = Colmforce.N[i][j];
			double My = Colmforce.My[i][j];
			double Mz = Colmforce.Mz[i][j];
			float ri = N/GPara::NplRd;
			float rtemp;
			if(ri>=0.2){
				rtemp = ri+8*(My/GPara::MyplNRd+Mz/GPara::MzplNRd)/9;
			}
			else{
				rtemp = ri*0.5+(My/GPara::MyplNRd+Mz/GPara::MzplNRd);
			}
			if(rtemp>RMN)
				RMN = rtemp;
		}
	}


	//轴向屈曲承载力
	float rx,ry;
	rx = GPara::N_max/NbplRd_x;
	if(rx<=1)
		BucklingNby_Check.iOK=true;
	else
		BucklingNby_Check.iOK=false;
	BucklingNby_Check.CalVal = GPara::N_max;
	BucklingNby_Check.PermitVal_upper = NbplRd_x;

	ry = GPara::N_max/NbplRd_y;
	if(ry<=1)
		BucklingNbz_Check.iOK=true;
	else
		BucklingNbz_Check.iOK=false;
	BucklingNbz_Check.CalVal = GPara::N_max;
	BucklingNbz_Check.PermitVal_upper = NbplRd_y;

	return true;
}

void AISC::MemberCheck()
{
	SectProperty m_Sectinfo;
	m_Sectinfo.SectGeoInfo();
	//1)混凝土
	int exposelevel;
	int barlevel;
	float cover;
	if(exposelevel==1)
		cover = 3.f;
	else if(exposelevel==2){
		if(barlevel>5 && barlevel<19)
			cover = 2.f;
		else
			cover = 1.5f;
	}
	else if(exposelevel==3)
		cover = 1.5f;
	else
		cover = 1.5f;
	Con_CoverCheck.PermitVal_lower = cover;
	Con_CoverCheck.PermitVal_upper = 10000;
	Con_CoverCheck.CalVal = m_Sectinfo.m_cover;
	Tools::CompareVal(Con_CoverCheck);
	float con_fc;
	Con_StrCheck.PermitVal_lower = 3;		//ksi
	Con_StrCheck.PermitVal_upper = 10;
	Con_StrCheck.CalVal = con_fc;
	Tools::CompareVal(Con_StrCheck);
	//2)型钢
	Pro_RatioCheck.PermitVal_lower = 0.01;
	Pro_RatioCheck.PermitVal_upper = 10000;
	Pro_RatioCheck.CalVal = m_Sectinfo.As/m_Sectinfo.Ag;
	Tools::CompareVal(Pro_RatioCheck);
	float Pro_fy;
	Pro_RatioCheck.PermitVal_lower = 10000;
	Pro_RatioCheck.PermitVal_upper = 75;		//ksi
	Pro_RatioCheck.CalVal = Pro_fy;
	Tools::CompareVal(Pro_RatioCheck);
	//3)纵筋
	float sr_ratio = m_Sectinfo.Asr/(m_Sectinfo.Ag-m_Sectinfo.As);
	Bar_RatioCheck.PermitVal_lower = 0.01;
	Bar_RatioCheck.PermitVal_upper = 0.08;
	Bar_RatioCheck.CalVal = sr_ratio;
	Tools::CompareVal(Bar_RatioCheck);
	float dagg = GPara::m_DesnPara->Dagg;
	dagg = 4*dagg/3;
	Bar_SpaceCheck.PermitVal_lower = max(max(dagg,1.5),1.5*m_Sectinfo.bar_d);
	Bar_SpaceCheck.PermitVal_upper = 10000;
	Bar_SpaceCheck.CalVal = m_Sectinfo.bar_inv;
	Tools::CompareVal(Bar_SpaceCheck);
	Bar_CoreCheck.PermitVal_lower = max(1.5*m_Sectinfo.bar_d,1.5);
	Bar_CoreCheck.PermitVal_upper = 10000;
	Bar_CoreCheck.CalVal = m_Sectinfo.bartopro_x;
	Bar_CoreCheck.CalVal2 = m_Sectinfo.bartopro_y;
	Tools::CompareVal(Bar_CoreCheck,true);
	float bar_fy;
	Bar_StrCheck.PermitVal_lower = 80;
	Bar_StrCheck.PermitVal_upper = 10000;
	Bar_StrCheck.CalVal = bar_fy;
	Tools::CompareVal(Bar_StrCheck);
	//4)箍筋
		//Vu>0.5ψVc判断
// 	float tie_fy;
// 	float Avmin1 = 0.75*sqrt(con_fc)*m_Sectinfo.Tie_inv*m_Sectinfo.Tie_d/tie_fy;
// 	float Avmin2 = 50*m_Sectinfo.Tie_inv*m_Sectinfo.Tie_d/tie_fy;
// 	Tie_AvCheck.PermitVal_lower = max(Avmin1,Avmin2);
// 	Tie_AvCheck.PermitVal_upper = 10000;
// 	Tie_AvCheck.CalVal = PI*m_Sectinfo.Tie_d*m_Sectinfo.Tie_d*0.25;
// 	Tools::CompareVal(Tie_AvCheck);
// 	Tie_StrCheck.PermitVal_lower = 80;
// 	Tie_StrCheck.PermitVal_upper = 10000;
// 	Tie_StrCheck.CalVal = tie_fy;
// 	Tools::CompareVal(Tie_StrCheck);
// 	Tie_CSSpaceCheck.PermitVal_lower = dagg;
// 	Tie_CSSpaceCheck.PermitVal_upper = 10000;
// 	Tie_CSSpaceCheck.CalVal = m_Sectinfo.Tie_inv - m_Sectinfo.Tie_d;
// 	Tools::CompareVal(Tie_CSSpaceCheck);
// 	Tie_CCSpaceCheck.PermitVal_lower = min(min(16*m_Sectinfo.bar_d,8*m_Sectinfo.Tie_d),min(GPara::m_Sect_in->Sect_h,GPara::m_Sect_in->Sect_b));
// 	Tie_CCSpaceCheck.PermitVal_upper = 10000;
// 	Tie_CCSpaceCheck.CalVal = m_Sectinfo.Tie_inv;
// 	Tools::CompareVal(Tie_CCSpaceCheck);
	//钢筋直径检查没有
}

void AISC::CrackCheck()
{
	//这里采用简单的方法校验
	SectProperty m_sect;
	m_sect.SectGeoInfo();
	float fy = Tools::GetBarFyv(2);
	float fs = 2*fy/3;
	int cover = GPara::m_Sect_in->Con_coverd_x;
	float s1 = 15*40000/fs - 2.5*cover;
	float s2 = 12*40000/fs;
	float ss = min(s1,s2);
	if(ss-m_sect.bar_inv_x>0)
		Crack_Check1.iOK = true;
	else
		Crack_Check1.iOK = false;
	Crack_Check1.CalVal = m_sect.bar_inv_x;
	Crack_Check1.PermitVal_lower = 0;
	Crack_Check1.PermitVal_upper = ss;


	cover = GPara::m_Sect_in->Con_coverd_y;
	s1 = 15*40000/fs - 2.5*cover;
	s2 = 12*40000/fs;
	ss = min(s1,s2);
	if(ss-m_sect.bar_inv_y>0)
		Crack_Check2.iOK = true;
	else 
		Crack_Check2.iOK = false;
	Crack_Check2.CalVal = m_sect.bar_inv_y;
	Crack_Check2.PermitVal_lower = 0;
	Crack_Check2.PermitVal_upper = ss;

}

bool AISC::SectDesn()
{
	CalNpl();
	AmplfyN();		//考虑屈曲的轴压承载力
	CalVpl();
	if(GPara::PDM_switch){
		NMCurve_PDM();
//		DrawPDMCurve();
	}
	MemberCheck();
	CrackCheck();
//	WriteRst();
	return true;
}

bool AISC::CalNpl()
{
	SectProperty m_Sect;
	m_Sect.SectGeoInfo();
	float As = m_Sect.As;	//型钢面积
	float Asr = m_Sect.Asr;	//钢筋等效钢板面积
	float Ac = m_Sect.Ac;			//混凝土面积
	float fyd;		//钢材屈服强度
	float fsd;		//钢筋屈服强度
	float fcd;		//混凝土屈服强度
	float faic;
	fyd = m_Sect.fy;	
	fsd = m_Sect.fs;
	fcd = m_Sect.fc;
	faic = GPara::m_DesnPara->faic;
	float Npl = As*fyd+Asr*fsd+Ac*0.85*fcd;
	Pn0 = Npl;
	Npl = Npl*faic;
	GPara::NplRd = Npl;
	return true;
}

bool AISC::CalVpl()
{
	int nSects=1;
	if(GPara::m_Sect_in->m_kind>2 && GPara::m_Sect_in->m_kind<6)
		nSects = 4;
	else if(GPara::m_Sect_in->m_kind==6 || GPara::m_Sect_in->m_kind==7)
		nSects = 6;
	int H = GPara::m_Sect_in->HSect.H1;
	int B = GPara::m_Sect_in->HSect.B1;
	float Tf = GPara::m_Sect_in->HSect.RI;
	float Tw = GPara::m_Sect_in->HSect.RJ;
	int ProE = Tools::GetProE(2);
	float Fy = Tools::GetProFy_AISC(GPara::m_DesnPara->ProType);
	float Av1,Av2,Av;
	float faiv;
	Av1 = (H-2*Tf)*Tw;
	float Hratio = (H-2*Tf)/Tw;
	float Hratio_limit = 2.24*sqrt(ProE/Fy);
	if(Hratio_limit>Hratio)
		faiv = 1.0;
	else
		faiv = 0.9;
	double Vpl = faiv*0.6*Fy*nSects*Av1*1.0;
	GPara::VplRd_y = Vpl;
	//弱轴
	Av1 = 2*B*Tf;
	Vpl = faiv*0.6*Fy*nSects*Av1*1.0;
	GPara::VplRd_z = Vpl;
	return true;
}

//这个只对轴力进行放大
void AISC::AmplfyN()
{
	float Ned;
	SectProperty m_Sect;
	m_Sect.SectGeoInfo();
	m_Sect.SectCal();
	float Len = GPara::m_DesnPara->Lens*1000;
	float faic = GPara::m_DesnPara->faic;
	//有效刚度，轴力
	float C1 = 0.25+3*(m_Sect.As+m_Sect.Asr)/m_Sect.Ag;
	if(C1>0.7)
		C1 = 0.7;
	int Esr = Tools::GetBarE(2);
	int Es  = Tools::GetProE(2);
	int Ecm = Tools::GetConEcm(GPara::m_Sect_in->Concrete_mat);
	double EIeff_x,EIeff_y;
	EIeff_x = Esr*m_Sect.Isr1x+Es*m_Sect.Isx+C1*Ecm*m_Sect.Icx;
	EIeff_y = Esr*m_Sect.Isr2y+Es*m_Sect.Isy+C1*Ecm*m_Sect.Icy;
	double Pe_x = PI*PI*EIeff_x/(Len*Len);
	double Pe_y = PI*PI*EIeff_y/(Len*Len);
	double Pn_x,Pn_y;
	float rx = Pn0/Pe_x;
	float ry = Pn0/Pe_y;
	if(rx<2.25)
		Pn_x = Pn0*exp(rx*log(0.658));
	else
		Pn_x = 0.877*Pe_x;

	if(ry<2.25)
		Pn_y = Pn0*exp(ry*log(0.658));
	else
		Pn_y = 0.877*Pe_y;
	NbplRd_x = faic*Pn_x;
	NbplRd_y = faic*Pn_y;
	float ks_y = Pn_x/Pn0;
	float ks_z = Pn_y/Pn0;
	GPara::ks_y = ks_y;
	GPara::ks_z = ks_z;
	//考虑弯矩
// 	bool Amplify_x=false;
// 	bool Amplify_y=false;
// 	double Mmax_x,Mmin_x;
// 	double Mmax_y,Mmin_y;
// 	double Pc_x,Pc_y;
// 	float betadns = 1.f;
// 	float deltans;
// 	float Cm;
// 	int b = GPara::m_Sect_in->Sect_b;
// 	int h = GPara::m_Sect_in->Sect_h;
// 	float kk = GPara::m_DesnPara->uL;
// 	double M2min;
// 	if(!GPara::m_DesnPara->ifsway){	//无侧移
// 		//沿x方向
// 		float rr = 0.3*h;
// 		float LL = kk*Len/rr;
// 		float LLlim = min(34+12*Mmin_x/Mmax_x,40);
// 		if(LL>LLlim){
// 			Amplify_x=true;
// 			EIeff_x = (0.2*Ecm*m_Sect.Ig_x)/(1+betadns)+Es*m_Sect.Isx;
// 			Pc_x = PI*PI*EIeff_x/(kk*Len*Len);
// 			M2min = Ned*(0.6+0.03*h);
// 			if(M2min>Mmax_x)
// 				Cm = 1.0;
// 			else
// 				Cm = 0.6-0.4*Mmin_x/Mmax_x;
// 			deltans = Cm/(1-Ned/(0.75*Pc_x));
// 			if(deltans<1.f)
// 				deltans = 1.f;
// 			if(deltans>1.4)
// 				deltans = 1.4f;
// 			Mbed_x = Mmax_x*deltans;
// 		}
// 		else
// 			Mbed_x = Mmax_x;
// 			//沿y方向
// 		rr = 0.3*b;
// 		LL = kk*Len/rr;
// 		LLlim = min(34+12*Mmin_y/Mmax_y,40);
// 		if(LL>LLlim){
// 			Amplify_y=true;
// 			EIeff_y = (0.2*Ecm*m_Sect.Ig_y)/(1+betadns)+Es*m_Sect.Isy;
// 			Pc_y = PI*PI*EIeff_y/(kk*Len*Len);
// 			M2min = Ned*(0.6+0.03*b);
// 			if(M2min>Mmax_y)
// 				Cm = 1.0;
// 			else
// 				Cm = 0.6-0.4*Mmin_y/Mmax_y;
// 			deltans = Cm/(1-Ned/(0.75*Pc_y));
// 			if(deltans<1.f)
// 				deltans = 1.f;
// 			if(deltans>1.4)
// 				deltans = 1.4f;		
// 			Mbed_y = Mmax_y*deltans;
// 		}
// 		else
// 			Mbed_y = Mmax_y;
// 	}
}

//通用NM曲线折减
void AISC::Line1(double N_in[FirbePoints],double N_out[FirbePoints],int yz,int nCase)
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

void AISC::ReduceNMCurve(double N[][FirbePoints],double M[][FirbePoints])
{
	//前两组是y,后两组是z
	float faic = GPara::m_DesnPara->faic;
	float faib = GPara::m_DesnPara->faib;
	double N_draw[3][FirbePoints],M_draw[3][FirbePoints];
	//y方向初始化
	memcpy(M_draw[0],N[0],FirbePoints*sizeof(double));
	memcpy(M_draw[0],M[0],FirbePoints*sizeof(double));
	memcpy(M_draw[1],M[0],FirbePoints*sizeof(double));
	memcpy(M_draw[2],M[0],FirbePoints*sizeof(double));
	double Ntemp[FirbePoints];
	int tempcase = 0;
	memcpy(Ntemp,N[0],FirbePoints*sizeof(double));
	Line1(N[0],Ntemp,1,tempcase);	//第一条线折减,压缩N
	memcpy(N_draw[1],Ntemp,FirbePoints*sizeof(double));
	//设计值折减
	for(int i=0;i<FirbePoints;i++){
		N_draw[2][i] = N_draw[1][i]*faic;	//直接在第二条线上折减分项系数
		M_draw[2][i] = M_draw[1][i]*faib;
	}
	DrawFiberCurve(N_draw,M_draw,12);
	DrawFiberCurve(N_draw,M_draw,13);
	//z方向初始化
	memcpy(N_draw[0],N[2],FirbePoints*sizeof(double));
	memcpy(M_draw[0],M[2],FirbePoints*sizeof(double));
	memcpy(M_draw[1],M[2],FirbePoints*sizeof(double));
	memcpy(M_draw[2],M[2],FirbePoints*sizeof(double));

	memcpy(Ntemp,N[0],FirbePoints*sizeof(double));
	Line1(N[0],Ntemp,2,tempcase);	//第一条线折减,压缩N
	memcpy(N_draw[1],Ntemp,FirbePoints*sizeof(double));
	//设计值折减
	for(int i=0;i<FirbePoints;i++){
		N_draw[2][i] = N_draw[1][i]*faic;	//直接在第二条线上折减分项系数
		M_draw[2][i] = M_draw[1][i]*faib;
	}
	DrawFiberCurve(N_draw,M_draw,22);
	DrawFiberCurve(N_draw,M_draw,23);
}