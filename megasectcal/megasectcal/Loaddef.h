#include "stdafx.h"
#include "vector"
#ifndef MEGA_LOADS_HEADER_CAL
#define MEGA_LOADS_HEADER_CAL
 class __declspec(dllexport) ConLoad
{
public:
	float x;
	float F;

	ConLoad();
	ConLoad(const ConLoad& ll);
};
class  __declspec(dllexport)  DisLoad
{
public:
	float x1;
	float q1;
	float x2;
	float q2;

	DisLoad();
	DisLoad(const DisLoad& ll);
};
class  __declspec(dllexport)  EndLoad
{
public:
	float Bottom;
	float Top;
	EndLoad();
	EndLoad(const EndLoad& ll);
};





///////////////////////////////////////////////////////
struct  mega_loads
{
public:
//	{   工况信息
	CString Notatio;
	CString Description;
	float f0,f1,f2;
//	}
	


//{ 集中力
	std::vector<ConLoad> ConYY;
	std::vector<ConLoad> ConZZ;
//} 

// { 分布荷载
	std::vector<DisLoad> DisYY;
	std::vector<DisLoad> DisZZ;

//

// { 端部荷载
	EndLoad EndmYY;
	EndLoad EndmZZ;
// }
	
   float AnxialForceYY;
   float AnxialForceZZ;
//ZHLEI 2017/11 :拷贝构造和赋值

	mega_loads();
	//mega_loads(const mega_loads &ld);
	mega_loads& operator = (const mega_loads &ld);


	void fromData();
	 
};





///////////////////////////////////////////////////////
///以下是内部类

class Mega_CasesCal
{
private:
	static Mega_CasesCal s_Mega_Cases;
private:
	std::vector<mega_loads> m_Cases;
public:
	static Mega_CasesCal& Get();
public:
	std::vector<mega_loads>& Cases();
	mega_loads& CaseAt(int idx);
	void SetCases(const std::vector<mega_loads>& cs);
};

#endif



