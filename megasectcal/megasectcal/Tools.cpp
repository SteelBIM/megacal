#include "StdAfx.h"
#include "Tools.h"
#include "GPara.h"
#include "SectProperty.h"
#include <math.h>
#include "Force_combine.h"
#include "EUROCode.h"
#include "cfgbas.h"
extern "C" __declspec(dllimport) CCfgBas* m_pCfgBas;

#define  min_dis 1
Tools::Tools(void)
{
}


Tools::~Tools(void)
{
}

void Tools::CompareVal(CheckRst_ &m_check,const bool checkval2)
{
	if(fabs(m_check.PermitVal_upper-10000)<0.01){		//没有上限的情况
		if(m_check.CalVal-m_check.PermitVal_lower>0.01){
			m_check.iOK = true;
		}
		else{
			m_check.iOK = false;
		}
	}
	else if(fabs(m_check.PermitVal_lower-10000)<0.01){		//没有下限的情况
		if(m_check.PermitVal_upper-m_check.CalVal>0.01){
			m_check.iOK = true;
		}
		else{
			m_check.iOK = false;
		}
	}
	else{	//有上限情况
		if(m_check.CalVal-m_check.PermitVal_lower>0.01 && m_check.PermitVal_upper-m_check.CalVal>0.01){
			m_check.iOK = true;
		}
		else{
			m_check.iOK = false;
		}
	}

	//存在第二个数据
	if(checkval2){
		if(fabs(m_check.PermitVal_upper-10000)<0.01){		//没有上限的情况
			if(m_check.CalVal2-m_check.PermitVal_lower>0.01){
				m_check.iOK2 = true;
			}
			else{
				m_check.iOK2 = false;
			}
		}
		else if(fabs(m_check.PermitVal_lower-10000)<0.01){		//没有下限的情况
			if(m_check.PermitVal_upper-m_check.CalVal2>0.01){
				m_check.iOK2 = true;
			}
			else{
				m_check.iOK2 = false;
			}
		}
		else{	//有上限情况
			if(m_check.CalVal2-m_check.PermitVal_lower>0.01 && m_check.PermitVal_upper-m_check.CalVal2>0.01){
				m_check.iOK2 = true;
			}
			else{
				m_check.iOK2 = false;
			}
		}
	}
}

int Tools::GetCmindur_EURO(int &stru_class,int &exposure_class)
{
	//stru_class
	//s1 s2 s3 s4 s5 s6
	// 0  1  2  3  4  5
	//exposure_class
	//x0 xc1 xc2 xc3 xc4 xd1 xd2 xd3 xs1 xs2 xs3
	// 0  1   2   3   4   5   6   7   8   9   10
	int Cmindur[6][11]={
	{10,10,10,15,20,25,30},
	{10,10,15,20,25,30,35},
	{10,10,20,25,30,35,40},
	{10,15,25,30,35,40,45},
	{15,20,30,35,40,45,50},
	{20,25,35,40,45,50,55}
	};
	int ex_class = exposure_class;
	if(exposure_class==3)
		ex_class = 2;
	else if(exposure_class==4)
		ex_class = 3;
	else if(exposure_class==5 || exposure_class==8)
		ex_class = 4;
	else if(exposure_class==6 || exposure_class==9)
		ex_class = 5;
	else if(exposure_class==7 || exposure_class==10)
		ex_class = 6;
	int Cmin = Cmindur[stru_class][ex_class];
	return Cmin;
}

void Tools::GetConFc_EURO(int ConLevel,float &fck)
{
	fck = ConLevel;
}

void Tools::GetConFc_Chi(int ConLevel,float &fck)
{
	if(ConLevel==15)
		fck = 7.2;
	else if(ConLevel==20)
		fck = 9.6;
	else if(ConLevel==25)
		fck = 11.9;
	else if(ConLevel==30)
		fck = 14.3;
	else if(ConLevel==35)
		fck = 16.7;
	else if(ConLevel==40)
		fck = 19.1;
	else if(ConLevel==45)
		fck = 21.1;
	else if(ConLevel==50)
		fck = 23.1;
	else if(ConLevel==55)
		fck = 25.3;
	else if(ConLevel==60)
		fck = 27.5;
	else if(ConLevel==65)
		fck = 29.7;
	else if(ConLevel==70)
		fck = 31.8;
	else if(ConLevel==75)
		fck = 33.8;
	else if(ConLevel==80)
		fck = 35.9;
}

void Tools::GetConFtk_Chi(int ConLevel,float &ftk)
{
	if(ConLevel==15)
		ftk = 1.27;
	else if(ConLevel==20)
		ftk = 1.54;
	else if(ConLevel==25)
		ftk = 1.78;
	else if(ConLevel==30)
		ftk = 2.01;
	else if(ConLevel==35)
		ftk = 2.2;
	else if(ConLevel==40)
		ftk = 2.39;
	else if(ConLevel==45)
		ftk = 2.51;
	else if(ConLevel==50)
		ftk = 2.64;
	else if(ConLevel==55)
		ftk = 2.74;
	else if(ConLevel==60)
		ftk = 2.85;
	else if(ConLevel==65)
		ftk = 2.93;
	else if(ConLevel==70)
		ftk = 2.99;
	else if(ConLevel==75)
		ftk = 3.05;
	else if(ConLevel==80)
		ftk = 3.11;
}

