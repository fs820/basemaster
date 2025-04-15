//---------------------------------------
//
//ポリゴン表示処理の定義・宣言[bullet.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BULLET_H_
#define _BULLET_H_

#include"main.h"

#define BULLET_TEX "data\\TEXTURE\\bullet000.png"
#define BULLET_MAX (256)
#define BULLET_SPEED (10.0f)
#define BULEET_LIFE (600)

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 dir;
	D3DXVECTOR3 scale;
	D3DXMATRIX mtxWorld;
	int nLife;
	bool bUse;
	int nIdxShadow;
}Bullet;

void InitBullet(void);//ポリゴンの初期化処理
void UninitBullet(void);//ポリゴンの終了処理
void UpdateBullet(void);//ポリゴンの更新処理
void DrawBullet(void);//ポリゴンの描画処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 dir, D3DXVECTOR3 scale);

#endif _BULLET_H_