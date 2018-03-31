#pragma once
#include <math.h>
#include "PredefSectDescrib.h"
//�����������������淶�����õ���������ܷ�
class InterfaceCheck
{
public:
	InterfaceCheck(void);
	~InterfaceCheck(void);

public:
	bool InterfaceDesn();

public:
	//SectDescribe m_SectData;
	static float stirruprate[10];
	float Vcc1_x,Vcc2_x,Vcc3_x,Vcc4_x;	//��Ӧ�Ĳ�ͬ����Ľ��浥λ��Ӧ��
	float Vcc1_y,Vcc2_y,Vcc3_y,Vcc4_y;
	int Ncc1_x,Ncc2_x,Ncc3_x,Ncc4_x;
	int Ncc1_y,Ncc2_y,Ncc3_y,Ncc4_y;
private:
	double Ieffcbs_x,Ieffcbs_y;		//bs����ĵ�Ч���Ծ�
	double Ieffcbs2_x,Ieffcbs2_y;  //bs'����ĵ�Ч���Ծ�
	float C1;		//�������ն��ۼ�ϵ��
	float Vbs_x,Vbs_y;		//bs�����ϵļ���,ʵ���Ǵ�ֱ�ڸö��巽��ļ���
	float Vbs2_x,Vbs2_y;		//bs'�����ϵļ���
	void CalInterface();	//��������ļ���
	void CalSepVbs();		//�����������ĵ�ЧEI��������V
	void InitCal();		//׼��һЩ�������
	bool TrussModel(float &r_y,float &r_z,int i_count = 0);	//���ģ�Ϳ��Ը�����ͬ�乿���µĳ�����
	float GetShearStud(void);
	float Getn(const int direction,float bw,float Ec,float Es,float z,float Li);		//�������ģ���е�n
	double CalVRd(float &b,float &h,float &ratio,float &MVratio,int &cover);		//���㴿���������ֵĿ���������
//	bool CalVcRd(float &b,float &h,float &ratio,float &MVratio,int &cover)
};

