//----------------------------------------
//
//�|���S���\������[billboard.cpp]
//Author fuma sato
//
//----------------------------------------

#include"billboard.h"
#include"camera.h"
#include"shadow.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;//�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;
Billboard g_aBillboard[BILLBOARD_MAX];

//----------------------
//�|���S���̏���������
//----------------------
void InitBillboard(void)
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
		&g_pVtxBuffBillboard,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		BILLBOARD_TEX,
		&g_pTextureBillboard
	);

	int nCntBillboard;
	for (nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		g_aBillboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBillboard[nCntBillboard].bUse = false;
		g_aBillboard[nCntBillboard].nIdxShadow = -1;
	}
	//1��


	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_WIDTH * 0.5f, BILLBOARD_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BILLBOARD_WIDTH * 0.5f, BILLBOARD_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_WIDTH * 0.5f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BILLBOARD_WIDTH * 0.5f, 0.0f, 0.0f);

	//nor
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

	g_pVtxBuffBillboard->Unlock();
}

//-------------------
//�|���S���I������
//-------------------
void UninitBillboard(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//-------------------
//�|���S���X�V����
//-------------------
void UpdateBillboard(void)
{
	int nCntBillboard;
	for (nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse)
		{
			SetPositionShadow(g_aBillboard[nCntBillboard].nIdxShadow, g_aBillboard[nCntBillboard].pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), BILLBOARD_HEIGHT);
		}
	}
}

//-------------------
//�|���S���`�揈��
//-------------------
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	D3DXMATRIX mtxScale, mtxTrans;//�v�Z�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�e�X�g�I�t
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	int nCntBillboard;
	for (nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse)
		{
			//�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aBillboard[nCntBillboard].mtxWorld);

			//�r���[�}�g���b�N�X�擾
			D3DXMATRIX mtxView;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			g_aBillboard[nCntBillboard].mtxWorld._11 = mtxView._11;
			//g_aBillboard[nCntBillboard].mtxWorld._12 = mtxView._21;
			g_aBillboard[nCntBillboard].mtxWorld._13 = mtxView._31;
			//g_aBillboard[nCntBillboard].mtxWorld._21 = mtxView._12;
			//g_aBillboard[nCntBillboard].mtxWorld._22 = mtxView._22;
			//g_aBillboard[nCntBillboard].mtxWorld._23 = mtxView._32;
			g_aBillboard[nCntBillboard].mtxWorld._31 = mtxView._13;
			//g_aBillboard[nCntBillboard].mtxWorld._32 = mtxView._23;
			g_aBillboard[nCntBillboard].mtxWorld._33 = mtxView._33;

			//�傫���̔��f
			D3DXMatrixScaling(&mtxScale, g_aBillboard[nCntBillboard].scale.x, g_aBillboard[nCntBillboard].scale.y, g_aBillboard[nCntBillboard].scale.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxScale);

			//�ʒu�̌v�Z
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBillboard].pos.x, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBillboard].mtxWorld);

			//���_�o�b�t�@
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBillboard);

			//�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				0,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}

	//Z�e�X�g�I��
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//----------
//�ݒ�
//----------
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	int nCntBillboard;
	for (nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		if (!g_aBillboard[nCntBillboard].bUse)
		{
			g_aBillboard[nCntBillboard].pos = pos;
			g_aBillboard[nCntBillboard].scale = scale;
			g_aBillboard[nCntBillboard].bUse = true;
			g_aBillboard[nCntBillboard].nIdxShadow = SetShadow(g_aBillboard[nCntBillboard].pos);
			break;
		}
	}
}