float Tools::GetProFy(int ProType,float TT,int ProNo)
{
	float gamas = 1.f;
	if(ProType>10){
		ProType = ProType%10;
		if(GPara::m_DesnPara->CodeType==3)		//中国规范
			gamas = 1.11;
		else
			gamas = 1;
	}
	float Fyy;
	if(ProType==0){		//EN-10025
		if((TT-16)<0.01){			//s355jr s460jr s500jo s355m s420m s460m s500m
			if(ProNo==0)
				Fyy = 355/gamas;
			else if(ProNo==1)
				Fyy = 460/gamas;
			else if(ProNo==2)
				Fyy = 500/gamas;
			else if(ProNo==3)
				Fyy = 355/gamas;
			else if(ProNo==4)
				Fyy = 420/gamas;
			else if(ProNo==5)
				Fyy = 460/gamas;
			else if(ProNo==6)
				Fyy = 500/gamas;
			else 
				Fyy = 355/gamas;
		}
		else if((TT-16)>0.01 && (TT-40)<0.01){
			if(ProNo==0)
				Fyy = 345/gamas;
			else if(ProNo==1)
				Fyy = 440/gamas;
			else if(ProNo==2)
				Fyy = 480/gamas;
			else if(ProNo==3)
				Fyy = 345/gamas;
			else if(ProNo==4)
				Fyy = 400/gamas;
			else if(ProNo==5)
				Fyy = 440/gamas;
			else if(ProNo==6)
				Fyy = 480/gamas;
			else 
				Fyy = 355/gamas;			
		}
		else if((TT-40)>0.01 && (TT-63)<0.01){
			if(ProNo==0)
				Fyy = 335/gamas;
			else if(ProNo==1)
				Fyy = 420/gamas;
			else if(ProNo==2)
				Fyy = 460/gamas;
			else if(ProNo==3)
				Fyy = 335/gamas;
			else if(ProNo==4)
				Fyy = 390/gamas;
			else if(ProNo==5)
				Fyy = 430/gamas;
			else if(ProNo==6)
				Fyy = 460/gamas;
			else 
				Fyy = 355/gamas;
		}
		else if((TT-63)>0.01 && (TT-80)<0.01){
			if(ProNo==0)
				Fyy = 325/gamas;
			else if(ProNo==1)
				Fyy = 400/gamas;
			else if(ProNo==2)
				Fyy = 450/gamas;
			else if(ProNo==3)
				Fyy = 325/gamas;
			else if(ProNo==4)
				Fyy = 380/gamas;
			else if(ProNo==5)
				Fyy = 410/gamas;
			else if(ProNo==6)
				Fyy = 450/gamas;
			else 
				Fyy = 355/gamas;
		}
		else if((TT-80)>0.01 && (TT-100)<0.01){
			if(ProNo==0)
				Fyy = 315/gamas;
			else if(ProNo==1)
				Fyy = 390/gamas;
			else if(ProNo==2)
				Fyy = 450/gamas;
			else if(ProNo==3)
				Fyy = 325/gamas;
			else if(ProNo==4)
				Fyy = 370/gamas;
			else if(ProNo==5)
				Fyy = 400/gamas;
			else if(ProNo==6)
				Fyy = 450/gamas;
			else 
				Fyy = 355/gamas;
		}
		else if((TT-100)>0.01 && (TT-150)<0.01){
			if(ProNo==0)
				Fyy = 295/gamas;
			else if(ProNo==1)
				Fyy = 390/gamas;
			else if(ProNo==2)
				Fyy = 450/gamas;
			else if(ProNo==3)
				Fyy = 320/gamas;
			else if(ProNo==4)
				Fyy = 365/gamas;
			else if(ProNo==5)
				Fyy = 385/gamas;
			else if(ProNo==6)
				Fyy = 450/gamas;
			else 
				Fyy = 355/gamas;
		}		
	}
	else if(ProType==1){	//ETA-10/0156  HISTAR
		if((TT-82)<0.01){
			if(ProNo==0)
				Fyy = 355/gamas;
			else if(ProNo==1)
				Fyy = 460/gamas;
			else 
				Fyy = 355/gamas;
		}
		else if((TT-82)>0.01 && (TT-125)<0.01){
			if(ProNo==0)
				Fyy = 355/gamas;
			else if(ProNo==1)
				Fyy = 450/gamas;
			else 
				Fyy = 355/gamas;
		}
		else if((TT-125)>0.01 && (TT-140)<0.01){
			if(ProNo==0)
				Fyy = 355/gamas;
			else if(ProNo==1)
				Fyy = 450/gamas;
			else 
				Fyy = 450/gamas;
		}
	}
	else if(ProType==2){	//ASTM
		if(ProNo==0)
			Fyy = 345/gamas;
		else if(ProNo==1)
			Fyy = 450/gamas;
		else if(ProNo==2)
			Fyy = 485/gamas;
		else if(ProNo==3)
			Fyy = 550/gamas;
	}
	else if(ProType==3){	//chinese
		if(TT<=16){
			if(ProNo==0)		//Q235
				Fyy = 235/gamas;
			else if(ProNo==1)	//Q345
				Fyy = 345/gamas;
			else if(ProNo==2)	//Q345GJ
				Fyy = 345/gamas;
			else if(ProNo==3)	//Q390
				Fyy = 390/gamas;
			else if(ProNo==4)	//Q420
				Fyy = 420/gamas;
		}
		else if(TT>16 && TT<=35){
			if(ProNo==0)		//Q235
				Fyy = 225/gamas;
			else if(ProNo==1)	//Q345
				Fyy = 335/gamas;
			else if(ProNo==2)	//Q345GJ
				Fyy = 345/gamas;
			else if(ProNo==3)	//Q390
				Fyy = 370/gamas;
			else if(ProNo==4)	//Q420
				Fyy = 400/gamas;
		}
		else if(TT>35 && TT<=50){
			if(ProNo==0)		//Q235
				Fyy = 215/gamas;
			else if(ProNo==1)	//Q345
				Fyy = 325/gamas;
			else if(ProNo==2)	//Q345GJ
				Fyy = 335/gamas;
			else if(ProNo==3)	//Q390
				Fyy = 350/gamas;
			else if(ProNo==4)	//Q420
				Fyy = 380/gamas;
		}
		else if(TT>50 && TT<=100){
			if(ProNo==0)		//Q235
				Fyy = 215/gamas;
			else if(ProNo==1)	//Q345
				Fyy = 315/gamas;
			else if(ProNo==2)	//Q345GJ
				Fyy = 325/gamas;
			else if(ProNo==3)	//Q390
				Fyy = 330/gamas;
			else if(ProNo==4)	//Q420
				Fyy = 360/gamas;
		}
		else{
			if(ProNo==0)		//Q235
				Fyy = 215/gamas;
			else if(ProNo==1)	//Q345
				Fyy = 315/gamas;
			else if(ProNo==2)	//Q345GJ
				Fyy = 325/gamas;
			else if(ProNo==3)	//Q390
				Fyy = 330/gamas;
			else if(ProNo==4)	//Q420
				Fyy = 360/gamas;
		}
	}

	return Fyy;
}

