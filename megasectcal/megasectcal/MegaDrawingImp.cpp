#include "StdAfx.h"
#include "MegaDrawingImp.h"
#include "cfgbas.h"
#include "MegaCalInterface.h"
#include <math.h>

extern "C" __declspec( dllimport ) CCfgBas *m_pCfgBas;

CMegaDrawingImp::CMegaDrawingImp(void)
{
	m_enLarege = 10.0f;

	for (int i=0;i<Combine;i++)
	{
		m_MockF[i] = i*2;
	}

	m_MockF[8]=99;

	m_MDrawSc=1;
	m_VDrawSc=1;
	m_NDrawSc=1;
}


CMegaDrawingImp::~CMegaDrawingImp(void)
{
}

void CMegaDrawingImp::DrawZhizuoXY(float x0,float y0,float x1,float y1,double bl)
{
//	float PI=3.14159265359;
	float rc=1.*bl;
	float dd_2=2*bl+1*bl;
	float ang=atan2(y1-y0,x1-x0);
	float ang1=ang+PI/2.;
	float xt[100],yt[100];
	xt[0]=x0+rc*cos(ang);
	yt[0]=y0+rc*sin(ang);
	xt[1]=x1-rc*cos(ang);
	yt[1]=y1-rc*sin(ang);
	xt[2]=x1+rc*cos(ang);
	yt[2]=y1+rc*sin(ang);
	xt[3]=xt[2]+dd_2*cos(ang1);
	yt[3]=yt[2]+dd_2*sin(ang1);
	xt[4]=xt[2]-dd_2*cos(ang1);
	yt[4]=yt[2]-dd_2*sin(ang1);

	float xx=2.*bl;
	xt[11]=xt[2]+xx/2*cos(PI/4)*cos(ang1);
	yt[11]=yt[2]+xx/2*cos(PI/4)*sin(ang1);

	xt[10]=xt[11]+xx*cos(PI/4)*cos(ang1);
	yt[10]=yt[11]+xx*cos(PI/4)*sin(ang1);

	xt[12]=xt[11]-xx*cos(PI/4)*cos(ang1);
	yt[12]=yt[11]-xx*cos(PI/4)*sin(ang1);
	for(int i=0;i<3;i++)
	{
		xt[20+i]=xt[10+i]+xx*cos(ang1-3.*PI/4.);
		yt[20+i]=yt[10+i]+xx*sin(ang1-3.*PI/4.);
	}

	int cenghao=1;
	int xianxing=1;
	int xiankuan=1;
	int yanse=3;//10=浅绿色 7=灰白色
	int fanhui=0;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	m_pCfgBas->Circle(x0,y0,rc);
	m_pCfgBas->Circle(x1,y1,rc);
	xiankuan=3;
	yanse=7;//10=浅绿色 7=灰白色
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	m_pCfgBas->Line(xt[0],yt[0],xt[1],yt[1]);
	m_pCfgBas->Line(xt[3],yt[3],xt[4],yt[4]);
	xiankuan=1;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);
	for(int i=0;i<3;i++)
	{
		m_pCfgBas->Line(xt[10+i],yt[10+i],xt[20+i],yt[20+i]);
	}

}

