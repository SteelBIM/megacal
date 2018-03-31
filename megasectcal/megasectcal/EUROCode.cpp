#include "StdAfx.h"
#include "EUROCode.h"
#include "Tools.h"
#include "MegaCalIO.h"
#include "GlobalResult.h"


EUROCode::EUROCode(void)
{
}


EUROCode::~EUROCode(void)
{

}

bool EUROCode::SectDesn()
{
	CalNpl();
	CalVpl();
	AmplifyN();		//屈曲系数一开始就算好
	if(GPara::PDM_switch){
		NMCurve_PDM();
		NMCurve_PDM_Nominal();
//		DrawPDMCurve();
		int fiber=0;
		DrawPDMCurve(11);
		DrawPDMCurve(21);
		DrawPDMCurve(12);
		DrawPDMCurve(22);
		DrawNMPoint(fiber,1);
		DrawNMPoint(fiber,2);
	}
	MemberCheck();
	if(GPara::m_DesnPara->ifCrackCheck)
		CrackCheck();
	JudgeSect();
//	WriteRst();
	return true;
}

bool EUROCode::CalNpl()
{
	SectProperty m_Sect;
	m_Sect.SectGeoInfo();
	float As = m_Sect.As;	//型钢面积
	float Asr = m_Sect.Asr;	//钢筋等效钢板面积
	float Ac = m_Sect.Ac;			//混凝土面积
	float fyd;		//钢材屈服强度
	float fsd;		//钢筋屈服强度
	float fcd;		//混凝土屈服强度
	fyd = m_Sect.fy/1.f;	//欧洲规范有材料分项系数
	fsd = m_Sect.fs/1.15;
	fcd = m_Sect.fc/1.5;
	float Npl = As*fyd+Asr*fsd+Ac*0.85*fcd;
	float Npm = Ac*0.85*fcd;
	float Npk = As*m_Sect.fy+Asr*m_Sect.fs+Ac*0.85*m_Sect.fc;
	GPara::NplRd = Npl;
	GPara::NplRk = Npk;
	return true;
}

bool EUROCode::CalVpl()
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
	float Av1,Av2,Av;
	Av1 = H*B - (H-2*Tf)*(B-Tw);
	Av1 = (Av1-2*B*Tf+Tw*Tf)*nSects;
	Av2 = nSects*(H-2*Tf)*Tw;
	if(Av1>Av2)
		Av = Av1;
	else
		Av = Av2;
	float fy = Tools::GetProFy(GPara::m_DesnPara->ProType,GPara::m_Sect_in->HSect.RI,GPara::m_DesnPara->ProLevel);		//钢材屈服强度
	float rM0 = GPara::m_DesnPara->gama0;		//系数
	float Vpl = Av*fy/1.732/rM0;
	GPara::VplRd_z = Vpl;
	//y方向承载力
	Av = nSects*2*B*Tf;
	Vpl = Av*fy/1.732/rM0;
	GPara::VplRd_y = Vpl;
	return true;
}

void EUROCode::MemberCheck()
{
	SectProperty m_Sectinfo;
	m_Sectinfo.SectGeoInfo();
	//混凝土
 	int stru_class = GPara::m_DesnPara->Struc_Class;
 	int exposure_class = GPara::m_DesnPara->Exp_Class;
	float Cmindur = Tools::GetCmindur_EURO(stru_class,exposure_class);
	float Cnom,Cdev;
	Cdev = 10;
	float Cmin = max(GPara::m_Sect_in->Bar_d,max(Cmindur,10));
	Cnom = Cmin+Cdev;
	//y
	GlobalResult::Con_CoverCheck_y.PermitVal_lower = Cnom;
	GlobalResult::Con_CoverCheck_y.PermitVal_upper = 0.4*GPara::m_Sect_in->Sect_b;
	GlobalResult::Con_CoverCheck_y.CalVal = GPara::m_Sect_in->Con_coverd_x;
	Tools::CompareVal(GlobalResult::Con_CoverCheck_y);
	//z
	GlobalResult::Con_CoverCheck_z.PermitVal_lower = min(40,min(Cnom,GPara::m_Sect_in->HSect.B1/6));
	GlobalResult::Con_CoverCheck_z.PermitVal_upper = 0.3*GPara::m_Sect_in->Sect_h;
	GlobalResult::Con_CoverCheck_z.CalVal = GPara::m_Sect_in->Con_coverd_y;
	Tools::CompareVal(GlobalResult::Con_CoverCheck_z);


	float hb_ratio = GPara::m_Sect_in->Sect_h/GPara::m_Sect_in->Sect_b;
	if(hb_ratio>1)
		hb_ratio = 1/hb_ratio;
	GlobalResult::Con_HBCheck.PermitVal_lower = 0.2;
	GlobalResult::Con_HBCheck.PermitVal_upper = 5.0;
	GlobalResult::Con_HBCheck.CalVal = hb_ratio;
	Tools::CompareVal(GlobalResult::Con_HBCheck); 

	//型钢
	float Pro_fy = m_Sectinfo.fy;
	float delta = m_Sectinfo.As*Pro_fy/GPara::NplRd;
	GlobalResult::Pro_RatioCheck.PermitVal_lower = 0.2;
	GlobalResult::Pro_RatioCheck.PermitVal_upper = 0.9;
	GlobalResult::Pro_RatioCheck.CalVal = delta;
	Tools::CompareVal(GlobalResult::Pro_RatioCheck);

	//纵筋
	GlobalResult::Bar_RatioCheck.PermitVal_lower = 0.3;
	GlobalResult::Bar_RatioCheck.PermitVal_upper = 6;
	GlobalResult::Bar_RatioCheck.CalVal = m_Sectinfo.Asr*100/m_Sectinfo.Ac;
	Tools::CompareVal(GlobalResult::Bar_RatioCheck);
		//最小纵筋数量无法校核
	GlobalResult::Bar_SpaceCheck.PermitVal_lower = max(GPara::m_Sect_in->Bar_d,max(GPara::m_DesnPara->Dagg+5,20));
	GlobalResult::Bar_SpaceCheck.PermitVal_upper = 10000;
	GlobalResult::Bar_SpaceCheck.CalVal = GPara::m_Sect_in->Bar_interval;
	Tools::CompareVal(GlobalResult::Bar_SpaceCheck);

	GlobalResult::Bar_BarDCheck.PermitVal_lower = 8;
	GlobalResult::Bar_BarDCheck.PermitVal_upper = 10000;
	GlobalResult::Bar_BarDCheck.CalVal = GPara::m_Sect_in->Bar_d;
	Tools::CompareVal(GlobalResult::Bar_BarDCheck);

	//箍筋
// 	float Min_b = min(GPara::m_Sect_in->Sect_b,GPara::m_Sect_in->Sect_h);
// 	float Min_c = min(20*GPara::m_Sect_in->Bar_d,min(Min_b,400));
// 	Tie_SpaceCheck.PermitVal_upper = Min_c;
// 	Tie_SpaceCheck.PermitVal_lower = 10000;
// 	Tie_SpaceCheck.CalVal = m_Sectinfo.Tie_inv;
// 	Tools::CompareVal(Tie_SpaceCheck);
// 	Tie_DiaCheck.PermitVal_lower = max(6,0.25*GPara::m_Sect_in->Bar_d);
// 	Tie_DiaCheck.PermitVal_upper = 10000;
// 	Tie_DiaCheck.CalVal = m_Sectinfo.Tie_d;
// 	Tools::CompareVal(Tie_DiaCheck);
}

