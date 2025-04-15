//---------------------------------------
//
//ポリゴン表示処理の定義・宣言[hitshere.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _HITSHERE_H_
#define _HITSHERE_H_

#include"main.h"

#define HITSHERE_MAX (64)//球の数
#define HITSHERE_RADIUS (1.0f)//半径
#define HITSHERE_HNUM (16)//H分割数
#define HITSHERE_VNUM (16)//V分割数
#define HITSHERE_ANG (180.0f)//半球全球切替

//全球
#define VT_MAX_HS ((HITSHERE_HNUM + 1) * (HITSHERE_VNUM-1)+2)//横の頂点数*縦のライン数(2極を抜かす)+2極 (全球)
#define INDEX_HS_NUM (((HITSHERE_HNUM+1)*2)*(HITSHERE_VNUM-2)+(HITSHERE_VNUM-3)*2+(HITSHERE_HNUM+2)*2)//横の頂点数*2列**縦のライン数(2極を抜かす)+縮退ポリゴン+2極
#define POLYGON_HS_NUM (HITSHERE_HNUM*(HITSHERE_VNUM-2)*2+(HITSHERE_VNUM-3)*4+HITSHERE_HNUM*2)//横の分割*2*縮退ポリゴン+2極

#define DAMEGE (10)

typedef enum
{
	PLAYER = 0,
	ENEMY,
	MAX
}MASTER;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	D3DXMATRIX mtxWorld;
	int nLife;
	MASTER Master;
	bool bUse;
}HitShere;

void InitHitShere(void);//初期化処理
void UninitHitShere(void);//終了処理
void UpdateHitShere(void);//更新処理
void DrawHitShere(void);//描画処理
void SetHitShere(D3DXVECTOR3 pos, D3DXVECTOR3 scale, int Life, MASTER Master);//設定処理
void TriggerShere(int Indx);

#endif _HITSHERE_H_