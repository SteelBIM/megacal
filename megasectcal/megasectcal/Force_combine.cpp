#include "StdAfx.h"
#include "Force_combine.h"
#include <math.h>
#include "Tools.h"
#include "GPara.h"
#include "GlobalResult.h"


Force_combine::Force_combine(void)
{

}


Force_combine::~Force_combine(void)
{
}

bool Force_combine::GetForce()
{
// 	//先读入端部内力产生的内力
// 	double My_end[LoadCase][Combine];
// 	double Mz_end[LoadCase][Combine];
// 	char Dir[256];
// 	GetCurrentDirectory( 256, Dir ); 
// 	CString FileName;
// 	FileName.Format(_T("%s\\EndM.mgc"),Dir);
// 	CFileStatus rStatus;
// 	if (CFile::GetStatus(FileName,rStatus)){
// 		CFile ReadFile;
// 		ReadFile.Open(FileName,CFile::modeRead|CFile::typeBinary);
// 		ReadFile.Read(My_end,sizeof(double)*4*13);
// 		ReadFile.Read(Mz_end,sizeof(double)*4*13);
// 	}

	CFile InFile;
	//if (!InFile.Open(_T("juzhuP.sts"),CFile::modeRead|CFile::typeBinary)) { //"E:\\各种题\\1\\juzhuP.sts"
	//	return false;
	//}
	int bOpen;
#if defined _DEBUG
	bOpen = InFile.Open("juzhuP.sts",CFile::modeRead);
#else
	bOpen = InFile.Open("juzhuP.sts",CFile::modeRead|CFile::typeBinary);
#endif

	if(!bOpen)return 0;

	CFileStatus status;
	if( CFile::GetStatus("juzhuP.sts",status)){
		int iSize = status.m_size;
		if(iSize==0) return 0;
	}

	long lPos=0;
	InFile.Seek(lPos,CFile::begin);
	//InFile.Read(&lPos,sizeof(long));
	
	float N_y[4][10][13],M_y[4][10][13],V_y[4][10][13];
	double Nall_y[4][13],Mall_y[4][13],Vall_y[4][13];
	float N_z[4][10][13],M_z[4][10][13],V_z[4][10][13];
	double Nall_z[4][13],Mall_z[4][13],Vall_z[4][13];
	ZeroMemory(N_y,sizeof(float)*4*10*13);
	ZeroMemory(M_y,sizeof(float)*4*10*13);
	ZeroMemory(V_y,sizeof(float)*4*10*13);
	ZeroMemory(Nall_y,sizeof(double)*4*13);
	ZeroMemory(Mall_y,sizeof(double)*4*13);
	ZeroMemory(Vall_y,sizeof(double)*4*13);
	ZeroMemory(N_z,sizeof(float)*4*10*13);
	ZeroMemory(M_z,sizeof(float)*4*10*13);
	ZeroMemory(V_z,sizeof(float)*4*10*13);
	ZeroMemory(Nall_z,sizeof(double)*4*13);
	ZeroMemory(Mall_z,sizeof(double)*4*13);
	ZeroMemory(Vall_z,sizeof(double)*4*13);
	double forcetemp[5] = {0} ;
	int ctrltemp[5] = {0};
	int postemp[5] = {0};
	//读入的荷载是kN和kN*m
	//y向
	//GPara::m_DesnPara->nLoads_y=1; //临时
	//GPara::m_DesnPara->nLoadCase_y[0]=1;//临时
	if (NULL == GPara::m_DesnPara)
	{
		return true;
	}
	for(int i=0;i<GPara::m_DesnPara->nLoads_y;i++)//y向工况数量
	{
		for(int j=0;j<GPara::m_DesnPara->nLoadCase_y[i];j++)//y向i工况荷载数量
		{
			for(int k=0;k<13;k++)
			{
				InFile.Read(&N_y[i][j][k],sizeof(float));
				Nall_y[i][k]=Nall_y[i][k]+N_y[i][j][k]*1.e3;
			}
			for(int k=0;k<13;k++)
			{
				InFile.Read(&M_y[i][j][k],sizeof(float));
				Mall_y[i][k]=Mall_y[i][k]+M_y[i][j][k]*1.e6;
			}
			for(int k=0;k<13;k++)
			{
				InFile.Read(&V_y[i][j][k],sizeof(float));
				Vall_y[i][k]=Vall_y[i][k]+V_y[i][j][k]*1.e3;
			}
		}
	}

	//z向
	for(int i=0;i<GPara::m_DesnPara->nLoads_z;i++)//z向工况数量
	{
		for(int j=0;j<GPara::m_DesnPara->nLoadCase_z[i];j++)//z向i工况荷载数量
		{
			for(int k=0;k<13;k++)
			{
				InFile.Read(&N_z[i][j][k],sizeof(float));
				Nall_z[i][k]=Nall_z[i][k]+N_z[i][j][k]*1.e3;
			}
			for(int k=0;k<13;k++)
			{
				InFile.Read(&M_z[i][j][k],sizeof(float));
				Mall_z[i][k]=Mall_z[i][k]+M_z[i][j][k]*1.e6;
			}
			for(int k=0;k<13;k++)
			{
				InFile.Read(&V_z[i][j][k],sizeof(float));
				Vall_z[i][k]=Vall_z[i][k]+V_z[i][j][k]*1.e3;
			}
		}
	}
	//赋值
	float d1,d2;
	d1 = d2 = 1.f;
//	float dd1,dd2;
	for(int i=0;i<4;i++)	//1恒载，2~4活载
	{
		for(int j=0;j<13;j++)
		{
			if(GPara::m_DesnPara->CodeType==2){	//美国规范情况下
				if(GPara::m_DesnPara->ifsway){	//摇摆柱
					double V1[2],V2[2];
					if (j==0||j==13){
						V1[0] = V1[1] = V2[0] = V2[1] = 1;
						Tools::SwayColmAmplify(d1,d2,V1,V2);
					} 
					else{
						d1=1.0;
						d2=1.0;
					}
				}
				else{		//非摇摆柱		//先不在这里放大，留在后面组合时候放大
					float M1[2],M2[2];
					d1 = d2 = 1.0;
				}
			}

			N[i][j]=fabs(GPara::m_DesnPara->N_ed[i])+max(fabs(Nall_y[i][j]),fabs(Nall_z[i][j]));//轴力取两个方向较大值
			My[i][j]=GPara::m_DesnPara->My_ed[i][j]+d1*Mall_y[i][j];
			Vy[i][j]=Vall_y[i][j];
			Mz[i][j]=GPara::m_DesnPara->Mz_ed[i][j]+d2*Mall_z[i][j];
			Vz[i][j]=Vall_z[i][j];
			if(N[i][j]>forcetemp[0]){
				forcetemp[0] = N[i][j];
				ctrltemp[0] = i; 
				postemp[0] = j;
			}
			if(fabs(My[i][j])>forcetemp[1]){
				forcetemp[1] = fabs(My[i][j]);
				ctrltemp[1] = i; 
				postemp[1] = j;
			}
			if(fabs(Mz[i][j])>forcetemp[2]){
				forcetemp[2] = fabs(Mz[i][j]);
				ctrltemp[2] = i;
				postemp[2] = j;
			}
			if(fabs(Vy[i][j])>forcetemp[3]){
				forcetemp[3] = fabs(Vy[i][j]);
				ctrltemp[3] = i; 
				postemp[3] = j;
			}
			if(fabs(Vz[i][j])>forcetemp[4]){
				forcetemp[4] = fabs(Vz[i][j]);
				ctrltemp[4] = i; 
				postemp[4] = j;
			}
		}
	}
// 	GPara::N_max = forcetemp[0];
// 	GPara::My_max = forcetemp[1];
// 	GPara::Mz_max = forcetemp[2];
// 	GPara::Vy_max = forcetemp[3];
// 	GPara::Vz_max = forcetemp[4];
// 	GlobalResult::loadcomb.Nmax_comb = ctrltemp[0];
// 	GlobalResult::loadcomb.Nmax_pos  = postemp[0];
// 	GlobalResult::loadcomb.Vymax_comb = ctrltemp[3];
// 	GlobalResult::loadcomb.Vymax_pos = postemp[3];
// 	GlobalResult::loadcomb.Vzmax_comb = ctrltemp[4];
// 	GlobalResult::loadcomb.Vzmax_pos = postemp[4];
// 	GlobalResult::loadcomb.Mymax_comb = ctrltemp[1];
// 	GlobalResult::loadcomb.Mymax_pos = postemp[1];
// 	GlobalResult::loadcomb.Mzmax_comb = ctrltemp[2];
// 	GlobalResult::loadcomb.Mzmax_pos = postemp[2];
// 	GPara::N_Ctrl_Comb = ctrltemp[0];
// 	GPara::My_Ctrl_Comb = ctrltemp[1];
// 	GPara::Mz_Ctrl_Comb = ctrltemp[2];
// 	GPara::Vy_Ctrl_Comb = ctrltemp[3];
// 	GPara::Vz_Ctrl_Comb = ctrltemp[4];

	InFile.Close();
	return true;
}

