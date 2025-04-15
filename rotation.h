//---------------------------------------
//
//��]�ɂ��ړ������̒�`�E�錾[rotation.h]
//Author fuma sato
//
//---------------------------------------
#ifndef _ROTATION_H_
#define _ROTATION_H_
#include "main.h"

//-------------------------------------------
//
// Memo
// D3DX_PI * 0.0f =  sinf()
//           0.5f =  cosf()
//           1.0f = -sinf()
//          -0.5f = -cosf()
// �̂悤�ɕϊ�����sin,cos�ɂ����pos����]�ł���
// rot�͐��w�̍��W����ɂ��邽�ߍ���肪�������ł���(DirectX)
// ��]���鎲��rot�ɑ΂��A���̎��̃x�N�g����2�������W�̐��̕����Ƃ���]����
// 2�������W���X���ɓ�����x�N�g����cos����sin�ɓ����Ă���
// Y���ɓ�����x�N�g����-sin����cos�ɓ���
// �Ȃ��AD3DXMatrixRotation()��D3DXVec3TransformCoord()�ɒu�������\
// ���̏����̐i���n��OBB�ł���
//
//--------------------------------------------

// ���̗�
typedef enum
{
	TYPE_X,
	TYPE_Y,
	TYPE_Z,
	SHAFT_MAX
}SHAFT;

[[nodiscard]] D3DXVECTOR3 RotationofShaft(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXVECTOR3& rot, SHAFT Shaft);         // sin,cos��]����
D3DXVECTOR3 Rotation(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXVECTOR3& rot);                             // sin,cos��]����
D3DXVECTOR3 RotationofShaftbyMatrix(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXVECTOR3& rot, SHAFT Shaft); // Matrix��]����
D3DXVECTOR3 RotationbyMatrix(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXVECTOR3& rot);                     // Matrix��]����
D3DXVECTOR3 RotationofMatrix(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXMATRIX& Mtx);                      // Matrix��n����]����
#endif //_ROTATION_H_