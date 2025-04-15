//----------------------------------------
//
//Space処理[space.cpp]
//Author fuma sato
//
//----------------------------------------

#include "space.h"
#include "camera.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSpace = NULL;    //バッファのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSpace = NULL;     //インデックスバッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureSpace[SPACE_TEX_MAX] = { NULL };  //テクスチャのポインタ
Space g_Space;                                     //情報格納関数
//----------------------
//初期化処理
//----------------------
void InitSpace(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_3D_MULTI* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();


	//バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D_MULTI) * VT_MAX_SP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D_MULTI,
		D3DPOOL_DEFAULT,
		&g_pVtxBuffSpace,
		NULL
	);

	//情報の初期化
	for (int nCntTex = 0; nCntTex < SPACE_TEX_MAX; nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureSpace[nCntTex] != NULL)
		{
			g_pTextureSpace[nCntTex]->Release();
			g_pTextureSpace[nCntTex] = NULL;
		}

		g_Space.Texture[nCntTex].TexPass = TexPass[nCntTex];

		D3DXIMAGE_INFO ImageInfo;
		//テクスチャの読み込み
		HRESULT hr = D3DXCreateTextureFromFileEx
		(
			pDevice,
			g_Space.Texture[nCntTex].TexPass.aName,
			D3DX_DEFAULT,              // テクスチャの幅（D3DX_DEFAULTなら自動調整）
			D3DX_DEFAULT,              // テクスチャの高さ（D3DX_DEFAULTなら自動調整）
			D3DX_DEFAULT,              // ミップマップレベル（D3DX_DEFAULTで自動設定）
			0,                         // 使用しない場合は0（動的テクスチャならD3DUSAGE_DYNAMIC）
			D3DFMT_A8R8G8B8,           // フォーマット（A8R8G8B8 png X8R8G8B8 jpg）
			D3DPOOL_DEFAULT,           // メモリプール（通常は D3DPOOL_MANAGED）
			D3DX_FILTER_TRIANGLE,      // 画像の拡縮フィルタ（D3DX_FILTER_LINEAR など）
			D3DX_FILTER_TRIANGLE,      // ミップマップのフィルタ
			0,                         // カラーキー（透明にしたい色）
			&ImageInfo,                // `D3DXIMAGE_INFO`（画像情報を取得する場合）
			NULL,                      // パレットデータ（パレットなしならNULL）
			&g_pTextureSpace[nCntTex]
		);

		if (SUCCEEDED(hr))
		{
			g_Space.Texture[nCntTex].TexSize = D3DXVECTOR2((float)ImageInfo.Width, (float)ImageInfo.Height) * TexScale[nCntTex];
		}
		else
		{
			//テクスチャの破棄
			if (g_pTextureSpace[nCntTex] != NULL)
			{
				g_pTextureSpace[nCntTex]->Release();
				g_pTextureSpace[nCntTex] = NULL;
			}

			//テクスチャの読み込み
			HRESULT hr = D3DXCreateTextureFromFileEx
			(
				pDevice,
				g_Space.Texture[nCntTex].TexPass.aName,
				D3DX_DEFAULT,              // テクスチャの幅（D3DX_DEFAULTなら自動調整）
				D3DX_DEFAULT,              // テクスチャの高さ（D3DX_DEFAULTなら自動調整）
				D3DX_DEFAULT,              // ミップマップレベル（D3DX_DEFAULTで自動設定）
				0,                         // 使用しない場合は0（動的テクスチャならD3DUSAGE_DYNAMIC）
				D3DFMT_A8R8G8B8,           // フォーマット（A8R8G8B8 png X8R8G8B8 jpg）
				D3DPOOL_SYSTEMMEM,          // メモリプール（通常は D3DPOOL_MANAGED）
				D3DX_FILTER_LINEAR,        // 画像の拡縮フィルタ（D3DX_FILTER_LINEAR など）
				D3DX_FILTER_LINEAR,        // ミップマップのフィルタ
				0,                         // カラーキー（透明にしたい色）
				0,                         // `D3DXIMAGE_INFO`（画像情報を取得する場合）
				NULL,                      // パレットデータ（パレットなしならNULL）
				&g_pTextureSpace[nCntTex]
			);

			g_Space.Texture[nCntTex].TexSize = D3DXVECTOR2(6283.0f, 1571.0f) * TexScale[nCntTex];
		}
	}
	g_Space.bUse = false;//使用

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * INDEX_SP_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&g_pIdxBuffSpace,
		NULL
	);

	//頂点情報の設定
	g_pVtxBuffSpace->Lock(0, 0, (void**)&pVtx, 0);

	//極頂点
	pVtx[0].pos = D3DXVECTOR3(0.0f, SPACE_RADIUS, 0.0f);

	//法線ベクトル
	D3DXVECTOR3 npos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), dpos = D3DXVECTOR3(0.0f, SPACE_RADIUS, 0.0f);
	npos = npos - dpos;
	D3DXVec3Normalize(&npos, &npos);
	pVtx[0].nor = npos;

	//カラー
	pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.1f);

	//テクスチャ
	pVtx[0].tex1 = D3DXVECTOR2(0.0f,0.0f);

	//テクスチャ
	pVtx[0].tex2 = D3DXVECTOR2(0.0f, 0.0f);

	for (int nCntSpace = 0; nCntSpace < SPACE_VNUM; nCntSpace++)
	{//1枚分のZ軸のループ
		for (int nCntSpace2 = 0; nCntSpace2 < SPACE_HNUM + 1; nCntSpace2++)
		{//1枚分のX軸のループ

			//円形の角度
			float fangleH = (360.0f / SPACE_HNUM) * nCntSpace2 * (D3DX_PI / 180.0f), fangleV = (SPACE_ANG / SPACE_VNUM) * (nCntSpace + 1) * (D3DX_PI / 180.0f);

			//座標設定
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].pos = D3DXVECTOR3(sinf(fangleH) * sinf(fangleV) * SPACE_RADIUS, cosf(fangleV) * SPACE_RADIUS, cosf(fangleH) * sinf(fangleV) * SPACE_RADIUS);

			//nor
			D3DXVECTOR3 npos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), dpos = D3DXVECTOR3(sinf(fangleH) * sinf(fangleV) * SPACE_RADIUS, cosf(fangleV) * SPACE_RADIUS, cosf(fangleH) * sinf(fangleV) * SPACE_RADIUS);
			npos = npos - dpos;
			D3DXVec3Normalize(&npos, &npos);
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].nor = npos;

			//カラー
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.1f);

			//テクスチャ
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].tex1 = D3DXVECTOR2((float)(((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI) / g_Space.Texture[0].TexSize.x) / SPACE_HNUM) * nCntSpace2), (float)((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI * 0.5f) / g_Space.Texture[0].TexSize.y) / SPACE_VNUM) * nCntSpace);

			//テクスチャ
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].tex2 = D3DXVECTOR2((float)(((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI) / g_Space.Texture[1].TexSize.x) / SPACE_HNUM) * nCntSpace2), (float)((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI * 0.5f) / g_Space.Texture[1].TexSize.y) / SPACE_VNUM) * nCntSpace);
		}
	}

	g_pVtxBuffSpace->Unlock();

	//インデックスバッファの設定
	WORD* pIdx;
	g_pIdxBuffSpace->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	for (int nCntSpace = 1; nCntSpace < SPACE_HNUM + 2; nCntSpace++)
	{//FAN部分
		pIdx[nCntSpace] = SPACE_HNUM + 2 - nCntSpace;
	}
	for (int nCntSpace = SPACE_HNUM + 2; nCntSpace < INDEX_SP_NUM - (SPACE_VNUM - 2) * 2; nCntSpace++)
	{//STRIP部分
		if ((nCntSpace - (SPACE_HNUM + 2)) % (((SPACE_HNUM + 1) * 2)) == 0 && (nCntSpace - (SPACE_HNUM + 2)) != 0)
		{//縮退ポリゴン
			//インデックス指定
			pIdx[nCntSpace - 2 + (((nCntSpace - (SPACE_HNUM + 2)) / ((SPACE_HNUM + 1) * 2)) * 2)] = (SPACE_HNUM + 1) - (SPACE_HNUM + 1) * (((nCntSpace - (SPACE_HNUM + 2)) - 1) % 2) + (((nCntSpace - (SPACE_HNUM + 2)) - 1) / 2) + 1;

			//インデックス指定
			pIdx[nCntSpace - 1 + (((nCntSpace - (SPACE_HNUM + 2)) / ((SPACE_HNUM + 1) * 2)) * 2)] = (SPACE_HNUM + 1) - (SPACE_HNUM + 1) * ((nCntSpace - (SPACE_HNUM + 2)) % 2) + ((nCntSpace - (SPACE_HNUM + 2)) / 2) + 1;
		}

		//インデックス指定
		pIdx[nCntSpace + (((nCntSpace - (SPACE_HNUM + 2)) / ((SPACE_HNUM + 1) * 2)) * 2)] = (SPACE_HNUM + 1) - (SPACE_HNUM + 1) * ((nCntSpace - (SPACE_HNUM + 2)) % 2) + ((nCntSpace - (SPACE_HNUM + 2)) / 2) + 1;
	}

	g_pIdxBuffSpace->Unlock();
}