void CMegaDrawingImp::DrawZhizuo(float x0,float y0,bool bUp,double enLarge)
{
	double ang = 0 ;
	if (bUp)
	{
		ang = 180;
	}
	else
	{
		ang = 0;
	}
	MyUserCs2DEnlarge(x0,y0,ang,enLarge);
	{

		float x0=0.f,y0=0.f;
		float x2,y2,x3,y3,x4,x5;

		x2=x0-20;
		x3=x0+20;
		y2=y0;
		m_pCfgBas->Line(x0,y2,x2,y2); //向左向右水平线
		m_pCfgBas->Line(x0,y2,x3,y2);
		x4=x0-4;
		x5=x0-8;
		y3=y2-7;
		m_pCfgBas->Line(x4,y2,x5,y3);
		x4=x0-12;
		x5=x0-16;
		y3=y2-7;
		m_pCfgBas->Line(x4,y2,x5,y3);
		x4=x0+4;
		x5=x0;
		y3=y2-7;
		m_pCfgBas->Line(x4,y2,x5,y3);
		x4=x0+12;
		x5=x0+8;
		y3=y2-7;
		m_pCfgBas->Line(x4,y2,x5,y3);
	}
	
	m_pCfgBas->Outucs();


}
void CMegaDrawingImp::CalcMVNDrawSc()
{
	Force_combine &force = MegaCalCmdData::Get().Force();
	force.GetForce();
	
	double Mmax=0,Vmax=0,Nmax=0;
	if (NULL == GPara::m_DesnPara)
	{
		m_MDrawSc = 150;
		m_VDrawSc = 150;
		m_NDrawSc = 150;
		return ;
	}
	for (int i=0;i<GPara::m_DesnPara->nLoads_y;i++)
	{
		for (int iS=0;iS<Combine;iS++)
		{
			if (fabs(force.My[i][iS])>fabs(Mmax))
			{
				Mmax = force.My[i][iS];
			}
			if (fabs(force.Vy[i][iS])>fabs(Vmax))
			{
				Vmax = force.Vy[i][iS];
			}
			if (fabs(force.N[i][iS])>fabs(Nmax))
			{
				Nmax = force.N[i][iS];
			}
		}
	}
	for (int i=0;i<GPara::m_DesnPara->nLoads_z;i++)
	{
		for (int iS=0;iS<Combine;iS++)
		{
			if (fabs(force.Mz[i][iS])>fabs(Mmax))
			{
				Mmax = force.Mz[i][iS];
			}
			if (fabs(force.Vz[i][iS])>fabs(Vmax))
			{
				Vmax = force.Vz[i][iS];
			}
		}
	}
 
	const int maxValconst = 25000;
	if (fabs(Mmax)<1e-5)
	{
		m_MDrawSc = 0.3;
	}
	else
	{
		m_MDrawSc = fabs(maxValconst / Mmax);
	}

	if (fabs(Vmax)<1e-5)
	{
		m_VDrawSc = 0.3;
	}
	else
	{
		m_VDrawSc = fabs(maxValconst / Vmax);
	}
	
	if (fabs(Nmax)<1e-5)
	{
		m_NDrawSc = 0.3;
	}
	else
	{
		m_NDrawSc = fabs(maxValconst / Nmax);
	}
	
 
}

void CMegaDrawingImp::DrawCol(int nLoadCase,int nType)
{
	CalcMVNDrawSc();

	//文件名准备
	CString StrFile=MegaCalCmdData::Get().MakeDrawFiName(nLoadCase,nType);//("d:\\DRAWCOL.t");
	int n1=StrFile.GetLength();
	char FileName[256];
	strcpy_s(FileName,256,StrFile);
	FileName[n1]=char(0);

	//清理图纸
	int iWayDel = 99;
	m_pCfgBas->Delfil(FileName,n1);


	//m_hcol=1000*GPara::m_DesnPara->Lens;//朱工传
	m_hcol = 1800;

	float x0,y0,x1,y1;
	float x2,x3,x4,x5,x6,x7,x8,x9,x10;
	float y2,y3,y4,y5,y6,y7,y8,y9,y10;
	int iloadtype[100],nload;
	float hcol,ycol,yloadcol[100];

	float enLarege = m_enLarege;
	m_drawStickLen=220;//以下尺寸都是以竖向杆件220为美观进行绘制的


	//画图
	int iWay = 99;
	m_pCfgBas->Closfl(iWay);
//	m_pCfgBas->Redrw();
    m_pCfgBas->Setcfg();
	m_pCfgBas->Openfl(iWay,FileName,n1,n1);
	m_pCfgBas->Nowfil(iWay);
	m_pCfgBas->Tobegin0(iWay);
	int nSv = 1;
	m_pCfgBas->Ifsave(nSv);
	float WWX = (50+20)*enLarege;
    float WWY = (220+10)*enLarege;
	float WWZ=WWX;
	int  IUnit = 1;
	float dGrBli = 1.0;
	float xoo=25.5;
	float yoo=10;
	float zoo=10;
	m_pCfgBas->Grfild(WWX,WWY,WWZ,IUnit,dGrBli,xoo,yoo,zoo);
	//m_pCfgBas->ORIGN(25.5,CYB+1)


	int cenghao=1;
	int xianxing=1;
	int xiankuan=4;
	int yanse=7;//10=浅绿色 7=灰白色
	int fanhui=0;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);


	//1、画柱子
	x0=0.f;
	y0=0.f;
	x1=0.f;
	y1=m_drawStickLen*enLarege;


	//1、画柱子
	COLM(x0,y0,m_drawStickLen);
	//3、画上下支座
	xiankuan=3;
	yanse=7; //灰色
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);

   //  GlobalPara::m_DesnPara.Restrict = 3;		//铰接信息0,1,2,3,4（两端固接，左下铰，右上铰，两端铰,悬臂）
	int sup1=7;   // 7： 固定/1,2,(3),4,(5)(6)：铰  /0：悬臂
	int sup2=7;   // 7： 固定/1-3：铰  /0：悬臂

	if(GPara::m_DesnPara->Restrict ==1) sup1=1;
	if(GPara::m_DesnPara->Restrict ==2) sup2=1;
	if(GPara::m_DesnPara->Restrict ==2) sup2=1;
	switch (GPara::m_DesnPara->Restrict )
	{
	    case 0:                
			sup1=7;		
			sup2=7;		
		case 1:                
			sup1=1;	
			sup2=7;
			break;
		case 2:
			sup2=1;
			sup2=7;
			break;
		case 3:
			sup1=3;		
			sup2=1;		
			break;
		case 4:
			sup1=7;		
			sup2=0;		
			break;
	}
	if(sup1==7)  DrawZhizuo(x0,y0,false,enLarege);
	else 
	{
		if(sup1%2==1) DrawZhizuoXY(x0,y0,x0+7*enLarege,y0,enLarege);
	    if(sup1==2||sup1==3) DrawZhizuoXY(x0,y0,x0,y0-7*enLarege,enLarege);
	}

	if(sup2==7)  DrawZhizuo(x1,y1,true,enLarege);
	else 
	{
		if(sup2%2==1) DrawZhizuoXY(x1,y1,x1+7*enLarege,y1,enLarege);
		if(sup2==2||sup2==3) DrawZhizuoXY(x1,y1,x1,y1+7*enLarege,enLarege);
	}


	
	//下支座
