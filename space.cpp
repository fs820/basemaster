//----------------------------------------
//
//Space����[space.cpp]
//Author fuma sato
//
//----------------------------------------

#include "space.h"
#include "camera.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSpace = NULL;    //�o�b�t�@�̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSpace = NULL;     //�C���f�b�N�X�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureSpace[SPACE_TEX_MAX] = { NULL };  //�e�N�X�`���̃|�C���^
Space g_Space;                                     //���i�[�֐�
//----------------------
//����������
//----------------------
void InitSpace(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_3D_MULTI* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();


	//�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D_MULTI) * VT_MAX_SP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D_MULTI,
		D3DPOOL_DEFAULT,
		&g_pVtxBuffSpace,
		NULL
	);

	//���̏�����
	for (int nCntTex = 0; nCntTex < SPACE_TEX_MAX; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureSpace[nCntTex] != NULL)
		{
			g_pTextureSpace[nCntTex]->Release();
			g_pTextureSpace[nCntTex] = NULL;
		}

		g_Space.Texture[nCntTex].TexPass = TexPass[nCntTex];

		D3DXIMAGE_INFO ImageInfo;
		//�e�N�X�`���̓ǂݍ���
		HRESULT hr = D3DXCreateTextureFromFileEx
		(
			pDevice,
			g_Space.Texture[nCntTex].TexPass.aName,
			D3DX_DEFAULT,              // �e�N�X�`���̕��iD3DX_DEFAULT�Ȃ玩�������j
			D3DX_DEFAULT,              // �e�N�X�`���̍����iD3DX_DEFAULT�Ȃ玩�������j
			D3DX_DEFAULT,              // �~�b�v�}�b�v���x���iD3DX_DEFAULT�Ŏ����ݒ�j
			0,                         // �g�p���Ȃ��ꍇ��0�i���I�e�N�X�`���Ȃ�D3DUSAGE_DYNAMIC�j
			D3DFMT_A8R8G8B8,           // �t�H�[�}�b�g�iA8R8G8B8 png X8R8G8B8 jpg�j
			D3DPOOL_DEFAULT,           // �������v�[���i�ʏ�� D3DPOOL_MANAGED�j
			D3DX_FILTER_TRIANGLE,      // �摜�̊g�k�t�B���^�iD3DX_FILTER_LINEAR �Ȃǁj
			D3DX_FILTER_TRIANGLE,      // �~�b�v�}�b�v�̃t�B���^
			0,                         // �J���[�L�[�i�����ɂ������F�j
			&ImageInfo,                // `D3DXIMAGE_INFO`�i�摜�����擾����ꍇ�j
			NULL,                      // �p���b�g�f�[�^�i�p���b�g�Ȃ��Ȃ�NULL�j
			&g_pTextureSpace[nCntTex]
		);

		if (SUCCEEDED(hr))
		{
			g_Space.Texture[nCntTex].TexSize = D3DXVECTOR2((float)ImageInfo.Width, (float)ImageInfo.Height) * TexScale[nCntTex];
		}
		else
		{
			//�e�N�X�`���̔j��
			if (g_pTextureSpace[nCntTex] != NULL)
			{
				g_pTextureSpace[nCntTex]->Release();
				g_pTextureSpace[nCntTex] = NULL;
			}

			//�e�N�X�`���̓ǂݍ���
			HRESULT hr = D3DXCreateTextureFromFileEx
			(
				pDevice,
				g_Space.Texture[nCntTex].TexPass.aName,
				D3DX_DEFAULT,              // �e�N�X�`���̕��iD3DX_DEFAULT�Ȃ玩�������j
				D3DX_DEFAULT,              // �e�N�X�`���̍����iD3DX_DEFAULT�Ȃ玩�������j
				D3DX_DEFAULT,              // �~�b�v�}�b�v���x���iD3DX_DEFAULT�Ŏ����ݒ�j
				0,                         // �g�p���Ȃ��ꍇ��0�i���I�e�N�X�`���Ȃ�D3DUSAGE_DYNAMIC�j
				D3DFMT_A8R8G8B8,           // �t�H�[�}�b�g�iA8R8G8B8 png X8R8G8B8 jpg�j
				D3DPOOL_SYSTEMMEM,          // �������v�[���i�ʏ�� D3DPOOL_MANAGED�j
				D3DX_FILTER_LINEAR,        // �摜�̊g�k�t�B���^�iD3DX_FILTER_LINEAR �Ȃǁj
				D3DX_FILTER_LINEAR,        // �~�b�v�}�b�v�̃t�B���^
				0,                         // �J���[�L�[�i�����ɂ������F�j
				0,                         // `D3DXIMAGE_INFO`�i�摜�����擾����ꍇ�j
				NULL,                      // �p���b�g�f�[�^�i�p���b�g�Ȃ��Ȃ�NULL�j
				&g_pTextureSpace[nCntTex]
			);

			g_Space.Texture[nCntTex].TexSize = D3DXVECTOR2(6283.0f, 1571.0f) * TexScale[nCntTex];
		}
	}
	g_Space.bUse = false;//�g�p

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * INDEX_SP_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&g_pIdxBuffSpace,
		NULL
	);

	//���_���̐ݒ�
	g_pVtxBuffSpace->Lock(0, 0, (void**)&pVtx, 0);

	//�ɒ��_
	pVtx[0].pos = D3DXVECTOR3(0.0f, SPACE_RADIUS, 0.0f);

	//�@���x�N�g��
	D3DXVECTOR3 npos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), dpos = D3DXVECTOR3(0.0f, SPACE_RADIUS, 0.0f);
	npos = npos - dpos;
	D3DXVec3Normalize(&npos, &npos);
	pVtx[0].nor = npos;

	//�J���[
	pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.1f);

	//�e�N�X�`��
	pVtx[0].tex1 = D3DXVECTOR2(0.0f,0.0f);

	//�e�N�X�`��
	pVtx[0].tex2 = D3DXVECTOR2(0.0f, 0.0f);

	for (int nCntSpace = 0; nCntSpace < SPACE_VNUM; nCntSpace++)
	{//1������Z���̃��[�v
		for (int nCntSpace2 = 0; nCntSpace2 < SPACE_HNUM + 1; nCntSpace2++)
		{//1������X���̃��[�v

			//�~�`�̊p�x
			float fangleH = (360.0f / SPACE_HNUM) * nCntSpace2 * (D3DX_PI / 180.0f), fangleV = (SPACE_ANG / SPACE_VNUM) * (nCntSpace + 1) * (D3DX_PI / 180.0f);

			//���W�ݒ�
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].pos = D3DXVECTOR3(sinf(fangleH) * sinf(fangleV) * SPACE_RADIUS, cosf(fangleV) * SPACE_RADIUS, cosf(fangleH) * sinf(fangleV) * SPACE_RADIUS);

			//nor
			D3DXVECTOR3 npos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), dpos = D3DXVECTOR3(sinf(fangleH) * sinf(fangleV) * SPACE_RADIUS, cosf(fangleV) * SPACE_RADIUS, cosf(fangleH) * sinf(fangleV) * SPACE_RADIUS);
			npos = npos - dpos;
			D3DXVec3Normalize(&npos, &npos);
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].nor = npos;

			//�J���[
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.1f);

			//�e�N�X�`��
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].tex1 = D3DXVECTOR2((float)(((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI) / g_Space.Texture[0].TexSize.x) / SPACE_HNUM) * nCntSpace2), (float)((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI * 0.5f) / g_Space.Texture[0].TexSize.y) / SPACE_VNUM) * nCntSpace);

			//�e�N�X�`��
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].tex2 = D3DXVECTOR2((float)(((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI) / g_Space.Texture[1].TexSize.x) / SPACE_HNUM) * nCntSpace2), (float)((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI * 0.5f) / g_Space.Texture[1].TexSize.y) / SPACE_VNUM) * nCntSpace);
		}
	}

	g_pVtxBuffSpace->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	WORD* pIdx;
	g_pIdxBuffSpace->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	for (int nCntSpace = 1; nCntSpace < SPACE_HNUM + 2; nCntSpace++)
	{//FAN����
		pIdx[nCntSpace] = SPACE_HNUM + 2 - nCntSpace;
	}
	for (int nCntSpace = SPACE_HNUM + 2; nCntSpace < INDEX_SP_NUM - (SPACE_VNUM - 2) * 2; nCntSpace++)
	{//STRIP����
		if ((nCntSpace - (SPACE_HNUM + 2)) % (((SPACE_HNUM + 1) * 2)) == 0 && (nCntSpace - (SPACE_HNUM + 2)) != 0)
		{//�k�ރ|���S��
			//�C���f�b�N�X�w��
			pIdx[nCntSpace - 2 + (((nCntSpace - (SPACE_HNUM + 2)) / ((SPACE_HNUM + 1) * 2)) * 2)] = (SPACE_HNUM + 1) - (SPACE_HNUM + 1) * (((nCntSpace - (SPACE_HNUM + 2)) - 1) % 2) + (((nCntSpace - (SPACE_HNUM + 2)) - 1) / 2) + 1;

			//�C���f�b�N�X�w��
			pIdx[nCntSpace - 1 + (((nCntSpace - (SPACE_HNUM + 2)) / ((SPACE_HNUM + 1) * 2)) * 2)] = (SPACE_HNUM + 1) - (SPACE_HNUM + 1) * ((nCntSpace - (SPACE_HNUM + 2)) % 2) + ((nCntSpace - (SPACE_HNUM + 2)) / 2) + 1;
		}

		//�C���f�b�N�X�w��
		pIdx[nCntSpace + (((nCntSpace - (SPACE_HNUM + 2)) / ((SPACE_HNUM + 1) * 2)) * 2)] = (SPACE_HNUM + 1) - (SPACE_HNUM + 1) * ((nCntSpace - (SPACE_HNUM + 2)) % 2) + ((nCntSpace - (SPACE_HNUM + 2)) / 2) + 1;
	}

	g_pIdxBuffSpace->Unlock();
}

