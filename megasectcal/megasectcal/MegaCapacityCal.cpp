#include "StdAfx.h"
#include "MegaCapacityCal.h"
#include "GPara.h"
#include "SectProperty.h"
#include "cfgbas.h"
#include "GlobalResult.h"
#include "Force_combine.h"
#include "EUROCode.h"

extern "C" __declspec(dllimport) CCfgBas* m_pCfgBas;
#include <math.h>

MegaCapacityCal::MegaCapacityCal(void)
{

}


MegaCapacityCal::~MegaCapacityCal(void)
{
}

bool MegaCapacityCal::SectDesn()
{
	return true;
}

void MegaCapacityCal::MemberCheck()
{

}

void MegaCapacityCal::CrackCheck()
{

}

bool MegaCapacityCal::initDesn()
{
	return true;
}

bool MegaCapacityCal::CalNpl()
{

	return true;
}

bool MegaCapacityCal::CalVpl()
{
	return true;
}

//求名义强度下的PDM曲线(标准强度，不除材料分项系数)
//  查 ABCD点。
//   A: Nmax;
//   B:N=0;
//   C: 不配筋柱的弯矩最大点   NpmRd
//   D:Mmax; N=0.5*NpmRd，对应的 MmaxRd_x，MmaxRd_y

bool MegaCapacityCal::NMCurve_PDM_Nominal()
{
	//先求Mmaxrd
	double* pNA;

	double* pMBx;
	double* pMBy;

	double* pNC;
	double* pMCx;
	double* pMCy;

	double* pMDx;
	double* pMDy;

	

	
	SectProperty m_Sect;
	m_Sect.SectGeoInfo();
	m_Sect.SectCal();
	float fyd;		//钢材屈服强度
	float fsd;		//钢筋屈服强度
	float fcd;		//混凝土屈服强度
	if(GPara::m_DesnPara->CodeType==1){
		fyd = m_Sect.fy;	//欧洲规范有材料分项系数
		fsd = m_Sect.fs;
		fcd = m_Sect.fc;
	}
	else if(GPara::m_DesnPara->CodeType==2){	//美国规范
		fyd = m_Sect.fy;	
		fsd = m_Sect.fs;
		fcd = m_Sect.fc;
	}
	else if(GPara::m_DesnPara->CodeType==3){	//中国规范
		fyd = m_Sect.fy;	
		fsd = m_Sect.fs;
		fcd = m_Sect.fc;
	}
	double MmaxRd_x = m_Sect.Zsx*fyd+(m_Sect.Zsr1x+m_Sect.Zsr2x)*fsd+0.5*m_Sect.Zcx*0.85*fcd;
	double MmaxRd_y = m_Sect.Zsy*fyd+(m_Sect.Zsr1y+m_Sect.Zsr2y)*fsd+0.5*m_Sect.Zcy*0.85*fcd;
	double NpmRd = m_Sect.Ac*0.85*fcd;

	pNC=&NpmRd;	
	pMDx=&MmaxRd_x;
	pMDy=&MmaxRd_y;
	
	//再求MplRd,需要区分截面
	float hnx,hny;
	double MplRd_x=0.f;	//绕x轴塑形承载力
	double MplRd_y=0.f;
	pMCx=&MplRd_x;
	pMCy=&MplRd_y;


	int dsy = GPara::m_Sect_in->HSect_y;
	int dsx = GPara::m_Sect_in->HSect_x;
	float ds1y = m_Sect.d1y;
	float ds1x = m_Sect.d2x;
	int h   = GPara::m_Sect_in->Sect_h;
	int b   = GPara::m_Sect_in->Sect_b;
	float bb= m_Sect.bb;
	float hh= m_Sect.hh;
	float bb2 = m_Sect.bb2;
	float hh2 = m_Sect.hh2;
	float bs2 = m_Sect.bs2;
	float bs1 = m_Sect.bs1;
	float hs1 = m_Sect.hs1;
	float hs2 = m_Sect.hs2;
	//型钢尺寸
	float hw = GPara::m_Sect_in->HSect.H1 - 2*(GPara::m_Sect_in->HSect.RI);
	int bf = GPara::m_Sect_in->HSect.B1;
	float tf = GPara::m_Sect_in->HSect.RI;
	float tw = GPara::m_Sect_in->HSect.RJ;
	float checksp1,checksp2;
	bool flagok = false;
	//计算Mplrd
	double Zr2xn;
	double Zsxn;
	double Zcxn;
	int secttype = GPara::m_Sect_in->m_kind;
	if(secttype==1){
		//假定 0 <hnx < hh/2
		if(GPara::m_Sect_in->Bar_num_hside==2){		//如果只有两行钢筋的话，只考虑上下等效钢板
			hs2 = 0;
			bs2 = 0;
		}
		checksp1 = 0;
		checksp2 = hw*0.5;
		hnx = 0.85*fcd*(0.5*b*h-tf*bf-0.5*hw*tw-hs1*bs1-hs2*bs2+2*bs1*bs2)/(2*tw*fyd+4*bs2*fsd-0.85*fcd*(tw+2*bs2-b));
		if(hnx>0 && hnx<checksp2){
			flagok = true;
			double Zaup = tf*bf*(hw*0.5+tf*0.5)+(hw*0.5-hnx)*(hnx+hw*0.5)*tw*0.5;
			double Zsup = (hs1-2*bs1)*bs1*(hs2-bs2)*0.5+(hs2*0.5-hnx)*(hnx+hs2*0.5)*bs2*0.5;
			double Zcup = b*(h*0.5-hnx)*(h*0.5+hnx)*0.5-Zaup-Zsup;
			MplRd_x = 0.85*fcd*Zcup+2*Zaup*fyd+2*Zsup*fsd;
		}
		//假定 hh/2-tf<hnx<hh/2

		checksp1 = 0.5*hw;
		checksp2 = hh*0.5;

		double P,Q;
		P=0.5*b*h-0.5*hw*bf-tf*bf-hs1*bs1-hs2*bs2+2*bs1*bs2;
		Q=hw*bf-hw*tw;
		hnx=(0.85*fcd*P+fyd*Q)/(0.85*(b-bf-2*bs2)*fcd+2*bf*fyd+4*bs2*fsd);
		if(checksp1<hnx&& hnx<checksp2)
		{
			flagok = true;
			double Zaup = bf*(0.5*hw+tf-hnx)*((0.5*hw+tf-hnx)/2+hnx);
			double Zsup = (hs1-2*bs1)*bs1*(hs2-bs2)/2+(hs2*0.5-hnx)*(hnx+0.5*hs2)*bs2*0.5;
			double Zcup = b*(h*0.5-hnx)*(h*0.5+hnx)/2-Zaup-Zsup;
			MplRd_x = 0.85*fcd*Zcup+2*Zaup*fyd+2*Zsup*fsd;

		}


		//假定 hh/2-tf<hnx
		if(!flagok){
			checksp1 = hh*0.5;
			hnx = (0.85*fcd*(0.5*b*h-hs1*bs1-hs2*bs2+2*bs1*bs2)-m_Sect.As*fyd)/(4*bs2*fsd-0.85*(2*bs2-b)*fcd);
			if(hnx>checksp1){
				flagok = true;
				double Zsup = (hs1-2*bs1)*bs1*(hs2-bs2)*0.5+(hs2*0.5-hnx)*(hnx+hs2*0.5)*bs2*0.5;
				double Zcup = b*(h*0.5-hnx)*(h*0.5+hnx)*0.5-Zsup;
				MplRd_x = 0.85*fcd*Zcup+2*Zsup*fsd;
			}
		}
	}
	else if(secttype==2){
		//skip
		MplRd_x = 999;
		MplRd_y = 999;
	}
	else if(secttype==3){		//sect3 begin
		//假定hnx<dsy-d/2
		checksp1 = dsy-hh*0.5;
		checksp2 = dsy+hh*0.5;
		hnx = NpmRd/((2*b-4*bs2)*(0.85*fcd)+4*bs2*fsd);
		if((checksp1-hnx)>0){	//满足
			flagok = true;
			Zsxn = 0;
		}
		//假定dsy-d/2 < hnx < dsy+d/2
		if(!flagok){
			hnx = (NpmRd+4*checksp1*bb*(fyd-0.85*fcd))/((2*b-4*bb-4*bs2)*(0.85*fcd)+4*bb*fyd+4*bs2*fsd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				Zsxn = bb*(hnx-checksp1)*(3*hnx+checksp1);
			}
		}
		//假定dsy+d/2 < hnx <ds1y
		checksp1 = dsy+hh*0.5;
		checksp2 = ds1y;
		if(!flagok){
			hnx = (NpmRd-4*hh*bb*fyd+4*hh*bb*0.85*fcd)/((2*b-4*bs2)*(0.85*fcd)+4*bs2*fsd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				Zsxn = m_Sect.Zsx;
			}
		}
		if(!flagok){
			hnx = ds1y;
			Zsxn = m_Sect.Zsx;
		}
	}		//sect3 end
	else if(secttype==4){		//sect4 begin
		//假定 hnx< bb/2
		checksp1 = bb*0.5;
		hnx = NpmRd/((2*b-2*hh-4*bs2)*(0.85*fcd)+2*hh*fyd+4*bs2*fsd);
		if((checksp1-hnx)>0){
			flagok = true;
			Zsxn = 2*hh*hnx*hnx;
		}
		//假定 bb/2 < hnx < dsy-hh/2
		if(!flagok){
			checksp1 = bb*0.5;
			checksp2 = dsy-hh*0.5;
			hnx=(NpmRd+2*bb*hh*0.85*fcd-2*bb*hh*fyd)/((2*b-4*bs2)*0.85*fcd+4*bs2*fsd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				Zsxn = hh*bb*bb*0.5;
			}
		}
		//假定dsy-hh/2 < hnx <dsy+hh/2
		if(!flagok){
			checksp1 = dsy-hh*0.5;
			checksp2 = dsy+hh*0.5;
			hnx = (NpmRd+2*bb*hh*0.85*fcd-2*(dsy-hh*0.5)*bb*0.85*fcd+2*(dsy-hh*0.5)*bb*fyd-2*bb*hh*fyd)/((2*b-2*bb-4*bs2)*0.85*fcd+2*bb*fyd+4*bs2*fsd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				Zsxn = hh*bb*bb*0.5+0.5*bb*(hnx-checksp1)*(3*hnx+checksp1);
			}
		}
		//假定 dsy+hh/2 < hnx < ds1y
		if(!flagok){
			checksp1 = dsy+hh*0.5;
			checksp2 = ds1y;
			hnx = (NpmRd-4*bb*hh*fyd+4*bb*hh*0.85*fcd)/((2*b-4*bs2)*0.85*fcd+4*bs2*fsd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				Zsxn = m_Sect.Zsx;
			}
		}
	}		//sect4 end
	else if(secttype==5){
		//skip
	}
	else if(secttype==6){		//sect6 begin
		//假定 hnx < hh/2
		checksp1 = hh*0.5;
		hnx = NpmRd/((2*b-4*bb-4*bs2)*0.85*fcd+4*bb*fyd+4*bs2*fsd);
		if((checksp1-hnx)>0){
			flagok = true;
			Zsxn = 2*bb*hnx*hnx;
		}
		//假定 hh/2 <hnx< dsy-hh/2
		if(!flagok){
			checksp1 = hh*0.5;
			checksp2 = dsy-hh*0.5;
			hnx	 = (NpmRd+2*bb*hh*0.85*fcd-2*bb*hh*fyd)/((2*b-4*bs2)*0.85*fcd+4*bs2*fsd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				Zsxn = bb*hh*hh*0.5;
			}
		}
		//假定 dsy-hh/2 < hnx < dsy+hh/2
		if(!flagok){
			checksp1 = dsy-hh*0.5;
			checksp2 = dsy+hh*0.5;
			hnx = (NpmRd+2*bb*hh*0.85*fcd-4*checksp1*bb*0.85*fcd-2*bb*hh*fyd+4*checksp1*bb*fyd)/((2*b-4*bb-4*bs2)*0.85*fcd+4*bb*fyd+4*bs2*fsd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				Zsxn = hh*bb*bb*0.5+2*bb*(hnx-checksp1)*(hnx+checksp1);
			}
		}
		//假定 dsy+hh/2 < hnx < ds1y
		if(!flagok){
			checksp1 = dsy+hh*0.5;
			checksp2 = ds1y;
			hnx = (NpmRd-4*bb*hh*fyd+4*bb*hh*0.85*fcd)/((2*b-4*bs2)*0.85*fcd+4*bs2*fsd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				Zsxn = m_Sect.Zsx;
			}
		}
	}		//sect6 end
	else if(secttype==7){		//sect7 begin
		//假定hnx<dsx-hh/2
		checksp1 = dsx-hh*0.5;
		checksp2 = dsx+hh*0.5;
		hnx = NpmRd/((2*h-4*bs1)*(0.85*fcd)+4*bs1*fsd);
		if((checksp1-hnx)>0){	//满足
			flagok = true;
			Zsxn = 0;
		}
		//假定dsx-hh/2 < hnx < dsx+hh/2
		if(!flagok){
			hnx = (NpmRd+4*checksp1*bb*(fyd-0.85*fcd))/((2*h-6*bb-4*bs1)*(0.85*fcd)+6*bb*fyd+4*bs1*fsd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				Zsxn = hh2*(hnx-checksp1)*3*(hnx+checksp1);
			}
		}
		//假定dsx+bb2/2 < hnx <ds1x
		checksp1 = dsx+hh*0.5;
		checksp2 = ds1x;
		if(!flagok){
			hnx = (NpmRd-6*hh*bb*fyd+6*hh*bb*0.85*fcd)/((2*h-4*bs1)*(0.85*fcd)+4*bs1*fsd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				Zsxn = m_Sect.Zsy;
			}
		}
	}		//sect7 end

	else if(secttype==8){		//sect8 begin
		//假定纵筋在1/4截面处
		//假定在翼缘中
		checksp1 = hw*0.5;
		checksp2 = h*0.5;
		hnx = (NpmRd-m_Sect.Asr*fsd-0.85*fcd*m_Sect.Ac-(m_Sect.As-b*h)*fyd)/(2*b*fyd);
		if((hnx-checksp1)>0 && (checksp2-hnx)>0){
			flagok = true;
			MplRd_x = (h*h*0.25-hnx)*b*fyd;
		}
		if(!flagok){
			checksp1 = h*0.25;
			checksp2 = hw*0.5;
			hnx = (NpmRd-m_Sect.Asr*fsd-0.85*fcd*b*h*0.5)/(0.85*fcd*b+2*tw*fyd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				MplRd_x = 0.85*fcd*(b-tw)*0.5*(checksp2*checksp2-hnx*hnx)+2*fyd*b*tf*(checksp2+tf/2)+tw*(checksp2*checksp2-hnx*hnx)*fyd;
			}			
		}
		if(!flagok){
			checksp1 = -h*0.25;
			checksp2 = h*0.25;
			hnx = (NpmRd-0.85*fcd*(b-tw)*hw*0.5)/(0.85*fcd*(b-tw)+2*tw*fyd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				MplRd_x = m_Sect.Asr*fsd*0.25*h+0.85*fcd*(b-tw)*0.5*(hw*hw/16-hnx*hnx)+2*fyd*b*tf*(hw*0.5+tf*0.5)+tw*(hw*hw/16-hnx*hnx)*fyd;
			}
		}
		if(!flagok){
			checksp1 = -hw*0.5;
			checksp2 = -h*0.25;
			hnx = (NpmRd-0.85*fcd*(b-tw)*hw*0.5+m_Sect.Asr*fsd)/(0.85*fcd*(b-tw)+2*tw*fyd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				MplRd_x = 0.85*fcd*(b-tw)*0.5*(hw*hw/16-hnx*hnx)+2*fyd*b*tf*(hw*0.25+tf*0.5)+tw*(hw*hw/16-hnx*hnx)*fyd;
			}
		}
	}	//sect8 end
	//计算MplRd
	//排除一下secttype==1
	GPara::hnx = hnx;
	if(secttype!=1 && secttype!=8){
		Zr2xn = 2*bs2*hnx*hnx;
		Zcxn = b*hnx*hnx - Zr2xn - Zsxn;
		MplRd_x = MmaxRd_x-Zr2xn*fsd-Zsxn*fyd-0.5*Zcxn*0.85*fcd;
	}


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//						再计算My
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	hny = 0;
	flagok=false;
	if(secttype==1){
		//假定 0 <hnx < hh/2
		if(GPara::m_Sect_in->Bar_num_bside==2){		//如果只有两行钢筋的话，只考虑上下等效钢板
			hs1 = 0;
			bs1 = 0;
		}
		checksp1 = 0;
		checksp2 = bb2*0.5;
		hny = (0.85*fcd*(0.5*b*h-tf*bf-hs2*bs2-hs1*bs1+2*bs1*bs2)-hw*tw*fyd)/(4*tw*fyd+4*bs1*fsd-0.85*fcd*(2*tf+2*bs1-h));
		if(hny>0 && hny<checksp2){
			flagok = true;
			double Zaup = (bf*0.5-hny)*(bf*0.5+hny)*tf;
			double Zsup = (hs2-2*bs2)*bs2*(hs1-bs1)*0.5+(hs1*0.5-hny)*(hny+hs1*0.5)*bs1*0.5;
			double Zcup = b*(h*0.5-hny)*(h*0.5+hny)*0.5-Zaup-Zsup;
			MplRd_y = 0.85*fcd*Zcup+2*Zaup*fyd+2*Zsup*fsd;
		}
		//假定 hh/2<hnx
		if(!flagok){
			checksp1 = hh*0.5;
			hny = (0.85*fcd*(0.5*b*h-hs2*bs2-hs1*bs1+2*bs1*bs2)-m_Sect.As*fyd)/(4*bs1*fsd-0.85*(2*bs1-h)*fcd);
			if(hny>checksp1){
				flagok = true;
				double Zsup = (hs2-2*bs2)*bs2*(hs1-bs1)*0.5+(hs1*0.5-hny)*(hny+hs1*0.5)*bs1*0.5;
				double Zcup = h*(b*0.5-hny)*(b*0.5+hny)*0.5-Zsup;
				MplRd_y = 0.85*fcd*Zcup+2*Zsup*fsd;
			}
		}		
	}
	else if(secttype==2){
		MplRd_y = 999;
	}
	else if(secttype==3){		//sect3 begin
		//假定hnx<dsx-bb/2
		checksp1 = dsx-bb2*0.5;
		checksp2 = dsx+bb2*0.5;
		hny = NpmRd/((2*h-4*bs1)*(0.85*fcd)+4*bs1*fsd);
		if((checksp1-hny)>0){	//满足
			flagok = true;
			Zsxn = 0;
		}
		//假定dsx-bb/2 < hnx < dsx+bb/2
		if(!flagok){
			hny = (NpmRd+4*checksp1*hh2*(fyd-0.85*fcd))/((2*h-4*hh2-4*bs1)*(0.85*fcd)+4*hh2*fyd+4*bs1*fsd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				Zsxn = 2*hh2*(hny-checksp1)*(hny+checksp1);
			}
		}
		//假定dsx+bb2/2 < hnx <ds1x
		checksp1 = dsx+bb2*0.5;
		checksp2 = ds1x;
		if(!flagok){
			hny = (NpmRd-4*hh2*bb2*fyd+4*hh2*bb2*0.85*fcd)/((2*h-4*bs1)*(0.85*fcd)+4*bs1*fsd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				Zsxn = m_Sect.Zsy;
			}
		}
		if(!flagok){		//保证不出错
			hny = ds1x;
			Zsxn = m_Sect.Zsy;
		}
	}		//sect3 end
	else if(secttype==4){		//sect4 begin
		//假定 hnx< bb/2
		checksp1 = bb*0.5;
		hny = NpmRd/((2*h-2*hh-4*bs1)*(0.85*fcd)+2*hh*fyd+4*bs1*fsd);
		if((checksp1-hny)>0){
			flagok = true;
			Zsxn = 2*hh*hny*hny;
		}
		//假定 bb/2 < hnx < dsy-hh/2
		if(!flagok){
			checksp1 = bb*0.5;
			checksp2 = dsy-hh*0.5;
			hny=(NpmRd+2*bb*hh*0.85*fcd-2*bb*hh*fyd)/((2*h-4*bs1)*0.85*fcd+4*bs1*fsd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				Zsxn = hh*bb*bb*0.5;
			}
		}
		//假定dsx-hh/2 < hnx <dsx+hh/2
		if(!flagok){
			checksp1 = dsx-hh*0.5;
			checksp2 = dsx+hh*0.5;
			hny = (NpmRd+2*bb*hh*0.85*fcd-2*(dsx-hh*0.5)*bb*0.85*fcd+2*(dsx-hh*0.5)*bb*fyd-2*bb*hh*fyd)/((2*h-2*bb-4*bs1)*0.85*fcd+2*bb*fyd+4*bs1*fsd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				Zsxn = hh*bb*bb*0.5+0.5*bb*(hny-checksp1)*(3*hny+checksp1);
			}
		}
		//假定 dsx+hh/2 < hnx < ds1x
		if(!flagok){
			checksp1 = dsx+hh*0.5;
			checksp2 = ds1x;
			hny = (NpmRd-4*bb*hh*fyd+4*bb*hh*0.85*fcd)/((2*h-4*bs1)*0.85*fcd+4*bs1*fsd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				Zsxn = m_Sect.Zsy;
			}
		}
	}		//sect4 end
	else if(secttype==5){
		//skip
	}
	else if(secttype==6){		//sect6 begin 参考截面3
		//假定hnx<dsx-bb/2
		checksp1 = dsx-bb2*0.5;
		checksp2 = dsx+bb2*0.5;
		hny = NpmRd/((2*h-4*bs1)*(0.85*fcd)+4*bs1*fsd);
		if((checksp1-hny)>0){	//满足
			flagok = true;
			Zsxn = 0;
		}
		//假定dsx-bb2/2 < hnx < dsx+bb2/2
		if(!flagok){
			hny = (NpmRd+6*checksp1*hh2*(fyd-0.85*fcd))/((2*h-6*hh2-4*bs1)*(0.85*fcd)+6*hh2*fyd+4*bs1*fsd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				Zsxn = hh2*(hny-checksp1)*3*(hny+checksp1);
			}
		}
		//假定dsx+bb2/2 < hnx <ds1x
		checksp1 = dsx+bb2*0.5;
		checksp2 = ds1x;
		if(!flagok){
			hny = (NpmRd-6*hh2*bb2*fyd+6*hh2*bb2*0.85*fcd)/((2*h-6*bs1)*(0.85*fcd)+4*bs1*fsd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				Zsxn = m_Sect.Zsy;
			}
		}
	}		//sect6 end
	else if(secttype==7){		//sect7 begin

		/////////////////////////////
		//假定 hnx < bb/2
		checksp1 = bb2*0.5;
		hny = NpmRd/((2*b-4*hh2-4*bs1)*0.85*fcd+4*bb2*fyd+4*bs1*fsd);
		if((checksp1-hny)>0){
			flagok = true;
			Zsxn = 2*bb2*hny*hny;
		}
		//假定 bb/2 <hny< dsy-bb/2
		if(!flagok){
			checksp1 = bb2*0.5;
			checksp2 = dsy-bb2*0.5;
			hny	 = (NpmRd+2*bb2*hh2*0.85*fcd-2*bb2*hh2*fyd)/((2*b-4*bs1)*0.85*fcd+4*bs1*fsd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				Zsxn = bb2*bb2*hh2*0.5;
			}
		}
		//假定 dsy-bb2/2 < hny < dsy+bb2/2
		if(!flagok){
			checksp1 = dsy-bb2*0.5;
			checksp2 = dsy+bb2*0.5;
			hny = (NpmRd+2*bb2*hh2*0.85*fcd-4*checksp1*hh2*0.85*fcd-2*bb2*hh2*fyd+4*checksp1*hh2*fyd)/((2*b-4*hh2-4*bs1)*0.85*fcd+4*hh2*fyd+4*bs1*fsd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				Zsxn = bb2*hh2*hh2*0.5+2*hh2*(hny-checksp1)*(hny+checksp1);
			}
		}
		//假定 dsy+hh/2 < hny < ds1y
		if(!flagok){
			checksp1 = dsy+bb2*0.5;
			checksp2 = ds1y;
			hny = (NpmRd-4*bb2*hh2*fyd+4*bb2*hh2*0.85*fcd)/((2*b-4*bs1)*0.85*fcd+4*bs1*fsd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				Zsxn = m_Sect.Zsx;
			}
		}
	}		//sect7 end

	///////////////////////////



	else if(secttype==8){
		checksp1 = hw*0.5;
		checksp2 = h*0.5;
		hny = (NpmRd-m_Sect.Asr*fsd-0.85*fcd*m_Sect.Ac-(m_Sect.As-b*h)*fyd)/(2*b*fyd);
		if((hny-checksp1)>0 && (checksp2-hny)>0){
			flagok = true;
			MplRd_y = (h*h*0.25-hny)*b*fyd;
		}
		if(!flagok){
			checksp1 = h*0.25;
			checksp2 = hw*0.5;
			hny = (NpmRd-m_Sect.Asr*fsd-0.85*fcd*b*h*0.5)/(0.85*fcd*b+2*tw*fyd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				MplRd_y = 0.85*fcd*(b-tw)*0.5*(checksp2*checksp2-hny*hny)+2*fyd*b*tf*(checksp2+tf/2)+tw*(checksp2*checksp2-hny*hny)*fyd;
			}			
		}
		if(!flagok){
			checksp1 = -h*0.25;
			checksp2 = h*0.25;
			hny = (NpmRd-0.85*fcd*(b-tw)*hw*0.5)/(0.85*fcd*(b-tw)+2*tw*fyd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				MplRd_y = m_Sect.Asr*fsd*0.25*h+0.85*fcd*(b-tw)*0.5*(hw*hw/16-hny*hny)+2*fyd*b*tf*(hw*0.5+tf*0.5)+tw*(hw*hw/16-hny*hny)*fyd;
			}
		}
		if(!flagok){
			checksp1 = -hw*0.5;
			checksp2 = -h*0.25;
			hny = (NpmRd-0.85*fcd*(b-tw)*hw*0.5+m_Sect.Asr*fsd)/(0.85*fcd*(b-tw)+2*tw*fyd);
			if((hny-checksp1)>0 && (checksp2-hny)>0){
				flagok = true;
				MplRd_y = 0.85*fcd*(b-tw)*0.5*(hw*hw/16-hny*hny)+2*fyd*b*tf*(hw*0.25+tf*0.5)+tw*(hw*hw/16-hny*hny)*fyd;
			}
		}
	}

	GPara::hny = hny;
	//计算MplRd
	if(secttype!=1 && secttype!=8){
		Zr2xn = 2*bs1*hny*hny;
		Zcxn = h*hny*hny - Zr2xn - Zsxn;
		MplRd_y = MmaxRd_y-Zr2xn*fsd-Zsxn*fyd-0.5*Zcxn*0.85*fcd;
	}
	NMPoints_ nm_point;
// 	if(GPara::m_DesnPara->CodeType==2){
// 		float faib = GPara::m_DesnPara->faib;
// 		float faic = GPara::m_DesnPara->faic;
// 		MplRd_x = MplRd_x*faib;
// 		MplRd_y = MplRd_y*faib;
// 		NpmRd = NpmRd*faic;
// 		MmaxRd_x = MmaxRd_x*faib;
// 		MmaxRd_y = MmaxRd_y*faib;
// 	}

	//A点 Na = Nplrd  Ma = 0
	nm_point.x_M = 0;
	nm_point.y_N = GPara::NplRk*1.e-3;
	NMPoints_x_nominal.Add(nm_point);
	//C点
	nm_point.x_M = MplRd_x*1.e-6;
	nm_point.y_N = NpmRd*1.e-3;
	NMPoints_x_nominal.Add(nm_point);
	//D点
	nm_point.x_M = MmaxRd_x*1.e-6;
	nm_point.y_N = 0.5*NpmRd*1.e-3;
	NMPoints_x_nominal.Add(nm_point);
	//B点 
	nm_point.x_M = MplRd_x*1.e-6;
	nm_point.y_N = 0;
	NMPoints_x_nominal.Add(nm_point);

	//y方向
	//A点 Na = Nplrd  Ma = 0
	nm_point.x_M = 0;
	nm_point.y_N = GPara::NplRk*1.e-3;
	NMPoints_y_nominal.Add(nm_point);
	//C点
	nm_point.x_M = MplRd_y*1.e-6;
	nm_point.y_N = NpmRd*1.e-3;
	NMPoints_y_nominal.Add(nm_point);
	//D点
	nm_point.x_M = MmaxRd_y*1.e-6;
	nm_point.y_N = 0.5*NpmRd*1.e-3;
	NMPoints_y_nominal.Add(nm_point);
	//B点 
	nm_point.x_M = MplRd_y*1.e-6;
	nm_point.y_N = 0;
	NMPoints_y_nominal.Add(nm_point);

	return true;

}
//求设计强度下的PDM曲线(设计强度)
//  查 ABCD点。
//   A: Nmax;
//   B:N=0;
//   C:
//   D:Mmax;

