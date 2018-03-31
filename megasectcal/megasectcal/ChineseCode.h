#pragma once
#include "megacapacitycal.h"
class ChineseCode :
	public MegaCapacityCal
{
public:
	ChineseCode(void);
	~ChineseCode(void);

	//计算流程
//	virtual void CapacityCheck();	//cap check
//	virtual void MemberCheck();	//memb check
public:
	virtual bool CalNpl(void);
	virtual bool CalVpl(void);	//中国规范没有抗剪承载力 1
	virtual bool SectDesn();
	virtual void MemberCheck();	//memb check
	virtual void CrackCheck();
	virtual bool JudgeSect();
	virtual void ReduceNMCurve(double N[][FirbePoints],double M[][FirbePoints]);

	float AmplfyM(double &M1,double &M2);
	void Line1(double N_in[FirbePoints],double N_out[FirbePoints],int yz,int nCase);		//第一条标准值折减,y=1,z=2

	double Nu;
	float Ll;
public:
	float GetFai(float Li);

public:
	//混凝土
	CheckRst_ Con_CoverCheck1;
	CheckRst_ Con_CoverCheck2;
	CheckRst_ Con_CoverCheck3;

	//	CheckRst_ Con_StrCheck;
//	CheckRst_ Con_HBCheck;	//高宽比
	//型钢
	CheckRst_ Pro_RatioCheck;	//最小含钢率
	CheckRst_ Pro_btCheck;
	CheckRst_ Pro_htCheck;
//	CheckRst_ Pro_StrCheck;
	//纵筋
	CheckRst_ Bar_RatioCheck;
	CheckRst_ Bar_SideRatioCheck;
	CheckRst_ Bar_SpaceCheck_x;		//间距检查
	CheckRst_ Bar_SpaceCheck_y;
	CheckRst_ Bar_BarDCheck;		//到型心中心间距
	//箍筋
//	CheckRst_ Tie_SpaceCheck;	//净距
//	CheckRst_ Tie_DiaCheck;
	//裂缝
	CheckRst_ Crack_Check;
};

