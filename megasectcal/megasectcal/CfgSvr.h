#pragma once
enum
{
	PATYPE_UNKNOWN = 0,
	PATYPE_FULL_PATH = 1, // 全路径
	PATYPE_EXE_FOLDER_PATH = 2,// EXE 路径
	PATYPE_DOLLAR_MAKER_PATH = 3,
	PATYPE_RELATIVE_PATH = 4
};

class CfgSvrUser
{
private:
	//static const TCHAR _CFG_MARKER_[];
	static const TCHAR _EXE_FOLDER_[];
public:

	

	static CMapStringToString m_mapCfgpath;
	CfgSvrUser(void);
	~CfgSvrUser(void);
	//static CString GetRegCFGpath( );
	static bool GetAssignRegCFGpath(CString& strPath,bool bMarchine);
	static bool LoadFileCFGPATH();

	static bool FixPathStr(CString& strPath);
	//static CString CFG();

	static bool GetPathByMarker(CString maker,CString& path);
	//特别:
	static bool GetPmPath(CString& strPm);
	static bool GetCfgPath(CString& strCfg);

	static CString EnvPath();
	static int StrPathType(CString str,bool &hasSub);
	static bool TransferPath(int type,bool hasSub,CString str,CString& strPath,bool bPath = true);
 
	static bool GetExePath(CString& path);
	static bool GetCurrentDirectory(CString &path);
};