//-------------------
//�I������
//-------------------
void UninitSpace(void)
{
	g_Space.bUse = false;//�g�p

	for (int nCnt = 0; nCnt < SPACE_TEX_MAX; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureSpace[nCnt] != NULL)
		{
			g_pTextureSpace[nCnt]->Release();
			g_pTextureSpace[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSpace != NULL)
	{
		g_pVtxBuffSpace->Release();
		g_pVtxBuffSpace = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffSpace != NULL)
	{
		g_pIdxBuffSpace->Release();
		g_pIdxBuffSpace = NULL;
	}
}

//-------------------
//�X�V����
//-------------------
void UpdateSpace(void)
{
	VERTEX_3D_MULTI* pVtx;//���_���|�C���^
	static float nCntTex;//��]�x��
	int nCntSpace, nCntSpace2;//���_�J�E���g�p

	//���_�̃e�N�X�`������]����
	g_pVtxBuffSpace->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex1 = D3DXVECTOR2(0.0f, 0.0f);

	pVtx[0].tex2 = D3DXVECTOR2(0.0f, 0.0f);

	for (nCntSpace = 0; nCntSpace < SPACE_VNUM; nCntSpace++)
	{//1������Z���̃��[�v
		for (nCntSpace2 = 0; nCntSpace2 < SPACE_HNUM + 1; nCntSpace2++)
		{//1������X���̃��[�v
			//�e�N�X�`��
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].tex1 = D3DXVECTOR2((float)(((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI) / g_Space.Texture[0].TexSize.x) / SPACE_HNUM) * nCntSpace2) + nCntTex, (float)((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI * 0.5f) / g_Space.Texture[0].TexSize.y) / SPACE_VNUM) * nCntSpace - nCntTex);

			//�e�N�X�`��
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].tex2 = D3DXVECTOR2((float)(((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI) / g_Space.Texture[1].TexSize.x) / SPACE_HNUM) * nCntSpace2) - nCntTex - D3DX_PI, (float)((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI * 0.5f) / g_Space.Texture[1].TexSize.y) / SPACE_VNUM) * nCntSpace + nCntTex - D3DX_PI * 0.5f);
		}
	}

	g_pVtxBuffSpace->Unlock();
	nCntTex += D3DX_PI / g_Space.Texture[1].TexSize.x * 0.2f;//��]
}

