//---------------------------------------
//
//バリア処理の定義・宣言[barrier.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BARRIER_H_
#define _BARRIER_H_

#include"main.h"

#define BARRIER_FILE "data\\txt\\sato\\barrier.txt"                                //ファイル
#define BARRIER_TEX "data\\TEXTURE\\Base.jpg"                                      //テクスチャ
#define BARRIER_MAX (64)                                                           //最大数
#define BARRIER_WIDTH (350.0f)                                                     //高さ
#define BARRIER_HEIGHT (400.0f)                                                    //幅
#define BARRIER_Z (0.0f)                                                           //奥行
#define BARRIER_Z_DEF (0.0f)                                                       //テクスチャ用
#define BARRIER_XNUM (1)                                                           //幅分割
#define BARRIER_YNUM (1)                                                           //高さ分割
#define VT_MAX_BARRIER ((BARRIER_XNUM + 1) * (BARRIER_YNUM + 1))                   //頂点数
#define INDEX_NUM_BARRIER (((BARRIER_XNUM+1)*2)*BARRIER_YNUM+(BARRIER_YNUM-1)*2)   //インデックス数
#define POLYGON_NUM_BARRIER (BARRIER_XNUM*BARRIER_YNUM*2+(BARRIER_YNUM-1)*4)       //ポリゴン数
#define BARRIER_SCALE_MIN (0.5f)                                                   //大きさ最小
#define BARRIER_SCALE_MAX (2.0f)                                                   //大きさ最大

#define BARRIER_TOP_MAX (3)                                                   //蓋

// バリアの見た目変わる
#define BARRIER_DEF_COLOR D3DXCOLOR(0.3f,0.3f,0.3f,0.2f)                           //デフォルトカラー
#define BARRIER_DRAW_NUM (10)
#define BARRIER_ALPHA_SPEED (0.005f)
#define BARRIER_ALPHA_MIN (0.2f)
#define BARRIER_ALPHA_MAX (0.5f)
#define BARRIER_ALPHA_REACTION (0.8f)

//バリアタイプ
typedef enum
{
	BTYPE_1 = 0,
	BTYPE_MAX
}BTYPE;

typedef struct
{
	D3DXVECTOR3 pos;      //位置
	D3DXVECTOR3 rot;      //向き
	float Width;          //幅
	float Height;         //高さ
	D3DXMATRIX mtxWorld;  //マトリックス
	BTYPE Type;           //タイプ
	SWITCH Switch;        //Switch
	D3DXCOLOR col;        //カラー
	bool bTop;            //蓋
	bool bUse;            //使用
}Barrier;

void InitBarrier(void);                                                                                                 //初期化処理
void UninitBarrier(void);                                                                                               //終了処理
void UpdateBarrier(void);                                                                                               //更新処理
void DrawBarrier(void);                                                                                                 //描画処理
int SetBarrier(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, BTYPE Type, bool bTop, float Corner);       //設定
void EndBarrier(BTYPE Type);                                                                                            //破棄
Barrier* GetBarrier(void);                                                                                              //取得
void BarrierSwitch(BTYPE Type,SWITCH Switch);                                                                           //Switch
void CollisionBarrier(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fRadius, float fHeight);                           //壁刷り
void ReflectionBarrier(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius);                     //反射
#endif _BARRIER_H_