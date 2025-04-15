//----------------------------------------
//
//�I�u�W�F�N�g����[Object.cpp]
//Author fuma sato
//
//----------------------------------------

#include"object.h"
#include"shadow.h"
#include"player.h"
#include"camera.h"
#include"meshfield.h"
#include"barrier.h"
#include"rotation.h"
#include"input.h"
#include"vectorcollision.h"

//�O���[�o���ϐ��錾
Object g_aObject[OBJECT_MAX] = {}; //�I�u�W�F�N�g����
ObjectManager g_ObjectManager;     //�t�@�C������
OTYPE g_Type;                      //�I�u�W�F�N�g�ԍ��Ǘ�
int g_DelObject;                   //�I�u�W�F�N�g�����p
//----------------------
//����������
//----------------------
void InitObject(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	int nCntObject, nCntTex;
	for (nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//�ő吔�J��Ԃ�

		//���̏�����
		g_aObject[nCntObject].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObject[nCntObject].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObject[nCntObject].fLength = 0.0f;
		g_aObject[nCntObject].fHeight = 0.0f;
		g_aObject[nCntObject].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		for (int nCntShadow = 0; nCntShadow < 2; nCntShadow++)
		{
			g_aObject[nCntObject].nIdxShadow[nCntShadow] = -1;
		}
		g_aObject[nCntObject].Type = OTYPE_ROKET;
		g_aObject[nCntObject].bAlpha = false;
		g_aObject[nCntObject].bUse = false;
		g_aObject[nCntObject].nIdxRadar = -1;	// ���[�_�[�C���f�b�N�X tetsuji
	}

	//�t�@�C�����̏�����
	g_ObjectManager.ObjectNum = 0;
	for (nCntTex = 0; nCntTex < OBJECT_TEXTURE_MAX; nCntTex++)
	{//�e�N�X�`�����J��Ԃ�
		g_ObjectManager.apTexture[nCntTex] = NULL;
	}
	int nCntModel, nCntMat;
	for (nCntModel = 0; nCntModel < OTYPE_MAX; nCntModel++)
	{//��ސ���
		*g_ObjectManager.ObjectModel[nCntModel].Pass.aName = '\0';
		g_ObjectManager.ObjectModel[nCntModel].pMesh = NULL;
		g_ObjectManager.ObjectModel[nCntModel].pBuffMat = NULL;
		g_ObjectManager.ObjectModel[nCntModel].dwNumMat = 0;
		for (nCntMat = 0; nCntMat < OBJECT_SUBSET_MAX; nCntMat++)
		{//�}�e���A�����J��Ԃ�
			g_ObjectManager.ObjectModel[nCntModel].TexIdx[nCntMat] = -1;
		}
		g_ObjectManager.ObjectModel[nCntModel].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ObjectManager.ObjectModel[nCntModel].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ObjectManager.ObjectModel[nCntModel].nNumVtx = 0;
		g_ObjectManager.ObjectModel[nCntModel].sizeFVF = 0;
		g_ObjectManager.ObjectModel[nCntModel].pVtxBuff = NULL;
	}

	//�I�u�W�F�N�g�t�@�C�����擾
	LoadObjectManager();

	//�ԍ��̏�����
	g_Type = OTYPE_ROKET;
	g_DelObject = -1;
}

