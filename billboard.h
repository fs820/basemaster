//---------------------------------------
//
//ポリゴン表示処理の定義・宣言[billboard.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include"main.h"

#define BILLBOARD_TEX "data\\TEXTURE\\wood.png"
#define BILLBOARD_MAX (64)

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	D3DXMATRIX mtxWorld;
	bool bUse;
	int nIdxShadow;
}Billboard;

void InitBillboard(void);//ポリゴンの初期化処理
void UninitBillboard(void);//ポリゴンの終了処理
void UpdateBillboard(void);//ポリゴンの更新処理
void DrawBillboard(void);//ポリゴンの描画処理
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 scale);

#endif _BILLBOARD_H_