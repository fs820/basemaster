//---------------------------------------
//
//テリトリー処理の定義・宣言[territory.h]
//Author fuma sato
//
//---------------------------------------
#ifndef _TERRITORY_H_
#define _TERRITORY_H_
#include "main.h"

#define TERRITORY_FILE  "data\\txt\\sato\\territory.txt"
#define TERRITORY_TEX   "data\\TEXTURE\\shadow000.jpg"
#define TERRITORY_MAX (256)

//列挙型
typedef enum
{
	TERRITORY_TYPE_A = 0,
	TERRITORY_TYPE_B,
	TERRITORY_TYPE_BOSS,
	TERRITORY_TYPE_MAX
}TERRITORY_TYPE;

//テリトリー構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	float fRadius;			// 半径
	TERRITORY_TYPE Type;	// タイプ
	bool bUse;				// 使用
#ifdef _DEBUG
	D3DXCOLOR col;          // 透明
	D3DXMATRIX mtxWorld;    // マトリックス
#endif // _DEBUG
}Territory;

void InitTerritory(void);                                              //初期化処理
int SetTerritory(D3DXVECTOR3 pos, float fRadius, TERRITORY_TYPE Type); //設定処理
int LinkTerritory(int* nIdx, TERRITORY_TYPE Type);                     //リンク設定処理
int CheckTerritory(int* nIdx, int Quantity, D3DXVECTOR3 pos);          //チェック処理
D3DXVECTOR3 GetTerritoryPos(int nIdx);                                 //位置の取得

void UpdateTerritoryEdit(D3DXVECTOR3 Epos);                    //テリトリーーエディター
void LoadTerritory(void);                                      //設置情報読み込み
void SaveTerritory(void);                                      //設置情報書き出し
TERRITORY_TYPE GetTerritoryType(void);                         //タイプの取得
int GetDelTerritory(void);                                     //デリートナンバーの取得

void UninitTerritory(void); //終了処理
#ifdef _DEBUG
void DrawTerritory(void); //描画処理
#endif // _DEBUG

#endif //_TERRITORY_H_