//-------------------
//�I������
//-------------------
void UninitObject(void)
{
	for (int nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//�ő吔�J��Ԃ�
		//���̏�����
		g_aObject[nCntObject].bUse = false;
	}

	for (int nCntTex = 0; nCntTex < OBJECT_TEXTURE_MAX; nCntTex++)
	{//�e�N�X�`���̍ő吔�J��Ԃ� tetsuji
		//�e�N�X�`���̔j��
		if (g_ObjectManager.apTexture[nCntTex] != NULL)
		{
			g_ObjectManager.apTexture[nCntTex]->Release();
			g_ObjectManager.apTexture[nCntTex] = NULL;
		}
	}
	for (int nCntModel = 0; nCntModel < OTYPE_MAX; nCntModel++)
	{//�ő吔�J��Ԃ�
		//���_�o�b�t�@�̔j��
		if (g_ObjectManager.ObjectModel[nCntModel].pVtxBuff != NULL)
		{
			g_ObjectManager.ObjectModel[nCntModel].pVtxBuff = NULL;
		}
		//���b�V���̔j��
		if (g_ObjectManager.ObjectModel[nCntModel].pMesh != NULL)
		{
			g_ObjectManager.ObjectModel[nCntModel].pMesh->Release();
			g_ObjectManager.ObjectModel[nCntModel].pMesh = NULL;
		}
		//�}�e���A���̔j��
		if (g_ObjectManager.ObjectModel[nCntModel].pBuffMat != NULL)
		{
			g_ObjectManager.ObjectModel[nCntModel].pBuffMat->Release();
			g_ObjectManager.ObjectModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//-------------------
//�X�V����
//-------------------
void UpdateObject(void)
{
	for (int nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//�ő吔�J��Ԃ�
		if (g_aObject[nCntObject].bUse)
		{//�g�p��
			switch (g_aObject[nCntObject].Type)
			{
			case OTYPE_ROKET:
				SetPositionShadow(g_aObject[nCntObject].nIdxShadow[0], g_aObject[nCntObject].pos, g_aObject[nCntObject].scale, g_aObject[nCntObject].fLength);
				break;
			}
		}
	}
}

//-------------------
//�`�揈��
//-------------------
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;//�v�Z�}�g���b�N�X
	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	int nCntObject;
	for (nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//�ő吔�J��Ԃ�
		if (g_aObject[nCntObject].bUse)
		{//�g�p��
			//�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aObject[nCntObject].mtxWorld);

			//�傫���̔��f
			D3DXMatrixScaling(&mtxScale, g_aObject[nCntObject].scale.x, g_aObject[nCntObject].scale.y, g_aObject[nCntObject].scale.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxScale);

			//�����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObject[nCntObject].rot.y, g_aObject[nCntObject].rot.x, g_aObject[nCntObject].rot.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxRot);

			//�ʒu�̔��f
			D3DXMatrixTranslation(&mtxTrans, g_aObject[nCntObject].pos.x, g_aObject[nCntObject].pos.y, g_aObject[nCntObject].pos.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aObject[nCntObject].mtxWorld);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_ObjectManager.ObjectModel[(int)g_aObject[nCntObject].Type].pBuffMat->GetBufferPointer();

			// �e�T�u�Z�b�g�̃t�F�C�X�����擾
			DWORD numSubsets = 0;
			g_ObjectManager.ObjectModel[(int)g_aObject[nCntObject].Type].pMesh->GetAttributeTable(nullptr, &numSubsets);

			D3DXATTRIBUTERANGE* pAttributeTable = new D3DXATTRIBUTERANGE[numSubsets];
			g_ObjectManager.ObjectModel[(int)g_aObject[nCntObject].Type].pMesh->GetAttributeTable(pAttributeTable, &numSubsets);

			for (int nCntSubset = 0; nCntSubset < (int)numSubsets; nCntSubset++)
			{//�T�u�Z�b�g����
				if ((int)pAttributeTable[nCntSubset].FaceCount > 0) // �g���Ă���}�e���A���̂ݕ`��
				{
					//���݂̃}�e���A���擾
					pDevice->GetMaterial(&matDef);

					// �����x�̐ݒ�
					D3DMATERIAL9 mat = pMat[pAttributeTable[nCntSubset].AttribId].MatD3D;
					mat.Diffuse.a = 1.0f - 0.5f * g_aObject[nCntObject].bAlpha;
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&mat);

					if (g_ObjectManager.ObjectModel[(int)g_aObject[nCntObject].Type].TexIdx[nCntSubset] != -1)
					{//�e�N�X�`��������
						// �e�N�X�`���ݒ�
						pDevice->SetTexture(0, g_ObjectManager.apTexture[g_ObjectManager.ObjectModel[(int)g_aObject[nCntObject].Type].TexIdx[nCntSubset]]);
					}
					else
					{
						pDevice->SetTexture(0, nullptr);
					}

					// �T�u�Z�b�g�`��
					g_ObjectManager.ObjectModel[(int)g_aObject[nCntObject].Type].pMesh->DrawSubset(pAttributeTable[nCntSubset].AttribId);

					//�}�e���A���ۑ�
					pDevice->SetMaterial(&matDef);
				}
			}
			delete[] pAttributeTable; // ���������
		}
	}
}

//------------------------------
//�ǂݍ��񂾃t�@�C�����̎擾
//------------------------------
void SetObjectManager(ObjectManager ObjectManager)
{
	g_ObjectManager = ObjectManager;

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^
	int nCountModel, nCntTex = 0, nCntSubset;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	nCntTex = 0;
	LPSTR Filename[OBJECT_TEXTURE_MAX + 1]; //�e�N�X�`���p�X�ꎞ�ۑ��p
	for (nCountModel = 0; nCountModel < g_ObjectManager.ObjectNum; nCountModel++)
	{
		//X�t�@�C���ǂݍ���
		D3DXLoadMeshFromX
		(
			g_ObjectManager.ObjectModel[nCountModel].Pass.aName,
			D3DXMESH_MANAGED,
			pDevice,
			NULL,
			&g_ObjectManager.ObjectModel[nCountModel].pBuffMat,
			NULL,
			&g_ObjectManager.ObjectModel[nCountModel].dwNumMat,
			&g_ObjectManager.ObjectModel[nCountModel].pMesh
		);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_ObjectManager.ObjectModel[nCountModel].pBuffMat->GetBufferPointer();

		// �e�T�u�Z�b�g�̃t�F�C�X�����擾
		DWORD numSubsets = 0;
		g_ObjectManager.ObjectModel[nCountModel].pMesh->GetAttributeTable(nullptr, &numSubsets);

		D3DXATTRIBUTERANGE* pAttributeTable = new D3DXATTRIBUTERANGE[numSubsets];
		g_ObjectManager.ObjectModel[nCountModel].pMesh->GetAttributeTable(pAttributeTable, &numSubsets);

		for (nCntSubset = 0; nCntSubset < (int)numSubsets; nCntSubset++)
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
						g_ObjectManager.ObjectModel[nCountModel].TexIdx[nCntSubset] = nCntTex2; //�����e�N�X�`����o�^
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
						&g_ObjectManager.apTexture[nCntTex]
					);

					//�ǂݍ��񂾃e�N�X�`����o�^
					g_ObjectManager.ObjectModel[nCountModel].TexIdx[nCntSubset] = nCntTex;
					//�e�N�X�`���ԍ��i�s
					nCntTex++;
				}
			}
			else
			{//�e�N�X�`�����Ȃ�
				g_ObjectManager.ObjectModel[nCountModel].TexIdx[nCntSubset] = -1;
			}
		}
		delete[] pAttributeTable; // ���������

		//���_��
		g_ObjectManager.ObjectModel[nCountModel].nNumVtx = g_ObjectManager.ObjectModel[nCountModel].pMesh->GetNumVertices();
		//���_�T�C�Y
		g_ObjectManager.ObjectModel[nCountModel].sizeFVF = D3DXGetFVFVertexSize(g_ObjectManager.ObjectModel[nCountModel].pMesh->GetFVF());
		//���_�o�b�t�@�̎擾
		g_ObjectManager.ObjectModel[nCountModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_ObjectManager.ObjectModel[nCountModel].pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < g_ObjectManager.ObjectModel[nCountModel].nNumVtx; nCntVtx++)
		{//���_���J��Ԃ�
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_ObjectManager.ObjectModel[nCountModel].pVtxBuff;

			//���_�̑傫���̔��f
			if (vtx.x > g_ObjectManager.ObjectModel[nCountModel].vtxMax.x)
			{
				g_ObjectManager.ObjectModel[nCountModel].vtxMax.x = vtx.x;
			}
			if (vtx.x < g_ObjectManager.ObjectModel[nCountModel].vtxMin.x)
			{
				g_ObjectManager.ObjectModel[nCountModel].vtxMin.x = vtx.x;
			}
			if (vtx.y > g_ObjectManager.ObjectModel[nCountModel].vtxMax.y)
			{
				g_ObjectManager.ObjectModel[nCountModel].vtxMax.y = vtx.y;
			}
			if (vtx.y < g_ObjectManager.ObjectModel[nCountModel].vtxMin.y)
			{
				g_ObjectManager.ObjectModel[nCountModel].vtxMin.y = vtx.y;
			}
			if (vtx.z > g_ObjectManager.ObjectModel[nCountModel].vtxMax.z)
			{
				g_ObjectManager.ObjectModel[nCountModel].vtxMax.z = vtx.z;
			}
			if (vtx.z < g_ObjectManager.ObjectModel[nCountModel].vtxMin.z)
			{
				g_ObjectManager.ObjectModel[nCountModel].vtxMin.z = vtx.z;
			}

			g_ObjectManager.ObjectModel[nCountModel].pVtxBuff += g_ObjectManager.ObjectModel[nCountModel].sizeFVF;
		}

		// ���_�o�b�t�@�̃A�����b�N
        g_ObjectManager.ObjectModel[nCountModel].pMesh->UnlockVertexBuffer();
	}
}

