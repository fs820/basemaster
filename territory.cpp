//---------------------------------------
//
//�e���g���[����[territory.cpp]
//Author fuma sato
//
//---------------------------------------
#include "territory.h"
#ifdef _DEBUG
#include "camera.h"
#include "stddxbuff.h"
#include "input.h"
#endif // _DEBUG

Territory g_aTerritory[TERRITORY_MAX];	// �e���g���[���
#ifdef _DEBUG
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTerritory = NULL; //�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTerritory = NULL;      //�e�N�X�`���̃|�C���^
TERRITORY_TYPE g_Type = TERRITORY_TYPE_A;	// �^�C�v
int g_nDelTerritory;					    // �f���[�g�i���o�[
#endif // _DEBUG
//******************************************************
// �e���g���[������
//******************************************************
void InitTerritory(void)
{
	for (int nCntTerritory = 0; nCntTerritory < TERRITORY_MAX; nCntTerritory++)
	{// �ő�e���g���[����
		// �e��ϐ��̏�����
		g_aTerritory[nCntTerritory].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aTerritory[nCntTerritory].fRadius = 0.0f;						    // ���a
		g_aTerritory[nCntTerritory].Type = TERRITORY_TYPE_A;				// �^�C�v
		g_aTerritory[nCntTerritory].bUse = false;						    // �g�p
#ifdef _DEBUG
		g_aTerritory[nCntTerritory].col = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);	// �F
#endif // _DEBUG
	}

#ifdef _DEBUG
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4* TERRITORY_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_DEFAULT,
		&g_pVtxBuffTerritory,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFileEx
	(
		pDevice,
		TERRITORY_TEX,
		D3DX_DEFAULT,              // �e�N�X�`���̕��iD3DX_DEFAULT�Ȃ玩�������j
		D3DX_DEFAULT,              // �e�N�X�`���̍����iD3DX_DEFAULT�Ȃ玩�������j
		D3DX_DEFAULT,              // �~�b�v�}�b�v���x���iD3DX_DEFAULT�Ŏ����ݒ�j
		0,                         // �g�p���Ȃ��ꍇ��0�i���I�e�N�X�`���Ȃ�D3DUSAGE_DYNAMIC�j
		D3DFMT_X8R8G8B8,           // �t�H�[�}�b�g�iA8R8G8B8 png X8R8G8B8 jpg�j
		D3DPOOL_DEFAULT,           // �������v�[���i�ʏ�� D3DPOOL_MANAGED�j
		D3DX_FILTER_TRIANGLE,      // �摜�̊g�k�t�B���^�iD3DX_FILTER_LINEAR �Ȃǁj
		D3DX_FILTER_TRIANGLE,      // �~�b�v�}�b�v�̃t�B���^
		0,                         // �J���[�L�[�i�����ɂ������F�j
		NULL,                      // `D3DXIMAGE_INFO`�i�摜�����擾����ꍇ�j
		NULL,                      // �p���b�g�f�[�^�i�p���b�g�Ȃ��Ȃ�NULL�j
		&g_pTextureTerritory
	);

	//���_�̐ݒ�
	SetVertex3DZX
	(
		&g_pVtxBuffTerritory,
		0,
		TERRITORY_MAX,
		1,
		1,
		1.0f,
		1.0f,
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f,
		1.0f
	);
#endif // _DEBUG
}

