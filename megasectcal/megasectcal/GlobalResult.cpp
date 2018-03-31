#include "StdAfx.h"
#include "GlobalResult.h"

int GlobalResult::con_u = 0;
float GlobalResult::con_h0 = 0;
float GlobalResult::fai0 = 0;

NMPoint_Draw GlobalResult::draw_point;
double GlobalResult::NplRd;
double GlobalResult::NpmRd;
double GlobalResult::MmaxRd_y;
double GlobalResult::MmaxRd_z;
double GlobalResult::MplRd_y;
double GlobalResult::MplRd_z;
float GlobalResult::hnx;
float GlobalResult::hny;

double GlobalResult::Eceff;
double GlobalResult::EIeff_y;
double GlobalResult::EIeff_z;
double GlobalResult::Ncr_y;
double GlobalResult::Ncr_z;
double GlobalResult::NplRk;
float GlobalResult::Lx_y;
float GlobalResult::Lx_z;
float GlobalResult::ll_y;
float GlobalResult::ll_z;

float GlobalResult::beta;
double GlobalResult::EIeff1_y;
double GlobalResult::EIeff1_z;
double GlobalResult::Ncreff_y;
double GlobalResult::Ncreff_z;

//抗剪计算
double GlobalResult::Ved_y;
double GlobalResult::Ved_z;
float  GlobalResult::Vstud;
double GlobalResult::Vcc1_y=0;
double GlobalResult::Vcc2_y=0;
double GlobalResult::Vcc3_y=0;
double GlobalResult::Vcc4_y=0;
double GlobalResult::Vcc1_z=0;
double GlobalResult::Vcc2_z=0;
double GlobalResult::Vcc3_z=0;
double GlobalResult::Vcc4_z=0;
int GlobalResult::Ncc1_y=0;
int GlobalResult::Ncc2_y=0;
int GlobalResult::Ncc3_y=0;
int GlobalResult::Ncc4_y=0;
int GlobalResult::Ncc1_z=0;
int GlobalResult::Ncc2_z=0;
int GlobalResult::Ncc3_z=0;
int GlobalResult::Ncc4_z=0;

double GlobalResult::Truss_Vcbs_y[10];
double GlobalResult::Truss_Vrd_y[10];
double GlobalResult::Truss_Vcbc_y[10];
double GlobalResult::Truss_Vcr_y[10];
double GlobalResult::Truss_Vcbs_z[10];
double GlobalResult::Truss_Vrd_z[10];
double GlobalResult::Truss_Vcbc_z[10];
double GlobalResult::Truss_Vcr_z[10];
int    GlobalResult::Truss_Nstud_y[10];
int    GlobalResult::Truss_Nstud_z[10];
float GlobalResult::Vrou_y[100][13];
float GlobalResult::Vrou_z[100][13];

BeamModelRst GlobalResult::BeamRst_y;
BeamModelRst GlobalResult::BeamRst_z;
CArray<TrussModelRst,TrussModelRst> GlobalResult::Truss_y; 
CArray<TrussModelRst,TrussModelRst> GlobalResult::Truss_z;
LoadCombPos GlobalResult::loadcomb;

//混凝土
CheckRst_ GlobalResult::Con_CoverCheck_y;
CheckRst_ GlobalResult::Con_CoverCheck_z;

CheckRst_ GlobalResult::Con_StrCheck;
CheckRst_ GlobalResult::Con_HBCheck;	//高宽比
//型钢
CheckRst_ GlobalResult::Pro_RatioCheck;	//最小含钢率
CheckRst_ GlobalResult::Pro_StrCheck;
//纵筋
CheckRst_ GlobalResult::Bar_RatioCheck;
CheckRst_ GlobalResult::Bar_SpaceCheck;		//间距检查
CheckRst_ GlobalResult::Bar_BarDCheck;		//到型心中心间距
//箍筋
CheckRst_ GlobalResult::Tie_SpaceCheck;	//净距
CheckRst_ GlobalResult::Tie_DiaCheck;
//裂缝
CheckRst_ GlobalResult::Crack_Check1;
CheckRst_ GlobalResult::Crack_Check2;
CrackInfo GlobalResult::Crack_Desn;
//截面评价
CheckRst_ GlobalResult::AixalForce_Check;
CheckRst_ GlobalResult::ShearForcevy_Check;
CheckRst_ GlobalResult::ShearForcevz_Check;
CheckRst_ GlobalResult::My_Check;
CheckRst_ GlobalResult::Mz_Check;
CheckRst_ GlobalResult::MNV_Check;
CheckRst_ GlobalResult::bMy_Check;
CheckRst_ GlobalResult::bMz_Check;
CheckRst_ GlobalResult::bMNV_Check;
CheckRst_ GlobalResult::BucklingNby_Check;
CheckRst_ GlobalResult::BucklingNbz_Check;
CheckRst_ GlobalResult::MN15y_Check;
CheckRst_ GlobalResult::MN15z_Check;

GlobalResult::GlobalResult(void)
{
}


GlobalResult::~GlobalResult(void)
{
}
