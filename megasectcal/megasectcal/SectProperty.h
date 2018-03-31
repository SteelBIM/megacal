#pragma once
#include "PredefSectDescrib.h"
#if !defined(SECTPROPER_H_)
#define  SECTPROPER_H_
//�������Լ���
class SectProperty
{
public:
	SectProperty(void);
	~SectProperty(void);

public:
	//����������
	double Zcx,Zcy;
	double Icx,Icy;

	//�ֽ�����
	//��x�� 	���˸ֽ�����أ�����ֽ������
	double Zsr1x,Zsr2x;
	//��x�� 	���˸ֽ���Ծأ�����ֽ���Ծ�
	double Isr1x,Isr2x;
	double Zsr1y,Zsr2y;
	double Isr1y,Isr2y;
	float hs1,hs2;
	float bs1,bs2;
	float d1y,d1x;	//b���ϵĸֽ��Ч�ְ����ĵ��������ĵľ���
	float d2y,d2x;	//h�ߵ�ͬ��
	float bar_inv_x,bar_inv_y;
	float As1,As2;
		//euro
	

	//�͸�����
	double Zsx;
	double Isx;
	double Zsy;
	double Isy;
	double Is_x,Is_y;
	float bb,hh;
	float bb2,hh2;		//��y���Ч�����������

	//����У��ʱ�õ�����
	float m_cover;	//������
	float As,Asr,Ac,Ag;	
	double Ig_x,Ig_y;
	float bar_inv;
	float bar_d;
	float bartopro_x,bartopro_y;

	//ǿ��ָ��
	float fy;		//�ֲ�����ǿ��
	float fs;		//�ֽ�����ǿ��
	float fc;		//����������ǿ��
	//ǿ�ȱ�׼ֵ
	float fyk;
	float fsk;
	float fck;

	//����ģ��
	float Es,Ecm,Esr;

//	void InitSect(SectDescribe &m_sect);
	void SectCal();	//�������Լ���
	void SectGeoInfo();		//�������õĲ���
//	void SectCapCal();		//�ܼ����������������
private:
//	SectDescribe m_SectData;
	void EQASCal();		//�ֽ��Ч�ְ��������
	void EQAGCal();
	void EQACCal();
};

#endif