float Tools::GetProFy_EURO(int ProType,float TT,int ProNo)
{
	float Fyy;
	if(ProType==11){		//EURO_HISTAR
		if((TT-82)<0.01){
			if(ProNo==1)
				Fyy = 355;
			else if(ProNo==2)
				Fyy = 355;
			else if(ProNo==3)
				Fyy = 460;
			else if(ProNo==4)
				Fyy = 460;
			else 
				Fyy = 460;
		}
		else if((TT-82)>0.01 && (TT-125)<0.01){
			if(ProNo==1)
				Fyy = 355;
			else if(ProNo==2)
				Fyy = 355;
			else if(ProNo==3)
				Fyy = 450;
			else if(ProNo==4)
				Fyy = 450;
			else 
				Fyy = 450;
		}
		else if((TT-125)>0.01 && (TT-140)<0.01){
			if(ProNo==1)
				Fyy = 355;
			else if(ProNo==2)
				Fyy = 0;
			else if(ProNo==3)
				Fyy = 450;
			else if(ProNo==4)
				Fyy = 450;
			else 
				Fyy = 450;
		}
	}
	else if(ProType==12){		//EURO_EN10025-2
		if((TT-16)<0.01){
			if(ProNo==1)
				Fyy = 355;
			else if(ProNo==2)
				Fyy = 460;
			else if(ProNo==3)
				Fyy = 500;
			else if(ProNo==4)
				Fyy = 350;
			else if(ProNo==5)
				Fyy = 420;
			else if(ProNo==6)
				Fyy = 460;
			else if(ProNo==7)
				Fyy = 500;
			else 
				Fyy = 355;
		}
		else if((TT-16)>0.01 && (TT-40)<0.01){
			if(ProNo==1)
				Fyy = 345;
			else if(ProNo==2)
				Fyy = 440;
			else if(ProNo==3)
				Fyy = 480;
			else if(ProNo==4)
				Fyy = 345;
			else if(ProNo==5)
				Fyy = 400;
			else if(ProNo==6)
				Fyy = 440;
			else if(ProNo==7)
				Fyy = 480;
			else 
				Fyy = 355;			
		}
		else if((TT-40)>0.01 && (TT-63)<0.01){
			if(ProNo==1)
				Fyy = 335;
			else if(ProNo==2)
				Fyy = 420;
			else if(ProNo==3)
				Fyy = 460;
			else if(ProNo==4)
				Fyy = 335;
			else if(ProNo==5)
				Fyy = 390;
			else if(ProNo==6)
				Fyy = 430;
			else if(ProNo==7)
				Fyy = 460;
			else 
				Fyy = 355;
		}
		else if((TT-63)>0.01 && (TT-80)<0.01){
			if(ProNo==1)
				Fyy = 325;
			else if(ProNo==2)
				Fyy = 400;
			else if(ProNo==3)
				Fyy = 450;
			else if(ProNo==4)
				Fyy = 325;
			else if(ProNo==5)
				Fyy = 380;
			else if(ProNo==6)
				Fyy = 410;
			else if(ProNo==7)
				Fyy = 450;
			else 
				Fyy = 355;
		}
		else if((TT-80)>0.01 && (TT-100)<0.01){
			if(ProNo==1)
				Fyy = 315;
			else if(ProNo==2)
				Fyy = 390;
			else if(ProNo==3)
				Fyy = 450;
			else if(ProNo==4)
				Fyy = 325;
			else if(ProNo==5)
				Fyy = 370;
			else if(ProNo==6)
				Fyy = 400;
			else if(ProNo==7)
				Fyy = 450;
			else 
				Fyy = 355;
		}
		else if((TT-100)>0.01 && (TT-150)<0.01){
			if(ProNo==1)
				Fyy = 295;
			else if(ProNo==2)
				Fyy = 390;
			else if(ProNo==3)
				Fyy = 450;
			else if(ProNo==4)
				Fyy = 320;
			else if(ProNo==5)
				Fyy = 365;
			else if(ProNo==6)
				Fyy = 385;
			else if(ProNo==7)
				Fyy = 450;
			else 
				Fyy = 355;
		}
		else if((TT-150)>0.01 && (TT-200)<0.01){
			if(ProNo==1)
				Fyy = 285;
			else
				Fyy = 285;
		}
		else if((TT-200)>0.01 && (TT-250)<0.01){
			if(ProNo==1)
				Fyy = 275;
			else
				Fyy = 275;
		}
		else if((TT-250)>0.01 && (TT-400)<0.01){
			if(ProNo==1)
				Fyy = 265;
			else
				Fyy = 265;
		}
	}

	return Fyy;
}

