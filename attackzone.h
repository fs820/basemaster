//---------------------------------------
//
//アタックゾーン処理の定義・宣言[AttackZone.h]
//Author fuma sato
//
//---------------------------------------
#ifndef _ATTACK_ZONE_H_
#define _ATTACK_ZONE_H_
#include "main.h"

#define ATTACK_ZONE_MAX (128)

constexpr String ATTACK_ZONE_TEX =
{
	"data\\TEXTURE\\shadow000.jpg"
};

//テリトリー構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	float fRadius;			// 半径
	bool bLess;             // 減算
	bool bUse;				// 使用
	D3DXCOLOR col;          // 透明
	D3DXMATRIX mtxWorld;    // マトリックス
	unsigned int idexLight;	// ライトのインデックス
}AttackZone;

void InitAttackZone(void);   //初期化処理
void UninitAttackZone(void); //終了処理
void DrawAttackZone(void);   //描画処理
int SetAttackZone(D3DXVECTOR3 pos, float fRadius, D3DXCOLOR col, bool Less); //設定処理
void EndAttackZone(int nIdx);
#endif //_TERRITORY_H_