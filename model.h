//---------------------------------------
//
//���f���\�������̒�`�E�錾[model.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _MODEL_H_
#define _MODEL_H_

#include"main.h"

typedef struct
{
	LPD3DXMESH pMesh;//���_���|�C���^
	LPD3DXBUFFER pBuffMat;//�}�e���A���|�C���^
	LPDIRECT3DTEXTURE9 apTexture[64];
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posDef;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scale;
	D3DXMATRIX mtxWorld;
	DWORD dwNumMat;//�}�e���A����
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	D3DXVECTOR3 vtxMinDef;
	D3DXVECTOR3 vtxMaxDef;
	int nNumVtx;
	DWORD sizeFVF;
	BYTE* pVtxBuff;
	int nIdxModelParent;
}Model;

void InitModel(void);//�|���S���̏���������
void UninitModel(void);//�|���S���̏I������
void UpdateModel(void);//�|���S���̍X�V����
void DrawModel(void);//�|���S���̕`�揈��

#endif _MODEL_H_