//----------------------------------------
//
//�o���A����[barrier.cpp]
//Author fuma sato
//
//----------------------------------------
#include"barrier.h"
#include"camera.h"
#include"stddx.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBarrier = NULL;//�o�b�t�@�̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffBarrier = NULL;//�C���f�b�N�X�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBarrier = NULL;    //�e�N�X�`���̃|�C���^
Barrier g_aBarrier[BARRIER_MAX]; //����
//----------------------
//����������
//----------------------
void InitBarrier(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�փ|�C���^
	VERTEX_3D* pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * (VT_MAX_BARRIER * BARRIER_MAX + BARRIER_TOP_MAX * 10),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_DEFAULT,
		&g_pVtxBuffBarrier,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFileEx
	(
		pDevice,
		BARRIER_TEX,
		D3DX_DEFAULT,              // �e�N�X�`���̕��iD3DX_DEFAULT�Ȃ玩�������j
		D3DX_DEFAULT,              // �e�N�X�`���̍����iD3DX_DEFAULT�Ȃ玩�������j
		D3DX_DEFAULT,              // �~�b�v�}�b�v���x���iD3DX_DEFAULT�Ŏ����ݒ�j
		0,                         // �g�p���Ȃ��ꍇ��0�i���I�e�N�X�`���Ȃ�D3DUSAGE_DYNAMIC�j
		D3DFMT_A8R8G8B8,           // �t�H�[�}�b�g�iD3DFMT_A8R8G8B8 �Ȃǁj
		D3DPOOL_DEFAULT,           // �������v�[���i�ʏ�� D3DPOOL_MANAGED�j
		D3DX_FILTER_TRIANGLE,      // �摜�̊g�k�t�B���^�iD3DX_FILTER_LINEAR �Ȃǁj
		D3DX_FILTER_TRIANGLE,      // �~�b�v�}�b�v�̃t�B���^
		0,                         // �J���[�L�[�i�����ɂ������F�j
		NULL,                      // `D3DXIMAGE_INFO`�i�摜�����擾����ꍇ�j
		NULL,                      // �p���b�g�f�[�^�i�p���b�g�Ȃ��Ȃ�NULL�j
		&g_pTextureBarrier
	);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * INDEX_NUM_BARRIER,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&g_pIdxBuffBarrier,
		NULL
	);

	int nCntBarrier;
	for (nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{//�ő吔��
		//���̂̏�����
		g_aBarrier[nCntBarrier].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBarrier[nCntBarrier].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBarrier[nCntBarrier].Width = 0.0f;
		g_aBarrier[nCntBarrier].Height = 0.0f;
		g_aBarrier[nCntBarrier].Type = BTYPE_GATE_1;
		g_aBarrier[nCntBarrier].Switch = OFF;
		g_aBarrier[nCntBarrier].col = BARRIER_DEF_COLOR;
		g_aBarrier[nCntBarrier].bTop = false;
		g_aBarrier[nCntBarrier].bUse = false;
	}
	//1��

	//���_�̐ݒ�
	SetVertex3DYX
	(
		&g_pVtxBuffBarrier,
		0, BARRIER_MAX,
		BARRIER_YNUM, BARRIER_XNUM,
		BARRIER_WIDTH, BARRIER_HEIGHT,
		D3DXVECTOR3(0.0f, 0.0f, -1.0f), BARRIER_DEF_COLOR,
		BARRIER_WIDTH, BARRIER_HEIGHT,
		false, false
	);

	//�W
	g_pVtxBuffBarrier->Lock(sizeof(VERTEX_3D) * VT_MAX_BARRIER * BARRIER_MAX, sizeof(VERTEX_3D) * 3 * 10, (void**)&pVtx, 0);

	for (int nCntTopBarrier = 0; nCntTopBarrier < BARRIER_TOP_MAX; nCntTopBarrier++)
	{
		for (int nCntFan = 0; nCntFan < 10; nCntFan++)
		{
			pVtx[nCntFan].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[nCntFan].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nCntFan].col = BARRIER_DEF_COLOR;
			pVtx[nCntFan].tex = D3DXVECTOR2(((BARRIER_WIDTH / BARRIER_WIDTH) / 10) * nCntFan, ((BARRIER_HEIGHT / BARRIER_HEIGHT) / 10) * nCntFan);
		}
		pVtx += 10;
	}

	g_pVtxBuffBarrier->Unlock();


	//�C���f�b�N�X�̐ݒ�
	SetIndex
	(
		&g_pIdxBuffBarrier,
		BARRIER_YNUM, BARRIER_XNUM
	);
}

