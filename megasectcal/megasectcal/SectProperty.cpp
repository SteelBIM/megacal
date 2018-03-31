#include "StdAfx.h"
#include "SectProperty.h"
#include "GPara.h"
#include "Tools.h"

SectProperty::SectProperty(void)
{
	Zcx=Zcy=0;
	Icx=Icy=0;

	//钢筋特性
	Zsr1x=Zsr2x=0;
	Isr1x=Isr2x=0;
	Zsr1y=Zsr2y=0;
	Isr1y=Isr2y=0;
	hs1=hs2=0;
	bs1=bs2=0;
	d1y=d1x=0;	//b边上的钢筋等效钢板型心到截面中心的距离
	d2y=d2x=0;	//h边的同上
	bar_inv_x=bar_inv_y=0;
	//euro


	//型钢特性
	Zsx=Zsy=0;
	Isx=Isy=0;
	Is_x=Is_y=0;
	bb=hh=bb2=hh2=0;

	//截面校核时用的特性
	m_cover=0;	//保护层
	As=Asr=Ac=Ag=0;	
	Ig_x=Ig_y=0;
	bar_inv=0;
	bar_d=0;
	bartopro_x=bartopro_y=0;

	//强度指标
	fy=0;		//钢材屈服强度
	fs=0;		//钢筋屈服强度
	fc=0;		//混凝土屈服强度
}


SectProperty::~SectProperty(void)
{
}

void SectProperty::SectCal()
{
	EQAGCal();	//型钢等效
	EQASCal();	//钢筋等效
	EQACCal();	//混凝土相关计算
}

void SectProperty::SectGeoInfo()
{
	if (NULL==GPara::m_Sect_in)
	{
		return ;
	}
	int type = GPara::m_Sect_in->m_kind;
	int nsects=1;
	if(type>2 && type<6)
		nsects = 4;
	else if(type==6)
		nsects = 6;
	int con_h = GPara::m_Sect_in->Sect_h;
	int con_b = GPara::m_Sect_in->Sect_b;
	int h = GPara::m_Sect_in->HSect.H1;
	int b = GPara::m_Sect_in->HSect.B1;
	float tf = GPara::m_Sect_in->HSect.RI;
	float tw = GPara::m_Sect_in->HSect.RJ;
	int bard = GPara::m_Sect_in->Bar_d;
//	int cover = GPara::m_Sect_in->Concrete_coverd;
	int cover_y = GPara::m_Sect_in->Con_coverd_x;
	int cover_z = GPara::m_Sect_in->Con_coverd_y;
	int bar_x = GPara::m_Sect_in->Bar_num_bside;
	int bar_y = GPara::m_Sect_in->Bar_num_hside;
	bar_inv_x = (con_b-2*cover_y)*1.f/(bar_x-1);
	bar_inv_y = (con_h-2*cover_z)*1.f/(bar_y-1);
	if(type==2 || type==5)
	{
		Ag = con_b*con_b*PI*0.25;
		Ig_x=Ig_y = PI*con_b*con_b*con_b*con_b/64;
		As = nsects*(h*b-(h-2*tf)*(b-tw));
		Asr = bar_x*bard*bard*PI*0.25;
	}else
	{
		Ag = con_b*con_h;
		Ig_x = con_b/12.f*con_h*con_h*con_h;
		Ig_y = con_h/12.f*con_b*con_b*con_b;
		As = nsects*(h*b-(h-2*tf)*(b-tw));
		Asr = (2*(bar_x+bar_y)-4)*bard*bard*PI*0.25;
	}
	Ac = Ag-As-Asr;
	if(GPara::m_DesnPara->CodeType==1){
		Tools::GetConFc_EURO(GPara::m_Sect_in->Concrete_mat,fc);		//fck
		fy = Tools::GetProFy(GPara::m_DesnPara->ProType,GPara::m_Sect_in->HSect.RI,GPara::m_DesnPara->ProLevel);
	}
	else if(GPara::m_DesnPara->CodeType==2){
		Tools::GetConFc_EURO(GPara::m_Sect_in->Concrete_mat,fc);	//美国规范也是用的fck
		fy = Tools::GetProFy(GPara::m_DesnPara->ProType,GPara::m_Sect_in->HSect.RI,GPara::m_DesnPara->ProLevel);
	}
	else if(GPara::m_DesnPara->CodeType==3){
		Tools::GetConFc_Chi(GPara::m_Sect_in->Concrete_mat,fc);
		fy = Tools::GetProFy(GPara::m_DesnPara->ProType,GPara::m_Sect_in->HSect.RI,GPara::m_DesnPara->ProLevel);
	}
	fs = GPara::m_Sect_in->Bar_mat;
	Es = Tools::GetProE(GPara::m_DesnPara->CodeType);
	Ecm = Tools::GetConEcm(GPara::m_Sect_in->Concrete_mat);
	Esr = Tools::GetBarE(GPara::m_DesnPara->CodeType);
}

