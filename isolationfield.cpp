//---------------------------------------
//
//隔離バリア処理[isolationfield.cpp]
//Author fuma sato
//
//---------------------------------------
#include"isolationfield.h"
#include"camera.h"
#include"input.h"

//グローバル変数
IsolationField g_aIsolationField[ISOLATION_FIELD_MAX];
#ifdef _DEBUG
ISO_TYPE g_Type = ISO_TYPE_1;
int g_DelIso = -1;
#endif // _DEBUG

//---------------
//初期化処理
//---------------
void InitIsolationField(void)
{
	//構造体の初期化
	for (int nCnt = 0; nCnt < ISOLATION_FIELD_MAX; nCnt++)
	{//最大数回す
		g_aIsolationField[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置
		g_aIsolationField[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //向き
		g_aIsolationField[nCnt].Width = 0.0f;                          //幅
		g_aIsolationField[nCnt].Height = 0.0f;                         //高さ
		g_aIsolationField[nCnt].Depth = 0.0f;                          //奥行
		g_aIsolationField[nCnt].Corner = 0.0f;                         //角
		g_aIsolationField[nCnt].Type = ISO_TYPE_1;                     //タイプ
		g_aIsolationField[nCnt].Switch = OFF;                          //Switch
		g_aIsolationField[nCnt].bUse = false;                          //使用
	}

#ifdef _DEBUG
	g_Type = ISO_TYPE_1;
	g_DelIso = -1;
#endif // _DEBUG
}

//---------------
//終了処理
//---------------
void UninitIsolationField(void)
{
	//構造体の初期化
	for (int nCnt = 0; nCnt < ISOLATION_FIELD_MAX; nCnt++)
	{//最大数回す
		g_aIsolationField[nCnt].bUse = false;                          //使用
	}
}

//---------------
//更新処理
//---------------
void UpdateIsolationField(void)
{

}

//---------------
//描画処理
//---------------
void DrawIsolationField(void)
{

}

//---------------
//設定
//---------------
int SetIsolationField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, float Depth, float Corner, ISO_TYPE Type)
{
	for (int nCnt = 0; nCnt < ISOLATION_FIELD_MAX; nCnt++)
	{//最大数回す
		if (g_aIsolationField[nCnt].bUse == false)
		{//使用していない
			g_aIsolationField[nCnt].pos = pos;   //位置
			g_aIsolationField[nCnt].rot = rot;   //向き
			g_aIsolationField[nCnt].Width = Width;                           //幅
			g_aIsolationField[nCnt].Height = Height;                         //高さ
			g_aIsolationField[nCnt].Depth = Depth;                           //奥行
			g_aIsolationField[nCnt].Corner = Corner;                         //角
			g_aIsolationField[nCnt].Type = Type;                             //タイプ
			//バリアの生成
			SetBarrier(g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * 0.5f), g_aIsolationField[nCnt].rot, g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner), g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);
			SetBarrier(g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * 0.5f), g_aIsolationField[nCnt].rot + D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner), g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);
			SetBarrier(g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * 0.5f), g_aIsolationField[nCnt].rot + D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner), g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);
			SetBarrier(g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * 0.5f), g_aIsolationField[nCnt].rot + D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner), g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);

			//角のバリアの生成
			D3DXVECTOR3 aPos[2] = { D3DXVECTOR3(0.0f,0.0f,0.0f) }, SenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f),Space = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			float fAngle = 0.0f, fLong = 0.0f;

			aPos[0] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			aPos[1] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			Space = aPos[1] - aPos[0];
			SenterPos = aPos[0] + Space * 0.5f;
			fAngle = atan2f(Space.x, Space.z) + D3DX_PI * -0.5f;
			fLong = sqrtf(Space.x * Space.x + Space.z * Space.z);
			SetBarrier(SenterPos, D3DXVECTOR3(0.0f, fAngle, 0.0f), fLong, g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);

			aPos[0] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			aPos[1] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			Space = aPos[1] - aPos[0];
			SenterPos = aPos[0] + Space * 0.5f;
			fAngle = atan2f(Space.x, Space.z) + D3DX_PI * -0.5f;
			fLong = sqrtf(Space.x * Space.x + Space.z * Space.z);
			SetBarrier(SenterPos, D3DXVECTOR3(0.0f, fAngle, 0.0f), fLong, g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);

			aPos[0] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			aPos[1] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			Space = aPos[1] - aPos[0];
			SenterPos = aPos[0] + Space * 0.5f;
			fAngle = atan2f(Space.x, Space.z) + D3DX_PI * -0.5f;
			fLong = sqrtf(Space.x * Space.x + Space.z * Space.z);
			SetBarrier(SenterPos, D3DXVECTOR3(0.0f, fAngle, 0.0f), fLong, g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);

			aPos[0] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			aPos[1] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			Space = aPos[1] - aPos[0];
			SenterPos = aPos[0] + Space * 0.5f;
			fAngle = atan2f(Space.x, Space.z) + D3DX_PI * -0.5f;
			fLong = sqrtf(Space.x * Space.x + Space.z * Space.z);
			SetBarrier(SenterPos, D3DXVECTOR3(0.0f, fAngle, 0.0f), fLong, g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);

			SetBarrier(g_aIsolationField[nCnt].pos + D3DXVECTOR3(0.0f, g_aIsolationField[nCnt].Height, 0.0f), g_aIsolationField[nCnt].rot, g_aIsolationField[nCnt].Width, g_aIsolationField[nCnt].Depth, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), true, g_aIsolationField[nCnt].Corner);

			g_aIsolationField[nCnt].Switch = ON;                            //Switch
			g_aIsolationField[nCnt].bUse = true;                             //使用
			return nCnt;
		}
	}
	return -1;
}

