//----------------------------------------
//
//�|���S���\������[cylinder.cpp]
//Author fuma sato
//
//----------------------------------------

#include"cylinder.h"
#include"effect.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCylinder = NULL;//�o�b�t�@�̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffCylinder = NULL;//�C���f�b�N�X�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureCylinder = NULL;
Cylinder g_aCylinder[CYLINDER_MAX];

//----------------------
//�|���S���̏���������
//----------------------
void InitCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_3D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VT_MAX_CYLI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCylinder,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		CYLINDER_TEX,
		&g_pTextureCylinder
	);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * INDEX_CYLI_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffCylinder,
		NULL
	);

	int nCntCylinder, nCntCylinder2;
	for (nCntCylinder = 0; nCntCylinder < CYLINDER_MAX; nCntCylinder++)
	{
		g_aCylinder[nCntCylinder].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCylinder[nCntCylinder].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCylinder[nCntCylinder].bAlpha = false;
		g_aCylinder[nCntCylinder].bUse = false;
	}
	//1��


	g_pVtxBuffCylinder->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntCylinder = 0; nCntCylinder < CYLINDER_VNUM + 1; nCntCylinder++)
	{//1������Z���̃��[�v
		for (nCntCylinder2 = 0; nCntCylinder2 < CYLINDER_HNUM + 1; nCntCylinder2++)
		{//1������X���̃��[�v
			//���W�ݒ�
			float fangle = (360.0f / CYLINDER_HNUM) * nCntCylinder2 * (D3DX_PI / 180.0f);
			pVtx[nCntCylinder * (CYLINDER_HNUM + 1) + nCntCylinder2].pos = D3DXVECTOR3(sinf(fangle) * CYLINDER_RADIUS, CYLINDER_HEIGHT - (CYLINDER_HEIGHT / CYLINDER_VNUM) * nCntCylinder, cosf(fangle) * CYLINDER_RADIUS);

			//nor
			D3DXVECTOR3 npos = D3DXVECTOR3(0.0f, CYLINDER_HEIGHT - (CYLINDER_HEIGHT / CYLINDER_VNUM) * nCntCylinder, 0.0f), dpos = D3DXVECTOR3(sinf(fangle) * CYLINDER_RADIUS, CYLINDER_HEIGHT - (CYLINDER_HEIGHT / CYLINDER_VNUM) * nCntCylinder, cosf(fangle) * CYLINDER_RADIUS);
			npos = npos - dpos;
			D3DXVec3Normalize(&npos, &npos);
			pVtx[nCntCylinder * (CYLINDER_HNUM + 1) + nCntCylinder2].nor = npos;

			//�J���[
			pVtx[nCntCylinder * (CYLINDER_HNUM + 1) + nCntCylinder2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`��
			pVtx[nCntCylinder * (CYLINDER_HNUM + 1) + nCntCylinder2].tex = D3DXVECTOR2(((((CYLINDER_RADIUS + CYLINDER_RADIUS) * D3DX_PI) / CYLINDER_WIDTH_DEF) / CYLINDER_HNUM) * nCntCylinder2, ((CYLINDER_HEIGHT / CYLINDER_HEIGHT_DEF) / CYLINDER_VNUM) * nCntCylinder);
		}
	}

	g_pVtxBuffCylinder->Unlock();

	WORD* pIdx;
	g_pIdxBuffCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntCylinder = 0; nCntCylinder < INDEX_CYLI_NUM - (CYLINDER_VNUM - 1) * 2; nCntCylinder++)
	{
		if (nCntCylinder % ((CYLINDER_HNUM + 1) * 2) == 0 && nCntCylinder != 0)
		{
			//�C���f�b�N�X�w��
			pIdx[nCntCylinder - 2 + ((nCntCylinder / ((CYLINDER_HNUM + 1) * 2)) * 2)] = (CYLINDER_HNUM + 1) - (CYLINDER_HNUM + 1) * ((nCntCylinder - 1) % 2) + ((nCntCylinder - 1) / 2);
			//�C���f�b�N�X�w��
			pIdx[nCntCylinder - 1 + ((nCntCylinder / ((CYLINDER_HNUM + 1) * 2)) * 2)] = (CYLINDER_HNUM + 1) - (CYLINDER_HNUM + 1) * (nCntCylinder % 2) + (nCntCylinder / 2);
		}

		//�C���f�b�N�X�w��
		pIdx[nCntCylinder + ((nCntCylinder / ((CYLINDER_HNUM + 1) * 2)) * 2)] = (CYLINDER_HNUM + 1) - (CYLINDER_HNUM + 1) * (nCntCylinder % 2) + (nCntCylinder / 2);
	}

	g_pIdxBuffCylinder->Unlock();
}

//-------------------
//�|���S���I������
//-------------------
void UninitCylinder(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureCylinder != NULL)
	{
		g_pTextureCylinder->Release();
		g_pTextureCylinder = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCylinder != NULL)
	{
		g_pVtxBuffCylinder->Release();
		g_pVtxBuffCylinder = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffCylinder != NULL)
	{
		g_pIdxBuffCylinder->Release();
		g_pIdxBuffCylinder = NULL;
	}
}

//-------------------
//�|���S���X�V����
//-------------------
void UpdateCylinder(void)
{

}

//-------------------
//�|���S���`�揈��
//-------------------
void DrawCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	int nCntCylinder;
	for (nCntCylinder = 0; nCntCylinder < CYLINDER_MAX; nCntCylinder++)
	{
		if (g_aCylinder[nCntCylinder].bUse && !g_aCylinder[nCntCylinder].bAlpha)
		{
			//�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aCylinder[nCntCylinder].mtxWorld);

			//�����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCylinder[nCntCylinder].rot.y, g_aCylinder[nCntCylinder].rot.x, g_aCylinder[nCntCylinder].rot.z);
			D3DXMatrixMultiply(&g_aCylinder[nCntCylinder].mtxWorld, &g_aCylinder[nCntCylinder].mtxWorld, &mtxRot);

			//�ʒu�̌v�Z
			D3DXMatrixTranslation(&mtxTrans, g_aCylinder[nCntCylinder].pos.x, g_aCylinder[nCntCylinder].pos.y, g_aCylinder[nCntCylinder].pos.z);
			D3DXMatrixMultiply(&g_aCylinder[nCntCylinder].mtxWorld, &g_aCylinder[nCntCylinder].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aCylinder[nCntCylinder].mtxWorld);

			//���_�o�b�t�@
			pDevice->SetStreamSource(0, g_pVtxBuffCylinder, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffCylinder);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureCylinder);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				0,
				0,
				VT_MAX_CYLI,//���_��
				0,
				POLYGON_CYLI_NUM//�|���S���̌�
			);
		}
	}
}

//----------
//�ݒ�
//----------
void SetCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntCylinder;
	for (nCntCylinder = 0; nCntCylinder < CYLINDER_MAX; nCntCylinder++)
	{
		if (!g_aCylinder[nCntCylinder].bUse)
		{
			g_aCylinder[nCntCylinder].pos = pos;
			g_aCylinder[nCntCylinder].rot = rot;
			g_aCylinder[nCntCylinder].bAlpha = false;
			g_aCylinder[nCntCylinder].bUse = true;
			break;
		}
	}
}