//-------------------
//�`�揈��
//-------------------
void DrawSpace(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//// �T���v���[�X�e�[�g
	//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);      // �e�N�X�`���J��Ԃ�(�~���[)
	//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);      // �e�N�X�`���J��Ԃ�(�~���[)
	//pDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);      // �e�N�X�`���J��Ԃ�(�~���[)
	//pDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);      // �e�N�X�`���J��Ԃ�(�~���[)

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	// 0
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_ADD);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	// 1
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_ADD);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	if (g_Space.bUse)
	{//�g�p���Ă�����
		//�}�g���b�N�X������
		D3DXMatrixIdentity(&g_Space.mtxWorld);

		//�ʒu�̌v�Z
		extern  Camera g_camera[1];
		D3DXMatrixTranslation(&mtxTrans, g_camera[0].posV.x, g_camera[0].posV.y, g_camera[0].posV.z);
		D3DXMatrixMultiply(&g_Space.mtxWorld, &g_Space.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Space.mtxWorld);

		//���_�o�b�t�@
		pDevice->SetStreamSource(0, g_pVtxBuffSpace, 0, sizeof(VERTEX_3D_MULTI));

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffSpace);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D_MULTI);

		for (int nCnt = 0; nCnt < SPACE_TEX_MAX; nCnt++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(nCnt, g_pTextureSpace[nCnt]);
		}

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			//�Ƀ|���S���̕`��
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLEFAN,//�^�C�v
				0,
				0,
				SPACE_HNUM + 2,//���_��
				0,
				SPACE_HNUM//�|���S���̌�
			);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				0,
				0,
				VT_MAX_SP - (SPACE_HNUM + 2),//���_��
				SPACE_HNUM + 2,
				POLYGON_SP_NUM - SPACE_HNUM//�|���S���̌�
			);
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
	pDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);      // �e�N�X�`���J��Ԃ�(�^�C��)
	pDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);      // �e�N�X�`���J��Ԃ�(�^�C��)

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �A���t�@�e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z�e�X�g��߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//----------
//�ݒ菈��
//----------
void SetSpace(void)
{
	if (!g_Space.bUse)
	{//�g�p���Ă��Ȃ�
		//�L����
		g_Space.bUse = true;//�g�p
	}
}