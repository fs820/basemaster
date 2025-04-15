//----------------------------------------
//
//カメラ処理[camera.cpp]
//Author fuma sato
//
//----------------------------------------

#include "main.h"
#include "camera.h"
#include "input.h"
#include "pause.h"
#include "player.h"
#include "wall.h"
#include <algorithm>
using std::swap;

//グローバル変数宣言
Camera g_camera[CAMERA_XNUM * CAMERA_YNUM];//カメラ情報
//----------------------
//初期化処理
//----------------------
void InitCamera(void)
{
	for (int nCount = 0; nCount < CAMERA_XNUM * CAMERA_YNUM; nCount++)
	{
		//カメラ設定
		g_camera[nCount].posV = D3DXVECTOR3(0.0f, 120.0f, -30.0f);
		g_camera[nCount].posVDest = D3DXVECTOR3(0.0f, 12.0f, -30.0f);
		g_camera[nCount].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCount].posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCount].posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		g_camera[nCount].rot = D3DXVECTOR3(-atan2f(12.0f, 30.0f), 0.0f, 0.0f);
		g_camera[nCount].fDistance = sqrtf((g_camera[nCount].posR.x - g_camera[nCount].posV.x) * (g_camera[nCount].posR.x - g_camera[nCount].posV.x) + (g_camera[nCount].posR.y - g_camera[nCount].posV.y) * (g_camera[nCount].posR.y - g_camera[nCount].posV.y) + (g_camera[nCount].posR.z - g_camera[nCount].posV.z) * (g_camera[nCount].posR.z - g_camera[nCount].posV.z));
		g_camera[nCount].fDistanceMin = CAMERA_DISTANCE_MIN;
		g_camera[nCount].fDistanceMax = CAMERA_DISTANCE_MAX;
		g_camera[nCount].viewport.X = (nCount % CAMERA_XNUM) * (DWORD)SCREEN_WIDTH / CAMERA_XNUM;
		g_camera[nCount].viewport.Y = (nCount / CAMERA_XNUM) * (DWORD)SCREEN_HEIGHT / CAMERA_YNUM;
		g_camera[nCount].viewport.Width = (DWORD)SCREEN_WIDTH / CAMERA_XNUM;
		g_camera[nCount].viewport.Height = (DWORD)SCREEN_HEIGHT / CAMERA_YNUM;
		g_camera[nCount].viewport.MinZ = 0.0f;
		g_camera[nCount].viewport.MaxZ = 1.0f;
	}
}

//-------------------
//終了処理
//-------------------
void UninitCamera(void)
{

}