//-----------
//�z�u
//-----------
int SetObject(OTYPE Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	int nCntObject;
	for (nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//�ő吔�J��Ԃ�
		if (!g_aObject[nCntObject].bUse)
		{//�g�p��
			//�I�u�W�F�N�g���̐ݒ�
			g_aObject[nCntObject].pos = pos;
			g_aObject[nCntObject].rot = rot;
			g_aObject[nCntObject].scale = scale;
			g_aObject[nCntObject].fLength = sqrtf((g_ObjectManager.ObjectModel[Type].vtxMax.x * g_aObject[nCntObject].scale.x - g_ObjectManager.ObjectModel[Type].vtxMin.x * g_aObject[nCntObject].scale.x) * (g_ObjectManager.ObjectModel[Type].vtxMax.x * g_aObject[nCntObject].scale.x - g_ObjectManager.ObjectModel[Type].vtxMin.x * g_aObject[nCntObject].scale.x) + (g_ObjectManager.ObjectModel[Type].vtxMax.z * g_aObject[nCntObject].scale.z - g_ObjectManager.ObjectModel[Type].vtxMin.z * g_aObject[nCntObject].scale.z) * (g_ObjectManager.ObjectModel[Type].vtxMax.z * g_aObject[nCntObject].scale.z - g_ObjectManager.ObjectModel[Type].vtxMin.z * g_aObject[nCntObject].scale.z)) / 2.0f;
			g_aObject[nCntObject].fHeight = g_ObjectManager.ObjectModel[Type].vtxMax.y * g_aObject[nCntObject].scale.y - g_ObjectManager.ObjectModel[Type].vtxMin.y * g_aObject[nCntObject].scale.y;
			switch (Type)
			{
			case OTYPE_ROKET:
				g_aObject[nCntObject].nIdxShadow[0] = SetShadow(g_aObject[nCntObject].pos);
				g_aObject[nCntObject].nIdxShadow[1] = -1;
				break;
			}
			g_aObject[nCntObject].Type = Type;
			g_aObject[nCntObject].bAlpha = false;
			g_aObject[nCntObject].bUse = true;
			break;
		}
	}
	return nCntObject;
}