//---------------
//取得
//---------------
IsolationField* GetIsolationField(void)
{
	return &g_aIsolationField[0];
}

//---------------
//破棄
//---------------
void IsolationFieldSwitch(ISO_TYPE Type, SWITCH Switch)
{
	//バリアのオンオフ
	BarrierSwitch((BTYPE)(BTYPE_ISOLATION_1 + Type), Switch);
	for (int nCnt = 0; nCnt < ISOLATION_FIELD_MAX; nCnt++)
	{
		if (g_aIsolationField[nCnt].bUse == true && g_aIsolationField[nCnt].Type == Type)
		{
			g_aIsolationField[nCnt].Switch = Switch;
		}
	}
}

//--------------------------
//オブジェクトエディター
//--------------------------
void UpdateIsolationFieldEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot)
{
#ifdef _DEBUG
	Camera* pCamera = GetCamera();
	static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static int HoldNumber = -1, EndNumber = -1;
	static float Width = 100.0f, Height = 100.0f, Depth = 100.0f, Corner = 0.0f;
	MODE Mode = GetMode();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_9))
	{//表示の切替
		static bool bON = false;
		if (bON)
		{
			IsolationFieldSwitch(ISO_TYPE_1, OFF);
			IsolationFieldSwitch(ISO_TYPE_2, OFF);
			IsolationFieldSwitch(ISO_TYPE_3, OFF);
			bON = false;
		}
		else
		{
			IsolationFieldSwitch(ISO_TYPE_1, ON);
			IsolationFieldSwitch(ISO_TYPE_2, ON);
			IsolationFieldSwitch(ISO_TYPE_3, ON);
			bON = true;
		}
	}