float Tools::GetProFy_Chi(int ProType,float TT)
{
	float Fyy;
	if(TT<=16){
		if(ProType==1)		//Q235
			Fyy = 235;
		else if(ProType==2)	//Q345
			Fyy = 345;
		else if(ProType==3)	//Q345GJ
			Fyy = 345;
		else if(ProType==4)	//Q390
			Fyy = 390;
		else if(ProType==5)	//Q420
			Fyy = 420;
	}
	else if(TT>16 && TT<=35){
		if(ProType==1)		//Q235
			Fyy = 225;
		else if(ProType==2)	//Q345
			Fyy = 335;
		else if(ProType==3)	//Q345GJ
			Fyy = 345;
		else if(ProType==4)	//Q390
			Fyy = 370;
		else if(ProType==5)	//Q420
			Fyy = 400;
	}
	else if(TT>35 && TT<=50){
		if(ProType==1)		//Q235
			Fyy = 215;
		else if(ProType==2)	//Q345
			Fyy = 325;
		else if(ProType==3)	//Q345GJ
			Fyy = 335;
		else if(ProType==4)	//Q390
			Fyy = 350;
		else if(ProType==5)	//Q420
			Fyy = 380;
	}
	else if(TT>50 && TT<=100){
		if(ProType==1)		//Q235
			Fyy = 215;
		else if(ProType==2)	//Q345
			Fyy = 315;
		else if(ProType==3)	//Q345GJ
			Fyy = 325;
		else if(ProType==4)	//Q390
			Fyy = 330;
		else if(ProType==5)	//Q420
			Fyy = 360;
	}
	else{
		if(ProType==1)		//Q235
			Fyy = 215;
		else if(ProType==2)	//Q345
			Fyy = 315;
		else if(ProType==3)	//Q345GJ
			Fyy = 325;
		else if(ProType==4)	//Q390
			Fyy = 330;
		else if(ProType==5)	//Q420
			Fyy = 360;
	}
	return Fyy;
}

float Tools::GetProFy_AISC(int ProType)
{
	return 1;
}

float Tools::GetBarFyv(int CodeType)
{
	if(CodeType==1){		//EURO

	}
	else if(CodeType==2){		//AISC

	}
	else if(CodeType==3){		//chinese

	}
	return GPara::m_DesnPara->BarMp;	//直接用户输入钢筋强度
}

void Tools::GetConPara_EURO(int fck,float &fcm,float &Ecm,float &fctm)
{
	fcm = fck+8;
	Ecm = 1000*22*0.3*exp(log(fcm/10));
	if(fck<=50)
		fctm = 0.3*exp(2*log(1.f*fck)/3);
	else
		fctm = 2.12*log(1+fcm/10);
}

float Tools::GetConEcm_EURO(int fck)
{
	float Ecm_euro;
	float fcm = fck+8;
	Ecm_euro = 22*exp(0.3*log(fcm/10));
// 	if(fck==12)
// 		Ecm_euro = 27;
// 	else if(fck==16)
// 		Ecm_euro = 29;
// 	else if(fck==20)
// 		Ecm_euro = 30;
// 	else if(fck==25)
// 		Ecm_euro = 31;
// 	else if(fck==30)
// 		Ecm_euro = 33;
// 	else if(fck==35)
// 		Ecm_euro = 34;
// 	else if(fck==40)
// 		Ecm_euro = 35;
// 	else if(fck==45)
// 		Ecm_euro = 36;
// 	else if(fck==50)
// 		Ecm_euro = 37;
// 	else if(fck==55)
// 		Ecm_euro = 38;
// 	else if(fck==60)
// 		Ecm_euro = 39;
// 	else if(fck==70)
// 		Ecm_euro = 41;
// 	else if(fck==80)
// 		Ecm_euro = 42;
// 	else if(fck==90)
// 		Ecm_euro = 44;
// 	else
// 		Ecm_euro = 44;

	return Ecm_euro*1000;
}

float Tools::GetConEcm_Chi(int fck)
{
	float Ecm_chi;
	if(fck==20)
		Ecm_chi = 2.8;
	else if(fck==30)
		Ecm_chi = 3.0;
	else if(fck==35)
		Ecm_chi = 3.15;
	else if(fck==40)
		Ecm_chi = 3.25;
	else if(fck==45)
		Ecm_chi = 3.35;
	else if(fck==50)
		Ecm_chi = 3.45;
	else if(fck==55)
		Ecm_chi = 3.55;
	else if(fck==60)
		Ecm_chi = 3.60;
	else if(fck==65)
		Ecm_chi = 3.65;
	else if(fck==70)
		Ecm_chi = 3.70;
	else if(fck==75)
		Ecm_chi = 3.75;
	else if(fck==80)
		Ecm_chi = 3.8;
	else 
		Ecm_chi = 3.8;

	return Ecm_chi*10000;
}

float Tools::GetConEcm_AISC(int fck)
{
	float fc = fck/6.89476;
	float Ecm = sqrt(fc)*exp(1.5*log(GPara::m_DesnPara->m_Conp));
	return Ecm;
}

int Tools::GetProE(const int code)
{
	int ProE;
	if(code==1){	//欧洲规范
		ProE = 210000;
	}
	else if(code==2){	//美国规范
		ProE = 29000;	//ksi
	}
	else {	//中国规范
		ProE = 206000;
	}
	return ProE;
}

int Tools::GetProG(const int code)
{
	int ProE =  GetProE(code);
	float temp = ProE*0.5/(1+GPara::m_DesnPara->m_Possion);
	int ProG = int(temp+0.5);
	return ProG;
}

float Tools::GetConEcm(int fck)
{
	if(GPara::m_DesnPara->CodeType==1)
	{
		return GetConEcm_EURO(fck);
	}
	else if(GPara::m_DesnPara->CodeType==2)
	{
		return GetConEcm_AISC(fck);
	}
	else if(GPara::m_DesnPara->CodeType==3)
	{
		return GetConEcm_Chi(fck);
	}
}

int Tools::GetEcc(int Ec)
{
	return Ec;
}

void Tools::Getbthtlim_chi(int &bt,int &ht)
{
	int pro_mat = GPara::m_Sect_in->HSect_mat;
	if(pro_mat==1){
		bt = 23;ht = 96;
	}
	else if(pro_mat==2 || pro_mat==3){
		bt=19;ht=81;
	}
	else if(pro_mat==4){
		bt=18;ht=75;
	}
	else if(pro_mat==5){
		bt=17;ht=71;
	}
}