/*
	//down
	DrawZhizuo(x1,y1,true,enLarege);
*/
	COLM13(x0,y0,nLoadCase,nType);

	 
//	m_pCfgBas->Closfl(iWay);


//	m_pCfgBas->Openfl(iWay,FileName,n1,n1);
//	m_pCfgBas->Zoome();
}

CMegaDrawingImp CMegaDrawingImp::s_MegaDrawingImp;
CMegaDrawingImp& CMegaDrawingImp::Get()
{
	return s_MegaDrawingImp;
}

 
void CMegaDrawingImp::MyUserCs2d(float X0,float Y0,float ang,float BL)
{
	float PRCS[9];
	PRCS[0] = X0;
	PRCS[1] = Y0;
	PRCS[2] = 0;

	if (fabs(BL)<1E-9)
	{
		BL=1;
	}
	PRCS[3] = 1./BL;
	PRCS[4] = 1./BL;
	PRCS[5] = 1./BL;


	PRCS[6] = 0;
	PRCS[7] = 0;
	PRCS[8] = ang;

	m_pCfgBas->Usercs(PRCS);
 
}
	
void CMegaDrawingImp::MyUserCs2DEnlarge(float X0,float Y0,float ang,float Sc)	 
{
	float PRCS[9];
	PRCS[0] = X0;
	PRCS[1] = Y0;
	PRCS[2] = 0;

	PRCS[3] = Sc;
	PRCS[4] = Sc;
	PRCS[5] = Sc;


	PRCS[6] = 0;
	PRCS[7] = 0;
	PRCS[8] = ang;

	m_pCfgBas->Usercs(PRCS);

}
void CMegaDrawingImp::MyUserCs2DExit()
{
	m_pCfgBas->Outucs();
}
 

//世界坐标系 x1,y1
//WCS->UCS 的角度 angle (°)
void CMegaDrawingImp::Rot_Coord_GtoL(float angle, float x1,float y1,float& x2,float& y2)  
{
	float x_in =x1;
	float y_in =y1;
	float out_x=x2;
	float out_y=y2;

	float ang = angle * 3.14159265359/180. ;

	out_x =  x_in*cos(ang) + y_in*sin(ang);
	out_y = -x_in*sin(ang) + y_in*cos(ang);

	x1=x_in;
	y1=y_in;
	x2=out_x;
	y2=out_y;
	return;
}

