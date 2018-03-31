#pragma once
#include "PredefSectDescrib.h"
#if !defined(SECTPROPER_H_)
#define  SECTPROPER_H_
//截面特性计算
class SectProperty
{
public:
	SectProperty(void);
	~SectProperty(void);

public:
	//混凝土特性
	double Zcx,Zcy;
	double Icx,Icy;

	//钢筋特性
	//绕x轴 	两端钢筋面积矩，两侧钢筋面积矩
	double Zsr1x,Zsr2x;
	//绕x轴 	两端钢筋惯性矩，两侧钢筋惯性矩
	double Isr1x,Isr2x;
	double Zsr1y,Zsr2y;
	double Isr1y,Isr2y;
	float hs1,hs2;
	float bs1,bs2;
	float d1y,d1x;	//b边上的钢筋等效钢板型心到截面中心的距离
	float d2y,d2x;	//h边的同上
	float bar_inv_x,bar_inv_y;
	float As1,As2;
		//euro
	

	//型钢特性
	double Zsx;
	double Isx;
	double Zsy;
	double Isy;
	double Is_x,Is_y;
	float bb,hh;
	float bb2,hh2;		//绕y轴等效矩形面积特性

	//截面校核时用的特性
	float m_cover;	//保护层
	float As,Asr,Ac,Ag;	
	double Ig_x,Ig_y;
	float bar_inv;
	float bar_d;
	float bartopro_x,bartopro_y;

	//强度指标
	float fy;		//钢材屈服强度
	float fs;		//钢筋屈服强度
	float fc;		//混凝土屈服强度
	//强度标准值
	float fyk;
	float fsk;
	float fck;

	//弹性模量
	float Es,Ecm,Esr;

//	void InitSect(SectDescribe &m_sect);
	void SectCal();	//截面特性计算
	void SectGeoInfo();		//截面检查用的参数
//	void SectCapCal();		//受剪和受弯承载力计算
private:
//	SectDescribe m_SectData;
	void EQASCal();		//钢筋等效钢板面积计算
	void EQAGCal();
	void EQACCal();
};

#endif