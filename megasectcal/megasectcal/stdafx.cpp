// stdafx.cpp : 只包括标准包含文件的源文件
// megasectcal.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

#ifdef _DEBUG
//Debug连接库
#pragma comment(lib, "Sts_SectD.lib")
#pragma comment(lib, "StsKeyDllD.lib")
//#pragma comment(lib, "Sts_DimD.lib")


//Debug忽略库
#pragma comment(linker, "/NODEFAULTLIB:msvcrtd.lib")
#pragma comment(linker, "/NODEFAULTLIB:nafxcw.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcpmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libc.lib")
#else
#pragma comment(lib, "Sts_Sect.lib")
#pragma comment(lib, "StsKeyDll.lib")
//#pragma comment(lib, "Sts_Dim.lib")

//Release忽略库
#pragma comment(linker, "/NODEFAULTLIB:msvcrt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libc.lib")



#endif
