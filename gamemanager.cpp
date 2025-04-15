//----------------------------------------
//
//�Q�[���Ǘ�����[gamemanager.cpp]
//Author fuma sato
//
//----------------------------------------
#include"gamemanager.h"
#include"game.h"
#include"player.h"
#include"item.h"
#include"interact.h"
#include"barrier.h"
#include"isolationfield.h"
#include"object.h"
#include"virtualcollision.h"
#include "sound.h"
#include <chrono>
#include "meshfield.h"
#include "wall.h"
#include "light.h"
#include "input.h"

// �O���[�o���ϐ�
EVENTSTATE g_EventState = EVENT_START; // �Q�[���i�s�Ǘ�
SOUND_LABEL g_NowBgm;
int g_CountEvent = 0; // �C�x���g�J�E���g
bool g_bStage[3] = { false }; // �X�e�[�W�̓����t���O
//------------------------
//�Q�[���}�l�[�W���[
//------------------------
void GameManager(void)
{
	extern MODE g_mode; // ���[�h�Q��
	extern Player g_player; // �v���C���[�Q��

	//�C���^���N�g�^�C�v �ؑ�
	if (IsJoyPadUsed())
	{//�R���g���[���[����
		SetInteractType(INTERACT_TYPE_JOY);
	}
	else if (IsKeyboardUsed() || IsMouseUsed())
	{//�L�[�{�[�h����
		SetInteractType(INTERACT_TYPE_KEY);
	}

	switch (g_mode)
	{
	case MODE_GAME:
		//�Q�[���i�s�Ǘ�
		switch (g_EventState)
		{
		case EVENT_START:
			break;
		}
	}
}

//---------------------
//�C�x���g�X�^�[�g
//---------------------
void StartEvent(void)
{
	SetEvent(EVENT_START);
}

//---------------------
//�C�x���g�X�^�[�g
//---------------------
void EndEvent(void)
{
	SetEvent(EVENT_START);
}

//--------------------------------
//�C�x���g�؂�ւ�
//--------------------------------
void SetEvent(EVENTSTATE Event)
{
	g_EventState = Event;
}

//-----------------------
//�C�x���g�擾
//-----------------------
EVENTSTATE GetEvent(void)
{
	return g_EventState;
}

//---------------------
//�o���A�̐ݒ�
//---------------------
void LinkBarrier(void)
{
	extern Object g_aObject[OBJECT_MAX]; //�I�u�W�F�N�g�Q��

}

//------------------------------------
//�I�u�W�F�N�g�ɓ����蔻���t�^����
//------------------------------------
void LinkVirtualObjectCollision(void)
{
	extern Object g_aObject[OBJECT_MAX]; //�I�u�W�F�N�g�Q��

}