void EUROCode::CrackCheck()
{
	SectProperty m_Sect;
	m_Sect.SectGeoInfo();
	//使用准永久组合
	float Qs = fabs(GPara::m_DesnPara->BarQ);	//钢筋应力,需要从李文辉的纤维模型中获得
	float hnx = GPara::hnx;
	float hny = GPara::hny;	
	float As = m_Sect.As;
//	int cover = GPara::m_Sect_in->Concrete_coverd;
	int cover_y = GPara::m_Sect_in->Con_coverd_x;
	int cover_z = GPara::m_Sect_in->Con_coverd_y;
	//h~y b~x
	int ProE = Tools::GetProE(1);
	int ConLevel = GPara::m_Sect_in->Concrete_mat;
	int h = GPara::m_Sect_in->Sect_h;
	int b = GPara::m_Sect_in->Sect_b;
	int BarD = GPara::m_Sect_in->Bar_d;
	float xx = hnx+b*0.5;
	float hcef_x = min(min(2.5*cover_y,(b-xx)/3),b*0.5);
	float Aceff_x = hcef_x*b;
	float rpeff_x = As/Aceff_x;
	float Ecm,fcm,fctm;
	Tools::GetConPara_EURO(ConLevel,fcm,Ecm,fctm);
	float fcteff = fctm;
	float Ecp = GetCreepEc();
	float aE = ProE/Ecp;
	double smcm_x = (Qs-0.4*fcteff*(1+aE*rpeff_x)/rpeff_x)/ProE;

	float yy = hny+h*0.5;
	float hcef_y = min(min(2.5*cover_z,(h-yy)/3),h*0.5);
	float Aceff_y = hcef_y*h;
	float rpeff_y = As/Aceff_y;
	double smcm_y = (Qs-0.4*fcteff*(1+aE*rpeff_y)/rpeff_y)/ProE;

	//判断裂缝计算情况
	float Srmax_x,Srmax_y;
	float bar_inv = GPara::m_Sect_in->Bar_interval;
	float cover = GPara::m_Sect_in->Con_coverd_x;
	float bard = GPara::m_Sect_in->Bar_d;
	if(bar_inv<=5*(cover+bard*0.5)){
		Srmax_x = 3.4*cover_y+0.8*0.5*0.425*BarD/rpeff_x;
	}
	else{
		Srmax_x = 1.3*(b-xx);
	}
	cover = GPara::m_Sect_in->Con_coverd_y;
	if(bar_inv<=5*(cover+bard*0.5)){
		Srmax_y = 3.4*cover_z+0.8*0.5*0.425*BarD/rpeff_y;
	}
	else{
		Srmax_y = 1.3*(h-yy);
	}
	//记录结果数据
	GlobalResult::Crack_Desn.ae = aE;
	GlobalResult::Crack_Desn.hcef_y = hcef_x;
	GlobalResult::Crack_Desn.hcef_z = hcef_y;
	GlobalResult::Crack_Desn.Aceff_y = Aceff_x;
	GlobalResult::Crack_Desn.Aceff_z = Aceff_y;
	GlobalResult::Crack_Desn.peff_y = rpeff_x;
	GlobalResult::Crack_Desn.peff_z = rpeff_y;
	GlobalResult::Crack_Desn.em_y = smcm_x;
	GlobalResult::Crack_Desn.em_z = smcm_y;
	GlobalResult::Crack_Desn.srmax_y = Srmax_x;
	GlobalResult::Crack_Desn.srmax_z = Srmax_y;

	float wk_x = Srmax_x*smcm_x;
	float wk_y = Srmax_y*smcm_y;
	float wmax;
	if(GPara::m_DesnPara->Exp_Class==0 || GPara::m_DesnPara->Exp_Class==1)
		wmax = 0.4;
	else 
		wmax = 0.3;

	if(wmax-wk_x>0)
		GlobalResult::Crack_Check1.iOK = true;
	else
		GlobalResult::Crack_Check1.iOK = false;
	GlobalResult::Crack_Check1.CalVal = wk_x;
	GlobalResult::Crack_Check1.PermitVal_lower = 0;
	GlobalResult::Crack_Check1.PermitVal_upper = wmax;

	if(wmax-wk_y>0)
		GlobalResult::Crack_Check2.iOK = true;
	else 
		GlobalResult::Crack_Check2.iOK = false;
	GlobalResult::Crack_Check2.CalVal = wk_y;
	GlobalResult::Crack_Check2.PermitVal_lower = 0;
	GlobalResult::Crack_Check2.PermitVal_upper = wmax;
}

