#pragma once
#include "GPara.h"
class Force_combine
{
public:
	Force_combine(void);
	~Force_combine(void);

public:
	//各工况13个断面内力
	double My[LoadCase][Parts];
	double Mz[LoadCase][Parts];
	double N[LoadCase][Parts];
	double Vy[LoadCase][Parts];
	double Vz[LoadCase][Parts];
	
	double My_zuhe[MaxCombine][Parts];
	double Mz_zuhe[MaxCombine][Parts];
	double N_zuhe[MaxCombine][Parts];
	double Vy_zuhe[MaxCombine][Parts];
	double Vz_zuhe[MaxCombine][Parts];
	CString CaseName[LoadCase];

	double My_max_bz;
	double Mz_max_bz;
	double N_max_bz;
	double Mmix_max_bz;

public:
	bool GetForce();
	void Euro_combine();
	void AISC_combine();
	void Chinese_combine();
	void BiaoZhun_combine();
	void HeZai_combine();
//	void HeZai_combine();
};

