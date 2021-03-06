#include "winreg.h"

class CRegKey1
{
// Construction
public:
	CRegKey1();
	virtual ~CRegKey1();

// Operations
public:
	LONG Open (HKEY hKeyRoot, LPCTSTR pszPath, REGSAM samDesired = KEY_ALL_ACCESS );
	void Close();

	LONG Write (LPCTSTR pszKey, DWORD dwVal);
	LONG Write (LPCTSTR pszKey, LPCTSTR pszVal);
	LONG Write (LPCTSTR pszKey, const BYTE* pData, DWORD dwLength);

	LONG Read (LPCTSTR pszKey, DWORD& dwVal);
	LONG Read (LPCTSTR pszKey, CString& sVal);
	LONG Read (LPCTSTR pszKey, BYTE* pData, DWORD& dwLength);

protected:
	HKEY 	m_hKey;
	CString m_sPath;
};