//-------------------
//更新処理
//-------------------
void UpdateCamera(void)
{
	for (int nCount = 0; nCount < CAMERA_XNUM * CAMERA_YNUM; nCount++)
	{
		if (!bPause())
		{
			float fMouseWheel;
			fMouseWheel = GetMouseWheel();
			g_camera[nCount].fDistance += fMouseWheel * CAMERA_DISTANCE_SPEED;
			if (GetKeyboardPress(DIK_1) || GetJoykeyPress(JOYKEY_RB, CONTROLLER_1))
			{
				g_camera[nCount].fDistance += CAMERA_DISTANCE_SPEED;
			}
			if (GetKeyboardPress(DIK_2) || GetJoykeyPress(JOYKEY_RT, CONTROLLER_1))
			{
				g_camera[nCount].fDistance += -CAMERA_DISTANCE_SPEED;
			}

			if (!strcmp(ControllerName(CONTROLLER_1), ELE_CON))
			{
				if (GetdJoykeyPress(ELEKEY_RB, CONTROLLER_1))
				{
					g_camera[nCount].fDistance += CAMERA_DISTANCE_SPEED;
				}
				if (GetdJoykeyPress(ELEKEY_RT, CONTROLLER_1))
				{
					g_camera[nCount].fDistance += -CAMERA_DISTANCE_SPEED;
				}
			}
			else if (!strcmp(ControllerName(CONTROLLER_1), PS_CON))
			{
				if (GetdJoykeyPress(PSKEY_RB, CONTROLLER_1))
				{
					g_camera[nCount].fDistance += CAMERA_DISTANCE_SPEED;
				}
				if (GetdJoykeyPress(PSKEY_RT, CONTROLLER_1))
				{
					g_camera[nCount].fDistance += -CAMERA_DISTANCE_SPEED;
				}
			}
			else if (!strcmp(ControllerName(CONTROLLER_1), NIN_CON))
			{
				if (GetdJoykeyPress(NINKEY_RB, CONTROLLER_1))
				{
					g_camera[nCount].fDistance += CAMERA_DISTANCE_SPEED;
				}
				if (GetdJoykeyPress(NINKEY_RT, CONTROLLER_1))
				{
					g_camera[nCount].fDistance += -CAMERA_DISTANCE_SPEED;
				}
			}
			else if (!IsXInputControllerConnected(CONTROLLER_1) && IsDirectInputControllerConnected(CONTROLLER_1))
			{
				if (GetdJoykeyPress(DKEY_RB, CONTROLLER_1))
				{
					g_camera[nCount].fDistance += CAMERA_DISTANCE_SPEED;
				}
				if (GetdJoykeyPress(DKEY_RT, CONTROLLER_1))
				{
					g_camera[nCount].fDistance += -CAMERA_DISTANCE_SPEED;
				}
			}

			//スケール制限
			g_camera[nCount].fDistance = max(g_camera[nCount].fDistanceMin, min(g_camera[nCount].fDistanceMax, g_camera[nCount].fDistance));
		}

		if (GetMode() == MODE_TITLE || GetMode() == MODE_RESULT || GetMode() == MODE_RANK)
		{
			g_camera[nCount].rot.y += CAMERA_VIEW_SPEED;

			//スケール制限
			g_camera[nCount].rot.x = max(-D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX, min(D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX_GRA, g_camera[nCount].rot.x));

			//正規化
			if (g_camera[nCount].rot.x > D3DX_PI)
			{
				g_camera[nCount].rot.x -= D3DX_PI * 2.0f;
			}
			else if (g_camera[nCount].rot.x < -D3DX_PI)
			{
				g_camera[nCount].rot.x += D3DX_PI * 2.0f;
			}

			//正規化
			if (g_camera[nCount].rot.y > D3DX_PI)
			{
				g_camera[nCount].rot.y -= D3DX_PI * 2.0f;
			}
			else if (g_camera[nCount].rot.y < -D3DX_PI)
			{
				g_camera[nCount].rot.y += D3DX_PI * 2.0f;
			}

			if (GetKeyboardPress(DIK_R))
			{
				g_camera[nCount].posR.x = g_camera[nCount].posV.x + sinf(g_camera[nCount].rot.y) * cosf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;
				g_camera[nCount].posR.y = g_camera[nCount].posV.y + sinf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;
				g_camera[nCount].posR.z = g_camera[nCount].posV.z + cosf(g_camera[nCount].rot.y) * cosf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;
			}
			else
			{
				g_camera[nCount].posV.x = g_camera[nCount].posR.x + sinf(g_camera[nCount].rot.y - D3DX_PI) * cosf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;
				g_camera[nCount].posV.y = g_camera[nCount].posR.y + sinf(g_camera[nCount].rot.x - D3DX_PI) * g_camera[nCount].fDistance;
				g_camera[nCount].posV.z = g_camera[nCount].posR.z + cosf(g_camera[nCount].rot.y - D3DX_PI) * cosf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;
			}
		}
		else if (!bPause())
		{
			Player* pPlayer = GetPlayer();
			float* pStick;
			float* pMouseMove;
			float Oldrot = g_camera[nCount].rot.y;
			//X
			if (IsXInputControllerConnected(CONTROLLER_1))
			{
				//右スティック処理
				pStick = GetJoyStick(STICK_RIGHT, CONTROLLER_1);
				if (sqrtf(*pStick * *pStick + *(pStick + 1) * *(pStick + 1)) >= STICK_DED)
				{
					g_camera[nCount].rot.y += cosf(atan2f(*(pStick + 1), *pStick)) * CAMERA_ROT_SPEED;
					g_camera[nCount].rot.x += sinf(atan2f(*(pStick + 1), *pStick)) * CAMERA_ROT_SPEED;
				}
			}
			//d
			else if (IsDirectInputControllerConnected(CONTROLLER_1))
			{
				//右スティック処理
				pStick = GetdJoyStick(STICK_RIGHT, CONTROLLER_1);
				if (sqrtf(*pStick * *pStick + *(pStick + 1) * *(pStick + 1)) >= STICK_DED)
				{
					g_camera[nCount].rot.y += cosf(atan2f(*(pStick + 1), *pStick)) * CAMERA_ROT_SPEED;
					g_camera[nCount].rot.x += sinf(-atan2f(*(pStick + 1), *pStick)) * CAMERA_ROT_SPEED;
				}
			}

			pMouseMove = GetMouseMove();
			if (sqrtf(*pMouseMove * *pMouseMove + *(pMouseMove + 1) * *(pMouseMove + 1)) >= STICK_DED)
			{
				g_camera[nCount].rot.y += cosf(atan2f(*(pMouseMove + 1), *pMouseMove)) * CAMERA_ROT_SPEED;
				g_camera[nCount].rot.x += sinf(-atan2f(*(pMouseMove + 1), *pMouseMove)) * CAMERA_ROT_SPEED;
			}

			if (GetKeyboardPress(DIK_DOWN))
			{
				g_camera[nCount].rot.x -= CAMERA_ROT_SPEED;
			}

			if (GetKeyboardPress(DIK_UP))
			{
				g_camera[nCount].rot.x += CAMERA_ROT_SPEED;
			}

			if (GetKeyboardPress(DIK_LEFT))
			{
				g_camera[nCount].rot.y -= CAMERA_ROT_SPEED;
			}

			if (GetKeyboardPress(DIK_RIGHT))
			{
				g_camera[nCount].rot.y += CAMERA_ROT_SPEED;
			}

			if (pPlayer->move.x == 0.0f && pPlayer->move.z == 0.0f && Oldrot == g_camera[nCount].rot.y)
			{
				static int nCnt = 0;
				float rot = g_camera[nCount].rot.y;
				nCnt++;
				if (nCnt % 300 < 50)
				{
					rot = pPlayer->Destrot.y - D3DX_PI;
				}
				//正規化
				if (rot - g_camera[nCount].rot.y > D3DX_PI)
				{
					rot = g_camera[nCount].rot.y + ((rot - g_camera[nCount].rot.y) - D3DX_PI * 2.0f);
				}
				else if (rot - g_camera[nCount].rot.y < -D3DX_PI)
				{
					rot = g_camera[nCount].rot.y + ((rot - g_camera[nCount].rot.y) + D3DX_PI * 2.0f);
				}

				g_camera[nCount].rot.y += (rot - g_camera[nCount].rot.y) * CAMERA_INA;
			}

			if (!GetKeyboardPress(DIK_R) && !GetMousePress(MOUSE_B1))
			{
				//スケール制限
				g_camera[nCount].rot.x = max(-D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX, min(D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX_GRA, g_camera[nCount].rot.x));
			}

			//正規化
			if (g_camera[nCount].rot.x > D3DX_PI)
			{
				g_camera[nCount].rot.x -= D3DX_PI * 2.0f;
			}
			else if (g_camera[nCount].rot.x < -D3DX_PI)
			{
				g_camera[nCount].rot.x += D3DX_PI * 2.0f;
			}

			//正規化
			if (g_camera[nCount].rot.y > D3DX_PI)
			{
				g_camera[nCount].rot.y -= D3DX_PI * 2.0f;
			}
			else if (g_camera[nCount].rot.y < -D3DX_PI)
			{
				g_camera[nCount].rot.y += D3DX_PI * 2.0f;
			}

			if (GetKeyboardPress(DIK_R) || GetMousePress(MOUSE_B1))
			{
				//スケール制限
				g_camera[nCount].rot.x = max(-D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX * 0.5f, min(D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX * 0.5f, g_camera[nCount].rot.x));
				g_camera[nCount].posV = pPlayer->pos;
				g_camera[nCount].posV.y += pPlayer->aModel[0].pos.y + pPlayer->aModel[1].pos.y;
				g_camera[nCount].posR.x = g_camera[nCount].posV.x + sinf(g_camera[nCount].rot.y) * cosf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;
				g_camera[nCount].posR.y = g_camera[nCount].posV.y + sinf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;
				g_camera[nCount].posR.z = g_camera[nCount].posV.z + cosf(g_camera[nCount].rot.y) * cosf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;
			}
			else
			{
				Player* pPlayer = GetPlayer();
				g_camera[nCount].posRDest.x = pPlayer->pos.x + sinf(atan2f(pPlayer->move.z, pPlayer->move.x)) * SPEED;
				g_camera[nCount].posRDest.y = pPlayer->pos.y + cosf(g_camera[nCount].rot.x) * SPEED;
				g_camera[nCount].posRDest.z = pPlayer->pos.z + cosf(atan2f(pPlayer->move.z, pPlayer->move.x)) * SPEED;
				g_camera[nCount].posVDest.x = pPlayer->pos.x + sinf(g_camera[nCount].rot.y - D3DX_PI) * cosf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;
				g_camera[nCount].posVDest.y = pPlayer->pos.y + sinf(g_camera[nCount].rot.x - D3DX_PI) * g_camera[nCount].fDistance;
				g_camera[nCount].posVDest.z = pPlayer->pos.z + cosf(g_camera[nCount].rot.y - D3DX_PI) * cosf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;

				g_camera[nCount].posR.x += (g_camera[nCount].posRDest.x - g_camera[nCount].posR.x) * CAMERA_INA;
				g_camera[nCount].posR.y += (g_camera[nCount].posRDest.y - g_camera[nCount].posR.y) * CAMERA_INA;
				g_camera[nCount].posR.z += (g_camera[nCount].posRDest.z - g_camera[nCount].posR.z) * CAMERA_INA;
				g_camera[nCount].posV.x += (g_camera[nCount].posVDest.x - g_camera[nCount].posV.x) * CAMERA_INA;
				g_camera[nCount].posV.y += (g_camera[nCount].posVDest.y - g_camera[nCount].posV.y) * CAMERA_INA;
				g_camera[nCount].posV.z += (g_camera[nCount].posVDest.z - g_camera[nCount].posV.z) * CAMERA_INA;
			}

			if (GetKeyboardPress(DIK_P) || GetMousePress(MOUSE_B2) == true || GetJoykeyPress(JOYKEY_X, CONTROLLER_1) == true)
			{
				float* pStick;
				float* pMouseMove;
				//X
				if (IsXInputControllerConnected(CONTROLLER_1))
				{
					//右スティック処理
					pStick = GetJoyStick(STICK_RIGHT, CONTROLLER_1);
					if (sqrtf(*pStick * *pStick + *(pStick + 1) * *(pStick + 1)) >= STICK_DED)
					{
						g_camera[nCount].posV.x += sinf(g_camera[nCount].rot.y - D3DX_PI * 0.5f + atan2f(*(pStick + 1), -*pStick)) * CAMERA_SPEED;
						g_camera[nCount].posV.z += cosf(g_camera[nCount].rot.y - D3DX_PI * 0.5f + atan2f(*(pStick + 1), -*pStick)) * CAMERA_SPEED;
					}
				}
				//d
				else if (IsDirectInputControllerConnected(CONTROLLER_1))
				{
					//右スティック処理
					pStick = GetdJoyStick(STICK_RIGHT, CONTROLLER_1);
					if (sqrtf(*pStick * *pStick + *(pStick + 1) * *(pStick + 1)) >= STICK_DED)
					{
						g_camera[nCount].posV.x += sinf(g_camera[nCount].rot.y - D3DX_PI * 0.5f + atan2f(-*(pStick + 1), -*pStick)) * CAMERA_SPEED;
						g_camera[nCount].posV.z += cosf(g_camera[nCount].rot.y - D3DX_PI * 0.5f + atan2f(-*(pStick + 1), -*pStick)) * CAMERA_SPEED;
					}
				}

				pMouseMove = GetMouseMove();
				if (sqrtf(*pMouseMove * *pMouseMove + *(pMouseMove + 1) * *(pMouseMove + 1)) >= STICK_DED)
				{
					g_camera[nCount].posV.x -= sinf(g_camera[nCount].rot.y - D3DX_PI * 0.5f + atan2f(*(pMouseMove + 1), *pMouseMove)) * CAMERA_SPEED;
					g_camera[nCount].posV.z -= cosf(g_camera[nCount].rot.y - D3DX_PI * 0.5f + atan2f(*(pMouseMove + 1), *pMouseMove)) * CAMERA_SPEED;
				}

				if (GetKeyboardPress(DIK_LEFT) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1))
				{
					g_camera[nCount].posV.x += sinf(g_camera[nCount].rot.y - D3DX_PI * 0.5f) * CAMERA_SPEED;
					g_camera[nCount].posV.z += cosf(g_camera[nCount].rot.y - D3DX_PI * 0.5f) * CAMERA_SPEED;
				}

				if (GetKeyboardPress(DIK_RIGHT) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1))
				{
					g_camera[nCount].posV.x -= sinf(g_camera[nCount].rot.y - D3DX_PI * 0.5f) * CAMERA_SPEED;
					g_camera[nCount].posV.z -= cosf(g_camera[nCount].rot.y - D3DX_PI * 0.5f) * CAMERA_SPEED;
				}

				if (GetKeyboardPress(DIK_DOWN) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1))
				{
					g_camera[nCount].posV.x += sinf(g_camera[nCount].rot.y - D3DX_PI) * CAMERA_SPEED;
					g_camera[nCount].posV.z += cosf(g_camera[nCount].rot.y - D3DX_PI) * CAMERA_SPEED;
				}

				if (GetKeyboardPress(DIK_UP) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1))
				{
					g_camera[nCount].posV.x -= sinf(g_camera[nCount].rot.y - D3DX_PI) * CAMERA_SPEED;
					g_camera[nCount].posV.z -= cosf(g_camera[nCount].rot.y - D3DX_PI) * CAMERA_SPEED;
				}

				if (GetKeyboardPress(DIK_RSHIFT) || GetJoykeyPress(JOYKEY_X, CONTROLLER_1))
				{
					g_camera[nCount].posV.y += CAMERA_SPEED;
				}

				if (GetKeyboardPress(DIK_RCONTROL) || GetJoykeyPress(JOYKEY_X, CONTROLLER_1))
				{
					g_camera[nCount].posV.y -= CAMERA_SPEED;
				}

				g_camera[nCount].posR.x = g_camera[nCount].posV.x + sinf(g_camera[nCount].rot.y) * cosf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;
				g_camera[nCount].posR.y = g_camera[nCount].posV.y + sinf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;
				g_camera[nCount].posR.z = g_camera[nCount].posV.z + cosf(g_camera[nCount].rot.y) * cosf(g_camera[nCount].rot.x) * g_camera[nCount].fDistance;
			}
		}
	}
}

