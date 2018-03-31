#pragma once
#include "MegaResults.h"
//保存结果的静态成员
struct NMPoint_Draw{
	double My,Mz;
	double N;
};

struct LoadCombPos{
	int Nmax_comb,Nmax_pos;
	int Vymax_comb,Vymax_pos;
	int Vzmax_comb,Vzmax_pos;
	int Myz_comb,Myz_pos;
	int bMyz_comb,bMyz_pos;
	int Mymax_comb,Mymax_pos;
	int Mzmax_comb,Mzmax_pos;
};

struct BeamModelRst{
	float bc1,bc2;
	float bs;
	double EIeffbc1,EIeffbc2;
	double EIeffbs;
	double EIeff;
	double Vedbc1,Vedbc2;
	double Vedbs;
	double Ic;
	double Scc1,Scc2,Scc3;
	double VedCC1,VedCC2,VedCC3;
	int nstud_cc1,nstud_cc2,nstud_cc3;
};

struct TrussModelRst{
//	float s;
	float rou;
	float bc,bs;
	float nn;
	float Ge;
	float Av;
	float Zz;
	double Srcbc,Srcbs,Sspbs;
	double Vcbc,Vcbs;
	double Vcbc_r;	//混凝土抗剪承载力
	double Vrdbond;
	int Nstud;
	bool ifVcOk;
};

//给裂缝计算用的结构体
struct CrackInfo{
	double fctm;
	float ae;
	double hcef_y,hcef_z;
	double Aceff_y,Aceff_z;
	float peff_y,peff_z;
	double fe;
	float em_y,em_z;
	float srmax_y,srmax_z;
};

class GlobalResult
{
public:
	GlobalResult(void);
	~GlobalResult(void);

	//euro
		//load
		static LoadCombPos loadcomb;
		//creep
		static int con_u;
		static float con_h0;
		static float fai0;
		//NMcurve
		static NMPoint_Draw draw_point;
		static double NplRd;
		static double NpmRd;
		static double MmaxRd_y;
		static double MmaxRd_z;
		static double MplRd_y;
		static double MplRd_z;
		static float hnx;
		static float hny;
		//Buckling
		static double Eceff;
		static double EIeff_y;
		static double EIeff_z;
		static double Ncr_y;
		static double Ncr_z;
		static double NplRk;
		static float Lx_y;
		static float Lx_z;
		static float ll_y;
		static float ll_z;

		static float beta;
		static double EIeff1_y;
		static double EIeff1_z;
		static double Ncreff_y;
		static double Ncreff_z;

		//抗剪验算
		static BeamModelRst BeamRst_y;
		static BeamModelRst BeamRst_z;
		static CArray<TrussModelRst,TrussModelRst> Truss_y;
		static CArray<TrussModelRst,TrussModelRst> Truss_z;

		static double Ved_y;
		static double Ved_z;
		static float Vstud;
		static double Vcc1_y,Vcc2_y,Vcc3_y,Vcc4_y;
		static double Vcc1_z,Vcc2_z,Vcc3_z,Vcc4_z;
		static int Ncc1_y,Ncc2_y,Ncc3_y,Ncc4_y;
		static int Ncc1_z,Ncc2_z,Ncc3_z,Ncc4_z;

		static double Truss_Vcbs_y[10];
		static double Truss_Vrd_y[10];
		static double Truss_Vcbc_y[10];
		static double Truss_Vcr_y[10];
		static double Truss_Vcbs_z[10];
		static double Truss_Vrd_z[10];
		static double Truss_Vcbc_z[10];
		static double Truss_Vcr_z[10];
		static int    Truss_Nstud_y[10];
		static int    Truss_Nstud_z[10];
		static float Vrou_y[100][13];
		static float Vrou_z[100][13];

		//混凝土
		static CheckRst_ Con_CoverCheck_y;
		static CheckRst_ Con_CoverCheck_z;
		static CheckRst_ Con_StrCheck;
		static CheckRst_ Con_HBCheck;	//高宽比
		//型钢
		static CheckRst_ Pro_RatioCheck;	//最小含钢率
		static CheckRst_ Pro_StrCheck;
		//纵筋
		static CheckRst_ Bar_RatioCheck;
		static CheckRst_ Bar_SpaceCheck;		//间距检查
		static CheckRst_ Bar_BarDCheck;		//到型心中心间距
		//箍筋
		static CheckRst_ Tie_SpaceCheck;	//净距
		static CheckRst_ Tie_DiaCheck;
		//裂缝
		static CheckRst_ Crack_Check1;
		static CheckRst_ Crack_Check2;
		static CrackInfo Crack_Desn;

		//截面评价
		static CheckRst_ AixalForce_Check;
		static CheckRst_ ShearForcevy_Check;
		static CheckRst_ ShearForcevz_Check;
		static CheckRst_ My_Check;
		static CheckRst_ Mz_Check;
		static CheckRst_ MNV_Check;
		static CheckRst_ bMy_Check;
		static CheckRst_ bMz_Check;
		static CheckRst_ bMNV_Check;
		static CheckRst_ BucklingNby_Check;
		static CheckRst_ BucklingNbz_Check;

		static CheckRst_ MN15y_Check;
		static CheckRst_ MN15z_Check;
};