void SectProperty::EQASCal()
{
	int nLayers=1;		//默认钢筋层数为1
	int rebar_nx = GPara::m_Sect_in->Bar_num_bside;
	int rebar_ny = GPara::m_Sect_in->Bar_num_hside;
	int rebar_disx = GPara::m_Sect_in->Bar_interval;
	int	rebar_disy = GPara::m_Sect_in->Bar_interval;
//	int rebar_edge = GPara::m_Sect_in->Concrete_coverd;		//保护层厚度
	int rebar_edge_y = GPara::m_Sect_in->Con_coverd_x;
	int rebar_edge_z = GPara::m_Sect_in->Con_coverd_y;
	int secth = GPara::m_Sect_in->Sect_h;
	int sectb = GPara::m_Sect_in->Sect_b;
	//where b~x,h~y
	//临时的变量
	int rebar_d = GPara::m_Sect_in->Bar_d;
	float Asr = PI*rebar_d*rebar_d*0.25;
	//cal 1x
	As1 = nLayers*rebar_nx*Asr;
	hs1 = (rebar_nx-1)*rebar_disx;
	bs1 = As1/hs1;
	d1y = secth*0.5 - rebar_edge_y - rebar_d*0.5;
	Zsr1x = 2*As1*d1y;			//只考虑一层钢筋，所以ds1y和d1y是一样的
	Isr1x = 2*nLayers*rebar_nx*Asr*d1y*d1y;
	//cal 1y
	Zsr1y = 2*bs1*hs1*hs1/4;
	Isr1y = 2*bs1*hs1*hs1*hs1/12;
	//cal 2x
	As2 = nLayers*rebar_ny*Asr;
	hs2 = (rebar_ny-1)*rebar_disy;
	bs2 = As2/hs2;
	d2x = sectb*0.5 - rebar_edge_z - rebar_d*0.5;
	Zsr2x = 2*bs2*hs2*hs2/4;
	Isr2x = 2*bs2*hs2*hs2*hs2/12;
	//cal 2y
	Zsr2y = 2*As2*d2x;
	Isr2y = 2*nLayers*rebar_ny*Asr*d2x*d2x;

	//如果是圆形截面，等效为矩形
	if(GPara::m_Sect_in->m_kind == 2 || GPara::m_Sect_in->m_kind==5){
		float rr = sqrt(PI*secth*secth*0.25);
		hs1 = rr-rebar_edge_y*2;
		bs1 = rebar_nx*0.25*Asr/hs1;
		hs2 = hs1;
		bs2 = bs1;
		d1y = rr*0.5 - rebar_edge_y + rebar_d*0.5;
		d2y = d1y;
		Zsr1x = rebar_nx*0.5*Asr*d1y;
		Isr1x = 2*nLayers*rebar_nx*0.25*Asr*d1y*d1y;
		Zsr2y = Zsr1x;
		Isr2y = Isr1x;
		Zsr1y = 2*bs1*hs1*hs1/4;
		Isr1y = 2*bs1*hs1*hs1*hs1/12;
		Zsr2x = Zsr1y;
		Isr2x = Isr1y;
	}
}
	//cal 1y
	//cal 2y
 	//对圆形可以考虑等效为矩形


