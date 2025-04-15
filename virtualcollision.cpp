//---------------------------------------
//
//仮想的当たり判定処理[virtualcollision.cpp]
//Author fuma sato
//
//---------------------------------------
#include"virtualcollision.h"
#include"wall.h"

//---------------
//四角
//---------------
void SetVirtualCollisionBox(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, float Depth, float Corner, bool bInside)
{
	//バリアの生成
	SetWall(pos + D3DXVECTOR3(sinf(rot.y) * Depth * 0.5f, 0.0f, cosf(rot.y) * Depth * 0.5f), rot - D3DXVECTOR3(0.0f, D3DX_PI * bInside, 0.0f), Width * (1.0f - Corner), Height, WTYPE_INVISIBLE);
	SetWall(pos + D3DXVECTOR3(sinf(rot.y + D3DX_PI * 0.5f) * Width * 0.5f, 0.0f, cosf(rot.y + D3DX_PI * 0.5f) * Width * 0.5f), rot + D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f) - D3DXVECTOR3(0.0f, D3DX_PI * bInside, 0.0f), Depth * (1.0f - Corner), Height, WTYPE_INVISIBLE);
	SetWall(pos + D3DXVECTOR3(sinf(rot.y + D3DX_PI) * Depth * 0.5f, 0.0f, cosf(rot.y + D3DX_PI) * Depth * 0.5f), rot + D3DXVECTOR3(0.0f, D3DX_PI, 0.0f) - D3DXVECTOR3(0.0f, D3DX_PI * bInside, 0.0f), Width * (1.0f - Corner), Height, WTYPE_INVISIBLE);
	SetWall(pos + D3DXVECTOR3(sinf(rot.y + D3DX_PI * -0.5f) * Width * 0.5f, 0.0f, cosf(rot.y + D3DX_PI * -0.5f) * Width * 0.5f), rot + D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f) - D3DXVECTOR3(0.0f, D3DX_PI * bInside, 0.0f), Depth * (1.0f - Corner), Height, WTYPE_INVISIBLE);

	//角のバリアの生成
	D3DXVECTOR3 aPos[2] = { D3DXVECTOR3(0.0f,0.0f,0.0f) }, SenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), Space = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fAngle = 0.0f, fLong = 0.0f;

	aPos[0] = pos + D3DXVECTOR3(sinf(rot.y) * Depth * 0.5f, 0.0f, cosf(rot.y) * Depth * 0.5f) + D3DXVECTOR3(sinf(rot.y + D3DX_PI * 0.5f) * Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(rot.y + D3DX_PI * 0.5f) * Width * (1.0f - Corner) * 0.5f);
	aPos[1] = pos + D3DXVECTOR3(sinf(rot.y + D3DX_PI * 0.5f) * Width * 0.5f, 0.0f, cosf(rot.y + D3DX_PI * 0.5f) * Width * 0.5f) + D3DXVECTOR3(sinf(rot.y) * Depth * (1.0f - Corner) * 0.5f, 0.0f, cosf(rot.y) * Depth * (1.0f - Corner) * 0.5f);
	Space = aPos[1] - aPos[0];
	SenterPos = aPos[0] + Space * 0.5f;
	fAngle = atan2f(Space.x, Space.z) + D3DX_PI * -0.5f;
	fLong = sqrtf(Space.x * Space.x + Space.z * Space.z);
	SetWall(SenterPos, D3DXVECTOR3(0.0f, fAngle - D3DX_PI * bInside, 0.0f), fLong, Height, WTYPE_INVISIBLE);

	aPos[0] = pos + D3DXVECTOR3(sinf(rot.y + D3DX_PI * 0.5f) * Width * 0.5f, 0.0f, cosf(rot.y + D3DX_PI * 0.5f) * Width * 0.5f) + D3DXVECTOR3(sinf(rot.y + D3DX_PI) * Depth * (1.0f - Corner) * 0.5f, 0.0f, cosf(rot.y + D3DX_PI) * Depth * (1.0f - Corner) * 0.5f);
	aPos[1] = pos + D3DXVECTOR3(sinf(rot.y + D3DX_PI) * Depth * 0.5f, 0.0f, cosf(rot.y + D3DX_PI) * Depth * 0.5f) + D3DXVECTOR3(sinf(rot.y + D3DX_PI * 0.5f) * Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(rot.y + D3DX_PI * 0.5f) * Width * (1.0f - Corner) * 0.5f);
	Space = aPos[1] - aPos[0];
	SenterPos = aPos[0] + Space * 0.5f;
	fAngle = atan2f(Space.x, Space.z) + D3DX_PI * -0.5f;
	fLong = sqrtf(Space.x * Space.x + Space.z * Space.z);
	SetWall(SenterPos, D3DXVECTOR3(0.0f, fAngle - D3DX_PI * bInside, 0.0f), fLong, Height, WTYPE_INVISIBLE);

	aPos[0] = pos + D3DXVECTOR3(sinf(rot.y + D3DX_PI) * Depth * 0.5f, 0.0f, cosf(rot.y + D3DX_PI) * Depth * 0.5f) + D3DXVECTOR3(sinf(rot.y + D3DX_PI * -0.5f) * Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(rot.y + D3DX_PI * -0.5f) * Width * (1.0f - Corner) * 0.5f);
	aPos[1] = pos + D3DXVECTOR3(sinf(rot.y + D3DX_PI * -0.5f) * Width * 0.5f, 0.0f, cosf(rot.y + D3DX_PI * -0.5f) * Width * 0.5f) + D3DXVECTOR3(sinf(rot.y + D3DX_PI) * Depth * (1.0f - Corner) * 0.5f, 0.0f, cosf(rot.y + D3DX_PI) * Depth * (1.0f - Corner) * 0.5f);
	Space = aPos[1] - aPos[0];
	SenterPos = aPos[0] + Space * 0.5f;
	fAngle = atan2f(Space.x, Space.z) + D3DX_PI * -0.5f;
	fLong = sqrtf(Space.x * Space.x + Space.z * Space.z);
	SetWall(SenterPos, D3DXVECTOR3(0.0f, fAngle - D3DX_PI * bInside, 0.0f), fLong, Height, WTYPE_INVISIBLE);

	aPos[0] = pos + D3DXVECTOR3(sinf(rot.y + D3DX_PI * -0.5f) * Width * 0.5f, 0.0f, cosf(rot.y + D3DX_PI * -0.5f) * Width * 0.5f) + D3DXVECTOR3(sinf(rot.y) * Depth * (1.0f - Corner) * 0.5f, 0.0f, cosf(rot.y) * Depth * (1.0f - Corner) * 0.5f);
	aPos[1] = pos + D3DXVECTOR3(sinf(rot.y) * Depth * 0.5f, 0.0f, cosf(rot.y) * Depth * 0.5f) + D3DXVECTOR3(sinf(rot.y + D3DX_PI * -0.5f) * Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(rot.y + D3DX_PI * -0.5f) * Width * (1.0f - Corner) * 0.5f);
	Space = aPos[1] - aPos[0];
	SenterPos = aPos[0] + Space * 0.5f;
	fAngle = atan2f(Space.x, Space.z) + D3DX_PI * -0.5f;
	fLong = sqrtf(Space.x * Space.x + Space.z * Space.z);
	SetWall(SenterPos, D3DXVECTOR3(0.0f, fAngle - D3DX_PI * bInside, 0.0f), fLong, Height, WTYPE_INVISIBLE);
}

