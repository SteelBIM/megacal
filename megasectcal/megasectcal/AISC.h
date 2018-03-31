#pragma once
#include "megacapacitycal.h"
#include "MegaResults.h"
class AISC :
	public MegaCapacityCal
{
public:
	AISC(void);
	~AISC(void);

	//��������
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
	void Line1(double N_in[FirbePoints],double N_out[FirbePoints],int yz,int nCase);		//��һ����׼ֵ�ۼ�,y=1,z=2

public:
	double Pn0;
	double NbplRd_x,NbplRd_y;
	double Mbed_x,Mbed_y;

public:
	//������
	CheckRst_ Con_CoverCheck;
	CheckRst_ Con_StrCheck;
	//�͸�
	CheckRst_ Pro_RatioCheck;	//��С������
	CheckRst_ Pro_StrCheck;
	//�ݽ�
	CheckRst_ Bar_RatioCheck;
	CheckRst_ Bar_SpaceCheck;		//�����
	CheckRst_ Bar_CoreCheck;		//���������ļ��
	CheckRst_ Bar_StrCheck;
	//����
	CheckRst_ Tie_AvCheck;
	CheckRst_ Tie_StrCheck;
	CheckRst_ Tie_CSSpaceCheck;	//����
	CheckRst_ Tie_CCSpaceCheck;	//���ľ�
	CheckRst_ Tie_DiaCheck1;
	CheckRst_ Tie_DiaCheck2;
	//�ѷ�
	CheckRst_ Crack_Check1;
	CheckRst_ Crack_Check2;

	//��������
	//��������
	CheckRst_ AixalForce_Check;
	CheckRst_ ShearForcevy_Check;
	CheckRst_ ShearForcevz_Check;
	CheckRst_ MNV_Check;
	CheckRst_ BucklingNby_Check;
	CheckRst_ BucklingNbz_Check;


};

