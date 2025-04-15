//---------------------------------------
//
//エフェクト表示処理の定義・宣言[effect.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

#define EFFECT_TEX "data\\TEXTURE\\shadow000.jpg"
#define MAX_EFFECT (4096)//最大数
#define EFFECT_SIZE_DEF (20.0f)

typedef enum
{
	EFFECT_TYPE_NORMAL = 0,
	EFFECT_TYPE_MAX
}EFFECT_TYPE;

void InitEffect(void);//エフェクトの初期化処理
void UninitEffect(void);//エフェクトの終了処理
void UpdateEffect(void);//エフェクトの更新処理
void DrawEffect(void);//エフェクトの描画処理
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 scale, int nLife, EFFECT_TYPE Type);//エフェクト設定

#endif _EFFECT_H_
