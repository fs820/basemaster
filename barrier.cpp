//----------------------------------------
//
//バリア処理[barrier.cpp]
//Author fuma sato
//
//----------------------------------------
#include"barrier.h"
#include"camera.h"
#include"stddx.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBarrier = NULL;//バッファのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffBarrier = NULL;//インデックスバッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBarrier = NULL;    //テクスチャのポインタ
Barrier g_aBarrier[BARRIER_MAX]; //情報体
//----------------------
//初期化処理
//----------------------
void InitBarrier(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへポインタ
	VERTEX_3D* pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * (VT_MAX_BARRIER * BARRIER_MAX + BARRIER_TOP_MAX * 10),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_DEFAULT,
		&g_pVtxBuffBarrier,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFileEx
	(
		pDevice,
		BARRIER_TEX,
		D3DX_DEFAULT,              // テクスチャの幅（D3DX_DEFAULTなら自動調整）
		D3DX_DEFAULT,              // テクスチャの高さ（D3DX_DEFAULTなら自動調整）
		D3DX_DEFAULT,              // ミップマップレベル（D3DX_DEFAULTで自動設定）
		0,                         // 使用しない場合は0（動的テクスチャならD3DUSAGE_DYNAMIC）
		D3DFMT_A8R8G8B8,           // フォーマット（D3DFMT_A8R8G8B8 など）
		D3DPOOL_DEFAULT,           // メモリプール（通常は D3DPOOL_MANAGED）
		D3DX_FILTER_TRIANGLE,      // 画像の拡縮フィルタ（D3DX_FILTER_LINEAR など）
		D3DX_FILTER_TRIANGLE,      // ミップマップのフィルタ
		0,                         // カラーキー（透明にしたい色）
		NULL,                      // `D3DXIMAGE_INFO`（画像情報を取得する場合）
		NULL,                      // パレットデータ（パレットなしならNULL）
		&g_pTextureBarrier
	);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * INDEX_NUM_BARRIER,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&g_pIdxBuffBarrier,
		NULL
	);

	int nCntBarrier;
	for (nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{//最大数回す
		//情報体の初期化
		g_aBarrier[nCntBarrier].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBarrier[nCntBarrier].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBarrier[nCntBarrier].Width = 0.0f;
		g_aBarrier[nCntBarrier].Height = 0.0f;
		g_aBarrier[nCntBarrier].Type = BTYPE_GATE_1;
		g_aBarrier[nCntBarrier].Switch = OFF;
		g_aBarrier[nCntBarrier].col = BARRIER_DEF_COLOR;
		g_aBarrier[nCntBarrier].bTop = false;
		g_aBarrier[nCntBarrier].bUse = false;
	}
	//1つ目

	//頂点の設定
	SetVertex3DYX
	(
		&g_pVtxBuffBarrier,
		0, BARRIER_MAX,
		BARRIER_YNUM, BARRIER_XNUM,
		BARRIER_WIDTH, BARRIER_HEIGHT,
		D3DXVECTOR3(0.0f, 0.0f, -1.0f), BARRIER_DEF_COLOR,
		BARRIER_WIDTH, BARRIER_HEIGHT,
		false, false
	);

	//蓋
	g_pVtxBuffBarrier->Lock(sizeof(VERTEX_3D) * VT_MAX_BARRIER * BARRIER_MAX, sizeof(VERTEX_3D) * 3 * 10, (void**)&pVtx, 0);

	for (int nCntTopBarrier = 0; nCntTopBarrier < BARRIER_TOP_MAX; nCntTopBarrier++)
	{
		for (int nCntFan = 0; nCntFan < 10; nCntFan++)
		{
			pVtx[nCntFan].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[nCntFan].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nCntFan].col = BARRIER_DEF_COLOR;
			pVtx[nCntFan].tex = D3DXVECTOR2(((BARRIER_WIDTH / BARRIER_WIDTH) / 10) * nCntFan, ((BARRIER_HEIGHT / BARRIER_HEIGHT) / 10) * nCntFan);
		}
		pVtx += 10;
	}

	g_pVtxBuffBarrier->Unlock();


	//インデックスの設定
	SetIndex
	(
		&g_pIdxBuffBarrier,
		BARRIER_YNUM, BARRIER_XNUM
	);
}

