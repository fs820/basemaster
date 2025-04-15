//------------------------------------------
//
//ゲームメインの処理[Pause.cpp]
//Author fuma sato
//
//------------------------------------------

#include"pause.h"
#include"fade.h"
#include"input.h"
#include"game.h"

#define SELECT_WIDTH (512)
#define SELECT_HEIGHT (128)

typedef enum
{
	SELECT_RESTART = 0,
	SELECT_RETRY,
	SELECT_BACK,
	SELECT_EXIT,
	SELECT_MAX
}SELECT;

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTexturePause[SELECT_MAX] = { NULL };//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//バッファのポインタ
SELECT g_SelectNew = SELECT_RESTART;
bool g_bPause = false;

//--------------------
//初期化処理
//--------------------
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ
	D3DXVECTOR3 posSelect;//スコアの位置

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * SELECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_RESTART,
		&g_apTexturePause[0]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_RETRY,
		&g_apTexturePause[1]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_BACKTITLE,
		&g_apTexturePause[2]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EXIT,
		&g_apTexturePause[3]
	);

	g_bPause = false;
	posSelect = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SELECT_HEIGHT * 1.5f, 0.0f);

	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	for (int i = 0; i < SELECT_MAX; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[1].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[2].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[3].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;

	}

	g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック
}

