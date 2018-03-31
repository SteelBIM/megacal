#include "StdAfx.h"
#include "GPara.h"
// static GPara m_GPara;
// GPara *gGPara = &m_GPara;


DesnPara *GPara::m_DesnPara=NULL;
SectDescribe *GPara::m_Sect_in=NULL;
double GPara::NplRd = 0;
double GPara::VplRd_y = 0;
double GPara::VplRd_z = 0;
double GPara::MyplNRd = 0;
double GPara::MzplNRd = 0;
double GPara::NplRk = 0;
double GPara::Vy_max =0;
double GPara::Vz_max = 0;
double GPara::My_max = 0;
double GPara::Mz_max = 0;
double GPara::N_max = 0;
int GPara::N_Ctrl_Comb=-1;
int GPara::My_Ctrl_Comb=-1;
int GPara::Mz_Ctrl_Comb=-1;
int GPara::Vy_Ctrl_Comb=-1;
int GPara::Vz_Ctrl_Comb=-1;
//�����13�������е��������
double GPara::My_combine[LoadCombine];
double GPara::Mz_combine[LoadCombine];
double GPara::N_combine[LoadCombine];
double GPara::Vy_combine[LoadCombine];
double GPara::Vz_combine[LoadCombine];

double GPara::My_combine_bz[LoadCombine];
double GPara::Mz_combine_bz[LoadCombine];
double GPara::N_combine_bz[LoadCombine];
double GPara::Vy_combine_bz[LoadCombine];
double GPara::Vz_combine_bz[LoadCombine];

double GPara::M_Curve[4][FirbePoints];
double GPara::N_Curve[4][FirbePoints];

int GPara::Capacity_switch=1;			//����������
int GPara::PDM_switch=1;					
int GPara::Interface_beam_switch=1;		//��ģ��
int GPara::Interface_truss_switch=1;		//���ģ��
int GPara::Fiber_switch=1;
int GPara::ifExpertMode=0;

float GPara::hnx=0;
float GPara::hny=0;
float GPara::roux=1.f;
float GPara::rouy=1.f;
float GPara::Nsq = 1.f;
float GPara::Ke = 0.6;
float GPara::ks_y = 1.f;
float GPara::ks_z = 1.f;
float GPara::M12 = 1.f;
float GPara::kk_y[LoadCombine]={1.f};
float GPara::kk_z[LoadCombine]={1.f};
float GPara::beta_y[LoadCombine]={1.f};
float GPara::beta_z[LoadCombine]={1.f};
double GPara::Pn_y = 0.f;
double GPara::Pn_z = 0.f;
double GPara::EIin[6];
//�ѷ���
float GPara::Qs_Euro=0;
//tips
CArray <Points_info,Points_info> GPara::Pt_info;

GPara::GPara(void)
{
	initPara();
}


GPara::~GPara(void)
{
}

void GPara::initPara()
{

}

void GPara::SetEI_Fiber(double EIin[])
{
	for(int i=0;i<6;i++)
		GPara::EIin[i] = EIin[i];
}

//�趨��������
void GPara::SetDesnContent()
{
	//ŷ�޹淶
	if(m_DesnPara->CodeType==1){
		if(m_Sect_in->m_kind==3 || m_Sect_in->m_kind==4 || m_Sect_in->m_kind==6 || m_Sect_in->m_kind==7) //��Ҫ�����ģ��
			Interface_truss_switch = 1;
		else	//ֻ�ܿ�����ģ��
			Interface_truss_switch = 0;

		if(m_Sect_in->m_kind==2 || m_Sect_in->m_kind==5)
			Interface_beam_switch = 0;
		else
			Interface_beam_switch = 1;

		if(m_Sect_in->m_kind==2 || m_Sect_in->m_kind==5)	//ֻ�ܲ�����άģ��
			PDM_switch = 0;
		else
			PDM_switch = 1;
	}
	else if(m_DesnPara->CodeType==2){	//�����淶
		if(m_Sect_in->m_kind==3 || m_Sect_in->m_kind==4 || m_Sect_in->m_kind==6 || m_Sect_in->m_kind==7) //��Ҫ�����ģ��
			Interface_truss_switch = 1;
		else	//ֻ�ܿ�����ģ��
			Interface_truss_switch = 0;

		if(m_Sect_in->m_kind==2 || m_Sect_in->m_kind==5)	//ֻ�ܲ�����άģ��
			PDM_switch = 0;
		else
			PDM_switch = 1;
	}
	else if(m_DesnPara->CodeType==3){	//�й��淶
		if(m_Sect_in->m_kind==1)	//ֻ�е�һ�־��ν����й�ʽ
			Capacity_switch = 1;
		else 
			Capacity_switch = 0;

		PDM_switch = 0;		//������pdm��ʽ
	}
}
//          3,   2
//       0(4),   1
//  return  ����ζ�������ԲΪ1
int GPara::GetMegaColPoly(float xp[], float yp[])
{
	int n=0;
	if(m_Sect_in->m_kind==2||m_Sect_in->m_kind==5) 
	{
			n=1;
			xp[0]=0.5*m_Sect_in->Sect_b;
			yp[0]=0.5*m_Sect_in->Sect_b;
			xp[2]=0.5*m_Sect_in->Sect_b;
	}
	else 
	{
		n=4;
		xp[0]=xp[3]=xp[4]=-0.5*m_Sect_in->Sect_b;
		xp[1]=xp[2]=0.5*m_Sect_in->Sect_b;
		yp[0]=yp[1]=yp[4]=-0.5*m_Sect_in->Sect_h;
		yp[2]=yp[3]=0.5*m_Sect_in->Sect_h;

	}
	return n;
}
//
//                             7                                                6 
//                              8                       9    4                5
//                                                        |     |
//                                                        |     | 
//                               11                    10   3                2
//                                0    								          1
int GPara::GetProfilePoly(float x0,float y0,float ang,float xp[],float yp[])
{
	float ang1=ang+0.5*PI;
	int n=12;	
	float HSect_b = m_Sect_in->HSect.B1;
	float HSect_h = m_Sect_in->HSect.H1;
	float HSect_tw = m_Sect_in->HSect.RI;
	float HSect_tf = m_Sect_in->HSect.RJ;	
	
	xp[0] = xp[11] = xp[8] = xp[7] = -HSect_b*0.5;
	xp[1] = xp[2] = xp[5] = xp[6] = HSect_b*0.5;
	xp[10] = xp[9] = -HSect_tw*0.5;
	xp[3] = xp[4] = HSect_tw*0.5;

	yp[0] = yp[1] = -HSect_h*0.5;
	yp[2] = yp[3] = yp[10] = yp[11] =-HSect_h*0.5+HSect_tf;
	yp[4] = yp[5] = yp[8] = yp[9] = HSect_h*0.5-HSect_tf;	
	yp[6] = yp[7] = HSect_h*0.5;
	if(fabs(ang)>0.000001)
	{
		for(int i=0;i<n;i++ )
		{
			float xtmp=xp[i];
			float ytmp=yp[i];
			xp[i]=xtmp*cos(ang)+ytmp*sin(ang);
			yp[i]=xtmp*cos(ang1)+ytmp*sin(ang1);
		}
	}
	for(int i=0;i<n;i++)
	{
		xp[i]+=x0;
		yp[i]+=y0;
	}
	xp[n]=xp[0];
	yp[n]=yp[0];

	return n;
}
