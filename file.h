//---------------------------------------
//
//�Z�[�u�E���[�h�����̒�`�E�錾[file.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _FILE_H_
#define _FILE_H_

#include"main.h"

#define MAX_DATA (10)//�ۑ�����X�R�A�̐�
#define ROT_INA (D3DX_PI/180.0f)

//�v���g�^�C�v�錾
int* Soat(int* pData);//�\�[�g����
void SaveFile(int* pData);//�Z�[�u����
int* LoadFile(void);//���[�h����

#endif // _FILE_H_
