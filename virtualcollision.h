//---------------------------------------
//
//仮想的当たり判定処理の定義・宣言[virtualcollision.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _VIRTUALCOLLISION_H_
#define _VIRTUALCOLLISION_H_

#include"main.h"

void SetVirtualCollisionBox(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, float Depth, float Corner, bool bInside);          //四角
void SetVirtualCollisionCircle(D3DXVECTOR3 pos, float Radius, float Height, int Division, float LostAngle, float LostRange, bool bInside);  //円
#endif //_VIRTUALCOLLISION_H_