//------------------------------
//�ʒu�擾
//------------------------------
Object* GetObject_EX(void)
{
	return &g_aObject[0]; //�I�u�W�F�N�g���
}

//------------------------------
//��ސ��擾
//------------------------------
int GetObjectNum(void)
{
	return g_ObjectManager.ObjectNum; //��ސ��擾
}

//----------------------
//�C���f�b�N�X�̎擾
//----------------------
int GetObjectIdx(OTYPE Type, int Number)
{
	int nCnt = 0;
	for (int nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//�ő吔��
		if (g_aObject[nCntObject].bUse&& g_aObject[nCntObject].Type==Type)
		{//�w��̃^�C�v�̎�
			if (Number == nCnt)
			{
				return nCntObject;
			}
			nCnt++;
		}
	}
	return 0;
}

//------------------------------
//�����蔻��
//------------------------------
bool CollisionObject(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius, float fHeight, Object** pObject)
{
	bool bRanding = false;
	int nCntObject = 0;

	for (nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//�ő吔��
		if (g_aObject[nCntObject].bUse && (g_aObject[nCntObject].Type == OTYPE_ROKET || (g_aObject[nCntObject].Type >= OTYPE_STONE1 && g_aObject[nCntObject].Type <= OTYPE_STONE4) || g_aObject[nCntObject].Type == OTYPE_TUTO_BLOCK || g_aObject[nCntObject].Type == OTYPE_TUTO_GATE))
		{
			// 4���_�̃��[�J�����W�����
			D3DXVECTOR3 RightUp = D3DXVECTOR3(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.x * g_aObject[nCntObject].scale.x, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.z * g_aObject[nCntObject].scale.z),
				         LeftUp = D3DXVECTOR3(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.x * g_aObject[nCntObject].scale.x, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.z * g_aObject[nCntObject].scale.z),
				       LeftDown = D3DXVECTOR3(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.x * g_aObject[nCntObject].scale.x, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.z * g_aObject[nCntObject].scale.z),
				      RightDown = D3DXVECTOR3(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.x * g_aObject[nCntObject].scale.x, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.z * g_aObject[nCntObject].scale.z);

			// 4���_�ɉ�]��K������
			RotationofShaftbyMatrix(RightUp, RightUp, g_aObject[nCntObject].rot, TYPE_Y);
			RotationofShaftbyMatrix(LeftUp, LeftUp, g_aObject[nCntObject].rot, TYPE_Y);
			RotationofShaftbyMatrix(LeftDown, LeftDown, g_aObject[nCntObject].rot, TYPE_Y);
			RotationofShaftbyMatrix(RightDown, RightDown, g_aObject[nCntObject].rot, TYPE_Y);

			// 2���_�����[���h���W�ɂ���
			RightUp += g_aObject[nCntObject].pos;
			LeftUp += g_aObject[nCntObject].pos;
			LeftDown += g_aObject[nCntObject].pos;
			RightDown += g_aObject[nCntObject].pos;

			D3DXVECTOR3 PosBox[4] = { RightUp,RightDown, LeftDown ,LeftUp }; // �G���A�̐���

			//y��
			if (VectorInside(*pPos, fRadius, PosBox))
			{// �����ɂ���
				//�������
				if (pPosOld->y + fHeight <= g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y* g_aObject[nCntObject].scale.y && pPos->y + fHeight > g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y* g_aObject[nCntObject].scale.y)
				{
					pPos->y = g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y* g_aObject[nCntObject].scale.y - fHeight;
					pMove->y = 0.0f;
				}
				//�ォ�牺
				if (pPosOld->y >= g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y* g_aObject[nCntObject].scale.y && pPos->y < g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y* g_aObject[nCntObject].scale.y)
				{
					pPos->y = g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y * g_aObject[nCntObject].scale.y;
					pMove->y = 0.0f;
					bRanding = true;
					if (pObject != NULL)
					{
						*pObject = &g_aObject[nCntObject];
					}
				}
			}

			//x,z��
			VectorCollision(*pPos, *pPosOld, fRadius, fHeight, RightUp, LeftUp, fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y * g_aObject[nCntObject].scale.y) + fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y));
			VectorCollision(*pPos, *pPosOld, fRadius, fHeight, LeftUp, LeftDown, fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y * g_aObject[nCntObject].scale.y) + fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y));
			VectorCollision(*pPos, *pPosOld, fRadius, fHeight, LeftDown, RightDown, fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y * g_aObject[nCntObject].scale.y) + fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y));
			VectorCollision(*pPos, *pPosOld, fRadius, fHeight, RightDown, RightUp, fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y * g_aObject[nCntObject].scale.y) + fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y));
		}
	}
	return bRanding;
}