void Force_combine::Euro_combine()
{
	ZeroMemory(GPara::My_combine,sizeof(GPara::My_combine));
	ZeroMemory(GPara::Mz_combine,sizeof(GPara::Mz_combine));
	ZeroMemory(GPara::N_combine,sizeof(GPara::N_combine));
	ZeroMemory(GPara::Vy_combine,sizeof(GPara::Vy_combine));
	ZeroMemory(GPara::Vz_combine,sizeof(GPara::Vz_combine));

	//各组合13个断面内力
	double My_zuhe[30][13];
	double Mz_zuhe[30][13];
	double N_zuhe[30][13];
	double Vy_zuhe[30][13];
	double Vz_zuhe[30][13];
	ZeroMemory(My_zuhe,sizeof(float)*30*13);
	ZeroMemory(Mz_zuhe,sizeof(float)*30*13);
	ZeroMemory(N_zuhe,sizeof(float)*30*13);
	ZeroMemory(Vy_zuhe,sizeof(float)*30*13);
	ZeroMemory(Vz_zuhe,sizeof(float)*30*13);

	float f1y=GPara::m_DesnPara->f1_y;
	float f2y=GPara::m_DesnPara->f2_y;
	float f3y=GPara::m_DesnPara->f3_y;
	float f1z=GPara::m_DesnPara->f1_z;
	float f2z=GPara::m_DesnPara->f2_z;
	float f3z=GPara::m_DesnPara->f3_z;
	for(int i=0;i<13;i++)
	{
		//1.35恒+1.5活1+1.5*f2*活2+1.5*f3*活3
		My_zuhe[0][i]=1.35*My[0][i]+1.5*My[1][i]+1.5*f2y*My[2][i]+1.5*f3y*My[3][i];
		Mz_zuhe[0][i]=1.35*Mz[0][i]+1.5*Mz[1][i]+1.5*f2z*Mz[2][i]+1.5*f3z*Mz[3][i];
		Vy_zuhe[0][i]=1.35*Vy[0][i]+1.5*Vy[1][i]+1.5*f2y*Vy[2][i]+1.5*f3y*Vy[3][i];
		Vz_zuhe[0][i]=1.35*Vz[0][i]+1.5*Vz[1][i]+1.5*f2z*Vz[2][i]+1.5*f3z*Vz[3][i];
		N_zuhe[0][i] =1.35*N[0][i]+1.5*N[1][i]+1.5*f2y*N[2][i]+1.5*f3y*N[3][i];

		//1.35恒+1.5*f1*活1+1.5*活2+1.5*f3*活3
		My_zuhe[1][i]=1.35*My[0][i]+1.5*f1y*My[1][i]+1.5*My[2][i]+1.5*f3y*My[3][i];
		Mz_zuhe[1][i]=1.35*Mz[0][i]+1.5*f1z*Mz[1][i]+1.5*Mz[2][i]+1.5*f3z*Mz[3][i];
		Vy_zuhe[1][i]=1.35*Vy[0][i]+1.5*f1y*Vy[1][i]+1.5*Vy[2][i]+1.5*f3y*Vy[3][i];
		Vz_zuhe[1][i]=1.35*Vz[0][i]+1.5*f1z*Vz[1][i]+1.5*Vz[2][i]+1.5*f3z*Vz[3][i];
		N_zuhe[1][i] =1.35*N[0][i]+1.5*f1y*N[1][i]+1.5*N[2][i]+1.5*f3y*N[3][i];

		//1.35恒+1.5*f1*活1+1.5*f2*活2+1.5*活3
		My_zuhe[2][i]=1.35*My[0][i]+1.5*f1y*My[1][i]+1.5*f2y*My[2][i]+1.5*My[3][i];
		Mz_zuhe[2][i]=1.35*Mz[0][i]+1.5*f1z*Mz[1][i]+1.5*f2z*Mz[2][i]+1.5*Mz[3][i];
		Vy_zuhe[2][i]=1.35*Vy[0][i]+1.5*f1y*Vy[1][i]+1.5*f2y*Vy[2][i]+1.5*Vy[3][i];
		Vz_zuhe[2][i]=1.35*Vz[0][i]+1.5*f1z*Vz[1][i]+1.5*f2z*Vz[2][i]+1.5*Vz[3][i];
		N_zuhe[2][i] =1.35*N[0][i]+1.5*f1y*N[1][i]+1.5*f2y*N[2][i]+1.5*N[3][i];

		//1.0恒+1.3活1+1.3*f2*活2+1.3*f3*活3
		My_zuhe[3][i]=1.0*My[0][i]+1.3*My[1][i]+1.3*f2y*My[2][i]+1.3*f3y*My[3][i];
		Mz_zuhe[3][i]=1.0*Mz[0][i]+1.3*Mz[1][i]+1.3*f2z*Mz[2][i]+1.3*f3z*Mz[3][i];
		Vy_zuhe[3][i]=1.0*Vy[0][i]+1.3*Vy[1][i]+1.3*f2y*Vy[2][i]+1.3*f3y*Vy[3][i];
		Vz_zuhe[3][i]=1.0*Vz[0][i]+1.3*Vz[1][i]+1.3*f2z*Vz[2][i]+1.3*f3z*Vz[3][i];
		N_zuhe[3][i] =1.0*N[0][i]+1.3*N[1][i]+1.3*f2y*N[2][i]+1.3*f3y*N[3][i];

		//1.0恒+1.3*f1*活1+1.3*活2+1.3*f3*活3
		My_zuhe[4][i]=1.0*My[0][i]+1.3*f1y*My[1][i]+1.3*My[2][i]+1.3*f3y*My[3][i];
		Mz_zuhe[4][i]=1.0*Mz[0][i]+1.3*f1z*Mz[1][i]+1.3*Mz[2][i]+1.3*f3z*Mz[3][i];
		Vy_zuhe[4][i]=1.0*Vy[0][i]+1.3*f1y*Vy[1][i]+1.3*Vy[2][i]+1.3*f3y*Vy[3][i];
		Vz_zuhe[4][i]=1.0*Vz[0][i]+1.3*f1z*Vz[1][i]+1.3*Vz[2][i]+1.3*f3z*Vz[3][i];
		N_zuhe[4][i] =1.0*N[0][i]+1.3*f1y*N[1][i]+1.3*N[2][i]+1.3*f3y*N[3][i];

		//1.0恒+1.3*f1*活1+1.3*f2*活2+1.3*活3
		My_zuhe[5][i]=1.0*My[0][i]+1.3*f1y*My[1][i]+1.3*f2y*My[2][i]+1.3*My[3][i];
		Mz_zuhe[5][i]=1.0*Mz[0][i]+1.3*f1z*Mz[1][i]+1.3*f2z*Mz[2][i]+1.3*Mz[3][i];
		Vy_zuhe[5][i]=1.0*Vy[0][i]+1.3*f1y*Vy[1][i]+1.3*f2y*Vy[2][i]+1.3*Vy[3][i];
		Vz_zuhe[5][i]=1.0*Vz[0][i]+1.3*f1z*Vz[1][i]+1.3*f2z*Vz[2][i]+1.3*Vz[3][i];
		N_zuhe[5][i] =1.0*N[0][i]+1.3*f1y*N[1][i]+1.3*f2y*N[2][i]+1.3*N[3][i];
	}
	for(int i=0;i<6;i++)//6个工况
	{
		for(int j=0;j<13;j++)//13个断面
		{
			if (GPara::My_combine[i]<fabs(My_zuhe[i][j]))
			{
				GPara::My_combine[i]=My_zuhe[i][j];
			}
			if (GPara::Mz_combine[i]<fabs(Mz_zuhe[i][j]))
			{
				GPara::Mz_combine[i]=Mz_zuhe[i][j];
			}
			if (GPara::Vy_combine[i]<fabs(Vy_zuhe[i][j]))
			{
				GPara::Vy_combine[i]=Vy_zuhe[i][j];
			}
			if (GPara::Vz_combine[i]<fabs(Vz_zuhe[i][j]))
			{
				GPara::Vz_combine[i]=Vz_zuhe[i][j];
			}
			if (GPara::N_combine[i]<fabs(N_zuhe[i][j]))
			{
				GPara::N_combine[i]=N_zuhe[i][j];
			}
		}
	}
}

