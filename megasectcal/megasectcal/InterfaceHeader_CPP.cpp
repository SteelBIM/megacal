#include "stdafx.h"
#include "InterfaceHeader_CPP.h"
#include "Pk_MainOutPut.h"

//#include "Param_MainParam.h"
#include <math.h>
//#include "..\OptLib\CmdDataMgr.h"
//#include "..\OptLib\PluginCmd.h"
#include "ConfigGen.h"


// extern "C" 
// {
//  
// 	void __stdcall REFPRT(char NOTE[], int n2 );
// }

bool ProgressSetT(int np)
{
	CString strProgress;
	strProgress.Format("���ڼ���ͼֽ�߽� %3d%%",np);
	//REFPRT(strProgress.GetBuffer(),strProgress.GetLength());
	strProgress.ReleaseBuffer();

	//PluginCmd::BigProgress(np);

	return true;
}

void SetConfigT(CConfigGenCalc& conf)
{
	conf.setPathT();
	conf.addFiberNM_T(_T("Fiber_NM_y.t"),  _T("Fiber N-M(y)"));
	conf.addFiberNM_T(_T("Fiber_NM_z.t"),  _T("Fiber N-M(z)"));
	conf.addPDMNM_T(_T("PDM_NM_y.t"),  _T("PDM N-M(y)"));
	conf.addPDMNM_T(_T("PDM_NM_z.t"),  _T("PDM N-M(z)"));
	//�����ǳ���ЧӦ��ͼ
	conf.addFiberNM_T2(_T("Fiber_NM_y_nominal.t"),  _T("Fiber N-M(y)(without length effect)"));
	conf.addFiberNM_T2(_T("Fiber_NM_z_nominal.t"),  _T("Fiber N-M(z)(without length effect)"));
	conf.addPDMNM_T2(_T("PDM_NM_y_design.t"),  _T("PDM N-M(y)(without length effect)"));
	conf.addPDMNM_T2(_T("PDM_NM_z_design.t"),  _T("PDM N-M(z)(without length effect)"));
	conf.addSection_T(_T("temptemp.t"), _T("Section"));
// 	conf.setPathT();
// 
// 	//PluginCmd::ShowBigProgress();
// 
// 	ProgressSetT(0);
// 	 
// 	conf.addViewT(_T("�ռܼ�ͼ.T"),  _T("�ռܼ�ͼ"));//KLM-CAL.T
// 
// 	conf.addLoadsT(_T("���ؼ�ͼ.T"),  _T("���ؼ�ͼ"));//D-L-CAL.T
// 	if(StruCalcInfo().IsHasDiaoGua())
// 	{
// 		conf.addLoadsT(_T("���Һ��ؼ�ͼ.T"),  _T("���Һ��ؼ�ͼ"));//DG-D-L-CAL.T
// 	}
// 	ProgressSetT(10);
// 	if (1 == g_Param_MainParam.KLL )
// 	{
// 		
// 		conf.addLoadsT(_T("���ؼ�ͼ.T"), _T("���ؼ�ͼ"));//L-L-CAL.T
// 	}
// 	else if (g_Param_MainParam.KLL >1)
// 	{
// 		CString strfile;
// 		CString strtitle;
// 		for (int i=0;i<g_Param_MainParam.KLL;i++)
// 		{
// 			//strfile.Format(_T("��%d�黥�����ؼ�ͼ.T"),i+1);//L-L%d-CAL.T
// 			if (0 == i)
// 			{
// 				strfile = _T("���ؼ�ͼ.T");
// 				strtitle = _T("���ؼ�ͼ");
// 			}
// 			else
// 			{
// 				strfile.Format(_T("��%d�黥�����ؼ�ͼ.T"),i);
// 				strtitle.Format(_T("��%d�黥�����ؼ�ͼ"),i);
// 			}
// 			conf.addLoadsT(strfile, strtitle);
// 		}
// 		
// 	}
// 
// 	//�Զ�����ؼ�ͼ
// 	//�Զ�����ؼ�ͼ
// 	{
// 		int UserZiDingYi = CmdDataMgr::Get().NZiDingYi();
// 		const TCHAR * formatUserCalcTmpFile = "%s���ؼ�ͼ.T";
// 		const TCHAR * formatUserCalcTmpTitle= "%s���ؼ�ͼ";
// 		CString strUserLCalcTmpFile;
// 		CString strUserLCalcTmpTitle;
// 		if (UserZiDingYi>0)
// 		{
// 			for (int i=0;i<UserZiDingYi;i++)
// 			{
// 				strUserLCalcTmpFile.Format(formatUserCalcTmpFile,CmdDataMgr::Get().NameZiDingYi(i));
// 				strUserLCalcTmpTitle.Format(formatUserCalcTmpTitle,CmdDataMgr::Get().NameZiDingYi(i));
// 				conf.addLoadsT(strUserLCalcTmpFile, strUserLCalcTmpTitle);
// 			}
// 		}
// 	}
// 	
// 
// 	ProgressSetT(20);
// 
// 	if (StruCalcInfo().nWindP > 0)
// 	{
// 		conf.addLoadsT(_T("���1��ͼ.T"), _T("���1��ͼ"));//L-W-1-CAL.T
// 		conf.addLoadsT(_T("�ҷ�1��ͼ.T") , _T("�ҷ�1��ͼ"));//R-W-1-CAL.T
// 	}
// 	ProgressSetT(25);
// 	if (StruCalcInfo().nWindP > 1)
// 	{
// 		conf.addLoadsT(_T("���2��ͼ.T"), _T("���2��ͼ"));//L-W-2-CAL.T
// 		conf.addLoadsT(_T("�ҷ�2��ͼ.T") , _T("�ҷ�2��ͼ"));//R-W-2-CAL.T
// 	}
// 	ProgressSetT(30);
// 	if (StruCalcInfo().nWindP > 2)
// 	{	
// 		conf.addLoadsT(_T("���3��ͼ.T"), _T("���3��ͼ"));//L-W-3-CAL.T
// 		conf.addLoadsT(_T("�ҷ�3��ͼ.T") , _T("�ҷ�3��ͼ"));//R-W-3-CAL.T
// 	}
// 	ProgressSetT(35);
// 	if (StruCalcInfo().nDiaoChe>0)
// 	{
// 		conf.addLoadsT(_T("�������ؼ�ͼ.T") , _T("�������ؼ�ͼ"));//C-H-CAL.T
// 	}
// 
// 	ProgressSetT(40);
// 	conf.addStressT(_T("�������ͼ.T"), _T("�������ͼ"));//D-M.T
// 	conf.addStressT(_T("���ؼ���ͼ.T"), _T("���ؼ���ͼ"));//D-V.T
// 	conf.addStressT(_T("��������ͼ.T"), _T("��������ͼ"));//D-N.T  
// 
// 	ProgressSetT(45);
// 	if(StruCalcInfo().IsHasDiaoGua())
// 	{
// 		conf.addStressT(_T("���Һ������ͼ.T"), _T("���Һ������ͼ"));//DG-M.T
// 		conf.addStressT(_T("���Һ�������ͼ.T"), _T("���Һ�������ͼ"));//DG-N.T
// 		conf.addStressT(_T("���Һ��ؼ���ͼ.T"), _T("���Һ��ؼ���ͼ"));//DG-V.T
// 	}
// 
// 	ProgressSetT(50);
// 	conf.addStressT(_T("�������ͼ.T"), _T("�������ͼ"));//L-M.T
// 	conf.addStressT(_T("���ؼ���ͼ.T"), _T("���ؼ���ͼ"));//L-V.T
// 	conf.addStressT(_T("��������ͼ.T"), _T("��������ͼ"));//L-N.T
// 
// 	ProgressSetT(55);
// 	if (StruCalcInfo().nWindP > 0)
// 	{
// 		conf.addStressT(_T("���1���ͼ.T"), _T("���1���ͼ"));//WL.T
// 		conf.addStressT(_T("�ҷ�1���ͼ.T"), _T("�ҷ�1���ͼ"));//WR.T
// 	}
// 	ProgressSetT(60);
// 	if (StruCalcInfo().nWindP > 1)
// 	{
// 		conf.addStressT(_T("���2���ͼ.T"), _T("���2���ͼ"));//WL2.T
// 		conf.addStressT(_T("�ҷ�2���ͼ.T"), _T("�ҷ�2���ͼ"));//WR2.T
// 	}
// 	ProgressSetT(75);
// 	if (StruCalcInfo().nWindP > 2)
// 	{
// 		conf.addStressT(_T("���3���ͼ.T"), _T("���3���ͼ"));//WL3.T
// 		conf.addStressT(_T("�ҷ�3���ͼ"), _T("�ҷ�3���ͼ"));//WR3.T 
// 	}
// 	ProgressSetT(80);
// 	conf.addStressT(_T("��������ͼ.T"), _T("��������ͼ"));//EL.T
// 	conf.addStressT(_T("�ҵ������ͼ.T"), _T("�ҵ������ͼ"));//ER.T
// 
// 	ProgressSetT(82);
// 	{
// 		//�Զ�����ؼ���ͼ
// 		{
// 			int UserZiDingYi = CmdDataMgr::Get().NZiDingYi();
// 
// 			CString strUserFname;
// 			CString strTitle;
// 			if (UserZiDingYi>0)
// 			{
// 				for (int i=0;i<CmdDataMgr::Get().NameZiDingYiNeiLiTu().GetCount();i++)
// 				{
// 					strUserFname =  CmdDataMgr::Get().FiNameZiDingYiNeiLiTu(i);
// 					strTitle = CmdDataMgr::Get().NameZiDingYiNeiLiTu(i);
// 					conf.addStressT(strUserFname,strTitle);
// 				}
// 			}
// 		}
// 	}
// 
// 
// 	ProgressSetT(85);
// 	conf.addStressT(_T("��ذ���ͼ.T"), _T("��ذ���ͼ"));//M.T
// 	conf.addStressT(_T("��������ͼ.T"), _T("��������ͼ"));//Q.T
// 	conf.addStressT(_T("��������ͼ.T"), _T("��������ͼ"));//N.T
// 
// 	conf.addStressScaleT(_T("Ӧ����ͼ.T"), _T("Ӧ����ͼ"));//AS.T
// 
// 	ProgressSetT(90);
// 	conf.addDispT(_T("����λ��ͼ.T"), _T("����λ��ͼ"));//DISP-D.T
// 	if (StruCalcInfo().IsHasDiaoGua())
// 	{
// 		conf.addDispT(_T("���Һ���λ��ͼ.T"), _T("���Һ���λ��ͼ"));//DISP-DG.T
// 	}
// 	conf.addDispT(_T("����λ��ͼ.T"), _T("����λ��ͼ"));//DISP-L.T
// 	if (StruCalcInfo().nWindP > 0)
// 	{
// 		conf.addDispT(_T("���1λ��ͼ.T"), _T("���1λ��ͼ"));//DISP-WL.T
// 		conf.addDispT(_T("�ҷ�1λ��ͼ.T"), _T("�ҷ�1λ��ͼ"));//DISP-WR.T
// 	}
// 
// 	ProgressSetT(95);
// 	if (StruCalcInfo().nWindP > 1)
// 	{
// 		conf.addDispT(_T("���2λ��ͼ.T"), _T("���2λ��ͼ"));//DISP-WL2.T
// 		conf.addDispT(_T("�ҷ�2λ��ͼ.T"), _T("�ҷ�2λ��ͼ"));//DISP-WR2.T
// 	}
// 
// 	if (StruCalcInfo().nWindP > 2)
// 	{
// 		conf.addDispT(_T("���3λ��ͼ.T"), _T("���3λ��ͼ"));//DISP-WL3.T
// 		conf.addDispT(_T("�ҷ�3λ��ͼ.T"), _T("�ҷ�3λ��ͼ"));//DISP-WR3.T
// 	}
// 	ProgressSetT(100);
// 
// 	if (StruCalcInfo().bHasEarthLR)
// 	{
// 		conf.addDispT(_T("�����λ��ͼ.T"), _T("�����λ��ͼ"));//DISP-EL.T
// 		conf.addDispT(_T("�ҵ���λ��ͼ.T"), _T("�ҵ���λ��ͼ"));//DISP-ER.T
// 	}
// 
// 	if (StruCalcInfo().bHasDiaoGua  )
// 	{
// 		conf.addDispT(_T("��+��λ��ͼ.T"), _T("��+���Һ�+��λ��ͼ"));//DISP-DL.T
// 		// ע������û�г���,ͼֽ�������  ��ӻ�λ��ͼ.t
// 	}
// 	else
// 	{
// 		conf.addDispT(_T("��+��λ��ͼ.T"), _T("��+��λ��ͼ"));//DISP-DL.T
// 	}
// 	 
// 	
// 	if (StruCalcInfo().nDiaoChe >0 )
// 	{
// 		conf.addDispT(_T("����ɲ��λ��ͼ.T"), _T("����ɲ��λ��ͼ"));//DISP-TD.T
// 	}
// 
// 
// 
// 	{ //�Զ���λ��ͼ
// 		CString strFi;
// 		CString strTitle;
// 		for (int i=0;i<CmdDataMgr::Get().NameZdySingleDispTu().GetCount();i++)
// 		{
// 			strFi = CmdDataMgr::Get().FiNameZdySingleDispTu(i);
// 			strTitle = CmdDataMgr::Get().NameZdySingleDispTu(i);
// 			conf.addDispT(strFi, strTitle);
// 		}
// 		bool bfirst=true;
// 		for (int i=0;i<CmdDataMgr::Get().NameZdyMultDispTu().GetCount();i++)
// 		{
// 			if (bfirst)
// 			{
// 				bfirst=false;
// 				if(CmdDataMgr::Get().AvoidFirstZdyMultDisp())
// 				{
// 					continue;
// 				}
// 
// 			}
// 			strFi = CmdDataMgr::Get().FiNameZdyMultDispTu(i);
// 			strTitle = CmdDataMgr::Get().NameZdyMultDispTu(i);
// 			conf.addDispT(strFi, strTitle);
// 		}
// 
// 	}
// 
// 	PluginCmd::CloseBigProgress();	
// 
// 
// 	CString str = "���ͼֽ�������";
// 	int n = str.GetLength();
// 	REFPRT(str.GetBuffer(),n);
// 
// 	str = "��\"����������\"�Ի���...";
// 	n = str.GetLength();
// 	REFPRT(str.GetBuffer(),n);


}
extern void CalcOutWord()
{	
	//�����ͼ  + ���� CalculationReport.xml
	CPk_MainOutPut::initReportTool();//�Ѿ�׼�������Tͼ
	CConfigGenCalc conf;
	conf.DeleteCalculationReportXml();

	//�������ļ�,��Tͼ
	SetConfigT(conf);
	//CPk_MainOutPut::ClearCal_T(); 
	conf.saveToFile(CPk_MainOutPut::configXml);

	CPk_MainOutPut::dlgConfig();
}

extern void Main_Param()
{

}


//int &Itype, int &NInt, int IPara[], int &NFloat, float RPara[], int &NStr, int &LStr, char *StrPara[], int n1
extern "C" void __stdcall Param_PrjName(char* prjName,int Len)
{
	//g_Param_MainParam.PrjName  = prjName;
}
