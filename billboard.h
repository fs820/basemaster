//---------------------------------------
//
//�|���S���\�������̒�`�E�錾[billboard.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include"main.h"

#define BILLBOARD_TEX "data\\TEXTURE\\wood.png"
#define BILLBOARD_MAX (64)

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	D3DXMATRIX mtxWorld;
	bool bUse;
	int nIdxShadow;
}Billboard;

void InitBillboard(void);//�|���S���̏���������
void UninitBillboard(void);//�|���S���̏I������
void UpdateBillboard(void);//�|���S���̍X�V����
void DrawBillboard(void);//�|���S���̕`�揈��
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 scale);

#endif _BILLBOARD_H_