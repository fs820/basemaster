//------------------------------------------
//
//リザルトの処理[result.cpp]
//Author fuma sato
//
//------------------------------------------

#include"result.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"game.h"
#include"score.h"
#include"file.h"

#define RESULT_TIME (600)
#define RESULT_WIDTH (200)
#define RESULT_HEIGHT (200)

LPDIRECT3DTEXTURE9 g_apTextureResult = NULL;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;//バッファのポインタ

//--------------------
//初期化処理
//--------------------
void InitResult(void)
{
	HWND hWnd = GethWnd();

	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ
	D3DXVECTOR3 posScore;//スコアの位置
	int nScore = 0;
	int aPosTexUr[SCORE_MAX];

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * SCORE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NUMBER,
		&g_apTextureResult
	);


	//スコアの取得
	nScore = GetClearTime();
	for (int i = 0; i < 2; i++)
	{
		aPosTexUr[i] = Digit(nScore, i);
	}
	aPosTexUr[2] = 10;
	for (int i = 3; i < 5; i++)
	{
		aPosTexUr[i] = Digit(nScore, i - 1);
	}
	posScore = D3DXVECTOR3(SCREEN_WIDTH / 2 - (SCORE_WIDTH / SCORE_MAX) * (SCORE_MAX / 2), SCREEN_HEIGHT / 2, 0.0f);

	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	for (int i = 0; i < SCORE_MAX; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(posScore.x - (SCORE_WIDTH / SCORE_MAX) / 2, posScore.y - SCORE_HEIGHT / 2, posScore.z);
		pVtx[1].pos = D3DXVECTOR3(posScore.x + (SCORE_WIDTH / SCORE_MAX) / 2, posScore.y - SCORE_HEIGHT / 2, posScore.z);
		pVtx[2].pos = D3DXVECTOR3(posScore.x - (SCORE_WIDTH / SCORE_MAX) / 2, posScore.y + SCORE_HEIGHT / 2, posScore.z);
		pVtx[3].pos = D3DXVECTOR3(posScore.x + (SCORE_WIDTH / SCORE_MAX) / 2, posScore.y + SCORE_HEIGHT / 2, posScore.z);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i], UV_DEF / V_MAX_S * (aPosTexUr[i] / U_MAX_S));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexUr[i] / U_MAX_S));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i], UV_DEF / V_MAX_S * (aPosTexUr[i] / U_MAX_S) + UV_DEF / V_MAX_S);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexUr[i] / U_MAX_S) + UV_DEF / V_MAX_S);

		posScore.x += SCORE_WIDTH / SCORE_MAX;
		pVtx += VT_MAX;
	}

	g_pVtxBuffResult->Unlock();//プレイヤーバッファのアンロック
}

//------------------
//終了処理
//------------------
void UninitResult(void)
{
	//テクスチャの破棄
	if (g_apTextureResult != NULL)
	{
		g_apTextureResult->Release();
		g_apTextureResult = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//--------------
//更新処理
//--------------
void UpdateResult(void)
{
	static int nResultTime = 0;

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START, CONTROLLER_MAX) == true || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_LEFT) == true)
	{
		nResultTime = 0;
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//切替
			SetFade(MODE_RANK, true);
		}
	}

	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (GetdJoykeyTrigger(ELEKEY_A, (CONTROLLER)i) || GetdJoykeyTrigger(ELEKEY_START, (CONTROLLER)i))
			{
				nResultTime = 0;
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//切替
					SetFade(MODE_RANK, true);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{
			if (GetdJoykeyTrigger(PSKEY_CI, (CONTROLLER)i) || GetdJoykeyTrigger(PSKEY_START, (CONTROLLER)i))
			{
				nResultTime = 0;
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//切替
					SetFade(MODE_RANK, true);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{
			if (GetdJoykeyTrigger(NINKEY_A, (CONTROLLER)i) || GetdJoykeyTrigger(NINKEY_＋, (CONTROLLER)i))
			{
				nResultTime = 0;
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//切替
					SetFade(MODE_RANK, true);
				}
			}
		}
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{
			if (GetdJoykeyTrigger(DKEY_A, (CONTROLLER)i) || GetdJoykeyTrigger(DKEY_START, (CONTROLLER)i))
			{
				nResultTime = 0;
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//切替
					SetFade(MODE_RANK, true);
				}
			}
		}
	}

	if (nResultTime % RESULT_TIME == 0)
	{
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//切替
			SetFade(MODE_RANK, true);
		}
	}

	nResultTime++;
}

//-------------------
//描画処理
//-------------------
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < SCORE_MAX; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureResult);

		//背景の描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			i * VT_MAX,//始まりの番号
			2//ポリゴンの個数
		);
	}
}