bool EUROCode::JudgeSect()
{
	Force_combine Colmforce;
	Colmforce.GetForce();
	Colmforce.HeZai_combine();
	SectProperty m_Sect;
	m_Sect.SectGeoInfo();
	m_Sect.SectCal();

	//轴向承载力
	double Ned = GPara::N_max;
	float RN = Ned/GPara::NplRd;
	if(RN<=1)
		GlobalResult::AixalForce_Check.iOK = true;
	else 
		GlobalResult::AixalForce_Check.iOK = false;
	GlobalResult::AixalForce_Check.CalVal = Ned;
	GlobalResult::AixalForce_Check.PermitVal_upper = GPara::NplRd;
	GlobalResult::AixalForce_Check.floatVal[0] = RN;

	//截面剪切承载力
	double Ved_z=GPara::Vz_max;
	double Ved_y=GPara::Vy_max;
	float RVz = fabs(Ved_z)/GPara::VplRd_z;
	float Rvy = fabs(Ved_y)/GPara::VplRd_y;
	if(Rvy<=1)
		GlobalResult::ShearForcevy_Check.iOK = true;
	else 
		GlobalResult::ShearForcevy_Check.iOK = false;
	GlobalResult::ShearForcevy_Check.CalVal = Ved_y;
	GlobalResult::ShearForcevy_Check.PermitVal_upper = GPara::VplRd_y;
	GlobalResult::ShearForcevy_Check.floatVal[0] = Rvy;

	if(RVz<=1)
		GlobalResult::ShearForcevz_Check.iOK = true;
	else
		GlobalResult::ShearForcevz_Check.iOK = false;
	GlobalResult::ShearForcevz_Check.CalVal = Ved_z;
	GlobalResult::ShearForcevz_Check.PermitVal_upper = GPara::VplRd_z;
	GlobalResult::ShearForcevz_Check.floatVal[0] = RVz;
	//截面受弯承载力
 	double Myed,Mzed;
// 	Myed = GPara::My_max;
// 	Mzed = GPara::Mz_max;
// 	float RMy = Myed/GPara::MyplNRd;
// 	float RMz = Myed/GPara::MzplNRd;
// 	if((0.8-RMy)>=0.001)
// 		My_Check.iOK = true;
// 	else
// 		My_Check.iOK = false;
// 	My_Check.CalVal = Myed;
// 	My_Check.PermitVal_upper = GPara::MyplNRd;
// 
// 	if((0.8-RMz)>0.001)
// 		Mz_Check.iOK = true;
// 	else
// 		Mz_Check.iOK = false;
// 	Mz_Check.CalVal = Mzed;
// 	Mz_Check.PermitVal_upper = GPara::MzplNRd;

	float RMyz = 0.f;
	int ctrl_i,ctrl_j;
	double ctrl_My,ctrl_Mz;
	ctrl_My = ctrl_Mz = 0;
	float kky,kkz;
	double Med1[2],Med2[2];
	double Vyed,Vzed;
	float fyd,fsd,fcd;
	fyd = m_Sect.fy/1.f;	//欧洲规范有材料分项系数
	fsd = m_Sect.fs/1.15;
	fcd = m_Sect.fc/1.5;
//	double Ned;
	//截面承载力，不放大
	for(int i=0;i<GPara::m_DesnPara->n_Combines;i++){
		for(int j=0;j<Parts;j++){
			Vyed = Colmforce.Vy_zuhe[i][j];
			Vzed = Colmforce.Vz_zuhe[i][j];
			double Vax = Vyed*(m_Sect.Zsx*fyd/GlobalResult::MmaxRd_y);
			double Vay = Vzed*(m_Sect.Zsy*fyd/GlobalResult::MmaxRd_z);
			float roux = 1.f;
			float rouy = 1.f;
			if(GPara::m_DesnPara->CodeType==1){
				if(Vax>0.5*GPara::VplRd_y){
					roux = (2*Vyed/GPara::VplRd_y-1)*(2*Vyed/GPara::VplRd_y-1);
				}
				if(Vay>0.5*GPara::VplRd_z){
					rouy = (2*Vzed/GPara::VplRd_z-1)*(2*Vzed/GPara::VplRd_z-1);
				}
			}
			GlobalResult::Vrou_y[i][j] = roux;
			GlobalResult::Vrou_z[i][j] = rouy;
			Myed = Colmforce.My_zuhe[i][j];
			Mzed = Colmforce.Mz_zuhe[i][j];
			Ned  = Colmforce.N_zuhe[i][j];
			int yz = 1;
			double MyplNRd = GetMFromN_PDM(Ned,yz);
			yz = 2;
			double MzplNRd = GetMFromN_PDM(Ned,yz);
			double r1 = Myed/MyplNRd;
			double r2 = Mzed/MzplNRd;
			double rtemp = fabs(r1) + fabs(r2);
			if(rtemp>RMyz){
				RMyz = rtemp;
				ctrl_i = i;
				ctrl_j = j;
				ctrl_My = Myed;
				ctrl_Mz = Mzed;
			}
		}
	}
	Myed = ctrl_My;	//Colmforce.My[ctrl_i][ctrl_j];
	Mzed = ctrl_Mz;	//Colmforce.Mz[ctrl_i][ctrl_j];
	GlobalResult::loadcomb.Myz_comb = ctrl_i;
	GlobalResult::loadcomb.Myz_pos = ctrl_j;
	GlobalResult::draw_point.My = Myed;
	GlobalResult::draw_point.Mz = Mzed;
	GlobalResult::draw_point.N  = Ned;

	float RMy = fabs(Myed/GPara::MyplNRd);
	float RMz = fabs(Myed/GPara::MzplNRd);
	if((0.8-RMy)>=0.001)
		GlobalResult::My_Check.iOK = true;
	else
		GlobalResult::My_Check.iOK = false;
	GlobalResult::My_Check.CalVal = Myed;
	GlobalResult::My_Check.PermitVal_upper = GPara::MyplNRd;
	GlobalResult::My_Check.floatVal[0] = RMy;

	if((0.8-RMz)>0.001)
		GlobalResult::Mz_Check.iOK = true;
	else
		GlobalResult::Mz_Check.iOK = false;
	GlobalResult::Mz_Check.CalVal = Mzed;
	GlobalResult::Mz_Check.PermitVal_upper = GPara::MzplNRd;
	GlobalResult::Mz_Check.floatVal[0] = RMz;

	if(RMyz<=1)
		GlobalResult::MNV_Check.iOK = true;
	else
		GlobalResult::MNV_Check.iOK = false;
	GlobalResult::MNV_Check.floatVal[0] = RMyz;

	//在构件验算的时候考虑二阶放大
	double max_rat_y,max_rat_z;
	max_rat_y = max_rat_z = 0;
	for(int i=0;i<GPara::m_DesnPara->n_Combines;i++){
		for(int j=0;j<Parts;j++){
			Med1[0] = Colmforce.My_zuhe[i][0];
			Med1[1] = Colmforce.My_zuhe[i][12];
			Med2[0] = Colmforce.Mz_zuhe[i][0];
			Med2[1] = Colmforce.Mz_zuhe[i][12];
			Ned = Colmforce.N_zuhe[i][j];
			AmplifyM(Med1,Med2,kky,kkz,i);
			if(Colmforce.My[i][j]>0)
				Myed = kky*Colmforce.My[i][j];
			else
				Myed = Colmforce.My[i][j] - (kky-1)*Colmforce.My[i][j];
			if(Colmforce.Mz[i][j]>0)
				Mzed = kkz*Colmforce.Mz[i][j];
			else
				Mzed = Colmforce.Mz[i][j] - (kkz-1)*Colmforce.Mz[i][j];
			int yz = 1;
			double MyplNRd = GetMFromN_PDM(Ned,yz);
			yz = 2;
			double MzplNRd = GetMFromN_PDM(Ned,yz);
			double r1 = Myed/MyplNRd;
			double r2 = Mzed/MzplNRd;
			double rat_y = Myed/Ned/GPara::m_Sect_in->Sect_h;
			if(fabs(rat_y)>max_rat_y)
				max_rat_y = fabs(rat_y);
			double rat_z = Mzed/Ned/GPara::m_Sect_in->Sect_b;
			if(fabs(rat_z)>max_rat_z)
				max_rat_z = fabs(rat_z);
			double rtemp = fabs(r1) + fabs(r2);
			if(rtemp>RMyz){
				RMyz = rtemp;
				ctrl_i = i;
				ctrl_j = j;
				ctrl_My = Myed;
				ctrl_Mz = Mzed;
			}
		}
	}
	GlobalResult::MN15y_Check.CalVal = max_rat_y;
	GlobalResult::MN15z_Check.CalVal = max_rat_z;
	Myed = ctrl_My;	//Colmforce.My[ctrl_i][ctrl_j];
	Mzed = ctrl_Mz;	//Colmforce.Mz[ctrl_i][ctrl_j];
	GlobalResult::loadcomb.bMyz_comb = ctrl_i;
	GlobalResult::loadcomb.bMyz_pos = ctrl_j;

	RMy = fabs(Myed)/GPara::MyplNRd;
	RMz = fabs(Myed)/GPara::MzplNRd;
	if((0.8-RMy)>=0.001)
		GlobalResult::bMy_Check.iOK = true;
	else
		GlobalResult::bMy_Check.iOK = false;
	GlobalResult::bMy_Check.CalVal = Myed;
	GlobalResult::bMy_Check.PermitVal_upper = GPara::MyplNRd;
	GlobalResult::bMy_Check.floatVal[0] = RMy;

	if((0.8-RMz)>0.001)
		GlobalResult::bMz_Check.iOK = true;
	else
		GlobalResult::bMz_Check.iOK = false;
	GlobalResult::bMz_Check.CalVal = Mzed;
	GlobalResult::bMz_Check.PermitVal_upper = GPara::MzplNRd;
	GlobalResult::bMz_Check.floatVal[0] = RMz;

	if(RMyz<=1)
		GlobalResult::bMNV_Check.iOK = true;
	else
		GlobalResult::bMNV_Check.iOK = false;
	GlobalResult::bMNV_Check.floatVal[0] = RMyz;
//	MNV_Check.PermitVal_upper = 1.f;

	//构件轴向承载力
	float Rby,Rbz;
	Rby = Ned/(GPara::ks_y*GPara::NplRd);
	Rbz = Ned/(GPara::ks_z*GPara::NplRd);

	if(Rby<=1)
		GlobalResult::BucklingNby_Check.iOK = true;
	else 
		GlobalResult::BucklingNby_Check.iOK = false;
	GlobalResult::BucklingNby_Check.CalVal = Ned;
	GlobalResult::BucklingNby_Check.PermitVal_upper = GPara::ks_y*GPara::NplRd;
	GlobalResult::BucklingNby_Check.floatVal[0] = Rby;

	if(Rbz<=1)
		GlobalResult::BucklingNbz_Check.iOK = true;
	else
		GlobalResult::BucklingNbz_Check.iOK = false;
	GlobalResult::BucklingNbz_Check.CalVal = Ned;
	GlobalResult::BucklingNbz_Check.PermitVal_upper = GPara::ks_z*GPara::NplRd;
	GlobalResult::BucklingNbz_Check.floatVal[0] = Rbz;

	return true;
}
// 计算欧拉系数  待查。
float EUROCode::GetCreepEc()
{
	float Ecp;
	SectProperty m_Sect;
	m_Sect.SectGeoInfo();
	int h = GPara::m_Sect_in->Sect_h;
	int b = GPara::m_Sect_in->Sect_b;
	float RH = GPara::m_DesnPara->RH;
	int day_t0 = GPara::m_DesnPara->day_t0;
	int u = 2*(b+h);
	float h0 = 2*m_Sect.Ac/u;
	float fcm,Ecm,fctm;
	float fRH;
	Tools::GetConPara_EURO(GPara::m_Sect_in->Concrete_mat,fcm,Ecm,fctm);
	float alpha1 = exp(0.7*log(35/fcm));
	float alpha2 = exp(0.2*log(35/fcm));
	float temp1 = 0.1*exp(3*log(h0));
	if(fcm>35)
		fRH = (1+(1-(RH/100))*alpha1/temp1)*alpha2;
	else
		fRH = 1+(1-RH/100)/temp1;
	float betafcm = 16.8/sqrt(fcm);
	float betat0 = 1/(0.1+exp(0.2*log(day_t0*1.f)));
	float fait0 = fRH*betafcm*betat0;
	Ecp = Ecm/(1+fait0*GPara::Nsq);
	//统一把中间变量存一下
	GlobalResult::con_u = u;
	GlobalResult::con_h0 = h0;
	GlobalResult::fai0 = fait0;
	return Ecp;
}