//-------------------
//終了処理
//-------------------
void UninitSpace(void)
{
	g_Space.bUse = false;//使用

	for (int nCnt = 0; nCnt < SPACE_TEX_MAX; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureSpace[nCnt] != NULL)
		{
			g_pTextureSpace[nCnt]->Release();
			g_pTextureSpace[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSpace != NULL)
	{
		g_pVtxBuffSpace->Release();
		g_pVtxBuffSpace = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffSpace != NULL)
	{
		g_pIdxBuffSpace->Release();
		g_pIdxBuffSpace = NULL;
	}
}

//-------------------
//更新処理
//-------------------
void UpdateSpace(void)
{
	VERTEX_3D_MULTI* pVtx;//頂点情報ポインタ
	static float nCntTex;//回転度数
	int nCntSpace, nCntSpace2;//頂点カウント用

	//頂点のテクスチャを回転する
	g_pVtxBuffSpace->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex1 = D3DXVECTOR2(0.0f, 0.0f);

	pVtx[0].tex2 = D3DXVECTOR2(0.0f, 0.0f);

	for (nCntSpace = 0; nCntSpace < SPACE_VNUM; nCntSpace++)
	{//1枚分のZ軸のループ
		for (nCntSpace2 = 0; nCntSpace2 < SPACE_HNUM + 1; nCntSpace2++)
		{//1枚分のX軸のループ
			//テクスチャ
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].tex1 = D3DXVECTOR2((float)(((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI) / g_Space.Texture[0].TexSize.x) / SPACE_HNUM) * nCntSpace2) + nCntTex, (float)((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI * 0.5f) / g_Space.Texture[0].TexSize.y) / SPACE_VNUM) * nCntSpace - nCntTex);

			//テクスチャ
			pVtx[nCntSpace * (SPACE_HNUM + 1) + nCntSpace2 + 1].tex2 = D3DXVECTOR2((float)(((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI) / g_Space.Texture[1].TexSize.x) / SPACE_HNUM) * nCntSpace2) - nCntTex - D3DX_PI, (float)((((SPACE_RADIUS + SPACE_RADIUS) * D3DX_PI * 0.5f) / g_Space.Texture[1].TexSize.y) / SPACE_VNUM) * nCntSpace + nCntTex - D3DX_PI * 0.5f);
		}
	}

	g_pVtxBuffSpace->Unlock();
	nCntTex += D3DX_PI / g_Space.Texture[1].TexSize.x * 0.2f;//回転
}

