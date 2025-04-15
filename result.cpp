//------------------------------------------
//
//���U���g�̏���[result.cpp]
//Author fuma sato
//
//------------------------------------------

#include"result.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"game.h"
#include"score.h"
#include"file.h"

#define RESULT_TIME (600)
#define RESULT_WIDTH (200)
#define RESULT_HEIGHT (200)

LPDIRECT3DTEXTURE9 g_apTextureResult = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;//�o�b�t�@�̃|�C���^

//--------------------
//����������
//--------------------
void InitResult(void)
{
	HWND hWnd = GethWnd();

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^
	D3DXVECTOR3 posScore;//�X�R�A�̈ʒu
	int nScore = 0;
	int aPosTexUr[SCORE_MAX];

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * SCORE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NUMBER,
		&g_apTextureResult
	);


	//�X�R�A�̎擾
	nScore = GetClearTime();
	for (int i = 0; i < 2; i++)
	{
		aPosTexUr[i] = Digit(nScore, i);
	}
	aPosTexUr[2] = 10;
	for (int i = 3; i < 5; i++)
	{
		aPosTexUr[i] = Digit(nScore, i - 1);
	}
	posScore = D3DXVECTOR3(SCREEN_WIDTH / 2 - (SCORE_WIDTH / SCORE_MAX) * (SCORE_MAX / 2), SCREEN_HEIGHT / 2, 0.0f);

	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	for (int i = 0; i < SCORE_MAX; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(posScore.x - (SCORE_WIDTH / SCORE_MAX) / 2, posScore.y - SCORE_HEIGHT / 2, posScore.z);
		pVtx[1].pos = D3DXVECTOR3(posScore.x + (SCORE_WIDTH / SCORE_MAX) / 2, posScore.y - SCORE_HEIGHT / 2, posScore.z);
		pVtx[2].pos = D3DXVECTOR3(posScore.x - (SCORE_WIDTH / SCORE_MAX) / 2, posScore.y + SCORE_HEIGHT / 2, posScore.z);
		pVtx[3].pos = D3DXVECTOR3(posScore.x + (SCORE_WIDTH / SCORE_MAX) / 2, posScore.y + SCORE_HEIGHT / 2, posScore.z);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i], UV_DEF / V_MAX_S * (aPosTexUr[i] / U_MAX_S));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexUr[i] / U_MAX_S));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i], UV_DEF / V_MAX_S * (aPosTexUr[i] / U_MAX_S) + UV_DEF / V_MAX_S);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexUr[i] / U_MAX_S) + UV_DEF / V_MAX_S);

		posScore.x += SCORE_WIDTH / SCORE_MAX;
		pVtx += VT_MAX;
	}

	g_pVtxBuffResult->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
}

//------------------
//�I������
//------------------
void UninitResult(void)
{
	//�e�N�X�`���̔j��
	if (g_apTextureResult != NULL)
	{
		g_apTextureResult->Release();
		g_apTextureResult = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//--------------
//�X�V����
//--------------
void UpdateResult(void)
{
	static int nResultTime = 0;

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START, CONTROLLER_MAX) == true || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_LEFT) == true)
	{
		nResultTime = 0;
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//�ؑ�
			SetFade(MODE_RANK, true);
		}
	}

	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (GetdJoykeyTrigger(ELEKEY_A, (CONTROLLER)i) || GetdJoykeyTrigger(ELEKEY_START, (CONTROLLER)i))
			{
				nResultTime = 0;
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//�ؑ�
					SetFade(MODE_RANK, true);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{
			if (GetdJoykeyTrigger(PSKEY_CI, (CONTROLLER)i) || GetdJoykeyTrigger(PSKEY_START, (CONTROLLER)i))
			{
				nResultTime = 0;
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//�ؑ�
					SetFade(MODE_RANK, true);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{
			if (GetdJoykeyTrigger(NINKEY_A, (CONTROLLER)i) || GetdJoykeyTrigger(NINKEY_�{, (CONTROLLER)i))
			{
				nResultTime = 0;
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//�ؑ�
					SetFade(MODE_RANK, true);
				}
			}
		}
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{
			if (GetdJoykeyTrigger(DKEY_A, (CONTROLLER)i) || GetdJoykeyTrigger(DKEY_START, (CONTROLLER)i))
			{
				nResultTime = 0;
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//�ؑ�
					SetFade(MODE_RANK, true);
				}
			}
		}
	}

	if (nResultTime % RESULT_TIME == 0)
	{
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//�ؑ�
			SetFade(MODE_RANK, true);
		}
	}

	nResultTime++;
}

//-------------------
//�`�揈��
//-------------------
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < SCORE_MAX; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureResult);

		//�w�i�̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			i * VT_MAX,//�n�܂�̔ԍ�
			2//�|���S���̌�
		);
	}
}