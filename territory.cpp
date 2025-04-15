//---------------------------------------
//
//テリトリー処理[territory.cpp]
//Author fuma sato
//
//---------------------------------------
#include "territory.h"
#ifdef _DEBUG
#include "camera.h"
#include "stddxbuff.h"
#include "input.h"
#endif // _DEBUG

Territory g_aTerritory[TERRITORY_MAX];	// テリトリー情報
#ifdef _DEBUG
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTerritory = NULL; //バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTerritory = NULL;      //テクスチャのポインタ
TERRITORY_TYPE g_Type = TERRITORY_TYPE_A;	// タイプ
int g_nDelTerritory;					    // デリートナンバー
#endif // _DEBUG
//******************************************************
// テリトリー初期化
//******************************************************
void InitTerritory(void)
{
	for (int nCntTerritory = 0; nCntTerritory < TERRITORY_MAX; nCntTerritory++)
	{// 最大テリトリー分回す
		// 各種変数の初期化
		g_aTerritory[nCntTerritory].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aTerritory[nCntTerritory].fRadius = 0.0f;						    // 半径
		g_aTerritory[nCntTerritory].Type = TERRITORY_TYPE_A;				// タイプ
		g_aTerritory[nCntTerritory].bUse = false;						    // 使用
#ifdef _DEBUG
		g_aTerritory[nCntTerritory].col = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);	// 色
#endif // _DEBUG
	}

#ifdef _DEBUG
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4* TERRITORY_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_DEFAULT,
		&g_pVtxBuffTerritory,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFileEx
	(
		pDevice,
		TERRITORY_TEX,
		D3DX_DEFAULT,              // テクスチャの幅（D3DX_DEFAULTなら自動調整）
		D3DX_DEFAULT,              // テクスチャの高さ（D3DX_DEFAULTなら自動調整）
		D3DX_DEFAULT,              // ミップマップレベル（D3DX_DEFAULTで自動設定）
		0,                         // 使用しない場合は0（動的テクスチャならD3DUSAGE_DYNAMIC）
		D3DFMT_X8R8G8B8,           // フォーマット（A8R8G8B8 png X8R8G8B8 jpg）
		D3DPOOL_DEFAULT,           // メモリプール（通常は D3DPOOL_MANAGED）
		D3DX_FILTER_TRIANGLE,      // 画像の拡縮フィルタ（D3DX_FILTER_LINEAR など）
		D3DX_FILTER_TRIANGLE,      // ミップマップのフィルタ
		0,                         // カラーキー（透明にしたい色）
		NULL,                      // `D3DXIMAGE_INFO`（画像情報を取得する場合）
		NULL,                      // パレットデータ（パレットなしならNULL）
		&g_pTextureTerritory
	);

	//頂点の設定
	SetVertex3DZX
	(
		&g_pVtxBuffTerritory,
		0,
		TERRITORY_MAX,
		1,
		1,
		1.0f,
		1.0f,
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f,
		1.0f
	);
#endif // _DEBUG
}

//******************************************************
// テリトリー設定
//******************************************************
int SetTerritory(D3DXVECTOR3 pos, float fRadius, TERRITORY_TYPE Type)
{
	for (int nCntTerritory = 0; nCntTerritory < TERRITORY_MAX; nCntTerritory++)
	{// 最大テリトリー分回す
		if (g_aTerritory[nCntTerritory].bUse == false)
		{// 使っていないテリトリー
			g_aTerritory[nCntTerritory].pos = pos;		    // 位置
			g_aTerritory[nCntTerritory].fRadius = fRadius;	// 半径
			g_aTerritory[nCntTerritory].Type = Type;		// タイプ
			g_aTerritory[nCntTerritory].bUse = true;		// 使用

#ifdef _DEBUG

			switch (g_aTerritory[nCntTerritory].Type)
			{
			case TERRITORY_TYPE_A:
				g_aTerritory[nCntTerritory].col = D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f);
				break;
			case TERRITORY_TYPE_B:
				g_aTerritory[nCntTerritory].col = D3DXCOLOR(0.5f, 0.0f, 0.5f, 1.0f);
				break;
			case TERRITORY_TYPE_BOSS:
				g_aTerritory[nCntTerritory].col = D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f);
				break;
			}

			//頂点の設定
			SetVertex3DZX
			(
				&g_pVtxBuffTerritory,
				nCntTerritory,
				1,
				1,
				1,
				g_aTerritory[nCntTerritory].fRadius * 2.0f,
				g_aTerritory[nCntTerritory].fRadius * 2.0f,
				D3DXVECTOR3(0.0f, 1.0f, 0.0f),
				g_aTerritory[nCntTerritory].col,
				g_aTerritory[nCntTerritory].fRadius * 2.0f,
				g_aTerritory[nCntTerritory].fRadius * 2.0f
			);
#endif // _DEBUG
			return nCntTerritory;
		}
	}
	return -1;
}

