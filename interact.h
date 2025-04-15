//---------------------------------------
//
//インタラクト処理の定義・宣言[interact.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _INTERACT_H_
#define _INTERACT_H_

#include"main.h"

#define INTERACT_TEX_KEY "data\\TEXTURE\\Base.jpg"          //キーボードテクスチャ
#define INTERACT_TEX_JOY "data\\TEXTURE\\Base.jpg"          //コントローラーテクスチャ

//インタラクトタイプの列挙型定義
typedef enum
{
	INTERACT_TYPE_KEY = 0,
	INTERACT_TYPE_JOY,
	INTERACT_TYPE_MAX
}INTERACT_TYPE;

constexpr String INTERACT_TEX[INTERACT_TYPE_MAX] =
{
	"data\\TEXTURE\\Base.jpg",
	"data\\TEXTURE\\Base.jpg"
};

//インタラクト構造体
typedef struct
{
	D3DXVECTOR3 pos;      //位置
	D3DXVECTOR3 scale;    //大きさ
	INTERACT_TYPE Type;   //タイプ
	D3DXMATRIX mtxWorld;  //マトリックス
	bool bUse;            //使用
}Interact;

void InitInteract(void);                              //初期化処理
void UninitInteract(void);                            //終了処理
void UpdateInteract(void);                            //更新処理
void DrawInteract(void);                              //描画処理
void SetInteract(D3DXVECTOR3 pos, D3DXVECTOR3 scale); //設定
void EndInteract(void);                               //破棄
void SetInteractType(INTERACT_TYPE Type);             //タイプ変更

#endif _INTERACT_H_