//局部坐标x1 y1
//世界坐标x2 y2
 void CMegaDrawingImp::Rot_Coord_LtoG(float angle, float x1,float y1,float& x2,float& y2)
 {
	float x_in =x1;
	float y_in =y1;
	float out_x=x2;
	float out_y=y2;

	float ang = angle * 3.14159265359/180. ;
	out_x = x_in * cos(ang) - y_in * sin(ang);
	out_y = x_in * sin(ang) + y_in * cos(ang);

	x1=x_in;
	y1=y_in;
	x2=out_x;
	y2=out_y;
 }
void CMegaDrawingImp::ZFANG(float ANG1,float ANG2)
{
	int Iang = int(ANG1);
	if (Iang<0)
	{
		Iang=Iang+360;
	}
	if (Iang>0 && Iang<=05)
	{
		ANG2 = ANG1;
	}
	else if (Iang<285)
	{
		ANG2 = ANG1-180.;
	}
	else
	{
		ANG2 = ANG1-360;
	}

	return;
}
void CMegaDrawingImp::COLM(float x1,float y1,float len)
{
	MyUserCs2DEnlarge(x1,y1,0,m_enLarege);
	{
		//1、画柱子
		float x2=x1;
		float y2=y1+len;
		m_pCfgBas->Line(x1,y1,x2,y2);

	}
	MyUserCs2DExit();
}
void CMegaDrawingImp::COLM13(float x1,float y1,int nLoadCase,int nType)
{
	float DX1=x1;
	float DY1=y1;
	float UX1=x1;
	float UY1=m_drawStickLen;
	float x2=UX1;
	float y2=UY1;

	float angL;
	m_pCfgBas->Degre(angL,DX1,DY1,UX1,UY1);

	//转动局部坐标系下看
	//Rot_Coord_GtoL(ang,DX1,DY1,DX1,DY1);
	//Rot_Coord_GtoL(ang,UX1,UY1,UX1,UY1);
	int cenghao=3;
	int xianxing=1;
	int xiankuan=1;
	int yanse=6;//10=浅绿色 7=灰白色
	int fanhui=0;
	m_pCfgBas->Layers(cenghao,xianxing,xiankuan,yanse,fanhui);

	float xMax=0,yMax=0,xMin=0,yMin=0;
	float valMax=-20000,valMin=20000;
	MyUserCs2DEnlarge(x1,y1,angL,m_enLarege);
	{
		Force_combine &force = MegaCalCmdData::Get().Force();
		force.GetForce();
		double* pF;
		switch(nType)
		{
		case eMy:
			pF = force.My[nLoadCase];
			break;
		case eMz:
			pF = force.Mz[nLoadCase];
			break;
		case eVy:
			pF = force.Vy[nLoadCase];
			break;
		case eVz:
			pF = force.Vz[nLoadCase];
			break;
		case eN:
			pF = force.N[nLoadCase];
			break;
		}

		//pF = MockForce(nLoadCase,nType);

		float xL1,yL1,xR2,yR2;
		Rot_Coord_GtoL(angL,x1,y1,xL1,yL1);
		Rot_Coord_GtoL(angL,x2,y2,xR2,yR2);
		float len = xR2-xL1;

		float xx[Combine],yy[Combine];
		float stepLen = (len / (Combine-1));
		for (int i=0;i<Combine;i++)
		{
			xx[i] = stepLen*i;
			yy[i] = pF[i]/1000;
			if (valMax<pF[i])
			{
				valMax = pF[i];
				xMax = xx[i];
				yMax = yy[i];
			}
			if (valMin>pF[i])
			{
				valMin= pF[i];
				xMin = xx[i];
				yMin = yy[i];
			}
		}
		double scMax;
		switch(nType)
		{
		case eMy:
			scMax = m_MDrawSc;
			break;
		case eMz:
			scMax = m_MDrawSc;
			break;
		case eVy:
			scMax = m_VDrawSc;
			break;
		case eVz:
			scMax = m_VDrawSc;
			break;
		case eN:
			scMax = m_NDrawSc;
			break;
		}


		for (int i=0;i<Combine;i++)
		{
			yy[i] = yy[i] * scMax;
//			yMax = yMax * scMax	;
//			yMin = yMin * scMax;
		}
		yMax = yMax * scMax	;
		yMin = yMin * scMax;
		

		int nPt = Combine;
		m_pCfgBas->Poly(nPt,xx,yy);

		m_pCfgBas->Line(xL1,yL1,xx[0],yy[0]);
		m_pCfgBas->Line(xR2,yR2,xx[Combine-1],yy[Combine-1]);


		//文字
		CString strMax,strMin;
		switch(nType)
		{
		case eMy:
			strMax.Format("%0.1f (kN-m)",valMax*1.e-6);
			strMin.Format("%0.1f (kN-m)",valMin*1.e-6);

			break;
		case eMz:
			strMax.Format("%0.1f (kN-m)",valMax*1.e-6);
			strMin.Format("%0.1f (kN-m)",valMin*1.e-6);

			break;
		case eVy:
			strMax.Format("%0.1f (kN)",valMax*1.e-3);
			strMin.Format("%0.1f (kN)",valMin*1.e-3);

			break;
		case eVz:
			strMax.Format("%0.1f (kN)",valMax*1.e-3);
			strMin.Format("%0.1f (kN)",valMin*1.e-3);
			break;
		case eN:
			strMax.Format("%0.1f (kN)",valMax*1.e-3);
			strMin.Format("%0.1f (kN)",valMin*1.e-3);
			break;
		}

		int nLen = strMax.GetLength();
		float wid=9,hgt=9,angCh=-90;int IXI=1,IYI=1;
		m_pCfgBas->Drawhz(nLen,strMax.GetBuffer(),nLen,xMax,yMax,wid,hgt,IXI,IYI,angCh);
		strMax.ReleaseBuffer();
		nLen = strMin.GetLength();
		m_pCfgBas->Drawhz(nLen,strMin.GetBuffer(),nLen,xMin,yMin,wid,hgt,IXI,IYI,angCh);
		strMin.ReleaseBuffer();

	}
	MyUserCs2DExit();

	
	
}

