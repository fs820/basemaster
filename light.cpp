//---------------------------------------
//
//ライト処理[light.cpp]
//Author fuma sato
//
//---------------------------------------
#include "light.h"

//グローバル変数宣言
LIGHT g_light[LIGHT_MAX];	//ライト情報

//*****************************************
//ライトの初期化
//*****************************************
void InitLight(void)
{
	//// ライトをクリアする
	//ZeroMemory(g_light, sizeof(g_light));

	//// ライトの種類を設定
	//g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

	//// ライトの拡散光を設定
	//g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// 正規化する(大きさ１のベクトルにする)
	//D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);
	//g_light[nCntLight].Direction = vecDir[nCntLight];

	//// ライトを設定する
	//pDevice->SetLight(nCntLight, &g_light[nCntLight]);

	//// ライトを有効にする
	//pDevice->LightEnable(nCntLight, TRUE);

	////種類
	//g_light[3].Type = D3DLIGHT_POINT;
	////拡散光
	//g_light[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
	////環境光
	//g_light[3].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
	////鏡面反射光
	//g_light[3].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
	////位置
	//g_light[3].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	////範囲
	//g_light[3].Range = 0.0f;
	////距離減衰率(一定)
	//g_light[3].Attenuation0 = 0.1f;
	////距離減衰率(距離)
	//g_light[3].Attenuation1 = 0.0f;
	////距離減衰率(距離*2)
	//g_light[3].Attenuation2 = 0.0f;
	////ライトセット
	//pDevice->SetLight(3, &g_light[3]);
	////有効化
	//pDevice->LightEnable(3, FALSE);

	////種類
	//g_light[nCnt].Type = D3DLIGHT_SPOT;
	////拡散光
	//g_light[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	////環境光
	//g_light[nCnt].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	////鏡面反射光
	//g_light[nCnt].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	////位置
	//g_light[nCnt].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	////方向
	//vecDir[nCnt] = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	////正規化
	//D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
	//g_light[nCnt].Direction = vecDir[nCnt];
	////範囲
	//g_light[nCnt].Range = 0.0f;
	////コーン減衰率
	//g_light[nCnt].Falloff = 0.01f;
	////距離減衰率
	//g_light[nCnt].Attenuation0 = 0.1f;
	////コーン角度(内)
	//g_light[nCnt].Theta = D3DXToRadian(90.0f);
	////コーン角度(外)
	//g_light[nCnt].Phi = D3DXToRadian(360.0f);
	////ライトセット
	//pDevice->SetLight(nCnt, &g_light[nCnt]);
	////有効化
	//pDevice->LightEnable(nCnt, FALSE);
}

//*****************************************
//ライトの終了
//*****************************************
void UninitLight(void)
{
	
}

//*****************************************
//ライトの更新
//*****************************************
void UpdateLight(void)
{

}

//*****************************************
//ライトの取得
//*****************************************
LIGHT* GetLight(void)
{
	return &g_light[0];
}

//--------------------------
//セット
//--------------------------
unsigned int SetLight(D3DLIGHT9* pLight)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (unsigned int nCntLight = 0; nCntLight < LIGHT_MAX; nCntLight++)
	{
		if (g_light[nCntLight].bUse == false)
		{// 使用されていない
			//ライトセット
			CHECK_HR(pDevice->SetLight(nCntLight, pLight))

			//有効化
			CHECK_HR(pDevice->LightEnable(nCntLight, TRUE))

			// ライト情報を保存
			g_light[nCntLight].light = *pLight;

			// 使用フラグを立てる
			g_light[nCntLight].bUse = true;

			return nCntLight;
		}
	}

	return -1; // 使用されていないライトがない
}

//--------------------------
//破棄
//--------------------------
void EndLight(unsigned int nIdx)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ライトを無効にする
	CHECK_HR(pDevice->LightEnable(nIdx, FALSE))

	//使用フラグを下ろす
	g_light[nIdx].bUse = false;
}