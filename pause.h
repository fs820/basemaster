//------------------------------------------
//
//�|�[�Y�����̒�`�E�錾[pause.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _PAUSE_H_
#define _PAUSE_H_

#include"main.h"

#define TEXTURE_RESTART "data\\TEXTURE\\Base.jpg"
#define TEXTURE_RETRY "data\\TEXTURE\\Base.jpg"
#define TEXTURE_BACKTITLE "data\\TEXTURE\\Base.jpg"
#define TEXTURE_EXIT "data\\TEXTURE\\Base.jpg"

void InitPause(void);//�Q�[���̏���������
void UninitPause(void);//�Q�[���̏I������
void UpdatePause(void);//�Q�[���̍X�V����
void DrawPause(void);//�Q�[���̕`�揈��
void Pause(void);//�|�[�Y
bool bPause(void);//�|�[�Y���ǂ���

#endif _PAUSE_H_