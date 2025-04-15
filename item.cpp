//----------------------------------------
//
// �I�u�W�F�N�g����[Item.cpp]
// Author fuma sato
//
//----------------------------------------
#include"Item.h"
#include"shadow.h"
#include"player.h"
#include"camera.h"
#include"meshfield.h"
#include"input.h"

// �O���[�o���ϐ��錾
Item g_aItem[ITEM_MAX] = {};								// �I�u�W�F�N�g����
ItemManager g_ItemManager;									// �t�@�C������
ITYPE g_Type;												// �I�u�W�F�N�g�ԍ��Ǘ�
int g_DelItem;												// �I�u�W�F�N�g�����p
//----------------------
// ����������
//----------------------
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�փ|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{// �ő吔�J��Ԃ�

		// �I�u�W�F�N�g���̏�����
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu������������
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����������������
		g_aItem[nCntItem].fLength = 0.0f;							// ����������������
		g_aItem[nCntItem].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �傫��������������
		g_aItem[nCntItem].nIdxShadow = -1;						    // �e�̔ԍ�������������
		g_aItem[nCntItem].Type = ITYPE_TEST;							// �^�C�v���ʎq������������
		g_aItem[nCntItem].bAlpha = false;							// �������ǂ���������������
		g_aItem[nCntItem].bUse = false;								// �g�p���Ă��邩�ǂ���������������
		g_aItem[nCntItem].nRadarIdx = -1;							// ���[�_�[�C���f�b�N�X
	}

	// �t�@�C�����̏�����
	g_ItemManager.ItemNum = 0;
	for (int nCntTex = 0; nCntTex < ITEM_TEXTURE_MAX; nCntTex++)
	{//�e�N�X�`�����J��Ԃ�
		g_ItemManager.apTexture[nCntTex] = NULL;
	}
	for (int nCntModel = 0; nCntModel < ITYPE_MAX; nCntModel++)
	{
		*g_ItemManager.ItemModel[nCntModel].Pass.aName = '\0';						// �t�@�C���p�X������������
		g_ItemManager.ItemModel[nCntModel].pMesh = NULL;							// ���b�V��������������
		g_ItemManager.ItemModel[nCntModel].pBuffMat = NULL;							// �}�e���A��������������
		g_ItemManager.ItemModel[nCntModel].dwNumMat = 0;							// �}�e���A����������������
		for (int nCntMat = 0; nCntMat < ITEM_SUBSET_MAX; nCntMat++)
		{//�}�e���A�����J��Ԃ�
			g_ItemManager.ItemModel[nCntModel].TexIdx[nCntMat] = -1;
		}
		g_ItemManager.ItemModel[nCntModel].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ŏ��l������������
		g_ItemManager.ItemModel[nCntModel].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ő�l������������
		g_ItemManager.ItemModel[nCntModel].nNumVtx = 0;								// ���_��������������
		g_ItemManager.ItemModel[nCntModel].sizeFVF = 0;								// �T�C�Y������������
		g_ItemManager.ItemModel[nCntModel].pVtxBuff = NULL;							// �o�b�t�@�̃|�C���^������������
	}

	//�I�u�W�F�N�g�t�@�C�����擾
	LoadItemManager();

	//�ԍ��̏�����
	g_Type = ITYPE_TEST;
	g_DelItem = -1;
}