#endif // _DEBUG

	if (GetKeyboardTrigger(DIK_RIGHT))
	{// オブジェクト切替
		g_Type = (ISO_TYPE)(g_Type + 1);
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{// オブジェクト切替
		g_Type = (ISO_TYPE)(g_Type - 1);
	}

	//範囲制限
	g_Type = (ISO_TYPE)(g_Type % (ISO_TYPE_MAX));

	if (g_Type < 0)
	{//ループ
		g_Type = (ISO_TYPE)(ISO_TYPE_MAX - 1);
	}

	if (GetKeyboardTrigger(DIK_UP))
	{// DELオブジェクト切替
		g_DelIso++;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{// DELオブジェクト切替
		g_DelIso--;
	}

	for (int nCount = 0; nCount < ISOLATION_FIELD_MAX; nCount++)
	{//最大数回す

		//範囲制限
		if (g_DelIso < 0)
		{
			g_DelIso = ISOLATION_FIELD_MAX - 1;
		}
		g_DelIso %= ISOLATION_FIELD_MAX;

		if (g_aIsolationField[g_DelIso].bUse)
		{//使っているオブジェクトの選択
			break;
		}
		else
		{//使っていない番号はスキップ

			if (GetKeyboardTrigger(DIK_DOWN))
			{// DELオブジェクト切替
				g_DelIso--;
			}
			else
			{// DELオブジェクト切替
				g_DelIso++;
			}
		}

		if (nCount == ISOLATION_FIELD_MAX - 1)
		{//オブジェクトがない
			g_DelIso = -1;
		}
	}

	if (GetKeyboardPress(DIK_F) == true || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) == true)//Aキー
	{
		if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//Wキー
		{//左上
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.25f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.25f) * 10.0f;
		}
		else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//Sキー
		{//左下
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.75f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.75f) * 10.0f;
		}
		else
		{//左
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.5f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.5f) * 10.0f;
		}
	}
	else if (GetKeyboardPress(DIK_H) == true || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) == true)//Dキー
	{
		if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//Wキー
		{//右上
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * 10.0f;
		}
		else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//Sキー
		{//右下
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * 10.0f;
		}
		else
		{//右
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * 10.0f;
		}
	}
	else if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//Wキー
	{//上
		pos.x += sinf(pCamera->rot.y) * 10.0f;
		pos.z += cosf(pCamera->rot.y) * 10.0f;
	}
	else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//Sキー
	{//下
		pos.x += sinf(pCamera->rot.y + D3DX_PI) * 10.0f;
		pos.z += cosf(pCamera->rot.y + D3DX_PI) * 10.0f;
	}

	if (GetKeyboardPress(DIK_V) == true)//Wキー
	{//上
		pos.y -= 10.0f;
	}
	if (GetKeyboardPress(DIK_B) == true)//Sキー
	{//下
		pos.y += 10.0f;
	}

	if (GetKeyboardPress(DIK_U) || GetJoykeyPress(JOYKEY_A, CONTROLLER_1))
	{// 押している間
		rot.x -= 0.1f;
	}
	if (GetKeyboardPress(DIK_I) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// 押している間
		rot.x += 0.1f;
	}
	if (GetKeyboardPress(DIK_J) || GetJoykeyPress(JOYKEY_LT, CONTROLLER_1))
	{// 押している間
		rot.y -= 0.1f;
	}
	if (GetKeyboardPress(DIK_K) || GetJoykeyPress(JOYKEY_RT, CONTROLLER_1))
	{// 押している間
		rot.y += 0.1f;
	}
	if (GetKeyboardPress(DIK_M) || GetJoykeyPress(JOYKEY_X, CONTROLLER_1))
	{// 押している間
		rot.z -= 0.1f;
	}
	if (GetKeyboardPress(DIK_COMMA) || GetJoykeyPress(JOYKEY_Y, CONTROLLER_1))
	{// 押している間
		rot.z += 0.1f;
	}
	if (GetKeyboardPress(DIK_1) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// 押している間
		Corner -= 0.01f;
	}
	if (GetKeyboardPress(DIK_2) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// 押している間
		Corner += 0.01f;
	}
	if (GetKeyboardPress(DIK_3) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// 押している間
		Width -= 10.0f;
	}
	if (GetKeyboardPress(DIK_4) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// 押している間
		Width += 10.0f;
	}
	if (GetKeyboardPress(DIK_5) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// 押している間
		Height -= 10.0f;
	}
	if (GetKeyboardPress(DIK_6) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// 押している間
		Height += 10.0f;
	}
	if (GetKeyboardPress(DIK_7) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// 押している間
		Depth -= 10.0f;
	}
	if (GetKeyboardPress(DIK_8) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// 押している間
		Depth += 10.0f;
	}

	if (GetKeyboardTrigger(DIK_0) || GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1))
	{// 初期化
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Width = 0.0f;
		Height = 0.0f;
		Depth = 0.0f;
		Corner = 0.0f;
	}

	//正規化
	if (rot.x > D3DX_PI)
	{
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x < -D3DX_PI)
	{
		rot.x += D3DX_PI * 2.0f;
	}
	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}
	if (rot.z > D3DX_PI)
	{
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z < -D3DX_PI)
	{
		rot.z += D3DX_PI * 2.0f;
	}

	//スケール制限
	Width = max(ISOLATION_WIDTH_MIN, min(ISOLATION_WIDTH_MAX, Width));
	Height = max(ISOLATION_HEIGHT_MIN, min(ISOLATION_HEIGHT_MAX, Height));
	Depth = max(ISOLATION_DEPTH_MIN, min(ISOLATION_DEPTH_MAX, Depth));
	Corner = max(0.0f, min(1.0f, Corner));

	if (GetKeyboardTrigger(DIK_RSHIFT) || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1))
	{// 押したとき
		HoldNumber = SetIsolationField(Epos + pos, Erot + rot, Width, Height, Depth, Corner, g_Type);
		EndNumber = HoldNumber;
	}
	if (GetKeyboardPress(DIK_RSHIFT) || GetJoykeyPress(JOYKEY_RB, CONTROLLER_1))
	{// 押している間
		g_aIsolationField[HoldNumber].bUse = false;
		EndBarrier((BTYPE)(BTYPE_ISOLATION_1 + (BTYPE)g_aIsolationField[HoldNumber].Type));
		HoldNumber = SetIsolationField(Epos + pos, Erot + rot, Width, Height, Depth, Corner, g_Type);
		EndNumber = HoldNumber;
	}
	else
	{
		HoldNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_BACK) || GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1))
	{// 押したとき
		g_aIsolationField[EndNumber].bUse = false;
		EndBarrier((BTYPE)(BTYPE_ISOLATION_1 + (BTYPE)g_aIsolationField[EndNumber].Type));
		EndNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_F8))
	{// セーブ
		SaveIsolationField();
	}

	if (GetKeyboardTrigger(DIK_DELETE) || (GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1) && GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1)))
	{// 押したとき
		if (g_DelIso != -1)
		{
			g_aIsolationField[g_DelIso].bUse = false;
			EndBarrier((BTYPE)(BTYPE_ISOLATION_1 + (BTYPE)g_aIsolationField[g_DelIso].Type));
		}
	}