//根据规范放大端部弯矩和屈曲折减
void EUROCode::AmplifyM(double Myed[2],double Mzed[2],float &kky,float &kkz,int nLoadcase)
{	
	int Es = Tools::GetProE(1);
	int Esr = Tools::GetBarE(1);
	float Ecp = GetCreepEc();
	double L0 = GPara::m_DesnPara->uL*GPara::m_DesnPara->Lens*1000;
	SectProperty m_Sect;
	m_Sect.SectCal();
	//根据约束条件来确定beta值
	float beta_y,beta_z;
	if(GPara::m_DesnPara->EC_beta<0){
	if(GPara::m_DesnPara->Restrict = 3 && GPara::m_DesnPara->My_hasmidforce){	//两端铰接，且有跨中有荷载
		beta_y = 1.0;
	}
	else{
		float r1 = Myed[0]/Myed[1];
		if(fabs(r1)>1)
			r1 = 1/r1;
		beta_y = 0.66+0.44*r1;
		if(beta_y<0.44)
			beta_y = 0.44;
	}
	}
	else{
		//beta_y = GPara::m_DesnPara->EC_beta;
		beta_y = GPara::m_DesnPara->combbeta[nLoadcase];
	}
	if(GPara::m_DesnPara->EC_beta<0){
	if(GPara::m_DesnPara->Restrict = 3 && GPara::m_DesnPara->Mz_hasmidforce){	//两端铰接，且有跨中有荷载
		beta_z = 1.0;
	}
	else{
		float r1 = Mzed[0]/Mzed[1];
		if(fabs(r1)>1)
			r1 = 1/r1;
		beta_z = 0.66+0.44*r1;
		if(beta_z<0.44)
			beta_z = 0.44;
	}
	}
	else{
		//beta_z = GPara::m_DesnPara->EC_beta;
		beta_z =GPara::m_DesnPara->combbeta[nLoadcase];
	}
//	else if(GPara::m_DesnPara->Restrict==4)	//悬臂，梯形
//		beta = 0.66;
	double EIeff1_x,EIeff1_y;
	double Ncreff_x,Ncreff_y;
	float K0,K1;
	K0 = 0.9;
	K1 = 0.5;
	EIeff1_x = K0*(Es*m_Sect.Isx+Esr*m_Sect.Isr1x+K1*Ecp*m_Sect.Icx);
	Ncreff_x = PI*PI*EIeff1_x/(L0*L0);
//	if(fabs(Ned)<0.001)		//判断是传入的N还是直接取最大值
	double Ned=GPara::N_max;
	float KK_x = beta_y/(1-Ned/Ncreff_x);
	if(KK_x<1)
		KK_x = 1.f;
	EIeff1_y = K0*(Es*m_Sect.Isy+Esr*m_Sect.Isr2x+K1*Ecp*m_Sect.Icy);
	Ncreff_y = PI*PI*EIeff1_y/(L0*L0);
	float KK_y = beta_z/(1-Ned/Ncreff_y);
	if(KK_y<1)
		KK_y = 1.f;
	//保存一下
//	GlobalResult::beta = beta;
	GlobalResult::EIeff1_y = EIeff1_x;
	GlobalResult::EIeff1_z = EIeff1_y;
	GlobalResult::Ncreff_y = Ncreff_x;
	GlobalResult::Ncreff_z = Ncreff_y;
	GPara::kk_y[nLoadcase] = KK_x;
	GPara::kk_z[nLoadcase] = KK_y;
	GPara::beta_y[nLoadcase]=beta_y;
	GPara::beta_z[nLoadcase]=beta_z;
	kky = KK_x;
	kkz = KK_y;
}