//******************************************************
// �e���g���[�ݒ�
//******************************************************
int SetTerritory(D3DXVECTOR3 pos, float fRadius, TERRITORY_TYPE Type)
{
	for (int nCntTerritory = 0; nCntTerritory < TERRITORY_MAX; nCntTerritory++)
	{// �ő�e���g���[����
		if (g_aTerritory[nCntTerritory].bUse == false)
		{// �g���Ă��Ȃ��e���g���[
			g_aTerritory[nCntTerritory].pos = pos;		    // �ʒu
			g_aTerritory[nCntTerritory].fRadius = fRadius;	// ���a
			g_aTerritory[nCntTerritory].Type = Type;		// �^�C�v
			g_aTerritory[nCntTerritory].bUse = true;		// �g�p

#ifdef _DEBUG

			switch (g_aTerritory[nCntTerritory].Type)
			{
			case TERRITORY_TYPE_A:
				g_aTerritory[nCntTerritory].col = D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f);
				break;
			case TERRITORY_TYPE_B:
				g_aTerritory[nCntTerritory].col = D3DXCOLOR(0.5f, 0.0f, 0.5f, 1.0f);
				break;
			case TERRITORY_TYPE_BOSS:
				g_aTerritory[nCntTerritory].col = D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f);
				break;
			}

			//���_�̐ݒ�
			SetVertex3DZX
			(
				&g_pVtxBuffTerritory,
				nCntTerritory,
				1,
				1,
				1,
				g_aTerritory[nCntTerritory].fRadius * 2.0f,
				g_aTerritory[nCntTerritory].fRadius * 2.0f,
				D3DXVECTOR3(0.0f, 1.0f, 0.0f),
				g_aTerritory[nCntTerritory].col,
				g_aTerritory[nCntTerritory].fRadius * 2.0f,
				g_aTerritory[nCntTerritory].fRadius * 2.0f
			);
#endif // _DEBUG
			return nCntTerritory;
		}
	}
	return -1;
}

//******************************************************
// �e���g���[�����N�ݒ�
//******************************************************
int LinkTerritory(int* nIdx, TERRITORY_TYPE Type)
{
	int nCnt = 0;

	for (int nCntTerritory = 0; nCntTerritory < TERRITORY_MAX; nCntTerritory++)
	{// �ő�e���g���[����
		if (g_aTerritory[nCntTerritory].bUse == true && g_aTerritory[nCntTerritory].Type == Type)
		{// �w��̃e���g���[
			nIdx[nCnt] = nCntTerritory;
			nCnt++;
		}
	}
	return nCnt;
}

//******************************************************
// �e���g���[�`�F�b�N
//******************************************************
int CheckTerritory(int* nIdx, int Quantity, D3DXVECTOR3 pos)
{
	for (int nCntIdx = 0; nCntIdx < Quantity; nCntIdx++)
	{// �ő�e���g���[����
		if (g_aTerritory[nIdx[nCntIdx]].bUse == true)
		{// �g���Ă���e���g���[
			D3DXVECTOR3 Space = pos - g_aTerritory[nIdx[nCntIdx]].pos;
			float fDistance = D3DXVec3Length(&Space);	// ����
			if (fDistance < g_aTerritory[nIdx[nCntIdx]].fRadius)
			{// �e���g���[��
				return nCntIdx;
			}
		}
	}

	return -1;
}

//******************************************************
// �e���g���[�ʒu�擾
//******************************************************
D3DXVECTOR3 GetTerritoryPos(int nIdx)
{
	return g_aTerritory[nIdx].pos;
}

