//---------------------------------------
//
//sphereΜθ`EιΎ[space.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _SPACE_H_
#define _SPACE_H_

#include"main.h"

#define SPACE_TEX_MAX (2)//eNX`
#define SPACE_RADIUS (10000.0f)//Όa
#define SPACE_HNUM (256)//Hͺ
#define SPACE_VNUM (64)//Vͺ
#define SPACE_ANG (90.0f)//Ό

//Ό
#define VT_MAX_SP ((SPACE_HNUM + 1) * SPACE_VNUM+1)//‘ΜΈ_*cΜC(Ιπ²©·)+Ι (Ό)
#define INDEX_SP_NUM (((SPACE_HNUM+1)*2)*SPACE_VNUM+(SPACE_VNUM-2)*2-SPACE_HNUM)//‘ΜΈ_*2ρ**cΜC(Ιπ²©·)+kή|S-ΙΜsvͺ
#define POLYGON_SP_NUM (SPACE_HNUM*SPACE_VNUM*2+(SPACE_VNUM-2)*4-SPACE_HNUM)//SΜΜͺ*2*kή|S-ΙΜsvͺ

const String TexPass[SPACE_TEX_MAX] =
{
	"data\\TEXTURE\\Base.jpg",
	"data\\TEXTURE\\Base.jpg"
};

const float TexScale[SPACE_TEX_MAX] =
{
	10.0f,
	10.0f
};

//Texture\’Μ
typedef struct
{
	String TexPass;
	D3DXVECTOR2 TexSize;
}TEXTURE;

//Space\’Μ
typedef struct
{
	D3DXMATRIX mtxWorld;//}gbNX
	TEXTURE Texture[SPACE_TEX_MAX];
	bool bUse;//gp
}Space;

void InitSpace(void);   //ϊ»
void UninitSpace(void); //IΉ
void UpdateSpace(void); //XV
void DrawSpace(void);   //`ζ
void SetSpace(void);    //έθ

#endif _SPACE_H_