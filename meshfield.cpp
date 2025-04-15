//----------------------------------------
//
//�|���S���\������[meshfield.cpp]
//Author fuma sato
//
//----------------------------------------

#include"meshfield.h"
#include"player.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;//�o�b�t�@�̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;//�C���f�b�N�X�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureMeshField[MESHFIELD_TEX_MAX] = { NULL };
MeshField g_aMeshField[MESHFIELD_MAX];

//----------------------
//�|���S���̏���������
//----------------------
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_3D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VT_MAX_MESH,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		MESHFIELD_TEX,
		&g_pTextureMeshField[0]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		MESHFIELD_TEX2,
		&g_pTextureMeshField[1]
	);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL
	);

	int nCntMeshField, nCntMeshField2;
	for (nCntMeshField = 0; nCntMeshField < MESHFIELD_MAX; nCntMeshField++)
	{
		g_aMeshField[nCntMeshField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshField[nCntMeshField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshField[nCntMeshField].bAlpha = false;
		g_aMeshField[nCntMeshField].bUse = false;
	}
	//1��


	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshField = 0; nCntMeshField < MESHFIELD_ZNUM + 1; nCntMeshField++)
	{//1������Z���̃��[�v
		for (nCntMeshField2 = 0; nCntMeshField2 < MESHFIELD_XNUM + 1; nCntMeshField2++)
		{//1������X���̃��[�v
			//���W�ݒ�
			pVtx[nCntMeshField * (MESHFIELD_XNUM + 1) + nCntMeshField2].pos = D3DXVECTOR3(-MESHFIELD_WIDTH * 0.5f + (MESHFIELD_WIDTH / MESHFIELD_XNUM) * nCntMeshField2, MESHFIELD_HEIGHT, MESHFIELD_Z * 0.5f - (MESHFIELD_Z / MESHFIELD_ZNUM) * nCntMeshField);

			//nor
			pVtx[nCntMeshField * (MESHFIELD_XNUM + 1) + nCntMeshField2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//�J���[
			pVtx[nCntMeshField * (MESHFIELD_XNUM + 1) + nCntMeshField2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`��
			pVtx[nCntMeshField * (MESHFIELD_XNUM + 1) + nCntMeshField2].tex = D3DXVECTOR2(((MESHFIELD_WIDTH / MESHFIELD_WIDTH_DEF) / MESHFIELD_XNUM) * nCntMeshField2, ((MESHFIELD_Z / MESHFIELD_Z_DEF) / MESHFIELD_ZNUM) * nCntMeshField);
		}
	}

	g_pVtxBuffMeshField->Unlock();

	WORD* pIdx;
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshField = 0; nCntMeshField < INDEX_NUM - (MESHFIELD_ZNUM - 1) * 2; nCntMeshField++)
	{
		if (nCntMeshField % ((MESHFIELD_XNUM + 1) * 2) == 0 && nCntMeshField != 0)
		{
			//�C���f�b�N�X�w��
			pIdx[nCntMeshField - 2 + ((nCntMeshField / ((MESHFIELD_XNUM + 1) * 2)) * 2)] = (MESHFIELD_XNUM + 1) - (MESHFIELD_XNUM + 1) * ((nCntMeshField - 1) % 2) + ((nCntMeshField - 1) / 2);
			//�C���f�b�N�X�w��
			pIdx[nCntMeshField - 1 + ((nCntMeshField / ((MESHFIELD_XNUM + 1) * 2)) * 2)] = (MESHFIELD_XNUM + 1) - (MESHFIELD_XNUM + 1) * (nCntMeshField % 2) + (nCntMeshField / 2);
		}

		//�C���f�b�N�X�w��
		pIdx[nCntMeshField + ((nCntMeshField / ((MESHFIELD_XNUM + 1) * 2)) * 2)] = (MESHFIELD_XNUM + 1) - (MESHFIELD_XNUM + 1) * (nCntMeshField % 2) + (nCntMeshField / 2);
	}

	g_pIdxBuffMeshField->Unlock();
}

//-------------------
//�|���S���I������
//-------------------
void UninitMeshField(void)
{
	for (int nCnt = 0; nCnt < MESHFIELD_TEX_MAX; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureMeshField[nCnt] != NULL)
		{
			g_pTextureMeshField[nCnt]->Release();
			g_pTextureMeshField[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//-------------------
//�|���S���X�V����
//-------------------
void UpdateMeshField(void)
{

}

//-------------------
//�|���S���`�揈��
//-------------------
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	int nCntMeshField;
	for (nCntMeshField = 0; nCntMeshField < MESHFIELD_MAX; nCntMeshField++)
	{
		if (g_aMeshField[nCntMeshField].bUse && !g_aMeshField[nCntMeshField].bAlpha)
		{
			//�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aMeshField[nCntMeshField].mtxWorld);

			//�����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField[nCntMeshField].rot.y, g_aMeshField[nCntMeshField].rot.x, g_aMeshField[nCntMeshField].rot.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntMeshField].mtxWorld, &g_aMeshField[nCntMeshField].mtxWorld, &mtxRot);

			//�ʒu�̌v�Z
			D3DXMatrixTranslation(&mtxTrans, g_aMeshField[nCntMeshField].pos.x, g_aMeshField[nCntMeshField].pos.y, g_aMeshField[nCntMeshField].pos.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntMeshField].mtxWorld, &g_aMeshField[nCntMeshField].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField[nCntMeshField].mtxWorld);

			//���_�o�b�t�@
			pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshField);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMeshField[GetbSnow()]);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				0,
				0,
				VT_MAX_MESH,//���_��
				0,
				POLYGON_NUM//�|���S���̌�
			);
		}
	}
}