//******************************************************
// �e���g���[�G�f�B�^�[�X�V
//******************************************************
void UpdateTerritoryEdit(D3DXVECTOR3 Epos)
{
#ifdef _DEBUG
	Camera* pCamera = GetCamera();
	static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static float fRadius = 100.0f;
	static int HoldNumber = -1, EndNumber = -1;

	if (GetKeyboardTrigger(DIK_RIGHT))
	{// �I�u�W�F�N�g�ؑ�
		g_Type = (TERRITORY_TYPE)(g_Type + 1);
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{// �I�u�W�F�N�g�ؑ�
		g_Type = (TERRITORY_TYPE)(g_Type - 1);
	}

	if (g_Type < 0)
	{//���[�v
		g_Type = (TERRITORY_TYPE)(TERRITORY_TYPE_MAX - 1);
	}

	//�͈͐���
	g_Type = (TERRITORY_TYPE)(g_Type % TERRITORY_TYPE_MAX);


	if (GetKeyboardTrigger(DIK_UP))
	{// DEL�I�u�W�F�N�g�ؑ�
		g_nDelTerritory++;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{// DEL�I�u�W�F�N�g�ؑ�
		g_nDelTerritory--;
	}

	for (int nCount = 0; nCount < TERRITORY_MAX; nCount++)
	{//�ő吔��
		//�͈͐���
		if (g_nDelTerritory < 0)
		{
			g_nDelTerritory = TERRITORY_MAX - 1;
		}
		g_nDelTerritory %= TERRITORY_MAX;

		if (g_aTerritory[g_nDelTerritory].bUse)
		{//�g���Ă���I�u�W�F�N�g�̑I��
			break;
		}
		else
		{//�g���Ă��Ȃ��ԍ��̓X�L�b�v

			if (GetKeyboardTrigger(DIK_DOWN))
			{// DEL�I�u�W�F�N�g�ؑ�
				g_nDelTerritory--;
			}
			else
			{// DEL�I�u�W�F�N�g�ؑ�
				g_nDelTerritory++;
			}
		}

		if (nCount >= TERRITORY_MAX - 1)
		{//�ǂ���g���Ă��Ȃ��Ƃ�
			g_nDelTerritory = -1;
		}
	}

	if (GetKeyboardPress(DIK_F) == true || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) == true)//A�L�[
	{
		if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
		{//����
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.25f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.25f) * 10.0f;
		}
		else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
		{//����
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.75f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.75f) * 10.0f;
		}
		else
		{//��
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.5f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.5f) * 10.0f;
		}
	}
	else if (GetKeyboardPress(DIK_H) == true || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) == true)//D�L�[
	{
		if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
		{//�E��
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * 10.0f;
		}
		else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
		{//�E��
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * 10.0f;
		}
		else
		{//�E
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * 10.0f;
		}
	}
	else if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
	{//��
		pos.x += sinf(pCamera->rot.y) * 10.0f;
		pos.z += cosf(pCamera->rot.y) * 10.0f;
	}
	else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
	{//��
		pos.x += sinf(pCamera->rot.y + D3DX_PI) * 10.0f;
		pos.z += cosf(pCamera->rot.y + D3DX_PI) * 10.0f;
	}

	if (GetKeyboardPress(DIK_V) == true)//W�L�[
	{//��
		pos.y -= 10.0f;
	}
	if (GetKeyboardPress(DIK_B) == true)//S�L�[
	{//��
		pos.y += 10.0f;
	}

	if (GetKeyboardPress(DIK_J) || GetJoykeyPress(JOYKEY_LT, CONTROLLER_1))
	{// �����Ă����
		fRadius -= 10.0f;
	}
	if (GetKeyboardPress(DIK_K) || GetJoykeyPress(JOYKEY_RT, CONTROLLER_1))
	{// �����Ă����
		fRadius += 10.0f;
	}

	if (GetKeyboardTrigger(DIK_0) || GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1))
	{// ������
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fRadius = 100.0f;
	}

	static TERRITORY_TYPE TypeOld = TERRITORY_TYPE_A; //�ꎞ�ۑ��p�i���o�[
	static int DelEnemyOld = -1; //�ꎞ�ۑ��p�i���o�[

	if (GetKeyboardTrigger(DIK_RSHIFT) || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1))
	{// �������Ƃ�
		HoldNumber = SetTerritory(Epos + pos,fRadius, g_Type);
		EndNumber = HoldNumber;
	}
	if (GetKeyboardPress(DIK_RSHIFT) || GetJoykeyPress(JOYKEY_RB, CONTROLLER_1))
	{// �����Ă����
		if (HoldNumber != -1)
		{
			g_aTerritory[HoldNumber].pos = Epos + pos;
			g_aTerritory[HoldNumber].fRadius = fRadius;

			//���_�̐ݒ�
			SetVertex3DZX
			(
				&g_pVtxBuffTerritory,
				HoldNumber,
				1,
				1,
				1,
				g_aTerritory[HoldNumber].fRadius * 2.0f,
				g_aTerritory[HoldNumber].fRadius * 2.0f,
				D3DXVECTOR3(0.0f, 1.0f, 0.0f),
				g_aTerritory[HoldNumber].col,
				g_aTerritory[HoldNumber].fRadius * 2.0f,
				g_aTerritory[HoldNumber].fRadius * 2.0f
			);

			static TERRITORY_TYPE Type = TERRITORY_TYPE_A; //�ꎞ�ۑ��p�i���o�[

			if (Type != g_Type)
			{//�i���o�[���ω�����
				//�I�u�W�F�N�g�̏���
				g_aTerritory[HoldNumber].bUse = false;
				HoldNumber = SetTerritory(Epos + pos, fRadius, g_Type);
				//�I�u�W�F�N�g�̐؂�ւ�
				EndNumber = HoldNumber;
				Type = g_Type; //�i���o�[�̍X�V
			}
		}
	}
	else
	{
		HoldNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_BACK) || GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1))
	{// �������Ƃ�
		g_aTerritory[EndNumber].bUse = false;
		EndNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_F8))
	{// �Z�[�u
		SaveTerritory();
	}

	if (GetKeyboardTrigger(DIK_DELETE) || (GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1) && GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1)))
	{// �������Ƃ�
		g_aTerritory[g_nDelTerritory].bUse = false;
	}

	static int DelNumber = -1; //�ꎞ�ۑ��p�i���o�[

	if (DelNumber != g_nDelTerritory)
	{//�f���[�g�i���o�[���ω�����
		if (DelNumber != -1)
		{//�O��̃I�u�W�F�N�g�����݂���
			g_aTerritory[DelNumber].col.a = 1.0f;

			//���_�̐ݒ�
			SetVertex3DZX
			(
				&g_pVtxBuffTerritory,
				DelNumber,
				1,
				1,
				1,
				g_aTerritory[DelNumber].fRadius * 2.0f,
				g_aTerritory[DelNumber].fRadius * 2.0f,
				D3DXVECTOR3(0.0f, 1.0f, 0.0f),
				g_aTerritory[DelNumber].col,
				g_aTerritory[DelNumber].fRadius * 2.0f,
				g_aTerritory[DelNumber].fRadius * 2.0f
			);
		}
		DelNumber = g_nDelTerritory; //�I���I�u�W�F�N�g�̍X�V
		if (DelNumber != -1)
		{//�I�u�W�F�N�g�����݂���
			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			g_aTerritory[DelNumber].col.a = 0.5f;

			//���_�̐ݒ�
			SetVertex3DZX
			(
				&g_pVtxBuffTerritory,
				DelNumber,
				1,
				1,
				1,
				g_aTerritory[DelNumber].fRadius * 2.0f,
				g_aTerritory[DelNumber].fRadius * 2.0f,
				D3DXVECTOR3(0.0f, 1.0f, 0.0f),
				g_aTerritory[DelNumber].col,
				g_aTerritory[DelNumber].fRadius * 2.0f,
				g_aTerritory[DelNumber].fRadius * 2.0f
			);
		}
	}