void Force_combine::AISC_combine()
{
	ZeroMemory(GPara::My_combine,sizeof(GPara::My_combine));
	ZeroMemory(GPara::Mz_combine,sizeof(GPara::Mz_combine));
	ZeroMemory(GPara::N_combine,sizeof(GPara::N_combine));
	ZeroMemory(GPara::Vy_combine,sizeof(GPara::Vy_combine));
	ZeroMemory(GPara::Vz_combine,sizeof(GPara::Vz_combine));

	//各组合13个断面内力
	double My_zuhe[30][13];
	double Mz_zuhe[30][13];
	double N_zuhe[30][13];
	double Vy_zuhe[30][13];
	double Vz_zuhe[30][13];
	ZeroMemory(My_zuhe,sizeof(float)*30*13);
	ZeroMemory(Mz_zuhe,sizeof(float)*30*13);
	ZeroMemory(N_zuhe,sizeof(float)*30*13);
	ZeroMemory(Vy_zuhe,sizeof(float)*30*13);
	ZeroMemory(Vz_zuhe,sizeof(float)*30*13);

	float f1y=GPara::m_DesnPara->f1_y;
	float f2y=GPara::m_DesnPara->f2_y;
	float f3y=GPara::m_DesnPara->f3_y;
	float f1z=GPara::m_DesnPara->f1_z;
	float f2z=GPara::m_DesnPara->f2_z;
	float f3z=GPara::m_DesnPara->f3_z;
	for(int i=0;i<13;i++)
	{
		//1.4恒
		My_zuhe[0][i]=1.4*My[0][i];
		Mz_zuhe[0][i]=1.4*Mz[0][i];
		Vy_zuhe[0][i]=1.4*Vy[0][i];
		Vz_zuhe[0][i]=1.4*Vz[0][i];
		N_zuhe[0][i] =1.4*N[0][i];

		//1.2恒+1.6*f1活1+1.6*f2*活2+1.6*f3*活3
		My_zuhe[1][i]=1.2*My[0][i]+1.6*f1y*My[1][i]+1.6*f2y*My[2][i]+1.6*f3y*My[3][i];
		Mz_zuhe[1][i]=1.2*Mz[0][i]+1.6*f1y*Mz[1][i]+1.6*f2z*Mz[2][i]+1.6*f3z*Mz[3][i];
		Vy_zuhe[1][i]=1.2*Vy[0][i]+1.6*f1y*Vy[1][i]+1.6*f2y*Vy[2][i]+1.6*f3y*Vy[3][i];
		Vz_zuhe[1][i]=1.2*Vz[0][i]+1.6*f1y*Vz[1][i]+1.6*f2z*Vz[2][i]+1.6*f3z*Vz[3][i];
		N_zuhe[1][i] =1.2*N[0][i]+1.6*f1y*N[1][i]+1.6*f2y*N[2][i]+1.6*f3y*N[3][i];
	}
	for(int i=0;i<2;i++)//2个工况
	{
		for(int j=0;j<13;j++)//13个断面
		{
			if (GPara::My_combine[i]<fabs(My_zuhe[i][j]))
			{
				GPara::My_combine[i]=My_zuhe[i][j];
			}
			if (GPara::Mz_combine[i]<fabs(Mz_zuhe[i][j]))
			{
				GPara::Mz_combine[i]=Mz_zuhe[i][j];
			}
			if (GPara::Vy_combine[i]<fabs(Vy_zuhe[i][j]))
			{
				GPara::Vy_combine[i]=Vy_zuhe[i][j];
			}
			if (GPara::Vz_combine[i]<fabs(Vz_zuhe[i][j]))
			{
				GPara::Vz_combine[i]=Vz_zuhe[i][j];
			}
			if (GPara::N_combine[i]<fabs(N_zuhe[i][j]))
			{
				GPara::N_combine[i]=N_zuhe[i][j];
			}
		}
	}
}