bool MegaCapacityCal::NMCurve_PDM()		//PDM方法计算N-M曲线
{
	//先求Mmaxrd
	SectProperty m_Sect;
	m_Sect.SectGeoInfo();
	m_Sect.SectCal();
	float fyd;		//钢材屈服强度
	float fsd;		//钢筋屈服强度
	float fcd;		//混凝土屈服强度
	if(GPara::m_DesnPara->CodeType==1){
		fyd = m_Sect.fy/1.f;	//欧洲规范有材料分项系数
		fsd = m_Sect.fs/1.15;
		fcd = m_Sect.fc/1.5;
	}
	else if(GPara::m_DesnPara->CodeType==2){	//美国规范
		fyd = m_Sect.fy;	
		fsd = m_Sect.fs;
		fcd = m_Sect.fc;
	}
	else if(GPara::m_DesnPara->CodeType==3){	//中国规范
		fyd = m_Sect.fy/1.11;	
		fsd = m_Sect.fs/1.1;
		fcd = m_Sect.fc/1.4;
	}
	double MmaxRd_x,MmaxRd_y,NpmRd;
	if(GPara::m_DesnPara->CodeType==1){
		double conbar=1.;
		if(GPara::m_Sect_in->Bar_num_hside==2) conbar=0.0;
		MmaxRd_x = m_Sect.Zsx*fyd+(m_Sect.Zsr1x+m_Sect.Zsr2x*conbar)*fsd+0.5*m_Sect.Zcx*0.85*fcd;
		MmaxRd_y = m_Sect.Zsy*fyd+(m_Sect.Zsr1y*conbar+m_Sect.Zsr2y)*fsd+0.5*m_Sect.Zcy*0.85*fcd;
		NpmRd = m_Sect.Ac*0.85*fcd;
	}
	else if(GPara::m_DesnPara->CodeType==2){

	}
	else if(GPara::m_DesnPara->CodeType==3){

	}
	//再求MplRd,需要区分截面
	float hnx,hny;
	double MplRd_x=0.f;	//绕x轴塑形承载力
	double MplRd_y=0.f;
	int dsy = GPara::m_Sect_in->HSect_y;
	int dsx = GPara::m_Sect_in->HSect_x;
	float ds1y = m_Sect.d1y;
	float ds1x = m_Sect.d2x;
	int h   = GPara::m_Sect_in->Sect_h;
	int b   = GPara::m_Sect_in->Sect_b;
	float bb= m_Sect.bb;
	float hh= m_Sect.hh;
	float bb2 = m_Sect.bb2;
	float hh2 = m_Sect.hh2;
	float bs2 = m_Sect.bs2;
	float bs1 = m_Sect.bs1;
	float hs1 = m_Sect.hs1;
	float hs2 = m_Sect.hs2;
	
	double V_x = fabs(GPara::Vy_max);
	double V_y = fabs(GPara::Vz_max);
	float roux = 1.f;
	float rouy = 1.f;
	 int nvv=2;
	 if(GPara::m_DesnPara->CodeType!=1) nvv=1;
	 for(int ivv=0;ivv<nvv;ivv++)
	 {
		 
		//型钢尺寸
		float hw = GPara::m_Sect_in->HSect.H1 - 2*(GPara::m_Sect_in->HSect.RI);
		int bf = GPara::m_Sect_in->HSect.B1;
		float tf = GPara::m_Sect_in->HSect.RI;
		float tw = roux*GPara::m_Sect_in->HSect.RJ;
		float checksp1,checksp2;
		bool flagok = false;
		//计算Mplrd
		double Zr2xn;
		double Zsxn;
		double Zcxn;
		int secttype = GPara::m_Sect_in->m_kind;

		if(GPara::m_Sect_in->Bar_num_hside==2){		//如果只有两行钢筋的话，只考虑上下等效钢板
			//				hs2 = 0;              //  hs2还用于定位hs1*bs1的位置
			bs2 = 0;
		}
	//先算mx
		if(secttype==1){
			//假定 0 <hnx < hh/2
			checksp1 = 0;
			checksp2 = hw*0.5;
			hnx = 0.85*fcd*(0.5*b*h-tf*bf-0.5*hw*tw-hs1*bs1-hs2*bs2+2*bs1*bs2)/(2*tw*fyd+4*bs2*fsd-0.85*fcd*(tw+2*bs2-b));
			if(hnx>0 && hnx<checksp2){
				flagok = true;
				double Zaup = tf*bf*(hw*0.5+tf*0.5)+(hw*0.5-hnx)*(hnx+hw*0.5)*tw*0.5;
				double Zsup = (hs1-2*bs2)*bs1*(hs2-bs2)*0.5+(hs2*0.5-hnx)*(hnx+hs2*0.5)*bs2*0.5;
				double Zcup = b*(h*0.5-hnx)*(h*0.5+hnx)*0.5-Zaup-Zsup;
				MplRd_x = 0.85*fcd*Zcup+2*Zaup*fyd+2*Zsup*fsd;
			}
			//假定 hh/2-tf<hnx<hh/2
			
			checksp1 = 0.5*hw;
			checksp2 = hh*0.5;

			double P,Q;
			P=0.5*b*h-0.5*hw*bf-tf*bf-hs1*bs1-hs2*bs2+2*bs1*bs2;
			Q=hw*bf-hw*tw;
			hnx=(0.85*fcd*P+fyd*Q)/(0.85*(b-bf-2*bs2)*fcd+2*bf*fyd+4*bs2*fsd);
			if(checksp1<hnx&& hnx<checksp2)
			{
				flagok = true;
				double Zaup = bf*(0.5*hw+tf-hnx)*((0.5*hw+tf-hnx)/2+hnx);
				double Zsup = (hs1-2*bs2)*bs1*(hs2-bs2)/2+(hs2*0.5-hnx)*(hnx+0.5*hs2)*bs2*0.5;
				double Zcup = b*(h*0.5-hnx)*(h*0.5+hnx)/2-Zaup-Zsup;
				MplRd_x = 0.85*fcd*Zcup+2*Zaup*fyd+2*Zsup*fsd;

			}


			//假定 hh/2-tf<hnx
			if(!flagok){
				checksp1 = hh*0.5;
				hnx = (0.85*fcd*(0.5*b*h-hs1*bs1-hs2*bs2+2*bs1*bs2)-m_Sect.As*fyd)/(4*bs2*fsd-0.85*(2*bs2-b)*fcd);
				if(hnx>checksp1){
					flagok = true;
					double Zsup = (hs1-2*bs2)*bs1*(hs2-bs2)*0.5+(hs2*0.5-hnx)*(hnx+hs2*0.5)*bs2*0.5;
					double Zcup = b*(h*0.5-hnx)*(h*0.5+hnx)*0.5-Zsup;
					MplRd_x = 0.85*fcd*Zcup+2*Zsup*fsd;
				}
			}
		}
		else if(secttype==2){
			//skip
			MplRd_x = 999;
			MplRd_y = 999;
		}
		else if(secttype==3){		//sect3 begin
			//假定hnx<dsy-d/2
		checksp1 = dsy-hh*0.5;
		checksp2 = dsy+hh*0.5;
		hnx = NpmRd/((2*b-4*bs2)*(0.85*fcd)+4*bs2*fsd);
		if((checksp1-hnx)>0){	//满足
			flagok = true;
			Zsxn = 0;
		}
			//假定dsy-d/2 < hnx < dsy+d/2
		if(!flagok){
			hnx = (NpmRd+4*checksp1*bb*(fyd-0.85*fcd))/((2*b-4*bb-4*bs2)*(0.85*fcd)+4*bb*fyd+4*bs2*fsd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				Zsxn = bb*(hnx-checksp1)*(3*hnx+checksp1);
			}
		}
			//假定dsy+d/2 < hnx <ds1y
		checksp1 = dsy+hh*0.5;
		checksp2 = ds1y;
		if(!flagok){
			hnx = (NpmRd-4*hh*bb*fyd+4*hh*bb*0.85*fcd)/((2*b-4*bs2)*(0.85*fcd)+4*bs2*fsd);
			if((hnx-checksp1)>0 && (checksp2-hnx)>0){
				flagok = true;
				Zsxn = m_Sect.Zsx;
			}
		}
		if(!flagok){
			hnx = ds1y;
			Zsxn = m_Sect.Zsx;
		}
		}		//sect3 end
		else if(secttype==4){		//sect4 begin
			//假定 hnx< bb/2
			checksp1 = bb*0.5;
			hnx = NpmRd/((2*b-2*hh-4*bs2)*(0.85*fcd)+2*hh*fyd+4*bs2*fsd);
			if((checksp1-hnx)>0){
				flagok = true;
				Zsxn = 2*hh*hnx*hnx;
			}
			//假定 bb/2 < hnx < dsy-hh/2
			if(!flagok){
				checksp1 = bb*0.5;
				checksp2 = dsy-hh*0.5;
				hnx=(NpmRd+2*bb*hh*0.85*fcd-2*bb*hh*fyd)/((2*b-4*bs2)*0.85*fcd+4*bs2*fsd);
				if((hnx-checksp1)>0 && (checksp2-hnx)>0){
					flagok = true;
					Zsxn = hh*bb*bb*0.5;
				}
			}
			//假定dsy-hh/2 < hnx <dsy+hh/2
			if(!flagok){
				checksp1 = dsy-hh*0.5;
				checksp2 = dsy+hh*0.5;
				hnx = (NpmRd+2*bb*hh*0.85*fcd-2*(dsy-hh*0.5)*bb*0.85*fcd+2*(dsy-hh*0.5)*bb*fyd-2*bb*hh*fyd)/((2*b-2*bb-4*bs2)*0.85*fcd+2*bb*fyd+4*bs2*fsd);
				if((hnx-checksp1)>0 && (checksp2-hnx)>0){
					flagok = true;
					Zsxn = hh*bb*bb*0.5+0.5*bb*(hnx-checksp1)*(3*hnx+checksp1);
				}
			}
			//假定 dsy+hh/2 < hnx < ds1y
			if(!flagok){
				checksp1 = dsy+hh*0.5;
				checksp2 = ds1y;
				hnx = (NpmRd-4*bb*hh*fyd+4*bb*hh*0.85*fcd)/((2*b-4*bs2)*0.85*fcd+4*bs2*fsd);
				if((hnx-checksp1)>0 && (checksp2-hnx)>0){
					flagok = true;
					Zsxn = m_Sect.Zsx;
				}
			}
		}		//sect4 end
		else if(secttype==5){
			//skip
		}
		else if(secttype==6){		//sect6 begin
			//假定 hnx < hh/2
			checksp1 = hh*0.5;
			hnx = NpmRd/((2*b-4*bb-4*bs2)*0.85*fcd+4*bb*fyd+4*bs2*fsd);
			if((checksp1-hnx)>0){
				flagok = true;
				Zsxn = 2*bb*hnx*hnx;
			}
			//假定 hh/2 <hnx< dsy-hh/2
			if(!flagok){
				checksp1 = hh*0.5;
				checksp2 = dsy-hh*0.5;
				hnx	 = (NpmRd+2*bb*hh*0.85*fcd-2*bb*hh*fyd)/((2*b-4*bs2)*0.85*fcd+4*bs2*fsd);
				if((hnx-checksp1)>0 && (checksp2-hnx)>0){
					flagok = true;
					Zsxn = bb*hh*hh*0.5;
				}
			}
			//假定 dsy-hh/2 < hnx < dsy+hh/2
			if(!flagok){
				checksp1 = dsy-hh*0.5;
				checksp2 = dsy+hh*0.5;
				hnx = (NpmRd+2*bb*hh*0.85*fcd-4*checksp1*bb*0.85*fcd-2*bb*hh*fyd+4*checksp1*bb*fyd)/((2*b-4*bb-4*bs2)*0.85*fcd+4*bb*fyd+4*bs2*fsd);
				if((hnx-checksp1)>0 && (checksp2-hnx)>0){
					flagok = true;
					Zsxn = hh*bb*bb*0.5+2*bb*(hnx-checksp1)*(hnx+checksp1);
				}
			}
			//假定 dsy+hh/2 < hnx < ds1y
			if(!flagok){
				checksp1 = dsy+hh*0.5;
				checksp2 = ds1y;
				hnx = (NpmRd-4*bb*hh*fyd+4*bb*hh*0.85*fcd)/((2*b-4*bs2)*0.85*fcd+4*bs2*fsd);
				if((hnx-checksp1)>0 && (checksp2-hnx)>0){
					flagok = true;
					Zsxn = m_Sect.Zsx;
				}
			}
		}		//sect6 end
		else if(secttype==7){		//sect7 begin
			//假定hnx<dsx-hh/2
			checksp1 = dsx-hh*0.5;
			checksp2 = dsx+hh*0.5;
			hnx = NpmRd/((2*h-4*bs1)*(0.85*fcd)+4*bs1*fsd);
			if((checksp1-hnx)>0){	//满足
				flagok = true;
				Zsxn = 0;
			}
			//假定dsx-hh/2 < hnx < dsx+hh/2
			if(!flagok){
				hnx = (NpmRd+4*checksp1*bb*(fyd-0.85*fcd))/((2*h-6*bb-4*bs1)*(0.85*fcd)+6*bb*fyd+4*bs1*fsd);
				if((hnx-checksp1)>0 && (checksp2-hnx)>0){
					flagok = true;
					Zsxn = hh2*(hnx-checksp1)*3*(hnx+checksp1);
				}
			}
			//假定dsx+bb2/2 < hnx <ds1x
			checksp1 = dsx+hh*0.5;
			checksp2 = ds1x;
			if(!flagok){
				hnx = (NpmRd-6*hh*bb*fyd+6*hh*bb*0.85*fcd)/((2*h-4*bs1)*(0.85*fcd)+4*bs1*fsd);
				if((hnx-checksp1)>0 && (checksp2-hnx)>0){
					flagok = true;
					Zsxn = m_Sect.Zsy;
				}
			}
		}		//sect7 end

		else if(secttype==8){		//sect8 begin
			//假定纵筋在1/4截面处			
			//   case 2:
			checksp1 = -hw*0.5;
			checksp2 = -hs2*0.25;
			hnx = (NpmRd-m_Sect.Asr*fsd-0.85*fcd*b*h*0.5)/(0.85*fcd*b+2*tw*fyd);
			if(checksp1<hnx&& hnx>checksp2){
				flagok = true;
				MplRd_x = 0.85*fcd*(b-tw)*0.5*(hw*hw*0.25-hnx*hnx)+2*fyd*b*tf*(hw*0.5+tf*0.5)+tw*(hw*hw*0.25-hnx*hnx)*fyd;
			}			
			
			if(!flagok){   
				checksp1 = -0.5*hs2;          // case 3
				checksp2 = 0.5*hs2;
				hnx = (NpmRd-0.85*fcd*(b-hw)*hw*0.5)/(0.85*fcd*(b-tw)+2*tw*fyd);
				if((hnx-checksp1)>0 && (checksp2-hnx)>0){
					flagok = true;
					MplRd_x = m_Sect.Asr*fsd*hs1+0.85*fcd*(b-tw)*0.5*(hw*hw*0.25-hnx*hnx)+2*fcd*b*tf*(hw*0.5+tf*0.5)+tw*(hw*hw*0.25-hnx*hnx)*fyd;
				}
			}
			if(!flagok){
				checksp1 = hs2*0.5;
				checksp2 = hw*0.5;
				hnx = (NpmRd-0.85*fcd*(b-tw)*hw*0.5+m_Sect.Asr*fsd)/(0.85*fcd*(b-tw)+2*tw*fyd);
				if((hnx-checksp1)>0 && (checksp2-hnx)>0){
					flagok = true;
					MplRd_x = 0.85*fcd*(b-tw)*0.5*(hw*hw*0.25-hnx*hnx)+2*fyd*b*tf*(hw*0.5+tf*0.5)+tw*(hw*hw*0.25-hnx*hnx)*fyd;
				}
			}
		}	//sect8 end
		//计算MplRd
		//排除一下secttype==1
		GPara::hnx = hnx;
		if(secttype!=1 && secttype!=8){
			Zr2xn = 2*bs2*hnx*hnx;
			Zcxn = b*hnx*hnx - Zr2xn - Zsxn;
			MplRd_x = MmaxRd_x-Zr2xn*fsd-Zsxn*fyd-0.5*Zcxn*0.85*fcd;
		}


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//						再计算My
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		tw = rouy*GPara::m_Sect_in->HSect.RJ;

		if(GPara::m_Sect_in->Bar_num_bside==2){		//如果只有两行钢筋的话，只考虑上下等效钢板
			//				hs1 = 0;    //  hs2还用于定位hs1*bs1的位置
			float bs2 = m_Sect.bs2;
			bs1 = 0;
		}

		hny = 0;
		flagok=false;
		if(secttype==1){
			//假定 0 <hnx < hh/2
			//假定 0 <hnx < tw*0.5
			checksp1 = 0;
			checksp2 = tw*0.5;
			double P,Q;
			P=0.5*b*h-tf*bf-0.5*hw*tw-bs1*hs1-bs2*hs2+2*bs1*bs2;
			Q=0.85*fcd*(b-2*tf-hw-2*bs2)+(2*hw+4*tf)*fyd+4*bs2*fsd;

			hny =0.85*fcd*P/Q;
			if(hny<checksp2)
			{
				flagok = true;
				double Zaup = (bf*0.5-hny)*(bf*0.5+hny)*tf+hw*(0.5*tw-hny)*(0.5*tw+hny)*0.5;
				double Zsup = (hs2-2*bs2)*bs2*(hs1-bs1)*0.5+(hs2*0.5-hny)*(hny+hs1*0.5)*bs1*0.5;
				double Zcup = h*(b*0.5-hny)*(b*0.5+hny)*0.5-Zaup-Zsup;
				MplRd_y = 0.85*fcd*Zcup+2*Zaup*fyd+2*Zsup*fsd;
			}


			checksp1 = tw*0.5;
			checksp2 = tf*0.5;
			hny = (0.85*fcd*(0.5*b*h-tf*bf-hs2*bs2-hs1*bs1+2*bs1*bs2)-hw*tw*fyd)/(4*tw*fyd+4*bs1*fsd-0.85*fcd*(2*tf+2*bs1-h));
			if(hny>0 && hny<checksp2){
				flagok = true;
				double Zaup = (bf*0.5-hny)*(bf*0.5+hny)*tf;
				double Zsup = (hs2-2*bs1)*bs2*(hs1-bs1)*0.5+(hs1*0.5-hny)*(hny+hs1*0.5)*bs1*0.5;
				double Zcup = b*(h*0.5-hny)*(h*0.5+hny)*0.5-Zaup-Zsup;
				MplRd_y = 0.85*fcd*Zcup+2*Zaup*fyd+2*Zsup*fsd;
			}
			//假定 hh/2<hnx
			if(!flagok){
				checksp1 = tf*0.5;
				hny = (0.85*fcd*(0.5*b*h-hs2*bs2-hs1*bs1+2*bs1*bs2)-m_Sect.As*fyd)/(4*bs1*fsd-0.85*(2*bs1-h)*fcd);
				if(hny>checksp1){
					flagok = true;
					double Zsup = (hs2-2*bs1)*bs2*(hs1-bs1)*0.5+(hs1*0.5-hny)*(hny+hs1*0.5)*bs1*0.5;
					double Zcup = h*(b*0.5-hny)*(b*0.5+hny)*0.5-Zsup;
					MplRd_y = 0.85*fcd*Zcup+2*Zsup*fsd;
				}
			}		
		}
		else if(secttype==2){
			MplRd_y = 999;
		}
		else if(secttype==3){		//sect3 begin
			//假定hnx<dsx-bb/2
			checksp1 = dsx-bb2*0.5;
			checksp2 = dsx+bb2*0.5;
			hny = NpmRd/((2*h-4*bs1)*(0.85*fcd)+4*bs1*fsd);
			if((checksp1-hny)>0){	//满足
				flagok = true;
				Zsxn = 0;
			}
			//假定dsx-bb/2 < hnx < dsx+bb/2
			if(!flagok){
				hny = (NpmRd+4*checksp1*hh2*(fyd-0.85*fcd))/((2*h-4*hh2-4*bs1)*(0.85*fcd)+4*hh2*fyd+4*bs1*fsd);
				if((hny-checksp1)>0 && (checksp2-hny)>0){
					flagok = true;
					Zsxn = 2*hh2*(hny-checksp1)*(hny+checksp1);
				}
			}
			//假定dsx+bb2/2 < hnx <ds1x
			checksp1 = dsx+bb2*0.5;
			checksp2 = ds1x;
			if(!flagok){
				hny = (NpmRd-4*hh2*bb2*fyd+4*hh2*bb2*0.85*fcd)/((2*h-4*bs1)*(0.85*fcd)+4*bs1*fsd);
				if((hny-checksp1)>0 && (checksp2-hny)>0){
					flagok = true;
					Zsxn = m_Sect.Zsy;
				}
			}
			if(!flagok){		//保证不出错
				hny = ds1x;
				Zsxn = m_Sect.Zsy;
			}
		}		//sect3 end
		else if(secttype==4){		//sect4 begin
			//假定 hnx< bb/2
			checksp1 = bb*0.5;
			hny = NpmRd/((2*h-2*hh-4*bs1)*(0.85*fcd)+2*hh*fyd+4*bs1*fsd);
			if((checksp1-hny)>0){
				flagok = true;
				Zsxn = 2*hh*hny*hny;
			}
			//假定 bb/2 < hnx < dsy-hh/2
			if(!flagok){
				checksp1 = bb*0.5;
				checksp2 = dsy-hh*0.5;
				hny=(NpmRd+2*bb*hh*0.85*fcd-2*bb*hh*fyd)/((2*h-4*bs1)*0.85*fcd+4*bs1*fsd);
				if((hny-checksp1)>0 && (checksp2-hny)>0){
					flagok = true;
					Zsxn = hh*bb*bb*0.5;
				}
			}
			//假定dsx-hh/2 < hnx <dsx+hh/2
			if(!flagok){
				checksp1 = dsx-hh*0.5;
				checksp2 = dsx+hh*0.5;
				hny = (NpmRd+2*bb*hh*0.85*fcd-2*(dsx-hh*0.5)*bb*0.85*fcd+2*(dsx-hh*0.5)*bb*fyd-2*bb*hh*fyd)/((2*h-2*bb-4*bs1)*0.85*fcd+2*bb*fyd+4*bs1*fsd);
				if((hny-checksp1)>0 && (checksp2-hny)>0){
					flagok = true;
					Zsxn = hh*bb*bb*0.5+0.5*bb*(hny-checksp1)*(3*hny+checksp1);
				}
			}
			//假定 dsx+hh/2 < hnx < ds1x
			if(!flagok){
				checksp1 = dsx+hh*0.5;
				checksp2 = ds1x;
				hny = (NpmRd-4*bb*hh*fyd+4*bb*hh*0.85*fcd)/((2*h-4*bs1)*0.85*fcd+4*bs1*fsd);
				if((hny-checksp1)>0 && (checksp2-hny)>0){
					flagok = true;
					Zsxn = m_Sect.Zsy;
				}
			}
		}		//sect4 end
		else if(secttype==5){
			//skip
		}
		else if(secttype==6){		//sect6 begin 参考截面3
			//假定hnx<dsx-bb/2
			checksp1 = dsx-bb2*0.5;
			checksp2 = dsx+bb2*0.5;
			hny = NpmRd/((2*h-4*bs1)*(0.85*fcd)+4*bs1*fsd);
			if((checksp1-hny)>0){	//满足
				flagok = true;
				Zsxn = 0;
			}
			//假定dsx-bb2/2 < hnx < dsx+bb2/2
			if(!flagok){
				hny = (NpmRd+6*checksp1*hh2*(fyd-0.85*fcd))/((2*h-6*hh2-4*bs1)*(0.85*fcd)+6*hh2*fyd+4*bs1*fsd);
				if((hny-checksp1)>0 && (checksp2-hny)>0){
					flagok = true;
					Zsxn = hh2*(hny-checksp1)*3*(hny+checksp1);
				}
			}
			//假定dsx+bb2/2 < hnx <ds1x
			checksp1 = dsx+bb2*0.5;
			checksp2 = ds1x;
			if(!flagok){
				hny = (NpmRd-6*hh2*bb2*fyd+6*hh2*bb2*0.85*fcd)/((2*h-6*bs1)*(0.85*fcd)+4*bs1*fsd);
				if((hny-checksp1)>0 && (checksp2-hny)>0){
					flagok = true;
					Zsxn = m_Sect.Zsy;
				}
			}
		}		//sect6 end
		else if(secttype==7){		//sect7 begin

	/////////////////////////////
			//假定 hny < bb/2
			checksp1 = bb2*0.5;
			hny = NpmRd/((2*b-4*hh2-4*bs1)*0.85*fcd+4*bb2*fyd+4*bs1*fsd);
			if((checksp1-hny)>0){
				flagok = true;
				Zsxn = 2*bb2*hny*hny;
			}
			//假定 bb/2 <hny< dsy-bb/2
			if(!flagok){
				checksp1 = bb2*0.5;
				checksp2 = dsy-bb2*0.5;
				hny	 = (NpmRd+2*bb2*hh2*0.85*fcd-2*bb2*hh2*fyd)/((2*b-4*bs1)*0.85*fcd+4*bs1*fsd);
				if((hny-checksp1)>0 && (checksp2-hny)>0){
					flagok = true;
					Zsxn = bb2*bb2*hh2*0.5;
				}
			}
			//假定 dsy-bb2/2 < hny < dsy+bb2/2
			if(!flagok){
				checksp1 = dsy-bb2*0.5;
				checksp2 = dsy+bb2*0.5;
				hny = (NpmRd+2*bb2*hh2*0.85*fcd-4*checksp1*hh2*0.85*fcd-2*bb2*hh2*fyd+4*checksp1*hh2*fyd)/((2*b-4*hh2-4*bs1)*0.85*fcd+4*hh2*fyd+4*bs1*fsd);
				if((hny-checksp1)>0 && (checksp2-hny)>0){
					flagok = true;
					Zsxn = bb2*hh2*hh2*0.5+2*hh2*(hny-checksp1)*(hny+checksp1);
				}
			}
			//假定 dsy+hh/2 < hny < ds1y
			if(!flagok){
				checksp1 = dsy+bb2*0.5;
				checksp2 = ds1y;
				hny = (NpmRd-4*bb2*hh2*fyd+4*bb2*hh2*0.85*fcd)/((2*b-4*bs1)*0.85*fcd+4*bs1*fsd);
				if((hny-checksp1)>0 && (checksp2-hny)>0){
					flagok = true;
					Zsxn = m_Sect.Zsx;
				}
			}
		}		//sect7 end

	///////////////////////////



		else if(secttype==8){
			checksp1 = bs1*0.5;
			checksp2 = b*0.5;
			double hc=h-2*tf;
			double bc=(b-tw)*0.5;
			// case 1							
			hny=(0.85*fcd*0.5*b*hc-hw*tw*fyd-2*m_Sect.As*fsd)/(4*tf*fyd+0.85*hc*fcd);
			if(checksp2>hny&& hny>=checksp1)
			{
				flagok = true;
				MplRd_y = 0.85*fcd*hc*(b*0.5-hny)*(b*0.5+hny)+2*(bf*0.5-hny)*(bf*0.5+hny)*tf*fcd;
			}
			// case 2							
			if(!flagok)
			{
				checksp1 = tw*0.5;
				checksp2 = bs1*0.5;
				hny=(0.85*fcd*0.5*b*hc-hw*tw*fyd)/(4*tf*fyd+0.85*hc*fcd);
				if(checksp2>hny&&hny>=checksp1)
				{
					flagok = true;
					MplRd_y = 0.85*fcd*(hc*(b*0.5-hny)*(b*0.5+hny)-m_Sect.As*bs1*0.5) +
						             2*(bf*0.5-hny)*(bf*0.5+hny)*tf*fyd+
									 2*m_Sect.As*bs1*fsd;
				}
			}
			// case 3							
			if(!flagok)
			{
				checksp1 = -tw*0.5;
				checksp2 = tw*0.5;
				hny=0.85*fcd*bc*hc/2*h*fyd;
				if(checksp2>hny&&hny>checksp1)
				{
					flagok = true;
					MplRd_y = 0.85*fcd*(hc*bc*(bc+tw)*0.5-m_Sect.As*bs1*0.5) +
						             2*( (bf*0.5-hny)*(bf*0.5+hny)*tf+hw*(tw*0.5-hny)*(tw*0.5+hny)*0.5)*fyd+
						             2*m_Sect.As*bs1*fsd;
				}
			}
		}
			
		GPara::hny = hny;
		//计算MplRd
		if(secttype!=1 && secttype!=8){
			Zr2xn = 2*bs1*hny*hny;
			Zcxn = h*hny*hny - Zr2xn - Zsxn;
			MplRd_y = MmaxRd_y-Zr2xn*fsd-Zsxn*fyd-0.5*Zcxn*0.85*fcd;
		}
		double Vax = V_x*(m_Sect.Zsx*fyd/MplRd_x);
		double Vay = V_y*(m_Sect.Zsy*fyd/MplRd_y);

		if(GPara::m_DesnPara->CodeType==1)
		{
			if(Vax>0.5*GPara::VplRd_y){
				roux = (2*V_x/GPara::VplRd_y-1)*(2*V_x/GPara::VplRd_y-1);
			}
			if(Vay>0.5*GPara::VplRd_z){
				rouy = (2*V_y/GPara::VplRd_z-1)*(2*V_y/GPara::VplRd_z-1);
			}
		}
		GPara::roux = roux;
		GPara::rouy = rouy;

	}



	NMPoints_ nm_point;
	if(GPara::m_DesnPara->CodeType==2){
		float faib = GPara::m_DesnPara->faib;
		float faic = GPara::m_DesnPara->faic;
		MplRd_x = MplRd_x*faib;
		MplRd_y = MplRd_y*faib;
		NpmRd = NpmRd*faic;
		MmaxRd_x = MmaxRd_x*faib;
		MmaxRd_y = MmaxRd_y*faib;
	}

	//A点 Na = Nplrd  Ma = 0
	nm_point.x_M = 0;
	nm_point.y_N = GPara::NplRd*1.e-3;
	NMPoints_x.Add(nm_point);
	//C点
	nm_point.x_M = MplRd_x*1.e-6;
	nm_point.y_N = NpmRd*1.e-3;
	NMPoints_x.Add(nm_point);
	//D点
	nm_point.x_M = MmaxRd_x*1.e-6;
	nm_point.y_N = 0.5*NpmRd*1.e-3;
	NMPoints_x.Add(nm_point);
	//B点 
	nm_point.x_M = MplRd_x*1.e-6;
	nm_point.y_N = 0;
	NMPoints_x.Add(nm_point);

	//y方向
	//A点 Na = Nplrd  Ma = 0
	nm_point.x_M = 0;
	nm_point.y_N = GPara::NplRd*1.e-3;
	NMPoints_y.Add(nm_point);
	//C点
	nm_point.x_M = MplRd_y*1.e-6;
	nm_point.y_N = NpmRd*1.e-3;
	NMPoints_y.Add(nm_point);
	//D点
	nm_point.x_M = MmaxRd_y*1.e-6;
	nm_point.y_N = 0.5*NpmRd*1.e-3;
	NMPoints_y.Add(nm_point);
	//B点 
	nm_point.x_M = MplRd_y*1.e-6;
	nm_point.y_N = 0;
	NMPoints_y.Add(nm_point);
	
	//再存一套考虑长度效应的点
	for(int i=0;i<4;i++){
		float ks = LengthEffect(1);
		nm_point.y_N = NMPoints_x[i].y_N*ks;
		nm_point.x_M = NMPoints_x[i].x_M;
		NMPoints_x_Length.Add(nm_point);
		ks = LengthEffect(2);
		nm_point.y_N = NMPoints_y[i].y_N*ks;
		nm_point.x_M = NMPoints_y[i].x_M;
		NMPoints_y_Length.Add(nm_point);
	}

	GPara::MyplNRd = MplRd_x;
	GPara::MzplNRd = MplRd_y;

	//存一下结果
	GlobalResult::NplRd = GPara::NplRd;
	GlobalResult::NpmRd = NpmRd;
	GlobalResult::MmaxRd_y = MmaxRd_x;
	GlobalResult::MmaxRd_z = MmaxRd_y;
	GlobalResult::MplRd_y = MplRd_x;
	GlobalResult::MplRd_z = MplRd_y;
	return true;
}

