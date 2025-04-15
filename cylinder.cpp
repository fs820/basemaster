//----------------------------------------
//
//ポリゴン表示処理[cylinder.cpp]
//Author fuma sato
//
//----------------------------------------

#include"cylinder.h"
#include"effect.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCylinder = NULL;//バッファのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffCylinder = NULL;//インデックスバッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureCylinder = NULL;
Cylinder g_aCylinder[CYLINDER_MAX];

//----------------------
//ポリゴンの初期化処理
//----------------------
void InitCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_3D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VT_MAX_CYLI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCylinder,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		CYLINDER_TEX,
		&g_pTextureCylinder
	);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * INDEX_CYLI_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffCylinder,
		NULL
	);

	int nCntCylinder, nCntCylinder2;
	for (nCntCylinder = 0; nCntCylinder < CYLINDER_MAX; nCntCylinder++)
	{
		g_aCylinder[nCntCylinder].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCylinder[nCntCylinder].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCylinder[nCntCylinder].bAlpha = false;
		g_aCylinder[nCntCylinder].bUse = false;
	}
	//1つ目


	g_pVtxBuffCylinder->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntCylinder = 0; nCntCylinder < CYLINDER_VNUM + 1; nCntCylinder++)
	{//1枚分のZ軸のループ
		for (nCntCylinder2 = 0; nCntCylinder2 < CYLINDER_HNUM + 1; nCntCylinder2++)
		{//1枚分のX軸のループ
			//座標設定
			float fangle = (360.0f / CYLINDER_HNUM) * nCntCylinder2 * (D3DX_PI / 180.0f);
			pVtx[nCntCylinder * (CYLINDER_HNUM + 1) + nCntCylinder2].pos = D3DXVECTOR3(sinf(fangle) * CYLINDER_RADIUS, CYLINDER_HEIGHT - (CYLINDER_HEIGHT / CYLINDER_VNUM) * nCntCylinder, cosf(fangle) * CYLINDER_RADIUS);

			//nor
			D3DXVECTOR3 npos = D3DXVECTOR3(0.0f, CYLINDER_HEIGHT - (CYLINDER_HEIGHT / CYLINDER_VNUM) * nCntCylinder, 0.0f), dpos = D3DXVECTOR3(sinf(fangle) * CYLINDER_RADIUS, CYLINDER_HEIGHT - (CYLINDER_HEIGHT / CYLINDER_VNUM) * nCntCylinder, cosf(fangle) * CYLINDER_RADIUS);
			npos = npos - dpos;
			D3DXVec3Normalize(&npos, &npos);
			pVtx[nCntCylinder * (CYLINDER_HNUM + 1) + nCntCylinder2].nor = npos;

			//カラー
			pVtx[nCntCylinder * (CYLINDER_HNUM + 1) + nCntCylinder2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ
			pVtx[nCntCylinder * (CYLINDER_HNUM + 1) + nCntCylinder2].tex = D3DXVECTOR2(((((CYLINDER_RADIUS + CYLINDER_RADIUS) * D3DX_PI) / CYLINDER_WIDTH_DEF) / CYLINDER_HNUM) * nCntCylinder2, ((CYLINDER_HEIGHT / CYLINDER_HEIGHT_DEF) / CYLINDER_VNUM) * nCntCylinder);
		}
	}

	g_pVtxBuffCylinder->Unlock();

	WORD* pIdx;
	g_pIdxBuffCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntCylinder = 0; nCntCylinder < INDEX_CYLI_NUM - (CYLINDER_VNUM - 1) * 2; nCntCylinder++)
	{
		if (nCntCylinder % ((CYLINDER_HNUM + 1) * 2) == 0 && nCntCylinder != 0)
		{
			//インデックス指定
			pIdx[nCntCylinder - 2 + ((nCntCylinder / ((CYLINDER_HNUM + 1) * 2)) * 2)] = (CYLINDER_HNUM + 1) - (CYLINDER_HNUM + 1) * ((nCntCylinder - 1) % 2) + ((nCntCylinder - 1) / 2);
			//インデックス指定
			pIdx[nCntCylinder - 1 + ((nCntCylinder / ((CYLINDER_HNUM + 1) * 2)) * 2)] = (CYLINDER_HNUM + 1) - (CYLINDER_HNUM + 1) * (nCntCylinder % 2) + (nCntCylinder / 2);
		}

		//インデックス指定
		pIdx[nCntCylinder + ((nCntCylinder / ((CYLINDER_HNUM + 1) * 2)) * 2)] = (CYLINDER_HNUM + 1) - (CYLINDER_HNUM + 1) * (nCntCylinder % 2) + (nCntCylinder / 2);
	}

	g_pIdxBuffCylinder->Unlock();
}

//-------------------
//ポリゴン終了処理
//-------------------
void UninitCylinder(void)
{
	//テクスチャの破棄
	if (g_pTextureCylinder != NULL)
	{
		g_pTextureCylinder->Release();
		g_pTextureCylinder = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffCylinder != NULL)
	{
		g_pVtxBuffCylinder->Release();
		g_pVtxBuffCylinder = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffCylinder != NULL)
	{
		g_pIdxBuffCylinder->Release();
		g_pIdxBuffCylinder = NULL;
	}
}

//-------------------
//ポリゴン更新処理
//-------------------
void UpdateCylinder(void)
{

}

//-------------------
//ポリゴン描画処理
//-------------------
void DrawCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATRIX mtxRot, mtxTrans;//計算マトリックス

	//デバイスの取得
	pDevice = GetDevice();

	int nCntCylinder;
	for (nCntCylinder = 0; nCntCylinder < CYLINDER_MAX; nCntCylinder++)
	{
		if (g_aCylinder[nCntCylinder].bUse && !g_aCylinder[nCntCylinder].bAlpha)
		{
			//マトリックス初期化
			D3DXMatrixIdentity(&g_aCylinder[nCntCylinder].mtxWorld);

			//向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCylinder[nCntCylinder].rot.y, g_aCylinder[nCntCylinder].rot.x, g_aCylinder[nCntCylinder].rot.z);
			D3DXMatrixMultiply(&g_aCylinder[nCntCylinder].mtxWorld, &g_aCylinder[nCntCylinder].mtxWorld, &mtxRot);

			//位置の計算
			D3DXMatrixTranslation(&mtxTrans, g_aCylinder[nCntCylinder].pos.x, g_aCylinder[nCntCylinder].pos.y, g_aCylinder[nCntCylinder].pos.z);
			D3DXMatrixMultiply(&g_aCylinder[nCntCylinder].mtxWorld, &g_aCylinder[nCntCylinder].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aCylinder[nCntCylinder].mtxWorld);

			//頂点バッファ
			pDevice->SetStreamSource(0, g_pVtxBuffCylinder, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffCylinder);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureCylinder);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				0,
				0,
				VT_MAX_CYLI,//頂点数
				0,
				POLYGON_CYLI_NUM//ポリゴンの個数
			);
		}
	}
}

//----------
//設定
//----------
void SetCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntCylinder;
	for (nCntCylinder = 0; nCntCylinder < CYLINDER_MAX; nCntCylinder++)
	{
		if (!g_aCylinder[nCntCylinder].bUse)
		{
			g_aCylinder[nCntCylinder].pos = pos;
			g_aCylinder[nCntCylinder].rot = rot;
			g_aCylinder[nCntCylinder].bAlpha = false;
			g_aCylinder[nCntCylinder].bUse = true;
			break;
		}
	}
}