//-------------------
//描画処理
//-------------------
void SetCamera(int nNumber)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATRIX mtxRot, mtxTrans;//計算マトリックス

	//デバイスの取得
	pDevice = GetDevice();
	//ビューマトリックス初期化
	D3DXMatrixIdentity(&g_camera[nNumber].mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH
	(
		&g_camera[nNumber].mtxView,
		&g_camera[nNumber].posV,
		&g_camera[nNumber].posR,
		&g_camera[nNumber].posU
	);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nNumber].mtxView);

	//プロジェクションマトリックス初期化
	D3DXMatrixIdentity(&g_camera[nNumber].mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH
	(
		&g_camera[nNumber].mtxProjection,
		D3DXToRadian(CAMERA_WIDTH),//視界の広さ
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,//比率
		CAMERA_MIN,//どこから
		CAMERA_MAX//どこまで
	);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nNumber].mtxProjection);
}

//--------------------------
//カメラ取得
//--------------------------
Camera* GetCamera(void)
{
	return &g_camera[0];
}

//--------------------
//背景用
//--------------------
void ViewCamera(int nNumber)
{
	if (nNumber == CAMERA_XNUM * CAMERA_YNUM)
	{
		for (int nCount = 0; nCount < CAMERA_XNUM * CAMERA_YNUM; nCount++)
		{
			g_camera[nCount].posV = D3DXVECTOR3(0.0f, 120.0f, -6000.0f);
			g_camera[nCount].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_camera[nCount].fDistance = sqrtf((g_camera[nCount].posR.x - g_camera[nCount].posV.x) * (g_camera[nCount].posR.x - g_camera[nCount].posV.x) + (g_camera[nCount].posR.y - g_camera[nCount].posV.y) * (g_camera[nCount].posR.y - g_camera[nCount].posV.y) + (g_camera[nCount].posR.z - g_camera[nCount].posV.z) * (g_camera[nCount].posR.z - g_camera[nCount].posV.z));
			g_camera[nCount].fDistanceMax = CAMERA_DISTANCE_VIEW_MAX;
		}
		return;
	}

	g_camera[nNumber].posV = D3DXVECTOR3(0.0f, 120.0f, -6000.0f);
	g_camera[nNumber].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[nNumber].fDistance = sqrtf((g_camera[nNumber].posR.x - g_camera[nNumber].posV.x) * (g_camera[nNumber].posR.x - g_camera[nNumber].posV.x) + (g_camera[nNumber].posR.y - g_camera[nNumber].posV.y) * (g_camera[nNumber].posR.y - g_camera[nNumber].posV.y) + (g_camera[nNumber].posR.z - g_camera[nNumber].posV.z) * (g_camera[nNumber].posR.z - g_camera[nNumber].posV.z));
	g_camera[nNumber].fDistanceMax = CAMERA_DISTANCE_VIEW_MAX;
}