//------------------
//終了処理
//------------------
void UninitPause(void)
{
	for (int i = 0; i < SELECT_MAX; i++)
	{
		//テクスチャの破棄
		if (g_apTexturePause[i] != NULL)
		{
			g_apTexturePause[i]->Release();
			g_apTexturePause[i] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//--------------
//更新処理
//--------------
void UpdatePause(void)
{
	if (g_bPause)
	{
		VERTEX_2D* pVtx;//頂点情報ポインタ

		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

		pVtx += VT_MAX * g_SelectNew;

		//カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック

		if (GetKeyboardTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_B, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_RIGHT) == true)
		{
			SetGameState(GAMESTATE_NORMAL);
			g_bPause = !g_bPause;
		}
		else if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_SENTER) == true)
		{
			FADE fade;
			switch (g_SelectNew)
			{
			case SELECT_RESTART:
				SetGameState(GAMESTATE_NORMAL);
				g_bPause = !g_bPause;
				break;
			case SELECT_RETRY:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//切替
					SetFade(MODE_GAME, true);
					g_bPause = !g_bPause;
				}
				break;
			case SELECT_BACK:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//切替
					SetFade(MODE_TITLE, true);
					g_bPause = !g_bPause;
				}
				break;
			case SELECT_EXIT:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//切替
					HWND hWnd;
					hWnd = GethWnd();
					PostMessage(hWnd, WM_QUIT, 0, 0);
					g_bPause = !g_bPause;
				}
				break;
			}
		}
		else if (GetMouseTrigger(MOUSE_LEFT) == true)
		{
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			for (int i = 0; i < SELECT_MAX; i++)
			{
				POINT point = GetCursorPoint(); //カーソルの位置を取得する

				if (point.x >= pVtx[0].pos.x && point.x <= pVtx[3].pos.x && point.y >= pVtx[0].pos.y && point.y <= pVtx[3].pos.y)
				{
					FADE fade;
					switch (i)
					{
					case SELECT_RESTART:
						SetGameState(GAMESTATE_NORMAL);
						g_bPause = !g_bPause;
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//切替
							SetFade(MODE_GAME, true);
							g_bPause = !g_bPause;
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//切替
							SetFade(MODE_TITLE, true);
							g_bPause = !g_bPause;
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
							g_bPause = !g_bPause;
						}
						break;
					}
				}
				pVtx += VT_MAX;
			}

			g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック
		}
		else if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true || GetJoykeyTrigger(JOYKEY_UP, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_UP, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() > 0.0f)
		{
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * g_SelectNew;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック

			switch (g_SelectNew)
			{
			case SELECT_RESTART:
				g_SelectNew = SELECT_EXIT;
				break;
			case SELECT_RETRY:
				g_SelectNew = SELECT_RESTART;
				break;
			case SELECT_BACK:
				g_SelectNew = SELECT_RETRY;
				break;
			case SELECT_EXIT:
				g_SelectNew = SELECT_BACK;
				break;
			}

			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * g_SelectNew;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック
		}
		else if (GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true || GetJoykeyTrigger(JOYKEY_DOWN, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() < 0.0f)
		{
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * g_SelectNew;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック

			switch (g_SelectNew)
			{
			case SELECT_RESTART:
				g_SelectNew = SELECT_RETRY;
				break;
			case SELECT_RETRY:
				g_SelectNew = SELECT_BACK;
				break;
			case SELECT_BACK:
				g_SelectNew = SELECT_EXIT;
				break;
			case SELECT_EXIT:
				g_SelectNew = SELECT_RESTART;
				break;
			}
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * g_SelectNew;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック
		}

		for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
		{
			if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
			{
				if (GetdJoykeyTrigger(ELEKEY_BACK, (CONTROLLER)i) == true)
				{
					SetGameState(GAMESTATE_NORMAL);
					g_bPause = !g_bPause;
				}
				else if (GetdJoykeyTrigger(ELEKEY_A, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (g_SelectNew)
					{
					case SELECT_RESTART:
						SetGameState(GAMESTATE_NORMAL);
						g_bPause = !g_bPause;
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//切替
							SetFade(MODE_GAME, true);
							g_bPause = !g_bPause;
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//切替
							SetFade(MODE_TITLE, true);
							g_bPause = !g_bPause;
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
							g_bPause = !g_bPause;
						}
						break;
					}
				}
				else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * g_SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック

					switch (g_SelectNew)
					{
					case SELECT_RESTART:
						g_SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						g_SelectNew = SELECT_RESTART;
						break;
					case SELECT_BACK:
						g_SelectNew = SELECT_RETRY;
						break;
					case SELECT_EXIT:
						g_SelectNew = SELECT_BACK;
						break;
					}

					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * g_SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * g_SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック

					switch (g_SelectNew)
					{
					case SELECT_RESTART:
						g_SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						g_SelectNew = SELECT_BACK;
						break;
					case SELECT_BACK:
						g_SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						g_SelectNew = SELECT_RESTART;
						break;
					}
					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * g_SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック
				}
			}
			else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
			{
				if (GetdJoykeyTrigger(PSKEY_BACK, (CONTROLLER)i) == true)
				{
					SetGameState(GAMESTATE_NORMAL);
					g_bPause = !g_bPause;
				}
				else if (GetdJoykeyTrigger(PSKEY_CI, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (g_SelectNew)
					{
					case SELECT_RESTART:
						SetGameState(GAMESTATE_NORMAL);
						g_bPause = !g_bPause;
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//切替
							SetFade(MODE_GAME, true);
							g_bPause = !g_bPause;
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//切替
							SetFade(MODE_TITLE, true);
							g_bPause = !g_bPause;
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
							g_bPause = !g_bPause;
						}
						break;
					}
				}
				else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * g_SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック

					switch (g_SelectNew)
					{
					case SELECT_RESTART:
						g_SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						g_SelectNew = SELECT_RESTART;
						break;
					case SELECT_BACK:
						g_SelectNew = SELECT_RETRY;
						break;
					case SELECT_EXIT:
						g_SelectNew = SELECT_BACK;
						break;
					}

					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * g_SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
				{
					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * g_SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック

					switch (g_SelectNew)
					{
					case SELECT_RESTART:
						g_SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						g_SelectNew = SELECT_BACK;
						break;
					case SELECT_BACK:
						g_SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						g_SelectNew = SELECT_RESTART;
						break;
					}
					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * g_SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック
				}
				else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
				{
					if (GetdJoykeyTrigger(NINKEY_－, (CONTROLLER)i) == true)
					{
						SetGameState(GAMESTATE_NORMAL);
						g_bPause = !g_bPause;
					}
					else if (GetdJoykeyTrigger(NINKEY_A, (CONTROLLER)i) == true)
					{
						FADE fade;
						switch (g_SelectNew)
						{
						case SELECT_RESTART:
							SetGameState(GAMESTATE_NORMAL);
							g_bPause = !g_bPause;
							break;
						case SELECT_RETRY:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//切替
								SetFade(MODE_GAME, true);
								g_bPause = !g_bPause;
							}
							break;
						case SELECT_BACK:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//切替
								SetFade(MODE_TITLE, true);
								g_bPause = !g_bPause;
							}
							break;
						case SELECT_EXIT:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//切替
								HWND hWnd;
								hWnd = GethWnd();
								PostMessage(hWnd, WM_QUIT, 0, 0);
								g_bPause = !g_bPause;
							}
							break;
						}
					}
					else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
					{
						g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * g_SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック

						switch (g_SelectNew)
						{
						case SELECT_RESTART:
							g_SelectNew = SELECT_EXIT;
							break;
						case SELECT_RETRY:
							g_SelectNew = SELECT_RESTART;
							break;
						case SELECT_BACK:
							g_SelectNew = SELECT_RETRY;
							break;
						case SELECT_EXIT:
							g_SelectNew = SELECT_BACK;
							break;
						}

						g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * g_SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック
					}
					else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
					{
						g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * g_SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック

						switch (g_SelectNew)
						{
						case SELECT_RESTART:
							g_SelectNew = SELECT_RETRY;
							break;
						case SELECT_RETRY:
							g_SelectNew = SELECT_BACK;
							break;
						case SELECT_BACK:
							g_SelectNew = SELECT_EXIT;
							break;
						case SELECT_EXIT:
							g_SelectNew = SELECT_RESTART;
							break;
						}
						g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * g_SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック
					}
				}
				else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
				{
					if (GetdJoykeyTrigger(DKEY_BACK, (CONTROLLER)i) == true)
					{
						SetGameState(GAMESTATE_NORMAL);
						g_bPause = !g_bPause;
					}
					else if (GetdJoykeyTrigger(DKEY_A, (CONTROLLER)i) == true)
					{
						FADE fade;
						switch (g_SelectNew)
						{
						case SELECT_RESTART:
							SetGameState(GAMESTATE_NORMAL);
							g_bPause = !g_bPause;
							break;
						case SELECT_RETRY:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//切替
								SetFade(MODE_GAME, true);
								g_bPause = !g_bPause;
							}
							break;
						case SELECT_BACK:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//切替
								SetFade(MODE_TITLE, true);
								g_bPause = !g_bPause;
							}
							break;
						case SELECT_EXIT:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//切替
								HWND hWnd;
								hWnd = GethWnd();
								PostMessage(hWnd, WM_QUIT, 0, 0);
								g_bPause = !g_bPause;
							}
							break;
						}
					}
					else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
					{
						g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * g_SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック

						switch (g_SelectNew)
						{
						case SELECT_RESTART:
							g_SelectNew = SELECT_EXIT;
							break;
						case SELECT_RETRY:
							g_SelectNew = SELECT_RESTART;
							break;
						case SELECT_BACK:
							g_SelectNew = SELECT_RETRY;
							break;
						case SELECT_EXIT:
							g_SelectNew = SELECT_BACK;
							break;
						}

						g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * g_SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック
					}
					else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
					{
						g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * g_SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック

						switch (g_SelectNew)
						{
						case SELECT_RESTART:
							g_SelectNew = SELECT_RETRY;
							break;
						case SELECT_RETRY:
							g_SelectNew = SELECT_BACK;
							break;
						case SELECT_BACK:
							g_SelectNew = SELECT_EXIT;
							break;
						case SELECT_EXIT:
							g_SelectNew = SELECT_RESTART;
							break;
						}
						g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * g_SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffPause->Unlock();//プレイヤーバッファのアンロック
					}
				}
			}
		}
	}
}

//-------------------
//描画処理
//-------------------
void DrawPause(void)
{
	if (g_bPause)
	{
		LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

		//デバイスの取得
		pDevice = GetDevice();

		//頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < SELECT_MAX; i++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTexturePause[i]);

			//背景の描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				i * VT_MAX,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}
}

//--------------------
//ポーズ
//--------------------
void Pause(void)
{
	g_bPause = !g_bPause;
}

//--------------------
//ポーズかどうか
//--------------------
bool bPause(void)
{
	return g_bPause;
}