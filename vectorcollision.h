//---------------------------------------
//
//ベクトルの当たり判定処理の定義・宣言[vectorcollision.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _VECTORCOLLISION_H_
#define _VECTORCOLLISION_H_

#include"main.h"

void VectorCollision(D3DXVECTOR3& Movingpos, D3DXVECTOR3& MovingposOld, float MovingWidth, float MovingHeight, D3DXVECTOR3& StaticStartpos, D3DXVECTOR3& StaticEndpos, float StaticHeight);  //当たり判定
_NODISCARD bool VectorTrigger(D3DXVECTOR3& Movingpos, D3DXVECTOR3& MovingposOld, float MovingWidth, float MovingHeight, D3DXVECTOR3& StaticStartpos, D3DXVECTOR3& StaticEndpos, float StaticHeight);    //当たり判定
void VectorReflection(D3DXVECTOR3& Movingpos, D3DXVECTOR3& MovingposOld, float MovingWidth, float MovingHeight, D3DXVECTOR3& StaticStartpos, D3DXVECTOR3& StaticEndpos, float StaticHeight); //当たり判定
bool VectorInside(D3DXVECTOR3& Movingpos, float MovingWidth, D3DXVECTOR3* pPos); //当たり判定
#endif //_VECTORCOLLISION_H_