void Force_combine::Chinese_combine()
{
	ZeroMemory(GPara::My_combine,sizeof(GPara::My_combine));
	ZeroMemory(GPara::Mz_combine,sizeof(GPara::Mz_combine));
	ZeroMemory(GPara::N_combine,sizeof(GPara::N_combine));
	ZeroMemory(GPara::Vy_combine,sizeof(GPara::Vy_combine));
	ZeroMemory(GPara::Vz_combine,sizeof(GPara::Vz_combine));

	//各组合13个断面内力
	double My_zuhe[30][13];
	double Mz_zuhe[30][13];
	double N_zuhe[30][13];
	double Vy_zuhe[30][13];
	double Vz_zuhe[30][13];
	ZeroMemory(My_zuhe,sizeof(float)*30*13);
	ZeroMemory(Mz_zuhe,sizeof(float)*30*13);
	ZeroMemory(N_zuhe,sizeof(float)*30*13);
	ZeroMemory(Vy_zuhe,sizeof(float)*30*13);
	ZeroMemory(Vz_zuhe,sizeof(float)*30*13);

	float f1y=GPara::m_DesnPara->f1_y;
	float f2y=GPara::m_DesnPara->f2_y;
	float f3y=GPara::m_DesnPara->f3_y;
	float f1z=GPara::m_DesnPara->f1_z;
	float f2z=GPara::m_DesnPara->f2_z;
	float f3z=GPara::m_DesnPara->f3_z;
	for(int i=0;i<13;i++)
	{
		//1.35恒+1.4*f1活1+1.4*f2*活2+1.4*f3*活3
		My_zuhe[0][i]=1.35*My[0][i]+1.4*f1y*My[1][i]+1.4*f2y*My[2][i]+1.4*f3y*My[3][i];
		Mz_zuhe[0][i]=1.35*Mz[0][i]+1.4*f1y*Mz[1][i]+1.4*f2z*Mz[2][i]+1.4*f3z*Mz[3][i];
		Vy_zuhe[0][i]=1.35*Vy[0][i]+1.4*f1y*Vy[1][i]+1.4*f2y*Vy[2][i]+1.4*f3y*Vy[3][i];
		Vz_zuhe[0][i]=1.35*Vz[0][i]+1.4*f1y*Vz[1][i]+1.4*f2z*Vz[2][i]+1.4*f3z*Vz[3][i];
		N_zuhe[0][i]= 1.35*N[0][i]+1.4*f1y*N[1][i]+1.4*f2y*N[2][i]+1.4*f3y*N[3][i];

		//1.2恒+1.4活1+1.4*f2*活2+1.4*f3*活3
		My_zuhe[1][i]=1.2*My[0][i]+1.4*My[1][i]+1.4*f2y*My[2][i]+1.4*f3y*My[3][i];
		Mz_zuhe[1][i]=1.2*Mz[0][i]+1.4*Mz[1][i]+1.4*f2z*Mz[2][i]+1.4*f3z*Mz[3][i];
		Vy_zuhe[1][i]=1.2*Vy[0][i]+1.4*Vy[1][i]+1.4*f2y*Vy[2][i]+1.4*f3y*Vy[3][i];
		Vz_zuhe[1][i]=1.2*Vz[0][i]+1.4*Vz[1][i]+1.4*f2z*Vz[2][i]+1.4*f3z*Vz[3][i];
		N_zuhe[1][i] =1.2*N[0][i]+1.4*N[1][i]+1.4*f2y*N[2][i]+1.4*f3y*N[3][i];
		
		//1.2恒+1.4*f1*活1+1.4*活2+1.4*f3*活3
		My_zuhe[2][i]=1.2*My[0][i]+1.4*f1y*My[1][i]+1.4*My[2][i]+1.4*f3y*My[3][i];
		Mz_zuhe[2][i]=1.2*Mz[0][i]+1.4*f1z*Mz[1][i]+1.4*Mz[2][i]+1.4*f3z*Mz[3][i];
		Vy_zuhe[2][i]=1.2*Vy[0][i]+1.4*f1y*Vy[1][i]+1.4*Vy[2][i]+1.4*f3y*Vy[3][i];
		Vz_zuhe[2][i]=1.2*Vz[0][i]+1.4*f1z*Vz[1][i]+1.4*Vz[2][i]+1.4*f3z*Vz[3][i];
		N_zuhe[2][i] =1.2*N[0][i]+1.4*f1y*N[1][i]+1.4*N[2][i]+1.4*f3y*N[3][i];

		//1.2恒+1.4*f1*活1+1.4*f2*活2+1.4*活3
		My_zuhe[3][i]=1.2*My[0][i]+1.4*f1y*My[1][i]+1.4*f2y*My[2][i]+1.4*My[3][i];
		Mz_zuhe[3][i]=1.2*Mz[0][i]+1.4*f1z*Mz[1][i]+1.4*f2z*Mz[2][i]+1.4*Mz[3][i];
		Vy_zuhe[3][i]=1.2*Vy[0][i]+1.4*f1y*Vy[1][i]+1.4*f2y*Vy[2][i]+1.4*Vy[3][i];
		Vz_zuhe[3][i]=1.2*Vz[0][i]+1.4*f1z*Vz[1][i]+1.4*f2z*Vz[2][i]+1.4*Vz[3][i];
		N_zuhe[3][i] =1.2*N[0][i]+1.4*f1y*N[1][i]+1.4*f2y*N[2][i]+1.4*N[3][i];

		//1.0恒+1.4活1+1.4*f2*活2+1.4*f3*活3
		My_zuhe[4][i]=1.0*My[0][i]+1.4*My[1][i]+1.4*f2y*My[2][i]+1.4*f3y*My[3][i];
		Mz_zuhe[4][i]=1.0*Mz[0][i]+1.4*Mz[1][i]+1.4*f2z*Mz[2][i]+1.4*f3z*Mz[3][i];
		Vy_zuhe[4][i]=1.0*Vy[0][i]+1.4*Vy[1][i]+1.4*f2y*Vy[2][i]+1.4*f3y*Vy[3][i];
		Vz_zuhe[4][i]=1.0*Vz[0][i]+1.4*Vz[1][i]+1.4*f2z*Vz[2][i]+1.4*f3z*Vz[3][i];
		N_zuhe[4][i] =1.0*N[0][i]+1.4*N[1][i]+1.4*f2y*N[2][i]+1.4*f3y*N[3][i];

		//1.0恒+1.4*f1*活1+1.4*活2+1.4*f3*活3
		My_zuhe[5][i]=1.0*My[0][i]+1.4*f1y*My[1][i]+1.4*My[2][i]+1.4*f3y*My[3][i];
		Mz_zuhe[5][i]=1.0*Mz[0][i]+1.4*f1z*Mz[1][i]+1.4*Mz[2][i]+1.4*f3z*Mz[3][i];
		Vy_zuhe[5][i]=1.0*Vy[0][i]+1.4*f1y*Vy[1][i]+1.4*Vy[2][i]+1.4*f3y*Vy[3][i];
		Vz_zuhe[5][i]=1.0*Vz[0][i]+1.4*f1z*Vz[1][i]+1.4*Vz[2][i]+1.4*f3z*Vz[3][i];
		N_zuhe[5][i] =1.0*N[0][i]+1.4*f1y*N[1][i]+1.4*N[2][i]+1.4*f3y*N[3][i];

		//1.0恒+1.4*f1*活1+1.4*f2*活2+1.4*活3
		My_zuhe[6][i]=1.0*My[0][i]+1.4*f1y*My[1][i]+1.4*f2y*My[2][i]+1.4*My[3][i];
		Mz_zuhe[6][i]=1.0*Mz[0][i]+1.4*f1z*Mz[1][i]+1.4*f2z*Mz[2][i]+1.4*Mz[3][i];
		Vy_zuhe[6][i]=1.0*Vy[0][i]+1.4*f1y*Vy[1][i]+1.4*f2y*Vy[2][i]+1.4*Vy[3][i];
		Vz_zuhe[6][i]=1.0*Vz[0][i]+1.4*f1z*Vz[1][i]+1.4*f2z*Vz[2][i]+1.4*Vz[3][i];
		N_zuhe[6][i] =1.0*N[0][i]+1.4*f1y*N[1][i]+1.4*f2y*N[2][i]+1.4*N[3][i];
	}
	for(int i=0;i<7;i++)//7个工况
	{
		for(int j=0;j<13;j++)//13个断面
		{
			if (GPara::My_combine[i]<fabs(My_zuhe[i][j]))
			{
				GPara::My_combine[i]=My_zuhe[i][j];
			}
			if (GPara::Mz_combine[i]<fabs(Mz_zuhe[i][j]))
			{
				GPara::Mz_combine[i]=Mz_zuhe[i][j];
			}
			if (GPara::Vy_combine[i]<fabs(Vy_zuhe[i][j]))
			{
				GPara::Vy_combine[i]=Vy_zuhe[i][j];
			}
			if (GPara::Vz_combine[i]<fabs(Vz_zuhe[i][j]))
			{
				GPara::Vz_combine[i]=Vz_zuhe[i][j];
			}
			if (GPara::N_combine[i]<fabs(N_zuhe[i][j]))
			{
				GPara::N_combine[i]=N_zuhe[i][j];
			}
		}
	}
}

