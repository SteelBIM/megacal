#pragma once


#include "MegaResults.h"
#include "SectProperty.h"
#include "GPara.h"

class MegaCapacityCal
{
public:
	MegaCapacityCal(void);
	~MegaCapacityCal(void);

public:
	//CArray<CPoint,CPoint> NMPoints_x;
	//CArray<CPoint,CPoint> NMPoints_y;

	CArray<NMPoints_,NMPoints_> NMPoints_x;
	CArray<NMPoints_,NMPoints_> NMPoints_y;

	CArray<NMPoints_,NMPoints_> NMPoints_x_Length;
	CArray<NMPoints_,NMPoints_> NMPoints_y_Length;

	CArray<NMPoints_,NMPoints_> NMPoints_x_nominal;
	CArray<NMPoints_,NMPoints_> NMPoints_y_nominal;

	CArray<NMPoints_,NMPoints_> NMPoints_x_nominal_Length;
	CArray<NMPoints_,NMPoints_> NMPoints_y_nominal_Length;
public:

	double Max_Mx;
	double Max_My;
	double Max_N;
	double Max_Vx;
	double Max_Vy;

public:
	bool initDesn(void);

	bool NMCurve_PDM(void);
	bool NMCurve_PDM_Nominal(void);		//名义强度

	void OutToFile(CFile &OutFile,int &j,CString String[]);

	void DrawPDMCurve(const int yz);
	float LengthEffect(const int yz);
	void DrawFiberCurve(double N[][FirbePoints],double M[][FirbePoints],const int yz);
	void DrawNMPoint(int Fiber,int yz);		//补充绘制内力点，现在只能对规则截面
	void InitUCS(float scale_x,float scale_y);
	double GetMFromN_PDM(double Ned,const int yz);		//从PDM曲线中获得M

	//计算流程
	virtual bool CalNpl(void);
	virtual bool CalVpl(void);
	virtual bool SectDesn(void);
//	virtual void CapacityCheck();	//cap check
	virtual void MemberCheck();	//memb check
	virtual void CrackCheck();		//crack check
	virtual bool JudgeSect();	//judge sect
	virtual bool JudgeSect_Fiber(double N[][FirbePoints],double M[][FirbePoints]);	//纤维模型下校核截面
	virtual void ReduceNMCurve(double N[][FirbePoints],double M[][FirbePoints]);		//NM曲线折减
	void TxtLegends(float x1, float y1, float x2, float y2, float zfb, float zfh,CString str);
	int NumPointFiberCurve(int NoCurve, int isYZ);
};

