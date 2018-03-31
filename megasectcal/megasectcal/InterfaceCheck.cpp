#include "StdAfx.h"
#include "InterfaceCheck.h"
#include "SectProperty.h"
#include "GPara.h"
#include "Tools.h"
#include "GlobalResult.h"

float InterfaceCheck::stirruprate[10] = {0.05,0.1,0.2,0.4,0.6,0.8,1.0,1.2,1.6,2.0};

InterfaceCheck::InterfaceCheck(void)
{
}


InterfaceCheck::~InterfaceCheck(void)
{
}

bool InterfaceCheck::InterfaceDesn()
{
	InitCal();
	if(GPara::m_DesnPara->shearmethod==0){	//设定为梁模型
		if(GPara::Interface_beam_switch){
			CalSepVbs();
			CalInterface();
		}
	}
	else{
		float r1,r2;
		if(GPara::Interface_truss_switch){	//算桁架模型
 			r1 = GPara::m_Sect_in->Tran_ratio_x;		//第一行是用户输入的配箍率
 			r2 = GPara::m_Sect_in->Tran_ratio_y;
 			r1=r1/100;
 			r2=r2/100;
 			TrussModel(r1,r2,0);
			for(int i=1;i<10;i++){
				r1 = stirruprate[i]/100;
				r2 = stirruprate[i]/100;
				TrussModel(r1,r2,i);		//给出多个配筋结果
			}
		}
	}
	return true;
}

void InterfaceCheck::InitCal()
{
// 	int SectType;
// 	int b,h;
// 	int Hsect_b,Hsect_h;
// 	float tf,tw;
// 	int Bar_d;
// 	int Bar_nx,Bar_ny;
// 	int Bar_int;

	int SectType = GPara::m_Sect_in->m_kind;

	float h = GPara::m_Sect_in->Sect_h;
	float b = GPara::m_Sect_in->Sect_b;
	float Hsect_h = GPara::m_Sect_in->HSect.H1;
	float Hsect_b = GPara::m_Sect_in->HSect.B1;
	float tf = GPara::m_Sect_in->HSect.RI;
	float tw = GPara::m_Sect_in->HSect.RJ;
	int Bar_d = GPara::m_Sect_in->Bar_d;
	int cover = GPara::m_Sect_in->Concrete_coverd;
	int Bar_nx = GPara::m_Sect_in->Bar_num_bside;
	int Bar_ny = GPara::m_Sect_in->Bar_num_hside;
	int Bar_int = (b-2*cover)/(Bar_nx-1);
//	bar_inv_y = (h-2*cover)/(bar_y-1);

	float Ag,As,Asr;		//截面面积，型钢面积，钢筋面积
	Ag = b*h;
	As = Hsect_h*Hsect_b - (Hsect_b-tw)*(Hsect_h-2*tf);
	Asr = PI*Bar_d*Bar_d*0.25*(Bar_nx*2+Bar_ny*2-4);
	if(SectType>2 && SectType<6)
		As = As*4;
	else if(SectType==6 || SectType==7)
		As = As*6;
	//计算C1
	if(GPara::m_DesnPara->CodeType==1){
		C1 = 0.6;
	}
	else if(GPara::m_DesnPara->CodeType==2){
	C1 = 0.25+3*(As+Asr)/Ag;
	if(C1>0.7)
		C1 = 0.7;
	}
	else if(GPara::m_DesnPara->CodeType==3){
		C1 = 1.f;
	}
	//等效钢板计算
	float Vstud = GetShearStud();
	Vstud = Vstud*0.001;
	GlobalResult::Vstud = Vstud;
}