//------------------------------
//�����蔻��
//------------------------------
bool TriggerObject(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fRadius)
{
	bool bTrigger = false;
	int nCntObject = 0;

	for (nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//�ő吔��
		if (g_aObject[nCntObject].bUse && (g_aObject[nCntObject].Type == OTYPE_ROKET || (g_aObject[nCntObject].Type >= OTYPE_STONE1 && g_aObject[nCntObject].Type <= OTYPE_STONE4) || g_aObject[nCntObject].Type == OTYPE_TUTO_BLOCK))
		{
			//y��
			if (pPos->x + fRadius > g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.x* g_aObject[nCntObject].scale.x && pPos->x - fRadius < g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.x* g_aObject[nCntObject].scale.x && pPos->z + fRadius > g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.z* g_aObject[nCntObject].scale.z && pPos->z - fRadius < g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.z* g_aObject[nCntObject].scale.z)
			{
				//�������
				if (pPosOld->y + fRadius <= g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y* g_aObject[nCntObject].scale.y && pPos->y + fRadius > g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y* g_aObject[nCntObject].scale.y)
				{
					bTrigger = true;
				}
				//�ォ�牺
				if (pPosOld->y - fRadius >= g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y* g_aObject[nCntObject].scale.y && pPos->y - fRadius < g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y* g_aObject[nCntObject].scale.y)
				{
					bTrigger = true;
				}
			}
			//x,z��
			if (pPos->y + fRadius > g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y* g_aObject[nCntObject].scale.y && pPos->y - fRadius < g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y* g_aObject[nCntObject].scale.y)
			{
				//x��
				if (pPos->z + fRadius > g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.z* g_aObject[nCntObject].scale.z && pPos->z - fRadius < g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.z* g_aObject[nCntObject].scale.z)
				{
					//������E
					if (pPosOld->x + fRadius <= g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.x* g_aObject[nCntObject].scale.x && pPos->x + fRadius > g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.x* g_aObject[nCntObject].scale.x)
					{
						bTrigger = true;
					}
					//�E���獶
					if (pPosOld->x - fRadius >= g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.x* g_aObject[nCntObject].scale.x && pPos->x - fRadius < g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.x* g_aObject[nCntObject].scale.x)
					{
						bTrigger = true;
					}
				}
				//z��
				if (pPos->x + fRadius > g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.x* g_aObject[nCntObject].scale.x && pPos->x - fRadius < g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.x* g_aObject[nCntObject].scale.x)
				{
					//��O���牜
					if (pPosOld->z + fRadius <= g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.z* g_aObject[nCntObject].scale.z && pPos->z + fRadius > g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.z* g_aObject[nCntObject].scale.z)
					{
						bTrigger = true;
					}
					//�������O
					if (pPosOld->z - fRadius >= g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.z* g_aObject[nCntObject].scale.z && pPos->z - fRadius < g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.z* g_aObject[nCntObject].scale.z)
					{
						bTrigger = true;
					}
				}
			}
		}
	}
	return bTrigger;
}