void MegaCapacityCal::OutToFile(CFile &OutFile,int &j,CString String[])
{
	for(int i=0;i<j;i++){
		int length=String[i].GetLength();
		OutFile.Write(String[i],length);
	}
	j=0;
}

bool MegaCapacityCal::JudgeSect()
{
	return true;
}

void MegaCapacityCal::ReduceNMCurve(double N[][FirbePoints],double M[][FirbePoints])
{
	return;
}

float MegaCapacityCal::LengthEffect(const int yz)
{
	float ks=1.f;
	if(GPara::m_DesnPara->CodeType==1){
		if(yz==1)
			ks = GPara::ks_y;
		else
			ks = GPara::ks_z;
	}
	else if(GPara::m_DesnPara->CodeType==2){
		if(yz==1)
			ks = fabs(GPara::Pn_y/GPara::NplRd);
		else
			ks = fabs(GPara::Pn_z/GPara::NplRd);
	}
	else if(GPara::m_DesnPara->CodeType==3){
		ks = fabs(GPara::Pn_y/GPara::NplRd);
	}
	return ks;
}
void MegaCapacityCal::TxtLegends(float x1, float y1, float x2, float y2, float zfb, float zfh,CString str)
{
	char* ZFSTR;
	m_pCfgBas->Line(x1,y1,x2,y2);	
	int N=str.GetLength();	
	ZFSTR=str.GetBuffer(str.GetLength());	
	float Xzf=x2+zfb; 
	float Yzf=y2;
	int IX=3;
	int IY=2;
	float ANGLE=0;
	m_pCfgBas->Drawzf(N,ZFSTR,sizeof(ZFSTR),Xzf,Yzf,zfb,zfh,IX,IY,ANGLE);
}