//-------------------
//�I������
//-------------------
void UninitBarrier(void)
{
	for (int nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{//�ő吔��
		g_aBarrier[nCntBarrier].bUse = false;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureBarrier != NULL)
	{
		g_pTextureBarrier->Release();
		g_pTextureBarrier = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBarrier != NULL)
	{
		g_pVtxBuffBarrier->Release();
		g_pVtxBuffBarrier = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffBarrier != NULL)
	{
		g_pIdxBuffBarrier->Release();
		g_pIdxBuffBarrier = NULL;
	}
}

//-------------------
//�X�V����
//-------------------
void UpdateBarrier(void)
{
	VERTEX_3D* pVtx;
	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBarrier->Lock(0, 0, (void**)&pVtx, 0);
	static bool bLess[BARRIER_MAX] = { false };

	for (int nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{
		if (g_aBarrier[nCntBarrier].bUse == true)
		{
			if (g_aBarrier[nCntBarrier].bTop == true)
			{
				for (int nCntFan = 0; nCntFan < 10; nCntFan++)
				{
					// ���_�J���[�̐ݒ�
					pVtx[BARRIER_MAX * VT_MAX_BARRIER + (g_aBarrier[nCntBarrier].Type - BTYPE_ISOLATION_1) * 10 + nCntFan].col = g_aBarrier[nCntBarrier].col;
				}
			}
			else
			{
				// ���_�J���[�̐ݒ�
				pVtx[nCntBarrier * VT_MAX_BARRIER].col = g_aBarrier[nCntBarrier].col;
				pVtx[nCntBarrier * VT_MAX_BARRIER + 1].col = g_aBarrier[nCntBarrier].col;
				pVtx[nCntBarrier * VT_MAX_BARRIER + 2].col = g_aBarrier[nCntBarrier].col;
				pVtx[nCntBarrier * VT_MAX_BARRIER + 3].col = g_aBarrier[nCntBarrier].col;
			}

			if (bLess[nCntBarrier] == true)
			{
				g_aBarrier[nCntBarrier].col.a -= BARRIER_ALPHA_SPEED;
				if (g_aBarrier[nCntBarrier].col.a <= BARRIER_ALPHA_MIN)
				{
					bLess[nCntBarrier] = false;
				}
			}
			else
			{
				g_aBarrier[nCntBarrier].col.a += BARRIER_ALPHA_SPEED;
				if (g_aBarrier[nCntBarrier].col.a >= BARRIER_ALPHA_MAX)
				{
					bLess[nCntBarrier] = true;
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBarrier->Unlock();
}

//-------------------
//�`�揈��
//-------------------
void DrawBarrier(void)
{
	LPDIRECT3DDEVICE9 pDevice; // �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans; // �v�Z�}�g���b�N�X

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���݂̃J�����O�ݒ��ۑ�
	DWORD prevCullMode;
	pDevice->GetRenderState(D3DRS_CULLMODE, &prevCullMode);

	// �J�����O���I�t
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Z�e�X�g
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// �T���v���[�X�e�[�g
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);      // �e�N�X�`���J��Ԃ�(�~���[)
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);      // �e�N�X�`���J��Ԃ�(�~���[)

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	// 0
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// �����ق�����`�悷��
	static int nIdx[BARRIER_MAX]; // �������i�[�p
	static int nLimit = 0;
	if (nLimit % 30 == 0)
	{// �t���[���������Ƃɏ��� (���t���[�����Əd���Ȃ�\�������邽��)
		D3DXVECTOR3 PosList[BARRIER_MAX] = { D3DXVECTOR3(0.0f,0.0f,0.0f) }; // �|�X���X�g
		Camera* pCamera = GetCamera();
		for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
		{// �|�X���X�g�쐬
			if (g_aBarrier[nCnt].bUse && g_aBarrier[nCnt].Switch == ON)
			{
				PosList[nCnt] = g_aBarrier[nCnt].pos;
			}
			else
			{
				PosList[nCnt] = pCamera->posV;
			}
		}
		DrawCameraFarSort(nIdx, PosList, BARRIER_MAX); //�������Ƀ\�[�g
	}
	nLimit++;

	for (int nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{
		if (g_aBarrier[nIdx[nCntBarrier]].bUse == false || g_aBarrier[nIdx[nCntBarrier]].Switch == OFF)
		{
			break;
		}

		// �}�g���b�N�X������
		D3DXMatrixIdentity(&g_aBarrier[nIdx[nCntBarrier]].mtxWorld);

		// �����̔��f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBarrier[nIdx[nCntBarrier]].rot.y, g_aBarrier[nIdx[nCntBarrier]].rot.x, g_aBarrier[nIdx[nCntBarrier]].rot.z);
		D3DXMatrixMultiply(&g_aBarrier[nIdx[nCntBarrier]].mtxWorld, &g_aBarrier[nIdx[nCntBarrier]].mtxWorld, &mtxRot);

		// �ʒu�̌v�Z
		D3DXMatrixTranslation(&mtxTrans, g_aBarrier[nIdx[nCntBarrier]].pos.x, g_aBarrier[nIdx[nCntBarrier]].pos.y, g_aBarrier[nIdx[nCntBarrier]].pos.z);
		D3DXMatrixMultiply(&g_aBarrier[nIdx[nCntBarrier]].mtxWorld, &g_aBarrier[nIdx[nCntBarrier]].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aBarrier[nIdx[nCntBarrier]].mtxWorld);

		// ���_�o�b�t�@
		pDevice->SetStreamSource(0, g_pVtxBuffBarrier, 0, sizeof(VERTEX_3D));

		if (g_aBarrier[nIdx[nCntBarrier]].bTop == false)
		{
			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffBarrier);
		}

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBarrier);

		for (int i = 0; i < BARRIER_DRAW_NUM; i++)
		{
			if (g_aBarrier[nIdx[nCntBarrier]].bTop == true)
			{
				// �|���S���̕`��
				pDevice->DrawPrimitive
				(
					D3DPT_TRIANGLEFAN,            // �^�C�v
					VT_MAX_BARRIER * BARRIER_MAX + (g_aBarrier[nIdx[nCntBarrier]].Type - BTYPE_ISOLATION_1) * 10,
					8                             // �|���S���̌�
				);
			}
			else
			{
				// �|���S���̕`��
				pDevice->DrawIndexedPrimitive
				(
					D3DPT_TRIANGLESTRIP,        // �^�C�v
					VT_MAX_BARRIER * nIdx[nCntBarrier],
					0,
					VT_MAX_BARRIER,             // ���_��
					0,
					POLYGON_NUM_BARRIER         // �|���S���̌�
				);
			}
		}
	}

	// �e�N�X�`���X�e�[�W�X�e�[�g��߂�
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	// �X�e�[�W 1 �ȍ~�𖳌���
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	// �T���v���[�X�e�[�g��߂�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);      // �e�N�X�`���J��Ԃ�(�^�C��)
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);      // �e�N�X�`���J��Ԃ�(�^�C��)

	// Z�e�X�g��߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �J�����O�ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, prevCullMode);

	// ���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//----------
//�ݒ�
//----------
int SetBarrier(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, BTYPE Type, bool bTop, float Corner)
{
	int nCntBarrier;
	for (nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{//�ő吔��
		if (!g_aBarrier[nCntBarrier].bUse)
		{//�g���Ă��Ȃ�
			//�o���A�̐���
			g_aBarrier[nCntBarrier].pos = pos;
			g_aBarrier[nCntBarrier].rot = rot;
			g_aBarrier[nCntBarrier].Width = Width;
			g_aBarrier[nCntBarrier].Height = Height;
			g_aBarrier[nCntBarrier].Type = Type;
			if (bTop)
			{
				VERTEX_3D* pVtx;

				//�W
				g_pVtxBuffBarrier->Lock(sizeof(VERTEX_3D) * VT_MAX_BARRIER * BARRIER_MAX, sizeof(VERTEX_3D) * 3 * 10, (void**)&pVtx, 0);

				pVtx += (g_aBarrier[nCntBarrier].Type - BTYPE_ISOLATION_1) * 10;

				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(sinf(0.0f) * g_aBarrier[nCntBarrier].Height * 0.5f, 0.0f, cosf(0.0f) * g_aBarrier[nCntBarrier].Height * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f);
				pVtx[2].pos = D3DXVECTOR3(sinf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f) + D3DXVECTOR3(sinf(0.0f) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f);
				pVtx[3].pos = D3DXVECTOR3(sinf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f);
				pVtx[4].pos = D3DXVECTOR3(sinf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * 0.5f, 0.0f, cosf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f);
				pVtx[5].pos = D3DXVECTOR3(sinf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * 0.5f, 0.0f, cosf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f);
				pVtx[6].pos = D3DXVECTOR3(sinf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f);
				pVtx[7].pos = D3DXVECTOR3(sinf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f) + D3DXVECTOR3(sinf(0.0f) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f);
				pVtx[8].pos = D3DXVECTOR3(sinf(0.0f) * g_aBarrier[nCntBarrier].Height * 0.5f, 0.0f, cosf(0.0f) * g_aBarrier[nCntBarrier].Height * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f);
				pVtx[9].pos = D3DXVECTOR3(sinf(0.0f) * g_aBarrier[nCntBarrier].Height * 0.5f, 0.0f, cosf(0.0f) * g_aBarrier[nCntBarrier].Height * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f);

				for (int nCntFan = 0; nCntFan < 10; nCntFan++)
				{
					pVtx[nCntFan].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					pVtx[nCntFan].col = BARRIER_DEF_COLOR;
					pVtx[nCntFan].tex = D3DXVECTOR2(((BARRIER_WIDTH / BARRIER_WIDTH) / 10) * nCntFan, ((BARRIER_HEIGHT / BARRIER_HEIGHT) / 10) * nCntFan);
				}

				g_pVtxBuffBarrier->Unlock();
			}
			else
			{
				SetVertex3DYX
				(
					&g_pVtxBuffBarrier,
					nCntBarrier, 1,
					BARRIER_YNUM, BARRIER_XNUM,
					Width, Height,
					D3DXVECTOR3(0.0f, 0.0f, -1.0f), BARRIER_DEF_COLOR,
					Width, Height,
					false, false
				);
			}
			g_aBarrier[nCntBarrier].Switch = ON;
			g_aBarrier[nCntBarrier].bTop = bTop;
			g_aBarrier[nCntBarrier].bUse = true;
			break;
		}
	}

	return nCntBarrier; //�C���f�b�N�X��Ԃ�
}

//--------------------------
//�j��
//--------------------------
void EndBarrier(BTYPE Type)
{
	for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
	{
		if (g_aBarrier[nCnt].bUse && g_aBarrier[nCnt].Type == Type)
		{
			g_aBarrier[nCnt].bUse = false;
		}
	}
}

//--------------------------
//���擾
//--------------------------
Barrier* GetBarrier(void)
{
	return &g_aBarrier[0];
}

//--------------------------
//Switch
//--------------------------
void BarrierSwitch(BTYPE Type, SWITCH Switch)
{
	for (int nCount = 0; nCount < BARRIER_MAX; nCount++)
	{//�ő吔��
		if (g_aBarrier[nCount].bUse && g_aBarrier[nCount].Type == Type)
		{//�w�肳�ꂽ�^�C�v
			g_aBarrier[nCount].Switch = Switch; //�I���I�t
		}
	}
}

//----------------------
//�����蔻��
//----------------------
void CollisionBarrier(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fRadius, float fHeight)
{
	D3DXVECTOR3 aPos[2] = {}, Barriervec = {}, Posvec = {}, PosOldvec = {}, movevec = {}, Norvec = {}, Dovec = {}, Hit = {};
	int nCntBarrier;
	float BarrierCross, PosCross;

	for (nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{//�ǂ̐�
		if (g_aBarrier[nCntBarrier].bUse && g_aBarrier[nCntBarrier].Switch == ON)
		{//�g���Ă���O�ʂ̃o���A
			if (g_aBarrier[nCntBarrier].Type >= BTYPE_ISOLATION_1)
			{
				aPos[0] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f));
				aPos[1] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f));

				if (g_aBarrier[nCntBarrier].bTop==true)
				{
					switch (g_aBarrier[nCntBarrier].Type)
					{
					case BTYPE_ISOLATION_1:
						Barriervec = aPos[1] - aPos[0];//�ǂ̃x�N�g��

						// �W�̖@���x�N�g��
						Norvec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

						Posvec = *pPos - aPos[0];//�ǂɑ΂���v���C���[�̃x�N�g��
						PosOldvec = *pPosOld - aPos[0];//�ǂɑ΂���v���C���[�̋��x�N�g��
						movevec = *pPos - *pPosOld;//�v���C���[�̈ړ��x�N�g��

						if ((Barriervec.x * Posvec.y) - (Barriervec.y * Posvec.x) <= 0.0f && (Barriervec.x * PosOldvec.y) - (Barriervec.y * PosOldvec.x) >= 0.0f && pPos->z - fRadius < g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Height * 0.5f && pPos->z + fRadius > g_aBarrier[nCntBarrier].pos.z - g_aBarrier[nCntBarrier].Height * 0.5f)
						{//�ǂ̊O��
							BarrierCross = (Barriervec.x * movevec.y) - (Barriervec.y * movevec.x);
							PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
							PosCross /= BarrierCross;
							if (PosCross >= -0.1f && PosCross < 1.1f)
							{
								Hit = aPos[0] + Barriervec * PosCross;
								movevec = *pPos - Hit;//�v���C���[�̈ړ��x�N�g��
								Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
								*pPos += Dovec * 1.1f;

								D3DXVECTOR3 Space = *pPos - g_aBarrier[nCntBarrier].pos;
								Space.y = 0.0f;
								D3DXVec3Normalize(&Space, &Space);
								*pPos += Space * 5.0f;

								for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
								{
									if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
									{// �o���A�̃��A�N�V����
										g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
									}
								}
							}
						}
						break;
					case BTYPE_ISOLATION_2:
						Barriervec = aPos[1] - aPos[0];//�ǂ̃x�N�g��

						// �W�̖@���x�N�g��
						Norvec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

						Posvec = *pPos - aPos[0];//�ǂɑ΂���v���C���[�̃x�N�g��
						PosOldvec = *pPosOld - aPos[0];//�ǂɑ΂���v���C���[�̋��x�N�g��
						movevec = *pPos - *pPosOld;//�v���C���[�̈ړ��x�N�g��

						if ((Barriervec.x * Posvec.y) - (Barriervec.y * Posvec.x) <= 0.0f && (Barriervec.x * PosOldvec.y) - (Barriervec.y * PosOldvec.x) >= 0.0f && pPos->z - fRadius < g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Height * 0.5f && pPos->z + fRadius > g_aBarrier[nCntBarrier].pos.z - g_aBarrier[nCntBarrier].Height * 0.5f)
						{//�ǂ̊O��
							BarrierCross = (Barriervec.x * movevec.y) - (Barriervec.y * movevec.x);
							PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
							PosCross /= BarrierCross;
							if (PosCross >= -0.1f && PosCross < 1.1f)
							{
								Hit = aPos[0] + Barriervec * PosCross;
								movevec = *pPos - Hit;//�v���C���[�̈ړ��x�N�g��
								Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
								*pPos += Dovec * 1.1f;

								D3DXVECTOR3 Space = *pPos - g_aBarrier[nCntBarrier].pos;
								Space.y = 0.0f;
								D3DXVec3Normalize(&Space, &Space);
								*pPos += Space * 5.0f;

								for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
								{
									if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
									{// �o���A�̃��A�N�V����
										g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
									}
								}
							}
						}
						break;
					case BTYPE_ISOLATION_3:
						Barriervec = aPos[0] - aPos[1];//�ǂ̃x�N�g��

						// �W�̖@���x�N�g��
						Norvec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

						Posvec = *pPos - aPos[1];//�ǂɑ΂���v���C���[�̃x�N�g��
						PosOldvec = *pPosOld - aPos[1];//�ǂɑ΂���v���C���[�̋��x�N�g��
						movevec = *pPos - *pPosOld;//�v���C���[�̈ړ��x�N�g��

						if ((Barriervec.x * Posvec.y) - (Barriervec.y * Posvec.x) <= 0.0f && (Barriervec.x * PosOldvec.y) - (Barriervec.y * PosOldvec.x) >= 0.0f && pPos->z - fRadius < g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Height * 0.5f && pPos->z + fRadius > g_aBarrier[nCntBarrier].pos.z - g_aBarrier[nCntBarrier].Height * 0.5f)
						{//�ǂ̊O��
							BarrierCross = (Barriervec.x * movevec.y) - (Barriervec.y * movevec.x);
							PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
							PosCross /= BarrierCross;
							if (PosCross >= -0.1f && PosCross < 1.1f)
							{
								Hit = aPos[1] + Barriervec * PosCross;
								movevec = *pPos - Hit;//�v���C���[�̈ړ��x�N�g��
								Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
								*pPos += Dovec * 1.1f;

								D3DXVECTOR3 Space = *pPos - g_aBarrier[nCntBarrier].pos;
								Space.y = 0.0f;
								D3DXVec3Normalize(&Space, &Space);
								*pPos += Space * 5.0f;

								for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
								{
									if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
									{// �o���A�̃��A�N�V����
										g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
									}
								}
							}
						}
						break;
					}
				}
				else
				{
					Barriervec = aPos[0] - aPos[1];//�ǂ̃x�N�g��

					// �ǂ̖@���x�N�g�����v�Z
					Norvec = D3DXVECTOR3(Barriervec.z, Barriervec.y, -Barriervec.x);
					D3DXVec3Normalize(&Norvec, &Norvec);

					Posvec = *pPos - aPos[1];//�ǂɑ΂���v���C���[�̃x�N�g��
					PosOldvec = *pPosOld - aPos[1];//�ǂɑ΂���v���C���[�̋��x�N�g��
					movevec = *pPos - *pPosOld;//�v���C���[�̈ړ��x�N�g��

					if ((Barriervec.z * Posvec.x) - (Barriervec.x * Posvec.z) <= 0.0f && (Barriervec.z * PosOldvec.x) - (Barriervec.x * PosOldvec.z) >= 0.0f && pPos->y < g_aBarrier[nCntBarrier].pos.y + g_aBarrier[nCntBarrier].Height && pPos->y + fHeight > g_aBarrier[nCntBarrier].pos.y - 100.0f)
					{//�ǂ̊O��
						BarrierCross = (Barriervec.z * movevec.x) - (Barriervec.x * movevec.z);
						PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
						PosCross /= BarrierCross;
						if (PosCross >= -0.1f && PosCross < 1.1f)
						{
							Hit = aPos[1] + Barriervec * PosCross;
							movevec = *pPos - Hit;//�v���C���[�̈ړ��x�N�g��
							Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
							*pPos += Dovec * 1.1f;

							for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
							{
								if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
								{// �o���A�̃��A�N�V����
									g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
								}
							}
						}
					}
				}
			}
			else
			{
				const float offsetDistance = 125.0f; // �o���A�̑O�ɃI�t�Z�b�g���鋗��

				aPos[0] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f));
				aPos[1] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f));

				Barriervec = aPos[1] - aPos[0];//�ǂ̃x�N�g��

				// �ǂ̖@���x�N�g�����v�Z
				Norvec = D3DXVECTOR3(Barriervec.z, Barriervec.y, -Barriervec.x);
				D3DXVec3Normalize(&Norvec, &Norvec);

				// �o���A�̈ʒu��@�������ɃI�t�Z�b�g
				aPos[0] += Norvec * offsetDistance;
				aPos[1] += Norvec * offsetDistance;

				Posvec = *pPos - aPos[0];//�ǂɑ΂���v���C���[�̃x�N�g��
				PosOldvec = *pPosOld - aPos[0];//�ǂɑ΂���v���C���[�̋��x�N�g��
				movevec = *pPos - *pPosOld;//�v���C���[�̈ړ��x�N�g��

				if ((Barriervec.z * Posvec.x) - (Barriervec.x * Posvec.z) <= 0.0f && (Barriervec.z * PosOldvec.x) - (Barriervec.x * PosOldvec.z) >= 0.0f && pPos->y < g_aBarrier[nCntBarrier].pos.y + g_aBarrier[nCntBarrier].Height && pPos->y + fHeight > g_aBarrier[nCntBarrier].pos.y - 100.0f)
				{//�ǂ̊O��
					BarrierCross = (Barriervec.z * movevec.x) - (Barriervec.x * movevec.z);
					PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
					PosCross /= BarrierCross;
					if (g_aBarrier[nCntBarrier].Type <= BTYPE_GATE_4)
					{
						if (PosCross >= -1.0f && PosCross < 2.0f)
						{
							Hit = aPos[0] + Barriervec * PosCross;
							movevec = *pPos - Hit;//�v���C���[�̈ړ��x�N�g��
							Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
							*pPos += Dovec * 1.1f;

							for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
							{
								if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
								{// �o���A�̃��A�N�V����
									g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
								}
							}
						}
					}
					else
					{
						if (PosCross >= -0.1f && PosCross < 1.1f)
						{
							Hit = aPos[0] + Barriervec * PosCross;
							movevec = *pPos - Hit;//�v���C���[�̈ړ��x�N�g��
							Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
							*pPos += Dovec * 1.1f;

							for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
							{
								if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
								{// �o���A�̃��A�N�V����
									g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
								}
							}
						}
					}
				}

				aPos[0] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f));
				aPos[1] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f));

				Barriervec = aPos[0] - aPos[1];//�ǂ̔��x�N�g��

				// �ǂ̔��@���x�N�g�����v�Z
				Norvec = D3DXVECTOR3(Barriervec.z, Barriervec.y, -Barriervec.x);
				D3DXVec3Normalize(&Norvec, &Norvec);

				// �o���A�̈ʒu�𔽖@�������ɃI�t�Z�b�g
				aPos[0] += Norvec * offsetDistance;
				aPos[1] += Norvec * offsetDistance;

				Posvec = *pPos - aPos[1];//�ǂɑ΂���v���C���[�̃x�N�g��
				PosOldvec = *pPosOld - aPos[1];//�ǂɑ΂���v���C���[�̋��x�N�g��
				movevec = *pPos - *pPosOld;//�v���C���[�̈ړ��x�N�g��

				if ((Barriervec.z * Posvec.x) - (Barriervec.x * Posvec.z) <= 0.0f && (Barriervec.z * PosOldvec.x) - (Barriervec.x * PosOldvec.z) >= 0.0f && pPos->y < g_aBarrier[nCntBarrier].pos.y + g_aBarrier[nCntBarrier].Height && pPos->y + fHeight > g_aBarrier[nCntBarrier].pos.y - 100.0f)
				{//�ǂ̊O��
					BarrierCross = (Barriervec.z * movevec.x) - (Barriervec.x * movevec.z);
					PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
					PosCross /= BarrierCross;
					if (g_aBarrier[nCntBarrier].Type <= BTYPE_GATE_4)
					{
						if (PosCross >= -1.0f && PosCross < 2.0f)
						{
							Hit = aPos[1] + Barriervec * PosCross;
							movevec = *pPos - Hit;//�v���C���[�̈ړ��x�N�g��
							Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
							*pPos += Dovec * 1.1f;

							for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
							{
								if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
								{// �o���A�̃��A�N�V����
									g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
								}
							}
						}
					}
					else
					{
						if (PosCross >= -0.1f && PosCross < 1.1f)
						{
							Hit = aPos[1] + Barriervec * PosCross;
							movevec = *pPos - Hit;//�v���C���[�̈ړ��x�N�g��
							Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
							*pPos += Dovec * 1.1f;

							for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
							{
								if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
								{// �o���A�̃��A�N�V����
									g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
								}
							}
						}
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------
//����
//--------------------------------------------------------------------------------
void ReflectionBarrier(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius)
{
	D3DXVECTOR3 aPos[2] = {}, Barriervec = {}, Posvec = {}, PosOldvec = {}, movevec = {}, Norvec = {}, Dovec = {}, Hit = {};
	int nCntBarrier;
	float BarrierCross, PosCross;
	for (nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{//�ǂ̐�
		if (g_aBarrier[nCntBarrier].bUse && g_aBarrier[nCntBarrier].Switch == ON)
		{//�g���Ă���O�ʂ̃o���A
			if (g_aBarrier[nCntBarrier].Type >= BTYPE_ISOLATION_1)
			{
				const float offsetDistance = -125.0f; // �o���A�̑O�ɃI�t�Z�b�g���鋗��

				aPos[0] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f));
				aPos[1] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f));

				if (g_aBarrier[nCntBarrier].bTop == true)
				{
					switch (g_aBarrier[nCntBarrier].Type)
					{
					case BTYPE_ISOLATION_1:
						Barriervec = aPos[1] - aPos[0];//�ǂ̃x�N�g��

						// �W�̖@���x�N�g��
						Norvec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

						Posvec = *pPos - aPos[0];//�ǂɑ΂���x�N�g��
						PosOldvec = *pPosOld - aPos[0];//�ǂɑ΂��鋌�x�N�g��
						movevec = *pPos - *pPosOld;//�ړ��x�N�g��
						if ((Barriervec.x * Posvec.y) - (Barriervec.y * Posvec.x) <= 0.0f && (Barriervec.x * PosOldvec.y) - (Barriervec.y * PosOldvec.x) >= 0.0f && pPos->z - fRadius < g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Height * 0.5f && pPos->z + fRadius > g_aBarrier[nCntBarrier].pos.y - g_aBarrier[nCntBarrier].Height * 0.5f)
						{//�ǂ̊O��
							BarrierCross = (Barriervec.x * movevec.y) - (Barriervec.y * movevec.x);
							PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
							PosCross /= BarrierCross;
							if (PosCross >= 0.0f && PosCross < 1.0f)
							{
								Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
								*pMove += Dovec * 2.0f;
								*pPos += *pMove;

								for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
								{
									if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
									{// �o���A�̃��A�N�V����
										g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
									}
								}
							}
						}
						break;
					case BTYPE_ISOLATION_2:
						Barriervec = aPos[1] - aPos[0];//�ǂ̃x�N�g��

						// �W�̖@���x�N�g��
						Norvec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

						Posvec = *pPos - aPos[0];//�ǂɑ΂���x�N�g��
						PosOldvec = *pPosOld - aPos[0];//�ǂɑ΂��鋌�x�N�g��
						movevec = *pPos - *pPosOld;//�ړ��x�N�g��
						if ((Barriervec.x * Posvec.y) - (Barriervec.y * Posvec.x) <= 0.0f && (Barriervec.x * PosOldvec.y) - (Barriervec.y * PosOldvec.x) >= 0.0f && pPos->z - fRadius < g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Height * 0.5f && pPos->z + fRadius > g_aBarrier[nCntBarrier].pos.y - g_aBarrier[nCntBarrier].Height * 0.5f)
						{//�ǂ̊O��
							BarrierCross = (Barriervec.x * movevec.y) - (Barriervec.y * movevec.x);
							PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
							PosCross /= BarrierCross;
							if (PosCross >= 0.0f && PosCross < 1.0f)
							{
								Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
								*pMove += Dovec * 2.0f;
								*pPos += *pMove;

								for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
								{
									if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
									{// �o���A�̃��A�N�V����
										g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
									}
								}
							}
						}
						break;
					case BTYPE_ISOLATION_3:
						Barriervec = aPos[0] - aPos[1];//�ǂ̃x�N�g��

						// �W�̖@���x�N�g��
						Norvec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

						Posvec = *pPos - aPos[1];//�ǂɑ΂���x�N�g��
						PosOldvec = *pPosOld - aPos[1];//�ǂɑ΂��鋌�x�N�g��
						movevec = *pPos - *pPosOld;//�ړ��x�N�g��
						if ((Barriervec.x * Posvec.y) - (Barriervec.y * Posvec.x) <= 0.0f && (Barriervec.x * PosOldvec.y) - (Barriervec.y * PosOldvec.x) >= 0.0f && pPos->z - fRadius < g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Height * 0.5f && pPos->z + fRadius > g_aBarrier[nCntBarrier].pos.y - g_aBarrier[nCntBarrier].Height * 0.5f)
						{//�ǂ̊O��
							BarrierCross = (Barriervec.x * movevec.y) - (Barriervec.y * movevec.x);
							PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
							PosCross /= BarrierCross;
							if (PosCross >= 0.0f && PosCross < 1.0f)
							{
								Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
								*pMove += Dovec * 2.0f;
								*pPos += *pMove;

								for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
								{
									if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
									{// �o���A�̃��A�N�V����
										g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
									}
								}
							}
						}
						break;
					}
				}
				else
				{
					Barriervec = aPos[0] - aPos[1];//�ǂ̃x�N�g��

					// �ǂ̖@���x�N�g�����v�Z
					Norvec = D3DXVECTOR3(Barriervec.z, Barriervec.y, -Barriervec.x);
					D3DXVec3Normalize(&Norvec, &Norvec);

					// �o���A�̈ʒu��@�������ɃI�t�Z�b�g
					aPos[0] += Norvec * offsetDistance;
					aPos[1] += Norvec * offsetDistance;

					Posvec = *pPos - aPos[1];//�ǂɑ΂���x�N�g��
					PosOldvec = *pPosOld - aPos[1];//�ǂɑ΂��鋌�x�N�g��
					movevec = *pPos - *pPosOld;//�ړ��x�N�g��
					if ((Barriervec.z * Posvec.x) - (Barriervec.x * Posvec.z) <= 0.0f && (Barriervec.z * PosOldvec.x) - (Barriervec.x * PosOldvec.z) >= 0.0f && pPos->y - fRadius < g_aBarrier[nCntBarrier].pos.y + g_aBarrier[nCntBarrier].Height && pPos->y + fRadius > g_aBarrier[nCntBarrier].pos.y)
					{//�ǂ̊O��
						BarrierCross = (Barriervec.z * movevec.x) - (Barriervec.x * movevec.z);
						PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
						PosCross /= BarrierCross;
						if (PosCross >= 0.0f && PosCross < 1.0f)
						{
							Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
							*pMove += Dovec * 2.0f;
							*pPos += *pMove;

							for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
							{
								if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
								{// �o���A�̃��A�N�V����
									g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
								}
							}
						}
					}
				}
			}
			else
			{
				aPos[0] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f));
				aPos[1] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f));

				Barriervec = aPos[1] - aPos[0];//�ǂ̃x�N�g��
				Posvec = *pPos - aPos[0];//�ǂɑ΂���x�N�g��
				PosOldvec = *pPosOld - aPos[0];//�ǂɑ΂��鋌�x�N�g��
				movevec = *pPos - *pPosOld;//�ړ��x�N�g��
				if ((Barriervec.z * Posvec.x) - (Barriervec.x * Posvec.z) <= 0.0f && (Barriervec.z * PosOldvec.x) - (Barriervec.x * PosOldvec.z) >= 0.0f && pPos->y - fRadius < g_aBarrier[nCntBarrier].pos.y + g_aBarrier[nCntBarrier].Height && pPos->y + fRadius > g_aBarrier[nCntBarrier].pos.y)
				{//�ǂ̊O��
					BarrierCross = (Barriervec.z * movevec.x) - (Barriervec.x * movevec.z);
					PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
					PosCross /= BarrierCross;
					if (PosCross >= 0.0f && PosCross < 1.0f)
					{
						Norvec = D3DXVECTOR3(Barriervec.z, Barriervec.y, -Barriervec.x);
						D3DXVec3Normalize(&Norvec, &Norvec);
						Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
						*pMove += Dovec * 2.0f;
						*pPos += *pMove;

						for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
						{
							if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
							{// �o���A�̃��A�N�V����
								g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
							}
						}
					}
				}

				Barriervec = aPos[0] - aPos[1];//�ǂ̃x�N�g��
				Posvec = *pPos - aPos[1];//�ǂɑ΂���x�N�g��
				PosOldvec = *pPosOld - aPos[1];//�ǂɑ΂��鋌�x�N�g��
				movevec = *pPos - *pPosOld;//�ړ��x�N�g��
				if ((Barriervec.z * Posvec.x) - (Barriervec.x * Posvec.z) <= 0.0f && (Barriervec.z * PosOldvec.x) - (Barriervec.x * PosOldvec.z) >= 0.0f && pPos->y - fRadius < g_aBarrier[nCntBarrier].pos.y + g_aBarrier[nCntBarrier].Height && pPos->y + fRadius > g_aBarrier[nCntBarrier].pos.y)
				{//�ǂ̊O��
					BarrierCross = (Barriervec.z * movevec.x) - (Barriervec.x * movevec.z);
					PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
					PosCross /= BarrierCross;
					if (PosCross >= 0.0f && PosCross < 1.0f)
					{
						Norvec = D3DXVECTOR3(Barriervec.z, Barriervec.y, -Barriervec.x);
						D3DXVec3Normalize(&Norvec, &Norvec);
						Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
						*pMove += Dovec * 2.0f;
						*pPos += *pMove;

						for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
						{
							if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
							{// �o���A�̃��A�N�V����
								g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
							}
						}
					}
				}
			}
		}
	}
}