double* CMegaDrawingImp::MockForce( int nLoadCase,int nType )
{
	return m_MockF;
}



// 	DO I=1,NL-nAppCol
	// 	IK=KCR(I)
	// 	KK=CROS(IK).KIND !!,1)
	// 	DMAX=ColVal(I,MNV_C)
	// 	UMAX=ColVal(I,MNV_C+3)
	// 
	// 	CALL GYXY(I,XL1,YL1,XR2,YR2)
	// 	DX1=XL1
	// 	DY1=YL1
	// 	UX1=XR2
	// 	UY1=YR2
	// 	CALL DEGRE(angL,DX1,DY1,UX1,UY1)
	// 	ANG=ANGL-90
	// 	CALL ZFANG(ANG,ANG1)
	// 	CALL Rot_Coord_GtoL(ang,DX1,DY1,DX1,DY1)
	// 	CALL Rot_Coord_GtoL(ang,UX1,UY1,UX1,UY1)
	// 	CALL WriteRealToChar(DMAX,CD1,ND1)
	// 	CALL WriteRealToChar(UMAX,CU1,NU1)
	// 
	// 	PDX2=DX1-0.1*SIGN(ND1,NINT(DMAX))
	// 	PDY2=DY1+0.2
	// 	PUX2=UX1+0.1*SIGN(NU1,NINT(UMAX))
	// 	PUY2=UY1-0.2
	// 
	// 	DX2=DX1-SCR*DMAX
	// 	DY2=DY1
	// 	UX2=UX1+SCR*UMAX
	// 	UY2=UY1
	// 
	// 	CALL Rot_Coord_LtoG(ang,DX1,DY1,DX1,DY1)
	// 	CALL Rot_Coord_LtoG(ang,UX1,UY1,UX1,UY1)
	// 
	// 	CALL Rot_Coord_LtoG(ang,DX2,DY2,DX2,DY2)
	// 	CALL Rot_Coord_LtoG(ang,UX2,UY2,UX2,UY2)
	// 
	// 	CALL Rot_Coord_LtoG(ang,PDX2,PDY2,PDX2,PDY2)
	// 	CALL Rot_Coord_LtoG(ang,PUX2,PUY2,PUX2,PUY2)
	// 
	// 	CALL LAYERS(2,1,1,14,1000000)
	// 	CALL LINE(DX1,DY1,DX2,DY2)
	// 	CALL LINE(UX1,UY1,UX2,UY2)
	// 	CALL LINE(UX2,UY2,DX2,DY2)
	// 
	// 	CALL DRAWZF(ND1,CD1,PDX2,PDY2,.2,.3,2,2,ANG1)
	// 	IF(MNV_C.EQ.2) CYCLE
	// 	CALL DRAWZF(NU1,CU1,PUX2,PUY2,.2,.3,2,2,ANG1)
	// 
	// 	ENDDO
	// 
	// 	RETURN
	// 	END