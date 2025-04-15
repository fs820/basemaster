//---------------------------------------
//
//�G�t�F�N�g�\�������̒�`�E�錾[effect.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

#define EFFECT_TEX "data\\TEXTURE\\shadow000.jpg"
#define MAX_EFFECT (4096)//�ő吔
#define EFFECT_SIZE_DEF (20.0f)

typedef enum
{
	EFFECT_TYPE_NORMAL = 0,
	EFFECT_TYPE_MAX
}EFFECT_TYPE;

void InitEffect(void);//�G�t�F�N�g�̏���������
void UninitEffect(void);//�G�t�F�N�g�̏I������
void UpdateEffect(void);//�G�t�F�N�g�̍X�V����
void DrawEffect(void);//�G�t�F�N�g�̕`�揈��
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 scale, int nLife, EFFECT_TYPE Type);//�G�t�F�N�g�ݒ�

#endif _EFFECT_H_