void MegaCapacityCal::DrawPDMCurve(const int yz)
{
	if(NMPoints_x_nominal.GetSize()<1) return;
	Force_combine Colmforce;
	Colmforce.GetForce();
	Colmforce.HeZai_combine();
	int cenghao,xianxing,xiankuan,yanse,fanhui;
	int N,n2,IX,IY;
	float Xzf,Yzf,ANGLE;
	CString str;
	char* ZFSTR;
	//画图
	int iWay = 99;
//  int iWay = 88;
//	char Dir[256];
//	GetCurrentDirectory( 256, Dir ); 
	CString StrFile;
	int selcurve = yz%10;		//存的时候，10位是y,z轴，个位是控制画曲线的上限
	if(yz>10 && yz<20){
		if(selcurve<2)
			StrFile=_T("PDM_NM_y_design.t");
//			StrFile.Format(_T("%s\\PDM_NM_y_design.t"),Dir);
		else
			StrFile=_T("PDM_NM_y.t");
//			StrFile.Format(_T("%s\\PDM_NM_y.t"),Dir);
		
	}
	else{
		if(selcurve<2)
//			StrFile.Format(_T("%s\\PDM_NM_z_design.t"),Dir);
			StrFile=_T("PDM_NM_z_design.t");
		else
//			StrFile.Format(_T("%s\\PDM_NM_z.t"),Dir);
			StrFile=_T("PDM_NM_z.t");
	}
	int n1=StrFile.GetLength();
	char FileName[256];
	strcpy(FileName,StrFile);
	FileName[n1]=char(0);
	m_pCfgBas->Closfl(iWay);
	m_pCfgBas->Setcfg();
	m_pCfgBas->Openfl(iWay,FileName,n1,n1);
	m_pCfgBas->Nowfil(iWay);
	m_pCfgBas->Tobegin0(iWay);
	int isave=1;
	m_pCfgBas->Ifsave(isave);
	int ibcolor=15;
	m_pCfgBas->Bcolor(ibcolor);
	m_pCfgBas->Nowbcl(ibcolor);
	int narcl=72;
	m_pCfgBas->Narcl(narcl);

	float max_x=0.f;
	float max_y=0.f;
	float  size_x=4000;
	float  size_y=3000;
	float  scale_x=1;
	float  scale_y=1;
	{
		for(int i=0;i<NMPoints_x_nominal.GetSize();i++)
		{
			float xtmp,ytmp;
			if(yz>10 && yz<20){
				xtmp = NMPoints_x_nominal[i].x_M;				
				ytmp = NMPoints_x_nominal[i].y_N;				
			}
			else{
				xtmp = NMPoints_y_nominal[i].x_M;				
				ytmp = NMPoints_y_nominal[i].y_N;				
			}
			max_x=max(max_x,xtmp);
			max_y=max(max_y,ytmp);
		}
		scale_x=max_x/size_x;
		scale_y=max_y/size_y;
		int iunit=1;
		float bl=1;
		float size_z=size_x;
		float xoo=0.;
		float yoo=0.;
		float zoo=0;
//		m_pCfgBas->Grfsiz(size_x.size_y,iunit,bl);
		m_pCfgBas->Grfild(size_x,size_y,size_z,iunit,bl,xoo,yoo,zoo);
//		m_pCfgBas->Wndows();
		InitUCS(scale_x,scale_y);
		
	}
	float XM1=45*scale_x;
	float YM1=60*scale_y;
	float XM2=55*scale_x;
	float YM2=70*scale_y;

	
	//在这里画线
	cenghao=1;
	xianxing=1;	
	yanse=2;
	fanhui=0;
	xiankuan=2;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	float ytl1=max_y-3*YM1;
	float ytl2=ytl1;
	float xtl1=0.8*max_x;
	float xtl2=xtl1+XM1*4;	
	TxtLegends(xtl1, ytl1, xtl2, ytl2,XM1, YM1,_T("PDM-Curve"));

	float temp_pt_x;
	float temp_pt_y;
	CString* temp_pt_info = new CString;
	{
		Points_info temp_info;
		for(int i=0;i<NMPoints_x.GetSize()-1;i++)
		{
			float x1,x2,y1,y2;
			if(yz>10 && yz<20){
				x1 = NMPoints_x[i].x_M;
				x2 = NMPoints_x[i+1].x_M;
				y1 = NMPoints_x[i].y_N;
				y2 = NMPoints_x[i+1].y_N;
			}
			else{
				x1 = NMPoints_y[i].x_M;
				x2 = NMPoints_y[i+1].x_M;
				y1 = NMPoints_y[i].y_N;
				y2 = NMPoints_y[i+1].y_N;
			}
			
			m_pCfgBas->Line(x1,y1,x2,y2);
			if(selcurve==2){
				//进行节点离散,取50个点离散
				for(int j=0;j<50;j++){
					temp_pt_x = (x2-x1)*j/50+x1;
					temp_pt_y = (y2-y1)*j/50+y1;
					temp_pt_info->Format(_T("N=%0.fkN,M=%0.fkN*m"),temp_pt_y/1.e3,temp_pt_x/1.e6);
					float *pt_x = &temp_pt_x;
					float *pt_y = &temp_pt_y;
					temp_info.pos_x->Add(*pt_x);
					temp_info.pos_y->Add(*pt_y);
					temp_info.pos_info->Add(*temp_pt_info);
				}
			}	
		}
		if(selcurve==2)  	GPara::Pt_info.Add(temp_info);
		yanse=4;
		m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);

		ytl1=ytl1-1.5*YM1;
		ytl2=ytl1;
		TxtLegends(xtl1, ytl1, xtl2, ytl2,XM1, YM1,_T("PDM-Curve-nominal"));

		Points_info temp_info_2;
		for(int i=0;i<NMPoints_x_nominal.GetSize()-1;i++)
		{
			float x1,x2,y1,y2;
			if(yz>10 && yz<20){
				x1 = NMPoints_x_nominal[i].x_M;
				x2 = NMPoints_x_nominal[i+1].x_M;
				y1 = NMPoints_x_nominal[i].y_N;
				y2 = NMPoints_x_nominal[i+1].y_N;
			}
			else{
				x1 = NMPoints_y_nominal[i].x_M;
				x2 = NMPoints_y_nominal[i+1].x_M;
				y1 = NMPoints_y_nominal[i].y_N;
				y2 = NMPoints_y_nominal[i+1].y_N;
			}
			m_pCfgBas->Line(x1,y1,x2,y2);
			if(selcurve==2){
				//进行节点离散,取50个点离散
				for(int j=0;j<50;j++){
					temp_pt_x = (x2-x1)*j/50+x1;
					temp_pt_y = (y2-y1)*j/50+y1;
					temp_pt_info->Format(_T("N=%0.fkN,M=%0.fkN*m"),temp_pt_y/1.e3,temp_pt_x/1.e6);
					float *pt_x = &temp_pt_x;
					float *pt_y = &temp_pt_y;
					temp_info_2.pos_x->Add(*pt_x);
					temp_info_2.pos_y->Add(*pt_y);
					temp_info_2.pos_info->Add(*temp_pt_info);
				}
			}	
		}
		if(selcurve==2) 	GPara::Pt_info.Add(temp_info_2);

		}
	yanse=3;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	ytl1=ytl1-1.5*YM1;
	ytl2=ytl1;
	TxtLegends(xtl1, ytl1, xtl2, ytl2,XM1, YM1,_T("PDM-Curve-LengthEffect"));

		if(selcurve==2){		//绘制长度效应折减曲线
			Points_info temp_info_3;
		for(int i=0;i<NMPoints_x_Length.GetSize()-1;i++)
		{
			float x1,x2,y1,y2;
			if(yz>10 && yz<20)
			{
				x1 = NMPoints_x_Length[i].x_M;
				x2 = NMPoints_x_Length[i+1].x_M;
				y1 = NMPoints_x_Length[i].y_N;
				y2 = NMPoints_x_Length[i+1].y_N;
			}
			else{
				x1 = NMPoints_y_Length[i].x_M;
				x2 = NMPoints_y_Length[i+1].x_M;
				y1 = NMPoints_y_Length[i].y_N;
				y2 = NMPoints_y_Length[i+1].y_N;
			}
			
			m_pCfgBas->Line(x1,y1,x2,y2);
			for(int j=0;j<50;j++){
				temp_pt_x = (x2-x1)*j/50+x1;
				temp_pt_y = (y2-y1)*j/50+y1;
				temp_pt_info->Format(_T("N=%0.fkN,M=%0.fkN*m"),temp_pt_y/1.e3,temp_pt_x/1.e6);
				float *pt_x = &temp_pt_x;
				float *pt_y = &temp_pt_y;
				temp_info_3.pos_x->Add(*pt_x);
				temp_info_3.pos_y->Add(*pt_y);
				temp_info_3.pos_info->Add(*temp_pt_info);
			}
		}
		GPara::Pt_info.Add(temp_info_3);
	}

	//画x,y轴
	cenghao=1;
	xianxing=1;
	xiankuan=2;
	yanse=8;
	fanhui=0;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	max_x = max_x*1.2;
	max_y = max_y*1.2;
	double x_step = max_x/10;
	double y_step = max_y/10;
	float x_base,y_base;
	x_base = y_base = 0;
	//x轴
	float x_temp = 0.f;
	m_pCfgBas->Line(x_base,y_base,max_x,y_base);
	float x_jt=max_x-max_x/20;
	float y_jt=x_temp+max_x/50;