void Tools::SwayColmAmplify(float &d1,float &d2,double Vd[2],double Vv[2])
{
	SectProperty m_Sect;
	m_Sect.SectGeoInfo();
	m_Sect.SectCal();
	float betadns_x = 1.f;
	if(betadns_x<fabs(Vd[0]/Vv[0]))
		betadns_x = fabs(Vd[0]/Vv[0]);
	float betadns_y = 1.f;
	if(betadns_y<fabs(Vd[1]/Vv[1]))
		betadns_y = fabs(Vd[1]/Vv[1]);
	float Len = GPara::m_DesnPara->Lens*1000;
	float faic = GPara::m_DesnPara->faic;
	int b = GPara::m_Sect_in->Sect_b;
	int h = GPara::m_Sect_in->Sect_h;
	float kk = GPara::m_DesnPara->uL;
	int Esr = Tools::GetBarE(2);
	int Es  = Tools::GetProE(2);
	int Ecm = Tools::GetConEcm(GPara::m_Sect_in->Concrete_mat);
	double EIeff_x,EIeff_y; 
	double Pc_x,Pc_y;
	float rr = 0.3*h;
	float LL = kk*Len/rr;
	double Pu = 1.2*GPara::m_DesnPara->N_ed[0]+1.6*(GPara::m_DesnPara->N_ed[1]+GPara::m_DesnPara->N_ed[2]+GPara::m_DesnPara->N_ed[3]);		//都是轴压力
	if(LL>22){
		EIeff_x = (0.2*Ecm*m_Sect.Is_x)/(1+betadns_x)+Es*m_Sect.Isx;
		EIeff_y = (0.2*Ecm*m_Sect.Is_y)/(1+betadns_y)+Es*m_Sect.Isy;
		Pc_x = PI*PI*EIeff_x/(kk*Len*Len);
		Pc_y = PI*PI*EIeff_y/(kk*Len*Len);
		float ks_x = 1/(1-(Pu/(0.75*Pc_x)));
		float ks_y = 1/(1-(Pu/(0.75*Pc_y)));
		if(ks_x<1)
			ks_x = 1.f;
		if(ks_y<1)
			ks_y = 1.f;
		d1 = ks_x;
		d2 = ks_y;
	}
	else{
		d1 = 1.f;
		d2 = 1.f;
	}
}

void Tools::NoSwayColmAmplify(double &Ns,double &Ned,double Med1[2],double Med2[2],float &d1,float &d2)
{
	//单位为inches
//	float Ned;
	SectProperty m_Sect;
	m_Sect.SectGeoInfo();
	m_Sect.SectCal();
	float Len = GPara::m_DesnPara->Lens*1000;
	float faic = GPara::m_DesnPara->faic;
	//有效刚度，轴力
	float C1 = 0.25+3*(m_Sect.As+m_Sect.Asr)/m_Sect.Ag;
	if(C1>0.7)
		C1 = 0.7;
	int Esr = Tools::GetBarE(2);
	int Es  = Tools::GetProE(2);
	int Ecm = Tools::GetConEcm(GPara::m_Sect_in->Concrete_mat);
	double EIeff_x,EIeff_y;
	EIeff_x = Esr*m_Sect.Isr1x+Es*m_Sect.Isx+C1*Ecm*m_Sect.Icx;
	EIeff_y = Esr*m_Sect.Isr2y+Es*m_Sect.Isy+C1*Ecm*m_Sect.Icy;
// 	double Pn0 = GPara::NplRd;
// 	double Pe_x = PI*PI*EIeff_x/(Len*Len);
// 	double Pe_y = PI*PI*EIeff_y/(Len*Len);
// 	double Pn_x,Pn_y;
// 	float rx = Pn0/Pe_x;
// 	float ry = Pn0/Pe_y;
// 	if(rx<2.25)
// 		Pn_x = Pn0*exp(rx*log(0.658));
// 	else
// 		Pn_x = 0.877*Pe_x;
// 
// 	if(ry<2.25)
// 		Pn_y = Pn0*exp(ry*log(0.658));
// 	else
// 		Pn_y = 0.877*Pe_y;
//	NbplRd_x = faic*Pn_x;
//	NbplRd_y = faic*Pn_y;
	//考虑弯矩
	bool Amplify_x=false;
	bool Amplify_y=false;
	double Mmax_x,Mmin_x;
	double Mmax_y,Mmin_y;
	double Pc_x,Pc_y;
	float betadns = fabs(Ns/Ned);
	float deltans;
	float Cm;
	int b = GPara::m_Sect_in->Sect_b;
	int h = GPara::m_Sect_in->Sect_h;
	float kk = GPara::m_DesnPara->uL;
	double M2min;
	float r1 = Med1[0]/Med2[0];
	if(fabs(r1)>1){
		r1 = 1/r1;
		double temp = Med1[0];
		Med1[0] = Med2[0];
		Med2[0] = temp;
//		std::swap(Med1[0],Med2[0]);
	}
	float r2 = Med1[1]/Med2[1];
	if(fabs(r2)>1){
		r2 = 1/r2;
		double temp = Med1[1];
		Med1[1] = Med2[1];
		Med2[1] = temp;
//		std::swap(Med1[1],Med2[1]);
	}
	if(!GPara::m_DesnPara->ifsway){	//无侧移
		//沿x方向
		float rr = 0.3*h;
		float LL = kk*Len/rr;
		float LLlim = min(34+12*r1,40);
		if(LL>LLlim){
			Amplify_x=true;
			EIeff_x = (0.2*Ecm*m_Sect.Ig_x)/(1+betadns)+Es*m_Sect.Isx;
			Pc_x = PI*PI*EIeff_x/(kk*Len*Len);
			M2min = Ned*(0.6+0.03*h);
			if(fabs(Med2[0])<M2min)
				Med2[0] = M2min*Med2[0]/fabs(Med2[0]);
			r1 = Med1[0]/Med2[0];
			if(GPara::m_DesnPara->My_hasmidforce)
				Cm = 1.0;
			else
				Cm = 0.6-0.4*r1;
			deltans = Cm/(1-Ned/(0.75*Pc_x));
			if(deltans<1.f)
				deltans = 1.f;
			if(deltans>1.4)
				deltans = 1.4f;
			d1 = deltans;
			//Mbed_x = Mmax_x*deltans;
		}
		else
			d1 = 1.f;
//			Mbed_x = Mmax_x;
		//沿y方向
		rr = 0.3*b;
		LL = kk*Len/rr;
		LLlim = min(34+12*r2,40);
		if(LL>LLlim){
			Amplify_y=true;
			EIeff_y = (0.2*Ecm*m_Sect.Ig_y)/(1+betadns)+Es*m_Sect.Isy;
			Pc_y = PI*PI*EIeff_y/(kk*Len*Len);
			M2min = Ned*(0.6+0.03*b);
			if(fabs(Med2[1])<M2min)
				Med2[1] = M2min*Med2[1]/fabs(Med2[1]);
			r2 = Med1[1]/Med2[1];
			if(GPara::m_DesnPara->Mz_hasmidforce)
				Cm = 1.0;
			else
				Cm = 0.6-0.4*r2;
			deltans = Cm/(1-Ned/(0.75*Pc_y));
			if(deltans<1.f)
				deltans = 1.f;
			if(deltans>1.4)
				deltans = 1.4f;		
			d2 = deltans;
//			Mbed_y = Mmax_y*deltans;
		}
		else
			d2 = 1.f;
//			Mbed_y = Mmax_y;
	}

}

