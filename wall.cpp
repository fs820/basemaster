//----------------------------------------
//
//�Ǐ���[Wall.cpp]
//Author fuma sato
//
//----------------------------------------
#include"Wall.h"
#include"camera.h"
#include"stddxbuff.h"
#include"input.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;//�o�b�t�@�̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffWall = NULL; //�C���f�b�N�X�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;     //�e�N�X�`���̃|�C���^
#ifdef _DEBUG
WALL_TYPE g_Type;                             //�^�C�v
int g_DelNumberWall;                          //�ݒu�ԍ�
#endif // _DEBUG
bool g_bDisp = false;                         //�\�����
Wall g_aWall[WALL_MAX];     //����
//----------------------
//����������
//----------------------
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VT_MAX_WALL * WALL_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_DEFAULT,
		&g_pVtxBuffWall,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFileEx
	(
		pDevice,
		WALL_TEX,
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
		&g_pTextureWall
	);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * INDEX_NUM_WALL,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&g_pIdxBuffWall,
		NULL
	);

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//������
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].mtxWorld = D3DXMATRIX();
		g_aWall[nCntWall].Width = 0.0f;
		g_aWall[nCntWall].Height = 0.0f;
		g_aWall[nCntWall].bAlpha = false;
		g_aWall[nCntWall].bBack = false;
		g_aWall[nCntWall].bUse = false;
		g_aWall[nCntWall].Type = WTYPE_NORMAL;
	}
	//1��

	//���_�̐ݒ�
	if(FAILED(SetVertex3DYX
	(
		&g_pVtxBuffWall,
		0, WALL_MAX,
		WALL_YNUM, WALL_XNUM,
		WALL_WIDTH, WALL_HEIGHT,
		D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		WALL_WIDTH_DEF, WALL_HEIGHT_DEF,
		false, false
	)))
	{
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "���_�G���[", "WALL", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}

	//�C���f�b�N�X�̐ݒ�
	if (FAILED(SetIndex
	(
		&g_pIdxBuffWall,
		WALL_YNUM, WALL_XNUM
	)))
	{
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "�C���f�b�N�X�G���[", "WALL", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}

#ifdef _DEBUG
	//�ݒu�ԍ�
	g_DelNumberWall = -1;
#endif // _DEBUG
	//�\�����
	g_bDisp = false;
}

//-------------------
//�I������
//-------------------
void UninitWall(void)
{
	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//������
		g_aWall[nCntWall].bUse = false;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffWall != NULL)
	{
		g_pIdxBuffWall->Release();
		g_pIdxBuffWall = NULL;
	}
}

//-------------------
//�X�V����
//-------------------
void UpdateWall(void)
{

}

//-------------------
//�`�揈��
//-------------------
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	int nCntWall;
	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//�ő吔��
		if (g_aWall[nCntWall].bUse && (g_aWall[nCntWall].Type == WTYPE_NORMAL || g_bDisp ))
		{//�g���Ă�����
			//�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			//�����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

			//�ʒu�̌v�Z
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			//���_�o�b�t�@
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffWall);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWall);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				VT_MAX_WALL * nCntWall,
				0,
				VT_MAX_WALL,//���_��
				0,
				POLYGON_NUM_WALL//�|���S���̌�
			);
		}
	}
}

//----------
//�ݒ�
//----------
int SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, WALL_TYPE Type)
{
	int nCntWall, nIdx = 0;
	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//�ő吔��
		if (!g_aWall[nCntWall].bUse)
		{//�g���Ă��Ȃ�
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = rot;
			g_aWall[nCntWall].Width = Width;
			g_aWall[nCntWall].Height = Height;

			//���_�̐ݒ�
			if (FAILED(SetVertex3DYX
			(
				&g_pVtxBuffWall,
				nCntWall, 1,
				WALL_YNUM, WALL_XNUM,
				g_aWall[nCntWall].Width, g_aWall[nCntWall].Height,
				D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				WALL_WIDTH_DEF, WALL_HEIGHT_DEF,
				false, false
			)))
			{
				HWND hWnd;
				hWnd = GethWnd();
				MessageBox(hWnd, "���_�G���[", "WALL", MB_OK | MB_ICONERROR);
				PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
			}

			g_aWall[nCntWall].bAlpha = false;
			g_aWall[nCntWall].bBack = false;
			g_aWall[nCntWall].bUse = true;
			g_aWall[nCntWall].Type = Type;
			if (g_aWall[nCntWall].Type == WTYPE_INVISIBLE)
			{
				return nCntWall;
			}
			break;
		}
	}

	nIdx = nCntWall;