//-------------------
// �I������
//-------------------
void UninitItem(void)
{
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{// �ő吔�J��Ԃ�
		// �I�u�W�F�N�g���̏�����
		g_aItem[nCntItem].bUse = false;								// �g�p���Ă��邩�ǂ���������������
	}

	for (int nCount = 0; nCount < ITYPE_MAX; nCount++)
	{// UI�̍ő吔���J��Ԃ�
		g_itemUI[nCount].bUse = false;																										// �A�C�e�����������Ă��邩������������
	}

	// UI�e�N�X�`���̏���
	for (int nCount = 0; nCount < ITEMUITYPE_MAX; nCount++)
	{// �^�C�v���� tetsuji
		// �e�N�X�`���̔j��
		if (g_pTextureitemUI[nCount] != NULL)
		{
			g_pTextureitemUI[nCount]->Release();
			g_pTextureitemUI[nCount] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffitemUI != NULL)
	{
		g_pVtxBuffitemUI->Release();
		g_pVtxBuffitemUI = NULL;
	}

	for (int nCntTex = 0; nCntTex < ITEM_TEXTURE_MAX; nCntTex++)
	{// �e�N�X�`�����J��Ԃ�
		// �e�N�X�`���̔j��
		if (g_ItemManager.apTexture[nCntTex] != NULL)
		{
			g_ItemManager.apTexture[nCntTex]->Release();
			g_ItemManager.apTexture[nCntTex] = NULL;
		}
	}
	for (int nCntModel = 0; nCntModel < ITYPE_MAX; nCntModel++)
	{// �ő吔�J��Ԃ�
				//���_�o�b�t�@�̔j��
		if (g_ItemManager.ItemModel[nCntModel].pVtxBuff != NULL)
		{
			g_ItemManager.ItemModel[nCntModel].pVtxBuff = NULL;
		}
		// ���b�V���̔j��
		if (g_ItemManager.ItemModel[nCntModel].pMesh != NULL)
		{
			g_ItemManager.ItemModel[nCntModel].pMesh->Release();
			g_ItemManager.ItemModel[nCntModel].pMesh = NULL;
		}
		// �}�e���A���̔j��
		if (g_ItemManager.ItemModel[nCntModel].pBuffMat != NULL)
		{
			g_ItemManager.ItemModel[nCntModel].pBuffMat->Release();
			g_ItemManager.ItemModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//-------------------
// �X�V����
//-------------------
void UpdateItem(void)
{
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{//�ő吔�J��Ԃ�
		if (g_aItem[nCntItem].bUse == true)
		{// �A�C�e�����g�p���Ă��鎞
			SetPositionShadow(g_aItem[nCntItem].nIdxShadow, g_aItem[nCntItem].pos, g_aItem[nCntItem].scale, g_aItem[nCntItem].fLength);
		}
	}
}

//-------------------
// �`�揈��
//-------------------
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�փ|�C���^
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// �v�Z�}�g���b�N�X
	D3DMATERIAL9 matDef;					// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{//�ő吔�J��Ԃ�
		if (g_aItem[nCntItem].bUse == true)
		{// �A�C�e�����g�p���Ă��鎞
			// �}�g���b�N�X������
			D3DXMatrixIdentity(&g_aItem[nCntItem].mtxWorld);

			// �傫���̔��f
			D3DXMatrixScaling(&mtxScale, g_aItem[nCntItem].scale.x, g_aItem[nCntItem].scale.y, g_aItem[nCntItem].scale.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxScale);

			// �����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxRot);

			// �ʒu�̔��f
			D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].mtxWorld);
			
			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].pBuffMat->GetBufferPointer();

			// �e�T�u�Z�b�g�̃t�F�C�X�����擾
			DWORD numSubsets = 0;
			g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].pMesh->GetAttributeTable(nullptr, &numSubsets);

			D3DXATTRIBUTERANGE* pAttributeTable = new D3DXATTRIBUTERANGE[numSubsets];
			g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].pMesh->GetAttributeTable(pAttributeTable, &numSubsets);

			for (int nCntSubset = 0; nCntSubset < (int)numSubsets; nCntSubset++)
			{//�T�u�Z�b�g����
				if (pAttributeTable[nCntSubset].FaceCount > 0) // �g���Ă���}�e���A���̂ݕ`��
				{
					// ���݂̃}�e���A���擾
					pDevice->GetMaterial(&matDef);

					// �����x�̐ݒ�
					D3DMATERIAL9 mat = pMat[pAttributeTable[nCntSubset].AttribId].MatD3D;
					if (nCntSubset == 0)
					{// �z�Ε���
						mat.Emissive = mat.Diffuse;
						mat.Emissive.a *= 0.5f;
						mat.Emissive.r *= 0.3f;
						mat.Emissive.g *= 0.3f;
						mat.Emissive.b *= 0.3f;
					}
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&mat);

					if (g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].TexIdx[nCntSubset] != -1)
					{//�e�N�X�`��������
						// �e�N�X�`���ݒ�
						pDevice->SetTexture(0, g_ItemManager.apTexture[g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].TexIdx[nCntSubset]]);
					}
					else
					{
						pDevice->SetTexture(0, nullptr);
					}

					if (nCntSubset == 0)
					{// �z�Ε���
						//���u�����f�B���O�����Z�����ɐݒ�
						pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
						pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
						pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
					}
					else
					{
						//���u�����f�B���O�����ɖ߂�
						pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
						pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
						pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
					}

					if (nCntSubset == 0)
					{// �z�Ε���
						for (int nCnt = 0; nCnt < 5; nCnt++)
						{// ����
							// �T�u�Z�b�g
							g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].pMesh->DrawSubset(pAttributeTable[nCntSubset].AttribId);
						}
					}
					else
					{
						// �T�u�Z�b�g
						g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].pMesh->DrawSubset(pAttributeTable[nCntSubset].AttribId);
					}

					// �}�e���A���ۑ�
					pDevice->SetMaterial(&matDef);
				}
			}
			delete[] pAttributeTable; // ���������
		}
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================
// �A�C�e��UI�̕`�揈��
//=============================================================
void DrawItemUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�փ|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �A�C�e��UI�̏���
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffitemUI, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < ITYPE_MAX; nCount++)
	{
		if (g_itemUI[nCount].bUse == true)
		{// �A�C�e�����������Ă��鎞
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureitemUI[ITYPE_MAX + nCount]);
			// �A�C�e��UI�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCount, 2);

		}
		else if (g_itemUI[nCount].bUse == false)
		{// �A�C�e�����������Ă��Ȃ���
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureitemUI[nCount]);
			// �A�C�e��UI�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCount, 2);
		}
	}
}

