#ifndef UPPER_CPP_INTERFACE_CONTRAL
#define UPPER_CPP_INTERFACE_CONTRAL

//#define CalcOutWord WORDOUT_CALCREPROT
#define Main_Param MAIN_PARAM
#define Init WORDOUT_INIT 
#endif //UPPER_CPP_INTERFACE_CONTRAL

extern "C"    
{
	extern void CalcOutWord();

	extern void Main_Param();

	extern void Init();
};


#define AdjustTxtOutOrder TXTOUT_ADJUSTTXTOUTORDER
extern "C"
{
	void __stdcall AdjustTxtOutOrder();
}
