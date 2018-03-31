#if !defined(GPARA_H_)
#define  GPARA_H_
#ifdef _MEGASECTCAL
#define MEGA_EXCORE __declspec(dllexport)
#else
#define MEGA_EXCORE __declspec(dllimport)
#endif
#define  Combine 13
#define  LoadCase 4
#define  LoadCombine 30
#define  MaxCombine 30
#define  Parts 13
#define  FirbePoints 300
//#define NM_Points_Num 110

#pragma once
#include "DesignParameter.h"
#include "PredefSectDescrib.h"

struct Points_info{
	CArray <float> *pos_x;
	CArray <float> *pos_y;
	CStringArray *pos_info;
	Points_info(){
		pos_x = new CArray<float>;
		pos_y = new CArray<float>;
		pos_info = new CStringArray;
	}
	~Points_info(){
		/*if(pos_x->GetSize()>0)
			pos_x->RemoveAll();
		if(pos_y->GetSize()>0)
			pos_y->RemoveAll();
		delete pos_x;
		delete pos_y;
		if(pos_info->GetSize()>0)
			pos_info->RemoveAll();
		delete pos_info;*/
	}
};

class GPara
{
public:
	static DesnPara *m_DesnPara;
	static SectDescribe *m_Sect_in;
	static double NplRd;
	static double Pn_y;
	static double Pn_z;
	static double VplRd_y;
	static double VplRd_z;
	static double MyplNRd;
	static double MzplNRd;
//ŷ�޹淶����ЧӦ����
	static double NplRk;
	static float ks_y;
	static float ks_z;
	static float kk_y[LoadCombine];		//�Բ�ͬ�������Ҫ��¼��طŴ�ϵ��
	static float kk_z[LoadCombine];
	static float beta_y[LoadCombine];
	static float beta_z[LoadCombine];
	static float  Slender;
	static double EIin[6];		//��άģ��ʱ����Ҫ�õ���EI
	//����tips��ʾ����ɢ��
	static CArray <Points_info,Points_info> Pt_info;
public:		//����
	//����
	static double My_combine[LoadCombine];
	static double Mz_combine[LoadCombine];
	static double N_combine[LoadCombine];
	static double Vy_combine[LoadCombine];
	static double Vz_combine[LoadCombine];
	//��׼
	static double My_combine_bz[LoadCombine];
	static double Mz_combine_bz[LoadCombine];
	static double N_combine_bz[LoadCombine];
	static double Vy_combine_bz[LoadCombine];
	static double Vz_combine_bz[LoadCombine];
	//���
	static double Vy_max;
	static double Vz_max;
	static double My_max;
	static double Mz_max;
	static double N_max;
	static int N_Ctrl_Comb;		//�������
	static int My_Ctrl_Comb;
	static int Mz_Ctrl_Comb;
	static int Vy_Ctrl_Comb;
	static int Vz_Ctrl_Comb;
public:		//���㿪��	
	static int Capacity_switch;			//����������
	static int PDM_switch;					
	static int Interface_beam_switch;		//��ģ��
	static int Interface_truss_switch;		//���ģ��
	static int Fiber_switch;
	static int ifExpertMode;
public:		//��Ҫ����ļ������
	static float hnx,hny;
	static float roux,rouy;		//����8�����ʱ����Ӱ��ϵ��
	static float Qs_Euro;
	static float Nsq;		//ŷ�޹淶�е���ѹ�� NGed/Ned
	static float Ke;
	static float M12;		//�й��淶���õģ�С���/�����
	//С���M,N����,һ����fk��,һ����f�ģ��ȿ�4��,ǿ��������1��2��������������3��4��
	static double M_Curve[4][FirbePoints];
	static double N_Curve[4][FirbePoints];

public:
	static void SetDesnContent();
	static void SetEI_Fiber(double EIin[]);
	GPara(void);
	virtual ~GPara(void);

private:
	void initPara(void);	//��ʼ������

public:
	int GetMegaColPoly(float xp[], float yp[]);
	int GetProfilePoly(float x0,float y0,float ang,float xp[],float yp[]);
};

#endif