#ifndef _DEBUG
	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//�ő吔��
		if (!g_aWall[nCntWall].bUse)
		{//�g���Ă��Ȃ�
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = rot;
			g_aWall[nCntWall].rot.y -= D3DX_PI;
			//���K��
			if (g_aWall[nCntWall].rot.y > D3DX_PI)
			{
				g_aWall[nCntWall].rot.y -= D3DX_PI * 2.0f;
			}
			else if (g_aWall[nCntWall].rot.y < -D3DX_PI)
			{
				g_aWall[nCntWall].rot.y += D3DX_PI * 2.0f;
			}
			g_aWall[nCntWall].Width = Width;
			g_aWall[nCntWall].Height = Height;

			//���_�̐ݒ�
			if (FAILED(SetVertex3DYX
			(
				&g_pVtxBuffWall,
				nCntWall, 1,
				WALL_YNUM, WALL_XNUM,
				g_aWall[nCntWall].Width, g_aWall[nCntWall].Height,
				D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, BACK_WALL_ALPHA),
				WALL_WIDTH_DEF, WALL_HEIGHT_DEF,
				false, false
			)))
			{
				HWND hWnd;
				hWnd = GethWnd();
				MessageBox(hWnd, "���_�G���[", "WALL", MB_OK | MB_ICONERROR);
				PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
			}

			g_aWall[nCntWall].bAlpha = false;
			g_aWall[nCntWall].bBack = true;
			g_aWall[nCntWall].bUse = true;
			g_aWall[nCntWall].Type = WTYPE_NORMAL;
			break;
		}
	}
#endif // !_DEBUG

	return nIdx; //�Z�b�g�����ԍ���Ԃ�
}

//--------------------------
//���擾
//--------------------------
Wall* GetWall(void)
{
	return &g_aWall[0];
}