//	m_pCfgBas->Line(max_x,x_temp,x_jt,y_jt);
	x_jt=max_x-max_x/20;
	y_jt=x_temp-max_x/50;
//	m_pCfgBas->Line(max_x,x_temp,x_jt,y_jt);
	float zf_scale = 100;
//	
	int x_scale,y_scale;
	CString tempchar;
	tempchar.Format(_T("%.0f"),10*y_step);
	y_scale = tempchar.GetLength();
	y_scale = y_scale-2;
	tempchar.Format(_T("%.0f"),10*x_step);
	x_scale = tempchar.GetLength();
	x_scale = x_scale-2;

	//标注
	str.Format("M(kN·m) X");
	tempchar.Format(_T("e%d"),x_scale);
	str.Append(tempchar);
	n2=str.GetLength();
	ZFSTR=str.GetBuffer(str.GetLength());
	N=n2;
	Xzf=max_x; 
	Yzf=x_temp;
	IX=3;
	IY=1;
	ANGLE=0;
	xiankuan=1;	
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	m_pCfgBas->Drawzf(N,ZFSTR,n2,Xzf,Yzf,XM2,YM2,IX,IY,ANGLE);

	for(int i=0;i<10;i++){
		x_temp = (i+1)*y_step;
		m_pCfgBas->Line(x_base,x_temp,max_x,x_temp);
		//绘制网格坐标
		str.Format(_T("%1.f"),(i+1)*y_step/pow(10.f,y_scale));
		n2=str.GetLength();
		ZFSTR=str.GetBuffer(str.GetLength());
		N=n2;
		Xzf=0.; 
		Yzf=(i+1)*y_step;
		IX=1;
		IY=2;
		ANGLE=0;
		m_pCfgBas->Drawzf(N,ZFSTR,n2,Xzf,Yzf,XM1,YM1,IX,IY,ANGLE);
	}
	//y轴
	xiankuan=2;	
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	m_pCfgBas->Line(x_base,y_base,x_base,max_y);
	xiankuan=1;	
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);

	x_jt=x_temp+max_x/50;
	y_jt=max_y-max_x/20;
