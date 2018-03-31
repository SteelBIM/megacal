#if !defined(CHECKRST_H_MEGA_)
#define  CHECKRST_H_MEGA_
#include "atltypes.h"

struct CheckRst_ {
		bool iOK;
		bool iOK2;
		double PermitVal_upper;		//上限
		double PermitVal_lower;		//下限
		double CalVal;
		double CalVal2;		//备用参数
		int intVal[5];
		float floatVal[5];
		double doubleVal[5];
};

struct NMPoints_{
	double x_M;
	double y_N;
};
#endif
// 	struct MegaCapResults {
// 		CArray<CPoint*,CPoint*> NMPoints;		//all N-M points
// 
// 		CheckRst_ Mega_Npl;
// 		CheckRst_ Mega_Vpl;
// 	};




