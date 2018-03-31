#pragma once
#include "GPara.h"

class CMegaDrawingImp
{
	//ZHLEI 2017/11 :单例模式
protected:
	static CMegaDrawingImp s_MegaDrawingImp;
public:
	static CMegaDrawingImp& Get();

	//ZHLEI 2017/11 :构造
public:
	CMegaDrawingImp(void);
	~CMegaDrawingImp(void);

private:
	float m_MDrawSc,m_VDrawSc,m_NDrawSc;
	float m_hcol;
	float m_enLarege;
	float m_drawStickLen;
	double m_MockF[Combine];
public:
	void DrawCol(int nLoadCase,int nType);

private:
	void CalcMVNDrawSc();
	void DrawZhizuo(float x0,float y0,bool bUp,double bl=1.0);
	void DrawZhizuoXY(float x0,float y0,float x1,float y1,double bl=1.0);	
	void MyUserCs2d(float X0,float Y0,float ang,float BL);
	void MyUserCs2DEnlarge(float X0,float Y0,float ang,float Sc);
	void MyUserCs2DExit();

	void Rot_Coord_LtoG(float angle, float x1,float y1,float& x2,float& y2);
	void Rot_Coord_GtoL(float angle, float x1,float y1,float& x2,float& y2);
	void COLM(float x1,float y1,float len);
	void COLM13(float x1,float y1,int nLoadCase,int nType);
	void ZFANG(float ANG1,float ANG2);

	double* MockForce(int nLoadCase,int nType);
};