//	m_pCfgBas->Line(x_temp,max_y,x_jt,y_jt);
	x_jt=x_temp-max_x/50;
	y_jt=max_y-max_x/20;
//	m_pCfgBas->Line(x_temp,max_y,x_jt,y_jt);
	//标注
	str.Format("N(kN) X");
	tempchar.Format(_T("e%d"),y_scale);
	str.Append(tempchar);
	n2=str.GetLength();
	ZFSTR=str.GetBuffer(str.GetLength());
	N=n2;
	Xzf=x_base; 
	Yzf=max_y+YM2;
	IX=2;
	IY=1;
	ANGLE=0;
	m_pCfgBas->Drawzf(N,ZFSTR,n2,Xzf,Yzf,XM2,YM2,IX,IY,ANGLE);
	for(int i=0;i<10;i++){
		x_temp = (i+1)*x_step;
		m_pCfgBas->Line(x_temp,y_base,x_temp,max_y);
		//x坐标
		str.Format(_T("%1.f"),(i+1)*x_step/pow(10.f,x_scale));
		n2=str.GetLength();
		ZFSTR=str.GetBuffer(str.GetLength());
		N=n2;
		Xzf=(i+1)*x_step; 
		Yzf=y_base ;
		IX=2;
		IY=3;
		ANGLE=0;
		m_pCfgBas->Drawzf(N,ZFSTR,n2,Xzf,Yzf,XM1,YM1,IX,IY,ANGLE);
	}
	
	//文字
