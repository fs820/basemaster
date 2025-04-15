//---------------------------------------
//
//�A�^�b�N�]�[������[AttackZone.cpp]
//Author fuma sato
//
//---------------------------------------
#include "AttackZone.h"
#include "camera.h"
#include "stddx.h"
#include "light.h"

AttackZone g_aAttackZone[ATTACK_ZONE_MAX];	// �A�^�b�N�]�[�����
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAttackZone = NULL; //�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureAttackZone = NULL;      //�e�N�X�`���̃|�C���^
//******************************************************
// �A�^�b�N�]�[��������
//******************************************************
void InitAttackZone(void)
{
	for (int nCntAttackZone = 0; nCntAttackZone < ATTACK_ZONE_MAX; nCntAttackZone++)
	{// �ő�A�^�b�N�]�[������
		// �e��ϐ��̏�����
		g_aAttackZone[nCntAttackZone].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	    // �ʒu
		g_aAttackZone[nCntAttackZone].fRadius = 0.0f;						    // ���a
		g_aAttackZone[nCntAttackZone].bLess = 0.0f;						        // ���a
		g_aAttackZone[nCntAttackZone].bUse = false;						        // �g�p
		g_aAttackZone[nCntAttackZone].col = D3DXCOLOR(0.3f, 0.0f, 0.0f, 0.1f);	// �F
		g_aAttackZone[nCntAttackZone].idexLight = -1;					        // ���C�g�̃C���f�b�N�X
	}

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//���_�o�b�t�@�̐���
	CHECK_HR(CreateVertex3D
	(
		&g_pVtxBuffAttackZone,   // ���_�o�b�t�@�̃|�C���^
		VT_MAX, ATTACK_ZONE_MAX, // ���_��,�o�b�t�@��
		D3DUSAGE_WRITEONLY,      // �g�p�@
		D3DPOOL_DEFAULT          // �v�[���̎��
	))

	//�e�N�X�`���̓ǂݍ���
	CHECK_HR(CreateTexture
	(
		&g_pTextureAttackZone,  // �e�N�X�`���̃|�C���^
		&ATTACK_ZONE_TEX,       // �e�N�X�`���̃p�X
		1,                      // �e�N�X�`����
		nullptr,                // �摜���iNULL�Ȃ�擾���Ȃ��j
		D3DX_FILTER_TRIANGLE,   // �t�B���^�����O
		D3DPOOL_DEFAULT         // �v�[���̎��
	))

	//���_�̐ݒ�
	CHECK_HR(SetVertex3D
	(
		&g_pVtxBuffAttackZone,                                // ���_�o�b�t�@�̃|�C���^
		0,                                                    // ���b�N�t���O
		0, ATTACK_ZONE_MAX,                                   // �o�b�t�@�̊J�n�I�t�Z�b�g,�o�b�t�@��
		1, 1,                                                 // y������, x������
		1.0f, 1.0f,                                           // ��,����
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),                    // �F
		1, nullptr,                                           // �e�N�X�`����,�摜���(null�̎��摜�̓|���S���̑傫���ɒ��߂����)
		true											      // �Z���^�[�|�W�V����
	))
}

//-----------------------------------------------------
//�I������
//-----------------------------------------------------
void UninitAttackZone(void)
{
	for (int nCntAttackZone = 0; nCntAttackZone < ATTACK_ZONE_MAX; nCntAttackZone++)
	{// �ő�A�^�b�N�]�[������
		EndAttackZone(nCntAttackZone); // �A�^�b�N�]�[���̏I��
	}

	SAFE_RELEASE(g_pTextureAttackZone); // �e�N�X�`���̔j��

	SAFE_RELEASE(g_pVtxBuffAttackZone); // ���_�o�b�t�@�̔j��
}

