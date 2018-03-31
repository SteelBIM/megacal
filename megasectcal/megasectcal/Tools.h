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
	static int  GetCmindur_EURO(int &stru_class,int &exposure_class);	//ŷ�޹淶�л���;���Ҫ����С��������
	static void GetConFc_EURO(int ConLevel,float &fck);
	static void GetConFc_Chi(int ConLevel,float &fck);
	static void GetConFtk_Chi(int ConLevel,float &ftk);
	static void GetConPara_EURO(int fck,float &fcm,float &Ecm,float &fctm);
	static float GetProFy_EURO(int ProType,float TT,int ProNo);
	static float GetProFy_Chi(int ProType,float TT);
	static float GetProFy_AISC(int ProType);
	static float GetProFy(int ProType,float TT,int ProNo);		//1�����ͺţ�en astm  2���ֺ�
	static float GetProFy(CString Mat,float TT,int forDesign=0);	//���ַ���������͸�ǿ��
	static float GetBarFyv(int CodeType);
	static float GetConEcm_EURO(int fck);
	static float GetConEcm_Chi(int fck);
	static float GetConEcm_AISC(int fck);
	static float GetConEcm(int fck);
	static int GetProE(const int code);
	static int GetProG(const int code);
	static int GetBarE(const int code);
	static int GetEcc(int Ec);
	static float GetPos(int Pos);	//ͨ������Ż�ý���λ��
	static void Getbthtlim_chi(int &bt,int &ht);
	static double GetMFromCurve(double Ned,const int yz);		//��N-M������ȡ��M
	static void DrawNMPoint_forfiber(float ratio,double Iin[],const int yz);
	static void AmplifyM_forfiber(double Myed[2],double Mzed[2],double Iin[],float &kky,float &kkz);
	//����AISC����طֱ��޲��ƺ��в��ƽ��зŴ�
	static void SwayColmAmplify(float &d1,float &d2,double Vd[2],double Vv[2]);		//AISC
	static void NoSwayColmAmplify(double &Ns,double &Ned,double Med1[2],double Med2[2],float &d1,float &d2);

//	static double GetNplFromCurve(double N[],double M[]);
	static double GetMnplFromCurve(double N[],double M[],double Ned);
//	static float GetEUkk(double *dpara);		//����һ�Ѳ�������ŷ�޹淶���ȶ�ϵ��

	//��tips�õĴ���������ѯ�ַ�
	static void GetStr(float& XCRS,float& YCRS,char* STR);
	static bool ifClose(float x1,float x2,float y1,float y2);
};

