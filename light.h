//---------------------------------------
//
//ライト処理の定義・宣言[light.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

constexpr unsigned int LIGHT_MAX = 8; // ライトの最大数

// ライト構造体
typedef struct
{
	D3DLIGHT9 light; // ライト情報
	bool bUse;       // 使用フラグ
}LIGHT;

//プロトタイプ宣言
void InitLight(void);                                                 //初期化処理
void UninitLight(void);                                               //終了処理
void UpdateLight(void);                                               //更新処理
LIGHT* GetLight(void);                                                //ライト取得
unsigned int SetLight(D3DLIGHT9* pLight);                             //ライト設定
void EndLight(unsigned int nIdx);                                     //ライト破棄
#endif // !_LIGHT_H_