//--------------------
//ゲーム用
//--------------------
void GameCamera(int nNumber)
{
	if (nNumber == CAMERA_XNUM * CAMERA_YNUM)
	{
		for (int nCount = 0; nCount < CAMERA_XNUM * CAMERA_YNUM; nCount++)
		{
			g_camera[nCount].posV = D3DXVECTOR3(0.0f, 120.0f, -300.0f);
			g_camera[nCount].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_camera[nCount].fDistance = sqrtf((g_camera[nCount].posR.x - g_camera[nCount].posV.x) * (g_camera[nCount].posR.x - g_camera[nCount].posV.x) + (g_camera[nCount].posR.y - g_camera[nCount].posV.y) * (g_camera[nCount].posR.y - g_camera[nCount].posV.y) + (g_camera[nCount].posR.z - g_camera[nCount].posV.z) * (g_camera[nCount].posR.z - g_camera[nCount].posV.z));
			g_camera[nCount].fDistanceMax = CAMERA_DISTANCE_MAX;
		}
		return;
	}

	g_camera[nNumber].posV = D3DXVECTOR3(0.0f, 120.0f, -300.0f);
	g_camera[nNumber].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[nNumber].fDistance = sqrtf((g_camera[nNumber].posR.x - g_camera[nNumber].posV.x) * (g_camera[nNumber].posR.x - g_camera[nNumber].posV.x) + (g_camera[nNumber].posR.y - g_camera[nNumber].posV.y) * (g_camera[nNumber].posR.y - g_camera[nNumber].posV.y) + (g_camera[nNumber].posR.z - g_camera[nNumber].posV.z) * (g_camera[nNumber].posR.z - g_camera[nNumber].posV.z));
	g_camera[nNumber].fDistanceMax = CAMERA_DISTANCE_MAX;
}