void EUROCode::AmplifyN()
{
	double EIeff_x,EIeff_y;
	double Ncr_x,Ncr_y;
	int Es = Tools::GetProE(1);
	int Esr = Tools::GetBarE(1);
	double L0 = GPara::m_DesnPara->uL*GPara::m_DesnPara->Lens*1000;
	SectProperty m_Sect;
	m_Sect.SectGeoInfo();
	m_Sect.SectCal();
	float Ecp = GetCreepEc();
	EIeff_x = Es*m_Sect.Isx+Esr*m_Sect.Isr1x+GPara::Ke*Ecp*m_Sect.Icx;
	Ncr_x = PI*PI*EIeff_x/(L0*L0);
	float LL = sqrt(GPara::NplRk/Ncr_x);
	GlobalResult::ll_y = LL;
	//强轴的曲线b
	float ff = 0.5*(1+0.34*(LL-0.2)+LL*LL);
	float ks = 1/(ff+sqrt(ff*ff-LL*LL));
	if(ks>1)
		ks = 1;
	GPara::ks_y = ks;
	//弱轴的曲线c
	EIeff_y = Es*m_Sect.Isy+Esr*m_Sect.Isr2y+GPara::Ke*Ecp*m_Sect.Icy;
	Ncr_y = PI*PI*EIeff_y/(L0*L0);
	LL = sqrt(GPara::NplRk/Ncr_y);
	GlobalResult::ll_z = LL;
	ff = 0.5*(1+0.49*(LL-0.2)+LL*LL);
	ks = 1/(ff+sqrt(ff*ff-LL*LL));
	GPara::ks_z = ks;
	//保存一下结果
	GlobalResult::Eceff = Ecp;
	GlobalResult::EIeff_y = EIeff_x;
	GlobalResult::EIeff_z = EIeff_y;
	GlobalResult::Ncr_y = Ncr_x;
	GlobalResult::Ncr_z = Ncr_y;
	GlobalResult::Lx_y = GPara::ks_y;
	GlobalResult::Lx_z = GPara::ks_z;
}

