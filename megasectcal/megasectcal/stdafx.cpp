// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// megasectcal.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

#ifdef _DEBUG
//Debug���ӿ�
#pragma comment(lib, "Sts_SectD.lib")
#pragma comment(lib, "StsKeyDllD.lib")
//#pragma comment(lib, "Sts_DimD.lib")


//Debug���Կ�
#pragma comment(linker, "/NODEFAULTLIB:msvcrtd.lib")
#pragma comment(linker, "/NODEFAULTLIB:nafxcw.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcpmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libc.lib")
#else
#pragma comment(lib, "Sts_Sect.lib")
#pragma comment(lib, "StsKeyDll.lib")
//#pragma comment(lib, "Sts_Dim.lib")

//Release���Կ�
#pragma comment(linker, "/NODEFAULTLIB:msvcrt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libc.lib")



#endif