//---------------
// 円
//---------------
void SetVirtualCollisionCircle(D3DXVECTOR3 pos, float Radius, float Height, int Division, float LostAngle, float LostRange, bool bInside)
{
	//正規化
	LostAngle = NormalizeAngle(LostAngle);

	float fAngleStep = (D3DX_PI * 2.0f) / Division;  // 1セグメントあたりの回転角度
	float WallWidth = 2.0f * Radius * sinf(D3DX_PI / Division);  // 壁の幅（多角形の一辺）

	float LostStart = LostAngle - (LostRange * 0.5f); // 除外範囲の開始角度
	float LostEnd = LostAngle + (LostRange * 0.5f);   // 除外範囲の終了角度

	// 角度の範囲を [0, 2π] に正規化
	LostStart = NormalizeAngle(LostStart);
	LostEnd = NormalizeAngle(LostEnd);

	for (int nCntWall = 0; nCntWall < Division; nCntWall++)
	{
		float fAngle = fAngleStep * nCntWall;  // 各壁の角度

		//正規化
		fAngle = NormalizeAngle(fAngle);

		// D3DX_PI をまたぐ場合のチェック
		if (LostStart <= LostEnd)
		{
			// 通常の範囲
			if (LostStart <= fAngle && fAngle <= LostEnd && LostRange != 0.0f)
			{
				continue;
			}
		}
		else
		{
			// D3DX_PI をまたぐ範囲 (例: 5.5 ~ 0.5)
			if (fAngle >= LostStart || fAngle <= LostEnd && LostRange != 0.0f)
			{
				continue;
			}
		}

		// 壁の位置
		D3DXVECTOR3 WallPos = pos + D3DXVECTOR3(sinf(fAngle) * Radius, 0.0f, cosf(fAngle) * Radius);

		// 壁の回転（y軸回転）
		D3DXVECTOR3 WallRotation = D3DXVECTOR3(0.0f, fAngle - D3DX_PI * bInside, 0.0f);

		// バリアの生成
		SetWall(WallPos, WallRotation, WallWidth, Height, WTYPE_INVISIBLE);
	}
}