void InterfaceCheck::CalSepVbs()
{
	//测试V
	double V_x = fabs(GPara::Vy_max);
	double V_y = fabs(GPara::Vz_max);

	double EIeff_x,EIeff_y;
	double EIeffbc1,EIeffbc2,EIeffbc3;
	double EIeffbs1,EIeffbs2;
	double EIeffbc1_y,EIeffbc2_y,EIeffbc3_y;
	double EIeffbs1_y,EIeffbs2_y;
	EIeffbs1 = EIeffbs2 = EIeffbs1_y = EIeffbs2_y = 0;
	float Es,Ecm,Esr;
	int SectType = GPara::m_Sect_in->m_kind;
	int b = GPara::m_Sect_in->Sect_b;
	int h = GPara::m_Sect_in->Sect_h;
	float secth = GPara::m_Sect_in->HSect.H1;
	float sectb = GPara::m_Sect_in->HSect.B1;
	float tf = GPara::m_Sect_in->HSect.RI;
	float tw = GPara::m_Sect_in->HSect.RJ;
	int dx = GPara::m_Sect_in->HSect_x;
	int dy = GPara::m_Sect_in->HSect_y;
	Es = Tools::GetProE(GPara::m_DesnPara->CodeType);
	int ConLevel = GPara::m_Sect_in->Concrete_mat;
	Ecm = Tools::GetConEcm(ConLevel);
	Esr = Tools::GetBarE(GPara::m_DesnPara->CodeType);
	SectProperty m_Sect;
//	m_Sect.InitSect(m_SectData);
	m_Sect.SectCal();
	//绕x轴特性计算
	if(SectType==1){	//矩形单型钢
		//截面bc1纯混凝土x2
		float b_bc1 = (b-sectb)*0.5;
		float m_edge = (b - m_Sect.hs1)*0.5;
		double Ic1 = b_bc1*h*h*h/12;
		double Isr1 = (b_bc1-m_edge)*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		Isr1 = Isr1*2;
		double Isr2 = m_Sect.bs2*m_Sect.hs2*m_Sect.hs2*m_Sect.hs2/12;
		double Ic = Ic1-Isr1-Isr2;
		EIeffbc1 = Esr*(Isr1+Isr2)+C1*Ecm*Ic;
		//截面bs
		Ic1 = sectb*h*h*h/12;
		double Is = m_Sect.Is_x;
		Isr1 = m_Sect.bs1*sectb*m_Sect.d1y*m_Sect.d1y;
		Ic = Ic1 - Is - Isr1*2;
		EIeffbs1 = Es*Is+Esr*Isr1*2+C1*Ecm*Ic;
		//总的EI
		EIeff_x = EIeffbc1*2+EIeffbs1;

		//保存一下截面特性
		GlobalResult::BeamRst_y.bc1 = b_bc1;
//		GlobalResult::BeamRst_y.bc2 = b_bc2;
		GlobalResult::BeamRst_y.bs  = sectb;
		GlobalResult::BeamRst_y.EIeffbc1 = EIeffbc1;
//		GlobalResult::BeamRst_y.EIeffbc2 = EIeffbc2;
		GlobalResult::BeamRst_y.EIeffbs  = EIeffbs1;
		GlobalResult::BeamRst_y.EIeff    = EIeff_x;

		//截面绕y轴的特性
		b_bc1 = (h-secth)*0.5;
		m_edge = (h - m_Sect.hs2)*0.5;
		Ic1 = b_bc1*b*b*b/12;
		Isr1 = (b_bc1-m_edge)*m_Sect.bs2*m_Sect.d2x*m_Sect.d2x;
		Isr1 = Isr1*2;
		Isr2 = m_Sect.bs1*m_Sect.hs1*m_Sect.hs1*m_Sect.hs1/12;
		Ic = Ic1-Isr1-Isr2;
		EIeffbc1_y = Esr*(Isr1+Isr2)+C1*Ecm*Ic;
		//截面bs
		Ic1 = secth*b*b*b/12;
		Is = m_Sect.Is_y;
		Isr1 = m_Sect.bs2*secth*m_Sect.d2x*m_Sect.d2x;
		Ic = Ic1 - Is - Isr1*2;
		EIeffbs1_y = Es*Is+Esr*Isr1*2+C1*Ecm*Ic;
		//y向总的EI
		EIeff_y = EIeffbc1_y*2+EIeffbs1_y;
		//保存一下截面特性
		GlobalResult::BeamRst_z.bc1 = b_bc1;
//		GlobalResult::BeamRst_z.bc2 = b_bc2;
		GlobalResult::BeamRst_z.bs  = secth;
		GlobalResult::BeamRst_z.EIeffbc1 = EIeffbc1_y;
//		GlobalResult::BeamRst_z.EIeffbc2 = EIeffbc2_y;
		GlobalResult::BeamRst_z.EIeffbs  = EIeffbs1_y;
		GlobalResult::BeamRst_z.EIeff    = EIeff_y;
		//计算截面的剪力
		Vbs_x = V_x*EIeffbs1/EIeff_x;
		Vbs_y = V_y*EIeffbs1_y/EIeff_y;
	}
	else if(SectType==2){	//单圆管,等效成方形
		//skip
		float rr = sqrt(PI*b*b*0.25);	//按面积直接等效方形
		//截面bc1纯混凝土x2
		float b_bc1 = (rr-sectb)*0.5;
		float m_edge = (rr - m_Sect.hs1)*0.5;
		double Ic1 = b_bc1*rr*rr*rr/12;
		double Isr1 = (b_bc1-m_edge)*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		Isr1 = Isr1*2;
		double Isr2 = m_Sect.bs2*m_Sect.hs2*m_Sect.hs2*m_Sect.hs2/12;
		double Ic = Ic1-Isr1-Isr2;
		EIeffbc1 = Esr*(Isr1+Isr2)+C1*Ecm*Ic;
		//截面bs
		Ic1 = sectb*rr*rr*rr/12;
		double Is = m_Sect.Is_x;
		Isr1 = m_Sect.bs1*sectb*m_Sect.d1y*m_Sect.d1y;
		Ic = Ic1 - Is - Isr1*2;
		EIeffbs1 = Es*Is+Esr*Isr1*2+C1*Ecm*Ic;
		//总的EI
		EIeff_x = EIeffbc1*2+EIeffbs1;

		//保存一下截面特性
		GlobalResult::BeamRst_y.bc1 = b_bc1;
//		GlobalResult::BeamRst_y.bc2 = b_bc2;
		GlobalResult::BeamRst_y.bs  = sectb;
		GlobalResult::BeamRst_y.EIeffbc1 = EIeffbc1;
//		GlobalResult::BeamRst_y.EIeffbc2 = EIeffbc2;
		GlobalResult::BeamRst_y.EIeffbs  = EIeffbs1;
		GlobalResult::BeamRst_y.EIeff    = EIeff_x;
		//截面绕y轴的特性
		b_bc1 = (rr-secth)*0.5;
		m_edge = (rr - m_Sect.hs2)*0.5;
		Ic1 = b_bc1*rr*rr*rr/12;
		Isr1 = (b_bc1-m_edge)*m_Sect.bs2*m_Sect.d2x*m_Sect.d2x;
		Isr1 = Isr1*2;
		Isr2 = m_Sect.bs1*m_Sect.hs1*m_Sect.hs1*m_Sect.hs1/12;
		Ic = Ic1-Isr1-Isr2;
		EIeffbc1_y = Esr*(Isr1+Isr2)+C1*Ecm*Ic;
		//截面bs
		Ic1 = secth*rr*rr*rr/12;
		Is = m_Sect.Is_y;
		Isr1 = m_Sect.bs2*secth*m_Sect.d2x*m_Sect.d2x;
		Ic = Ic1 - Is - Isr1*2;
		EIeffbs1_y = Es*Is+Esr*Isr1*2+C1*Ecm*Ic;
		//y向总的EI
		EIeff_y = EIeffbc1_y*2+EIeffbs1_y;
		//保存一下截面特性
		GlobalResult::BeamRst_z.bc1 = b_bc1;
//		GlobalResult::BeamRst_z.bc2 = b_bc2;
		GlobalResult::BeamRst_z.bs  = secth;
		GlobalResult::BeamRst_z.EIeffbc1 = EIeffbc1_y;
//		GlobalResult::BeamRst_z.EIeffbc2 = EIeffbc2_y;
		GlobalResult::BeamRst_z.EIeffbs  = EIeffbs1_y;
		GlobalResult::BeamRst_z.EIeff    = EIeff_y;
		//计算截面的剪力
		Vbs_x = V_x*EIeffbs1/EIeff_x;
		Vbs_y = V_y*EIeffbs1_y/EIeff_y;
	}
	else if(SectType==3){
		//绕x轴特性，分为bc1,bs,bc2三个区间
		//bc1
		float b_bc1 = (b-dx*2-sectb)*0.5;
		float m_edge = (b - m_Sect.hs1)*0.5;
		double Ic1 = b_bc1*h*h*h/12;
		double Isr1 = (b_bc1-m_edge)*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		Isr1 = Isr1*2;
		double Isr2 = m_Sect.bs2*m_Sect.hs2*m_Sect.hs2*m_Sect.hs2/12;
		double Ic = Ic1-Isr1-Isr2;
		EIeffbc1 = Esr*(Isr1+Isr2)+C1*Ecm*Ic;
		//bs
		Ic1 = sectb*h*h*h/12;
		double Is = m_Sect.Is_x+m_Sect.bb*m_Sect.hh*dy*dy;
		Isr1 = m_Sect.bs1*sectb*m_Sect.d1y*m_Sect.d1y;
		Ic = Ic1 - Is*2 - Isr1*2;
		EIeffbs1 = Es*Is*2+Esr*Isr1*2+C1*Ecm*Ic;
		//bc2
		float b_bc2 = dx*2-sectb;
		Ic1 = b_bc2*h*h*h/12;
		Isr1 = b_bc2*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		Ic = Ic1 - 2*Isr1;
		EIeffbc2 = Esr*Isr1*2+C1*Ecm*Ic;
		//总的EI
		EIeff_x = EIeffbc1*2+EIeffbs1*2+EIeffbc2;

		//保存一下截面特性
		GlobalResult::BeamRst_y.bc1 = b_bc1;
		GlobalResult::BeamRst_y.bc2 = b_bc2;
		GlobalResult::BeamRst_y.bs  = sectb;
		GlobalResult::BeamRst_y.EIeffbc1 = EIeffbc1;
		GlobalResult::BeamRst_y.EIeffbc2 = EIeffbc2;
		GlobalResult::BeamRst_y.EIeffbs  = EIeffbs1;
		GlobalResult::BeamRst_y.EIeff    = EIeff_x;

		///绕y轴截面特性
		b_bc1 = (h-dy*2-secth)*0.5;
		m_edge = (h - m_Sect.hs2)*0.5;
		Ic1 = b_bc1*b*b*b/12;
		Isr1 = (b_bc1-m_edge)*m_Sect.bs2*m_Sect.d2x*m_Sect.d2x;
		Isr1 = Isr1*2;
		Isr2 = m_Sect.bs1*m_Sect.hs1*m_Sect.hs1*m_Sect.hs1/12;
		Ic = Ic1-Isr1-Isr2;
		EIeffbc1_y = Esr*(Isr1+Isr2)+C1*Ecm*Ic;
		//bs
		Ic1 = secth*b*b*b/12;
		Is = m_Sect.Is_y+m_Sect.bb*m_Sect.hh*dx*dx;
		Isr1 = m_Sect.bs2*secth*m_Sect.d2x*m_Sect.d2x;
		Ic = Ic1 - Is*2 - Isr1*2;
		EIeffbs1_y = Es*Is*2+Esr*Isr1*2+C1*Ecm*Ic;
		//bc2
		b_bc2 = dy*2-secth;
		Ic1 = b_bc2*b*b*b/12;
		Isr1 = b_bc2*m_Sect.bs2*m_Sect.d2x*m_Sect.d2x;
		Ic = Ic1 - 2*Isr1;
		EIeffbc2_y = Esr*Isr1*2+C1*Ecm*Ic;
		//总的EI
		EIeff_y = EIeffbc1_y*2+EIeffbs1_y*2+EIeffbc2_y;
		//保存一下截面特性
		GlobalResult::BeamRst_z.bc1 = b_bc1;
		GlobalResult::BeamRst_z.bc2 = b_bc2;
		GlobalResult::BeamRst_z.bs  = secth;
		GlobalResult::BeamRst_z.EIeffbc1 = EIeffbc1_y;
		GlobalResult::BeamRst_z.EIeffbc2 = EIeffbc2_y;
		GlobalResult::BeamRst_z.EIeffbs  = EIeffbs1_y;
		GlobalResult::BeamRst_z.EIeff    = EIeff_y;

		//计算截面剪力
		Vbs_x = V_x*EIeffbs1/EIeff_x;
		Vbs_y = V_y*EIeffbs1_y/EIeff_y;
	}
	else if(SectType==4){
		//绕x轴特性，分为bc1,bs1,bc2,bs2四个区间
		//bc1 不考虑上下的钢板
		float b_bc1 = (b-dy*2-secth)*0.5;
//		float m_edge = (b - m_Sect.hs1)*0.5;
		double Ic1 = b_bc1*h*h*h/12;
		double Isr2 = m_Sect.bs2*m_Sect.hs2*m_Sect.hs2*m_Sect.hs2/12;
		double Ic = Ic1 - Isr2;
		EIeffbc1 = Esr*Isr2+C1*Ecm*Ic;
		//bs1
		Ic1 = secth*h*h*h/12;
		float Isr1 = secth*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		float Is = m_Sect.Is_y;
		Ic = Ic1 - 2*Isr1 - Is;
		EIeffbs1 = Es*Is+Esr*Isr1*2+C1*Ecm*Ic;
		//bc2
		float b_bc2 = dy*2 - sectb - secth;
		Ic1 = b_bc2*h*h*h/12;
		Isr1 = b_bc2*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		Ic = Ic1 - 2*Isr1;
		EIeffbc2 = Esr*Isr1*2+C1*Ecm*Ic;
		//bs2
		Ic1 = sectb*h*h*h/12;
		Isr1 = sectb*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		Is = m_Sect.Is_x+m_Sect.bb*m_Sect.hh*dy*dy;
		Ic = Ic1 - 2*Isr1 - 2*Is;
		EIeffbs2 = Es*Is*2+Esr*Isr1*2+C1*Ecm*Ic;
		//总的EI
		EIeff_x = EIeffbc1*2+EIeffbc2*2+EIeffbs1*2+EIeffbs2;

		//保存一下截面特性
		GlobalResult::BeamRst_y.bc1 = b_bc1;
		GlobalResult::BeamRst_y.bc2 = b_bc2;
		GlobalResult::BeamRst_y.bs  = sectb;
		GlobalResult::BeamRst_y.EIeffbc1 = EIeffbc1;
		GlobalResult::BeamRst_y.EIeffbc2 = EIeffbc2;
		GlobalResult::BeamRst_y.EIeffbs  = EIeffbs1;
		GlobalResult::BeamRst_y.EIeff    = EIeff_x;

		//计算截面剪力
		Vbs_x = V_x*EIeffbs1/EIeff_x;
		Vbs_y = V_y*EIeffbs1_y/EIeff_y;
		Vbs2_x = V_x*EIeffbs2/EIeff_x;
		Vbs2_y = V_y*EIeffbs2_y/EIeff_y;
		//这种截面应该做成正方形，这样两个方向的EI是相同的
	}
	else if(SectType==5){
		float rr = sqrt(PI*b*b*0.25);	//按面积直接等效方形
		//绕x轴特性，分为bc1,bs1,bc2,bs2四个区间
		//bc1 不考虑上下的钢板
		float b_bc1 = (rr-dy*2-secth)*0.5;
		//		float m_edge = (b - m_Sect.hs1)*0.5;
		double Ic1 = b_bc1*rr*rr*rr/12;
		double Isr2 = m_Sect.bs2*m_Sect.hs2*m_Sect.hs2*m_Sect.hs2/12;
		double Ic = Ic1 - Isr2;
		EIeffbc1 = Esr*Isr2+C1*Ecm*Ic;
		//bs1
		Ic1 = secth*rr*rr*rr/12;
		float Isr1 = secth*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		float Is = m_Sect.Is_y;
		Ic = Ic1 - 2*Isr1 - Is;
		EIeffbs1 = Es*Is+Esr*Isr1*2+C1*Ecm*Ic;
		//bc2
		float b_bc2 = dy*2 - sectb - secth;
		Ic1 = b_bc2*rr*rr*rr/12;
		Isr1 = b_bc2*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		Ic = Ic1 - 2*Isr1;
		EIeffbc2 = Esr*Isr1*2+C1*Ecm*Ic;
		//bs2
		Ic1 = sectb*rr*rr*rr/12;
		Isr1 = sectb*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		Is = m_Sect.Is_x+m_Sect.bb*m_Sect.hh*dy*dy;
		Ic = Ic1 - 2*Isr1 - 2*Is;
		EIeffbs2 = Es*Is*2+Esr*Isr1*2+C1*Ecm*Ic;
		//总的EI
		EIeff_x = EIeffbc1*2+EIeffbc2*2+EIeffbs1*2+EIeffbs2;

		//保存一下截面特性
		GlobalResult::BeamRst_y.bc1 = b_bc1;
		GlobalResult::BeamRst_y.bc2 = b_bc2;
		GlobalResult::BeamRst_y.bs  = sectb;
		GlobalResult::BeamRst_y.EIeffbc1 = EIeffbc1;
		GlobalResult::BeamRst_y.EIeffbc2 = EIeffbc2;
		GlobalResult::BeamRst_y.EIeffbs  = EIeffbs1;
		GlobalResult::BeamRst_y.EIeff    = EIeff_x;
		//计算截面剪力
		Vbs_x = V_x*EIeffbs1/EIeff_x;
		Vbs_y = V_y*EIeffbs1_y/EIeff_y;
		Vbs2_x = V_x*EIeffbs2/EIeff_x;
		Vbs2_y = V_y*EIeffbs2_y/EIeff_y;
		
		GlobalResult::BeamRst_y.Vedbs = Vbs_x*0.01;
		GlobalResult::BeamRst_z.Vedbs = Vbs_y*0.01;
	}
	else if(SectType==6){
		//绕x轴特性，分为bc1,bs,bc2三个区间
		//bc1
		float b_bc1 = (b-dx*2-sectb)*0.5;
		float m_edge = (b - m_Sect.hs1)*0.5;
		double Ic1 = b_bc1*h*h*h/12;
		double Isr1 = (b_bc1-m_edge)*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		Isr1 = Isr1*2;
		double Isr2 = m_Sect.bs2*m_Sect.hs2*m_Sect.hs2*m_Sect.hs2/12;
		double Ic = Ic1-Isr1-Isr2;
		EIeffbc1 = Esr*(Isr1+Isr2)+C1*Ecm*Ic;
		//bs
		Ic1 = sectb*h*h*h/12;
		double Is = m_Sect.Is_x+m_Sect.bb*m_Sect.hh*dy*dy;
		Isr1 = m_Sect.bs1*sectb*m_Sect.d1y*m_Sect.d1y;
		Ic = Ic1 - Is*2 - Isr1*2 - m_Sect.Is_x;
		EIeffbs1 = Es*(Is*2+m_Sect.Is_x)+Esr*Isr1*2+C1*Ecm*Ic;
		//bc2
		float b_bc2 = dx*2-sectb;
		Ic1 = b_bc2*h*h*h/12;
		Isr1 = b_bc2*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		Ic = Ic1 - 2*Isr1;
		EIeffbc2 = Esr*Isr1*2+C1*Ecm*Ic;
		//总的EI
		EIeff_x = EIeffbc1*2+EIeffbs1*2+EIeffbc2;
		//保存一下截面特性
		GlobalResult::BeamRst_y.bc1 = b_bc1;
		GlobalResult::BeamRst_y.bc2 = b_bc2;
		GlobalResult::BeamRst_y.bs  = sectb;
		GlobalResult::BeamRst_y.EIeffbc1 = EIeffbc1;
		GlobalResult::BeamRst_y.EIeffbc2 = EIeffbc2;
		GlobalResult::BeamRst_y.EIeffbs  = EIeffbs1;
		GlobalResult::BeamRst_y.EIeff    = EIeff_x;
		///y方向
		//bc1
		b_bc1 = (h-dy*2-secth)*0.5;
		m_edge = (h - m_Sect.hs2)*0.5;
		Ic1 = b_bc1*b*b*b/12;
		Isr2 = m_Sect.bs1*m_Sect.hs1*m_Sect.hs1*m_Sect.hs1/12;
		Ic = Ic1-Isr2;
		EIeffbc1_y = Esr*(Isr1+Isr2)+C1*Ecm*Ic;
		//bs1
		Ic1 = secth*b*b*b/12;
		Is = m_Sect.Is_y+m_Sect.bb*m_Sect.hh*dx*dx;
		Isr1 = m_Sect.bs2*secth*m_Sect.d2x*m_Sect.d2x;
		Ic = Ic1 - Is*2 - Isr1*2;
		EIeffbs1_y = Es*Is*2+Esr*Isr1*2+C1*Ecm*Ic;
		//bs2 = bs1
		EIeffbs2_y = EIeffbs1_y;
		//bc2
		b_bc2 = dy*2-secth*2;
		Ic1 = b_bc2*b*b*b/12;
		Isr1 = b_bc2*m_Sect.bs2*m_Sect.d2x*m_Sect.d2x;
		Ic = Ic1 - 2*Isr1;
		EIeffbc2_y = Esr*Isr1*2+C1*Ecm*Ic;
		//总的EI y
		EIeff_y = 2*EIeffbc1_y+2*EIeffbs1_y+2*EIeffbc2_y+EIeffbs2_y;
		//保存一下截面特性
		GlobalResult::BeamRst_z.bc1 = b_bc1;
		GlobalResult::BeamRst_z.bc2 = b_bc2;
		GlobalResult::BeamRst_z.bs  = secth;
		GlobalResult::BeamRst_z.EIeffbc1 = EIeffbc1_y;
		GlobalResult::BeamRst_z.EIeffbc2 = EIeffbc2_y;
		GlobalResult::BeamRst_z.EIeffbs  = EIeffbs1_y;
		GlobalResult::BeamRst_z.EIeff    = EIeff_y;
		//计算截面剪力
		Vbs_x = V_x*EIeffbs1/EIeff_x;
		Vbs_y = V_y*EIeffbs1_y/EIeff_y;
	}
	else if(SectType==7){
		float b_bc1 = (b-dx*2-sectb)*0.5;
		float m_edge = (b - m_Sect.hs1)*0.5;
		double Ic1 = b_bc1*h*h*h/12;
		double Isr1 = (b_bc1-m_edge)*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		Isr1 = Isr1*2;
		double Isr2 = m_Sect.bs2*m_Sect.hs2*m_Sect.hs2*m_Sect.hs2/12;
		double Ic = Ic1-Isr1-Isr2;
		EIeffbc1 = Esr*(Isr1+Isr2)+C1*Ecm*Ic;
		//bs
		Ic1 = sectb*h*h*h/12;
		double Is = m_Sect.Is_x+m_Sect.bb*m_Sect.hh*dy*dy;
		Isr1 = m_Sect.bs1*sectb*m_Sect.d1y*m_Sect.d1y;
		Ic = Ic1 - Is*2 - Isr1*2 - m_Sect.Is_x;
		EIeffbs1 = Es*(Is*2+m_Sect.Is_x)+Esr*Isr1*2+C1*Ecm*Ic;
		//bc2
		float b_bc2 = dx*2-sectb;
		Ic1 = b_bc2*h*h*h/12;
		Isr1 = b_bc2*m_Sect.bs1*m_Sect.d1y*m_Sect.d1y;
		Ic = Ic1 - 2*Isr1;
		EIeffbc2 = Esr*Isr1*2+C1*Ecm*Ic;
		//总的EI
		EIeff_x = EIeffbc1*2+EIeffbs1*2+EIeffbc2;
		//保存一下截面特性
		GlobalResult::BeamRst_y.bc1 = b_bc1;
		GlobalResult::BeamRst_y.bc2 = b_bc2;
		GlobalResult::BeamRst_y.bs  = sectb;
		GlobalResult::BeamRst_y.EIeffbc1 = EIeffbc1;
		GlobalResult::BeamRst_y.EIeffbc2 = EIeffbc2;
		GlobalResult::BeamRst_y.EIeffbs  = EIeffbs1;
		GlobalResult::BeamRst_y.EIeff    = EIeff_x;
		///y方向
		//bc1
		b_bc1 = (h-dy*2-secth)*0.5;
		m_edge = (h - m_Sect.hs2)*0.5;
		Ic1 = b_bc1*b*b*b/12;
		Isr2 = m_Sect.bs1*m_Sect.hs1*m_Sect.hs1*m_Sect.hs1/12;
		Ic = Ic1-Isr2;
		EIeffbc1_y = Esr*(Isr1+Isr2)+C1*Ecm*Ic;
		//bs1
		Ic1 = secth*b*b*b/12;
		Is = m_Sect.Is_y+m_Sect.bb*m_Sect.hh*dx*dx;
		Isr1 = m_Sect.bs2*secth*m_Sect.d2x*m_Sect.d2x;
		Ic = Ic1 - Is*2 - Isr1*2;
		EIeffbs1_y = Es*Is*2+Esr*Isr1*2+C1*Ecm*Ic;
		//bs2 = bs1
		EIeffbs2_y = EIeffbs1_y;
		//bc2
		b_bc2 = dy*2-secth*2;
		Ic1 = b_bc2*b*b*b/12;
		Isr1 = b_bc2*m_Sect.bs2*m_Sect.d2x*m_Sect.d2x;
		Ic = Ic1 - 2*Isr1;
		EIeffbc2_y = Esr*Isr1*2+C1*Ecm*Ic;
		//总的EI y
		EIeff_y = 2*EIeffbc1_y+2*EIeffbs1_y+2*EIeffbc2_y+EIeffbs2_y;
		//保存一下截面特性
		GlobalResult::BeamRst_z.bc1 = b_bc1;
		GlobalResult::BeamRst_z.bc2 = b_bc2;
		GlobalResult::BeamRst_z.bs  = secth;
		GlobalResult::BeamRst_z.EIeffbc1 = EIeffbc1_y;
		GlobalResult::BeamRst_z.EIeffbc2 = EIeffbc2_y;
		GlobalResult::BeamRst_z.EIeffbs  = EIeffbs1_y;
		GlobalResult::BeamRst_z.EIeff    = EIeff_y;
		//计算截面剪力
		Vbs_x = V_x*EIeffbs1/EIeff_x;
		Vbs_y = V_y*EIeffbs1_y/EIeff_y;
		                                                        
	}

	//计算完成后进行惯性矩的转换，等效到混凝土
	Ieffcbs_x = EIeffbs1/Ecm;
	Ieffcbs2_x = EIeffbs2/Ecm;
	Ieffcbs_y = EIeffbs1_y/Ecm;
	Ieffcbs2_y = EIeffbs2_y/Ecm;
	//保存数据
	GlobalResult::BeamRst_y.Vedbs = Vbs_x;
	GlobalResult::BeamRst_y.Ic = Ieffcbs_x;
	GlobalResult::BeamRst_z.Vedbs = Vbs_y;
	GlobalResult::BeamRst_z.Ic = Ieffcbs_y;


}

