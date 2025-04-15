//---------------------------------------
//
//�|���S���\�������̒�`�E�錾[bullet.h]
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

void InitBullet(void);//�|���S���̏���������
void UninitBullet(void);//�|���S���̏I������
void UpdateBullet(void);//�|���S���̍X�V����
void DrawBullet(void);//�|���S���̕`�揈��
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 dir, D3DXVECTOR3 scale);

#endif _BULLET_H_