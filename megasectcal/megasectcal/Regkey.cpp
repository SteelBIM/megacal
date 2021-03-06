#include "stdafx.h"
#include "Regkey.h"

CRegKey1::CRegKey1()
{
	m_hKey = NULL;
}

CRegKey1::~CRegKey1()
{
	Close();
}

LONG CRegKey1::Open (HKEY hKeyRoot, LPCTSTR pszPath, REGSAM samDesired)
{
	DWORD dw;
	m_sPath = pszPath;

	return RegCreateKeyEx (hKeyRoot, pszPath, 0L, NULL,
//		REG_OPTION_VOLATILE, samDesired, NULL, 
		REG_OPTION_NON_VOLATILE, samDesired, NULL, 
		&m_hKey, &dw);

}

void CRegKey1::Close()
{
	if (m_hKey)
	{
		RegCloseKey (m_hKey);
		m_hKey = NULL;
	}
}

LONG CRegKey1::Write (LPCTSTR pszKey, DWORD dwVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	return RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwVal, sizeof(DWORD));
}

LONG CRegKey1::Write (LPCTSTR pszKey, LPCTSTR pszData)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	ASSERT(pszData);
	ASSERT(AfxIsValidAddress(pszData, _tcslen(pszData), FALSE));

	return RegSetValueEx (m_hKey, pszKey, 0L, REG_SZ,
		(CONST BYTE*) pszData, _tcslen(pszData) + 1);
}

LONG CRegKey1::Write (LPCTSTR pszKey, const BYTE* pData,
	DWORD dwLength)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	ASSERT(pData && dwLength > 0);
	ASSERT(AfxIsValidAddress(pData, dwLength, FALSE));

	return RegSetValueEx (m_hKey, pszKey, 0L, REG_BINARY,
		pData, dwLength);
}

LONG CRegKey1::Read (LPCTSTR pszKey, DWORD& dwVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lRet = RegQueryValueEx (m_hKey, pszKey, NULL, 
		&dwType, (BYTE *) &dwDest, &dwSize);

	if (lRet == ERROR_SUCCESS)
		dwVal = dwDest;

	return lRet;
}

LONG CRegKey1::Read (LPCTSTR pszKey, CString& sVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = 200;
	char  string[200];

	LONG lReturn = RegQueryValueEx (m_hKey,  pszKey, NULL,
		&dwType, (BYTE *) string, &dwSize);

	if (lReturn == ERROR_SUCCESS)
		sVal = string;

	return lReturn;
}

LONG CRegKey1::Read (LPCTSTR pszKey, BYTE* pData, DWORD& dwLen)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;

	return RegQueryValueEx (m_hKey, pszKey, NULL,&dwType, pData, &dwLen);
}


 