//------------------------------
// �ǂݍ��񂾃t�@�C�����̎擾
//------------------------------
void SetItemManager(ItemManager ItemManager)
{
	g_ItemManager = ItemManager; //�ǂݍ��񂾃t�@�C�������Z�b�g

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�փ|�C���^
	D3DXMATERIAL* pMat;			// �}�e���A���f�[�^�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	int nCntTex = 0;
	LPSTR Filename[ITEM_TEXTURE_MAX + 1]; //�e�N�X�`���p�X�ꎞ�ۑ��p
	for (int nCntModel = 0; nCntModel < g_ItemManager.ItemNum; nCntModel++)
	{// �ő吔�J��Ԃ�
		// X�t�@�C���ǂݍ���
		D3DXLoadMeshFromX
		(
			g_ItemManager.ItemModel[nCntModel].Pass.aName,
			D3DXMESH_MANAGED,
			pDevice,
			NULL,
			&g_ItemManager.ItemModel[nCntModel].pBuffMat,
			NULL,
			&g_ItemManager.ItemModel[nCntModel].dwNumMat,
			&g_ItemManager.ItemModel[nCntModel].pMesh
		);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_ItemManager.ItemModel[nCntModel].pBuffMat->GetBufferPointer();

		// �e�T�u�Z�b�g�̃t�F�C�X�����擾
		DWORD numSubsets = 0;
		g_ItemManager.ItemModel[nCntModel].pMesh->GetAttributeTable(nullptr, &numSubsets);

		D3DXATTRIBUTERANGE* pAttributeTable = new D3DXATTRIBUTERANGE[numSubsets];
		g_ItemManager.ItemModel[nCntModel].pMesh->GetAttributeTable(pAttributeTable, &numSubsets);

		for (int nCntSubset = 0; nCntSubset < (int)numSubsets; nCntSubset++)
		{//�}�e���A�����J��Ԃ�
			if (pMat[pAttributeTable[nCntSubset].AttribId].pTextureFilename != NULL)
			{//�e�N�X�`��������

				//�����e�N�X�`���͓ǂݍ��܂Ȃ�
				Filename[nCntTex] = pMat[pAttributeTable[nCntSubset].AttribId].pTextureFilename; //�e�N�X�`���p�X�ꎞ�ۑ�
				bool bLoad = true; //�ǂݍ��݂̊m�F
				for (int nCntTex2 = nCntTex - 1; nCntTex2 >= 0; nCntTex2--)
				{//�O�̃e�N�X�`��������
					if (!strcmp(Filename[nCntTex], Filename[nCntTex2]))
					{//�������e�N�X�`���𔭌�
						g_ItemManager.ItemModel[nCntModel].TexIdx[nCntSubset] = nCntTex2; //�����e�N�X�`����o�^
						bLoad = false; //�e�N�X�`���͓ǂݍ��܂Ȃ�
						break;
					}
				}

				if (bLoad)
				{//�ǂݍ���
					//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFileEx
					(
						pDevice,
						pMat[pAttributeTable[nCntSubset].AttribId].pTextureFilename,
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
						&g_ItemManager.apTexture[nCntTex]
					);

					//�ǂݍ��񂾃e�N�X�`����o�^
					g_ItemManager.ItemModel[nCntModel].TexIdx[nCntSubset] = nCntTex;
					//�e�N�X�`���ԍ��i�s
					nCntTex++;
				}
			}
			else
			{//�e�N�X�`�����Ȃ�
				g_ItemManager.ItemModel[nCntModel].TexIdx[nCntSubset] = -1;
			}
		}
		delete[] pAttributeTable; // ���������

		// ���_��
		g_ItemManager.ItemModel[nCntModel].nNumVtx = g_ItemManager.ItemModel[nCntModel].pMesh->GetNumVertices();
		// ���_�T�C�Y
		g_ItemManager.ItemModel[nCntModel].sizeFVF = D3DXGetFVFVertexSize(g_ItemManager.ItemModel[nCntModel].pMesh->GetFVF());
		// ���_�o�b�t�@�̎擾
		g_ItemManager.ItemModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_ItemManager.ItemModel[nCntModel].pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < g_ItemManager.ItemModel[nCntModel].nNumVtx; nCntVtx++)
		{// ���_���J��Ԃ�
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_ItemManager.ItemModel[nCntModel].pVtxBuff;

			// ���_�̑傫���̔��f
			if (vtx.x > g_ItemManager.ItemModel[nCntModel].vtxMax.x)
			{
				g_ItemManager.ItemModel[nCntModel].vtxMax.x = vtx.x;
			}
			if (vtx.x < g_ItemManager.ItemModel[nCntModel].vtxMin.x)
			{
				g_ItemManager.ItemModel[nCntModel].vtxMin.x = vtx.x;
			}
			if (vtx.y > g_ItemManager.ItemModel[nCntModel].vtxMax.y)
			{
				g_ItemManager.ItemModel[nCntModel].vtxMax.y = vtx.y;
			}
			if (vtx.y < g_ItemManager.ItemModel[nCntModel].vtxMin.y)
			{
				g_ItemManager.ItemModel[nCntModel].vtxMin.y = vtx.y;
			}
			if (vtx.z > g_ItemManager.ItemModel[nCntModel].vtxMax.z)
			{
				g_ItemManager.ItemModel[nCntModel].vtxMax.z = vtx.z;
			}
			if (vtx.z < g_ItemManager.ItemModel[nCntModel].vtxMin.z)
			{
				g_ItemManager.ItemModel[nCntModel].vtxMin.z = vtx.z;
			}

			g_ItemManager.ItemModel[nCntModel].pVtxBuff += g_ItemManager.ItemModel[nCntModel].sizeFVF;
		}

		// ���_�o�b�t�@�̃A�����b�N
		g_ItemManager.ItemModel[nCntModel].pMesh->UnlockVertexBuffer();

	}
}

