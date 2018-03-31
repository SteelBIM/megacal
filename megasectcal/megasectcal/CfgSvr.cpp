#include "StdAfx.h"
#include "CfgSvr.h"
//#include "Regkey.h"


CfgSvrUser::CfgSvrUser(void)
{
}


CfgSvrUser::~CfgSvrUser(void)
{
}



//const TCHAR CfgSvr::_CFG_MARKER_[] = _T("CFGV31");
///////////////////////////////////////////////////////

//得到CFG路径
// CString CfgSvr::GetRegCFGpath( )
// {
// 	//找出注册表中的对应键值   键由CFG()函数提供
// 	CString CFGpath;
// 	CFGpath.Empty();
// 	if (GetAssignRegCFGpath(CFGpath,false))
// 	{
// 		return CFGpath;
// 	}
// 	else if(GetAssignRegCFGpath(CFGpath,true))
// 	{
// 		return CFGpath;
// 	}
// 
// 	return  CFGpath;
// }

//得到指定的CFG路径
// bool CfgSvr::GetAssignRegCFGpath(CString& strPath,bool bMarchine)
// {
// 	CString  CFGpath= _T("");
// 
// 	CRegKey1 regKey;
// 	HKEY hPos;
// 	if (bMarchine)
// 	{
// 		hPos = HKEY_LOCAL_MACHINE;
// 	}
// 	else
// 	{
// 		hPos = HKEY_CURRENT_USER;
// 	}
// 	long lResult = regKey.Open(hPos, _T("SOFTWARE\\PKPM\\MAIN\\PATH"));
// 	if (lResult != ERROR_SUCCESS)
// 	{
// 		lResult = regKey.Open(hPos, _T("SOFTWARE\\PKPM\\MAIN\\PATH"), KEY_EXECUTE);
// 	}
// 
// 	// Get contents of registered owner value.
// 	if (lResult == ERROR_SUCCESS)
// 	{
// 		regKey.Read(_CFG_MARKER_, CFGpath); 
// 	}
// 	regKey.Close();
// 
// 	FixPathStr(CFGpath);
// 
// 	strPath = CFGpath;
// 	return  CFGpath.IsEmpty()?false:true;
// }


//加载CFGPATH文件 到 m_mapCfgpath 中
bool CfgSvrUser::LoadFileCFGPATH()
{

	bool bSuccess = false;
	//设置CFG
	CString strP;
	if(false ==GetCfgPath(strP))
	{
		return false;
	}
	strP.Trim();
	FixPathStr(strP);
	m_mapCfgpath.SetAt(_T("CFG"),strP);
	
	CString CfgPath;
	CfgPath = strP + _T("CFGPATH"); //现在CfgPath 编程了CFGPATH文件本身
	CStdioFile	file;

	CString strItem;
	CString strItemPath;
	strItem.Empty();
	CString rString;//用来遍历cfgpath文件中的每一行
	bool bFind = false;
	if (file.Open(CfgPath,CFile::modeRead|CFile::shareDenyNone ))
	{
		bSuccess = true;
		while (file.ReadString(rString))
		{
			rString.MakeUpper();
			rString.Trim();
			if(rString.IsEmpty())
			{
				continue;
			}

			if (rString.FindOneOf(_T(":\\"))<0)
			{
				strItem = rString;
				file.ReadString(rString);
				if (rString.FindOneOf(_T(":\\"))<0)
				{
					continue;
				}
				strItemPath = rString;
				FixPathStr(strItemPath);
				m_mapCfgpath.SetAt(strItem,strItemPath);

			}

		}
		file.Close();
	}

	return bSuccess;


}
bool CfgSvrUser::FixPathStr(CString& strPath)
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

// CString CfgSvr::CFG()
// {
// 	return _CFG_MARKER_;
// }
bool CfgSvrUser::GetPathByMarker(CString maker,CString& path)
{
	maker.MakeUpper();
	if (m_mapCfgpath.GetSize()<=0)
	{
		LoadFileCFGPATH();
	}

	CString pa;
	if(m_mapCfgpath.Lookup(maker ,pa))
	{
		path = pa;
		return true;
	}

	return false;
}
bool CfgSvrUser::GetPmPath(CString& strPm)
{
	if (m_mapCfgpath.GetSize()<=0)
	{
		LoadFileCFGPATH();
	}

	CString pa;
	if(m_mapCfgpath.Lookup(_T("PM"),pa))
	{
		 strPm = pa;
		 return true;
	}
	 
	return false;
}
bool CfgSvrUser::GetCfgPath(CString& strCfg)
{
	CString pa = EnvPath();
	pa.MakeUpper();
	int nStart = 0;
	int nEnd = nStart;
	CString onePath;
	onePath.MakeUpper();
	do
	{
		nEnd = pa.Find(';',nStart);
		if (nEnd<=0)
		{
			break;//没有找到分号 直接退出
		}

		onePath = pa.Mid(nStart,nEnd - nStart);

		//已经找到退出循环
		if (onePath.Find("CFG")>=0)
		{
			break;
		}

		//没有找到
		nStart = nEnd+1;
	}while (nStart <= pa.GetLength()-1);


	strCfg =  onePath;

	if (strCfg.IsEmpty())
	{
		return false;
	}

	FixPathStr(strCfg);

	return true;
}