int Tools::GetBarE(const int code)
{
	int BarE = 100000;
	if(code==1){
		BarE = 200000;		//N/mm2
	}
	else if(code==2){
		BarE = 29000;	//ksi
	}
	else if(code==3){
		BarE = 200000;		//N/mm2
	}
	
	return BarE;
}

//从NM曲线中线性差值求Mnplrd
double Tools::GetMFromCurve(double Ned,const int yz)
{
	double Mm[10];
	double Nn[10];
	if(yz==1){
		memcpy(Mm,GPara::M_Curve[0],10*sizeof(double));
		memcpy(Nn,GPara::N_Curve[0],10*sizeof(double));
	}
	else{
		memcpy(Mm,GPara::M_Curve[2],10*sizeof(double));
		memcpy(Nn,GPara::N_Curve[2],10*sizeof(double));
	}
	double Mnplrd = Mm[0];
	//这里假定N都是大于0的，N从0到大排列
	for(int i=1;i<10;i++){
		if(Nn[i]>Ned){
			Mnplrd = Mm[i-1]+(Ned-Nn[i-1])*(Mm[i]-Mm[i-1])/(Nn[i]-Nn[i-1]);
			break;
		}
	}
	return Mnplrd;
}

float Tools::GetPos(int Pos)
{
	float parts = GPara::m_DesnPara->Lens/12;
	float pos_m = parts*Pos;
	return pos_m;
}