//******************************************************
// テリトリーリンク設定
//******************************************************
int LinkTerritory(int* nIdx, TERRITORY_TYPE Type)
{
	int nCnt = 0;

	for (int nCntTerritory = 0; nCntTerritory < TERRITORY_MAX; nCntTerritory++)
	{// 最大テリトリー分回す
		if (g_aTerritory[nCntTerritory].bUse == true && g_aTerritory[nCntTerritory].Type == Type)
		{// 指定のテリトリー
			nIdx[nCnt] = nCntTerritory;
			nCnt++;
		}
	}
	return nCnt;
}

//******************************************************
// テリトリーチェック
//******************************************************
int CheckTerritory(int* nIdx, int Quantity, D3DXVECTOR3 pos)
{
	for (int nCntIdx = 0; nCntIdx < Quantity; nCntIdx++)
	{// 最大テリトリー分回す
		if (g_aTerritory[nIdx[nCntIdx]].bUse == true)
		{// 使っているテリトリー
			D3DXVECTOR3 Space = pos - g_aTerritory[nIdx[nCntIdx]].pos;
			float fDistance = D3DXVec3Length(&Space);	// 距離
			if (fDistance < g_aTerritory[nIdx[nCntIdx]].fRadius)
			{// テリトリー内
				return nCntIdx;
			}
		}
	}

	return -1;
}

//******************************************************
// テリトリー位置取得
//******************************************************
D3DXVECTOR3 GetTerritoryPos(int nIdx)
{
	return g_aTerritory[nIdx].pos;
}