void Force_combine::BiaoZhun_combine()
{
	ZeroMemory(GPara::My_combine_bz,sizeof(GPara::My_combine_bz));
	ZeroMemory(GPara::Mz_combine_bz,sizeof(GPara::Mz_combine_bz));
	ZeroMemory(GPara::N_combine_bz,sizeof(GPara::N_combine_bz));
	ZeroMemory(GPara::Vy_combine_bz,sizeof(GPara::Vy_combine_bz));
	ZeroMemory(GPara::Vz_combine_bz,sizeof(GPara::Vz_combine_bz));

	//各组合13个断面内力
	double My_zuhe[30][13];
	double Mz_zuhe[30][13];
	double N_zuhe[30][13];
	double Vy_zuhe[30][13];
	double Vz_zuhe[30][13];
	My_max_bz = Mz_max_bz = N_max_bz = Mmix_max_bz = 0.f;
	ZeroMemory(My_zuhe,sizeof(float)*30*13);
	ZeroMemory(Mz_zuhe,sizeof(float)*30*13);
	ZeroMemory(N_zuhe,sizeof(float)*30*13);
	ZeroMemory(Vy_zuhe,sizeof(float)*30*13);
	ZeroMemory(Vz_zuhe,sizeof(float)*30*13);

	for(int i=0;i<13;i++)
	{
		//1.0恒+1.0活1+1.0活2+1.0活3
		My_zuhe[0][i]=1.0*My[0][i]+1.0*My[1][i]+1.0*My[2][i]+1.0*My[3][i];
		Mz_zuhe[0][i]=1.0*Mz[0][i]+1.0*Mz[1][i]+1.0*Mz[2][i]+1.0*Mz[3][i];
		Vy_zuhe[0][i]=1.0*Vy[0][i]+1.0*Vy[1][i]+1.0*Vy[2][i]+1.0*Vy[3][i];
		Vz_zuhe[0][i]=1.0*Vz[0][i]+1.0*Vz[1][i]+1.0*Vz[2][i]+1.0*Vz[3][i];
		N_zuhe[0][i] =1.0*N[0][i]+1.0*N[1][i]+1.0*N[2][i]+1.0*N[3][i];
	}
	for(int i=0;i<1;i++)//1个工况
	{
		for(int j=0;j<13;j++)//13个断面
		{
			double mmix = sqrt(My_zuhe[i][j]*My_zuhe[i][j]+Mz_zuhe[i][j]*Mz_zuhe[i][j]);
			if (fabs(GPara::My_combine_bz[i])<fabs(My_zuhe[i][j]))
			{
				GPara::My_combine_bz[i]=My_zuhe[i][j];
			}
			if (fabs(GPara::Mz_combine_bz[i])<fabs(Mz_zuhe[i][j]))
			{
				GPara::Mz_combine_bz[i]=Mz_zuhe[i][j];
			}
			if (fabs(GPara::Vy_combine_bz[i])<fabs(Vy_zuhe[i][j]))
			{
				GPara::Vy_combine_bz[i]=Vy_zuhe[i][j];
			}
			if (fabs(GPara::Vz_combine_bz[i])<fabs(Vz_zuhe[i][j]))
			{
				GPara::Vz_combine_bz[i]=Vz_zuhe[i][j];
			}
			if (fabs(GPara::N_combine_bz[i])<fabs(N_zuhe[i][j]))
			{
				GPara::N_combine_bz[i]=N_zuhe[i][j];
			}
			if(My_max_bz<fabs(My_zuhe[i][j]))
				My_max_bz = fabs(My_zuhe[i][j]);
			if(Mz_max_bz<fabs(Mz_zuhe[i][j]))
				Mz_max_bz = fabs(Mz_zuhe[i][j]);
			if(N_max_bz<fabs(N_zuhe[i][j]))
				N_max_bz = fabs(N_zuhe[i][j]);
			if(Mmix_max_bz<mmix)
				Mmix_max_bz = mmix;
		}
	}
}


