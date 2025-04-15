//----------------------------------------
//
//�|���S���\������[shadow.cpp]
//Author fuma sato
//
//----------------------------------------

#include"shadow.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;//�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;
Shadow g_aShadow[SHADOW_MAX];

//----------------------
//�|���S���̏���������
//----------------------
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_3D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		SHADOW_TEX,
		&g_pTextureShadow
	);

	int nCntShadow;
	for (nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.01f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aShadow[nCntShadow].bUse = false;
	}

	//1��

	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(-0.5f, 0.0f, 0.5f);
	pVtx[1].pos = D3DXVECTOR3(0.5f, 0.0f, 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-0.5f, 0.0f, -0.5f);
	pVtx[3].pos = D3DXVECTOR3(0.5f, 0.0f, -0.5f);

	//nor
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`��
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffShadow->Unlock();
}

//-------------------
//�|���S���I������
//-------------------
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//-------------------
//�|���S���X�V����
//-------------------
void UpdateShadow(void)
{

}

//-------------------
//�|���S���`�揈��
//-------------------
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;//�v�Z�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�A���t�@�e�X�g�I��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//Z�e�X�g�I�t
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	int nCntShadow;
	for (nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse)
		{
			//�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			//�傫���̔��f
			D3DXMatrixScaling(&mtxScale, g_aShadow[nCntShadow].scale.x, g_aShadow[nCntShadow].scale.y, g_aShadow[nCntShadow].scale.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxScale);

			//�����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			//�ʒu�̌v�Z
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			//���_�o�b�t�@
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				0,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//Z�e�X�g�I��
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//�A���t�@�e�X�g�I��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------------
//�ݒ�
//--------------------------------------------
int SetShadow(D3DXVECTOR3 pos)
{
	int nCntShadow;
	for (nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		if (!g_aShadow[nCntShadow].bUse)
		{
			g_aShadow[nCntShadow].pos.x = pos.x;
			g_aShadow[nCntShadow].pos.z = pos.z;
			g_aShadow[nCntShadow].bUse = true;
			break;
		}
	}
	return nCntShadow;
}

//------------------
//�ʒu�̍X�V
//------------------
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fSize)
{
	g_aShadow[nIdxShadow].pos.x = pos.x;
	g_aShadow[nIdxShadow].pos.z = pos.z;
	g_aShadow[nIdxShadow].scale = scale * fSize * pos.y;
}

//------------------------------
//�e������
//------------------------------
void NullShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}