//-----------
// �z�u
//-----------
int SetItem(ITYPE Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	int nCntItem;

	for (nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{// �ő吔�J��Ԃ�
		if (g_aItem[nCntItem].bUse == false)
		{// �A�C�e�����g�p���Ă��鎞
			// �I�u�W�F�N�g���̐ݒ�
			g_aItem[nCntItem].pos = pos;		// �ʒu��ݒ�
			g_aItem[nCntItem].rot = rot;		// ������ݒ�
			g_aItem[nCntItem].scale = scale;	// �傫����ݒ�
			// ������ݒ�
			g_aItem[nCntItem].fLength = sqrtf((g_ItemManager.ItemModel[Type].vtxMax.x * g_aItem[nCntItem].scale.x - g_ItemManager.ItemModel[Type].vtxMin.x * g_aItem[nCntItem].scale.x) * (g_ItemManager.ItemModel[Type].vtxMax.x * g_aItem[nCntItem].scale.x - g_ItemManager.ItemModel[Type].vtxMin.x * g_aItem[nCntItem].scale.x) + (g_ItemManager.ItemModel[Type].vtxMax.z * g_aItem[nCntItem].scale.z - g_ItemManager.ItemModel[Type].vtxMin.z * g_aItem[nCntItem].scale.z) * (g_ItemManager.ItemModel[Type].vtxMax.z * g_aItem[nCntItem].scale.z - g_ItemManager.ItemModel[Type].vtxMin.z * g_aItem[nCntItem].scale.z)) / 2.0f;
			// �e�̔ԍ���ݒ�
			g_aItem[nCntItem].nIdxShadow = SetShadow(g_aItem[nCntItem].pos);
			g_aItem[nCntItem].Type = Type;		// �^�C�v���ʎq��ݒ�
			g_aItem[nCntItem].bAlpha = false;	// �������ǂ�����ݒ肷��
			g_aItem[nCntItem].bUse = true;		// �g�p���Ă��邩�ǂ����ݒ肷��
			break;
		}
	}

	return nCntItem;
}

//------------------------------
// �ʒu�擾
//------------------------------
Item* GetItem(void)
{
	return &g_aItem[0]; //�A�C�e�����
}

//------------------------------
// ��ސ��擾
//------------------------------
int GetItemNum(void)
{
	return g_ItemManager.ItemNum; //�A�C�e���̎�ސ�
}

//------------------------------
// �����蔻��
//------------------------------
bool TriggerItem(D3DXVECTOR3 pos, Item** pItem)
{
	bool bGeting = false;
	int nCntItem = 0;

	for (nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{//�ő吔��
		if (g_aItem[nCntItem].bUse)
		{//�g�p���Ă���
			if (sqrtf((g_aItem[nCntItem].pos.x - pos.x) * (g_aItem[nCntItem].pos.x - pos.x) + (g_aItem[nCntItem].pos.y - pos.y) * (g_aItem[nCntItem].pos.y - pos.y) + (g_aItem[nCntItem].pos.z - pos.z) * (g_aItem[nCntItem].pos.z - pos.z)) <= ITEM_TRIGGER_SPACE)
			{//�߂Â�����
				bGeting = true; //�Q�b�g�\
				if (pItem != NULL)
				{// �A�C�e������n��
					*pItem = &g_aItem[nCntItem];
				}
				break;
			}
		}
	}
	return bGeting;
}

//------------------------------
// X�t�@�C�����擾
//------------------------------
char* GetItemName(int nNumber)
{
	return (nNumber < (int)ITYPE_MAX&& nNumber >= 0) ? g_ItemManager.ItemModel[nNumber].Pass.aName : NULL; //���O��n��
}

//---------------------------------------
// ���[�h����
//---------------------------------------
void LoadItemManager(void)
{
	FILE* pFile;	// �t�@�C���|�C���^��錾
	ItemManager ItemManager;
	int nCount = 0;

	//������
	ItemManager.ItemNum = 0;
	for (int nCntTex = 0; nCntTex < ITEM_TEXTURE_MAX; nCntTex++)
	{//�e�N�X�`�����J��Ԃ�
		ItemManager.apTexture[nCntTex] = NULL;
	}

	for (int nCntModel = 0; nCntModel < ITYPE_MAX; nCntModel++)
	{// ��ސ��J��Ԃ�
		//���ׂď�����
		*ItemManager.ItemModel[nCntModel].Pass.aName = '\0';
		ItemManager.ItemModel[nCntModel].pMesh = NULL;
		ItemManager.ItemModel[nCntModel].pBuffMat = NULL;
		ItemManager.ItemModel[nCntModel].dwNumMat = 0;
		for (int nCntMat = 0; nCntMat < ITEM_SUBSET_MAX; nCntMat++)
		{//�}�e���A�����J��Ԃ�
			ItemManager.ItemModel[nCntModel].TexIdx[nCntMat] = -1;
		}
		ItemManager.ItemModel[nCntModel].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ItemManager.ItemModel[nCntModel].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ItemManager.ItemModel[nCntModel].nNumVtx = 0;
		ItemManager.ItemModel[nCntModel].sizeFVF = 0;
		ItemManager.ItemModel[nCntModel].pVtxBuff = NULL;
	}

	// X�t�@�C�����J��
	if (fopen_s(&pFile, IMANAGER_FILE, "r") == 0 && pFile != NULL)
	{//�J������
		if (fscanf(pFile, "%d", &ItemManager.ItemNum) == EOF)//�A�C�e����
		{
			fclose(pFile);//�t�@�C�������
			return;
		}

		if (ItemManager.ItemNum > ITYPE_MAX)
		{//�^�C�v�ȏ�̃t�@�C����
			HWND hWnd;
			hWnd = GethWnd();
			MessageBox(hWnd, "���[�h�G���[", "X�t�@�C�������������܂�", MB_OK | MB_ICONERROR);
			PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
		}

		for (nCount = 0; nCount < ItemManager.ItemNum; nCount++)
		{//�p�X�̎擾
			if (fscanf(pFile, "%63s", ItemManager.ItemModel[nCount].Pass.aName) == EOF)break;//���l����������
		}

		//�ݒ�
		SetItemManager(ItemManager);

		fclose(pFile);//�t�@�C�������
	}
	else
	{//�J���Ȃ�����
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "���[�h�G���[", "ItemManager", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//---------------------------------------
// ���[�h����
//---------------------------------------
void LoadItem(void)
{
	FILE* pFile;	// �t�@�C���|�C���^��錾
	MODE Mode = GetMode();
	int nCount = 0;
	int nNumber = 0;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//X�t�@�C��
	switch (Mode)
	{
	case MODE_GAME:
		pFile = fopen(ITEM_FILE, "r");	// �t�@�C�����J��
		break;
	case MODE_TUTO:
		pFile = fopen(ITEM_TUTO_FILE, "r");	// �t�@�C�����J��
		break;
	default:
		pFile = fopen(nullptr, "r");//�t�@�C�����J��
		break;
	}

	if (pFile != NULL)
	{// �J������
		for (nCount = 0; nCount < ITEM_MAX; nCount++)
		{//�A�C�e������
			if (fscanf(pFile, "%d", &nNumber) == EOF)break;		// ���l����������
			if (fscanf(pFile, "%f", &pos.x) == EOF)break;		// ���l����������
			if (fscanf(pFile, "%f", &pos.y) == EOF)break;		// ���l����������
			if (fscanf(pFile, "%f", &pos.z) == EOF)break;		// ���l����������
			if (fscanf(pFile, "%f", &rot.x) == EOF)break;		// ���l����������
			if (fscanf(pFile, "%f", &rot.y) == EOF)break;		// ���l����������
			if (fscanf(pFile, "%f", &rot.z) == EOF)break;		// ���l����������
			if (fscanf(pFile, "%f", &scale.x) == EOF)break;		// ���l����������
			if (fscanf(pFile, "%f", &scale.y) == EOF)break;		// ���l����������
			if (fscanf(pFile, "%f", &scale.z) == EOF)break;		// ���l����������

			// �p�x�ϊ�
			rot = D3DXToRadian(rot);

			//�ُ�ԍ�
			switch (Mode)
			{
			case MODE_GAME:
				if (nNumber >= g_ItemManager.ItemNum - ITYPE_TUTO_MAX || nNumber < 0)
				{
					continue;
				}
				break;
			case MODE_TUTO:
				if (nNumber < g_ItemManager.ItemNum - ITYPE_TUTO_MAX || nNumber >= g_ItemManager.ItemNum)
				{
					continue;
				}
				break;
			}

			// �I�u�W�F�N�g�ݒu
			SetItem((ITYPE)nNumber, pos, rot, scale);
		}
		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �J���Ȃ�����
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "���[�h�G���[", "Item", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//---------------------------------------
//���[�h����
//---------------------------------------
void SaveItem(void)
{
	FILE* pFile;	// �t�@�C���|�C���^��錾
	MODE Mode = GetMode();
	D3DXVECTOR3 rot;

	// X�t�@�C��
	switch (Mode)
	{
	case MODE_GAME:
		pFile = fopen(ITEM_FILE, "w");	// �t�@�C�����J��
		break;
	case MODE_TUTO:
		pFile = fopen(ITEM_TUTO_FILE, "w");	// �t�@�C�����J��
		break;
	default:
		pFile = fopen(nullptr, "w");//�t�@�C�����J��
		break;
	}

	if (pFile != NULL)
	{// �J������
		for (int nCount = 0; nCount < ITEM_MAX; nCount++)
		{//�A�C�e������
			if (g_aItem[nCount].bUse == true)
			{//�g���Ă���
				// �p�x�ϊ�
				rot = D3DXToDegree(g_aItem[nCount].rot);

				fprintf(pFile, "%d\n", (int)g_aItem[nCount].Type);	// ���l����������
				fprintf(pFile, "%.1f ", g_aItem[nCount].pos.x);		// ���l����������
				fprintf(pFile, "%.1f ", g_aItem[nCount].pos.y);		// ���l����������
				fprintf(pFile, "%.1f\n", g_aItem[nCount].pos.z);	// ���l����������
				fprintf(pFile, "%.1f ", rot.x);						// ���l����������
				fprintf(pFile, "%.1f ", rot.y);						// ���l����������
				fprintf(pFile, "%.1f\n", rot.z);					// ���l����������
				fprintf(pFile, "%.1f ", g_aItem[nCount].scale.x);	// ���l����������
				fprintf(pFile, "%.1f ", g_aItem[nCount].scale.y);	// ���l����������
				fprintf(pFile, "%.1f\n", g_aItem[nCount].scale.z);	// ���l����������
			}
		}
		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �J���Ȃ�����
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "���[�h�G���[", "���[�h�ł��Ȃ�������", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//--------------------------
// �I�u�W�F�N�g�G�f�B�^�[
//--------------------------
void UpdateItemEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot)
{
	Camera* pCamera = GetCamera();
	static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	static int HoldNumber = -1, EndNumber = -1;
	MODE Mode = GetMode();

	if (GetKeyboardTrigger(DIK_RIGHT))
	{// �I�u�W�F�N�g�ؑ�
		g_Type = (ITYPE)(g_Type + 1);
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{// �I�u�W�F�N�g�ؑ�
		g_Type = (ITYPE)(g_Type - 1);
	}

	switch (Mode)
	{
	case MODE_GAME:
		//�͈͐���
		g_Type = (ITYPE)(g_Type % (g_ItemManager.ItemNum - ITYPE_TUTO_MAX));

		if (g_Type < 0)
		{//���[�v
			g_Type = (ITYPE)(g_ItemManager.ItemNum - 1 - ITYPE_TUTO_MAX);
		}	
		break;
	case MODE_TUTO:
		//�͈͐���
		if (g_Type > g_ItemManager.ItemNum - 1)
		{//���[�v
			g_Type = (ITYPE)(g_ItemManager.ItemNum - ITYPE_TUTO_MAX);
		}

		if (g_Type < g_ItemManager.ItemNum - ITYPE_TUTO_MAX)
		{//���[�v
			g_Type = (ITYPE)(g_ItemManager.ItemNum - 1);
		}
		break;
	}

	if (GetKeyboardTrigger(DIK_UP))
	{// DEL�I�u�W�F�N�g�ؑ�
		g_DelItem++;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{// DEL�I�u�W�F�N�g�ؑ�
		g_DelItem--;
	}

	for (int nCount = 0; nCount < ITEM_MAX; nCount++)
	{//�ő吔��

		//�͈͐���
		if (g_DelItem < 0)
		{
			g_DelItem = ITEM_MAX - 1;
		}
		g_DelItem %= ITEM_MAX;

		if (g_aItem[g_DelItem].bUse)
		{//�g���Ă���I�u�W�F�N�g�̑I��
			break;
		}
		else
		{//�g���Ă��Ȃ��ԍ��̓X�L�b�v

			if (GetKeyboardTrigger(DIK_DOWN))
			{// DEL�I�u�W�F�N�g�ؑ�
				g_DelItem--;
			}
			else
			{// DEL�I�u�W�F�N�g�ؑ�
				g_DelItem++;
			}
		}

		if (nCount == ITEM_MAX - 1)
		{//�I�u�W�F�N�g���Ȃ�
			g_DelItem = -1;
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

	if (GetKeyboardPress(DIK_U) || GetJoykeyPress(JOYKEY_A, CONTROLLER_1))
	{// �����Ă����
		rot.x -= 0.1f;
	}
	if (GetKeyboardPress(DIK_I) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// �����Ă����
		rot.x += 0.1f;
	}
	if (GetKeyboardPress(DIK_J) || GetJoykeyPress(JOYKEY_LT, CONTROLLER_1))
	{// �����Ă����
		rot.y -= 0.1f;
	}
	if (GetKeyboardPress(DIK_K) || GetJoykeyPress(JOYKEY_RT, CONTROLLER_1))
	{// �����Ă����
		rot.y += 0.1f;
	}
	if (GetKeyboardPress(DIK_M) || GetJoykeyPress(JOYKEY_X, CONTROLLER_1))
	{// �����Ă����
		rot.z -= 0.1f;
	}
	if (GetKeyboardPress(DIK_COMMA) || GetJoykeyPress(JOYKEY_Y, CONTROLLER_1))
	{// �����Ă����
		rot.z += 0.1f;
	}
	if (GetKeyboardPress(DIK_7) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// �����Ă����
		scale.x -= 0.2f;
		scale.y -= 0.2f;
		scale.z -= 0.2f;
	}
	if (GetKeyboardPress(DIK_8) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// �����Ă����
		scale.x += 0.2f;
		scale.y += 0.2f;
		scale.z += 0.2f;
	}

	if (GetKeyboardTrigger(DIK_0) || GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1))
	{// ������
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

	//���K��
	if (rot.x > D3DX_PI)
	{
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x < -D3DX_PI)
	{
		rot.x += D3DX_PI * 2.0f;
	}
	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}
	if (rot.z > D3DX_PI)
	{
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z < -D3DX_PI)
	{
		rot.z += D3DX_PI * 2.0f;
	}

	//�X�P�[������
	scale.x = max(ITEM_SCALE_MIN, min(ITEM_SCALE_MAX, scale.x));
	scale.y = max(ITEM_SCALE_MIN, min(ITEM_SCALE_MAX, scale.y));
	scale.z = max(ITEM_SCALE_MIN, min(ITEM_SCALE_MAX, scale.z));

	if (GetKeyboardTrigger(DIK_RSHIFT) || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1))
	{// �������Ƃ�
		HoldNumber = SetItem(g_Type, Epos + pos, Erot + rot, scale);
		EndNumber = HoldNumber;
	}
	if (GetKeyboardPress(DIK_RSHIFT) || GetJoykeyPress(JOYKEY_RB, CONTROLLER_1))
	{// �����Ă����
		g_aItem[HoldNumber].pos = Epos + pos;
		g_aItem[HoldNumber].rot = Erot + rot;
		g_aItem[HoldNumber].scale = scale;

		static ITYPE Type = ITYPE_1; //�ꎞ�ۑ��p�i���o�[

		if (Type != g_Type)
		{//�i���o�[���ω�����
			//�I�u�W�F�N�g�̏���
			NullShadow(g_aItem[HoldNumber].nIdxShadow);
			g_aItem[HoldNumber].bUse = false;
			//�I�u�W�F�N�g�̐؂�ւ�
			HoldNumber = SetItem(g_Type, Epos + pos, Erot + rot, scale);
			EndNumber = HoldNumber;
			Type = g_Type; //�i���o�[�̍X�V
		}
	}
	else
	{
		HoldNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_BACK) || GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1))
	{// �������Ƃ�
		NullShadow(g_aItem[EndNumber].nIdxShadow);
		g_aItem[EndNumber].bUse = false;
		EndNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_F8))
	{// �Z�[�u
		SaveItem();
	}

	if (GetKeyboardTrigger(DIK_DELETE) || (GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1) && GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1)))
	{// �������Ƃ�
		if (g_DelItem != -1)
		{
			NullShadow(g_aItem[g_DelItem].nIdxShadow);
			g_aItem[g_DelItem].bUse = false;
		}
	}

	static int DelNumber = -1; //�ꎞ�ۑ��p�i���o�[

	if (DelNumber != g_DelItem)
	{//�f���[�g�i���o�[���ω�����
		if (DelNumber != -1)
		{//�O��̃I�u�W�F�N�g�����݂���
			g_aItem[DelNumber].bAlpha = false; //���ɖ߂�
		}
		DelNumber = g_DelItem; //�I���I�u�W�F�N�g�̍X�V
		if (DelNumber != -1)
		{//�I�u�W�F�N�g�����݂���
			g_aItem[DelNumber].bAlpha = true; //���̒l�̕ێ�
		}
	}
}

//--------------------
//���݂�X�t�@�C���i���o�[
//--------------------
int GetInumber(void)
{
	return (int)g_Type; //���݂̎��(Edit)
}

//--------------------
//���݂̃f���[�g�i���o�[
//--------------------
int GetIDelnumber(void)
{
	return g_DelItem; //DEL�L�[�ŏ�����C���f�b�N�X�ԍ�
}

//==================================================
// �A�C�e��UI��\�����鏈�� misaki
//==================================================
void DisplayItem(bool bDisp, int nType)
{
	if (g_itemUI[nType].bUse == false)
	{
		g_itemUI[nType].bUse = bDisp;
	}
}