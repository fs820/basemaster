//---------------------------------------
//
//�e�\������[explosion.cpp]
//Author fuma sato
//
//---------------------------------------

#include"explosion.h"

#define MAX_EXPLOSION (256)
#define EXPLOSION_WIDTH (50.0f)
#define EXPLOSION_HEIGHT (50.0f)

//�e�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXCOLOR col;
	int nCntAnim;//�ړ���
	int nPatternAnim;//����
	D3DXVECTOR3 scale;
	bool bUse;//�g�p���Ă��邩�ǂ���
	D3DXMATRIX mtxWorld;
}Explosion;

//�O���[�o��
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;//�o�b�t�@�̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];//�e�̏��

//-----------------
//�e�̏���������
//-----------------
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_3D* pVtx;

	//�f�o�C�X���擾
	pDevice = GetDevice();

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		g_aExplosion[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[i].nCntAnim = 0;
		g_aExplosion[i].nPatternAnim = 0;
		g_aExplosion[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aExplosion[i].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VT_MAX * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		EXPLOSION_TEX,
		&g_pTextureExplosion
	);

	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH * 0.5f, g_aExplosion[i].pos.y + EXPLOSION_HEIGHT * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH * 0.5f, g_aExplosion[i].pos.y + EXPLOSION_HEIGHT * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH * 0.5f, g_aExplosion[i].pos.y - EXPLOSION_HEIGHT * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH * 0.5f, g_aExplosion[i].pos.y - EXPLOSION_HEIGHT * 0.5f, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim + UV_DEF / U_MAX_E, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E) + UV_DEF / V_MAX_E);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim + UV_DEF / U_MAX_E, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E) + UV_DEF / V_MAX_E);

		pVtx += VT_MAX;//�|���S��1���i�߂�
	}
	g_pVtxBuffExplosion->Unlock();//�o�b�t�@�̃A�����b�N
}

//----------------
//�e�̏I������
//----------------
void UninitExplosion(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//----------------
//�e�̍X�V����
//----------------
void UpdateExplosion(void)
{
	VERTEX_3D* pVtx;

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (g_aExplosion[i].bUse == true)
		{//�e���g�p����Ă���

			g_aExplosion[i].nCntAnim++;
			if ((g_aExplosion[i].nCntAnim % 5) == 0)//�X�V����
			{
				g_aExplosion[i].nCntAnim = (g_aExplosion[i].nCntAnim + 1) % (U_MAX_E * V_MAX_E);

				g_aExplosion[i].nPatternAnim++;
				if (g_aExplosion[i].nPatternAnim >= (U_MAX_E * V_MAX_E))
				{
					g_aExplosion[i].bUse = false;
				}

				g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N
				pVtx += i * VT_MAX;

				//�e�N�X�`��
				pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E));
				pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim + UV_DEF / U_MAX_E, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E));
				pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E) + UV_DEF / V_MAX_E);
				pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim + UV_DEF / U_MAX_E, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E) + UV_DEF / V_MAX_E);

				g_pVtxBuffExplosion->Unlock();//�o�b�t�@�̃A�����b�N
			}
		}
	}
}

//----------------
//�e�̕`�揈��
//----------------
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	D3DXMATRIX mtxScale, mtxTrans;//�v�Z�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	int nCntExplosion;
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse)
		{
			//�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aExplosion[nCntExplosion].mtxWorld);

			//�r���[�}�g���b�N�X�擾
			D3DXMATRIX mtxView;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			g_aExplosion[nCntExplosion].mtxWorld._11 = mtxView._11;
			g_aExplosion[nCntExplosion].mtxWorld._12 = mtxView._21;
			g_aExplosion[nCntExplosion].mtxWorld._13 = mtxView._31;
			g_aExplosion[nCntExplosion].mtxWorld._21 = mtxView._12;
			g_aExplosion[nCntExplosion].mtxWorld._22 = mtxView._22;
			g_aExplosion[nCntExplosion].mtxWorld._23 = mtxView._32;
			g_aExplosion[nCntExplosion].mtxWorld._31 = mtxView._13;
			g_aExplosion[nCntExplosion].mtxWorld._32 = mtxView._23;
			g_aExplosion[nCntExplosion].mtxWorld._33 = mtxView._33;

			//�傫���̔��f
			D3DXMatrixScaling(&mtxScale, g_aExplosion[nCntExplosion].scale.x, g_aExplosion[nCntExplosion].scale.y, g_aExplosion[nCntExplosion].scale.z);
			D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxScale);

			//�ʒu�̌v�Z
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntExplosion].mtxWorld);

			//���_�o�b�t�@
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureExplosion);

			//�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				nCntExplosion * VT_MAX,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}

	//���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//-------------------
//�����ݒ�
//-------------------
void SetExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (g_aExplosion[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aExplosion[i].pos = pos;
			g_aExplosion[i].scale = scale;
			g_aExplosion[i].nCntAnim = 0;
			g_aExplosion[i].nPatternAnim = 0;
			g_aExplosion[i].bUse = true;
			break;
		}
	}
}