//----------------------
//�����蔻��
//----------------------
void CollisionWall(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float Width, float fHeight)
{
	D3DXVECTOR3 aPos[2] = {}, Wallvec = {}, Posvec = {}, PosOldvec = {}, movevec = {}, Norvec = {}, Dovec = {}, Hit = {};
	int nCntWall;
	float WallCross, PosCross;
	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//�ǂ̐�
		if (g_aWall[nCntWall].bUse && g_aWall[nCntWall].bBack == false)
		{//�g���Ă����
			aPos[0] = D3DXVECTOR3(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].Width * 0.5f * sinf(g_aWall[nCntWall].rot.y + D3DX_PI * -0.5f), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z + g_aWall[nCntWall].Width * 0.5f * cosf(g_aWall[nCntWall].rot.y + D3DX_PI * -0.5f));
			aPos[1] = D3DXVECTOR3(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].Width * 0.5f * sinf(g_aWall[nCntWall].rot.y + D3DX_PI * 0.5f), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z + g_aWall[nCntWall].Width * 0.5f * cosf(g_aWall[nCntWall].rot.y + D3DX_PI * 0.5f));

			if (g_aWall[nCntWall].Type==WTYPE_NORMAL)
			{
				Wallvec = aPos[1] - aPos[0];		//�ǂ̃x�N�g��

				Norvec = D3DXVECTOR3(Wallvec.z, Wallvec.y, -Wallvec.x);
				D3DXVec3Normalize(&Norvec, &Norvec);

				// �̕��̌v�Z
				D3DXVECTOR3 Pos = *pPos + -Norvec * Width, PosOld = *pPosOld + -Norvec * Width;

				Posvec = Pos - aPos[0];			//�ǂɑ΂���v���C���[�̃x�N�g��
				PosOldvec = PosOld - aPos[0];		//�ǂɑ΂���v���C���[�̋��x�N�g��
				movevec = Pos - PosOld;			//�v���C���[�̈ړ��x�N�g��
				if ((Wallvec.z * Posvec.x) - (Wallvec.x * Posvec.z) <= 0.0f && (Wallvec.z * PosOldvec.x) - (Wallvec.x * PosOldvec.z) >= 0.0f && pPos->y < g_aWall[nCntWall].pos.y + g_aWall[nCntWall].Height && pPos->y + fHeight > g_aWall[nCntWall].pos.y)
				{//�ǂ̊O��
					WallCross = (Wallvec.z * movevec.x) - (Wallvec.x * movevec.z);
					PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
					PosCross /= WallCross;
					if (PosCross >= -0.05f && PosCross < 1.05f)
					{//�ǂ͈̔�
						Hit = aPos[0] + Wallvec * PosCross;
						movevec = Pos - Hit;//�v���C���[�̈ړ��x�N�g��
						Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
						*pPos += Dovec * 1.1f;
					}
				}
			}
			else
			{
				Wallvec = aPos[1] - aPos[0];		//�ǂ̃x�N�g��
				Posvec = *pPos - aPos[0];			//�ǂɑ΂���v���C���[�̃x�N�g��
				PosOldvec = *pPosOld - aPos[0];		//�ǂɑ΂���v���C���[�̋��x�N�g��
				movevec = *pPos - *pPosOld;			//�v���C���[�̈ړ��x�N�g��
				if ((Wallvec.z * Posvec.x) - (Wallvec.x * Posvec.z) <= 0.0f && (Wallvec.z * PosOldvec.x) - (Wallvec.x * PosOldvec.z) >= 0.0f && pPos->y < g_aWall[nCntWall].pos.y + g_aWall[nCntWall].Height && pPos->y + fHeight > g_aWall[nCntWall].pos.y)
				{//�ǂ̊O��
					WallCross = (Wallvec.z * movevec.x) - (Wallvec.x * movevec.z);
					PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
					PosCross /= WallCross;
					if (PosCross >= -0.05f && PosCross < 1.05f)
					{//�ǂ͈̔�
						Hit = aPos[0] + Wallvec * PosCross;
						movevec = *pPos - Hit;//�v���C���[�̈ړ��x�N�g��
						Norvec = D3DXVECTOR3(Wallvec.z, Wallvec.y, -Wallvec.x);
						D3DXVec3Normalize(&Norvec, &Norvec);
						Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
						*pPos += Dovec * 1.1f;
					}
				}
			}
		}
	}
}

//----------------------
//�����蔻��
//----------------------
bool TriggerWall(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fHeight)
{
	bool bTrigger = false;

	D3DXVECTOR3 aPos[2] = {}, Wallvec = {}, Posvec = {}, PosOldvec = {}, movevec = {}, Norvec = {}, Dovec = {}, Hit = {};
	int nCntWall;
	float WallCross, PosCross;
	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//�ǂ̐�
		if (g_aWall[nCntWall].bUse && g_aWall[nCntWall].bBack == false)
		{//�g���Ă����
			aPos[0] = D3DXVECTOR3(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].Width * 0.5f * sinf(g_aWall[nCntWall].rot.y + D3DX_PI * -0.5f), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z + g_aWall[nCntWall].Width * 0.5f * cosf(g_aWall[nCntWall].rot.y + D3DX_PI * -0.5f));
			aPos[1] = D3DXVECTOR3(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].Width * 0.5f * sinf(g_aWall[nCntWall].rot.y + D3DX_PI * 0.5f), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z + g_aWall[nCntWall].Width * 0.5f * cosf(g_aWall[nCntWall].rot.y + D3DX_PI * 0.5f));

			Wallvec = aPos[1] - aPos[0];		//�ǂ̃x�N�g��
			Posvec = *pPos - aPos[0];			//�ǂɑ΂���v���C���[�̃x�N�g��
			PosOldvec = *pPosOld - aPos[0];		//�ǂɑ΂���v���C���[�̋��x�N�g��
			movevec = *pPos - *pPosOld;			//�v���C���[�̈ړ��x�N�g��
			if ((Wallvec.z * Posvec.x) - (Wallvec.x * Posvec.z) <= 0.0f && (Wallvec.z * PosOldvec.x) - (Wallvec.x * PosOldvec.z) >= 0.0f && pPos->y < g_aWall[nCntWall].pos.y + g_aWall[nCntWall].Height && pPos->y + fHeight > g_aWall[nCntWall].pos.y)
			{//�ǂ̊O��
				WallCross = (Wallvec.z * movevec.x) - (Wallvec.x * movevec.z);
				PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
				PosCross /= WallCross;
				if (PosCross >= -0.05f && PosCross < 1.05f)
				{//�ǂ͈̔�
					bTrigger = true;
					break;
				}
			}
		}
	}

	return bTrigger;
}

