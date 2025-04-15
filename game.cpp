//------------------------------------------
//
//�Q�[�����C���̏���[game.cpp]
//Author fuma sato
//
//------------------------------------------

#include"game.h"
#include"input.h"
#include"player.h"
#include"bullet.h"
#include"explosion.h"
#include"effect.h"
#include"particle.h"
#include"model.h"
#include"pause.h"
#include"time.h"
#include"fade.h"
#include"sound.h"
#include"camera.h"
#include"hitshere.h"
#include"shadow.h"
#include"billboard.h"
#include"file.h"
#include"meshfield.h"
#include"cylinder.h"
#include"space.h"
#include "light.h"

//�O���[�o���ϐ�
GAMESTATE g_gameState = GAMESTATE_NONE;
int g_nCounterGameState = 0;
int g_GameTime = 0;
bool g_bSnow = false;
bool g_bClear = false;
//--------------------
//����������
//--------------------
void InitGame(void)
{
	InitModel();
	InitShadow();
	InitMeshField();
	InitCylinder();
	InitPlayer();//�v���C���[
	InitBullet();
	InitEffect();
	InitParticle();
	InitBillboard();
	InitHitShere();

	D3DLIGHT9 light; // ���C�g�̍\����
	// ���C�g���N���A����
	ZeroMemory(&light, sizeof(light));

	// ���C�g�̎�ނ�ݒ�
	light.Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g�̊g�U����ݒ�
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g�̕����x�N�g��
	D3DXVECTOR3 vecDir = D3DXVECTOR3(-3.0f, -10.0f, 1.0f);
	// ���K��
	D3DXVec3Normalize(&vecDir, &vecDir);
	// ���C�g�̕�����ݒ�
	light.Direction = vecDir; 

	// ���C�g��ݒ肷��
	SetLight(&light);

	//���
	SetSpace();
	SetMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetCylinder(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�J����
	GameCamera(CAMERA_XNUM * CAMERA_YNUM);

	D3DXVECTOR3 posScore;//�X�R�A�̈ʒu
	g_gameState = GAMESTATE_NORMAL;
	g_nCounterGameState = 0;
	g_GameTime = 0;
	g_bSnow = false;
	g_bClear = false;
}

//------------------
//�I������
//------------------
void UninitGame(void)
{
	g_gameState = GAMESTATE_NONE;

	UninitHitShere();
	UninitParticle();
	UninitEffect();
	UninitExplosion();
	UninitBullet();
	UninitBillboard();
	UninitCylinder();
	UninitMeshField();
	UninitPlayer();//�v���C���[
	UninitShadow();
	UninitModel();
}

//--------------
//�X�V����
//--------------
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true || GetJoykeyTrigger(JOYKEY_START, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_SENTER) == true)
	{
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			Pause();
		}
	}
	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (GetdJoykeyTrigger(ELEKEY_START, (CONTROLLER)i))
			{
				Pause();
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{
			if (GetdJoykeyTrigger(PSKEY_START, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					Pause();
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{
			if (GetdJoykeyTrigger(NINKEY_�{, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					Pause();
				}
			}
		}
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{
			if (GetdJoykeyTrigger(DKEY_START, (CONTROLLER)i))
			{
				Pause();
			}
		}
	}
	
	if (!bPause())
	{
		Player* pPlayer;

		pPlayer = GetPlayer();

		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:
			g_GameTime++;
			if (g_GameTime / FRAME >= TIME_LIMIT)
			{
				g_gameState = GAMESTATE_END;
				g_bClear = true;
			}
			else if (pPlayer->state == PLAYERSTATE_DIE)
			{
				g_gameState = GAMESTATE_END;
				g_bClear = false;
			}
			break;
		case GAMESTATE_END:
			g_nCounterGameState++;
			if (g_nCounterGameState >= INTER)
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//�ؑ�
					SetFade(MODE_RESULT, true);
				}
			}
			break;
		}

		UpdateModel();
		UpdateShadow();
		UpdatePlayer();//�v���C���[
		UpdateHitShere();
		UpdateMeshField();
		UpdateCylinder();
		UpdateBillboard();
		UpdateBullet();
		UpdateExplosion();
		UpdateEffect();
		UpdateParticle();
	}
}

//-------------------
//�`�揈��
//-------------------
void DrawGame(void)
{
	DrawModel();
	DrawMeshField();
	DrawCylinder();
	DrawShadow();
	DrawBillboard();
	DrawEffect();
	DrawBullet();
	DrawExplosion();
	DrawPlayer();//�v���C���[
	DrawHitShere();
}

//----------------------
//�Q�[����Ԑؑ�
//----------------------
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//-----------------------
//�Q�[����Ԏ擾
//-----------------------
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//-----------------------
//�X�m�[�擾
//-----------------------
bool GetbSnow(void)
{
	return g_bSnow;
}

//---------------------
//�N���A�擾
//---------------------
bool GetClear(void)
{
	return g_bClear;
}

//---------------------
//�N���A�擾
//---------------------
int GetClearTime(void)
{
	int H = (((g_GameTime / FRAME) * (int)(HOUR * ((float)MINUTE / (float)TIME_LIMIT))) / MINUTE) * 100;
	int M = ((g_GameTime / FRAME) * (int)(HOUR * ((float)MINUTE / (float)TIME_LIMIT))) % MINUTE;
	return H + M;
}

//---------------------
//�N���A�擾
//---------------------
int GetTime(void)
{
	return g_GameTime / FRAME;
}

//---------------------
//���ԏ�����
//---------------------
void TimeReset(void)
{
	g_GameTime = 0;
}