//******************************************************
// テリトリーエディター更新
//******************************************************
void UpdateTerritoryEdit(D3DXVECTOR3 Epos)
{
#ifdef _DEBUG
	Camera* pCamera = GetCamera();
	static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static float fRadius = 100.0f;
	static int HoldNumber = -1, EndNumber = -1;

	if (GetKeyboardTrigger(DIK_RIGHT))
	{// オブジェクト切替
		g_Type = (TERRITORY_TYPE)(g_Type + 1);
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{// オブジェクト切替
		g_Type = (TERRITORY_TYPE)(g_Type - 1);
	}

	if (g_Type < 0)
	{//ループ
		g_Type = (TERRITORY_TYPE)(TERRITORY_TYPE_MAX - 1);
	}

	//範囲制限
	g_Type = (TERRITORY_TYPE)(g_Type % TERRITORY_TYPE_MAX);


	if (GetKeyboardTrigger(DIK_UP))
	{// DELオブジェクト切替
		g_nDelTerritory++;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{// DELオブジェクト切替
		g_nDelTerritory--;
	}

	for (int nCount = 0; nCount < TERRITORY_MAX; nCount++)
	{//最大数回す
		//範囲制限
		if (g_nDelTerritory < 0)
		{
			g_nDelTerritory = TERRITORY_MAX - 1;
		}
		g_nDelTerritory %= TERRITORY_MAX;

		if (g_aTerritory[g_nDelTerritory].bUse)
		{//使っているオブジェクトの選択
			break;
		}
		else
		{//使っていない番号はスキップ

			if (GetKeyboardTrigger(DIK_DOWN))
			{// DELオブジェクト切替
				g_nDelTerritory--;
			}
			else
			{// DELオブジェクト切替
				g_nDelTerritory++;
			}
		}

		if (nCount >= TERRITORY_MAX - 1)
		{//どれも使っていないとき
			g_nDelTerritory = -1;
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

	if (GetKeyboardPress(DIK_J) || GetJoykeyPress(JOYKEY_LT, CONTROLLER_1))
	{// 押している間
		fRadius -= 10.0f;
	}
	if (GetKeyboardPress(DIK_K) || GetJoykeyPress(JOYKEY_RT, CONTROLLER_1))
	{// 押している間
		fRadius += 10.0f;
	}

	if (GetKeyboardTrigger(DIK_0) || GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1))
	{// 初期化
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fRadius = 100.0f;
	}

	static TERRITORY_TYPE TypeOld = TERRITORY_TYPE_A; //一時保存用ナンバー
	static int DelEnemyOld = -1; //一時保存用ナンバー

	if (GetKeyboardTrigger(DIK_RSHIFT) || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1))
	{// 押したとき
		HoldNumber = SetTerritory(Epos + pos,fRadius, g_Type);
		EndNumber = HoldNumber;
	}
	if (GetKeyboardPress(DIK_RSHIFT) || GetJoykeyPress(JOYKEY_RB, CONTROLLER_1))
	{// 押している間
		if (HoldNumber != -1)
		{
			g_aTerritory[HoldNumber].pos = Epos + pos;
			g_aTerritory[HoldNumber].fRadius = fRadius;

			//頂点の設定
			SetVertex3DZX
			(
				&g_pVtxBuffTerritory,
				HoldNumber,
				1,
				1,
				1,
				g_aTerritory[HoldNumber].fRadius * 2.0f,
				g_aTerritory[HoldNumber].fRadius * 2.0f,
				D3DXVECTOR3(0.0f, 1.0f, 0.0f),
				g_aTerritory[HoldNumber].col,
				g_aTerritory[HoldNumber].fRadius * 2.0f,
				g_aTerritory[HoldNumber].fRadius * 2.0f
			);

			static TERRITORY_TYPE Type = TERRITORY_TYPE_A; //一時保存用ナンバー

			if (Type != g_Type)
			{//ナンバーが変化した
				//オブジェクトの消去
				g_aTerritory[HoldNumber].bUse = false;
				HoldNumber = SetTerritory(Epos + pos, fRadius, g_Type);
				//オブジェクトの切り替え
				EndNumber = HoldNumber;
				Type = g_Type; //ナンバーの更新
			}
		}
	}
	else
	{
		HoldNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_BACK) || GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1))
	{// 押したとき
		g_aTerritory[EndNumber].bUse = false;
		EndNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_F8))
	{// セーブ
		SaveTerritory();
	}

	if (GetKeyboardTrigger(DIK_DELETE) || (GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1) && GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1)))
	{// 押したとき
		g_aTerritory[g_nDelTerritory].bUse = false;
	}

	static int DelNumber = -1; //一時保存用ナンバー

	if (DelNumber != g_nDelTerritory)
	{//デリートナンバーが変化した
		if (DelNumber != -1)
		{//前回のオブジェクトが存在する
			g_aTerritory[DelNumber].col.a = 1.0f;

			//頂点の設定
			SetVertex3DZX
			(
				&g_pVtxBuffTerritory,
				DelNumber,
				1,
				1,
				1,
				g_aTerritory[DelNumber].fRadius * 2.0f,
				g_aTerritory[DelNumber].fRadius * 2.0f,
				D3DXVECTOR3(0.0f, 1.0f, 0.0f),
				g_aTerritory[DelNumber].col,
				g_aTerritory[DelNumber].fRadius * 2.0f,
				g_aTerritory[DelNumber].fRadius * 2.0f
			);
		}
		DelNumber = g_nDelTerritory; //選択オブジェクトの更新
		if (DelNumber != -1)
		{//オブジェクトが存在する
			//マテリアルデータへのポインタを取得
			g_aTerritory[DelNumber].col.a = 0.5f;

			//頂点の設定
			SetVertex3DZX
			(
				&g_pVtxBuffTerritory,
				DelNumber,
				1,
				1,
				1,
				g_aTerritory[DelNumber].fRadius * 2.0f,
				g_aTerritory[DelNumber].fRadius * 2.0f,
				D3DXVECTOR3(0.0f, 1.0f, 0.0f),
				g_aTerritory[DelNumber].col,
				g_aTerritory[DelNumber].fRadius * 2.0f,
				g_aTerritory[DelNumber].fRadius * 2.0f
			);
		}
	}