void SectProperty::EQAGCal()
{
	int SectType = GPara::m_Sect_in->m_kind;
	int h = GPara::m_Sect_in->HSect.H1;
	int b = GPara::m_Sect_in->HSect.B1;
	float tw = GPara::m_Sect_in->HSect.RJ;
	float tf = GPara::m_Sect_in->HSect.RI;
	float Ass = b*1.f*h - (h-2*tf)*(b-tw);
	Is_x = b*1.f*h*h*h/12-(b-tw)*(h-2*tf)*(h-2*tf)*(h-2*tf)/12;
	Is_y = tf*b*b*b/6+(h-2*tf)*tw*tw*tw/12;
	bb = Ass/h;
	hh = h;
	bb2 = b;
	hh2 = Ass/b;

	//zsx
	float dsy = GPara::m_Sect_in->HSect_y;		//型钢的型心定位
	Zsx = Ass*dsy;
	Isx = Ass*dsy*dsy+Is_x;
	//zsy
	float dsx = GPara::m_Sect_in->HSect_y;
	Zsy = Ass*dsx;
	Isy = Ass*dsx*dsx+Is_y;

	//根据不同截面来计算
	if(SectType==1 || SectType==2 || SectType==8){
		Zsx = 2*(b*h*h/8-(b-tw)*(h-2*tf)*(h-2*tf)/8);
		Isx = Is_x;
		Zsy = 2*(h*b*b/8 - (h-2*tf)*(b-tw)*(b+tw)/8);
		Isy = Is_y;
	}
	else if(SectType==3){
		Zsx = Zsx*4;
		Isx = Isx*4;
		Zsy = Zsy*4;
		Isy = Isy*4;
	}
	else if(SectType==4 ||SectType==5){
		Zsx = 2*Ass*dsy+ 4*(h*b*b/8 - (h-2*tf)*(b-tw)*(b-tw)/8);
		Isx = Isx*2+Is_y*2;
		Zsy = 2*Ass*dsx+ 4*(h*b*b/8 - (h-2*tf)*(b-tw)*(b-tw)/8);
		Isy = Isx*2+Is_y*2;
	}
	else if(SectType==6){
		Zsx = Zsx*4+4*(b*h*h/8-(b-tw)*(h-2*tf)*(h-2*tf)/8);
		Isx = Isx*4+Is_x*2;
		Zsy = Zsy*6;
		Isy = Isy*6;
	}
	else if(SectType==7){
		Zsx = Zsx*4+4*(h*b*b/8 - (h-2*tf)*(b-tw)*(b-tw)/8);
		Isx = Is_y*6+Is_y*dsy*dsy*4;
		Zsy = Zsy*6;
		Isy = Is_x*6+Is_x*dsx*dsx*6;
	}
}


//                          b
//                 - ----------     
//					|              |
//					|              |
//			h		|        --   |          -----x
//					|              |
//                 - ----------     
//                        y
//   Zc_x  =  Wx
//   Zc_y  =  Wy
//  Zcx,Zcy   扣除型钢和钢筋
//   Icx、Icy  =  Ix，Iy  扣除型钢和钢筋
void SectProperty::EQACCal()
{
	double h = GPara::m_Sect_in->Sect_h;
	double b = GPara::m_Sect_in->Sect_b;
	double Zc_x = b*h*h*1.f*0.25;
	double Zc_y = h*b*b*1.f*0.25;
	Zcx = Zc_x - Zsr1x- Zsr2x-Zsx;
	Zcy = Zc_y - Zsr1y- Zsr2y-Zsy;
	Icx = Ig_x-Isx-Isr1x-Isr2x;
	Icy = Ig_y-Isy-Isr1y-Isr1y;
	if(GPara::m_Sect_in->m_kind==2 || GPara::m_Sect_in->m_kind==5){
		float rr = sqrt(PI*h*h*0.25);
		Zcx = rr*rr*rr*0.25 - Zsr1x - Zsr2x -Zsx;
		Zcy = rr*rr*rr*0.25 - Zsr1y - Zsr2y -Zsy;
		Icx = rr*rr*rr*rr/12-Isx-Isr1x-Isr2x;
		Icy = rr*rr*rr*rr/12-Isy-Isr1y-Isr2y;
	}
}

