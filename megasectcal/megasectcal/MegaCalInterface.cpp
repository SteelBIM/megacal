#include "StdAfx.h"
#include "MegaCalInterface.h"

///////////////////////////////////////////////////////
MegaCalCmdData MegaCalCmdData::s_MegaCalCmdData;

bool PathSver::GetCurrentDirectory( CString &path )
{
	::GetCurrentDirectory(MAX_PATH,path.GetBuffer(MAX_PATH));
	path.ReleaseBuffer();
	FixPathStr(path);
	return true;
}
bool PathSver::FixPathStr(CString& strPath)
{
	strPath.Trim();//清理多与字符串
	if (true == strPath.IsEmpty())
	{
		return false;
	}

	if ((strPath.GetLength() -1) != strPath.ReverseFind(_T('\\')))
	{
		strPath += _T('\\');
	}

	return true;
}
///////////////////////////////////////////////////////
MegaCalCmdData::MegaCalCmdData(void)
{
}


MegaCalCmdData::~MegaCalCmdData(void)
{
}

CString MegaCalCmdData::MakeDrawFiName( int nLoadCase,int nType/*=EmptyType*/ )
{

	if (nLoadCase<0||nLoadCase>=LoadCase)
	{
		return "";
	}
	CString strCase = m_cmdDataForceCombin.CaseName[nLoadCase];
	switch (nType)
	{
	case eEmptyType:
		strCase.Format("%s_Empty",strCase);
		break;
	case eMy:
		strCase.Format("%s_My",strCase);
		break;
	case eMz:
		strCase.Format("%sMz",strCase);
		break;
	case eN:
		strCase.Format("%s_N",strCase);
		break;
	case eVy:
		strCase.Format("%s_Vy",strCase);
		break;
	case eVz:
		strCase.Format("%s_Vz",strCase);
		break;
	 
	}
	CString strPa;
	GetCurrentDirectory(strPa);
	
	return strPa+strCase+".T";
}

bool MegaCalCmdData::ReadForceCombinData()
{
	return m_cmdDataForceCombin.GetForce();
}

Force_combine& MegaCalCmdData::Force()
{
	return m_cmdDataForceCombin;
}

MegaCalCmdData& MegaCalCmdData::Get()
{
	return s_MegaCalCmdData;
}








