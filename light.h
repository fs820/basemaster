//---------------------------------------
//
//���C�g�����̒�`�E�錾[light.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

constexpr unsigned int LIGHT_MAX = 8; // ���C�g�̍ő吔

// ���C�g�\����
typedef struct
{
	D3DLIGHT9 light; // ���C�g���
	bool bUse;       // �g�p�t���O
}LIGHT;

//�v���g�^�C�v�錾
void InitLight(void);                                                 //����������
void UninitLight(void);                                               //�I������
void UpdateLight(void);                                               //�X�V����
LIGHT* GetLight(void);                                                //���C�g�擾
unsigned int SetLight(D3DLIGHT9* pLight);                             //���C�g�ݒ�
void EndLight(unsigned int nIdx);                                     //���C�g�j��
#endif // !_LIGHT_H_