//-----------------------------------------------------
// カメラから遠い順インデックス作成関数
//-----------------------------------------------------
HRESULT DeptSort(int* nIdx, D3DXVECTOR3* pPos, int IdxNum)
{
	if (nIdx == nullptr || pPos == nullptr)
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < IdxNum; nCnt++)
	{// 通常の並びにしておく
		nIdx[nCnt] = nCnt;
	}

	int nRam = 0;
	for (int nCnt = 0; nCnt < IdxNum - 1; nCnt++)
	{// 比較元のループ
		for (int nNextCnt = nCnt + 1; nNextCnt < IdxNum; nNextCnt++)
		{// 比較先のループ
			D3DXVECTOR3 Space = pPos[nIdx[nCnt]] - g_camera[0].posV; //比較元の空間
			float NowLong = D3DXVec3Length(&Space); //比較元の距離
			Space = pPos[nIdx[nNextCnt]] - g_camera[0].posV; //比較先の空間
			float NextLong = D3DXVec3Length(&Space); //比較先の距離
			if (NextLong > NowLong)
			{// 遠かったら
				// 入れ替え
				swap(nIdx[nCnt], nIdx[nNextCnt]);
			}
		}
	}

	return S_OK;
}

//-----------------------------------------------------
// カメラから見てさえぎられてるか sato
//-----------------------------------------------------
bool DrawCameraCollision(D3DXVECTOR3 pos)
{
	if (TriggerObject(&pos, &g_camera[0].posV, 5.0f))
	{
		return true;
	}
	if (TriggerWall(&pos, &g_camera[0].posV, 10.0f))
	{
		return true;
	}

	return false;
}