void InterfaceCheck::CalInterface()
{

	//梁方法计算时，会出现2种情况
	//1、bs区有1~3个0度布置的工字形钢
	//2、bs区有1~2个90度布置的工字钢
	int SectType;	//截面类型，根据文档的截面
	float Hsect_b,Hsect_h;
	float Hsect_x,Hsect_y;
	float tw,tf;
	float h,b;
	float Es,Em;
	SectType = GPara::m_Sect_in->m_kind;
	Hsect_b = GPara::m_Sect_in->HSect.B1;
	Hsect_h = GPara::m_Sect_in->HSect.H1;
	Hsect_x = GPara::m_Sect_in->HSect_x;
	Hsect_y = GPara::m_Sect_in->HSect_y;
	tw = GPara::m_Sect_in->HSect.RJ;
	tf = GPara::m_Sect_in->HSect.RI;
	h = GPara::m_Sect_in->Sect_h;
	b = GPara::m_Sect_in->Sect_b;
	Es = Tools::GetProE(GPara::m_DesnPara->CodeType);
	int ConLevel = GPara::m_Sect_in->Concrete_mat;
	Em = Tools::GetConEcm(ConLevel);

	float ks = Es/Em;
	float Ag = Hsect_h*Hsect_b-(Hsect_b-tw)*(Hsect_h-2*tf);
// 	SectProperty m_Sect;
// 	m_Sect.SectGeoInfo();
// 	float Ag = m_Sect.Ag;

	float bb = b*ks;
	float tww = tw*ks;
	float tff = tf*ks;
	float hh = h*ks;

	//获得栓钉的抗剪承载力
	Vcc1_x=Vcc2_x=Vcc3_x=Vcc4_x=0;
	Vcc1_y=Vcc2_y=Vcc3_y=Vcc4_y=0;
	float Vstud = GlobalResult::Vstud;
	if(SectType==1){		//矩形单型钢
		//等效b和等效tw
		float A1 = (h-Hsect_h)*0.5*Hsect_b;
		float S1 = ((h-Hsect_h)*0.25+Hsect_h*0.5)*A1;
		Vcc1_x = Vbs_x*S1/Ieffcbs_x;
		//y_side
		A1 = (b-Hsect_b)*0.5*Hsect_h;
		S1 = ((b-Hsect_b)*0.25+Hsect_b*0.5)*A1;
		float A21 = tff*(Hsect_b-tw)*0.5*2;	//钢等效部分
		float A22 = (Hsect_h-2*tf)*(Hsect_b-tw)*0.5;	//混凝土部分
		float S21 = Hsect_b*0.25*A21;
		float S22 = ((Hsect_b-tw)*0.25+tw*0.5)*A22;
		float S2 = S1+S21+S22;
		Vcc1_y = Vbs_y*S2/Ieffcbs_y;	//腹板一侧,另一侧对称

		//保存中间数据
		GlobalResult::BeamRst_y.Scc1 = S1;
//		GlobalResult::BeamRst_y.Scc2 = S2;
		GlobalResult::BeamRst_y.VedCC1 = Vcc1_x;
//		GlobalResult::BeamRst_y.VedCC2 = Vcc2_x;
		GlobalResult::BeamRst_z.Scc1 = S2;
//		GlobalResult::BeamRst_z.Scc2 = S4;
		GlobalResult::BeamRst_z.VedCC1 = Vcc1_y;
//		GlobalResult::BeamRst_z.VedCC2 = Vcc2_y;
	}
	else if(SectType==2){	//圆形单型钢，等效为正方形
		float rr = sqrt(PI*b*b*0.25);
		float A1 = (rr-Hsect_h)*0.5*Hsect_b;
		float S1 = ((rr-Hsect_h)*0.25+Hsect_h*0.5)*A1;
		Vcc1_x = Vbs_x*S1/Ieffcbs_x;
		//y_side
		A1 = (rr-Hsect_b)*0.5*Hsect_h;
		S1 = ((rr-Hsect_b)*0.25+Hsect_b*0.5)*A1;
		float A21 = tff*(Hsect_b-tw)*0.5*2;	//钢等效部分
		float A22 = (Hsect_h-2*tf)*(Hsect_b-tw)*0.5;	//混凝土部分
		float S21 = Hsect_b*0.25*A21;
		float S22 = ((Hsect_b-tw)*0.25+tw*0.5)*A22;
		float S2 = S1+S21+S22;
		Vcc1_y = Vbs_y*S2/Ieffcbs_y;	//腹板一侧,另一侧对称
		//保存中间数据
		GlobalResult::BeamRst_y.Scc1 = S1;
//		GlobalResult::BeamRst_y.Scc2 = S2;
		GlobalResult::BeamRst_y.VedCC1 = Vcc1_x;
//		GlobalResult::BeamRst_y.VedCC2 = Vcc2_x;
		GlobalResult::BeamRst_z.Scc1 = S2;
//		GlobalResult::BeamRst_z.Scc2 = S4;
		GlobalResult::BeamRst_z.VedCC1 = Vcc1_y;
//		GlobalResult::BeamRst_z.VedCC2 = Vcc2_y;
// 		float CiR = h*0.5;
// 		float ArcH = CiR - sqrt(CiR*CiR-Hsect_b*Hsect_b*0.25);
// 		float A11 = CiR*CiR*asin(Hsect_b/(2*CiR))-sqrt(CiR*CiR-Hsect_b*Hsect_b*0.25)*Hsect_b*0.5;		//这里将弧形面积等效为矩形面积，与弧形面积同高
// 		float S11 = A11*(CiR-ArcH*0.5);
// 		float A12 = (b - Hsect_h - ArcH*2)*0.5*Hsect_b;
// 		float S12 = A12*((b - Hsect_h - ArcH*2)*0.25+Hsect_h*0.5);
// 		float S1 = S11+S12;
// 		Vcc1_x = Vbs_x*S1/Ieffcbs_x;
// 		//y_side
// 		float ArcH_y = CiR-sqrt(CiR*CiR-Hsect_h*Hsect_h*0.25);
// 		float A21 = CiR*CiR*asin(Hsect_h/(2*CiR))-sqrt(CiR*CiR-Hsect_h*Hsect_h*0.25)*Hsect_h*0.5;
// 		float S21 = A21*(CiR-ArcH_y*0.5);
// 		float A22 = (h - Hsect_b - ArcH_y*2)*0.5*Hsect_h;
// 		float S22 = A22*((h - Hsect_b - ArcH*2)*0.25+Hsect_b*0.5);
// 		float A23 = 2*tff*(Hsect_b-tw)*0.5;
// 		float S23 = A23*((Hsect_b-tw)*0.25+tw*0.5);	//翼缘等效
// 		float A24 = (Hsect_b-tw)*0.5*(Hsect_h-2*tf);
// 		float S24 = A24*((Hsect_b-tw)*0.25+tw*0.5);		//翼缘间混凝土
// 		float S2 = S21+S22+S23+S24;
// 		Vcc1_y = Vbs_y*S2/Ieffcbs_y;
	}
	else if(SectType==3){	//矩形阵列，四个型钢
		float A1 = (h-Hsect_y*2-Hsect_h)*0.5*Hsect_b;
		float S1 = ((h-Hsect_y*2-Hsect_h)*0.25+Hsect_h*0.5+Hsect_y)*A1;
		Vcc1_x = Vbs_x*S1/Ieffcbs_x;
		float A21 = Ag*ks; //型钢等效混凝土面积
		float S21 = Hsect_y*A21;
		float A22 = (Hsect_b-tw)*(Hsect_h-2*tf); //翼缘间的混凝土
		float S22 = Hsect_y*A22;
		float S2 = S1+S21+S22;
		Vcc2_x = Vbs_x*S2/Ieffcbs_x;

		//y_side
		float A31 = (b-Hsect_x*2-Hsect_b)*0.5*h;	//外侧混凝土
		float A32 = 2*tff*(Hsect_b-tw)*0.5;		//翼缘等效面积
		float A33 = (Hsect_h-2*tf)*(Hsect_b-tw)*0.5;	//翼缘间混凝土
		float S31 = ((b-Hsect_x*2-Hsect_b)*0.25+Hsect_b*0.5+Hsect_x)*A31;
		float S32 = (Hsect_b*0.25+Hsect_x)*A32;
		float S33 = ((Hsect_b-tw)*0.25+tw*0.5+Hsect_x)*A33;
		float S3 = S31+S32+S33;
		Vcc1_y = Vbs_y*S3/Ieffcbs_y;	//第一个型钢腹板左侧
		float S4 = S3+hh*tw*Hsect_x;
		Vcc2_y = Vbs_y*S4/Ieffcbs_y;	//第一个型钢腹板右侧

		//保存中间数据
		GlobalResult::BeamRst_y.Scc1 = S1;
		GlobalResult::BeamRst_y.Scc2 = S2;
		GlobalResult::BeamRst_y.VedCC1 = Vcc1_x;
		GlobalResult::BeamRst_y.VedCC2 = Vcc2_x;
		GlobalResult::BeamRst_z.Scc1 = S3;
		GlobalResult::BeamRst_z.Scc2 = S4;
		GlobalResult::BeamRst_z.VedCC1 = Vcc1_y;
		GlobalResult::BeamRst_z.VedCC2 = Vcc2_y;
	}
	else if(SectType==4){	//矩形旋转，四个型钢
		//x_side-1 bs区
		float A1 = (h-Hsect_y*2-Hsect_h)*0.5*Hsect_b;
		float S1 = ((h-Hsect_y*2-Hsect_h)*0.25+Hsect_h*0.5+Hsect_y)*A1;
		Vcc1_x = Vbs_x*S1/Ieffcbs_x;
		//x_side-2
		float A21 = Ag*ks; //型钢等效混凝土面积
		float S21 = Hsect_y*A21;
		float A22 = (Hsect_b-tw)*(Hsect_h-2*tf); //翼缘间的混凝土
		float S22 = Hsect_y*A22;
		float S2 = S1+S21+S22;
		Vcc2_x = Vbs_x*S2/Ieffcbs_x;
		//x_side-bs'	//单个弱轴，腹板
		float A31 = (h-Hsect_b)*0.5*Hsect_h;
		float S31 = ((h-Hsect_b)*0.25+Hsect_b*0.5)*A31;
		float A32 = 2*tff*(Hsect_b-tw)*0.5;
		float A33 = (Hsect_h-2*tf)*(Hsect_b-tw)*0.5;
		float S32 = ((Hsect_b-tw)*0.25+tw*0.5)*A32;
		float S33 = ((Hsect_b-tw)*0.25+tw*0.5)*A33;
		float S3 = S31+S32+S33;
		Vcc3_x = Vbs2_x*S3/Ieffcbs2_x;
		//x_side-bs'
		//腹板另一侧一样
		//y_side-1  bs区
		float A4 = (b-Hsect_y*2-Hsect_h)*0.5*Hsect_b;
		float S4 = ((b-Hsect_y*2-Hsect_h)*0.25+Hsect_h*0.5+Hsect_y)*A4;
		Vcc1_y = Vbs_y*S4/Ieffcbs_y;
		//y_side-2	
		float A51 = Ag*ks; //型钢等效混凝土面积
		float S51 = Hsect_y*A51;
		float A52 = (Hsect_b-tw)*(Hsect_h-2*tf); //翼缘间的混凝土
		float S52 = Hsect_y*A52;
		float S5 = S4+S51+S52;
		Vcc2_y = Vbs_y*S5/Ieffcbs_y;
		//y_side-bs'
		float A61 = (b-Hsect_b)*0.5*Hsect_h;
		float S61 = ((b-Hsect_b)*0.25+Hsect_b*0.5)*A31;
		float A62 = 2*tff*(Hsect_b-tw)*0.5;
		float A63 = (Hsect_h-2*tf)*(Hsect_b-tw)*0.5;
		float S62 = ((Hsect_b-tw)*0.25+tw*0.5)*A32;
		float S63 = ((Hsect_b-tw)*0.25+tw*0.5)*A33;
		float S6 = S61+S62+S63;
		Vcc3_y = Vbs2_y*S6/Ieffcbs2_y;
		//保存中间数据
		GlobalResult::BeamRst_y.Scc1 = S1;
		GlobalResult::BeamRst_y.Scc2 = S2;
		GlobalResult::BeamRst_y.VedCC1 = Vcc1_x;
		GlobalResult::BeamRst_y.VedCC2 = Vcc2_x;
		GlobalResult::BeamRst_z.Scc1 = S3;
		GlobalResult::BeamRst_z.Scc2 = S4;
		GlobalResult::BeamRst_z.VedCC1 = Vcc1_y;
		GlobalResult::BeamRst_z.VedCC2 = Vcc2_y;
	}
	else if(SectType==5){	//圆形旋转，四个型钢
		//等效成正方型,计算同类型4
		float rr = sqrt(PI*b*b*0.25);
		//x_side-1 bs区
		float A1 = (rr-Hsect_y*2-Hsect_h)*0.5*Hsect_b;
		float S1 = ((rr-Hsect_y*2-Hsect_h)*0.25+Hsect_h*0.5+Hsect_y)*A1;
		Vcc1_x = Vbs_x*S1/Ieffcbs_x;
		//x_side-2
		float A21 = Ag*ks; //型钢等效混凝土面积
		float S21 = Hsect_y*A21;
		float A22 = (Hsect_b-tw)*(Hsect_h-2*tf); //翼缘间的混凝土
		float S22 = Hsect_y*A22;
		float S2 = S1+S21+S22;
		Vcc2_x = Vbs_x*S2/Ieffcbs_x;
		//x_side-bs'	//单个弱轴，腹板
		float A31 = (rr-Hsect_b)*0.5*Hsect_h;
		float S31 = ((rr-Hsect_b)*0.25+Hsect_b*0.5)*A31;
		float A32 = 2*tff*(Hsect_b-tw)*0.5;
		float A33 = (Hsect_h-2*tf)*(Hsect_b-tw)*0.5;
		float S32 = ((Hsect_b-tw)*0.25+tw*0.5)*A32;
		float S33 = ((Hsect_b-tw)*0.25+tw*0.5)*A33;
		float S3 = S31+S32+S33;
		Vcc3_x = Vbs2_x*S3/Ieffcbs2_x;
		//x_side-bs'
		//腹板另一侧一样
		//y_side-1  bs区
		float A4 = (rr-Hsect_y*2-Hsect_h)*0.5*Hsect_b;
		float S4 = ((rr-Hsect_y*2-Hsect_h)*0.25+Hsect_h*0.5+Hsect_y)*A4;
		Vcc1_y = Vbs_y*S4/Ieffcbs_y;
		//y_side-2	
		float A51 = Ag*ks; //型钢等效混凝土面积
		float S51 = Hsect_y*A51;
		float A52 = (Hsect_b-tw)*(Hsect_h-2*tf); //翼缘间的混凝土
		float S52 = Hsect_y*A52;
		float S5 = S4+S51+S52;
		Vcc2_y = Vbs_y*S5/Ieffcbs_y;
		//y_side-bs'
		float A61 = (rr-Hsect_b)*0.5*Hsect_h;
		float S61 = ((rr-Hsect_b)*0.25+Hsect_b*0.5)*A31;
		float A62 = 2*tff*(Hsect_b-tw)*0.5;
		float A63 = (Hsect_h-2*tf)*(Hsect_b-tw)*0.5;
		float S62 = ((Hsect_b-tw)*0.25+tw*0.5)*A32;
		float S63 = ((Hsect_b-tw)*0.25+tw*0.5)*A33;
		float S6 = S61+S62+S63;
		Vcc3_y = Vbs2_y*S6/Ieffcbs2_y;
		//保存中间数据
		GlobalResult::BeamRst_y.Scc1 = S1;
		GlobalResult::BeamRst_y.Scc2 = S2;
		GlobalResult::BeamRst_y.Scc3 = S3;
		GlobalResult::BeamRst_y.VedCC1 = Vcc1_x;
		GlobalResult::BeamRst_y.VedCC2 = Vcc2_x;
		GlobalResult::BeamRst_y.VedCC3 = Vcc3_x;

		GlobalResult::BeamRst_z.Scc1 = S4;
		GlobalResult::BeamRst_z.Scc2 = S5;
		GlobalResult::BeamRst_z.Scc3 = S6;
		GlobalResult::BeamRst_z.VedCC1 = Vcc1_y;
		GlobalResult::BeamRst_z.VedCC2 = Vcc2_y;
		GlobalResult::BeamRst_z.VedCC3 = Vcc3_y;
	}
	else if(SectType==6){	//矩形阵列，六个型钢
		//x_side bs
		float A1 = (h-Hsect_y*2-Hsect_h)*0.5*Hsect_b;
		float S1 = ((h-Hsect_y*2-Hsect_h)*0.25+Hsect_h*0.5+Hsect_y)*A1;
		Vcc1_x = Vbs_x*S1/Ieffcbs_x;
		float A21 = Ag*ks;
		float A22 = (Hsect_b-tw)*(Hsect_h-2*tf);
		float S21 = Hsect_y*A21;
		float S22 = Hsect_y*A22;
		float S2 = S1+S21+S22;
		Vcc2_x = Vbs_x*S2/Ieffcbs_x;
		float A3 = (Hsect_y-Hsect_h)*Hsect_b;
		float S3 = S2+((Hsect_y-Hsect_h)*0.5+Hsect_h*0.5)*A3;
		Vcc3_x = Vbs_x*S3/Ieffcbs_x;
		//y_side bss
		float A41 = (b-Hsect_x*2-Hsect_b)*0.5*h;	//外侧混凝土
		float A42 = 2*tff*(Hsect_b-tw)*0.5;		//翼缘等效面积
		float A43 = (Hsect_h-2*tf)*(Hsect_b-tw)*0.5;	//翼缘间混凝土
		float S41 = ((b-Hsect_x*2-Hsect_b)*0.25+Hsect_b*0.5+Hsect_x)*A41;
		float S42 = (Hsect_b*0.25+Hsect_x)*A42;
		float S43 = ((Hsect_b-tw)*0.25+tw*0.5+Hsect_x)*A43;
		float S4 = S41+S42+S43;
		Vcc1_y = Vbs_y*S4/Ieffcbs_y;	//第一个型钢腹板左侧
		float S5 = S4+hh*tw*Hsect_x;
		Vcc2_y = Vbs_y*S5/Ieffcbs_y;	//第一个型钢腹板右侧
		//保存中间数据
		GlobalResult::BeamRst_y.Scc1 = S1;
		GlobalResult::BeamRst_y.Scc2 = S2;
		GlobalResult::BeamRst_y.VedCC1 = Vcc1_x;
		GlobalResult::BeamRst_y.VedCC2 = Vcc2_x;
		GlobalResult::BeamRst_z.Scc1 = S3;
		GlobalResult::BeamRst_z.Scc2 = S4;
		GlobalResult::BeamRst_z.VedCC1 = Vcc1_y;
		GlobalResult::BeamRst_z.VedCC2 = Vcc2_y;
	}
	else if(SectType==7){	//同6类

	}
	 

	//注意一下，都是转化为KN/M来算的
	Ncc1_x = int(Vcc1_x/Vstud+1);
	GlobalResult::Vcc1_y = Vcc1_x;
	GlobalResult::Ncc1_y = Ncc1_x;
	GlobalResult::BeamRst_y.nstud_cc1 = Ncc1_x;
	if(Vcc2_x>0){
		Ncc2_x = int(Vcc2_x/Vstud+1);
		GlobalResult::Vcc2_y = Vcc2_x;
		GlobalResult::Ncc2_y = Ncc2_x;
		GlobalResult::BeamRst_y.nstud_cc2 = Ncc2_x;
	}
	if(Vcc3_x>0){
		Ncc3_x = int(Vcc3_x/Vstud+1);
		GlobalResult::Vcc3_y = Vcc3_x;
		GlobalResult::Ncc3_y = Ncc3_x;
		GlobalResult::BeamRst_y.nstud_cc3 = Ncc3_x;
	}
	if(Vcc4_x>0){
		Ncc4_x = int(Vcc4_x/Vstud+1);
		GlobalResult::Vcc4_y = Vcc4_x;
		GlobalResult::Ncc4_y = Ncc4_x;
	}
	//弱轴
	Ncc1_y = int(Vcc1_y/Vstud+1);
	GlobalResult::Vcc1_z = Vcc1_y;
	GlobalResult::Ncc1_z = Ncc1_y;
	GlobalResult::BeamRst_z.nstud_cc1 = Ncc1_y;
	if(Vcc2_y>0){
		Ncc2_y = int(Vcc2_y/Vstud+1);
		GlobalResult::Vcc2_z = Vcc2_y;
		GlobalResult::Ncc2_z = Ncc2_y;
		GlobalResult::BeamRst_z.nstud_cc2 = Ncc2_y;
	}
	if(Vcc3_y>0){
		Ncc3_y = int(Vcc3_y/Vstud+1);
		GlobalResult::Vcc3_z = Vcc3_y;
		GlobalResult::Ncc3_z = Ncc3_y;
		GlobalResult::BeamRst_z.nstud_cc3 = Ncc3_y;
	}
	if(Vcc4_y>0){
		Ncc4_y = int(Vcc4_y/Vstud+1);
		GlobalResult::Vcc4_z = Vcc4_y;
		GlobalResult::Ncc4_z = Ncc4_y;
	}
}
bool InterfaceCheck::TrussModel(float &r_y,float &r_z,int i_count)
{
	int secttype = GPara::m_Sect_in->m_kind;
	double Srcbc;
	double Srcbs;
	double Sspbs;
	double Vcbs;
	double Vcbc;	//混凝土部分的剪力
	double Vcbc_r;
	double Vrdbond;
	double Ved;
	float Asw;
	float rou = GPara::m_Sect_in->Transversal_ratio/100.f;		//配箍率
	float rou_x = r_y;//GPara::m_Sect_in->Tran_ratio_x;
	float rou_y = r_z;//GPara::m_Sect_in->Tran_ratio_y;
	float mvratio_x = GPara::m_Sect_in->MV_ratio_x;
	float mvratio_y = GPara::m_Sect_in->MV_ratio_y;
	int cover_x = GPara::m_Sect_in->Con_coverd_x;
	int cover_y = GPara::m_Sect_in->Con_coverd_y;
	float Es,Ec;
	Es = Tools::GetProE(GPara::m_DesnPara->CodeType);
	Ec = Tools::GetConEcm(GPara::m_Sect_in->Concrete_mat);
	float Ge;
	Ge = Tools::GetProG(GPara::m_DesnPara->CodeType);
	float nn=1.f;
	float b = GPara::m_Sect_in->Sect_b;
	float h = GPara::m_Sect_in->Sect_h;
	float dx = GPara::m_Sect_in->HSect_x;
	float dy = GPara::m_Sect_in->HSect_y;
	float Hsect_h = GPara::m_Sect_in->HSect.H1;
	float Hsect_b = GPara::m_Sect_in->HSect.B1;
	float Tf = GPara::m_Sect_in->HSect.RI;
	float Tw = GPara::m_Sect_in->HSect.RJ;
	float z_y = 2*GPara::m_Sect_in->HSect_y;
	float z_z = 2*GPara::m_Sect_in->HSect_x;
	float Cprem = Hsect_h*2+Hsect_b*4-2*Tw-4*Tf;
	float bs,bc,Cz,Av;
	float Li;
	float beta;
	float Czmin = 40;
	float Trd = 0.3;
	float interval = 1000;	//GPara::m_Sect_in->Bar_interval;		//这个值应该是箍筋的间距
	float Vstud = GlobalResult::Vstud;
/*	if(secttype==1){
		//强轴方向
		Ved = fabs(GPara::Vy_max);
		bc = (b-Hsect_b)*0.5;
		bs = Hsect_b;
		Cz = bc;
		Asw = bc*interval*rou_x;
		Srcbc = 1/(interval/(Es*Asw)+2/(nn*Ec*bc));	//外侧混凝土区
		Asw = bs*interval*rou_x;
		Li = 0;
		nn = Getn(1,bs,Ec,Es,0.9*h,Li);
		Srcbs = 1/(interval/(Es*Asw)+2/(nn*Ec*bs));
		Av = (Hsect_h-2*Tf)*Tw;
		Sspbs = (Ge*Av)/(0.9*b);		//z = 0.9h
		Vcbs = Ved*Srcbs/(Srcbc+Sspbs+Srcbs);
		Vcbc = Ved*Srcbc/(Srcbc+Sspbs+Srcbs);
		//计算混凝土部分的承载力
		Vcbc_r = CalVRd(bc,h,rou_x,mvratio_x,cover_x);
		beta = 1+0.02*Cz*(1-40/Cz);
		if(beta>2.5)
			beta = 2.5;
		Vrdbond = Cprem*Trd*0.9*h*beta;
		//栓钉数量
		int nstud = int(Vcbs*0.5/(Vstud*0.9*h)+1);
		//记录
//		GlobalResult::Truss_Vcbs_y[i_count] = Vcbs;
//		GlobalResult::Truss_Vrd_y[i_count] = Vrdbond;
//		GlobalResult::Truss_Vcbc_y[i_count] = Vcbc;
		TrussModelRst temp;
		temp.rou = r_y*100;
		temp.bc = bc;
		temp.bs = bs;
		temp.nn = nn;
		temp.Ge = Ge;
		temp.Av = Av;
		temp.Zz = 0.9*b;
		temp.Vcbc = Vcbc;
		temp.Vcbs = Vcbs;
		temp.Vrdbond = Vrdbond;
		temp.Srcbc = Srcbc;
		temp.Srcbs = Srcbs;
		temp.Sspbs = Sspbs;
		temp.Vcbc_r = Vcbc_r;
		temp.Nstud = nstud;
		if(Vcbc_r>Vcbc)
			temp.ifVcOk = true;
		else
			temp.ifVcOk = false;
		GlobalResult::Truss_y.Add(temp);
		
		//弱轴方向
		nn=1.f;
		Ved = fabs(GPara::Vz_max);
		bc = (h-Hsect_h)*0.5;
		bs = Hsect_h;
		Cz = bc;
		Asw = bc*interval*rou_x;
		Srcbc = 1/(interval/(Es*Asw)+2/(nn*Ec*bc));	//外侧混凝土区
		Asw = bs*interval*rou_x;
		Li = 0;
		nn = Getn(2,bs,Ec,Es,0.9*b,Li);
		Srcbs = 1/(interval/(Es*Asw)+2/(nn*Ec*bs));
		Av = (Hsect_h-2*Tf)*Tw;
		Sspbs = (Ge*Av)/(0.9*h);	
		Vcbs = Ved*Srcbs/(Srcbc+Sspbs+Srcbs);
		Vcbc = Ved*Srcbc/(Srcbc+Sspbs+Srcbs);
		//计算纯混凝土承载力
		Vcbc_r = CalVRd(bc,b,rou_y,mvratio_y,cover_y);
		beta = 1+0.02*Cz*(1-40/Cz);
		if(beta>2.5)
			beta = 2.5;
		Vrdbond = (b-Cz*2)*Trd*0.9*b*beta;
		//栓钉数量
		nstud = int(Vcbs*0.5/(Vstud*0.9*h)+1);
		//记录
// 		GlobalResult::Truss_Vcbs_z[i_count] = Vcbs;
// 		GlobalResult::Truss_Vrd_z[i_count] = Vrdbond;
// 		GlobalResult::Truss_Vcbc_z[i_count] = Vcbc;
// 		GlobalResult::Truss_Vcr_z[i_count] = Vcbc_r;
// 		GlobalResult::Truss_Nstud_z[i_count] = nstud;
		temp.rou = r_z*100;
		temp.bc = bc;
		temp.bs = bs;
		temp.nn = nn;
		temp.Ge = Ge;
		temp.Av = Av;
		temp.Zz = 0.9*h;
		temp.Vcbc = Vcbc;
		temp.Vcbs = Vcbs;
		temp.Vrdbond = Vrdbond;
		temp.Srcbc = Srcbc;
		temp.Srcbs = Srcbs;
		temp.Sspbs = Sspbs;
		temp.Vcbc_r = Vcbc_r;
		temp.Nstud = nstud;
		if(Vcbc_r>Vcbc)
			temp.ifVcOk = true;
		else
			temp.ifVcOk = false;
		GlobalResult::Truss_z.Add(temp);
	}*/
	if(secttype==3){
		//强轴方向
		Ved = fabs(GPara::Vy_max);
		bs = b-2*dx;
		bc = b-2*bs;
		Cz = (h-2*dy - Hsect_h)*0.5;
		if(bc<0)
			return true;		//bc太小就没法算了
		Asw = bc*interval*rou_x;
		Srcbc = 1/(interval/(Es*Asw)+2/(nn*Ec*bc));
		Asw = bs*interval*rou_x;
		Srcbs = 1/(interval/(Es*Asw)+2/(nn*Ec*bs));
		Av = (Hsect_h-2*Tf)*Tw;
		Sspbs = (2*Ge*Av)/(2*dy);
		Vcbs = Ved*Srcbs/(Srcbc+2*(Sspbs+Srcbs));
		Vcbc = Ved*Srcbc/(Srcbc+2*(Sspbs+Srcbs));
		//纯混凝土部分
		Vcbc_r = CalVRd(bc,h,rou_x,mvratio_x,cover_x);
		beta = 1+0.02*Cz*(1-40/Cz);
		if(beta>2.5)
			beta = 2.5;
		Vrdbond = Cprem*Trd*2*dy*beta;
		//栓钉数量
		int nstud = int(Vcbs*0.5/(Vstud*z_y)+1);		//现在没考虑Vrdbond
		//记录
		TrussModelRst temp;
		temp.rou = r_y*100;
		temp.bc = bc;
		temp.bs = bs;
		temp.nn = nn;
		temp.Ge = Ge;
		temp.Av = Av;
		temp.Zz = 0.9*b;
		temp.Vcbc = Vcbc;
		temp.Vcbs = Vcbs;
		temp.Vrdbond = Vrdbond;
		temp.Srcbc = Srcbc;
		temp.Srcbs = Srcbs;
		temp.Sspbs = Sspbs;
		temp.Vcbc_r = Vcbc_r;
		temp.Nstud = nstud;
		if(Vcbc_r>Vcbc)
			temp.ifVcOk = true;
		else
			temp.ifVcOk = false;
		GlobalResult::Truss_y.Add(temp);
// 		GlobalResult::Truss_Vcbs_y[i_count] = Vcbs;
// 		GlobalResult::Truss_Vrd_y[i_count] = Vrdbond;
// 		GlobalResult::Truss_Vcbc_y[i_count] = Vcbc;
// 		GlobalResult::Truss_Vcr_y[i_count] = Vcbc_r;
// 		GlobalResult::Truss_Nstud_y[i_count] = nstud;
		//弱轴方向
		Ved = fabs(GPara::Vz_max);
		bs = h - 2*dy;
		bc = h - 2*bs;
		Cz = (b-2*dx-Hsect_b)*0.5;
		Asw = bc*interval*rou_y;
		Srcbc = 1/(interval/(Es*Asw)+2/(nn*Ec*bc));
		Asw = bs*interval*rou_y;
		Srcbs = 1/(interval/(Es*Asw)+2/(nn*Ec*bs));
		Av = Hsect_b*Tf*2;
		Sspbs = (2*Ge*Av)/(2*dx);
		Vcbs = Ved*Srcbs/(Srcbc+2*(Sspbs+Srcbs));
		Vcbc = Ved*Srcbc/(Srcbc+2*(Sspbs+Srcbs));
		Vcbc_r = CalVRd(bc,b,rou_y,mvratio_y,cover_y);
		beta = 1+0.02*Cz*(1-40/Cz);
		if(beta>2.5)
			beta = 2.5;
		Vrdbond = Cprem*Trd*2*dx*beta;		
		//栓钉数量
		nstud = int(Vcbs*0.5/(Vstud*z_z)+1);
		//记录
// 		GlobalResult::Truss_Vcbs_z[i_count] = Vcbs;
// 		GlobalResult::Truss_Vrd_z[i_count] = Vrdbond;
// 		GlobalResult::Truss_Vcbc_z[i_count] = Vcbc;
// 		GlobalResult::Truss_Vcr_z[i_count] = Vcbc_r;
// 		GlobalResult::Truss_Nstud_z[i_count] = nstud;
		temp.rou = r_z*100;
		temp.bc = bc;
		temp.bs = bs;
		temp.nn = nn;
		temp.Ge = Ge;
		temp.Av = Av;
		temp.Zz = 0.9*h;
		temp.Vcbc = Vcbc;
		temp.Vcbs = Vcbs;
		temp.Vrdbond = Vrdbond;
		temp.Srcbc = Srcbc;
		temp.Srcbs = Srcbs;
		temp.Sspbs = Sspbs;
		temp.Vcbc_r = Vcbc_r;
		temp.Nstud = nstud;
		if(Vcbc_r>Vcbc)
			temp.ifVcOk = true;
		else
			temp.ifVcOk = false;
		GlobalResult::Truss_z.Add(temp);
	}
	else if(secttype==4){		//矩形十字排布
		Ved = max(GPara::Vy_max,GPara::Vz_max);
		Ved = fabs(Ved);
		bs = (h-2*dy)*0.5+Hsect_h+(dy-Hsect_h*0.5-Hsect_b*0.5)*0.5;		//考虑型钢之间中分
		bc = h-2*bs;
		Cz = (h-2*dy-Hsect_h)*0.5;
		Asw = bc*interval*rou_x;
		Srcbc = 1/(interval/(Es*Asw)+2/(nn*Ec*bc));
		Asw = bs*interval*rou_x;
		Srcbs = 1/(interval/(Es*Asw)+2/(nn*Ec*bs));
		Av = (Hsect_h-2*Tf)*Tw;
		Sspbs = (2*Ge*Av)/(2*dy);
		Vcbs = Ved*Srcbs/(Srcbc+2*(Sspbs+Srcbs));
		Vcbc = Ved*Srcbc/(Srcbc+2*(Sspbs+Srcbs));
		Vcbc_r = CalVRd(bc,h,rou_x,mvratio_x,cover_x);
		beta = 1+0.02*Cz*(1-40/Cz);
		if(beta>2.5)
			beta = 2.5;
		Vrdbond = Cprem*Trd*2*dy*beta;
		//栓钉数量
		int nstud = int(Vcbs*0.5/(Vstud*z_y)+1);
		//记录
// 		GlobalResult::Truss_Vcbs_y[i_count] = Vcbs;
// 		GlobalResult::Truss_Vrd_y[i_count] = Vrdbond;
// 		GlobalResult::Truss_Vcbc_y[i_count] = Vcbc;
// 		GlobalResult::Truss_Vcr_y[i_count] = Vcbc_r;
// 		GlobalResult::Truss_Nstud_y[i_count] = nstud;
		TrussModelRst temp;
		temp.rou = r_y*100;
		temp.bc = bc;
		temp.bs = bs;
		temp.nn = nn;
		temp.Ge = Ge;
		temp.Av = Av;
		temp.Zz = 0.9*b;
		temp.Vcbc = Vcbc;
		temp.Vcbs = Vcbs;
		temp.Vrdbond = Vrdbond;
		temp.Srcbc = Srcbc;
		temp.Srcbs = Srcbs;
		temp.Sspbs = Sspbs;
		temp.Vcbc_r = Vcbc_r;
		temp.Nstud = nstud;
		if(Vcbc_r>Vcbc)
			temp.ifVcOk = true;
		else
			temp.ifVcOk = false;
		GlobalResult::Truss_y.Add(temp);
		GlobalResult::Truss_z.Add(temp);
		//弱轴同强轴
	}
	else if(secttype==6){	//6个型钢
		//强轴方向
		Ved = GPara::Vy_max;
		Ved = fabs(Ved);
		bs = b-2*dx;
		bc = b-2*bs;
		Cz = (h-2*dy - Hsect_h)*0.5;
		if(bc<0)
			return true;		//bc太小就没法算了
		Asw = bc*interval*rou_x;
		Srcbc = 1/(interval/(Es*Asw)+2/(nn*Ec*bc));
		Asw = bs*interval*rou_x;
		Li = 2*dy - 2*Hsect_h;
		nn = Getn(1,bs,Ec,Es,2*dy,Li);
		Srcbs = 1/(interval/(Es*Asw)+2/(nn*Ec*bs));
		Av = (Hsect_h-2*Tf)*Tw;
		Sspbs = (3*Ge*Av)/(2*dy);
		Vcbs = Ved*Srcbs/(Srcbc+2*(Sspbs+Srcbs));
		Vcbc = Ved*Srcbc/(Srcbc+2*(Sspbs+Srcbs));
		Vcbc_r = CalVRd(bc,h,rou_x,mvratio_x,cover_x);
		beta = 1+0.02*Cz*(1-40/Cz);
		if(beta>2.5)
			beta = 2.5;
		Vrdbond = Cprem*Trd*2*dy*beta;
		//栓钉数量
		int nstud = int(Vcbs*0.5/(Vstud*z_y)+1);
		//记录
// 		GlobalResult::Truss_Vcbs_y[i_count] = Vcbs;
// 		GlobalResult::Truss_Vrd_y[i_count] = Vrdbond;
// 		GlobalResult::Truss_Vcbc_y[i_count] = Vcbc;
// 		GlobalResult::Truss_Vcr_y[i_count] = Vcbc_r;
// 		GlobalResult::Truss_Nstud_y[i_count] = nstud;
		TrussModelRst temp;
		temp.rou = r_y*100;
		temp.bc = bc;
		temp.bs = bs;
		temp.nn = nn;
		temp.Ge = Ge;
		temp.Av = Av;
		temp.Zz = 0.9*b;
		temp.Vcbc = Vcbc;
		temp.Vcbs = Vcbs;
		temp.Vrdbond = Vrdbond;
		temp.Srcbc = Srcbc;
		temp.Srcbs = Srcbs;
		temp.Sspbs = Sspbs;
		temp.Vcbc_r = Vcbc_r;
		temp.Nstud = nstud;
		if(Vcbc_r>Vcbc)
			temp.ifVcOk = true;
		else
			temp.ifVcOk = false;
		GlobalResult::Truss_y.Add(temp);
		//弱轴方向,两种划分都带型钢
//		nn = 1.f;
		Ved = GPara::Vz_max;
		Ved = fabs(Ved);
		bs = h - 2*dy;
		bc = (h - 3*bs)*0.5;
		Cz = (b-2*dx-Hsect_b)*0.5;
		nn = Getn(2,bs,Ec,Es,z_z,dy-Hsect_h);
		Asw = bc*interval*rou_y;
		Srcbc = 1/(interval/(Es*Asw)+2/(nn*Ec*bc));
		Asw = bs*interval*rou_y;
		Srcbs = 1/(interval/(Es*Asw)+2/(nn*Ec*bs));
		Av = Hsect_b*Tf*2;
		Sspbs = (2*Ge*Av)/(2*dx);
		Vcbs = Ved*Srcbs/(2*Srcbc+3*(Sspbs+Srcbs));
		Vcbc = Ved*Srcbc/(2*Srcbc+3*(Sspbs+Srcbs));
		Vcbc_r = CalVRd(bc,b,rou_y,mvratio_y,cover_y);
		beta = 1+0.02*Cz*(1-40/Cz);
		if(beta>2.5)
			beta = 2.5;
		Vrdbond = Cprem*Trd*2*dx*beta;		
		//栓钉数量
		nstud = int(Vcbs*0.5/(Vstud*z_z)+1);
		//记录
// 		GlobalResult::Truss_Vcbs_z[i_count] = Vcbs;
// 		GlobalResult::Truss_Vrd_z[i_count] = Vrdbond;
// 		GlobalResult::Truss_Vcbc_z[i_count] = Vcbc;
// 		GlobalResult::Truss_Vcr_z[i_count] = Vcbc_r;
// 		GlobalResult::Truss_Nstud_z[i_count] = nstud;
		temp.rou = r_z*100;
		temp.bc = bc;
		temp.bs = bs;
		temp.nn = nn;
		temp.Ge = Ge;
		temp.Av = Av;
		temp.Zz = 0.9*h;
		temp.Vcbc = Vcbc;
		temp.Vcbs = Vcbs;
		temp.Vrdbond = Vrdbond;
		temp.Srcbc = Srcbc;
		temp.Srcbs = Srcbs;
		temp.Sspbs = Sspbs;
		temp.Vcbc_r = Vcbc_r;
		temp.Nstud = nstud;
		if(Vcbc_r>Vcbc)
			temp.ifVcOk = true;
		else
			temp.ifVcOk = false;
		GlobalResult::Truss_z.Add(temp);
	}
	else if(secttype==7){	//6个型钢，带转角
		//强轴方向
		Ved = GPara::Vy_max;
		Ved = fabs(Ved);
		bs = b-2*dx;
		bc = b-2*bs;
		Cz = (h-2*dy - Hsect_h)*0.5;
		if(bc<0)
			return true;		//bc太小就没法算了
		nn = Getn(1,bs,Ec,Es,z_y,dx-Hsect_b);
		Asw = bc*interval*rou_x;
		Srcbc = 1/(interval/(Es*Asw)+2/(nn*Ec*bc));
		Asw = bs*interval*rou_x;
		Li = 2*dy - 2*Hsect_b;
		nn = Getn(1,bs,Ec,Es,2*dy,Li);
		Srcbs = 1/(interval/(Es*Asw)+2/(nn*Ec*bs));
		Av = Hsect_b*Tf*2;
		Sspbs = (3*Ge*Av)/(2*dy);
		Vcbs = Ved*Srcbs/(Srcbc+2*(Sspbs+Srcbs));
		Vcbc = Ved*Srcbc/(Srcbc+2*(Sspbs+Srcbs));
		Vcbc_r = CalVRd(bc,h,rou_x,mvratio_x,cover_x);
		beta = 1+0.02*Cz*(1-40/Cz);
		if(beta>2.5)
			beta = 2.5;
		Vrdbond = Cprem*Trd*2*dy*beta;
		//栓钉数量
		int nstud = int(Vcbs*0.5/(Vstud*z_y)+1);
		//记录
// 		GlobalResult::Truss_Vcbs_y[i_count] = Vcbs;
// 		GlobalResult::Truss_Vrd_y[i_count] = Vrdbond;
// 		GlobalResult::Truss_Vcbc_y[i_count] = Vcbc;
// 		GlobalResult::Truss_Vcr_y[i_count] = Vcbc_r;
// 		GlobalResult::Truss_Nstud_y[i_count] = nstud;
		TrussModelRst temp;
		temp.rou = r_y*100;
		temp.bc = bc;
		temp.bs = bs;
		temp.nn = nn;
		temp.Ge = Ge;
		temp.Av = Av;
		temp.Zz = 0.9*b;
		temp.Vcbc = Vcbc;
		temp.Vcbs = Vcbs;
		temp.Vrdbond = Vrdbond;
		temp.Srcbc = Srcbc;
		temp.Srcbs = Srcbs;
		temp.Sspbs = Sspbs;
		temp.Vcbc_r = Vcbc_r;
		temp.Nstud = nstud;
		if(Vcbc_r>Vcbc)
			temp.ifVcOk = true;
		else
			temp.ifVcOk = false;
		GlobalResult::Truss_y.Add(temp);
		//弱轴方向,两种划分都带型钢
		nn = 1.f;
		Ved = GPara::Vz_max;
		Ved = fabs(Ved);
		bs = h - 2*dy;
		bc = (h - 3*bs)*0.5;
		Cz = (b-2*dx-Hsect_b)*0.5;
		Asw = bc*interval*rou_y;
		Srcbc = 1/(interval/(Es*Asw)+2/(nn*Ec*bc));
		Asw = bs*interval*rou_y;
		Srcbs = 1/(interval/(Es*Asw)+2/(nn*Ec*bs));
		Av = (Hsect_h-Tf*2)*Tw;
		Sspbs = (2*Ge*Av)/(2*dx);
		Vcbs = Ved*Srcbs/(2*Srcbc+3*(Sspbs+Srcbs));
		Vcbc = Ved*Srcbc/(2*Srcbc+3*(Sspbs+Srcbs));
		Vcbc_r = CalVRd(bc,b,rou_y,mvratio_y,cover_y);
		beta = 1+0.02*Cz*(1-40/Cz);
		if(beta>2.5)
			beta = 2.5;
		Vrdbond = Cprem*Trd*2*dx*beta;		
		//栓钉数量
		nstud = int(Vcbs*0.5/(Vstud*z_z)+1);
		//记录
// 		GlobalResult::Truss_Vcbs_z[i_count] = Vcbs;
// 		GlobalResult::Truss_Vrd_z[i_count] = Vrdbond;
// 		GlobalResult::Truss_Vcbc_z[i_count] = Vcbc;
// 		GlobalResult::Truss_Vcr_z[i_count] = Vcbc_r;
// 		GlobalResult::Truss_Nstud_z[i_count] = nstud;
		temp.rou = r_z*100;
		temp.bc = bc;
		temp.bs = bs;
		temp.nn = nn;
		temp.Ge = Ge;
		temp.Av = Av;
		temp.Zz = 0.9*h;
		temp.Vcbc = Vcbc;
		temp.Vcbs = Vcbs;
		temp.Vrdbond = Vrdbond;
		temp.Srcbc = Srcbc;
		temp.Srcbs = Srcbs;
		temp.Sspbs = Sspbs;
		temp.Vcbc_r = Vcbc_r;
		temp.Nstud = nstud;
		if(Vcbc_r>Vcbc)
			temp.ifVcOk = true;
		else
			temp.ifVcOk = false;
		GlobalResult::Truss_z.Add(temp);
	}

	return false;
}
// return in N
float InterfaceCheck::GetShearStud()
{
	float stud_d = GPara::m_DesnPara->stud_d;
	float stud_len = GPara::m_DesnPara->stud_len;
	float stud_fu = GPara::m_DesnPara->stud_fu;
	float stud_fy = GPara::m_DesnPara->stud_fy;
	float gamav = 1.25;
	int Fck = GPara::m_Sect_in->Concrete_mat;
	float Prk1 = 1,Prk2 =1,Prk=1;
	float Ecm;
	if(GPara::m_DesnPara->CodeType==1){	//欧洲规范
		float stud_alpha;
		float hscd = stud_len/stud_d;
		Ecm = Tools::GetConEcm_EURO(Fck);
		if(hscd<3){
			hscd = 3;
			stud_alpha = 0.2*(hscd+1);
		}
		else if(hscd>=3 && hscd<=4){
			stud_alpha = 0.2*(hscd+1);
		}
		else{
			stud_alpha = 1.f;
		}

		Prk1 = 0.8*stud_fu*PI*stud_d*stud_d*0.25/gamav;
		Prk2 = 0.29*stud_alpha*stud_d*stud_d*sqrt(Ecm*Fck)/gamav;
		Prk = min(Prk1,Prk2);
	}
	else if(GPara::m_DesnPara->CodeType==2){	//美国规范
		float faiv = 0.65;
		float Asa = PI*stud_d*stud_d*0.25;
		float Fu = min(1.9*stud_fy,125000);
		Prk = faiv*Fu*Asa;
	}
	else if(GPara::m_DesnPara->CodeType==3){	//中国规范
		float faiv = 0.43;
		float Asa = PI*stud_d*stud_d*0.25;
		Ecm = 10000*Tools::GetConEcm_Chi(Fck);
		Prk1 = faiv*Asa*sqrt(Ecm*Fck);
		Prk2 = 0.7*Asa*stud_fu;
		Prk = min(Prk1,Prk2);
	}

	return Prk;
}