//---------------------------
//�ǂݍ���
//---------------------------
void LoadWall(void)
{
	FILE* pFile;//�t�@�C���|�C���^��錾
	MODE Mode = GetMode();
	int nCount = 0;
	int nNumber = 0;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Width = 0.0f, Height = 0.0f;

	//X�t�@�C��
	switch (Mode)
	{
	case MODE_GAME:
		pFile = fopen(WALL_FILE, "r");//�t�@�C�����J��
		break;
	case MODE_TUTO:
		pFile = fopen(WALL_FILE_TUTORIAL, "r");//�t�@�C�����J��
		break;
	default:
		pFile = fopen(nullptr, "r");//�t�@�C�����J��
		break;
	}

	if (pFile != NULL)
	{//�J������
		for (nCount = 0; nCount < WALL_MAX; nCount++)
		{
			if (fscanf(pFile, "%d", &nNumber) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &pos.x) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &pos.y) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &pos.z) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &rot.x) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &rot.y) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &rot.z) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &Width) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &Height) == EOF)break;//���l����������

			//�p�x�ϊ�
			rot = D3DXToRadian(rot);

			//�I�u�W�F�N�g�ݒu
			SetWall(pos, rot, Width, Height, (WALL_TYPE)nNumber);
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

//---------------------------
//��������
//---------------------------
void SaveWall(void)
{
	FILE* pFile;//�t�@�C���|�C���^��錾
	MODE Mode = GetMode();
	D3DXVECTOR3 rot;
	int nCount = 0;

	//X�t�@�C��
	switch (Mode)
	{
	case MODE_GAME:
		pFile = fopen(WALL_FILE, "w");//�t�@�C�����J��
		break;
	case MODE_TUTO:
		pFile = fopen(WALL_FILE_TUTORIAL, "w");//�t�@�C�����J��
		break;
	default:
		pFile = fopen(nullptr, "w");//�t�@�C�����J��
		break;
	}

	if (pFile != NULL)
	{//�J������
		for (nCount = 0; nCount < WALL_MAX; nCount++)
		{
			if (g_aWall[nCount].bUse && g_aWall[nCount].bBack == false)
			{
				//�p�x�ϊ�
				rot = D3DXToDegree(g_aWall[nCount].rot);

				fprintf(pFile, "%d\n", (int)g_aWall[nCount].Type);//���l����������
				fprintf(pFile, "%.1f ", g_aWall[nCount].pos.x);//���l����������
				fprintf(pFile, "%.1f ", g_aWall[nCount].pos.y);//���l����������
				fprintf(pFile, "%.1f\n", g_aWall[nCount].pos.z);//���l����������
				fprintf(pFile, "%.1f ", rot.x);//���l����������
				fprintf(pFile, "%.1f ", rot.y);//���l����������
				fprintf(pFile, "%.1f\n", rot.z);//���l����������
				fprintf(pFile, "%.1f ", g_aWall[nCount].Width);   //���l����������
				fprintf(pFile, "%.1f\n\n", g_aWall[nCount].Height);//���l����������
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

//------------------------
//�G�f�B�^�[
//------------------------
void UpdateWallEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot)
{
#ifdef _DEBUG
	Camera* pCamera = GetCamera();
	static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static float Width = 0.0f, Height = 0.0f;
	static int HoldNumber = -1, EndNumber = -1;

	if (GetKeyboardTrigger(DIK_9))
	{//�\���̐ؑ�
		g_bDisp = !g_bDisp;
	}

	if (GetKeyboardTrigger(DIK_RIGHT))
	{// �^�C�v�ؑ�
		g_Type = (WALL_TYPE)(g_Type + 1);
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{// �^�C�v�ؑ�
		g_Type = (WALL_TYPE)(g_Type - 1);
	}

	//�͈͐���
	g_Type = (WALL_TYPE)(g_Type % (WTYPE_MAX));

	if (g_Type < 0)
	{//���[�v
		g_Type = (WALL_TYPE)(WTYPE_MAX - 1);
	}

	if (GetKeyboardTrigger(DIK_UP))
	{// DEL�I�u�W�F�N�g�ؑ�
		g_DelNumberWall++;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{// DEL�I�u�W�F�N�g�ؑ�
		g_DelNumberWall--;
	}

	for (int nCount = 0; nCount < WALL_MAX; nCount++)
	{//�ő吔��

		//�͈͐���
		if (g_DelNumberWall < 0)
		{
			g_DelNumberWall = WALL_MAX - 1;
		}
		g_DelNumberWall %= WALL_MAX;

		if (g_aWall[g_DelNumberWall].bUse && g_aWall[g_DelNumberWall].bBack == false)
		{//�g���Ă���I�u�W�F�N�g�̑I��
			break;
		}
		else
		{//�g���Ă��Ȃ��ԍ��̓X�L�b�v

			if (GetKeyboardTrigger(DIK_DOWN))
			{// DEL�I�u�W�F�N�g�ؑ�
				g_DelNumberWall--;
			}
			else
			{// DEL�I�u�W�F�N�g�ؑ�
				g_DelNumberWall++;
			}
		}

		if (nCount >= WALL_MAX - 1)
		{//�ǂ���g���Ă��Ȃ��Ƃ�
			g_DelNumberWall = -1;
		}
	}

	if (GetKeyboardPress(DIK_F) == true || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) == true)//A�L�[
	{
		if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
		{//����
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.25f) * 5.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.25f) * 5.0f;
		}
		else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
		{//����
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.75f) * 5.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.75f) * 5.0f;
		}
		else
		{//��
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.5f) * 5.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.5f) * 5.0f;
		}
	}
	else if (GetKeyboardPress(DIK_H) == true || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) == true)//D�L�[
	{
		if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
		{//�E��
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * 5.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * 5.0f;
		}
		else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
		{//�E��
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * 5.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * 5.0f;
		}
		else
		{//�E
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * 5.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * 5.0f;
		}
	}
	else if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
	{//��
		pos.x += sinf(pCamera->rot.y) * 5.0f;
		pos.z += cosf(pCamera->rot.y) * 5.0f;
	}
	else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
	{//��
		pos.x += sinf(pCamera->rot.y + D3DX_PI) * 5.0f;
		pos.z += cosf(pCamera->rot.y + D3DX_PI) * 5.0f;
	}

	if (GetKeyboardPress(DIK_V) == true)//W�L�[
	{//��
		pos.y -= 5.0f;
	}
	if (GetKeyboardPress(DIK_B) == true)//S�L�[
	{//��
		pos.y += 5.0f;
	}

	if (GetKeyboardPress(DIK_J) || GetJoykeyPress(JOYKEY_LT, CONTROLLER_1))
	{// �����Ă����
		rot.y -= 0.05f;
	}
	if (GetKeyboardPress(DIK_K) || GetJoykeyPress(JOYKEY_RT, CONTROLLER_1))
	{// �����Ă����
		rot.y += 0.05f;
	}
	if (GetKeyboardPress(DIK_7) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// �����Ă����
		Width -= 10.0f;
	}
	if (GetKeyboardPress(DIK_8) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// �����Ă����
		Width += 10.0f;
	}
	if (GetKeyboardPress(DIK_U) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// �����Ă����
		Height -= 10.0f;
	}
	if (GetKeyboardPress(DIK_I) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// �����Ă����
		Height += 10.0f;
	}

	if (GetKeyboardTrigger(DIK_0) || GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1))
	{// ������
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Width = 0.0f;
		Height = 0.0f;
	}

	//���K��
	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	//�X�P�[������
	Width = max(WALL_WIDTH_MIN, min(WALL_WIDTH_MAX, Width));
	Height = max(WALL_HEIGHT_MIN, min(WALL_HEIGHT_MAX, Height));


	if (GetKeyboardTrigger(DIK_RSHIFT) || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1))
	{// �������Ƃ�
		HoldNumber = SetWall(Epos + pos, Erot + rot, Width, Height, g_Type);
		EndNumber = HoldNumber;
	}
	if (GetKeyboardPress(DIK_RSHIFT) || GetJoykeyPress(JOYKEY_RB, CONTROLLER_1))
	{// �����Ă����
		if (HoldNumber != -1)
		{
			g_aWall[HoldNumber].pos = Epos + pos;
			g_aWall[HoldNumber].rot = Erot + rot;
			g_aWall[HoldNumber].Width = Width;
			g_aWall[HoldNumber].Height = Height;

			//���_�̐ݒ�
			if (FAILED(SetVertex3DYX
			(
				&g_pVtxBuffWall,
				HoldNumber, 1,
				WALL_YNUM, WALL_XNUM,
				g_aWall[HoldNumber].Width, g_aWall[HoldNumber].Height,
				D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				WALL_WIDTH_DEF, WALL_HEIGHT_DEF,
				false, false
			)))
			{
				HWND hWnd;
				hWnd = GethWnd();
				MessageBox(hWnd, "���_�G���[", "WALL", MB_OK | MB_ICONERROR);
				PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
			}
		}
	}
	else
	{
		HoldNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_BACK) || GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1))
	{// �������Ƃ�
		g_aWall[EndNumber].bUse = false;
	}

	if (GetKeyboardTrigger(DIK_F8))
	{// �Z�[�u
		SaveWall();
	}

	if (GetKeyboardTrigger(DIK_DELETE) || (GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1) && GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1)))
	{// �������Ƃ�
		g_aWall[g_DelNumberWall].bUse = false;
	}

	static int DelNumber = -1; //�ꎞ�ۑ��p�i���o�[

	if (DelNumber != g_DelNumberWall)
	{//�f���[�g�i���o�[���ω�����
		if (DelNumber != -1)
		{//�O��̃I�u�W�F�N�g�����݂���
			g_aWall[DelNumber].bAlpha = false; //���ɖ߂�

			//���_�̐ݒ�
			if (FAILED(SetVertex3DYX
			(
				&g_pVtxBuffWall,
				DelNumber, 1,
				WALL_YNUM, WALL_XNUM,
				g_aWall[DelNumber].Width, g_aWall[DelNumber].Height,
				D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				WALL_WIDTH_DEF, WALL_HEIGHT_DEF,
				false, false
			)))
			{
				HWND hWnd;
				hWnd = GethWnd();
				MessageBox(hWnd, "���_�G���[", "WALL", MB_OK | MB_ICONERROR);
				PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
			}
		}
		DelNumber = g_DelNumberWall; //�I���I�u�W�F�N�g�̍X�V
		if (DelNumber != -1)
		{//�I�u�W�F�N�g�����݂���
		g_aWall[DelNumber].bAlpha = true; //���̒l�̕ێ�

			//���_�̐ݒ�
			if (FAILED(SetVertex3DYX
			(
				&g_pVtxBuffWall,
				DelNumber, 1,
				WALL_YNUM, WALL_XNUM,
				g_aWall[DelNumber].Width, g_aWall[DelNumber].Height,
				D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f),
				WALL_WIDTH_DEF, WALL_HEIGHT_DEF,
				false, false
			)))
			{
				HWND hWnd;
				hWnd = GethWnd();
				MessageBox(hWnd, "���_�G���[", "WALL", MB_OK | MB_ICONERROR);
				PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
			}
		}
	}
#endif // _DEBUG
}

//--------------------------------
//���̎擾
//--------------------------------
int GetWallNum(WALL_TYPE Type)
{
	int Num = 0;
	if (Type == WTYPE_MAX)
	{
		for (int nCnt = 0; nCnt < WALL_MAX; nCnt++)
		{
			if (g_aWall[nCnt].bUse)
			{
				Num++;
			}
		}
		return Num;
	}

	for (int nCnt = 0; nCnt < WALL_MAX; nCnt++)
	{
		if (g_aWall[nCnt].bUse && g_aWall[nCnt].Type==Type)
		{
			Num++;
		}
	}

	return Num;
}

//-----------------------
//�^�C�v
//-----------------------
int GetWallType(void)
{
#ifdef _DEBUG
	return g_Type;
#endif // _DEBUG
}

//-----------------------
//�^�C�v
//-----------------------
int GetDelWall(void)
{
#ifdef _DEBUG
	return g_DelNumberWall;
#endif // _DEBUG
}