CString CfgSvrUser::EnvPath()
{
	TCHAR *szOriEnvPath = new TCHAR [4096];

	DWORD dwRet = ::GetEnvironmentVariable(_T("PATH"),szOriEnvPath,4069);
	if(4096 < dwRet)
	{
		delete[] szOriEnvPath;
		szOriEnvPath = new TCHAR[dwRet + 1];

		DWORD dwRetNewAlloc;
		dwRetNewAlloc = ::GetEnvironmentVariable(_T("PATH"),szOriEnvPath,dwRet);
	}
	CString PA = szOriEnvPath;
	delete[] szOriEnvPath;
	return PA;
}

const TCHAR CfgSvrUser::_EXE_FOLDER_[] = _T("$EXE_FOLDER$");

int CfgSvrUser::StrPathType(CString str,bool &hasSub)
{
	int nI = str.Find(_T('$'));
	int nII = str.ReverseFind(_T('$'));

	if (str.Find(':')>0)
	{
		hasSub = true;
		return PATYPE_FULL_PATH;
	}

	if (nII - nI>=2)
	{
		CString strMiddle = str.Mid(nI+1,nII-nI-1);
		CString strExeFolder = _EXE_FOLDER_;
		strExeFolder = strExeFolder.Trim("$");

		if (0 == strMiddle.CompareNoCase(strExeFolder))
		{
			if(str.GetLength()-1>nII)
			{
				hasSub = true;
			}
			else
			{
				hasSub = false;
			}
			return PATYPE_EXE_FOLDER_PATH;
		}
		else
		{
			if(str.GetLength()-1>nII)
			{
				hasSub = true;
			}
			else
			{
				hasSub = false;
			}
			return PATYPE_DOLLAR_MAKER_PATH;
		}
	}

	str.Trim('\\');

	{
		hasSub = true;
		return PATYPE_RELATIVE_PATH;
	}
	
}
bool CfgSvrUser::TransferPath(int type,bool hasSub,CString str,CString& strPath,bool bOutputPath )
{
	if (PATYPE_FULL_PATH == type)
	{
		strPath = str;
		return true;
	}

	int nI = str.Find(_T('$'));
	int nII = str.ReverseFind(_T('$'));

	if (PATYPE_EXE_FOLDER_PATH == type)
	{
		CString strExeFileName;
		GetModuleFileName(NULL,strExeFileName.GetBuffer(MAX_PATH),MAX_PATH);//本程序就是exe
		strExeFileName.ReleaseBuffer(-1);

		int nSpExe = strExeFileName.ReverseFind('\\');	
		strPath = strExeFileName.Left(nSpExe);
		
		if (true == hasSub)
		{
			strPath += str.Mid((nII+1)+1);//跳过 $\ 这两个字符
			if (bOutputPath)
			{
				CfgSvrUser::FixPathStr(strPath);
			}
			else
			{
				strPath.TrimRight('\\');
			}
		}
		else
		{
			CfgSvrUser::FixPathStr(strPath);
		}

		return true;
	}

	if (PATYPE_DOLLAR_MAKER_PATH == type)
	{
		CString strMiddle = str.Mid(nI+1,nII-nI-1);
		CfgSvrUser::GetPathByMarker(strMiddle,strPath);
		if (hasSub)
		{
			strPath = strPath + str.Mid((nII+1)+1);//跳过 $\ 这两个字符
			if (bOutputPath)
			{
				CfgSvrUser::FixPathStr(strPath);
				if (bOutputPath)
				{
					CfgSvrUser::FixPathStr(strPath);
				}
				else
				{
					strPath.TrimRight('\\');
				}
			}
		}
		return true;
	}

	if (PATYPE_RELATIVE_PATH == type)
	{
		CString strExeFileName;
		GetModuleFileName(NULL,strExeFileName.GetBuffer(MAX_PATH),MAX_PATH);//本程序就是exe
		strExeFileName.ReleaseBuffer(-1);

		int nSpExe = strExeFileName.ReverseFind('\\');	
		strPath = strExeFileName.Left(nSpExe);
		FixPathStr(strPath);
	
		strPath += str;
		if (bOutputPath)
		{
			CfgSvrUser::FixPathStr(strPath);
			if (bOutputPath)
			{
				CfgSvrUser::FixPathStr(strPath);
			}
			else
			{
				strPath.TrimRight('\\');
			}
		}

		return true;

	}
	return false;
}

bool CfgSvrUser::GetExePath(CString& path)
{
	CString strExeFileName;
	GetModuleFileName(NULL,strExeFileName.GetBuffer(MAX_PATH),MAX_PATH);//本程序就是exe
	strExeFileName.ReleaseBuffer(-1);
	int nsp = strExeFileName.ReverseFind('\\');
	path = strExeFileName.Left(nsp);
	FixPathStr(path);

	return true;
}

bool CfgSvrUser::GetCurrentDirectory( CString &path )
{
	::GetCurrentDirectory(MAX_PATH,path.GetBuffer(MAX_PATH));
	path.ReleaseBuffer();
	FixPathStr(path);
	return true;
}

CMapStringToString CfgSvrUser::m_mapCfgpath;
