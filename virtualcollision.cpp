//---------------------------------------
//
//���z�I�����蔻�菈��[virtualcollision.cpp]
//Author fuma sato
//
//---------------------------------------
#include"virtualcollision.h"
#include"wall.h"

//---------------
//�l�p
//---------------
void SetVirtualCollisionBox(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, float Depth, float Corner, bool bInside)
{
	//�o���A�̐���
	SetWall(pos + D3DXVECTOR3(sinf(rot.y) * Depth * 0.5f, 0.0f, cosf(rot.y) * Depth * 0.5f), rot - D3DXVECTOR3(0.0f, D3DX_PI * bInside, 0.0f), Width * (1.0f - Corner), Height, WTYPE_INVISIBLE);
	SetWall(pos + D3DXVECTOR3(sinf(rot.y + D3DX_PI * 0.5f) * Width * 0.5f, 0.0f, cosf(rot.y + D3DX_PI * 0.5f) * Width * 0.5f), rot + D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f) - D3DXVECTOR3(0.0f, D3DX_PI * bInside, 0.0f), Depth * (1.0f - Corner), Height, WTYPE_INVISIBLE);
	SetWall(pos + D3DXVECTOR3(sinf(rot.y + D3DX_PI) * Depth * 0.5f, 0.0f, cosf(rot.y + D3DX_PI) * Depth * 0.5f), rot + D3DXVECTOR3(0.0f, D3DX_PI, 0.0f) - D3DXVECTOR3(0.0f, D3DX_PI * bInside, 0.0f), Width * (1.0f - Corner), Height, WTYPE_INVISIBLE);
	SetWall(pos + D3DXVECTOR3(sinf(rot.y + D3DX_PI * -0.5f) * Width * 0.5f, 0.0f, cosf(rot.y + D3DX_PI * -0.5f) * Width * 0.5f), rot + D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f) - D3DXVECTOR3(0.0f, D3DX_PI * bInside, 0.0f), Depth * (1.0f - Corner), Height, WTYPE_INVISIBLE);

	//�p�̃o���A�̐���
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
// �~
//---------------
void SetVirtualCollisionCircle(D3DXVECTOR3 pos, float Radius, float Height, int Division, float LostAngle, float LostRange, bool bInside)
{
	//���K��
	LostAngle = NormalizeAngle(LostAngle);

	float fAngleStep = (D3DX_PI * 2.0f) / Division;  // 1�Z�O�����g������̉�]�p�x
	float WallWidth = 2.0f * Radius * sinf(D3DX_PI / Division);  // �ǂ̕��i���p�`�̈�Ӂj

	float LostStart = LostAngle - (LostRange * 0.5f); // ���O�͈͂̊J�n�p�x
	float LostEnd = LostAngle + (LostRange * 0.5f);   // ���O�͈͂̏I���p�x

	// �p�x�͈̔͂� [0, 2��] �ɐ��K��
	LostStart = NormalizeAngle(LostStart);
	LostEnd = NormalizeAngle(LostEnd);

	for (int nCntWall = 0; nCntWall < Division; nCntWall++)
	{
		float fAngle = fAngleStep * nCntWall;  // �e�ǂ̊p�x

		//���K��
		fAngle = NormalizeAngle(fAngle);

		// D3DX_PI ���܂����ꍇ�̃`�F�b�N
		if (LostStart <= LostEnd)
		{
			// �ʏ�͈̔�
			if (LostStart <= fAngle && fAngle <= LostEnd && LostRange != 0.0f)
			{
				continue;
			}
		}
		else
		{
			// D3DX_PI ���܂����͈� (��: 5.5 ~ 0.5)
			if (fAngle >= LostStart || fAngle <= LostEnd && LostRange != 0.0f)
			{
				continue;
			}
		}

		// �ǂ̈ʒu
		D3DXVECTOR3 WallPos = pos + D3DXVECTOR3(sinf(fAngle) * Radius, 0.0f, cosf(fAngle) * Radius);

		// �ǂ̉�]�iy����]�j
		D3DXVECTOR3 WallRotation = D3DXVECTOR3(0.0f, fAngle - D3DX_PI * bInside, 0.0f);

		// �o���A�̐���
		SetWall(WallPos, WallRotation, WallWidth, Height, WTYPE_INVISIBLE);
	}
}