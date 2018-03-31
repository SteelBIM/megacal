#pragma once
#include "megacapacitycal.h"
#include "MegaResults.h"
class AISC :
	public MegaCapacityCal
{
public:
	AISC(void);
	~AISC(void);

	//计算流程
//	virtual void CapacityCheck();	//cap check
	virtual bool CalNpl(void);
	virtual bool CalVpl(void);
	virtual bool SectDesn();
	virtual void MemberCheck();	//memb check
	virtual void CrackCheck();
	virtual bool JudgeSect();
	virtual void ReduceNMCurve(double N[][FirbePoints],double M[][FirbePoints]);

public:
	void AmplfyN();
	void Line1(double N_in[FirbePoints],double N_out[FirbePoints],int yz,int nCase);		//第一条标准值折减,y=1,z=2

public:
	double Pn0;
	double NbplRd_x,NbplRd_y;
	double Mbed_x,Mbed_y;

public:
	//混凝土
	CheckRst_ Con_CoverCheck;
	CheckRst_ Con_StrCheck;
	//型钢
	CheckRst_ Pro_RatioCheck;	//最小含钢率
	CheckRst_ Pro_StrCheck;
	//纵筋
	CheckRst_ Bar_RatioCheck;
	CheckRst_ Bar_SpaceCheck;		//间距检查
	CheckRst_ Bar_CoreCheck;		//到型心中心间距
	CheckRst_ Bar_StrCheck;
	//箍筋
	CheckRst_ Tie_AvCheck;
	CheckRst_ Tie_StrCheck;
	CheckRst_ Tie_CSSpaceCheck;	//净距
	CheckRst_ Tie_CCSpaceCheck;	//中心距
	CheckRst_ Tie_DiaCheck1;
	CheckRst_ Tie_DiaCheck2;
	//裂缝
	CheckRst_ Crack_Check1;
	CheckRst_ Crack_Check2;

	//截面评价
	//截面评价
	CheckRst_ AixalForce_Check;
	CheckRst_ ShearForcevy_Check;
	CheckRst_ ShearForcevz_Check;
	CheckRst_ MNV_Check;
	CheckRst_ BucklingNby_Check;
	CheckRst_ BucklingNbz_Check;


};

