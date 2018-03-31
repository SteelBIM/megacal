#pragma once
#include "Force_combine.h"

enum 
{
	eEmptyType=0,
	eMy=1,
	eMz, 	
	eVy,
	eVz,
	eN
};

class PathSver
{
public:
	static bool GetCurrentDirectory( CString &path );
	static bool FixPathStr(CString& strPath);
};
class MegaCalCmdData:public PathSver
{
public:
	static MegaCalCmdData s_MegaCalCmdData;
	static MegaCalCmdData& Get();
private:
	int m_hasRead;
	Force_combine m_cmdDataForceCombin;
public:
	MegaCalCmdData(void);
	~MegaCalCmdData(void);
	
public:
	bool ReadForceCombinData();
	CString MakeDrawFiName(int nLoadCase,int nType= eEmptyType);
	Force_combine& Force();

};

