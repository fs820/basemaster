//---------------------------------------
//
//�|���S���\�������̒�`�E�錾[earth.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EARTH_H_
#define _EARTH_H_

#include"main.h"

#define EARTH_TEX "data\\TEXTURE\\Base.jpg"//�e�N�X�`��

//�n���\����
typedef struct
{
	D3DXMATRIX mtxWorld;//�}�g���b�N�X
	bool bUse;//�g�p
}Earth;

void InitEarth(void);//����������
void UninitEarth(void);//�I������
void UpdateEarth(void);//�X�V����
void DrawEarth(void);//�`�揈��
void SetEarth(void);//�ݒ菈��

#endif _EARTH_H_