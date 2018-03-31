#pragma once
#include "megacapacitycal.h"
class EUROCode :
	public MegaCapacityCal
{
public:
	EUROCode(void);
	~EUROCode(void);

	//��������
//	virtual void CapacityCheck();	//cap check
	virtual bool CalNpl(void);
	virtual bool CalVpl(void);
	virtual bool SectDesn();
	virtual void MemberCheck();	//memb check
	virtual void CrackCheck();
	virtual bool JudgeSect();
	virtual bool JudgeSect_Fiber(double N[][FirbePoints],double M[][FirbePoints]);	//��άģ������
	virtual void ReduceNMCurve(double N[][FirbePoints],double M[][FirbePoints]);

	static float GetCreepEc();
	static void AmplifyM(double Myed[2],double Mzed[2],float &kky,float &kkz,int nLoadcase);
	static void AmplifyM_Fiber(double Myed[2],double Mzed[2],float &kky,float &kkz,int nLoadcase);
	void AmplifyN();
	void Line1(double N_in[FirbePoints],double N_out[FirbePoints],const int yz,int nCase);		//��һ����׼ֵ�ۼ�,y=1,z=2

public:



private:
	void WriteRst();
	
};