float InterfaceCheck::Getn(const int direction,float bw,float Ec,float Es,float z,float Li)
{
	//direction 1,H型受压  2,工型受压
	//Li已经为和值了
	int b = GPara::m_Sect_in->HSect.B1;
	int h = GPara::m_Sect_in->HSect.H1;
	float tf = GPara::m_Sect_in->HSect.RI;
	float tw = GPara::m_Sect_in->HSect.RJ;
	float Ka,Kc,Kcf;
	int Ecc = Tools::GetEcc(Ec);
	if(direction==1){
		Kc = Ec*(bw-b)*1.414/h;
		Ka = Es*1.414/(2*tf/b+(h-2*tf)/(tw+(b-tw)*Ecc/Es));
		Kcf = Ec*bw*1.414/h;
	}
	else {
		Kc = Ecc*(bw-h)*1.414/b;
		Ka = (2*tf*Es+(h-2*tf)*Ecc)*1.414/b;
		Kcf = Ec*bw*1.414/b;
	}
	float Eec= Ec*(Ka+Kc)/Kcf;
	float nn = Eec*(z-Li)+Ec*Li/(z*Ec);
	return 1;
}

//获得纯混凝土部分的抗剪承载力，用来和桁架模型混凝土部分的剪力进行对比
double InterfaceCheck::CalVRd(float &bw,float &h,float &ratio,float &MVratio,int &cover)
{
	double Vrmax;
	float Asw = bw*1000*ratio;
	float fywd = GPara::m_Sect_in->TransversalBar_mat;
	int Bar_s = GPara::m_Sect_in->Bar_interval;
	SectProperty m_Sect;
	m_Sect.SectGeoInfo();
	float fcd;
//	fcd = m_Sect.fc/1.5;
	if(GPara::m_DesnPara->CodeType==1){	//欧洲规范
		float v1;
		if(GPara::m_Sect_in->Concrete_mat<=60)
			v1 = 0.6;
		else
			v1 = 0.9-GPara::m_Sect_in->Concrete_mat/200;
		double Vr1 = bw*ratio*0.9*h*0.8*fywd;
		fcd = m_Sect.fc/1.5;
		double Vr2 = bw*0.9*h*v1*fcd/2;
		Vrmax = min(Vr1,Vr2);
	}
	else if(GPara::m_DesnPara->CodeType==2){		//美国规范		
		double Vc = 0.17*(1+GPara::NplRd/(14*m_Sect.Ag))*bw*h*sqrt(fcd);
		double Vs = Asw*fywd*h/Bar_s;
		Vrmax = Vc+Vs;	
	}
	else{	//中国规范
		float ftk,ft;
		Tools::GetConFtk_Chi(GPara::m_Sect_in->Concrete_mat,ftk);
		ft = ftk/1.4;
		Vrmax = 1.75*ft*bw*(h-cover)/(MVratio+1)+fywd*Asw*(h-cover)/Bar_s+0.07*GPara::N_max;
	}
	return Vrmax;
}