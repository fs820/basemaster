//----------------------------------------
//
//�C���^���N�g����[interact.cpp]
//Author fuma sato
//
//----------------------------------------
#include"interact.h"
#include"stddxbuff.h"
#include"camera.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffInteract = NULL;                   //�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureInteract[INTERACT_TYPE_MAX] = { NULL }; //�e�N�X�`���̃|�C���^
Interact g_Interact;                                                 //����
//----------------------
//�|���S���̏���������
//----------------------
void InitInteract(void)
{
	//���_�o�b�t�@�̐���
	CHECK_HR(CreateVertex3D
	(
		&g_pVtxBuffInteract, // ���_�o�b�t�@�̃|�C���^
		VT_MAX, 1,           // ���_��,�o�b�t�@��
		D3DUSAGE_WRITEONLY,  // �g�p�@
		D3DPOOL_DEFAULT      // �v�[���̎��
	))

	//�e�N�X�`���o�b�t�@�̐���
	CHECK_HR(CreateTexture
	(
		g_pTextureInteract,   // �e�N�X�`���̃|�C���^
		INTERACT_TEX,         // �e�N�X�`���̃p�X
		INTERACT_TYPE_MAX,    // �e�N�X�`����
		nullptr,              // �摜���iNULL�Ȃ�擾���Ȃ��j
		D3DX_FILTER_TRIANGLE, // �t�B���^�����O
		D3DPOOL_DEFAULT       // �v�[���̎��
	))

	//���_�̐ݒ�
	CHECK_HR(SetVertex3D
	(
		&g_pVtxBuffInteract,                // ���_�o�b�t�@�̃|�C���^
		0,                                  // ���b�N�t���O
		0, 1,                               // �o�b�t�@�̊J�n�I�t�Z�b�g,�o�b�t�@��
		1, 1,                               // y������, x������
		1.0f, 1.0f,                         // ��,����
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  // �F
		1, nullptr,                         // �e�N�X�`����,�摜���(null�̎��摜�̓|���S���̑傫���ɒ��߂����)
		false                               // �Z���^�[�|�W�V����
	))

	//���̂̏�����
	g_Interact.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Interact.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Interact.Type = INTERACT_TYPE_KEY;
	g_Interact.bUse = false;
}

//-------------------
//�|���S���I������
//-------------------
void UninitInteract(void)
{
	g_Interact.bUse = false;

	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < INTERACT_TYPE_MAX; nCntTex++)
	{
		SAFE_RELEASE(g_pTextureInteract[nCntTex]);
	}
	//���_�o�b�t�@�̔j��
	SAFE_RELEASE(g_pVtxBuffInteract);
}

//-------------------
//�|���S���X�V����
//-------------------
void UpdateInteract(void)
{

}

//-------------------
//�|���S���`�揈��
//-------------------
void DrawInteract(void)
{
	RSOP rsOp, rsOpOld;                 //�����_�[�X�e�[�g�̍\����
	CHECK_HR(GetRenderOperation(&rsOp)) //�����_�[�X�e�[�g�̎擾
	rsOpOld = rsOp;                     //���݂̃����_�[�X�e�[�g

	//�����_�[�X�e�[�g�̕ύX
	rsOp.Lighting = FALSE; //���C�g�𖳌��ɂ���
	rsOp.zEnable = TRUE;  //Z�e�X�g��L���ɂ���
	rsOp.zWriteEnable = FALSE; //Z�������݂𖳌��ɂ���
	rsOp.zFunc = D3DCMP_ALWAYS; //Z�e�X�g�̔�r�֐�����ɒʂ�

	//�����_�[�X�e�[�g�̐ݒ�
	CHECK_HR(SetRenderOperation(&rsOp))

	if (g_Interact.bUse)
	{//�g���Ă���
		//�`��
		CHECK_HR(DrawVertex3D
		(
			&g_pVtxBuffInteract, nullptr, &g_pTextureInteract[(unsigned int)g_Interact.Type], //�o�b�t�@(���_,�C���f�b�N�X,�e�N�X�`��)
			&g_Interact.pos, nullptr, &g_Interact.scale, &g_Interact.mtxWorld,                //�ʒu,��],�傫��,���[���h�}�g���b�N�X
			BILLBOARD_FULL, 0.0f,                                                             //�r���{�[�h�̃^�C�v�AZ�o�C�A�X�l
			D3DPT_TRIANGLESTRIP,                                                              //�`��̃^�C�v
			VT_MAX, 0, 2                                                                      //���_���A�J�n�C���f�b�N�X�A�|���S����
		))
	}

	CHECK_HR(SetRenderOperation(&rsOpOld)) //�����_�[�X�e�[�g��߂�
}

//----------
//�ݒ�
//----------
void SetInteract(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	g_Interact.pos = pos;      //�ʒu
	g_Interact.pos.y += 50.0f; //�����グ��
	g_Interact.scale = scale;  //�傫��
	g_Interact.bUse = true;    //�g�p
}

//----------------------
//�j��
//----------------------
void EndInteract(void)
{
	g_Interact.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ʒu
	g_Interact.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //�傫��
	g_Interact.bUse = false;                          //�g�p
}

//---------------------
//�^�C�v�ύX
//---------------------
void SetInteractType(INTERACT_TYPE Type)
{
	g_Interact.Type = Type;
}