#endif // _DEBUG
}

//---------------------------------------
//ロード処理
//---------------------------------------
void LoadIsolationField(void)
{
	FILE* pFile;//ファイルポインタを宣言
	int nCount = 0;
	int nNumber = 0;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Width = 0.0f, Height = 0.0f, Depth = 0.0f, Corner = 0.0f;

	pFile = fopen(ISOLATION_FIELD_FILE, "r");//ファイルを開く
	if (pFile != NULL)
	{//開けたら
		for (nCount = 0; nCount < ISOLATION_FIELD_MAX; nCount++)
		{//最大数回す
			if (fscanf(pFile, "%d", &nNumber) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &pos.x) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &pos.y) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &pos.z) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &rot.x) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &rot.y) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &rot.z) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &Width) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &Height) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &Depth) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &Corner) == EOF)break;//数値を書き入れ

			//角度変換
			rot = D3DXToRadian(rot);

			//異常番号
			if (nNumber >= ISO_TYPE_MAX || nNumber < 0)
			{
				continue;
			}

			//オブジェクト設置
			SetIsolationField(pos, rot, Width, Height, Depth, Corner, (ISO_TYPE)nNumber);
		}
		fclose(pFile);//ファイルを閉じる
	}
	else
	{//開けなかった
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "ロードエラー", "Iso", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//---------------------------------------
//ロード処理
//---------------------------------------
void SaveIsolationField(void)
{
	FILE* pFile;//ファイルポインタを宣言
	D3DXVECTOR3 rot;
	int nCount = 0;

	pFile = fopen(ISOLATION_FIELD_FILE, "w");//ファイルを開く

	if (pFile != NULL)
	{//開けたら
		for (nCount = 0; nCount < ISOLATION_FIELD_MAX; nCount++)
		{//最大数回す
			if (g_aIsolationField[nCount].bUse && g_aIsolationField[nCount].Switch == ON)
			{//使用している
				//角度変換
				rot = D3DXToDegree(g_aIsolationField[nCount].rot);

				fprintf(pFile, "%d\n", (int)g_aIsolationField[nCount].Type);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aIsolationField[nCount].pos.x);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aIsolationField[nCount].pos.y);//数値を書き入れ
				fprintf(pFile, "%.1f\n", g_aIsolationField[nCount].pos.z);//数値を書き入れ
				fprintf(pFile, "%.1f ", rot.x);//数値を書き入れ
				fprintf(pFile, "%.1f ", rot.y);//数値を書き入れ
				fprintf(pFile, "%.1f\n", rot.z);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aIsolationField[nCount].Width);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aIsolationField[nCount].Height);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aIsolationField[nCount].Depth);//数値を書き入れ
				fprintf(pFile, "%.1f\n", g_aIsolationField[nCount].Corner);//数値を書き入れ
			}
		}
		fclose(pFile);//ファイルを閉じる
	}
	else
	{//開けなかった
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "ロードエラー", "ロードできなかったよ", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//--------------------
//タイプナンバー
//--------------------
int GetIsonumber(void)
{
#ifdef _DEBUG
	return (int)g_Type;
#endif // _DEBUG
}

//--------------------
//デリートナンバー
//--------------------
int GetIsoDelnumber(void)
{
#ifdef _DEBUG
	return g_DelIso;
#endif // _DEBUG
}