//-------------------
//終了処理
//-------------------
void UninitBarrier(void)
{
	for (int nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{//最大数回す
		g_aBarrier[nCntBarrier].bUse = false;
	}

	//テクスチャの破棄
	if (g_pTextureBarrier != NULL)
	{
		g_pTextureBarrier->Release();
		g_pTextureBarrier = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBarrier != NULL)
	{
		g_pVtxBuffBarrier->Release();
		g_pVtxBuffBarrier = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffBarrier != NULL)
	{
		g_pIdxBuffBarrier->Release();
		g_pIdxBuffBarrier = NULL;
	}
}

//-------------------
//更新処理
//-------------------
void UpdateBarrier(void)
{
	VERTEX_3D* pVtx;
	// 頂点バッファをロック
	g_pVtxBuffBarrier->Lock(0, 0, (void**)&pVtx, 0);
	static bool bLess[BARRIER_MAX] = { false };

	for (int nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{
		if (g_aBarrier[nCntBarrier].bUse == true)
		{
			if (g_aBarrier[nCntBarrier].bTop == true)
			{
				for (int nCntFan = 0; nCntFan < 10; nCntFan++)
				{
					// 頂点カラーの設定
					pVtx[BARRIER_MAX * VT_MAX_BARRIER + (g_aBarrier[nCntBarrier].Type - BTYPE_ISOLATION_1) * 10 + nCntFan].col = g_aBarrier[nCntBarrier].col;
				}
			}
			else
			{
				// 頂点カラーの設定
				pVtx[nCntBarrier * VT_MAX_BARRIER].col = g_aBarrier[nCntBarrier].col;
				pVtx[nCntBarrier * VT_MAX_BARRIER + 1].col = g_aBarrier[nCntBarrier].col;
				pVtx[nCntBarrier * VT_MAX_BARRIER + 2].col = g_aBarrier[nCntBarrier].col;
				pVtx[nCntBarrier * VT_MAX_BARRIER + 3].col = g_aBarrier[nCntBarrier].col;
			}

			if (bLess[nCntBarrier] == true)
			{
				g_aBarrier[nCntBarrier].col.a -= BARRIER_ALPHA_SPEED;
				if (g_aBarrier[nCntBarrier].col.a <= BARRIER_ALPHA_MIN)
				{
					bLess[nCntBarrier] = false;
				}
			}
			else
			{
				g_aBarrier[nCntBarrier].col.a += BARRIER_ALPHA_SPEED;
				if (g_aBarrier[nCntBarrier].col.a >= BARRIER_ALPHA_MAX)
				{
					bLess[nCntBarrier] = true;
				}
			}
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBarrier->Unlock();
}

//-------------------
//描画処理
//-------------------
void DrawBarrier(void)
{
	LPDIRECT3DDEVICE9 pDevice; // デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans; // 計算マトリックス

	// デバイスの取得
	pDevice = GetDevice();

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 現在のカリング設定を保存
	DWORD prevCullMode;
	pDevice->GetRenderState(D3DRS_CULLMODE, &prevCullMode);

	// カリングをオフ
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテスト
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// サンプラーステート
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);      // テクスチャ繰り返し(ミラー)
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);      // テクスチャ繰り返し(ミラー)

	// テクスチャステージステートの設定
	// 0
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 遠いほうから描画する
	static int nIdx[BARRIER_MAX]; // 遠い順格納用
	static int nLimit = 0;
	if (nLimit % 30 == 0)
	{// フレーム半分ごとに処理 (毎フレームやると重くなる可能性があるため)
		D3DXVECTOR3 PosList[BARRIER_MAX] = { D3DXVECTOR3(0.0f,0.0f,0.0f) }; // ポスリスト
		Camera* pCamera = GetCamera();
		for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
		{// ポスリスト作成
			if (g_aBarrier[nCnt].bUse && g_aBarrier[nCnt].Switch == ON)
			{
				PosList[nCnt] = g_aBarrier[nCnt].pos;
			}
			else
			{
				PosList[nCnt] = pCamera->posV;
			}
		}
		DrawCameraFarSort(nIdx, PosList, BARRIER_MAX); //遠い順にソート
	}
	nLimit++;

	for (int nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{
		if (g_aBarrier[nIdx[nCntBarrier]].bUse == false || g_aBarrier[nIdx[nCntBarrier]].Switch == OFF)
		{
			break;
		}

		// マトリックス初期化
		D3DXMatrixIdentity(&g_aBarrier[nIdx[nCntBarrier]].mtxWorld);

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBarrier[nIdx[nCntBarrier]].rot.y, g_aBarrier[nIdx[nCntBarrier]].rot.x, g_aBarrier[nIdx[nCntBarrier]].rot.z);
		D3DXMatrixMultiply(&g_aBarrier[nIdx[nCntBarrier]].mtxWorld, &g_aBarrier[nIdx[nCntBarrier]].mtxWorld, &mtxRot);

		// 位置の計算
		D3DXMatrixTranslation(&mtxTrans, g_aBarrier[nIdx[nCntBarrier]].pos.x, g_aBarrier[nIdx[nCntBarrier]].pos.y, g_aBarrier[nIdx[nCntBarrier]].pos.z);
		D3DXMatrixMultiply(&g_aBarrier[nIdx[nCntBarrier]].mtxWorld, &g_aBarrier[nIdx[nCntBarrier]].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aBarrier[nIdx[nCntBarrier]].mtxWorld);

		// 頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffBarrier, 0, sizeof(VERTEX_3D));

		if (g_aBarrier[nIdx[nCntBarrier]].bTop == false)
		{
			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffBarrier);
		}

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBarrier);

		for (int i = 0; i < BARRIER_DRAW_NUM; i++)
		{
			if (g_aBarrier[nIdx[nCntBarrier]].bTop == true)
			{
				// ポリゴンの描画
				pDevice->DrawPrimitive
				(
					D3DPT_TRIANGLEFAN,            // タイプ
					VT_MAX_BARRIER * BARRIER_MAX + (g_aBarrier[nIdx[nCntBarrier]].Type - BTYPE_ISOLATION_1) * 10,
					8                             // ポリゴンの個数
				);
			}
			else
			{
				// ポリゴンの描画
				pDevice->DrawIndexedPrimitive
				(
					D3DPT_TRIANGLESTRIP,        // タイプ
					VT_MAX_BARRIER * nIdx[nCntBarrier],
					0,
					VT_MAX_BARRIER,             // 頂点数
					0,
					POLYGON_NUM_BARRIER         // ポリゴンの個数
				);
			}
		}
	}

	// テクスチャステージステートを戻す
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	// ステージ 1 以降を無効化
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	// サンプラーステートを戻す
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);      // テクスチャ繰り返し(タイル)
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);      // テクスチャ繰り返し(タイル)

	// Zテストを戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// カリング設定を元に戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, prevCullMode);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//----------
