#pragma once
#include "GPara.h"
#include "PredefSectDescrib.h"
#include "DesignParameter.h"
#include "Force_combine.h"

class MegaCalIO
{
public:
	MegaCalIO(void);
	~MegaCalIO(void);

public:
	static void MegaCheckIO(SectDescribe *pSect,DesnPara *pDesnPara);
	static void InitMegaCheckIO(SectDescribe *pSect,DesnPara *pDesnPara);
	static void MegaDesn(DesnPara *pDesnPara,double M[][FirbePoints],double N[][FirbePoints]);
	static void DrawNMPoints();		//给任意截面用的画内力点的函数
	static void ExitMega();
};

