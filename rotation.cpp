//---------------------------------------
//
//回転による移動処理[rotation.cpp]
//Author fuma sato
//
//---------------------------------------
#include "rotation.h"

//------------------------------
// sin,cos回転処理
//------------------------------
D3DXVECTOR3 RotationofShaft(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXVECTOR3& rot, SHAFT Shaft)
{
	switch (Shaft)
	{
	case TYPE_X:
		Outpos = 
			D3DXVECTOR3
			(
				Inpos.x,
				cosf(rot.x) * Inpos.y + sinf(rot.x) * Inpos.z,
				-sinf(rot.x) * Inpos.y + cosf(rot.x) * Inpos.z
			);
		break;
	case TYPE_Y:
		Outpos = 
			D3DXVECTOR3
			(
				cosf(rot.y) * Inpos.x + -sinf(rot.y) * Inpos.z,
				Inpos.y,
				sinf(rot.y) * Inpos.x + cosf(rot.y) * Inpos.z
			);
		break;
	case TYPE_Z:
		Outpos = 
			D3DXVECTOR3
			(
				cosf(rot.z) * Inpos.x + sinf(rot.z) * Inpos.y,
				-sinf(rot.z) * Inpos.x + cosf(rot.z) * Inpos.y,
				Inpos.z
			);
		break;
	}

	return Outpos;
}

//------------------------------
// sin,cos回転処理
//------------------------------
D3DXVECTOR3 Rotation(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXVECTOR3& rot)
{
	Outpos =
		D3DXVECTOR3
		(
			cosf(rot.y) * Inpos.x + -sinf(rot.y) * Inpos.z,
			Inpos.y,
			sinf(rot.y) * Inpos.x + cosf(rot.y) * Inpos.z
		);
	Outpos =
		D3DXVECTOR3
		(
			Outpos.x,
			cosf(rot.x) * Outpos.y + sinf(rot.x) * Outpos.z,
			-sinf(rot.x) * Outpos.y + cosf(rot.x) * Outpos.z
		);
	Outpos =
		D3DXVECTOR3
		(
			cosf(rot.z) * Outpos.x + sinf(rot.z) * Outpos.y,
			-sinf(rot.z) * Outpos.x + cosf(rot.z) * Outpos.y,
			Outpos.z
		);

	return Outpos;
}

//------------------------------
// マトリックスを使った回転処理
//------------------------------
D3DXVECTOR3 RotationofShaftbyMatrix(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXVECTOR3& rot, SHAFT Shaft)
{
	D3DXMATRIX Mtx;
	switch (Shaft)
	{
	case TYPE_X:
		D3DXMatrixRotationX(&Mtx, rot.x);
		break;
	case TYPE_Y:
		D3DXMatrixRotationY(&Mtx, rot.y);
		break;
	case TYPE_Z:
		D3DXMatrixRotationZ(&Mtx, rot.z);
		break;
	}
	D3DXVec3TransformCoord(&Outpos, &Inpos, &Mtx);

	return Outpos;
}

//------------------------------
// マトリックスを使った回転処理
//------------------------------
D3DXVECTOR3 RotationbyMatrix(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXVECTOR3& rot)
{
	D3DXMATRIX MtxX, MtxY, MtxZ, Mtx;

	D3DXMatrixRotationY(&MtxY, rot.y);
	D3DXMatrixRotationX(&MtxX, rot.x);
	D3DXMatrixRotationZ(&MtxZ, rot.z);

	Mtx = MtxY * MtxX * MtxZ;

	D3DXVec3TransformCoord(&Outpos, &Inpos, &Mtx);

	return Outpos;
}

//------------------------------
// マトリックスを渡す回転処理
//------------------------------
D3DXVECTOR3 RotationofMatrix(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXMATRIX& Mtx)
{
	D3DXVec3TransformCoord(&Outpos, &Inpos, &Mtx);
	return Outpos;
}