//考虑长度效应的折减曲线，M不折减，标准值和设计值通用
void EUROCode::Line1(double N_in[FirbePoints],double N_out[FirbePoints],int yz,int nCase)
{
	float fai,kk;
	int npoints;
	if(yz==1){
		fai = GPara::ks_y;
		kk  = GPara::kk_y[nCase];
		npoints = GPara::m_DesnPara->Fiberpoints_y;
	}
	else if(yz==2){
		fai = GPara::ks_z;
		kk  = GPara::kk_z[nCase]; 
		npoints = GPara::m_DesnPara->Fiberpoints_z;
	}

	for(int i=0;i<FirbePoints;i++){
		N_out[i] = N_in[i]*fai;
	}
}

void EUROCode::ReduceNMCurve(double N[][FirbePoints],double M[][FirbePoints])
{
	int fiber = 1;
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
//		memcpy(Ntemp,N[i],FirbePoints*sizeof(double));
		Line1(N[i],Ntemp,1,tempcase);
		memcpy(N_draw[i+1],Ntemp,FirbePoints*sizeof(double));
	}
	DrawFiberCurve(N_draw,M_draw,12);
	DrawFiberCurve(N_draw,M_draw,13);
	DrawNMPoint(fiber,1);
	//z方向初始化
	memcpy(N_draw[0],N[2],FirbePoints*sizeof(double));
	memcpy(M_draw[0],M[2],FirbePoints*sizeof(double));
	memcpy(M_draw[1],M[2],FirbePoints*sizeof(double));
	memcpy(M_draw[2],M[3],FirbePoints*sizeof(double));
	for(int i=2;i<4;i++){
//		memcpy(Ntemp,N[i],FirbePoints*sizeof(double));
		Line1(N[i],Ntemp,2,tempcase);
		memcpy(N_draw[i-1],Ntemp,FirbePoints*sizeof(double));
	}
	DrawFiberCurve(N_draw,M_draw,22);
	DrawFiberCurve(N_draw,M_draw,23);
	DrawNMPoint(fiber,2);
}