/*	str.Format("Axial force-bending moment interaction curve");
	n2=str.GetLength();
	ZFSTR=str.GetBuffer(str.GetLength());
	N=n2;
	Xzf=max_x/2; 
	Yzf=y_base - 4*zf_scale;
	XM=3.2*zf_scale;
	YM=4*zf_scale;
	IX=2;
	IY=2;
	ANGLE=0;
	m_pCfgBas->Drawzf(N,ZFSTR,n2,Xzf,Yzf,XM,YM,IX,IY,ANGLE);
*/
	m_pCfgBas->Outucs();
	int ivpd=0;
	m_pCfgBas->Xoyvpd(ivpd);
	m_pCfgBas->Redrwa();
	m_pCfgBas->Regens();
	m_pCfgBas->Zoome();

	CString pcxFileName=StrFile;

	pcxFileName=pcxFileName.Left(pcxFileName.Find(_T(".t")));

	n1=pcxFileName.GetLength();	
	strcpy(FileName,pcxFileName);
	int NRC[4];
	for(int i=0;i<4;i++) NRC[i]=1;
	int kw=0;
	m_pCfgBas->Mfpcx(FileName,sizeof(FileName),n1,NRC,kw);


//	m_pCfgBas->Closfl(iWay);

}

void MegaCapacityCal::DrawNMPoint(int Fiber,const int yz)
{
 	Force_combine Colmforce;
 	Colmforce.GetForce();
 	Colmforce.HeZai_combine();
	int cenghao,xianxing,xiankuan,yanse,fanhui;
	int N,n2,IX,IY;
	float Xzf,Yzf,XM,YM,ANGLE;
	CString str;
	char* ZFSTR;
	//画图
	int iWay = 99;
//	char Dir[256];
//	GetCurrentDirectory( 256, Dir ); 

	CString StrFile;
	CString StrOrgFile;
	if(Fiber==0){
		if(yz==1)
		{
		
			StrOrgFile=_T("PDM_NM_y.t");
			StrFile=_T("PDM_NM_y_Point.t");
		}
		else
		{
			StrOrgFile=_T("PDM_NM_z.t");
			StrFile=_T("PDM_NM_z_Point.t");
		}
	}
	else{
		if(yz==1)
		{
			StrOrgFile=_T("Fiber_NM_y.t");
			StrFile=_T("Fiber_NM_y_Point.t");
		}			
		else
		{
			StrOrgFile=_T("Fiber_NM_z.t");
			StrFile=_T("Fiber_NM_z_Point.t");
		}
	}
	
	int n1=StrFile.GetLength();
	char FileName[256];
	strcpy(FileName,StrFile);
	FileName[n1]=char(0);
	int n0=StrOrgFile.GetLength();
	char FileName0[256];
	strcpy(FileName0,StrOrgFile);
	FileName0[n0]=char(0);

	m_pCfgBas->Copfil(FileName0,n0,FileName,n1);

	int itmp=91;
	m_pCfgBas->Closfl(itmp);

	m_pCfgBas->Closfl(iWay);
//  这段是续画T文件
	m_pCfgBas->Setcfg();
	m_pCfgBas->Openfl(iWay,FileName,n1,n1);
	m_pCfgBas->Nowfil(iWay);
//    m_pCfgBas->Redrwf();   
	int isave=1;
	m_pCfgBas->Ifsave(isave);
	int ibc=15;
	m_pCfgBas->Bcolor(ibc);
	m_pCfgBas->Nowbcl(ibc);
	float XRI,YRI,ZRI,RTI,XOI,YOI,ZOI;
	int IUNITI;
	XRI=9999;
	int IFNE=0;
	m_pCfgBas->Grfild(XRI,YRI,ZRI,IUNITI,RTI,XOI,YOI,ZOI);
	{
		int NCK=1;
		float PMDU[9];		
		m_pCfgBas->Chkucs(NCK,PMDU,IFNE);
		if(IFNE==1)
		{
			m_pCfgBas->Usercs(PMDU);
		}

	}

//	InitUCS();

	//画内力点
	cenghao=1;
	xianxing=1;
	yanse=10;
	fanhui=0;
	xiankuan = 10;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	double x,y,z;
	z = 0;
	double Myed,Mzed,Ned;
	double Med1[2],Med2[2];
	float kky,kkz;
	for(int i=0;i<GPara::m_DesnPara->n_Combines;i++){
		for(int j=0;j<Parts;j++){
			if(GPara::m_DesnPara->ifSecondEffect){
				Med1[0] = Colmforce.My_zuhe[i][0];
				Med1[1] = Colmforce.My_zuhe[i][12];
				Med2[0] = Colmforce.Mz_zuhe[i][0];
				Med2[1] = Colmforce.Mz_zuhe[i][12];
				Ned = Colmforce.N_zuhe[i][j];
				EUROCode::AmplifyM(Med1,Med2,kky,kkz,i);
				if(Colmforce.My[i][j]>0)
					Myed = kky*Colmforce.My[i][j];
				else
					Myed = Colmforce.My[i][j] - (kky-1)*Colmforce.My[i][j];
				if(Colmforce.Mz[i][j]>0)
					Mzed = kkz*Colmforce.Mz[i][j];
				else
					Mzed = Colmforce.Mz[i][j] - (kkz-1)*Colmforce.Mz[i][j];
			}
			else{
				Myed = Colmforce.My_zuhe[i][j];
				Mzed = Colmforce.Mz_zuhe[i][j];
				Ned  = Colmforce.N_zuhe[i][j];
			}
			if(yz==1)
				x = Myed*1.e-6;                         //          /5000000000;
			else
				x = Mzed*1.e-6;                         //   /5000000000;
			y = Ned*1.e-3;                              //   /10000000;
			float tempx,tempy,tempz;
			tempx = fabs(x);
			tempy = y;
			tempz = z;
			m_pCfgBas->Point(tempx,tempy);                       // -> Pnt3d(tempx,tempy,tempz);


		}
	}
	if(IFNE==1)  m_pCfgBas->Outucs();

	int ivpd=0;
	m_pCfgBas->Xoyvpd(ivpd);
	m_pCfgBas->Redrwa();
	m_pCfgBas->Regens();
	m_pCfgBas->Zoome();

	CString pcxFileName=StrFile;
	pcxFileName=pcxFileName.Left(pcxFileName.Find(_T(".t")));

	n1=pcxFileName.GetLength();	
	strcpy(FileName,pcxFileName);
	int NRC[4];
	for(int i=0;i<4;i++) NRC[i]=1;
	int kw=0;
	m_pCfgBas->Mfpcx(FileName,sizeof(FileName),n1,NRC,kw);


}
//纤维模型的曲线点数
int MegaCapacityCal::NumPointFiberCurve(int NoCurve, int isYZ)
{
	int n=0;
	if(isYZ==1) 
	{
		if(NoCurve==2) n=GPara::m_DesnPara->Fiberpoints_y;  
		else n=GPara::m_DesnPara->Fiberpoints_y_k;  // 0,1
	}else // isYZ==2
	{
		if(NoCurve==2) n=GPara::m_DesnPara->Fiberpoints_z;  
		else n=GPara::m_DesnPara->Fiberpoints_z_k;  // 0,1
	}
	return n;
}

