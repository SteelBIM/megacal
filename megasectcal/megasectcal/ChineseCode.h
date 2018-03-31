#pragma once
#include "megacapacitycal.h"
class ChineseCode :
	public MegaCapacityCal
{
public:
	ChineseCode(void);
	~ChineseCode(void);

	//��������
//	virtual void CapacityCheck();	//cap check
//	virtual void MemberCheck();	//memb check
public:
	virtual bool CalNpl(void);
	virtual bool CalVpl(void);	//�й��淶û�п��������� 1
	virtual bool SectDesn();
	virtual void MemberCheck();	//memb check
	virtual void CrackCheck();
	virtual bool JudgeSect();
	virtual void ReduceNMCurve(double N[][FirbePoints],double M[][FirbePoints]);

	float AmplfyM(double &M1,double &M2);
	void Line1(double N_in[FirbePoints],double N_out[FirbePoints],int yz,int nCase);		//��һ����׼ֵ�ۼ�,y=1,z=2

	double Nu;
	float Ll;
public:
	float GetFai(float Li);

public:
	//������
	CheckRst_ Con_CoverCheck1;
	CheckRst_ Con_CoverCheck2;
	CheckRst_ Con_CoverCheck3;

	//	CheckRst_ Con_StrCheck;
//	CheckRst_ Con_HBCheck;	//�߿��
	//�͸�
	CheckRst_ Pro_RatioCheck;	//��С������
	CheckRst_ Pro_btCheck;
	CheckRst_ Pro_htCheck;
//	CheckRst_ Pro_StrCheck;
	//�ݽ�
	CheckRst_ Bar_RatioCheck;
	CheckRst_ Bar_SideRatioCheck;
	CheckRst_ Bar_SpaceCheck_x;		//�����
	CheckRst_ Bar_SpaceCheck_y;
	CheckRst_ Bar_BarDCheck;		//���������ļ��
	//����
//	CheckRst_ Tie_SpaceCheck;	//����
//	CheckRst_ Tie_DiaCheck;
	//�ѷ�
	CheckRst_ Crack_Check;
};

