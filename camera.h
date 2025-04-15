//---------------------------------------
//
//�J�����̒�`�E�錾[camera.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include"main.h"

#define CAMERA_XNUM (1) //�J�����̐�X
#define CAMERA_YNUM (1) //�J�����̐�Y
#define CAMERA_VIEW_SPEED (0.01f)//��]�X�s�[�h
#define CAMERA_ROT_SPEED (0.05f)//��]�X�s�[�h
#define CAMERA_SPEED (2.0f)//�ړ��X�s�[�h
#define CAMERA_INA (0.05f)//�Ǐ]�{��
#define CAMERA_ROTX (0.2f)//�c��]�͈�
#define CAMERA_ROTX_GRA (0.01f)//�c��]�n�ʃX�y�[�X
#define CAMERA_DISTANCE_SPEED (5.0f)
#define CAMERA_DISTANCE_MIN (5.0f)//�Y�[������
#define CAMERA_DISTANCE_MAX (100.0f)//�Y�[������
#define CAMERA_DISTANCE_VIEW_MAX (1000.0f)
#define CAMERA_WIDTH (90.0f)//����̍L��
#define CAMERA_MIN (1.0f)//����̍ŏ�
#define CAMERA_MAX (1000.0f)//����̍ő�

//�J�����\����
typedef struct
{
	D3DXVECTOR3 posV;
	D3DXVECTOR3 posVDest;
	D3DXVECTOR3 posR;
	D3DXVECTOR3 posRDest;
	D3DXVECTOR3 posU;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	float fDistance;
	float fDistanceMin;
	float fDistanceMax;
	D3DVIEWPORT9 viewport;
}Camera;

void InitCamera(void);//����������
void UninitCamera(void);//�I������
void UpdateCamera(void);//�X�V����
void SetCamera(int nNumber);//�`�揈��
Camera* GetCamera(void);
void ViewCamera(int nNumber);
void GameCamera(int nNumber);

HRESULT DeptSort(int* nIdx, D3DXVECTOR3* pPos, int IdxNum);	// �������C���f�b�N�X�쐬�֐�
bool DrawCameraCollision(D3DXVECTOR3 pos);		            // �J�������猩�Ă��������Ă��邩
#endif // _CAMERA_H_