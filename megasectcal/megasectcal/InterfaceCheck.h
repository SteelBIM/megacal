#pragma once
#include <math.h>
#include "PredefSectDescrib.h"
//计算界面剪力，两个规范都是用的梁法和桁架法
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
	float Vcc1_x,Vcc2_x,Vcc3_x,Vcc4_x;	//对应的不同方向的界面单位剪应力
	float Vcc1_y,Vcc2_y,Vcc3_y,Vcc4_y;
	int Ncc1_x,Ncc2_x,Ncc3_x,Ncc4_x;
	int Ncc1_y,Ncc2_y,Ncc3_y,Ncc4_y;
private:
	double Ieffcbs_x,Ieffcbs_y;		//bs区间的等效惯性矩
	double Ieffcbs2_x,Ieffcbs2_y;  //bs'区间的等效惯性矩
	float C1;		//混凝土刚度折减系数
	float Vbs_x,Vbs_y;		//bs截面上的剪力,实际是垂直于该定义方向的剪力
	float Vbs2_x,Vbs2_y;		//bs'截面上的剪力
	void CalInterface();	//界面剪力的计算
	void CalSepVbs();		//计算各个截面的等效EI，并分配V
	void InitCal();		//准备一些计算参数
	bool TrussModel(float &r_y,float &r_z,int i_count = 0);	//桁架模型可以给出不同配箍率下的承载力
	float GetShearStud(void);
	float Getn(const int direction,float bw,float Ec,float Es,float z,float Li);		//计算桁架模型中的n
	double CalVRd(float &b,float &h,float &ratio,float &MVratio,int &cover);		//计算纯混凝土部分的抗剪承载力
//	bool CalVcRd(float &b,float &h,float &ratio,float &MVratio,int &cover)
};

