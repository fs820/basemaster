//---------------------------------------
//
//ポリゴン表示処理の定義・宣言[meshfield.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include"main.h"
#include"game.h"

#define MESHFIELD_TEX "data\\TEXTURE\\field000.jpg"
#define MESHFIELD_TEX2 "data\\TEXTURE\\field000.jpg"
#define MESHFIELD_MAX (64)
#define MESHFIELD_WIDTH (8000.0f)
#define MESHFIELD_HEIGHT (0.0f)
#define MESHFIELD_Z (8000.0f)
#define MESHFIELD_ALPHA (0.001f)
#define MESHFIELD_XNUM (128)
#define MESHFIELD_ZNUM (128)
#define VT_MAX_MESH ((MESHFIELD_XNUM + 1) * (MESHFIELD_ZNUM + 1))
#define INDEX_NUM (((MESHFIELD_XNUM+1)*2)*MESHFIELD_ZNUM+(MESHFIELD_ZNUM-1)*2)
#define POLYGON_NUM (MESHFIELD_XNUM*MESHFIELD_ZNUM*2+(MESHFIELD_ZNUM-1)*4)

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	bool bAlpha;
	bool bUse;
}MeshField;

void InitMeshField(void);//ポリゴンの初期化処理
void UninitMeshField(void);//ポリゴンの終了処理
void UpdateMeshField(void);//ポリゴンの更新処理
void DrawMeshField(void);//ポリゴンの描画処理
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
MeshField* GetMeshField(void);

#endif _MESHFIELD_H_