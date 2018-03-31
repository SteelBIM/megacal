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
//	{   ������Ϣ
	CString Notatio;
	CString Description;
	float f0,f1,f2;
//	}
	


//{ ������
	std::vector<ConLoad> ConYY;
	std::vector<ConLoad> ConZZ;
//} 

// { �ֲ�����
	std::vector<DisLoad> DisYY;
	std::vector<DisLoad> DisZZ;

//

// { �˲�����
	EndLoad EndmYY;
	EndLoad EndmZZ;
// }
	
   float AnxialForceYY;
   float AnxialForceZZ;
//ZHLEI 2017/11 :��������͸�ֵ

	mega_loads();
	//mega_loads(const mega_loads &ld);
	mega_loads& operator = (const mega_loads &ld);


	void fromData();
	 
};





///////////////////////////////////////////////////////
///�������ڲ���

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