//-------------------
//描画処理
//-------------------
void DrawSpace(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATRIX mtxRot, mtxTrans;//計算マトリックス

	//デバイスの取得
	pDevice = GetDevice();

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテスト
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//// サンプラーステート
	//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);      // テクスチャ繰り返し(ミラー)
	//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);      // テクスチャ繰り返し(ミラー)
	//pDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);      // テクスチャ繰り返し(ミラー)
	//pDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);      // テクスチャ繰り返し(ミラー)

	// テクスチャステージステートの設定
	// 0
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_ADD);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	// 1
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_ADD);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	if (g_Space.bUse)
	{//使用していたら
		//マトリックス初期化
		D3DXMatrixIdentity(&g_Space.mtxWorld);

		//位置の計算
		extern  Camera g_camera[1];
		D3DXMatrixTranslation(&mtxTrans, g_camera[0].posV.x, g_camera[0].posV.y, g_camera[0].posV.z);
		D3DXMatrixMultiply(&g_Space.mtxWorld, &g_Space.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Space.mtxWorld);

		//頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffSpace, 0, sizeof(VERTEX_3D_MULTI));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffSpace);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D_MULTI);

		for (int nCnt = 0; nCnt < SPACE_TEX_MAX; nCnt++)
		{
			//テクスチャの設定
			pDevice->SetTexture(nCnt, g_pTextureSpace[nCnt]);
		}

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			//極ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLEFAN,//タイプ
				0,
				0,
				SPACE_HNUM + 2,//頂点数
				0,
				SPACE_HNUM//ポリゴンの個数
			);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				0,
				0,
				VT_MAX_SP - (SPACE_HNUM + 2),//頂点数
				SPACE_HNUM + 2,
				POLYGON_SP_NUM - SPACE_HNUM//ポリゴンの個数
			);
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
	pDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);      // テクスチャ繰り返し(タイル)
	pDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);      // テクスチャ繰り返し(タイル)

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Zテストを戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//----------
//設定処理
//----------
void SetSpace(void)
{
	if (!g_Space.bUse)
	{//使用していない
		//有効化
		g_Space.bUse = true;//使用
	}
}