//設定
//----------
int SetBarrier(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, BTYPE Type, bool bTop, float Corner)
{
	int nCntBarrier;
	for (nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{//最大数回す
		if (!g_aBarrier[nCntBarrier].bUse)
		{//使っていない
			//バリアの生成
			g_aBarrier[nCntBarrier].pos = pos;
			g_aBarrier[nCntBarrier].rot = rot;
			g_aBarrier[nCntBarrier].Width = Width;
			g_aBarrier[nCntBarrier].Height = Height;
			g_aBarrier[nCntBarrier].Type = Type;
			if (bTop)
			{
				VERTEX_3D* pVtx;

				//蓋
				g_pVtxBuffBarrier->Lock(sizeof(VERTEX_3D) * VT_MAX_BARRIER * BARRIER_MAX, sizeof(VERTEX_3D) * 3 * 10, (void**)&pVtx, 0);

				pVtx += (g_aBarrier[nCntBarrier].Type - BTYPE_ISOLATION_1) * 10;

				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(sinf(0.0f) * g_aBarrier[nCntBarrier].Height * 0.5f, 0.0f, cosf(0.0f) * g_aBarrier[nCntBarrier].Height * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f);
				pVtx[2].pos = D3DXVECTOR3(sinf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f) + D3DXVECTOR3(sinf(0.0f) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f);
				pVtx[3].pos = D3DXVECTOR3(sinf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f);
				pVtx[4].pos = D3DXVECTOR3(sinf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * 0.5f, 0.0f, cosf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f);
				pVtx[5].pos = D3DXVECTOR3(sinf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * 0.5f, 0.0f, cosf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f);
				pVtx[6].pos = D3DXVECTOR3(sinf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f);
				pVtx[7].pos = D3DXVECTOR3(sinf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * 0.5f) + D3DXVECTOR3(sinf(0.0f) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f) * g_aBarrier[nCntBarrier].Height * (1.0f - Corner) * 0.5f);
				pVtx[8].pos = D3DXVECTOR3(sinf(0.0f) * g_aBarrier[nCntBarrier].Height * 0.5f, 0.0f, cosf(0.0f) * g_aBarrier[nCntBarrier].Height * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * -0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f);
				pVtx[9].pos = D3DXVECTOR3(sinf(0.0f) * g_aBarrier[nCntBarrier].Height * 0.5f, 0.0f, cosf(0.0f) * g_aBarrier[nCntBarrier].Height * 0.5f) + D3DXVECTOR3(sinf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f, 0.0f, cosf(0.0f + D3DX_PI * 0.5f) * g_aBarrier[nCntBarrier].Width * (1.0f - Corner) * 0.5f);

				for (int nCntFan = 0; nCntFan < 10; nCntFan++)
				{
					pVtx[nCntFan].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					pVtx[nCntFan].col = BARRIER_DEF_COLOR;
					pVtx[nCntFan].tex = D3DXVECTOR2(((BARRIER_WIDTH / BARRIER_WIDTH) / 10) * nCntFan, ((BARRIER_HEIGHT / BARRIER_HEIGHT) / 10) * nCntFan);
				}

				g_pVtxBuffBarrier->Unlock();
			}
			else
			{
				SetVertex3DYX
				(
					&g_pVtxBuffBarrier,
					nCntBarrier, 1,
					BARRIER_YNUM, BARRIER_XNUM,
					Width, Height,
					D3DXVECTOR3(0.0f, 0.0f, -1.0f), BARRIER_DEF_COLOR,
					Width, Height,
					false, false
				);
			}
			g_aBarrier[nCntBarrier].Switch = ON;
			g_aBarrier[nCntBarrier].bTop = bTop;
			g_aBarrier[nCntBarrier].bUse = true;
			break;
		}
	}

	return nCntBarrier; //インデックスを返す
}