void Force_combine::HeZai_combine()
{
	ZeroMemory(GPara::My_combine,sizeof(GPara::My_combine));
	ZeroMemory(GPara::Mz_combine,sizeof(GPara::Mz_combine));
	ZeroMemory(GPara::N_combine,sizeof(GPara::N_combine));
	ZeroMemory(GPara::Vy_combine,sizeof(GPara::Vy_combine));
	ZeroMemory(GPara::Vz_combine,sizeof(GPara::Vz_combine));

	//各组合13个断面内力

	ZeroMemory(My_zuhe,sizeof(float)*MaxCombine*Parts);
	ZeroMemory(Mz_zuhe,sizeof(float)*MaxCombine*Parts);
	ZeroMemory(N_zuhe,sizeof(float)*MaxCombine*Parts);
	ZeroMemory(Vy_zuhe,sizeof(float)*MaxCombine*Parts);
	ZeroMemory(Vz_zuhe,sizeof(float)*MaxCombine*Parts);
	float f[4][Max_Combines]={0.f};
	int ncomb=GPara::m_DesnPara->n_Combines;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<ncomb;j++)
		{
			f[i][j]=GPara::m_DesnPara->f[i][j];
		}
	}
	float dd1,dd2;		//两个方向的弯矩放大系数
	for(int ic=0;ic<ncomb;ic++)
	{
		for(int i=0;i<13;i++)
		{

			My_zuhe[ic][i]=f[0][ic]*My[0][i]+f[1][ic]*My[1][i]+f[2][ic]*My[2][i]+f[3][ic]*My[3][i];
			Mz_zuhe[ic][i]=f[0][ic]*Mz[0][i]+f[1][ic]*Mz[1][i]+f[2][ic]*Mz[2][i]+f[3][ic]*Mz[3][i];
			Vy_zuhe[ic][i]=f[0][ic]*Vy[0][i]+f[1][ic]*Vy[1][i]+f[2][ic]*Vy[2][i]+f[3][ic]*Vy[3][i];
			Vz_zuhe[ic][i]=f[0][ic]*Vz[0][i]+f[1][ic]*Vz[1][i]+f[2][ic]*Vz[2][i]+f[3][ic]*Vz[3][i];
			N_zuhe[ic][i] =f[0][ic]*N[0][i] +f[1][ic]*N[1][i]+ f[2][ic]*N[2][i] +f[3][ic]*N[3][i];
		}
		if(GPara::m_DesnPara->CodeType==2){		//美国规范
			double Ns,Ned;
			double Med1[2],Med2[2];
			Ns = f[0][ic]*N[0][12];		//直接取根部的轴力
			Ned = N_zuhe[ic][12];
			Med1[0] = My_zuhe[ic][0];
			Med1[1] = My_zuhe[ic][12];
			Med2[0] = Mz_zuhe[ic][0];
			Med2[1] = Mz_zuhe[ic][12];
			if(!GPara::m_DesnPara->ifsway){	//非摇摆柱对弯矩进行放大
				Tools::NoSwayColmAmplify(Ns,Ned,Med1,Med2,dd1,dd2);
			}
		}
		else{
			dd1 = 1.0;
			dd2 = 1.0;
		}
		for(int i=0;i<13;i++)
		{
			My_zuhe[ic][i] = dd1*My_zuhe[ic][i];
			Mz_zuhe[ic][i] = dd2*Mz_zuhe[ic][i];
		}
	}

	//循环获得最大的内力点
	float forcetemp[5] = {0};
	int ctrltemp[5] = {0};
	int postemp[5] = {0};
	for(int i=0;i<ncomb;i++)//所有工况
	{
		for(int j=0;j<13;j++)//13个断面
		{
			if (fabs(GPara::My_combine[i])<fabs(My_zuhe[i][j]))
			{
				GPara::My_combine[i]=My_zuhe[i][j];
			}
			if(fabs(forcetemp[1])<fabs(My_zuhe[i][j])){
				forcetemp[1] =  My_zuhe[i][j];
				ctrltemp[1] = i;
				postemp[1] = j;
			}
			if (fabs(GPara::Mz_combine[i])<fabs(Mz_zuhe[i][j]))
			{
				GPara::Mz_combine[i]=Mz_zuhe[i][j];
			}
			if(fabs(forcetemp[2])<fabs(Mz_zuhe[i][j])){
				forcetemp[2] = Mz_zuhe[i][j];
				ctrltemp[2] = i;
				postemp[2] = j;
			}
			if (fabs(GPara::Vy_combine[i])<fabs(Vy_zuhe[i][j]))
			{
				GPara::Vy_combine[i]=Vy_zuhe[i][j];
			}
			if(fabs(forcetemp[3])<fabs(Vy_zuhe[i][j])){
				forcetemp[3] = Vy_zuhe[i][j];
				ctrltemp[3] = i;
				postemp[3] = j;
			}
			if (fabs(GPara::Vz_combine[i])<fabs(Vz_zuhe[i][j]))
			{
				GPara::Vz_combine[i]=Vz_zuhe[i][j];
			}
			if(fabs(forcetemp[4])<fabs(Vz_zuhe[i][j])){
				forcetemp[4] = Vz_zuhe[i][j];
				ctrltemp[4] = i;
				postemp[4] = j;
			}
			if (fabs(GPara::N_combine[i])<fabs(N_zuhe[i][j]))
			{
				GPara::N_combine[i]=N_zuhe[i][j];
			}
			if(fabs(forcetemp[0])<fabs(N_zuhe[i][j])){
				forcetemp[0] = N_zuhe[i][j];
				ctrltemp[0] = i;
				postemp[0] = j;
			}
		}
	}
	GPara::N_max = forcetemp[0];
	GPara::My_max = forcetemp[1];
	GPara::Mz_max = forcetemp[2];
	GPara::Vy_max = forcetemp[3];
	GPara::Vz_max = forcetemp[4];
	GlobalResult::loadcomb.Nmax_comb = ctrltemp[0];
	GlobalResult::loadcomb.Nmax_pos  = postemp[0];
	GlobalResult::loadcomb.Vymax_comb = ctrltemp[3];
	GlobalResult::loadcomb.Vymax_pos = postemp[3];
	GlobalResult::loadcomb.Vzmax_comb = ctrltemp[4];
	GlobalResult::loadcomb.Vzmax_pos = postemp[4];
	GlobalResult::loadcomb.Mymax_comb = ctrltemp[1];
	GlobalResult::loadcomb.Mymax_pos = postemp[1];
	GlobalResult::loadcomb.Mzmax_comb = ctrltemp[2];
	GlobalResult::loadcomb.Mzmax_pos = postemp[2];
	GPara::N_Ctrl_Comb = ctrltemp[0];
	GPara::My_Ctrl_Comb = ctrltemp[1];
	GPara::Mz_Ctrl_Comb = ctrltemp[2];
	GPara::Vy_Ctrl_Comb = ctrltemp[3];
	GPara::Vz_Ctrl_Comb = ctrltemp[4];
}