//画纤维模型的曲线
void MegaCapacityCal::DrawFiberCurve(double N[][FirbePoints],double M[][FirbePoints],const int yz)
{
	int cenghao,xianxing,xiankuan,yanse,fanhui;
	int NN,n2,IX,IY;
	float Xzf,Yzf,ANGLE;
	CString str;
	char* ZFSTR;
	int iWay = 99;
	int npoints;
	CString StrFile;
	int selcurve = yz%10;		//存的时候，10位是y,z轴，个位是控制画曲线的上限
	int isyz=yz/10;
	StrFile=_T("Fiber_NM_");
	if(isyz==1) StrFile+=_T("y");
	else  StrFile+=_T("z");
	if(selcurve<3) StrFile+=_T("_nominal.t");
	else StrFile+=_T(".t");


	int n1=StrFile.GetLength();
	char FileName[256];
	strcpy(FileName,StrFile);
	FileName[n1]=char(0);
	int itmp=91;
	m_pCfgBas->Closfl(itmp);
	m_pCfgBas->Closfl(iWay);
	int isave=1;

	m_pCfgBas->Setcfg();
	m_pCfgBas->Openfl(iWay,FileName,sizeof(FileName),n1);
	m_pCfgBas->Nowfil(iWay);
	m_pCfgBas->Savnewtoary(iWay);	
	m_pCfgBas->Tobegin0(iWay);
	
	m_pCfgBas->Ifsave(isave);
	int ibc=15;
	m_pCfgBas->Bcolor(ibc);
	m_pCfgBas->Nowbcl(ibc);
	float max_x=0;
	float max_y=0;
	float max_tmp;
	for(int j=0;j<selcurve;j++)
	{
		int npoints1=NumPointFiberCurve(j, isyz);
		Points_info temp_info;
		for(int i=0;i<npoints1;i++)
		{
			if(M[j][i]*1.e-6>max_x) max_x=M[j][i]*1.e-6;			
			if(N[j][i]*1.e-3>max_y) max_y=N[j][i]*1.e-3;
		}
	}

	int iunit=1;
	float bl=1;
	float xoo=0;
	float yoo=0;
	float zoo=0;
	float  size_x=4000;
	float  size_y=3000;
	float size_z=size_x;
	float scale_x=max_x/size_x;
	float scale_y=max_y/size_y;
	m_pCfgBas->Grfild(size_x,size_y,size_z,iunit,bl,xoo,yoo,zoo);
	InitUCS(scale_x,scale_y);
	float XM1=45*scale_x;
	float YM1=60*scale_y;
	float XM2=55*scale_x;
	float YM2=70*scale_y;


//	InitUCS();
	//在这里画线
	cenghao=1;
	xianxing=1;
	xiankuan=2;
	yanse=1;
	fanhui=0;
	
	float max_xy=0.f;
	float max_xx=0.f;
//	int npoints = result.size()*0.5;
//	 = FirbePoints;
	float temp_pt_x,temp_pt_y;
	CString* temp_pt_info=new CString;

	for(int j=0;j<selcurve;j++)
	{
		Points_info temp_info;
		npoints=NumPointFiberCurve(j,isyz);
		max_xx = max_xy=0.f;
		yanse++;
		m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
		if(selcurve==3){
			temp_pt_x = 0;
			temp_pt_y = N[j][0]*1.e-3;
			float *pt_x = &temp_pt_x;
			float *pt_y = &temp_pt_y;
			temp_info.pos_x->Add(*pt_x);
			temp_info.pos_y->Add(*pt_y);
			temp_pt_info->Format(_T("N=%.0fkN,M=%.0fkN*m"),temp_pt_y,temp_pt_x);
			temp_info.pos_info->Add(*temp_pt_info);
		}
		for(int i=0;i<npoints-1;i++)
		{
			float x1 = M[j][i]*1.e-6;
			float x2 = M[j][i+1]*1.e-6;
			float y1 = N[j][i]*1.e-3;
			float y2 = N[j][i+1]*1.e-3;
			m_pCfgBas->Line(x1,y1,x2,y2);
			//存入pt_info信息
			if(selcurve==3){
				temp_pt_x = x2;
				temp_pt_y = y2;
				temp_pt_info->Format(_T("N=%.0fkN,M=%.0fkN*m"),temp_pt_y,temp_pt_x);
				float *pt_x = &temp_pt_x;
				float *pt_y = &temp_pt_y;
				temp_info.pos_x->Add(*pt_x);
				temp_info.pos_y->Add(*pt_y);
				temp_info.pos_info->Add(*temp_pt_info);
			}
		}
	//画内力点
/*	xiankuan = 10;
	yanse = 11;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	float x,y;
	float z;
	z = 0;
	//最大弯矩点
	x = max_xx;
	y = max_xy;
	m_pCfgBas->Pnt3d(x,y,z);
	//最大轴力点
	x = M[j][0]/5000000000;
	y = N[j][0]/10000000;
	m_pCfgBas->Pnt3d(x,y,z);
	//最小轴力点
	x = M[j][npoints-1]/5000000000;
	y = N[j][npoints-1]/10000000;
	m_pCfgBas->Pnt3d(x,y,z);*/
	if(selcurve==3)
		GPara::Pt_info.Add(temp_info);     
	}


	//画x,y轴
	cenghao=1;
	xianxing=2;
	xiankuan=1;
	yanse=8;
	fanhui=0;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	max_x = max_x*1.2;
	max_y = max_y*1.2;
	double x_step = max_x/10;
	double y_step = max_y/10;
	float x_base,y_base;
	x_base=y_base=0.f;
	//x轴
	float x_temp = 0.f;
	m_pCfgBas->Line(x_base,y_base,max_x,y_base);

	float x_jt=max_x-max_x/20;
	float y_jt=x_temp+max_x/50;
//	m_pCfgBas->Line(max_x,x_temp,x_jt,y_jt);
	x_jt=max_x-max_x/20;
	y_jt=x_temp-max_x/50;
//	m_pCfgBas->Line(max_x,x_temp,x_jt,y_jt);
	//横向轴网
	float zf_scale = 100;
	
	int x_scale,y_scale;
	CString tempchar;
	tempchar.Format(_T("%.0f"),10*y_step);
	y_scale = tempchar.GetLength();
	y_scale = y_scale-2;
	tempchar.Format(_T("%.0f"),10*x_step);
	x_scale = tempchar.GetLength();
	x_scale = x_scale-2;
	xiankuan=1;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	for(int i=0;i<10;i++){
		x_temp = (i+1)*y_step;
		m_pCfgBas->Line(x_base,x_temp,max_x,x_temp);
		//绘制网格坐标
		str.Format(_T("%.1f"),(i+1)*y_step/pow(10.f,y_scale));
		n2=str.GetLength();
		ZFSTR=str.GetBuffer(str.GetLength());
		NN=n2;
		Xzf=0.; 
		Yzf=(i+1)*y_step;
		IX=1;
		IY=2;
		ANGLE=0;
		m_pCfgBas->Drawzf(NN,ZFSTR,n2,Xzf,Yzf,XM1,YM1,IX,IY,ANGLE);
	}
	//标注
	str.Format("M(kN·m) x");
	tempchar.Format(_T("e%d"),x_scale);
	str.Append(tempchar);
	n2=str.GetLength();
	ZFSTR=str.GetBuffer(str.GetLength());
	NN=n2;
	Xzf=max_x; 
	Yzf=y_base;
	IX=3;
	IY=1;
	ANGLE=0;
	m_pCfgBas->Drawzf(NN,ZFSTR,n2,Xzf,Yzf,XM2,YM2,IX,IY,ANGLE);

	//y轴
	xiankuan=2;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	m_pCfgBas->Line(x_base,y_base,x_base,max_y);
	xiankuan=1;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);

	x_jt=x_temp+max_x/50;
	y_jt=max_y-max_x/20;
//	m_pCfgBas->Line(x_temp,max_y,x_jt,y_jt);
	x_jt=x_temp-max_x/50;
	y_jt=max_y-max_x/20;
//	m_pCfgBas->Line(x_temp,max_y,x_jt,y_jt);
	for(int i=0;i<10;i++){
		x_temp = (i+1)*x_step;
		m_pCfgBas->Line(x_temp,y_base,x_temp,max_y);
		//x坐标
		str.Format(_T("%1.f"),(i+1)*x_step/pow(10.f,x_scale));
		n2=str.GetLength();
		ZFSTR=str.GetBuffer(str.GetLength());
		NN=n2;
		Xzf=x_temp; 
		Yzf=y_base ;
		IX=1;
		IY=3;
		ANGLE=0;
		m_pCfgBas->Drawzf(NN,ZFSTR,n2,Xzf,Yzf,XM1,YM1,IX,IY,ANGLE);
	}
	//标注
	str.Format("N(kN) x");
	tempchar.Format(_T("e%d"),y_scale);
	str.Append(tempchar);
	n2=str.GetLength();
	ZFSTR=str.GetBuffer(str.GetLength());
	NN=n2;
	Xzf=x_base; 
	Yzf=max_y+YM2;
	IX=2;
	IY=1;
	ANGLE=0;
	m_pCfgBas->Drawzf(NN,ZFSTR,n2,Xzf,Yzf,XM2,YM2,IX,IY,ANGLE);

	//文字
/*	str.Format("Axial force-bending moment interaction curve");
	n2=str.GetLength();
	ZFSTR=str.GetBuffer(str.GetLength());
	NN=n2;
	Xzf=max_x/2; 
	Yzf=-2.2*4*zf_scale;
	XM=3.2*zf_scale;
	YM=4*zf_scale;
	IX=2;
	IY=1;
	ANGLE=0;
	m_pCfgBas->Drawzf(NN,ZFSTR,n2,Xzf,Yzf,XM,YM,IX,IY,ANGLE);
*/
	
//	int ivpd=0;
//	m_pCfgBas->Xoyvpd(ivpd);
//	m_pCfgBas->Redrwa();

	m_pCfgBas->Outucs();
	int ivpd=0;
	m_pCfgBas->Xoyvpd(ivpd);
	m_pCfgBas->Redrwa();
	m_pCfgBas->Regens();
	m_pCfgBas->Zoome();


	CString pcxFileName=StrFile;
	pcxFileName=pcxFileName.Left(pcxFileName.Find(_T(".t")));

	n1=pcxFileName.GetLength();	
	strcpy(FileName,pcxFileName);
	int NRC[4];
	for(int i=0;i<4;i++) NRC[i]=1;
	int kw=0;
	m_pCfgBas->Mfpcx(FileName,sizeof(FileName),n1,NRC,kw);


/*
	m_pCfgBas->Closfl(iWay);
	StrFile.Format(_T("blank.t"));
	n1=StrFile.GetLength();
	FileName[256];
	strcpy(FileName,StrFile);
	FileName[n1]=char(0);
	m_pCfgBas->Closfl(iWay);
	m_pCfgBas->Openfl(iWay,FileName,n1,n1);
	m_pCfgBas->Nowfil(iWay);
	m_pCfgBas->Tobegin0(iWay);
	m_pCfgBas->Closfl(iWay);
*/
}

double MegaCapacityCal::GetMFromN_PDM(double Ned,int yz)
{
	float Mypd;
	if(yz==1){	//y
		if(Ned>=NMPoints_x[3].y_N && Ned<=NMPoints_x[2].y_N){
			Mypd = NMPoints_x[3].x_M+(NMPoints_x[2].x_M-NMPoints_x[3].x_M)*(Ned-NMPoints_x[3].y_N)/(NMPoints_x[2].y_N-NMPoints_x[3].y_N);
		}
		else if(Ned>NMPoints_x[2].y_N && Ned<=NMPoints_x[1].y_N){
			Mypd = NMPoints_x[1].x_M+(NMPoints_x[2].x_M-NMPoints_x[1].x_M)*(NMPoints_x[1].y_N-Ned)/(NMPoints_x[1].y_N-NMPoints_x[2].y_N);
		}
		else if(Ned>NMPoints_x[1].y_N && Ned<NMPoints_x[0].y_N){
			Mypd = NMPoints_x[1].x_M*(NMPoints_x[0].y_N-Ned)/(NMPoints_x[0].y_N-NMPoints_x[1].y_N);
		}
		else{
			Mypd = 1;
		}
	}
	else{		//z
		if(Ned>=NMPoints_y[3].y_N && Ned<=NMPoints_y[2].y_N){
			Mypd = NMPoints_y[3].x_M+(NMPoints_y[2].x_M-NMPoints_y[3].x_M)*(Ned-NMPoints_y[3].y_N)/(NMPoints_y[2].y_N-NMPoints_y[3].y_N);
		}
		else if(Ned>NMPoints_y[2].y_N && Ned<=NMPoints_y[1].y_N){
			Mypd = NMPoints_y[1].x_M+(NMPoints_y[2].x_M-NMPoints_y[1].x_M)*(NMPoints_y[1].y_N-Ned)/(NMPoints_y[1].y_N-NMPoints_y[2].y_N);
		}
		else if(Ned>NMPoints_y[1].y_N && Ned<NMPoints_y[0].y_N){
			Mypd = NMPoints_y[1].x_M*(NMPoints_y[0].y_N-Ned)/(NMPoints_y[0].y_N-NMPoints_y[1].y_N);
		}
		else{
			Mypd = 1;
		}
	}
	return Mypd;
}

bool MegaCapacityCal::JudgeSect_Fiber(double N[][FirbePoints],double M[][FirbePoints])
{
	return true;
}

void MegaCapacityCal::InitUCS(float scale_x,float sclae_y)
{
	float pmucs[9];

	pmucs[0]=0.0;
	pmucs[1]=0.0;
	//pmucs[2]=nUcs++;
	pmucs[2]=0.0; //cyl 2017-08-08

	pmucs[3]=1.0/scale_x;
	pmucs[4]=1.0/sclae_y;
	pmucs[5]=1.0;

	pmucs[6]=0;
	pmucs[7]=0;
	pmucs[8]=0;
	m_pCfgBas->Usercs(pmucs);
//	m_pCfgBas->Outucs();
}



