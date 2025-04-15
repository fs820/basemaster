//---------------------------------------
//
//sphere処理の定義・宣言[space.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _SPACE_H_
#define _SPACE_H_

#include"main.h"

#define SPACE_TEX_MAX (2)//テクスチャ
#define SPACE_RADIUS (10000.0f)//半径
#define SPACE_HNUM (256)//H分割数
#define SPACE_VNUM (64)//V分割数
#define SPACE_ANG (90.0f)//半球

//半球
#define VT_MAX_SP ((SPACE_HNUM + 1) * SPACE_VNUM+1)//横の頂点数*縦のライン数(極を抜かす)+極 (半球)
#define INDEX_SP_NUM (((SPACE_HNUM+1)*2)*SPACE_VNUM+(SPACE_VNUM-2)*2-SPACE_HNUM)//横の頂点数*2列**縦のライン数(極を抜かす)+縮退ポリゴン-極の不要分
#define POLYGON_SP_NUM (SPACE_HNUM*SPACE_VNUM*2+(SPACE_VNUM-2)*4-SPACE_HNUM)//全体の分割*2*縮退ポリゴン-極の不要分

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

//Texture構造体
typedef struct
{
	String TexPass;
	D3DXVECTOR2 TexSize;
}TEXTURE;

//Space構造体
typedef struct
{
	D3DXMATRIX mtxWorld;//マトリックス
	TEXTURE Texture[SPACE_TEX_MAX];
	bool bUse;//使用
}Space;

void InitSpace(void);   //初期化処理
void UninitSpace(void); //終了処理
void UpdateSpace(void); //更新処理
void DrawSpace(void);   //描画処理
void SetSpace(void);    //設定処理

#endif _SPACE_H_