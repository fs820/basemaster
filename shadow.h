//---------------------------------------
//
//ポリゴン表示処理の定義・宣言[shadow.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _SHADOW_H_
#define _SHADOW_H_

#include"main.h"
#include"game.h"

#define SHADOW_TEX "data\\TEXTURE\\shadow000.jpg"
#define SHADOW_MAX (512)

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scale;
	D3DXMATRIX mtxWorld;
	bool bUse;
}Shadow;

void InitShadow(void);//ポリゴンの初期化処理
void UninitShadow(void);//ポリゴンの終了処理
void UpdateShadow(void);//ポリゴンの更新処理
void DrawShadow(void);//ポリゴンの描画処理
int SetShadow(D3DXVECTOR3 pos);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fWidth);
void NullShadow(int nIdxShadow);

#endif _POLYGON_H_