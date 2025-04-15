//=============================================================================
//
// �T�E���h���� [sound.h]
// Author None: 
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TEST,	// TEST BGM
	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label, float volume = 1.0f);
HRESULT StopSound(SOUND_LABEL label);
HRESULT StopSound(void);
HRESULT SetSoundVolume(SOUND_LABEL label, float volume);

#endif
