//---------------------------------------
//
//回転による移動処理の定義・宣言[rotation.h]
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
// のように変換してsin,cosによってposを回転できる
// rotは数学の座標を基準にするため左回りが正方向である(DirectX)
// 回転する軸のrotに対し、他の軸のベクトルを2次元座標の正の方向とし回転する
// 2次元座標上のX軸に当たるベクトルはcosからsinに動いていき
// Y軸に当たるベクトルは-sinからcosに動く
// なお、D3DXMatrixRotation()とD3DXVec3TransformCoord()に置き換え可能
// この処理の進化系がOBBである
//
//--------------------------------------------

// 軸の列挙
typedef enum
{
	TYPE_X,
	TYPE_Y,
	TYPE_Z,
	SHAFT_MAX
}SHAFT;

[[nodiscard]] D3DXVECTOR3 RotationofShaft(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXVECTOR3& rot, SHAFT Shaft);         // sin,cos回転処理
D3DXVECTOR3 Rotation(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXVECTOR3& rot);                             // sin,cos回転処理
D3DXVECTOR3 RotationofShaftbyMatrix(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXVECTOR3& rot, SHAFT Shaft); // Matrix回転処理
D3DXVECTOR3 RotationbyMatrix(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXVECTOR3& rot);                     // Matrix回転処理
D3DXVECTOR3 RotationofMatrix(D3DXVECTOR3& Outpos, const D3DXVECTOR3& Inpos, const D3DXMATRIX& Mtx);                      // Matrixを渡す回転処理
#endif //_ROTATION_H_