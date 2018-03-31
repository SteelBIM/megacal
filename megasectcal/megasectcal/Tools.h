#pragma once
#include "MegaResults.h"
#include <math.h>
class Tools
{
public:
	Tools(void);
	~Tools(void);

public:
	static void CompareVal(CheckRst_ &m_check,const bool checkval2=false);
	static int  GetCmindur_EURO(int &stru_class,int &exposure_class);	//欧洲规范中获得耐久性要求最小保护层厚度
	static void GetConFc_EURO(int ConLevel,float &fck);
	static void GetConFc_Chi(int ConLevel,float &fck);
	static void GetConFtk_Chi(int ConLevel,float &ftk);
	static void GetConPara_EURO(int fck,float &fcm,float &Ecm,float &fctm);
	static float GetProFy_EURO(int ProType,float TT,int ProNo);
	static float GetProFy_Chi(int ProType,float TT);
	static float GetProFy_AISC(int ProType);
	static float GetProFy(int ProType,float TT,int ProNo);		//1、类型号，en astm  2、钢号
	static float GetProFy(CString Mat,float TT,int forDesign=0);	//从字符串来获得型钢强度
	static float GetBarFyv(int CodeType);
	static float GetConEcm_EURO(int fck);
	static float GetConEcm_Chi(int fck);
	static float GetConEcm_AISC(int fck);
	static float GetConEcm(int fck);
	static int GetProE(const int code);
	static int GetProG(const int code);
	static int GetBarE(const int code);
	static int GetEcc(int Ec);
	static float GetPos(int Pos);	//通过截面号获得截面位置
	static void Getbthtlim_chi(int &bt,int &ht);
	static double GetMFromCurve(double Ned,const int yz);		//从N-M曲线中取得M
	static void DrawNMPoint_forfiber(float ratio,double Iin[],const int yz);
	static void AmplifyM_forfiber(double Myed[2],double Mzed[2],double Iin[],float &kky,float &kkz);
	//根据AISC对弯矩分别按无侧移和有侧移进行放大
	static void SwayColmAmplify(float &d1,float &d2,double Vd[2],double Vv[2]);		//AISC
	static void NoSwayColmAmplify(double &Ns,double &Ned,double Med1[2],double Med2[2],float &d1,float &d2);

//	static double GetNplFromCurve(double N[],double M[]);
	static double GetMnplFromCurve(double N[],double M[],double Ned);
//	static float GetEUkk(double *dpara);		//传入一堆参数，算欧洲规范的稳定系数

	//给tips用的从坐标来查询字符
	static void GetStr(float& XCRS,float& YCRS,char* STR);
	static bool ifClose(float x1,float x2,float y1,float y2);
};