#endif // _DEBUG
}

//******************************************************
// テリトリー読み込み
//******************************************************
void LoadTerritory(void)
{
	FILE* pFile;//ファイルポインタを宣言
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fRadius = 0.0f;
	char Str[256];
	bool bEndPos = false, bEndRadius = false;

	//Xファイル
	pFile = fopen(TERRITORY_FILE, "r");//ファイルを開く
	if (pFile != NULL)
	{//開けたら
		do
		{
			//文字列の初期化
			for (int nCount = 0; nCount < 256; nCount++)
			{
				Str[nCount] = '\0';
			}

			//文字列の読み取り
			if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

			// 改行と#を削除
			Str[strcspn(Str, "\r\n")] = '\0';
			Str[strcspn(Str, "#")] = '\0';

			if (strstr(Str, "[TYPE_A]") != NULL)
			{//MadCatzAの時
				do
				{//ループ
					//文字列の初期化
					for (int nCount = 0; nCount < 256; nCount++)
					{
						Str[nCount] = '\0';
					}

					//文字列の読み取り
					if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

					// 改行と#を削除
					Str[strcspn(Str, "\r\n")] = '\0';
					Str[strcspn(Str, "#")] = '\0';

					if (strstr(Str, "[TYPE_B]") != NULL)
					{
						fseek(pFile, -(long)(strlen(Str) * 4 + 1), SEEK_CUR);
						break;
					}

					//番号の行を見つける
					if (strcspn(Str, "[") == 0 && strlen(Str) != 0)
					{
						do
						{
							//文字列の初期化
							for (int nCount = 0; nCount < 256; nCount++)
							{
								Str[nCount] = '\0';
							}

							if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

							if (strstr(Str, "POS") != NULL)
							{
								fseek(pFile, -(long)(strlen(Str) + 1), SEEK_CUR);
								//文字列の初期化
								for (int nCount = 0; nCount < 256; nCount++)
								{
									Str[nCount] = '\0';
								}

								if (fscanf(pFile, "%s", Str) == EOF)break;//数値を書き入れ

								if (strcmp(Str, "POS") == 0)
								{
									if (fscanf(pFile, "%s", Str) == EOF)break;//数値を書き入れ

									if (strcmp(Str, "=") == 0)
									{
										if (fscanf(pFile, "%f", &pos.x) == EOF)break;//数値を書き入れ
										if (fscanf(pFile, "%f", &pos.y) == EOF)break;//数値を書き入れ
										if (fscanf(pFile, "%f", &pos.z) == EOF)break;//数値を書き入れ
										bEndPos = true;
									}
								}
							}

							if (strstr(Str, "RADIUS") != NULL)
							{
								fseek(pFile, -(long)(strlen(Str) + 1), SEEK_CUR);
								//文字列の初期化
								for (int nCount = 0; nCount < 256; nCount++)
								{
									Str[nCount] = '\0';
								}

								if (fscanf(pFile, "%s", Str) == EOF)break;//数値を書き入れ

								if (strcmp(Str, "RADIUS") == 0)
								{
									if (fscanf(pFile, "%s", Str) == EOF)break;//数値を書き入れ

									if (strcmp(Str, "=") == 0)
									{
										if (fscanf(pFile, "%f", &fRadius) == EOF)break;//数値を書き入れ
										bEndRadius = true;
									}
								}
							}

							if (bEndPos && bEndRadius)
							{
								//オブジェクト設置
								SetTerritory(pos, fRadius, TERRITORY_TYPE_A);
								bEndPos = false, bEndRadius = false;
								break;
							}
						} while (true);
					}
				} while (true);
			}
			else if (strstr(Str, "[TYPE_B]") != NULL)
			{//MadCatzBの時
				do
				{//ループ
					//文字列の初期化
					for (int nCount = 0; nCount < 256; nCount++)
					{
						Str[nCount] = '\0';
					}

					//文字列の読み取り
					if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

					// 改行と#を削除
					Str[strcspn(Str, "\r\n")] = '\0';
					Str[strcspn(Str, "#")] = '\0';

					if (strstr(Str, "[TYPE_BOSS]") != NULL)
					{
						fseek(pFile, -(long)(strlen(Str) * 3), SEEK_CUR);
						break;
					}

					//番号の行を見つける
					if (strcspn(Str, "[") == 0 && strlen(Str) != 0)
					{
						do
						{
							//文字列の初期化
							for (int nCount = 0; nCount < 256; nCount++)
							{
								Str[nCount] = '\0';
							}

							if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

							if (strstr(Str, "POS") != NULL)
							{
								fseek(pFile, -(long)(strlen(Str) + 1), SEEK_CUR);
								//文字列の初期化
								for (int nCount = 0; nCount < 256; nCount++)
								{
									Str[nCount] = '\0';
								}

								if (fscanf(pFile, "%s", Str) == EOF)break;//数値を書き入れ

								if (strcmp(Str, "POS") == 0)
								{
									if (fscanf(pFile, "%s", Str) == EOF)break;//数値を書き入れ

									if (strcmp(Str, "=") == 0)
									{
										if (fscanf(pFile, "%f", &pos.x) == EOF)break;//数値を書き入れ
										if (fscanf(pFile, "%f", &pos.y) == EOF)break;//数値を書き入れ
										if (fscanf(pFile, "%f", &pos.z) == EOF)break;//数値を書き入れ
										bEndPos = true;
									}
								}
							}

							if (strstr(Str, "RADIUS") != NULL)
							{
								fseek(pFile, -(long)(strlen(Str) + 1), SEEK_CUR);
								//文字列の初期化
								for (int nCount = 0; nCount < 256; nCount++)
								{
									Str[nCount] = '\0';
								}

								if (fscanf(pFile, "%s", Str) == EOF)break;//数値を書き入れ

								if (strcmp(Str, "RADIUS") == 0)
								{
									if (fscanf(pFile, "%s", Str) == EOF)break;//数値を書き入れ

									if (strcmp(Str, "=") == 0)
									{
										if (fscanf(pFile, "%f", &fRadius) == EOF)break;//数値を書き入れ
										bEndRadius = true;
									}
								}
							}

							if (bEndPos && bEndRadius)
							{
								//オブジェクト設置
								SetTerritory(pos, fRadius, TERRITORY_TYPE_B);
								bEndPos = false, bEndRadius = false;
								break;
							}
						} while (true);
					}
				} while (true);
			}
			else if (strstr(Str, "[TYPE_BOSS]") != NULL)
			{//Xerosの時
				do
				{//ループ
					//文字列の初期化
					for (int nCount = 0; nCount < 256; nCount++)
					{
						Str[nCount] = '\0';
					}

					//文字列の読み取り
					if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

					// 改行と#を削除
					Str[strcspn(Str, "\r\n")] = '\0';
					Str[strcspn(Str, "#")] = '\0';

					//番号の行を見つける
					if (strcspn(Str, "[") == 0 && strlen(Str) != 0)
					{
						do
						{
							//文字列の初期化
							for (int nCount = 0; nCount < 256; nCount++)
							{
								Str[nCount] = '\0';
							}

							if (fgets(Str, sizeof(Str), pFile) == nullptr)break;

							if (strstr(Str, "POS") != NULL)
							{
								fseek(pFile, -(long)(strlen(Str) + 1), SEEK_CUR);
								//文字列の初期化
								for (int nCount = 0; nCount < 256; nCount++)
								{
									Str[nCount] = '\0';
								}

								if (fscanf(pFile, "%s", Str) == EOF)break;//数値を書き入れ

								if (strcmp(Str, "POS") == 0)
								{
									if (fscanf(pFile, "%s", Str) == EOF)break;//数値を書き入れ

									if (strcmp(Str, "=") == 0)
									{
										if (fscanf(pFile, "%f", &pos.x) == EOF)break;//数値を書き入れ
										if (fscanf(pFile, "%f", &pos.y) == EOF)break;//数値を書き入れ
										if (fscanf(pFile, "%f", &pos.z) == EOF)break;//数値を書き入れ

										bEndPos = true;
									}
								}
							}

							if (strstr(Str, "RADIUS") != NULL)
							{
								fseek(pFile, -(long)(strlen(Str) + 1), SEEK_CUR);
								//文字列の初期化
								for (int nCount = 0; nCount < 256; nCount++)
								{
									Str[nCount] = '\0';
								}

								if (fscanf(pFile, "%s", Str) == EOF)break;//数値を書き入れ

								if (strcmp(Str, "RADIUS") == 0)
								{
									if (fscanf(pFile, "%s", Str) == EOF)break;//数値を書き入れ

									if (strcmp(Str, "=") == 0)
									{
										if (fscanf(pFile, "%f", &fRadius) == EOF)break;//数値を書き入れ
										bEndRadius = true;
									}
								}
							}

							if (bEndPos && bEndRadius)
							{
								//オブジェクト設置
								SetTerritory(pos, fRadius, TERRITORY_TYPE_BOSS);
								bEndPos = false, bEndRadius = false;
								break;
							}
						} while (true);
					}
				} while (true);
			}
		} while (true);

		fclose(pFile);//ファイルを閉じる
	}
	else
	{//開けなかった
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "ロードエラー", "Territory", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//******************************************************
// テリトリー書き出し
//******************************************************
void SaveTerritory(void)
{
	FILE* pFile;//ファイルポインタを宣言
	int nCount = 0;

	//Xファイル
	pFile = fopen(TERRITORY_FILE, "w");//ファイルを開く
	if (pFile != NULL)
	{//開けたら

		fprintf(pFile, "[TYPE_A] #MadCatzAのテリトリー\n\n");//数値を書き入れ
		for (nCount = 0; nCount < TERRITORY_MAX; nCount++)
		{//最大数回す
			if (g_aTerritory[nCount].bUse && g_aTerritory[nCount].Type == TERRITORY_TYPE_A)
			{//使用している
				fprintf(pFile, "[%d] #インデックス\n", nCount);//数値を書き入れ
				fprintf(pFile, "POS = %.1f ", g_aTerritory[nCount].pos.x);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aTerritory[nCount].pos.y);//数値を書き入れ
				fprintf(pFile, "%.1f #位置\n", g_aTerritory[nCount].pos.z);//数値を書き入れ
				fprintf(pFile, "RADIUS = %.1f #半径\n\n", g_aTerritory[nCount].fRadius);//数値を書き入れ
			}
		}

		fprintf(pFile, "[TYPE_B] #MadCatzBのテリトリー\n\n");//数値を書き入れ
		for (nCount = 0; nCount < TERRITORY_MAX; nCount++)
		{//最大数回す
			if (g_aTerritory[nCount].bUse && g_aTerritory[nCount].Type == TERRITORY_TYPE_B)
			{//使用している
				fprintf(pFile, "[%d] #インデックス\n", nCount);//数値を書き入れ
				fprintf(pFile, "POS = %.1f ", g_aTerritory[nCount].pos.x);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aTerritory[nCount].pos.y);//数値を書き入れ
				fprintf(pFile, "%.1f #位置\n", g_aTerritory[nCount].pos.z);//数値を書き入れ
				fprintf(pFile, "RADIUS = %.1f #半径\n\n", g_aTerritory[nCount].fRadius);//数値を書き入れ
			}
		}

		fprintf(pFile, "[TYPE_BOSS] #Xerosのテリトリー\n\n");//数値を書き入れ
		for (nCount = 0; nCount < TERRITORY_MAX; nCount++)
		{//最大数回す
			if (g_aTerritory[nCount].bUse && g_aTerritory[nCount].Type == TERRITORY_TYPE_BOSS)
			{//使用している
				fprintf(pFile, "[%d] #インデックス\n", nCount);//数値を書き入れ
				fprintf(pFile, "POS = %.1f ", g_aTerritory[nCount].pos.x);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aTerritory[nCount].pos.y);//数値を書き入れ
				fprintf(pFile, "%.1f #位置\n", g_aTerritory[nCount].pos.z);//数値を書き入れ
				fprintf(pFile, "RADIUS = %.1f #半径\n\n", g_aTerritory[nCount].fRadius);//数値を書き入れ
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

//******************************************************
// タイプ取得
//******************************************************
TERRITORY_TYPE GetTerritoryType(void)
{
#ifdef _DEBUG
	return g_Type;
#endif // _DEBUG
}

//******************************************************
// デリートナンバー取得
//******************************************************
int GetDelTerritory(void)
{
#ifdef _DEBUG
	return g_nDelTerritory;
#endif // _DEBUG
}

//-----------------------------------------------------
//終了処理
//-----------------------------------------------------
void UninitTerritory(void)
{
	for (int nCntTerritory = 0; nCntTerritory < TERRITORY_MAX; nCntTerritory++)
	{// 最大テリトリー分回す
		// 各種変数の初期化
		g_aTerritory[nCntTerritory].bUse = false;						    // 使用
	}

#ifdef _DEBUG
	if (g_pTextureTerritory != NULL)
	{
		g_pTextureTerritory->Release();
		g_pTextureTerritory = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTerritory != NULL)
	{
		g_pVtxBuffTerritory->Release();
		g_pVtxBuffTerritory = NULL;
	}
#endif // _DEBUG
}

#ifdef _DEBUG
//------------------------------------------------------
//描画処理
//------------------------------------------------------
void DrawTerritory(void)
{
	LPDIRECT3DDEVICE9 pDevice; // デバイスへのポインタ
	D3DXMATRIX mtxTrans; // 計算マトリックス

	// デバイスの取得
	pDevice = GetDevice();

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0); // 0の方が適切 sato
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Zテスト
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Depth Bias 設定 sato
	float depthBias = -0.000001f; //Zバッファをカメラ方向にオフセットする値

	// 遠いほうから描画する
	static int nIdx[TERRITORY_MAX]; // 遠い順格納用
	static int nLimit = 0;
	if (nLimit % 60 == 0)
	{// フレームごとに処理 (毎フレームやると重くなる可能性があるため)
		D3DXVECTOR3 PosList[TERRITORY_MAX]; // ポスリスト
		Camera* pCamera = GetCamera();
		for (int nCnt = 0; nCnt < TERRITORY_MAX; nCnt++)
		{// ポスリスト作成
			if (g_aTerritory[nCnt].bUse)
			{
				PosList[nCnt] = g_aTerritory[nCnt].pos;
			}
			else
			{
				PosList[nCnt] = pCamera->posV;
			}
		}
		DrawCameraFarSort(nIdx, PosList, TERRITORY_MAX); //遠い順にソート
	}
	nLimit++;

	for (int nCntTerritory = 0; nCntTerritory < TERRITORY_MAX; nCntTerritory++)
	{
		if (g_aTerritory[nIdx[nCntTerritory]].bUse == false)
		{
			break;
		}

		depthBias += -0.000001f; //影同士の重なりを避ける
		pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Zバイアス設定

		// マトリックス初期化
		D3DXMatrixIdentity(&g_aTerritory[nIdx[nCntTerritory]].mtxWorld);

		// 位置の計算
		D3DXMatrixTranslation(&mtxTrans, g_aTerritory[nIdx[nCntTerritory]].pos.x, g_aTerritory[nIdx[nCntTerritory]].pos.y, g_aTerritory[nIdx[nCntTerritory]].pos.z);
		D3DXMatrixMultiply(&g_aTerritory[nIdx[nCntTerritory]].mtxWorld, &g_aTerritory[nIdx[nCntTerritory]].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aTerritory[nIdx[nCntTerritory]].mtxWorld);

		// 頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffTerritory, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTerritory);

		// ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,        // タイプ
			4 * nIdx[nCntTerritory],
			2                          // ポリゴンの個数
		);
	}

	// Depth Bias 設定を解除 sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// Zテストを戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// 設定をを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
#endif // _DEBUG