float Tools::GetProFy(CString Mat,float TT,int forDesign)
{
	float Fyy = 355;
	int ENcode,ETAcode,ASTMcode,Chinesecode,Custom,BenGou;
	ENcode=ETAcode=ASTMcode=Chinesecode=-1;
	ENcode = Mat.Find(_T("S"));
	ETAcode = Mat.Find(_T("Histar"));
	ASTMcode = Mat.Find(_T("Gr"));
	Chinesecode = Mat.Find(_T("Q"));
	Custom = Mat.Find(_T("MPa"));
	BenGou = Mat.Find(_T("Custom"));

	float gamas = 1.f;
	if(forDesign==1){
		if(GPara::m_DesnPara->CodeType==3)
			gamas = 1.11;			
	}
	if(ENcode==0){		//EN-10025
		Fyy = 355;
		if(Mat == "S355JR/J0/J2/K2"){	//1
			if((TT-16)<0.01){
				Fyy = 355/gamas;
			}
			else if((TT-16)>0.01 && (TT-40)<0.01){
				Fyy = 345/gamas;
			}
			else if((TT-40)>0.01 && (TT-63)<0.01){
				Fyy = 335/gamas;
			}
			else if((TT-63)>0.01 && (TT-80)<0.01){
				Fyy = 325/gamas;
			}
			else if((TT-80)>0.01 && (TT-100)<0.01){
				Fyy = 315/gamas;
			}
			else if((TT-100)>0.01 && (TT-150)<0.01){
				Fyy = 295/gamas;
			}
		}
		else if(Mat == "S460JR/J0/J2/K2"){	//2
			if((TT-16)<0.01){
				Fyy = 460/gamas;
			}
			else if((TT-16)>0.01 && (TT-40)<0.01){
				Fyy = 440/gamas;
			}
			else if((TT-40)>0.01 && (TT-63)<0.01){
				Fyy = 420/gamas;
			}
			else if((TT-63)>0.01 && (TT-80)<0.01){
				Fyy = 400/gamas;
			}
			else if((TT-80)>0.01 && (TT-100)<0.01){
				Fyy = 390/gamas;
			}
			else if((TT-100)>0.01 && (TT-150)<0.01){
				Fyy = 390/gamas;
			}
		}
		else if(Mat == "S500J0"){	//3
			if((TT-16)<0.01){
				Fyy = 500/gamas;
			}
			else if((TT-16)>0.01 && (TT-40)<0.01){
				Fyy = 480/gamas;
			}
			else if((TT-40)>0.01 && (TT-63)<0.01){
				Fyy = 460/gamas;
			}
			else if((TT-63)>0.01 && (TT-80)<0.01){
				Fyy = 450/gamas;
			}
			else if((TT-80)>0.01 && (TT-100)<0.01){
				Fyy = 450/gamas;
			}
			else if((TT-100)>0.01 && (TT-150)<0.01){
				Fyy = 450/gamas;
			}
		}
		else if(Mat == "S355M/ML"){	//4
			if((TT-16)<0.01){
				Fyy = 355/gamas;
			}
			else if((TT-16)>0.01 && (TT-40)<0.01){
				Fyy = 345/gamas;
			}
			else if((TT-40)>0.01 && (TT-63)<0.01){
				Fyy = 335/gamas;
			}
			else if((TT-63)>0.01 && (TT-80)<0.01){
				Fyy = 325/gamas;
			}
			else if((TT-80)>0.01 && (TT-100)<0.01){
				Fyy = 325/gamas;
			}
			else if((TT-100)>0.01 && (TT-150)<0.01){
				Fyy = 320/gamas;
			}
		}
		else if(Mat == "S420M/ML"){	//5
			if((TT-16)<0.01){
				Fyy = 420/gamas;
			}
			else if((TT-16)>0.01 && (TT-40)<0.01){
				Fyy = 400/gamas;
			}
			else if((TT-40)>0.01 && (TT-63)<0.01){
				Fyy = 390/gamas;
			}
			else if((TT-63)>0.01 && (TT-80)<0.01){
				Fyy = 380/gamas;
			}
			else if((TT-80)>0.01 && (TT-100)<0.01){
				Fyy = 370/gamas;
			}
			else if((TT-100)>0.01 && (TT-150)<0.01){
				Fyy = 365/gamas;
			}
		}
		else if(Mat == "S460M/ML"){	//6
			if((TT-16)<0.01){
				Fyy = 460/gamas;
			}
			else if((TT-16)>0.01 && (TT-40)<0.01){
				Fyy = 440/gamas;
			}
			else if((TT-40)>0.01 && (TT-63)<0.01){
				Fyy = 430/gamas;
			}
			else if((TT-63)>0.01 && (TT-80)<0.01){
				Fyy = 410/gamas;
			}
			else if((TT-80)>0.01 && (TT-100)<0.01){
				Fyy = 400/gamas;
			}
			else if((TT-100)>0.01 && (TT-150)<0.01){
				Fyy = 385/gamas;
			}
		}
		else if(Mat == "S500M/ML"){	//7
			if((TT-16)<0.01){
				Fyy = 500/gamas;
			}
			else if((TT-16)>0.01 && (TT-40)<0.01){
				Fyy = 480/gamas;
			}
			else if((TT-40)>0.01 && (TT-63)<0.01){
				Fyy = 460/gamas;
			}
			else if((TT-63)>0.01 && (TT-80)<0.01){
				Fyy = 450/gamas;
			}
			else if((TT-80)>0.01 && (TT-100)<0.01){
				Fyy = 450/gamas;
			}
			else if((TT-100)>0.01 && (TT-150)<0.01){
				Fyy = 450/gamas;
			}
		}
	}

	if(ETAcode==0){		//#2
		Fyy = 450;
		if(Mat=="Histar355"){
			if((TT-82)<0.01){
				Fyy = 355/gamas;
			}
			else if((TT-82)>0.01 && (TT-125)<0.01){
				Fyy = 355/gamas;
			}
			else if((TT-125)>0.01 && (TT-140)<0.01){
				Fyy = 355/gamas;
			}
		}
		else if(Mat=="Histar460"){
			if((TT-82)<0.01){
				Fyy = 460/gamas;
			}
			else if((TT-82)>0.01 && (TT-125)<0.01){
				Fyy = 450/gamas;
			}
			else if((TT-125)>0.01 && (TT-140)<0.01){
				Fyy = 450/gamas;
			}
		}
	}

	if(ASTMcode>0){
		Fyy = 345/gamas;
		if(Mat=="A913Gr50"){
			Fyy = 345/gamas;
		}
		else if(Mat=="A913Gr65"){
			Fyy = 450/gamas;
		}
		else if(Mat=="A913Gr70"){
			Fyy = 485/gamas;
		}
		else if(Mat=="A913Gr80"){
			Fyy = 550/gamas;
		}
	}

	if(Chinesecode==0){
		Fyy = 235/gamas;
		if(Mat=="Q235"){	//1
			if(TT<=16){
				Fyy = 235/gamas;
			}
			else if(TT>16 && TT<=40){
				Fyy = 225/gamas;
			}
			else if(TT>40 && TT<=100){
				Fyy = 215/gamas;
			}
		}
		else if(Mat=="Q345"){	//2
			if(TT<=16){
				Fyy = 345/gamas;
			}
			else if(TT>16 && TT<=35){
				Fyy = 335/gamas;
			}
			else if(TT>35 && TT<=50){
				Fyy = 325/gamas;
			}
			else if(TT>50 && TT<=100){
				Fyy = 315/gamas;
			}
		}
		else if(Mat=="Q345GJ"){	//3
			if(TT<=16){
				Fyy = 345/gamas;
			}
			else if(TT>16 && TT<=50){
				Fyy = 345/gamas;
			}
			else if(TT>50 && TT<=100){
				Fyy = 335/gamas;
			}
		}
		else if(Mat=="Q390"){	//4
			if(TT<=16){
				Fyy = 390/gamas;
			}
			else if(TT>16 && TT<=35){
				Fyy = 370/gamas;
			}
			else if(TT>35 && TT<=50){
				Fyy = 350/gamas;
			}
			else if(TT>50 && TT<=100){
				Fyy = 330/gamas;
			}
		}
		else if(Mat=="Q420"){	//5
			if(TT<=16){
				Fyy = 420/gamas;
			}
			else if(TT>16 && TT<=35){
				Fyy = 400/gamas;
			}
			else if(TT>35 && TT<=50){
				Fyy = 380/gamas;
			}
			else if(TT>50 && TT<=100){
				Fyy = 360/gamas;
			}
		}
	}

	if(Custom>0){
		char* endptr;
		Fyy = strtod(Mat,&endptr);
		Fyy = Fyy/gamas;
	}

	if(BenGou==0){		//走本构曲线,返回曲线的编号
		CString b = Mat.Left(1);
		char* stopstring;
		Fyy = strtol(b,&stopstring,10);
	}

	return Fyy;
}

