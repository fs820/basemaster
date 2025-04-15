//---------------------------------------
//
//ポリゴン表示処理の定義・宣言[earth.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EARTH_H_
#define _EARTH_H_

#include"main.h"

#define EARTH_TEX "data\\TEXTURE\\Base.jpg"//テクスチャ

//地球構造体
typedef struct
{
	D3DXMATRIX mtxWorld;//マトリックス
	bool bUse;//使用
}Earth;

void InitEarth(void);//初期化処理
void UninitEarth(void);//終了処理
void UpdateEarth(void);//更新処理
void DrawEarth(void);//描画処理
void SetEarth(void);//設定処理

#endif _EARTH_H_