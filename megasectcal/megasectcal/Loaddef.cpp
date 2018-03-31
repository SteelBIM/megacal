#include "stdafx.h"
#include "Loaddef.h"

mega_loads::mega_loads()
{
	Notatio="";
	Description="";
	f0=0.f;
	f1=0.f;
	f2=0.f;
 	ConYY.clear();
 	ConZZ.clear();
 	DisYY.clear();
 	DisZZ.clear();
	
	AnxialForceYY=0;
	AnxialForceZZ=0;
}

// mega_loads::mega_loads( const mega_loads &ld )
// {
// 	//*this = ld;
// 	*this = ld;
// }

mega_loads& mega_loads::operator=( const mega_loads& ld )
{
	Notatio = ld.Notatio;
	Description = ld.Description;
	f0 = ld.f0;
	f1 = ld.f1;
	f2 = ld.f2;

	ConYY.assign(ld.ConYY.begin(),ld.ConYY.end());
	ConZZ.assign(ld.ConZZ.begin(),ld.ConZZ.end());
 
	DisYY.assign(ld.DisYY.begin(),ld.DisYY.end());
	DisZZ.assign(ld.DisZZ.begin(),ld.DisZZ.end());
// 	for (int i=0;i<ld.ConYY.GetSize();i++)
// 	{
// 		ConYY.Add(ld.ConYY[i]);
// 	}
// 	for (int i=0;i<ld.ConZZ.GetSize();i++)
// 	{
// 		ConZZ.Add(ld.ConZZ[i]);
// 	}
// 
// 	for (int i=0;i<ld.DisYY.GetSize();i++)
// 	{
// 		DisYY.Add(ld.DisYY[i]);
// 	}
// 	for (int i=0;i<ld.DisZZ.GetSize();i++)
// 	{
// 		DisZZ.Add(ld.DisZZ[i]);
// 	}

	EndmYY = ld.EndmYY;
	EndmZZ = ld.EndmZZ;


	AnxialForceYY = ld.AnxialForceYY;
	AnxialForceZZ = ld.AnxialForceZZ;

	return *this;
}

void mega_loads::fromData()
{
	 
}


ConLoad::ConLoad()
{
	x = 0.f;
	F =0.f;
}

ConLoad::ConLoad( const ConLoad& ll )
{
	x = ll.x;
	F = ll.F;
}

DisLoad::DisLoad()
{
	x1=0.f;
	q1=0.f;
	x2=0.f;
	q2=0.f;
}

DisLoad::DisLoad( const DisLoad& ll )
{
	x1 = ll.x1;
	q1 = ll.q1;
	x2 = ll.x2;
	q2 = ll.q2;
}

EndLoad::EndLoad()
{
	Bottom=0.f;
	Top=0.f;
}

EndLoad::EndLoad( const EndLoad& ll )
{
	Bottom = ll.Bottom;
	Top = ll.Top;
}


///////////////////////////////////////////////////////
Mega_CasesCal Mega_CasesCal::s_Mega_Cases;

Mega_CasesCal& Mega_CasesCal::Get()
{
	return s_Mega_Cases;
}

std::vector<mega_loads>& Mega_CasesCal::Cases()
{
	return m_Cases;
}

mega_loads& Mega_CasesCal::CaseAt( int idx )
{
	return Cases()[idx];
}

void Mega_CasesCal::SetCases( const std::vector<mega_loads>& cs )
{
	Cases().clear();
	Cases().assign(cs.begin(),cs.end());
	
}



CString Notatio;
CString Description;
float f0,f1,f2;

std::vector<ConLoad> ConYY;
std::vector<ConLoad> ConZZ;
//} 

// { ∑÷≤º∫…‘ÿ
std::vector<DisLoad> DisYY;
std::vector<DisLoad> DisZZ;

//

// { ∂À≤ø∫…‘ÿ
EndLoad EndmYY;
EndLoad EndmZZ;
// }

float AnxialForceYY;
float AnxialForceZZ;

extern "C" __declspec( dllexport ) void MegaCal_LoadCaseSet(CString Notatio, CString Description,float f0,float f1,float f2,
	const ConLoad* ConYY,int nConYY, const ConLoad* ConZZ,int nConZZ,
	const DisLoad* DisYY,int nDisYY,const DisLoad* DisZZ,int nDisZZ,
	const  EndLoad* EndmYY,const  EndLoad* EndmZZ,
	float AnxialForceYY,float AnxialForceZZ)
{

	mega_loads ld;
	ld.Notatio = Notatio;
	ld.Description = Description;
	ld.f0 = f0;
	ld.f1 = f1;
	ld.f2 = f2;
	ld.ConYY.assign(ConYY,ConYY+nConYY);
	ld.ConZZ.assign(ConZZ,ConZZ+nConZZ);

	ld.DisYY.assign(DisYY,DisYY+nDisYY);
	ld.DisZZ.assign(DisZZ,DisZZ+nDisZZ);

	ld.EndmYY = *EndmYY;
	ld.EndmZZ = *EndmZZ;
	ld.AnxialForceYY = AnxialForceYY;
	ld.AnxialForceZZ = AnxialForceZZ;

	Mega_CasesCal::Get().Cases().push_back(ld);
	

}

extern "C" __declspec( dllexport ) void MegaCal_LoadCaseInit()
{
	Mega_CasesCal::Get().Cases().clear();
}