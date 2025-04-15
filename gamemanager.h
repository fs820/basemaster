//-----------------------------------------
//
//�����Ȃ��Ǐ����̒�`�E�錾[gamemanager.h
//
//-----------------------------------------

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include"main.h"

// �Q�[���i�s�Ǘ�
typedef enum
{
	EVENT_START = 0,          // �n�܂�
	EVENTSTATE_MAX
}EVENTSTATE;

//�v���g�^�C�v�錾
void GameManager(void);                // �Q�[���}�l�[�W���[
void StartEvent(void);                 // �C�x���g�X�^�[�g
void EndEvent(void);                   // �C�x���g�I��
void SetEvent(EVENTSTATE Event);       // �C�x���g�؂�ւ�
EVENTSTATE GetEvent(void);             // �C�x���g�擾
void LinkBarrier(void);                // �o���A�ݒ�
void LinkVirtualObjectCollision(void); // �I�u�W�F�N�g�ɓ����蔻���t�^����

#endif _GAMEMANAGER_H_