bool EUROCode::JudgeSect_Fiber(double N[][FirbePoints],double M[][FirbePoints])
{
	Force_combine Colmforce;
	Colmforce.GetForce();
	Colmforce.HeZai_combine();
	double Nmax_y = N[1][0];
	double Nmax_z = N[3][0];
	//轴向承载力
	double RN_y,RN_z;
	double Ned,tempNed;
	Ned=0.f;
	for(int i=0;i<GPara::m_DesnPara->n_Combines;i++){
		for(int j=0;j<Parts;j++){
			tempNed = fabs(Colmforce.N_zuhe[i][j]);
			if(tempNed>Ned){
				Ned = tempNed;
			}
		}
	}
	RN_y = Ned/Nmax_y;
	RN_z = Ned/Nmax_z;
//	float RN = Ned/GPara::NplRd;
	if(RN_y<=1)
		GlobalResult::BucklingNby_Check.iOK = true;
	else 
		GlobalResult::BucklingNby_Check.iOK = false;
	GlobalResult::BucklingNby_Check.CalVal = Ned;
	GlobalResult::BucklingNby_Check.PermitVal_upper = Nmax_y;
	GlobalResult::BucklingNby_Check.floatVal[0] = RN_y;

	if(RN_z<=1)
		GlobalResult::BucklingNbz_Check.iOK = true;
	else
		GlobalResult::BucklingNbz_Check.iOK = false;
	GlobalResult::BucklingNbz_Check.CalVal = Ned;
	GlobalResult::BucklingNbz_Check.PermitVal_upper = Nmax_z;
	GlobalResult::BucklingNbz_Check.floatVal[0] = RN_z;

	//截面受弯承载力
	double Myed,Mzed;
	double MyplNRd,MzplNRd;
	double tempMypd,tempMzpd;
	float RMyz = 0.f;
	int ctrl_i,ctrl_j;
	double ctrl_My,ctrl_Mz;
	ctrl_My = ctrl_Mz = 0;
	ctrl_i = ctrl_j=0;
	float kky,kkz;
	double Med1[2],Med2[2];
	//	double Ned;
	//截面承载力，不放大
	float RMy,RMz;
	RMy = RMz = 0.f;
	for(int i=0;i<GPara::m_DesnPara->n_Combines;i++){
		for(int j=0;j<Parts;j++){
			Myed = Colmforce.My_zuhe[i][j];
			Mzed = Colmforce.Mz_zuhe[i][j];
			Ned  = Colmforce.N_zuhe[i][j];
			tempMypd = Tools::GetMnplFromCurve(N[1],M[1],Ned);
//			double MyplNRd = GetMFromN_PDM(Ned,yz);
			tempMzpd = Tools::GetMnplFromCurve(N[3],M[3],Ned);
//			double MzplNRd = GetMFromN_PDM(Ned,yz);
			double r1 = Myed/tempMypd;
			double r2 = Mzed/tempMzpd;
			double rtemp = fabs(r1) + fabs(r2);
			if(fabs(r1)>RMy){
				RMy = fabs(r1);
			}
			if(fabs(r2)>RMz){
				RMz = fabs(r2);
			}
			if(rtemp>RMyz){
				RMyz = rtemp;
				ctrl_i = i;
				ctrl_j = j;
				ctrl_My = Myed;
				ctrl_Mz = Mzed;
				MyplNRd = tempMypd;
				MzplNRd = tempMzpd;
			}
		}
	}
	Myed = ctrl_My;	//Colmforce.My[ctrl_i][ctrl_j];
	Mzed = ctrl_Mz;	//Colmforce.Mz[ctrl_i][ctrl_j];
	GlobalResult::loadcomb.Myz_comb = ctrl_i;
	GlobalResult::loadcomb.Myz_pos = ctrl_j;
	GlobalResult::draw_point.My = Myed;
	GlobalResult::draw_point.Mz = Mzed;
	GlobalResult::draw_point.N  = Ned;

// 	float RMy = fabs(Myed/GPara::MyplNRd);
// 	float RMz = fabs(Myed/GPara::MzplNRd);
	if((0.8-RMy)>=0.001)
		GlobalResult::My_Check.iOK = true;
	else
		GlobalResult::My_Check.iOK = false;
	GlobalResult::My_Check.CalVal = Myed;
	GlobalResult::My_Check.PermitVal_upper = GPara::MyplNRd;
	GlobalResult::My_Check.floatVal[0] = RMy;

	if((0.8-RMz)>0.001)
		GlobalResult::Mz_Check.iOK = true;
	else
		GlobalResult::Mz_Check.iOK = false;
	GlobalResult::Mz_Check.CalVal = Mzed;
	GlobalResult::Mz_Check.PermitVal_upper = GPara::MzplNRd;
	GlobalResult::Mz_Check.floatVal[0] = RMz;

	if(RMyz<=1)
		GlobalResult::MNV_Check.iOK = true;
	else
		GlobalResult::MNV_Check.iOK = false;
	GlobalResult::MNV_Check.floatVal[0] = RMyz;

	//在构件验算的时候考虑二阶放大
	double max_rat_y,max_rat_z;
	max_rat_y = max_rat_z = 0;
	for(int i=0;i<GPara::m_DesnPara->n_Combines;i++){
		for(int j=0;j<Parts;j++){
			Med1[0] = Colmforce.My_zuhe[i][0];
			Med1[1] = Colmforce.My_zuhe[i][12];
			Med2[0] = Colmforce.Mz_zuhe[i][0];
			Med2[1] = Colmforce.Mz_zuhe[i][12];
			Ned = Colmforce.N_zuhe[i][j];
			AmplifyM_Fiber(Med1,Med2,kky,kkz,i);
			GPara::kk_y[i] = kky;
			GPara::kk_z[i] = kkz;
			if(Colmforce.My[i][j]>0)
				Myed = kky*Colmforce.My[i][j];
			else
				Myed = Colmforce.My[i][j] - (kky-1)*Colmforce.My[i][j];
			if(Colmforce.Mz[i][j]>0)
				Mzed = kkz*Colmforce.Mz[i][j];
			else
				Mzed = Colmforce.Mz[i][j] - (kkz-1)*Colmforce.Mz[i][j];
			tempMypd = Tools::GetMnplFromCurve(N[1],M[1],Ned);
			tempMzpd = Tools::GetMnplFromCurve(N[3],M[3],Ned);
			double r1 = Myed/tempMypd;
			double r2 = Mzed/tempMzpd;
			double rtemp = fabs(r1) + fabs(r2);
			if(fabs(r1)>RMy){
				RMy = fabs(r1);
			}
			if(fabs(r2)>RMz){
				RMz = fabs(r2);
			}
			if(rtemp>RMyz){
				RMyz = rtemp;
				ctrl_i = i;
				ctrl_j = j;
				ctrl_My = Myed;
				ctrl_Mz = Mzed;
				MyplNRd = tempMypd;
				MzplNRd = tempMzpd;
			}
		}
	}
//	GlobalResult::MN15y_Check.CalVal = max_rat_y;
//	GlobalResult::MN15z_Check.CalVal = max_rat_z;
	Myed = ctrl_My;	//Colmforce.My[ctrl_i][ctrl_j];
	Mzed = ctrl_Mz;	//Colmforce.Mz[ctrl_i][ctrl_j];
	GlobalResult::loadcomb.bMyz_comb = ctrl_i;
	GlobalResult::loadcomb.bMyz_pos = ctrl_j;

	if((0.8-RMy)>=0.001)
		GlobalResult::bMy_Check.iOK = true;
	else
		GlobalResult::bMy_Check.iOK = false;
	GlobalResult::bMy_Check.CalVal = Myed;
	GlobalResult::bMy_Check.PermitVal_upper = MyplNRd;
	GlobalResult::bMy_Check.floatVal[0] = RMy;

	if((0.8-RMz)>0.001)
		GlobalResult::bMz_Check.iOK = true;
	else
		GlobalResult::bMz_Check.iOK = false;
	GlobalResult::bMz_Check.CalVal = Mzed;
	GlobalResult::bMz_Check.PermitVal_upper = MzplNRd;
	GlobalResult::bMz_Check.floatVal[0] = RMz;

	if(RMyz<=1)
		GlobalResult::bMNV_Check.iOK = true;
	else
		GlobalResult::bMNV_Check.iOK = false;
	GlobalResult::bMNV_Check.floatVal[0] = RMyz;
	//	MNV_Check.PermitVal_upper = 1.f;

	return true;
}

