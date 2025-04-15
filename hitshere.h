//---------------------------------------
//
//�|���S���\�������̒�`�E�錾[hitshere.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _HITSHERE_H_
#define _HITSHERE_H_

#include"main.h"

#define HITSHERE_MAX (64)//���̐�
#define HITSHERE_RADIUS (1.0f)//���a
#define HITSHERE_HNUM (16)//H������
#define HITSHERE_VNUM (16)//V������
#define HITSHERE_ANG (180.0f)//�����S���ؑ�

//�S��
#define VT_MAX_HS ((HITSHERE_HNUM + 1) * (HITSHERE_VNUM-1)+2)//���̒��_��*�c�̃��C����(2�ɂ𔲂���)+2�� (�S��)
#define INDEX_HS_NUM (((HITSHERE_HNUM+1)*2)*(HITSHERE_VNUM-2)+(HITSHERE_VNUM-3)*2+(HITSHERE_HNUM+2)*2)//���̒��_��*2��**�c�̃��C����(2�ɂ𔲂���)+�k�ރ|���S��+2��
#define POLYGON_HS_NUM (HITSHERE_HNUM*(HITSHERE_VNUM-2)*2+(HITSHERE_VNUM-3)*4+HITSHERE_HNUM*2)//���̕���*2*�k�ރ|���S��+2��

#define DAMEGE (10)

typedef enum
{
	PLAYER = 0,
	ENEMY,
	MAX
}MASTER;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	D3DXMATRIX mtxWorld;
	int nLife;
	MASTER Master;
	bool bUse;
}HitShere;

void InitHitShere(void);//����������
void UninitHitShere(void);//�I������
void UpdateHitShere(void);//�X�V����
void DrawHitShere(void);//�`�揈��
void SetHitShere(D3DXVECTOR3 pos, D3DXVECTOR3 scale, int Life, MASTER Master);//�ݒ菈��
void TriggerShere(int Indx);

#endif _HITSHERE_H_