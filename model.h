//---------------------------------------
//
//モデル表示処理の定義・宣言[model.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _MODEL_H_
#define _MODEL_H_

#include"main.h"

typedef struct
{
	LPD3DXMESH pMesh;//頂点情報ポインタ
	LPD3DXBUFFER pBuffMat;//マテリアルポインタ
	LPDIRECT3DTEXTURE9 apTexture[64];
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posDef;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scale;
	D3DXMATRIX mtxWorld;
	DWORD dwNumMat;//マテリアル数
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	D3DXVECTOR3 vtxMinDef;
	D3DXVECTOR3 vtxMaxDef;
	int nNumVtx;
	DWORD sizeFVF;
	BYTE* pVtxBuff;
	int nIdxModelParent;
}Model;

void InitModel(void);//ポリゴンの初期化処理
void UninitModel(void);//ポリゴンの終了処理
void UpdateModel(void);//ポリゴンの更新処理
void DrawModel(void);//ポリゴンの描画処理

#endif _MODEL_H_