//---------------------------------------
//
//�p�[�e�B�N���\�������̒�`�E�錾[particle.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include"main.h"

#define MAX_PARTICLE (128)//�ő吔
#define PARTICLE_NUM (5)//�t���[��������
#define PARTICLE_LIFE (100)//�c���

void InitParticle(void);//�p�[�e�B�N���̏���������
void UninitParticle(void);//�p�[�e�B�N���̏I������
void UpdateParticle(void);//�p�[�e�B�N���̍X�V����
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 scale);//�p�[�e�B�N���ݒ�

#endif _PARTICLE_H_