//----------
//�ݒ�
//----------
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntMeshField;
	for (nCntMeshField = 0; nCntMeshField < MESHFIELD_MAX; nCntMeshField++)
	{
		if (!g_aMeshField[nCntMeshField].bUse)
		{
			g_aMeshField[nCntMeshField].pos = pos;
			g_aMeshField[nCntMeshField].rot = rot;
			g_aMeshField[nCntMeshField].bAlpha = false;
			g_aMeshField[nCntMeshField].bUse = true;
			break;
		}
	}
}

//---------------------------
//�擾
//---------------------------
MeshField* GetMeshField(void)
{
	return &g_aMeshField[0];
}

//----------------------
//�����蔻��
//----------------------
bool CollisionMeshField(D3DXVECTOR3& pos, D3DXVECTOR3& posOld, D3DXVECTOR3& move,float fWidth)
{
	D3DXVECTOR3 aPos[2] = {}, Fieldvec = {}, Posvec = {}, PosOldvec = {}, movevec = {}, Norvec = {}, Dovec = {}, Hit = {};
	int nCntMeshField;
	float FieldCross, PosCross;
	for (nCntMeshField = 0; nCntMeshField < MESHFIELD_MAX; nCntMeshField++)
	{//�n�ʂ̐�
		if (g_aMeshField[nCntMeshField].bUse && !g_aMeshField[nCntMeshField].bAlpha)
		{//�g���Ă��铧���łȂ���
			aPos[0] = D3DXVECTOR3(g_aMeshField[nCntMeshField].pos.x + MESHFIELD_WIDTH * 0.5f * sinf(g_aMeshField[nCntMeshField].rot.z + D3DX_PI * -0.5f), g_aMeshField[nCntMeshField].pos.y + MESHFIELD_WIDTH * 0.5f * cosf(g_aMeshField[nCntMeshField].rot.z + D3DX_PI * 0.5f), g_aMeshField[nCntMeshField].pos.z);
			aPos[1] = D3DXVECTOR3(g_aMeshField[nCntMeshField].pos.x + MESHFIELD_WIDTH * 0.5f * sinf(g_aMeshField[nCntMeshField].rot.z + D3DX_PI * 0.5f), g_aMeshField[nCntMeshField].pos.y + MESHFIELD_WIDTH * 0.5f * cosf(g_aMeshField[nCntMeshField].rot.z + D3DX_PI * -0.5f), g_aMeshField[nCntMeshField].pos.z);

			Fieldvec = aPos[1] - aPos[0];//�ǂ̃x�N�g��
			Posvec = pos - aPos[0];//�ǂɑ΂���v���C���[�̃x�N�g��
			PosOldvec = posOld - aPos[0];//�ǂɑ΂���v���C���[�̋��x�N�g��
			movevec = pos - posOld;//�v���C���[�̈ړ��x�N�g��
			if ((Fieldvec.x * Posvec.y) - (Fieldvec.y * Posvec.x) <= 0.0f && (Fieldvec.x * PosOldvec.y) - (Fieldvec.y * PosOldvec.x) >= 0.0f && pos.z - fWidth < g_aMeshField[nCntMeshField].pos.z + MESHFIELD_WIDTH * 0.5f && pos.z + fWidth > g_aMeshField[nCntMeshField].pos.z - MESHFIELD_WIDTH * 0.5f)
			{//�n�ʂ̉�
				FieldCross = (Fieldvec.x * movevec.y) - (Fieldvec.y * movevec.x);
				PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
				PosCross /= FieldCross;
				if (PosCross >= -0.01f && PosCross < 1.01f)
				{
					Hit = aPos[0] + Fieldvec * PosCross;
					movevec = pos - Hit;//�v���C���[�̈ړ��x�N�g��
					Norvec = D3DXVECTOR3(Fieldvec.y, -Fieldvec.x, Fieldvec.z);
					D3DXVec3Normalize(&Norvec, &Norvec);
					Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
					pos += Dovec * 1.001f;
					move.y = 0.0f;
					return true;
				}
			}
		}
	}

	return false;
}