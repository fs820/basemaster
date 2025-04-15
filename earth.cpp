//----------------------------------------
//
//�n������[earth.cpp]
//Author fuma sato
//
//----------------------------------------

#include"earth.h"
#include"stddxbuff.h"
#include "camera.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEarth = NULL;//�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEarth = NULL;     //�e�N�X�`���̃|�C���^
Earth g_Earth;                                 //����

//----------------------
//����������
//----------------------
void InitEarth(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_DEFAULT,
		&g_pVtxBuffEarth,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFileEx
	(
		pDevice,
		EARTH_TEX,
		D3DX_DEFAULT,              // �e�N�X�`���̕��iD3DX_DEFAULT�Ȃ玩�������j
		D3DX_DEFAULT,              // �e�N�X�`���̍����iD3DX_DEFAULT�Ȃ玩�������j
		D3DX_DEFAULT,              // �~�b�v�}�b�v���x���iD3DX_DEFAULT�Ŏ����ݒ�j
		0,                         // �g�p���Ȃ��ꍇ��0�i���I�e�N�X�`���Ȃ�D3DUSAGE_DYNAMIC�j
		D3DFMT_A8R8G8B8,           // �t�H�[�}�b�g�iA8R8G8B8 png X8R8G8B8 jpg�j
		D3DPOOL_DEFAULT,           // �������v�[���i�ʏ�� D3DPOOL_MANAGED�j
		D3DX_FILTER_TRIANGLE,      // �摜�̊g�k�t�B���^�iD3DX_FILTER_LINEAR �Ȃǁj
		D3DX_FILTER_TRIANGLE,      // �~�b�v�}�b�v�̃t�B���^
		0,                         // �J���[�L�[�i�����ɂ������F�j
		NULL,                      // `D3DXIMAGE_INFO`�i�摜�����擾����ꍇ�j
		NULL,                      // �p���b�g�f�[�^�i�p���b�g�Ȃ��Ȃ�NULL�j
		&g_pTextureEarth
	);

	g_Earth.bUse = false;

	//���_�̐ݒ�
	SetVertex3DYX
	(
		&g_pVtxBuffEarth,
		0,
		1,
		1,
		1,
		EARTH_WIDTH,
		EARTH_HEIGHT,
		D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		EARTH_WIDTH,
		EARTH_HEIGHT,
		true, false
	);
}

//-------------------
//�|���S���I������
//-------------------
void UninitEarth(void)
{
	g_Earth.bUse = false;

	//�e�N�X�`���̔j��
	if (g_pTextureEarth != NULL)
	{
		g_pTextureEarth->Release();
		g_pTextureEarth = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEarth != NULL)
	{
		g_pVtxBuffEarth->Release();
		g_pVtxBuffEarth = NULL;
	}
}

//-------------------
//�|���S���X�V����
//-------------------
void UpdateEarth(void)
{

}

//-------------------
//�|���S���`�揈��
//-------------------
void DrawEarth(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	D3DXMATRIX mtxTrans;//�v�Z�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	if (g_Earth.bUse)
	{
		//�}�g���b�N�X������
		D3DXMatrixIdentity(&g_Earth.mtxWorld);

		//�r���[�}�g���b�N�X�擾
		D3DXMATRIX mtxView;
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//�J�����̋t�s���ݒ�
		g_Earth.mtxWorld._11 = mtxView._11;
		g_Earth.mtxWorld._12 = mtxView._21;
		g_Earth.mtxWorld._13 = mtxView._31;
		g_Earth.mtxWorld._21 = mtxView._12;
		g_Earth.mtxWorld._22 = mtxView._22;
		g_Earth.mtxWorld._23 = mtxView._32;
		g_Earth.mtxWorld._31 = mtxView._13;
		g_Earth.mtxWorld._32 = mtxView._23;
		g_Earth.mtxWorld._33 = mtxView._33;

		//�ʒu�̌v�Z
		extern  Camera g_camera[1];
		D3DXVECTOR3 Pos;
		SpherePos(Pos, g_camera[0].posV, D3DXVECTOR3(D3DX_PI * 0.3f, D3DX_PI, 0.0f), 10000.0f);
		D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);
		D3DXMatrixMultiply(&g_Earth.mtxWorld, &g_Earth.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Earth.mtxWorld);

		//���_�o�b�t�@
		pDevice->SetStreamSource(0, g_pVtxBuffEarth, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureEarth);

		//�|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			0,//�n�܂�̔ԍ�
			2//�|���S���̌�
		);
	}

	// Z�e�X�g��߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//----------
//�ݒ�
//----------
void SetEarth(void)
{
	if (!g_Earth.bUse)
	{
		g_Earth.bUse = true;
	}
}