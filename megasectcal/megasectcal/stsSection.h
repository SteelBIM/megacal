#if !defined(STSSECTION_H)
#define STSSECTION_H

//   结构截面定义
struct SECTION {
    short  KIND;
	char   NAME[16];
	char   NAME1[16];
	short  B1, B2;
	short  H1, H2, B3, H3;
	short  T1, T2, T3, T4, T5, T6;
	short  M ;
	float  RI;
	float  RJ;
	float  UA;
	float  Xc, Yc;
	float  Rx, Ry;
	float  Fy, Fv;
	float  W1x, W2x, W1y, W2y;
	char   TypeX, TypeY;
	float  Rc;
	float  A, Ix, Iy, Ix1, Iy1, Sw, Sf;
};

#endif // !defined(STSSECTION_H)