//--------------------------
//破棄
//--------------------------
void EndBarrier(BTYPE Type)
{
	for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
	{
		if (g_aBarrier[nCnt].bUse && g_aBarrier[nCnt].Type == Type)
		{
			g_aBarrier[nCnt].bUse = false;
		}
	}
}

//--------------------------
//情報取得
//--------------------------
Barrier* GetBarrier(void)
{
	return &g_aBarrier[0];
}

//--------------------------
//Switch
//--------------------------
void BarrierSwitch(BTYPE Type, SWITCH Switch)
{
	for (int nCount = 0; nCount < BARRIER_MAX; nCount++)
	{//最大数回す
		if (g_aBarrier[nCount].bUse && g_aBarrier[nCount].Type == Type)
		{//指定されたタイプ
			g_aBarrier[nCount].Switch = Switch; //オンオフ
		}
	}
}

//----------------------
//当たり判定
//----------------------
void CollisionBarrier(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fRadius, float fHeight)
{
	D3DXVECTOR3 aPos[2] = {}, Barriervec = {}, Posvec = {}, PosOldvec = {}, movevec = {}, Norvec = {}, Dovec = {}, Hit = {};
	int nCntBarrier;
	float BarrierCross, PosCross;

	for (nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{//壁の数
		if (g_aBarrier[nCntBarrier].bUse && g_aBarrier[nCntBarrier].Switch == ON)
		{//使っている前面のバリア
			if (g_aBarrier[nCntBarrier].Type >= BTYPE_ISOLATION_1)
			{
				aPos[0] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f));
				aPos[1] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f));

				if (g_aBarrier[nCntBarrier].bTop==true)
				{
					switch (g_aBarrier[nCntBarrier].Type)
					{
					case BTYPE_ISOLATION_1:
						Barriervec = aPos[1] - aPos[0];//壁のベクトル

						// 蓋の法線ベクトル
						Norvec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

						Posvec = *pPos - aPos[0];//壁に対するプレイヤーのベクトル
						PosOldvec = *pPosOld - aPos[0];//壁に対するプレイヤーの旧ベクトル
						movevec = *pPos - *pPosOld;//プレイヤーの移動ベクトル

						if ((Barriervec.x * Posvec.y) - (Barriervec.y * Posvec.x) <= 0.0f && (Barriervec.x * PosOldvec.y) - (Barriervec.y * PosOldvec.x) >= 0.0f && pPos->z - fRadius < g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Height * 0.5f && pPos->z + fRadius > g_aBarrier[nCntBarrier].pos.z - g_aBarrier[nCntBarrier].Height * 0.5f)
						{//壁の外側
							BarrierCross = (Barriervec.x * movevec.y) - (Barriervec.y * movevec.x);
							PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
							PosCross /= BarrierCross;
							if (PosCross >= -0.1f && PosCross < 1.1f)
							{
								Hit = aPos[0] + Barriervec * PosCross;
								movevec = *pPos - Hit;//プレイヤーの移動ベクトル
								Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
								*pPos += Dovec * 1.1f;

								D3DXVECTOR3 Space = *pPos - g_aBarrier[nCntBarrier].pos;
								Space.y = 0.0f;
								D3DXVec3Normalize(&Space, &Space);
								*pPos += Space * 5.0f;

								for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
								{
									if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
									{// バリアのリアクション
										g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
									}
								}
							}
						}
						break;
					case BTYPE_ISOLATION_2:
						Barriervec = aPos[1] - aPos[0];//壁のベクトル

						// 蓋の法線ベクトル
						Norvec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

						Posvec = *pPos - aPos[0];//壁に対するプレイヤーのベクトル
						PosOldvec = *pPosOld - aPos[0];//壁に対するプレイヤーの旧ベクトル
						movevec = *pPos - *pPosOld;//プレイヤーの移動ベクトル

						if ((Barriervec.x * Posvec.y) - (Barriervec.y * Posvec.x) <= 0.0f && (Barriervec.x * PosOldvec.y) - (Barriervec.y * PosOldvec.x) >= 0.0f && pPos->z - fRadius < g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Height * 0.5f && pPos->z + fRadius > g_aBarrier[nCntBarrier].pos.z - g_aBarrier[nCntBarrier].Height * 0.5f)
						{//壁の外側
							BarrierCross = (Barriervec.x * movevec.y) - (Barriervec.y * movevec.x);
							PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
							PosCross /= BarrierCross;
							if (PosCross >= -0.1f && PosCross < 1.1f)
							{
								Hit = aPos[0] + Barriervec * PosCross;
								movevec = *pPos - Hit;//プレイヤーの移動ベクトル
								Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
								*pPos += Dovec * 1.1f;

								D3DXVECTOR3 Space = *pPos - g_aBarrier[nCntBarrier].pos;
								Space.y = 0.0f;
								D3DXVec3Normalize(&Space, &Space);
								*pPos += Space * 5.0f;

								for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
								{
									if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
									{// バリアのリアクション
										g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
									}
								}
							}
						}
						break;
					case BTYPE_ISOLATION_3:
						Barriervec = aPos[0] - aPos[1];//壁のベクトル

						// 蓋の法線ベクトル
						Norvec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

						Posvec = *pPos - aPos[1];//壁に対するプレイヤーのベクトル
						PosOldvec = *pPosOld - aPos[1];//壁に対するプレイヤーの旧ベクトル
						movevec = *pPos - *pPosOld;//プレイヤーの移動ベクトル

						if ((Barriervec.x * Posvec.y) - (Barriervec.y * Posvec.x) <= 0.0f && (Barriervec.x * PosOldvec.y) - (Barriervec.y * PosOldvec.x) >= 0.0f && pPos->z - fRadius < g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Height * 0.5f && pPos->z + fRadius > g_aBarrier[nCntBarrier].pos.z - g_aBarrier[nCntBarrier].Height * 0.5f)
						{//壁の外側
							BarrierCross = (Barriervec.x * movevec.y) - (Barriervec.y * movevec.x);
							PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
							PosCross /= BarrierCross;
							if (PosCross >= -0.1f && PosCross < 1.1f)
							{
								Hit = aPos[1] + Barriervec * PosCross;
								movevec = *pPos - Hit;//プレイヤーの移動ベクトル
								Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
								*pPos += Dovec * 1.1f;

								D3DXVECTOR3 Space = *pPos - g_aBarrier[nCntBarrier].pos;
								Space.y = 0.0f;
								D3DXVec3Normalize(&Space, &Space);
								*pPos += Space * 5.0f;

								for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
								{
									if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
									{// バリアのリアクション
										g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
									}
								}
							}
						}
						break;
					}
				}
				else
				{
					Barriervec = aPos[0] - aPos[1];//壁のベクトル

					// 壁の法線ベクトルを計算
					Norvec = D3DXVECTOR3(Barriervec.z, Barriervec.y, -Barriervec.x);
					D3DXVec3Normalize(&Norvec, &Norvec);

					Posvec = *pPos - aPos[1];//壁に対するプレイヤーのベクトル
					PosOldvec = *pPosOld - aPos[1];//壁に対するプレイヤーの旧ベクトル
					movevec = *pPos - *pPosOld;//プレイヤーの移動ベクトル

					if ((Barriervec.z * Posvec.x) - (Barriervec.x * Posvec.z) <= 0.0f && (Barriervec.z * PosOldvec.x) - (Barriervec.x * PosOldvec.z) >= 0.0f && pPos->y < g_aBarrier[nCntBarrier].pos.y + g_aBarrier[nCntBarrier].Height && pPos->y + fHeight > g_aBarrier[nCntBarrier].pos.y - 100.0f)
					{//壁の外側
						BarrierCross = (Barriervec.z * movevec.x) - (Barriervec.x * movevec.z);
						PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
						PosCross /= BarrierCross;
						if (PosCross >= -0.1f && PosCross < 1.1f)
						{
							Hit = aPos[1] + Barriervec * PosCross;
							movevec = *pPos - Hit;//プレイヤーの移動ベクトル
							Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
							*pPos += Dovec * 1.1f;

							for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
							{
								if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
								{// バリアのリアクション
									g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
								}
							}
						}
					}
				}
			}
			else
			{
				const float offsetDistance = 125.0f; // バリアの前にオフセットする距離

				aPos[0] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f));
				aPos[1] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f));

				Barriervec = aPos[1] - aPos[0];//壁のベクトル

				// 壁の法線ベクトルを計算
				Norvec = D3DXVECTOR3(Barriervec.z, Barriervec.y, -Barriervec.x);
				D3DXVec3Normalize(&Norvec, &Norvec);

				// バリアの位置を法線方向にオフセット
				aPos[0] += Norvec * offsetDistance;
				aPos[1] += Norvec * offsetDistance;

				Posvec = *pPos - aPos[0];//壁に対するプレイヤーのベクトル
				PosOldvec = *pPosOld - aPos[0];//壁に対するプレイヤーの旧ベクトル
				movevec = *pPos - *pPosOld;//プレイヤーの移動ベクトル

				if ((Barriervec.z * Posvec.x) - (Barriervec.x * Posvec.z) <= 0.0f && (Barriervec.z * PosOldvec.x) - (Barriervec.x * PosOldvec.z) >= 0.0f && pPos->y < g_aBarrier[nCntBarrier].pos.y + g_aBarrier[nCntBarrier].Height && pPos->y + fHeight > g_aBarrier[nCntBarrier].pos.y - 100.0f)
				{//壁の外側
					BarrierCross = (Barriervec.z * movevec.x) - (Barriervec.x * movevec.z);
					PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
					PosCross /= BarrierCross;
					if (g_aBarrier[nCntBarrier].Type <= BTYPE_GATE_4)
					{
						if (PosCross >= -1.0f && PosCross < 2.0f)
						{
							Hit = aPos[0] + Barriervec * PosCross;
							movevec = *pPos - Hit;//プレイヤーの移動ベクトル
							Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
							*pPos += Dovec * 1.1f;

							for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
							{
								if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
								{// バリアのリアクション
									g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
								}
							}
						}
					}
					else
					{
						if (PosCross >= -0.1f && PosCross < 1.1f)
						{
							Hit = aPos[0] + Barriervec * PosCross;
							movevec = *pPos - Hit;//プレイヤーの移動ベクトル
							Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
							*pPos += Dovec * 1.1f;

							for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
							{
								if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
								{// バリアのリアクション
									g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
								}
							}
						}
					}
				}

				aPos[0] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f));
				aPos[1] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f));

				Barriervec = aPos[0] - aPos[1];//壁の反ベクトル

				// 壁の反法線ベクトルを計算
				Norvec = D3DXVECTOR3(Barriervec.z, Barriervec.y, -Barriervec.x);
				D3DXVec3Normalize(&Norvec, &Norvec);

				// バリアの位置を反法線方向にオフセット
				aPos[0] += Norvec * offsetDistance;
				aPos[1] += Norvec * offsetDistance;

				Posvec = *pPos - aPos[1];//壁に対するプレイヤーのベクトル
				PosOldvec = *pPosOld - aPos[1];//壁に対するプレイヤーの旧ベクトル
				movevec = *pPos - *pPosOld;//プレイヤーの移動ベクトル

				if ((Barriervec.z * Posvec.x) - (Barriervec.x * Posvec.z) <= 0.0f && (Barriervec.z * PosOldvec.x) - (Barriervec.x * PosOldvec.z) >= 0.0f && pPos->y < g_aBarrier[nCntBarrier].pos.y + g_aBarrier[nCntBarrier].Height && pPos->y + fHeight > g_aBarrier[nCntBarrier].pos.y - 100.0f)
				{//壁の外側
					BarrierCross = (Barriervec.z * movevec.x) - (Barriervec.x * movevec.z);
					PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
					PosCross /= BarrierCross;
					if (g_aBarrier[nCntBarrier].Type <= BTYPE_GATE_4)
					{
						if (PosCross >= -1.0f && PosCross < 2.0f)
						{
							Hit = aPos[1] + Barriervec * PosCross;
							movevec = *pPos - Hit;//プレイヤーの移動ベクトル
							Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
							*pPos += Dovec * 1.1f;

							for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
							{
								if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
								{// バリアのリアクション
									g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
								}
							}
						}
					}
					else
					{
						if (PosCross >= -0.1f && PosCross < 1.1f)
						{
							Hit = aPos[1] + Barriervec * PosCross;
							movevec = *pPos - Hit;//プレイヤーの移動ベクトル
							Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
							*pPos += Dovec * 1.1f;

							for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
							{
								if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
								{// バリアのリアクション
									g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
								}
							}
						}
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------
//反射
//--------------------------------------------------------------------------------
void ReflectionBarrier(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius)
{
	D3DXVECTOR3 aPos[2] = {}, Barriervec = {}, Posvec = {}, PosOldvec = {}, movevec = {}, Norvec = {}, Dovec = {}, Hit = {};
	int nCntBarrier;
	float BarrierCross, PosCross;
	for (nCntBarrier = 0; nCntBarrier < BARRIER_MAX; nCntBarrier++)
	{//壁の数
		if (g_aBarrier[nCntBarrier].bUse && g_aBarrier[nCntBarrier].Switch == ON)
		{//使っている前面のバリア
			if (g_aBarrier[nCntBarrier].Type >= BTYPE_ISOLATION_1)
			{
				const float offsetDistance = -125.0f; // バリアの前にオフセットする距離

				aPos[0] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f));
				aPos[1] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f));

				if (g_aBarrier[nCntBarrier].bTop == true)
				{
					switch (g_aBarrier[nCntBarrier].Type)
					{
					case BTYPE_ISOLATION_1:
						Barriervec = aPos[1] - aPos[0];//壁のベクトル

						// 蓋の法線ベクトル
						Norvec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

						Posvec = *pPos - aPos[0];//壁に対するベクトル
						PosOldvec = *pPosOld - aPos[0];//壁に対する旧ベクトル
						movevec = *pPos - *pPosOld;//移動ベクトル
						if ((Barriervec.x * Posvec.y) - (Barriervec.y * Posvec.x) <= 0.0f && (Barriervec.x * PosOldvec.y) - (Barriervec.y * PosOldvec.x) >= 0.0f && pPos->z - fRadius < g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Height * 0.5f && pPos->z + fRadius > g_aBarrier[nCntBarrier].pos.y - g_aBarrier[nCntBarrier].Height * 0.5f)
						{//壁の外側
							BarrierCross = (Barriervec.x * movevec.y) - (Barriervec.y * movevec.x);
							PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
							PosCross /= BarrierCross;
							if (PosCross >= 0.0f && PosCross < 1.0f)
							{
								Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
								*pMove += Dovec * 2.0f;
								*pPos += *pMove;

								for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
								{
									if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
									{// バリアのリアクション
										g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
									}
								}
							}
						}
						break;
					case BTYPE_ISOLATION_2:
						Barriervec = aPos[1] - aPos[0];//壁のベクトル

						// 蓋の法線ベクトル
						Norvec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

						Posvec = *pPos - aPos[0];//壁に対するベクトル
						PosOldvec = *pPosOld - aPos[0];//壁に対する旧ベクトル
						movevec = *pPos - *pPosOld;//移動ベクトル
						if ((Barriervec.x * Posvec.y) - (Barriervec.y * Posvec.x) <= 0.0f && (Barriervec.x * PosOldvec.y) - (Barriervec.y * PosOldvec.x) >= 0.0f && pPos->z - fRadius < g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Height * 0.5f && pPos->z + fRadius > g_aBarrier[nCntBarrier].pos.y - g_aBarrier[nCntBarrier].Height * 0.5f)
						{//壁の外側
							BarrierCross = (Barriervec.x * movevec.y) - (Barriervec.y * movevec.x);
							PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
							PosCross /= BarrierCross;
							if (PosCross >= 0.0f && PosCross < 1.0f)
							{
								Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
								*pMove += Dovec * 2.0f;
								*pPos += *pMove;

								for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
								{
									if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
									{// バリアのリアクション
										g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
									}
								}
							}
						}
						break;
					case BTYPE_ISOLATION_3:
						Barriervec = aPos[0] - aPos[1];//壁のベクトル

						// 蓋の法線ベクトル
						Norvec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

						Posvec = *pPos - aPos[1];//壁に対するベクトル
						PosOldvec = *pPosOld - aPos[1];//壁に対する旧ベクトル
						movevec = *pPos - *pPosOld;//移動ベクトル
						if ((Barriervec.x * Posvec.y) - (Barriervec.y * Posvec.x) <= 0.0f && (Barriervec.x * PosOldvec.y) - (Barriervec.y * PosOldvec.x) >= 0.0f && pPos->z - fRadius < g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Height * 0.5f && pPos->z + fRadius > g_aBarrier[nCntBarrier].pos.y - g_aBarrier[nCntBarrier].Height * 0.5f)
						{//壁の外側
							BarrierCross = (Barriervec.x * movevec.y) - (Barriervec.y * movevec.x);
							PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
							PosCross /= BarrierCross;
							if (PosCross >= 0.0f && PosCross < 1.0f)
							{
								Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
								*pMove += Dovec * 2.0f;
								*pPos += *pMove;

								for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
								{
									if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
									{// バリアのリアクション
										g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
									}
								}
							}
						}
						break;
					}
				}
				else
				{
					Barriervec = aPos[0] - aPos[1];//壁のベクトル

					// 壁の法線ベクトルを計算
					Norvec = D3DXVECTOR3(Barriervec.z, Barriervec.y, -Barriervec.x);
					D3DXVec3Normalize(&Norvec, &Norvec);

					// バリアの位置を法線方向にオフセット
					aPos[0] += Norvec * offsetDistance;
					aPos[1] += Norvec * offsetDistance;

					Posvec = *pPos - aPos[1];//壁に対するベクトル
					PosOldvec = *pPosOld - aPos[1];//壁に対する旧ベクトル
					movevec = *pPos - *pPosOld;//移動ベクトル
					if ((Barriervec.z * Posvec.x) - (Barriervec.x * Posvec.z) <= 0.0f && (Barriervec.z * PosOldvec.x) - (Barriervec.x * PosOldvec.z) >= 0.0f && pPos->y - fRadius < g_aBarrier[nCntBarrier].pos.y + g_aBarrier[nCntBarrier].Height && pPos->y + fRadius > g_aBarrier[nCntBarrier].pos.y)
					{//壁の外側
						BarrierCross = (Barriervec.z * movevec.x) - (Barriervec.x * movevec.z);
						PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
						PosCross /= BarrierCross;
						if (PosCross >= 0.0f && PosCross < 1.0f)
						{
							Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
							*pMove += Dovec * 2.0f;
							*pPos += *pMove;

							for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
							{
								if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
								{// バリアのリアクション
									g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
								}
							}
						}
					}
				}
			}
			else
			{
				aPos[0] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * -0.5f));
				aPos[1] = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + g_aBarrier[nCntBarrier].Width * 0.5f * sinf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f), g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z + g_aBarrier[nCntBarrier].Width * 0.5f * cosf(g_aBarrier[nCntBarrier].rot.y + D3DX_PI * 0.5f));

				Barriervec = aPos[1] - aPos[0];//壁のベクトル
				Posvec = *pPos - aPos[0];//壁に対するベクトル
				PosOldvec = *pPosOld - aPos[0];//壁に対する旧ベクトル
				movevec = *pPos - *pPosOld;//移動ベクトル
				if ((Barriervec.z * Posvec.x) - (Barriervec.x * Posvec.z) <= 0.0f && (Barriervec.z * PosOldvec.x) - (Barriervec.x * PosOldvec.z) >= 0.0f && pPos->y - fRadius < g_aBarrier[nCntBarrier].pos.y + g_aBarrier[nCntBarrier].Height && pPos->y + fRadius > g_aBarrier[nCntBarrier].pos.y)
				{//壁の外側
					BarrierCross = (Barriervec.z * movevec.x) - (Barriervec.x * movevec.z);
					PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
					PosCross /= BarrierCross;
					if (PosCross >= 0.0f && PosCross < 1.0f)
					{
						Norvec = D3DXVECTOR3(Barriervec.z, Barriervec.y, -Barriervec.x);
						D3DXVec3Normalize(&Norvec, &Norvec);
						Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
						*pMove += Dovec * 2.0f;
						*pPos += *pMove;

						for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
						{
							if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
							{// バリアのリアクション
								g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
							}
						}
					}
				}

				Barriervec = aPos[0] - aPos[1];//壁のベクトル
				Posvec = *pPos - aPos[1];//壁に対するベクトル
				PosOldvec = *pPosOld - aPos[1];//壁に対する旧ベクトル
				movevec = *pPos - *pPosOld;//移動ベクトル
				if ((Barriervec.z * Posvec.x) - (Barriervec.x * Posvec.z) <= 0.0f && (Barriervec.z * PosOldvec.x) - (Barriervec.x * PosOldvec.z) >= 0.0f && pPos->y - fRadius < g_aBarrier[nCntBarrier].pos.y + g_aBarrier[nCntBarrier].Height && pPos->y + fRadius > g_aBarrier[nCntBarrier].pos.y)
				{//壁の外側
					BarrierCross = (Barriervec.z * movevec.x) - (Barriervec.x * movevec.z);
					PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
					PosCross /= BarrierCross;
					if (PosCross >= 0.0f && PosCross < 1.0f)
					{
						Norvec = D3DXVECTOR3(Barriervec.z, Barriervec.y, -Barriervec.x);
						D3DXVec3Normalize(&Norvec, &Norvec);
						Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
						*pMove += Dovec * 2.0f;
						*pPos += *pMove;

						for (int nCnt = 0; nCnt < BARRIER_MAX; nCnt++)
						{
							if (g_aBarrier[nCnt].bUse && g_aBarrier[nCntBarrier].Type == g_aBarrier[nCnt].Type)
							{// バリアのリアクション
								g_aBarrier[nCnt].col.a = BARRIER_ALPHA_REACTION;
							}
						}
					}
				}
			}
		}
	}
}