//-------------------------------------------------
//�����蔻��
//-------------------------------------------------
bool InsideObject(OTYPE Type, D3DXVECTOR3 pos, Object** pObject)
{
	bool bGeting = false;
	int nCntObject = 0;

	for (nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//�ő吔��
		if (g_aObject[nCntObject].bUse && g_aObject[nCntObject].Type == Type)
		{//�w�肳�ꂽ�^�C�v�̃I�u�W�F�N�g
			float TriggerSpace = 0.0f;
			if (g_aObject[nCntObject].Type == OTYPE_ROKET || g_aObject[nCntObject].Type == OTYPE_TUTO_GATE)
			{
				TriggerSpace = ROKET_TRIGGER_SPACE;
			}
			else if (g_aObject[nCntObject].Type == OTYPE_TUTO_BLOCK)
			{
				TriggerSpace = g_aObject[nCntObject].fLength;
			}

			if (sqrtf((g_aObject[nCntObject].pos.x - pos.x) * (g_aObject[nCntObject].pos.x - pos.x) + (g_aObject[nCntObject].pos.y - pos.y) * (g_aObject[nCntObject].pos.y - pos.y) + (g_aObject[nCntObject].pos.z - pos.z) * (g_aObject[nCntObject].pos.z - pos.z)) <= TriggerSpace)
			{//�߂Â�����
				if (pObject!=NULL)
				{
					*pObject = &g_aObject[nCntObject];
				}
				bGeting = true; //�擾�\
				break;
			}
		}
	}
	return bGeting;
}

//------------------------------
//X�t�@�C�����擾
//------------------------------
char* GetObjectName(int nNumber)
{
	return (nNumber < OTYPE_MAX&& nNumber >= 0) ? g_ObjectManager.ObjectModel[nNumber].Pass.aName : NULL; //�p�X��Ԃ�
}