#endif // _DEBUG
}

//******************************************************
// �e���g���[�ǂݍ���
//******************************************************
void LoadTerritory(void)
{
	FILE* pFile;//�t�@�C���|�C���^��錾
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fRadius = 0.0f;
	char Str[256];
	bool bEndPos = false, bEndRadius = false;

	//X�t�@�C��
	pFile = fopen(TERRITORY_FILE, "r");//�t�@�C�����J��
	if (pFile != NULL)
	{//�J������
		do
		{
			//������̏�����
			for (int nCount = 0; nCount < 256; nCount++)
			{
				Str[nCount] = '\0';
			}

			//������̓ǂݎ��
			if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

			// ���s��#���폜
			Str[strcspn(Str, "\r\n")] = '\0';
			Str[strcspn(Str, "#")] = '\0';

			if (strstr(Str, "[TYPE_A]") != NULL)
			{//MadCatzA�̎�
				do
				{//���[�v
					//������̏�����
					for (int nCount = 0; nCount < 256; nCount++)
					{
						Str[nCount] = '\0';
					}

					//������̓ǂݎ��
					if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

					// ���s��#���폜
					Str[strcspn(Str, "\r\n")] = '\0';
					Str[strcspn(Str, "#")] = '\0';

					if (strstr(Str, "[TYPE_B]") != NULL)
					{
						fseek(pFile, -(long)(strlen(Str) * 4 + 1), SEEK_CUR);
						break;
					}

					//�ԍ��̍s��������
					if (strcspn(Str, "[") == 0 && strlen(Str) != 0)
					{
						do
						{
							//������̏�����
							for (int nCount = 0; nCount < 256; nCount++)
							{
								Str[nCount] = '\0';
							}

							if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

							if (strstr(Str, "POS") != NULL)
							{
								fseek(pFile, -(long)(strlen(Str) + 1), SEEK_CUR);
								//������̏�����
								for (int nCount = 0; nCount < 256; nCount++)
								{
									Str[nCount] = '\0';
								}

								if (fscanf(pFile, "%s", Str) == EOF)break;//���l����������

								if (strcmp(Str, "POS") == 0)
								{
									if (fscanf(pFile, "%s", Str) == EOF)break;//���l����������

									if (strcmp(Str, "=") == 0)
									{
										if (fscanf(pFile, "%f", &pos.x) == EOF)break;//���l����������
										if (fscanf(pFile, "%f", &pos.y) == EOF)break;//���l����������
										if (fscanf(pFile, "%f", &pos.z) == EOF)break;//���l����������
										bEndPos = true;
									}
								}
							}

							if (strstr(Str, "RADIUS") != NULL)
							{
								fseek(pFile, -(long)(strlen(Str) + 1), SEEK_CUR);
								//������̏�����
								for (int nCount = 0; nCount < 256; nCount++)
								{
									Str[nCount] = '\0';
								}

								if (fscanf(pFile, "%s", Str) == EOF)break;//���l����������

								if (strcmp(Str, "RADIUS") == 0)
								{
									if (fscanf(pFile, "%s", Str) == EOF)break;//���l����������

									if (strcmp(Str, "=") == 0)
									{
										if (fscanf(pFile, "%f", &fRadius) == EOF)break;//���l����������
										bEndRadius = true;
									}
								}
							}

							if (bEndPos && bEndRadius)
							{
								//�I�u�W�F�N�g�ݒu
								SetTerritory(pos, fRadius, TERRITORY_TYPE_A);
								bEndPos = false, bEndRadius = false;
								break;
							}
						} while (true);
					}
				} while (true);
			}
			else if (strstr(Str, "[TYPE_B]") != NULL)
			{//MadCatzB�̎�
				do
				{//���[�v
					//������̏�����
					for (int nCount = 0; nCount < 256; nCount++)
					{
						Str[nCount] = '\0';
					}

					//������̓ǂݎ��
					if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

					// ���s��#���폜
					Str[strcspn(Str, "\r\n")] = '\0';
					Str[strcspn(Str, "#")] = '\0';

					if (strstr(Str, "[TYPE_BOSS]") != NULL)
					{
						fseek(pFile, -(long)(strlen(Str) * 3), SEEK_CUR);
						break;
					}

					//�ԍ��̍s��������
					if (strcspn(Str, "[") == 0 && strlen(Str) != 0)
					{
						do
						{
							//������̏�����
							for (int nCount = 0; nCount < 256; nCount++)
							{
								Str[nCount] = '\0';
							}

							if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

							if (strstr(Str, "POS") != NULL)
							{
								fseek(pFile, -(long)(strlen(Str) + 1), SEEK_CUR);
								//������̏�����
								for (int nCount = 0; nCount < 256; nCount++)
								{
									Str[nCount] = '\0';
								}

								if (fscanf(pFile, "%s", Str) == EOF)break;//���l����������

								if (strcmp(Str, "POS") == 0)
								{
									if (fscanf(pFile, "%s", Str) == EOF)break;//���l����������

									if (strcmp(Str, "=") == 0)
									{
										if (fscanf(pFile, "%f", &pos.x) == EOF)break;//���l����������
										if (fscanf(pFile, "%f", &pos.y) == EOF)break;//���l����������
										if (fscanf(pFile, "%f", &pos.z) == EOF)break;//���l����������
										bEndPos = true;
									}
								}
							}

							if (strstr(Str, "RADIUS") != NULL)
							{
								fseek(pFile, -(long)(strlen(Str) + 1), SEEK_CUR);
								//������̏�����
								for (int nCount = 0; nCount < 256; nCount++)
								{
									Str[nCount] = '\0';
								}

								if (fscanf(pFile, "%s", Str) == EOF)break;//���l����������

								if (strcmp(Str, "RADIUS") == 0)
								{
									if (fscanf(pFile, "%s", Str) == EOF)break;//���l����������

									if (strcmp(Str, "=") == 0)
									{
										if (fscanf(pFile, "%f", &fRadius) == EOF)break;//���l����������
										bEndRadius = true;
									}
								}
							}

							if (bEndPos && bEndRadius)
							{
								//�I�u�W�F�N�g�ݒu
								SetTerritory(pos, fRadius, TERRITORY_TYPE_B);
								bEndPos = false, bEndRadius = false;
								break;
							}
						} while (true);
					}
				} while (true);
			}
			else if (strstr(Str, "[TYPE_BOSS]") != NULL)
			{//Xeros�̎�
				do
				{//���[�v
					//������̏�����
					for (int nCount = 0; nCount < 256; nCount++)
					{
						Str[nCount] = '\0';
					}

					//������̓ǂݎ��
					if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

					// ���s��#���폜
					Str[strcspn(Str, "\r\n")] = '\0';
					Str[strcspn(Str, "#")] = '\0';

					//�ԍ��̍s��������
					if (strcspn(Str, "[") == 0 && strlen(Str) != 0)
					{
						do
						{
							//������̏�����
							for (int nCount = 0; nCount < 256; nCount++)
							{
								Str[nCount] = '\0';
							}

							if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

							if (strstr(Str, "POS") != NULL)
							{
								fseek(pFile, -(long)(strlen(Str) + 1), SEEK_CUR);
								//������̏�����
								for (int nCount = 0; nCount < 256; nCount++)
								{
									Str[nCount] = '\0';
								}

								if (fscanf(pFile, "%s", Str) == EOF)break;//���l����������

								if (strcmp(Str, "POS") == 0)
								{
									if (fscanf(pFile, "%s", Str) == EOF)break;//���l����������

									if (strcmp(Str, "=") == 0)
									{
										if (fscanf(pFile, "%f", &pos.x) == EOF)break;//���l����������
										if (fscanf(pFile, "%f", &pos.y) == EOF)break;//���l����������
										if (fscanf(pFile, "%f", &pos.z) == EOF)break;//���l����������

										bEndPos = true;
									}
								}
							}

							if (strstr(Str, "RADIUS") != NULL)
							{
								fseek(pFile, -(long)(strlen(Str) + 1), SEEK_CUR);
								//������̏�����
								for (int nCount = 0; nCount < 256; nCount++)
								{
									Str[nCount] = '\0';
								}

								if (fscanf(pFile, "%s", Str) == EOF)break;//���l����������

								if (strcmp(Str, "RADIUS") == 0)
								{
									if (fscanf(pFile, "%s", Str) == EOF)break;//���l����������

									if (strcmp(Str, "=") == 0)
									{
										if (fscanf(pFile, "%f", &fRadius) == EOF)break;//���l����������
										bEndRadius = true;
									}
								}
							}

							if (bEndPos && bEndRadius)
							{
								//�I�u�W�F�N�g�ݒu
								SetTerritory(pos, fRadius, TERRITORY_TYPE_BOSS);
								bEndPos = false, bEndRadius = false;
								break;
							}
						} while (true);
					}
				} while (true);
			}
		} while (true);

		fclose(pFile);//�t�@�C�������
	}
	else
	{//�J���Ȃ�����
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "���[�h�G���[", "Territory", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//******************************************************
// �e���g���[�����o��
//******************************************************
void SaveTerritory(void)
{
	FILE* pFile;//�t�@�C���|�C���^��錾
	int nCount = 0;

	//X�t�@�C��
	pFile = fopen(TERRITORY_FILE, "w");//�t�@�C�����J��
	if (pFile != NULL)
	{//�J������

		fprintf(pFile, "[TYPE_A] #MadCatzA�̃e���g���[\n\n");//���l����������
		for (nCount = 0; nCount < TERRITORY_MAX; nCount++)
		{//�ő吔��
			if (g_aTerritory[nCount].bUse && g_aTerritory[nCount].Type == TERRITORY_TYPE_A)
			{//�g�p���Ă���
				fprintf(pFile, "[%d] #�C���f�b�N�X\n", nCount);//���l����������
				fprintf(pFile, "POS = %.1f ", g_aTerritory[nCount].pos.x);//���l����������
				fprintf(pFile, "%.1f ", g_aTerritory[nCount].pos.y);//���l����������
				fprintf(pFile, "%.1f #�ʒu\n", g_aTerritory[nCount].pos.z);//���l����������
				fprintf(pFile, "RADIUS = %.1f #���a\n\n", g_aTerritory[nCount].fRadius);//���l����������
			}
		}

		fprintf(pFile, "[TYPE_B] #MadCatzB�̃e���g���[\n\n");//���l����������
		for (nCount = 0; nCount < TERRITORY_MAX; nCount++)
		{//�ő吔��
			if (g_aTerritory[nCount].bUse && g_aTerritory[nCount].Type == TERRITORY_TYPE_B)
			{//�g�p���Ă���
				fprintf(pFile, "[%d] #�C���f�b�N�X\n", nCount);//���l����������
				fprintf(pFile, "POS = %.1f ", g_aTerritory[nCount].pos.x);//���l����������
				fprintf(pFile, "%.1f ", g_aTerritory[nCount].pos.y);//���l����������
				fprintf(pFile, "%.1f #�ʒu\n", g_aTerritory[nCount].pos.z);//���l����������
				fprintf(pFile, "RADIUS = %.1f #���a\n\n", g_aTerritory[nCount].fRadius);//���l����������
			}
		}

		fprintf(pFile, "[TYPE_BOSS] #Xeros�̃e���g���[\n\n");//���l����������
		for (nCount = 0; nCount < TERRITORY_MAX; nCount++)
		{//�ő吔��
			if (g_aTerritory[nCount].bUse && g_aTerritory[nCount].Type == TERRITORY_TYPE_BOSS)
			{//�g�p���Ă���
				fprintf(pFile, "[%d] #�C���f�b�N�X\n", nCount);//���l����������
				fprintf(pFile, "POS = %.1f ", g_aTerritory[nCount].pos.x);//���l����������
				fprintf(pFile, "%.1f ", g_aTerritory[nCount].pos.y);//���l����������
				fprintf(pFile, "%.1f #�ʒu\n", g_aTerritory[nCount].pos.z);//���l����������
				fprintf(pFile, "RADIUS = %.1f #���a\n\n", g_aTerritory[nCount].fRadius);//���l����������
			}
		}

		fclose(pFile);//�t�@�C�������
	}
	else
	{//�J���Ȃ�����
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "���[�h�G���[", "���[�h�ł��Ȃ�������", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//******************************************************
// �^�C�v�擾
//******************************************************
TERRITORY_TYPE GetTerritoryType(void)
{
#ifdef _DEBUG
	return g_Type;
#endif // _DEBUG
}

//******************************************************
// �f���[�g�i���o�[�擾
//******************************************************
int GetDelTerritory(void)
{
#ifdef _DEBUG
	return g_nDelTerritory;
#endif // _DEBUG
}

//-----------------------------------------------------
//�I������
//-----------------------------------------------------
void UninitTerritory(void)
{
	for (int nCntTerritory = 0; nCntTerritory < TERRITORY_MAX; nCntTerritory++)
	{// �ő�e���g���[����
		// �e��ϐ��̏�����
		g_aTerritory[nCntTerritory].bUse = false;						    // �g�p
	}

#ifdef _DEBUG
	if (g_pTextureTerritory != NULL)
	{
		g_pTextureTerritory->Release();
		g_pTextureTerritory = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTerritory != NULL)
	{
		g_pVtxBuffTerritory->Release();
		g_pVtxBuffTerritory = NULL;
	}
#endif // _DEBUG
}

#ifdef _DEBUG
//------------------------------------------------------
//�`�揈��
//------------------------------------------------------
void DrawTerritory(void)
{
	LPDIRECT3DDEVICE9 pDevice; // �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxTrans; // �v�Z�}�g���b�N�X

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0); // 0�̕����K�� sato
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Z�e�X�g
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Depth Bias �ݒ� sato
	float depthBias = -0.000001f; //Z�o�b�t�@���J���������ɃI�t�Z�b�g����l

	// �����ق�����`�悷��
	static int nIdx[TERRITORY_MAX]; // �������i�[�p
	static int nLimit = 0;
	if (nLimit % 60 == 0)
	{// �t���[�����Ƃɏ��� (���t���[�����Əd���Ȃ�\�������邽��)
		D3DXVECTOR3 PosList[TERRITORY_MAX]; // �|�X���X�g
		Camera* pCamera = GetCamera();
		for (int nCnt = 0; nCnt < TERRITORY_MAX; nCnt++)
		{// �|�X���X�g�쐬
			if (g_aTerritory[nCnt].bUse)
			{
				PosList[nCnt] = g_aTerritory[nCnt].pos;
			}
			else
			{
				PosList[nCnt] = pCamera->posV;
			}
		}
		DrawCameraFarSort(nIdx, PosList, TERRITORY_MAX); //�������Ƀ\�[�g
	}
	nLimit++;

	for (int nCntTerritory = 0; nCntTerritory < TERRITORY_MAX; nCntTerritory++)
	{
		if (g_aTerritory[nIdx[nCntTerritory]].bUse == false)
		{
			break;
		}

		depthBias += -0.000001f; //�e���m�̏d�Ȃ�������
		pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Z�o�C�A�X�ݒ�

		// �}�g���b�N�X������
		D3DXMatrixIdentity(&g_aTerritory[nIdx[nCntTerritory]].mtxWorld);

		// �ʒu�̌v�Z
		D3DXMatrixTranslation(&mtxTrans, g_aTerritory[nIdx[nCntTerritory]].pos.x, g_aTerritory[nIdx[nCntTerritory]].pos.y, g_aTerritory[nIdx[nCntTerritory]].pos.z);
		D3DXMatrixMultiply(&g_aTerritory[nIdx[nCntTerritory]].mtxWorld, &g_aTerritory[nIdx[nCntTerritory]].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aTerritory[nIdx[nCntTerritory]].mtxWorld);

		// ���_�o�b�t�@
		pDevice->SetStreamSource(0, g_pVtxBuffTerritory, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTerritory);

		// �|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,        // �^�C�v
			4 * nIdx[nCntTerritory],
			2                          // �|���S���̌�
		);
	}

	// Depth Bias �ݒ������ sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// Z�e�X�g��߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// �A���t�@�e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// �ݒ�������ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
#endif // _DEBUG