//------------------------------------------------------
//�`�揈��
//------------------------------------------------------
void DrawAttackZone(void)
{
	RSOP rsop, rsOpOld;                 // �����_�[�X�e�[�g
	CHECK_HR(GetRenderOperation(&rsop)) // �����_�[�X�e�[�g�̎擾
	rsOpOld = rsop;                     // ���݂̃����_�[�X�e�[�g

	// �����_�[�X�e�[�g�̕ύX
	rsop.Lighting = FALSE;             // ���C�g�𖳌��ɂ���
	rsop.zEnable = TRUE;               // Z�e�X�g��L���ɂ���
	rsop.zWriteEnable = FALSE;         // Z�o�b�t�@��������
	rsop.zFunc = D3DCMP_LESSEQUAL;     // Z�o�b�t�@��r�֐�
	rsop.blendOp = D3DBLENDOP_ADD;     // �u�����h���Z
	rsop.srcBlend = D3DBLEND_SRCALPHA; // �\�[�X�u�����h
	rsop.destBlend = D3DBLEND_ONE;     // �f�X�e�B�l�[�V�����u�����h
	rsop.alphaTestEnable = TRUE;       // �A���t�@�e�X�g��L���ɂ���
	rsop.alphaRef = 0;                 // �A���t�@�e�X�g�Q�ƒl
	rsop.alphaFunc = D3DCMP_GREATER;   // �A���t�@�e�X�g��r�֐�

	CHECK_HR(SetRenderOperation(&rsop)) // �����_�[�X�e�[�g�̕ύX

	// �����ق�����`�悷��
	static int nIdx[ATTACK_ZONE_MAX]; // �������i�[�p
	static int nLimit = 0;
	if (nLimit % 60 == 0)
	{// �t���[�����Ƃɏ��� (���t���[�����Əd���Ȃ�\�������邽��)
		D3DXVECTOR3 PosList[ATTACK_ZONE_MAX]; // �|�X���X�g
		Camera* pCamera = GetCamera();
		for (int nCnt = 0; nCnt < ATTACK_ZONE_MAX; nCnt++)
		{// �|�X���X�g�쐬
			if (g_aAttackZone[nCnt].bUse)
			{
				PosList[nCnt] = g_aAttackZone[nCnt].pos;
			}
			else
			{
				PosList[nCnt] = pCamera->posV;
			}
		}
		CHECK_HR(DeptSort(nIdx, PosList, ATTACK_ZONE_MAX)) //�������Ƀ\�[�g
	}
	nLimit++;

	for (int nCntAttackZone = 0; nCntAttackZone < ATTACK_ZONE_MAX; nCntAttackZone++)
	{
		if (g_aAttackZone[nIdx[nCntAttackZone]].bUse == false)
		{// �g���Ă��Ȃ����̂��o�Ă�����I��
			break;
		}

		CHECK_HR(GetRenderOperation(&rsop)) // �����_�[�X�e�[�g�̎擾

		if (g_aAttackZone[nIdx[nCntAttackZone]].bLess)
		{
			// �����_�[�X�e�[�g�̕ύX
			rsop.blendOp = D3DBLENDOP_REVSUBTRACT;     // �u�����h���Z
		}
		else
		{
			// �����_�[�X�e�[�g�̕ύX
			rsop.blendOp = D3DBLENDOP_ADD;     // �u�����h���Z
		}

		CHECK_HR(SetRenderOperation(&rsop)) // �����_�[�X�e�[�g�̕ύX

		//�`��
		CHECK_HR(DrawVertex3D
		(
			&g_pVtxBuffAttackZone, nullptr, &g_pTextureAttackZone,                                                     //�o�b�t�@(���_,�C���f�b�N�X,�e�N�X�`��)
			&g_aAttackZone[nIdx[nCntAttackZone]].pos, nullptr, nullptr, &g_aAttackZone[nIdx[nCntAttackZone]].mtxWorld, //�ʒu,��],�傫��,���[���h�}�g���b�N�X
			BILLBOARD_XZ_AXIS, 0.0001f,                                                                                //�r���{�[�h�̃^�C�v�AZ�o�C�A�X�l
			D3DPT_TRIANGLESTRIP,                                                                                       //�`��̃^�C�v
			VT_MAX, nIdx[nCntAttackZone], 2                                                                            //���_���A�J�n�C���f�b�N�X�A�|���S����
		))
	}

	CHECK_HR(SetRenderOperation(&rsOpOld)) // �����_�[�X�e�[�g��߂�
}

//******************************************************
// �A�^�b�N�]�[���ݒ�
//******************************************************
int SetAttackZone(D3DXVECTOR3 pos, float fRadius, D3DXCOLOR col, bool Less)
{
	for (int nCntAttackZone = 0; nCntAttackZone < ATTACK_ZONE_MAX; nCntAttackZone++)
	{// �ő�A�^�b�N�]�[������
		if (g_aAttackZone[nCntAttackZone].bUse == false)
		{// �g���Ă��Ȃ��A�^�b�N�]�[��
			g_aAttackZone[nCntAttackZone].pos = pos;		    // �ʒu
			g_aAttackZone[nCntAttackZone].fRadius = fRadius;	// ���a
			g_aAttackZone[nCntAttackZone].col = col;	        // ���a
			g_aAttackZone[nCntAttackZone].bLess = Less;		// ���Z
			g_aAttackZone[nCntAttackZone].bUse = true;		// �g�p

			//���_�̐ݒ�
			CHECK_HR(SetVertex3D
			(
				&g_pVtxBuffAttackZone,
				0,
				nCntAttackZone, 1,
				1, 1,
				g_aAttackZone[nCntAttackZone].fRadius * 2.0f, g_aAttackZone[nCntAttackZone].fRadius * 2.0f,
				g_aAttackZone[nCntAttackZone].col,
				1, nullptr,
				true
			))

			D3DLIGHT9 light;
			//���
			light.Type = D3DLIGHT_SPOT;
			//�g�U��
			light.Diffuse = g_aAttackZone[nCntAttackZone].col * 20.0f;
			//����
			light.Ambient = g_aAttackZone[nCntAttackZone].col * 20.0f;
			//���ʔ��ˌ�
			light.Specular = g_aAttackZone[nCntAttackZone].col * 20.0f;
			//�ʒu
			light.Position = g_aAttackZone[nCntAttackZone].pos + D3DXVECTOR3(0.0f, g_aAttackZone[nCntAttackZone].fRadius * 0.5f, 0.0f);
			//����
			light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			//�͈�
			light.Range = g_aAttackZone[nCntAttackZone].fRadius;
			//�R�[��������
			light.Falloff = 0.01f;
			//����������
			light.Attenuation0 = 0.1f;
			//�R�[���p�x(��)
			light.Theta = D3DXToRadian(90.0f);
			//�R�[���p�x(�O)
			light.Phi = D3DXToRadian(360.0f);

			g_aAttackZone[nCntAttackZone].idexLight = SetLight(&light);
			return nCntAttackZone;
		}
	}
	return -1;
}

//******************************************************
// �A�^�b�N�]�[���j��
//******************************************************
void EndAttackZone(int nIdx)
{
	EndLight(g_aAttackZone[nIdx].idexLight); // ���C�g�̏I��
	g_aAttackZone[nIdx].idexLight = -1;	     // ���C�g�̃C���f�b�N�X
	g_aAttackZone[nIdx].bUse = false;		 // �g�p
}