//---------------------------------------
//���[�h����
//---------------------------------------
void LoadObjectManager(void)
{
	FILE* pFile;//�t�@�C���|�C���^��錾
	ObjectManager ObjectManager;
	int nCount = 0;

	//������
	int nCntModel, nCntTex, nCntMat;
	ObjectManager.ObjectNum = 0;
	for (nCntTex = 0; nCntTex < OBJECT_TEXTURE_MAX; nCntTex++)
	{//�e�N�X�`�����J��Ԃ�
		ObjectManager.apTexture[nCntTex] = NULL;
	}
	for (nCntModel = 0; nCntModel < OTYPE_MAX; nCntModel++)
	{//�ő吔��
		*ObjectManager.ObjectModel[nCntModel].Pass.aName = '\0';
		ObjectManager.ObjectModel[nCntModel].pMesh = NULL;
		ObjectManager.ObjectModel[nCntModel].pBuffMat = NULL;
		ObjectManager.ObjectModel[nCntModel].dwNumMat = 0;
		for (nCntMat = 0; nCntMat < OBJECT_SUBSET_MAX; nCntMat++)
		{//�}�e���A�����J��Ԃ�
			ObjectManager.ObjectModel[nCntModel].TexIdx[nCntMat] = -1;
		}
		ObjectManager.ObjectModel[nCntModel].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ObjectManager.ObjectModel[nCntModel].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ObjectManager.ObjectModel[nCntModel].nNumVtx = 0;
		ObjectManager.ObjectModel[nCntModel].sizeFVF = 0;
		ObjectManager.ObjectModel[nCntModel].pVtxBuff = NULL;
	}

	//X�t�@�C��
	pFile = fopen(OMANAGER_FILE, "r");//�t�@�C�����J��
	if (pFile != NULL)
	{//�J������
		if (fscanf(pFile, "%d", &ObjectManager.ObjectNum) == EOF)//���l����������
		{//�e�L�X�g���Ȃ�
			fclose(pFile);//�t�@�C�������
			return;
		}

		if (ObjectManager.ObjectNum > OTYPE_MAX)
		{//�^�C�v�ȏ�̃t�@�C����
			HWND hWnd;
			hWnd = GethWnd();
			MessageBox(hWnd, "���[�h�G���[", "X�t�@�C�������������܂�", MB_OK | MB_ICONERROR);
			PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
		}

		for (nCount = 0; nCount < ObjectManager.ObjectNum; nCount++)
		{//�w�萔��
			if (fscanf(pFile, "%63s", ObjectManager.ObjectModel[nCount].Pass.aName) == EOF)break;//���l����������
		}

		//�ݒ�
		SetObjectManager(ObjectManager);

		fclose(pFile);//�t�@�C�������
	}
	else
	{//�J���Ȃ�����
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "���[�h�G���[", "ObjectManager", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//---------------------------------------
//���[�h����
//---------------------------------------
void LoadObject(void)
{
	FILE* pFile;//�t�@�C���|�C���^��錾
	MODE Mode = GetMode();
	int nCount = 0;
	int nNumber = 0;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//X�t�@�C��
	switch (Mode)
	{
	case MODE_GAME:
		pFile = fopen(OBJECT_FILE, "r");//�t�@�C�����J��
		break;
	case MODE_TUTO:
		pFile = fopen(OBJECT_TUTO_FILE, "r");//�t�@�C�����J��
		break;
	default:
		pFile = fopen(nullptr, "r");//�t�@�C�����J��
		break;
	}

	if (pFile != NULL)
	{//�J������
		for (nCount = 0; nCount < OBJECT_MAX; nCount++)
		{//�ő吔��
			if (fscanf(pFile, "%d", &nNumber) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &pos.x) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &pos.y) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &pos.z) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &rot.x) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &rot.y) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &rot.z) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &scale.x) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &scale.y) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &scale.z) == EOF)break;//���l����������

			//�p�x�ϊ�
			rot = D3DXToRadian(rot);

			//�ُ�ԍ�
			switch (Mode)
			{
			case MODE_GAME:
				if (nNumber >= g_ObjectManager.ObjectNum - OTYPE_TUTO_MAX || nNumber < 0)
				{
					continue;
				}
				break;
			case MODE_TUTO:
				if (nNumber < g_ObjectManager.ObjectNum - OTYPE_TUTO_MAX || nNumber >= g_ObjectManager.ObjectNum)
				{
					continue;
				}
				break;
			}

			//�I�u�W�F�N�g�ݒu
			SetObject((OTYPE)nNumber, pos, rot, scale);
		}
		fclose(pFile);//�t�@�C�������
	}
	else
	{//�J���Ȃ�����
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "���[�h�G���[", "Object", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//---------------------------------------
//���[�h����
//---------------------------------------
void SaveObject(void)
{
	FILE* pFile;//�t�@�C���|�C���^��錾
	MODE Mode = GetMode();
	D3DXVECTOR3 rot;
	int nCount = 0;

	//X�t�@�C��
	switch (Mode)
	{
	case MODE_GAME:
		pFile = fopen(OBJECT_FILE, "w");//�t�@�C�����J��
		break;
	case MODE_TUTO:
		pFile = fopen(OBJECT_TUTO_FILE, "w");//�t�@�C�����J��
		break;
	default:
		pFile = fopen(nullptr, "w");//�t�@�C�����J��
		break;
	}

	if (pFile != NULL)
	{//�J������
		for (nCount = 0; nCount < OBJECT_MAX; nCount++)
		{//�ő吔��
			if (g_aObject[nCount].bUse)
			{//�g�p���Ă���
				//�p�x�ϊ�
				rot = D3DXToDegree(g_aObject[nCount].rot);

				fprintf(pFile, "%d\n", (int)g_aObject[nCount].Type);//���l����������
				fprintf(pFile, "%.1f ", g_aObject[nCount].pos.x);//���l����������
				fprintf(pFile, "%.1f ", g_aObject[nCount].pos.y);//���l����������
				fprintf(pFile, "%.1f\n", g_aObject[nCount].pos.z);//���l����������
				fprintf(pFile, "%.1f ", rot.x);//���l����������
				fprintf(pFile, "%.1f ", rot.y);//���l����������
				fprintf(pFile, "%.1f\n", rot.z);//���l����������
				fprintf(pFile, "%.1f ", g_aObject[nCount].scale.x);//���l����������
				fprintf(pFile, "%.1f ", g_aObject[nCount].scale.y);//���l����������
				fprintf(pFile, "%.1f\n", g_aObject[nCount].scale.z);//���l����������
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

//--------------------------
//�I�u�W�F�N�g�G�f�B�^�[
//--------------------------
void UpdateObjectEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot)
{
	Camera* pCamera = GetCamera();
	static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	static int HoldNumber = -1 , EndNumber = -1;
	MODE Mode = GetMode();

	if (GetKeyboardTrigger(DIK_RIGHT))
	{// �I�u�W�F�N�g�ؑ�
		g_Type = (OTYPE)(g_Type + 1);
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{// �I�u�W�F�N�g�ؑ�
		g_Type = (OTYPE)(g_Type - 1);
	}

	switch (Mode)
	{
	case MODE_GAME:
		//�͈͐���
		g_Type = (OTYPE)(g_Type % (g_ObjectManager.ObjectNum - OTYPE_TUTO_MAX));

		if (g_Type < 0)
		{//���[�v
			g_Type = (OTYPE)(g_ObjectManager.ObjectNum - 1 - OTYPE_TUTO_MAX);
		}
		break;
	case MODE_TUTO:
		//�͈͐���
		if (g_Type > g_ObjectManager.ObjectNum - 1)
		{//���[�v
			g_Type = (OTYPE)(g_ObjectManager.ObjectNum - OTYPE_TUTO_MAX);
		}

		if (g_Type < g_ObjectManager.ObjectNum - OTYPE_TUTO_MAX)
		{//���[�v
			g_Type = (OTYPE)(g_ObjectManager.ObjectNum - 1);
		}
		break;
	}

	if (GetKeyboardTrigger(DIK_UP))
	{// DEL�I�u�W�F�N�g�ؑ�
		g_DelObject++;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{// DEL�I�u�W�F�N�g�ؑ�
		g_DelObject--;
	}

	for (int nCount = 0; nCount < OBJECT_MAX; nCount++)
	{//�ő吔��

		//�͈͐���
		if (g_DelObject < 0)
		{
			g_DelObject = OBJECT_MAX - 1;
		}
		g_DelObject %= OBJECT_MAX;

		if (g_aObject[g_DelObject].bUse)
		{//�g���Ă���I�u�W�F�N�g�̑I��
			break;
		}
		else
		{//�g���Ă��Ȃ��ԍ��̓X�L�b�v

			if (GetKeyboardTrigger(DIK_DOWN))
			{// DEL�I�u�W�F�N�g�ؑ�
				g_DelObject--;
			}
			else
			{// DEL�I�u�W�F�N�g�ؑ�
				g_DelObject++;
			}
		}

		if (nCount == OBJECT_MAX - 1)
		{//�I�u�W�F�N�g���Ȃ�
			g_DelObject = -1;
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
		rot.x -= 0.01f;
	}
	if (GetKeyboardPress(DIK_I) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// �����Ă����
		rot.x += 0.01f;
	}
	if (GetKeyboardPress(DIK_J) || GetJoykeyPress(JOYKEY_LT, CONTROLLER_1))
	{// �����Ă����
		rot.y -= 0.01f;
	}
	if (GetKeyboardPress(DIK_K) || GetJoykeyPress(JOYKEY_RT, CONTROLLER_1))
	{// �����Ă����
		rot.y += 0.01f;
	}
	if (GetKeyboardPress(DIK_M) || GetJoykeyPress(JOYKEY_X, CONTROLLER_1))
	{// �����Ă����
		rot.z -= 0.01f;
	}
	if (GetKeyboardPress(DIK_COMMA) || GetJoykeyPress(JOYKEY_Y, CONTROLLER_1))
	{// �����Ă����
		rot.z += 0.01f;
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
	scale.x = max(OBJECT_SCALE_MIN, min(OBJECT_SCALE_MAX, scale.x));
	scale.y = max(OBJECT_SCALE_MIN, min(OBJECT_SCALE_MAX, scale.y));
	scale.z = max(OBJECT_SCALE_MIN, min(OBJECT_SCALE_MAX, scale.z));

	if (GetKeyboardTrigger(DIK_RSHIFT) || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1))
	{// �������Ƃ�
		HoldNumber = SetObject(g_Type, Epos + pos, Erot + rot, scale);
		EndNumber = HoldNumber;
	}
	if (GetKeyboardPress(DIK_RSHIFT) || GetJoykeyPress(JOYKEY_RB, CONTROLLER_1))
	{// �����Ă����
		g_aObject[HoldNumber].pos = Epos + pos;
		g_aObject[HoldNumber].rot = Erot + rot;
		g_aObject[HoldNumber].scale = scale;

		static OTYPE Type = OTYPE_ROKET; //�ꎞ�ۑ��p�i���o�[

		if (Type != g_Type)
		{//�i���o�[���ω�����
			//�I�u�W�F�N�g�̏���
			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				if (g_aObject[HoldNumber].nIdxShadow[nCnt] != -1)
				{
					NullShadow(g_aObject[HoldNumber].nIdxShadow[nCnt]);
				}
			}
			g_aObject[HoldNumber].bUse = false;
			//�I�u�W�F�N�g�̐؂�ւ�
			HoldNumber = SetObject(g_Type, Epos + pos, Erot + rot, scale);
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
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			if (g_aObject[EndNumber].nIdxShadow[nCnt] != -1)
			{
				NullShadow(g_aObject[EndNumber].nIdxShadow[nCnt]);
			}
		}
		g_aObject[EndNumber].bUse = false;
		EndNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_F8))
	{// �Z�[�u
		SaveObject();
	}

	if (GetKeyboardTrigger(DIK_DELETE) || (GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1) && GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1)))
	{// �������Ƃ�
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			if (g_aObject[g_DelObject].nIdxShadow[nCnt] != -1)
			{
				NullShadow(g_aObject[g_DelObject].nIdxShadow[nCnt]);
			}
		}
		g_aObject[g_DelObject].bUse = false;
	}

	static int DelNumber = -1; //�ꎞ�ۑ��p�i���o�[

	if (DelNumber != g_DelObject)
	{//�f���[�g�i���o�[���ω�����
		if (DelNumber != -1)
		{//�O��̃I�u�W�F�N�g�����݂���
			g_aObject[DelNumber].bAlpha = false;
		}
		DelNumber = g_DelObject; //�I���I�u�W�F�N�g�̍X�V
		if (DelNumber != -1)
		{//�I�u�W�F�N�g�����݂���
			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			g_aObject[DelNumber].bAlpha = true;
		}
	}
}

//--------------------
//���݂�X�t�@�C���i���o�[
//--------------------
int GetOnumber(void)
{
	return (int)g_Type;
}

//--------------------
//���݂̃f���[�g�i���o�[
//--------------------
int GetODelnumber(void)
{
	return g_DelObject;
}