void EUROCode::AmplifyM_Fiber(double Myed[2],double Mzed[2],float &kky,float &kkz,int nLoadcase)
{

	double L0 = GPara::m_DesnPara->uL*GPara::m_DesnPara->Lens*1000;

	//根据约束条件来确定beta值
	float beta_y,beta_z;
	if(GPara::m_DesnPara->EC_beta<0){
		if(GPara::m_DesnPara->Restrict = 3 && GPara::m_DesnPara->My_hasmidforce){	//两端铰接，且有跨中有荷载
			beta_y = 1.0;
		}
		else{
			float r1 = Myed[0]/Myed[1];
			if(fabs(r1)>1)
				r1 = 1/r1;
			beta_y = 0.66+0.44*r1;
			if(beta_y<0.44)
				beta_y = 0.44;
		}
	}
	else{
		//beta_y = GPara::m_DesnPara->EC_beta;
		beta_y = GPara::m_DesnPara->combbeta[nLoadcase];
	}
	if(GPara::m_DesnPara->EC_beta<0){
		if(GPara::m_DesnPara->Restrict = 3 && GPara::m_DesnPara->Mz_hasmidforce){	//两端铰接，且有跨中有荷载
			beta_z = 1.0;
		}
		else{
			float r1 = Mzed[0]/Mzed[1];
			if(fabs(r1)>1)
				r1 = 1/r1;
			beta_z = 0.66+0.44*r1;
			if(beta_z<0.44)
				beta_z = 0.44;
		}
	}
	else{
		//beta_z = GPara::m_DesnPara->EC_beta;
		beta_z = GPara::m_DesnPara->combbeta[nLoadcase];
	}
	//	else if(GPara::m_DesnPara->Restrict==4)	//悬臂，梯形
	//		beta = 0.66;
	double EIeff1_x,EIeff1_y;
	double Ncreff_x,Ncreff_y;
	float K0,K1;
	K0 = 0.9;
	K1 = 0.5;
	EIeff1_x = K0*(GPara::EIin[0]+GPara::EIin[1]+K1*0.5*GPara::EIin[2]);		//陈涛拍脑袋ecp取0.5ec
	Ncreff_x = PI*PI*EIeff1_x/(L0*L0);
	//	if(fabs(Ned)<0.001)		//判断是传入的N还是直接取最大值
	double Ned=GPara::N_max;
	float KK_x = beta_y/(1-Ned/Ncreff_x);
	if(KK_x<1)
		KK_x = 1.f;
	EIeff1_y = K0*(GPara::EIin[3]+GPara::EIin[4]+K1*0.5*GPara::EIin[5]);		//陈涛拍脑袋ecp取0.5ec
	Ncreff_y = PI*PI*EIeff1_y/(L0*L0);
	float KK_y = beta_z/(1-Ned/Ncreff_y);
	if(KK_y<1)
		KK_y = 1.f;
	//保存一下
	//	GlobalResult::beta = beta;
	kky = KK_x;
	kkz = KK_y;

}
void EUROCode::WriteRst()
{
// 	CFile m_file;
// 	CString FileName("D:\\OUT.txt");
// 	m_file.Open(FileName,CFile::modeCreate|CFile::modeWrite);
// 	m_file.SeekToBegin();
// 	CString String[80];
// 	int j=0;
// 
// 	String[j++].Format(_T("                  simple result                     "));
// 	String[j++].Format(_T("\r\n  ◆ Resistance of the cross-section to axial force \r\n"));
// 	String[j++].Format(_T("      NplRd = %.2f kN \r\n"),(GPara::NplRd)*0.001);
// //	String[j++].Format(_T("      VplRd = %.2f kN \r\n"),(GPara::VplRd)*0.001);
// 	String[j++].Format(_T("\r\n  ◆ Member Check \r\n"));
// 	//混凝土校核
// 	String[j++].Format(_T("\r\n    (1) Concrete:\r\n"));
// 	String[j++].Format(_T("      A.Cover\r\n"));
// 	if(Con_CoverCheck.iOK)
// 		String[j++].Format(_T("        %g mm =< Cy= %g mm =< %g mm \r\n"),Con_CoverCheck.PermitVal_lower,Con_CoverCheck.CalVal,Con_CoverCheck.PermitVal_upper);
// 	String[j++].Format(_T("      B.Ration of the h to b:\r\n"));
// 	if(Con_HBCheck.iOK)
// 		String[j++].Format(_T("        %g =< h1/h2=%g =< %g"),Con_HBCheck.PermitVal_lower,Con_HBCheck.CalVal,Con_HBCheck.PermitVal_upper);
// 	//型钢校核
// 	String[j++].Format(_T("\r\n    (2) Steel profile \r\n"));
// 	String[j++].Format(_T("      A.Structural steel contribution ratio \r\n"));
// 	if(Pro_RatioCheck.iOK)
// 		String[j++].Format(_T("        %g =< δ=%g =<%g \r\n"),Pro_RatioCheck.PermitVal_lower,Pro_RatioCheck.CalVal,Pro_RatioCheck.PermitVal_upper);
// 	//纵筋
// 	String[j++].Format(_T("\r\n    (3) Reinforcing bars \r\n"));
// 	String[j++].Format(_T("      A.Minimum longitudinal reinforcement ratio \r\n"));
// 	if(Bar_RatioCheck.iOK)
// 		String[j++].Format(_T("        %g%% =< Asr/Ac=%g%% =<%g%%\r\n"),Bar_RatioCheck.PermitVal_lower,Bar_RatioCheck.CalVal,Bar_RatioCheck.PermitVal_upper);
// 
// 	OutToFile(m_file,j,String);
// 	m_file.Flush();
// 	m_file.Close();
// 	ShellExecute(NULL,"open","D:\\OUT.txt",NULL,NULL,SW_SHOWNORMAL);

}