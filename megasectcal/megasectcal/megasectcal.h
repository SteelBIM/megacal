// megasectcal.h : megasectcal DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CmegasectcalApp
// �йش���ʵ�ֵ���Ϣ������� megasectcal.cpp
//

class CmegasectcalApp : public CWinApp
{
public:
	CmegasectcalApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
