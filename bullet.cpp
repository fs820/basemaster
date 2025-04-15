//----------------------------------------
//
//�|���S���\������[bullet.cpp]
//Author fuma sato
//
//----------------------------------------

#include"bullet.h"
#include"camera.h"
#include"shadow.h"
#include"explosion.h"
#include"effect.h"
#include"particle.h"
#include"meshfield.h"
#include"sound.h"
#include"hitshere.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;
Bullet g_aBullet[BULLET_MAX];

//----------------------
//�|���S���̏���������
//----------------------
void InitBullet(void)
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
		&g_pVtxBuffBullet,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		BULLET_TEX,
		&g_pTextureBullet
	);

	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].nIdxShadow = -1;
	}
	//1��


	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	//���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(-BULLET_WIDTH * 0.5f, BULLET_HEIGHT * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BULLET_WIDTH * 0.5f, BULLET_HEIGHT * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-BULLET_WIDTH * 0.5f, -BULLET_HEIGHT * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BULLET_WIDTH * 0.5f, -BULLET_HEIGHT * 0.5f, 0.0f);

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

	g_pVtxBuffBullet->Unlock();
}

//-------------------
//�|���S���I������
//-------------------
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//-------------------
//�|���S���X�V����
//-------------------
void UpdateBullet(void)
{
	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{
			g_aBullet[nCntBullet].nLife--;

			D3DXVec3Normalize(&g_aBullet[nCntBullet].dir, &g_aBullet[nCntBullet].dir);

			g_aBullet[nCntBullet].move.x = g_aBullet[nCntBullet].dir.x * BULLET_SPEED;
			g_aBullet[nCntBullet].move.y = g_aBullet[nCntBullet].dir.y * BULLET_SPEED;
			g_aBullet[nCntBullet].move.z = g_aBullet[nCntBullet].dir.z * BULLET_SPEED;

			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.1f, 0.5f, 0.4f, 0.1f), g_aBullet[nCntBullet].scale, 1000, EFFECT_TYPE_NORMAL);

			if (g_aBullet[nCntBullet].nLife < 0)
			{
				g_aBullet[nCntBullet].bUse = false;
				NullShadow(g_aBullet[nCntBullet].nIdxShadow);
				SetParticle(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].scale);
			}
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].scale, BULLET_HEIGHT);

			SetHitShere(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].scale * BULLET_WIDTH * 0.5f*2.0f, 2, PLAYER);
		}
	}
}

//-------------------
//�|���S���`�揈��
//-------------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	D3DXMATRIX mtxScale, mtxTrans;//�v�Z�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//�A���t�@�e�X�g�I��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{
			//�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			//�r���[�}�g���b�N�X�擾
			D3DXMATRIX mtxView;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			g_aBullet[nCntBullet].mtxWorld._11 = mtxView._11;
			g_aBullet[nCntBullet].mtxWorld._12 = mtxView._21;
			g_aBullet[nCntBullet].mtxWorld._13 = mtxView._31;
			g_aBullet[nCntBullet].mtxWorld._21 = mtxView._12;
			g_aBullet[nCntBullet].mtxWorld._22 = mtxView._22;
			g_aBullet[nCntBullet].mtxWorld._23 = mtxView._32;
			g_aBullet[nCntBullet].mtxWorld._31 = mtxView._13;
			g_aBullet[nCntBullet].mtxWorld._32 = mtxView._23;
			g_aBullet[nCntBullet].mtxWorld._33 = mtxView._33;

			//�傫���̔��f
			D3DXMatrixScaling(&mtxScale, g_aBullet[nCntBullet].scale.x, g_aBullet[nCntBullet].scale.y, g_aBullet[nCntBullet].scale.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxScale);

			//�ʒu�̌v�Z
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			//���_�o�b�t�@
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);

			//�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				0,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}

	//�A���t�@�e�X�g�I�t
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//----------
//�ݒ�
//----------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 dir, D3DXVECTOR3 scale)
{
	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (!g_aBullet[nCntBullet].bUse)
		{
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aBullet[nCntBullet].dir = dir;
			g_aBullet[nCntBullet].scale = scale;
			g_aBullet[nCntBullet].nLife = BULEET_LIFE;
			g_aBullet[nCntBullet].bUse = true;
			g_aBullet[nCntBullet].nIdxShadow = SetShadow(g_aBullet[nCntBullet].pos);
			break;
		}
	}
}