void Tools::DrawNMPoint_forfiber(float ratio,double Iin[],const int yz)
{
	Force_combine Colmforce;
	Colmforce.GetForce();
	Colmforce.HeZai_combine();
	int cenghao,xianxing,xiankuan,yanse,fanhui;
	int N,n2,IX,IY;
	float Xzf,Yzf,XM,YM,ANGLE;
	CString str;
	char* ZFSTR;
	//画图
	int iWay = 99;
//	char Dir[256];
//	GetCurrentDirectory( 256, Dir ); 
	CString StrFile;
		if(yz==1)
			StrFile.Format(_T("Fiber_NM_y.t"));
		else
			StrFile.Format(_T("Fiber_NM_z.t"));
	int n1=StrFile.GetLength();
	char FileName[256];
	strcpy(FileName,StrFile);
	FileName[n1]=char(0);
	m_pCfgBas->Closfl(iWay);
	m_pCfgBas->Setcfg();
	m_pCfgBas->Openfl(iWay,FileName,n1,n1);
	m_pCfgBas->Nowfil(iWay);
	

	//画内力点
	cenghao=1;
	xianxing=1;
	yanse=10;
	fanhui=0;
	xiankuan = 10;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	double x,y,z;
	z = 0;
	double Myed,Mzed,Ned;
	double Med1[2],Med2[2];
	float kky,kkz;
	for(int i=0;i<GPara::m_DesnPara->n_Combines;i++){
		for(int j=0;j<Parts;j++){
			if(GPara::m_DesnPara->ifSecondEffect){
				Med1[0] = Colmforce.My_zuhe[i][0];
				Med1[1] = Colmforce.My_zuhe[i][12];
				Med2[0] = Colmforce.Mz_zuhe[i][0];
				Med2[1] = Colmforce.Mz_zuhe[i][12];
				Ned = Colmforce.N_zuhe[i][j];
				//EUROCode::AmplifyM(Med1,Med2,kky,kkz,i);
				AmplifyM_forfiber(Med1,Med2,Iin,kky,kkz);
				if(Colmforce.My[i][j]>0)
					Myed = kky*Colmforce.My[i][j];
				else
					Myed = Colmforce.My[i][j] - (kky-1)*Colmforce.My[i][j];
				if(Colmforce.Mz[i][j]>0)
					Mzed = kkz*Colmforce.Mz[i][j];
				else
					Mzed = Colmforce.Mz[i][j] - (kkz-1)*Colmforce.Mz[i][j];
			}
			else{
				Myed = Colmforce.My_zuhe[i][j];
				Mzed = Colmforce.Mz_zuhe[i][j];
				Ned  = Colmforce.N_zuhe[i][j];
			}
			if(yz==1){
				x = Myed/(ratio*10000000);
			}
			else
				x = Mzed/(ratio*10000000);
			y = Ned/10000000;
			float tempx,tempy,tempz;
			tempx = fabs(x);
			tempy = y;
			tempz = z;
			m_pCfgBas->Pnt3d(tempx,tempy,tempz);
		}
	}
}

void Tools::AmplifyM_forfiber(double Myed[2],double Mzed[2],double Iin[],float &kky,float &kkz)
{
	int Es = Tools::GetProE(1);
	int Esr = Tools::GetBarE(1);
	float Ecp = Tools::GetConEcm(GPara::m_Sect_in->Concrete_mat);
	Ecp = Ecp*0.5;
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
		beta_y = GPara::m_DesnPara->EC_beta;
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
		beta_z = GPara::m_DesnPara->EC_beta;
	}
	//	else if(GPara::m_DesnPara->Restrict==4)	//悬臂，梯形
	//		beta = 0.66;
	double EIeff1_x,EIeff1_y;
	double Ncreff_x,Ncreff_y;
	float K0,K1;
	K0 = 0.9;
	K1 = 0.5;
	EIeff1_x = K0*(Iin[0]+Iin[1]+K1*0.5*Iin[2]);		//陈涛拍脑袋ecp取0.5ec
	Ncreff_x = PI*PI*EIeff1_x/(L0*L0);
	//	if(fabs(Ned)<0.001)		//判断是传入的N还是直接取最大值
	double Ned=GPara::N_max;
	float KK_x = beta_y/(1-Ned/Ncreff_x);
	if(KK_x<1)
		KK_x = 1.f;
	EIeff1_y = K0*(Iin[3]+Iin[4]+K1*0.5*Iin[5]);		//陈涛拍脑袋ecp取0.5ec
	Ncreff_y = PI*PI*EIeff1_y/(L0*L0);
	float KK_y = beta_z/(1-Ned/Ncreff_y);
	if(KK_y<1)
		KK_y = 1.f;
	//保存一下
	//	GlobalResult::beta = beta;
	kky = KK_x;
	kkz = KK_y;
}

double Tools::GetMnplFromCurve(double N[],double M[],double Ned)
{
	double Mnpl = 0.f;
	for(int i=0;i<FirbePoints-1;i++){
		double Npre = N[i];
		double Nnext = N[i+1];
		double Mpre = M[i];
		double Mnext = M[i+1];
		if(Ned<Npre && Ned>Nnext){
			Mnpl = Mnext - (Ned-Nnext)*(Mnext-Mpre)/(Npre-Nnext);
			Mnpl = fabs(Mnpl);
			break;
		}
	}
	return Mnpl;
}

//从平面坐标
void Tools::GetStr(float& XCRS,float& YCRS,char* STR)
{

	CString strshow;
	bool ifreturn=false;
	for(int i=0;i<GPara::Pt_info.GetSize();i++){
		for(int j=0;j<(GPara::Pt_info[i]).pos_x->GetSize();j++){
			float x1 = ((GPara::Pt_info[i]).pos_x->GetAt(j));
			float y1 = ((GPara::Pt_info[i]).pos_y->GetAt(j));
			if(ifClose(x1,XCRS,y1,YCRS)){
				strshow = ((GPara::Pt_info)[i].pos_info)->GetAt(j);
//				strshow.Format(_T("%s"),(*((((GPara::Pt_info)[i];
				strcpy(STR,strshow);
				ifreturn = true;
				break;
			}
		}
		if(ifreturn)
			break;
	}
}

//判断是否足够近
bool Tools::ifClose(float x1,float x2,float y1,float y2)
{
	double dis = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	float errorD;
	int iepi;
	m_pCfgBas->Getiep(iepi, errorD);
	if(fabs(dis)<errorD)
		return true;
	else
		return false;
}
