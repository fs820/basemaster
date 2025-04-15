//---------------------------------------
//
//���C�g����[light.cpp]
//Author fuma sato
//
//---------------------------------------
#include "light.h"

//�O���[�o���ϐ��錾
LIGHT g_light[LIGHT_MAX];	//���C�g���

//*****************************************
//���C�g�̏�����
//*****************************************
void InitLight(void)
{
	//// ���C�g���N���A����
	//ZeroMemory(g_light, sizeof(g_light));

	//// ���C�g�̎�ނ�ݒ�
	//g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

	//// ���C�g�̊g�U����ݒ�
	//g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ���K������(�傫���P�̃x�N�g���ɂ���)
	//D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);
	//g_light[nCntLight].Direction = vecDir[nCntLight];

	//// ���C�g��ݒ肷��
	//pDevice->SetLight(nCntLight, &g_light[nCntLight]);

	//// ���C�g��L���ɂ���
	//pDevice->LightEnable(nCntLight, TRUE);

	////���
	//g_light[3].Type = D3DLIGHT_POINT;
	////�g�U��
	//g_light[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
	////����
	//g_light[3].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
	////���ʔ��ˌ�
	//g_light[3].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
	////�ʒu
	//g_light[3].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	////�͈�
	//g_light[3].Range = 0.0f;
	////����������(���)
	//g_light[3].Attenuation0 = 0.1f;
	////����������(����)
	//g_light[3].Attenuation1 = 0.0f;
	////����������(����*2)
	//g_light[3].Attenuation2 = 0.0f;
	////���C�g�Z�b�g
	//pDevice->SetLight(3, &g_light[3]);
	////�L����
	//pDevice->LightEnable(3, FALSE);

	////���
	//g_light[nCnt].Type = D3DLIGHT_SPOT;
	////�g�U��
	//g_light[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	////����
	//g_light[nCnt].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	////���ʔ��ˌ�
	//g_light[nCnt].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	////�ʒu
	//g_light[nCnt].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	////����
	//vecDir[nCnt] = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	////���K��
	//D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
	//g_light[nCnt].Direction = vecDir[nCnt];
	////�͈�
	//g_light[nCnt].Range = 0.0f;
	////�R�[��������
	//g_light[nCnt].Falloff = 0.01f;
	////����������
	//g_light[nCnt].Attenuation0 = 0.1f;
	////�R�[���p�x(��)
	//g_light[nCnt].Theta = D3DXToRadian(90.0f);
	////�R�[���p�x(�O)
	//g_light[nCnt].Phi = D3DXToRadian(360.0f);
	////���C�g�Z�b�g
	//pDevice->SetLight(nCnt, &g_light[nCnt]);
	////�L����
	//pDevice->LightEnable(nCnt, FALSE);
}

//*****************************************
//���C�g�̏I��
//*****************************************
void UninitLight(void)
{
	
}

//*****************************************
//���C�g�̍X�V
//*****************************************
void UpdateLight(void)
{

}

//*****************************************
//���C�g�̎擾
//*****************************************
LIGHT* GetLight(void)
{
	return &g_light[0];
}

//--------------------------
//�Z�b�g
//--------------------------
unsigned int SetLight(D3DLIGHT9* pLight)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (unsigned int nCntLight = 0; nCntLight < LIGHT_MAX; nCntLight++)
	{
		if (g_light[nCntLight].bUse == false)
		{// �g�p����Ă��Ȃ�
			//���C�g�Z�b�g
			CHECK_HR(pDevice->SetLight(nCntLight, pLight))

			//�L����
			CHECK_HR(pDevice->LightEnable(nCntLight, TRUE))

			// ���C�g����ۑ�
			g_light[nCntLight].light = *pLight;

			// �g�p�t���O�𗧂Ă�
			g_light[nCntLight].bUse = true;

			return nCntLight;
		}
	}

	return -1; // �g�p����Ă��Ȃ����C�g���Ȃ�
}

//--------------------------
//�j��
//--------------------------
void EndLight(unsigned int nIdx)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���C�g�𖳌��ɂ���
	CHECK_HR(pDevice->LightEnable(nIdx, FALSE))

	//�g�p�t���O�����낷
	g_light[nIdx].bUse = false;
}