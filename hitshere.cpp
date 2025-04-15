//----------------------------------------
//
//ポリゴン表示処理[hitshere.cpp]
//Author fuma sato
//
//----------------------------------------

#include"hitshere.h"
#include"player.h"
#include"particle.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHitShere = NULL;//バッファのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffHitShere = NULL;//インデックスバッファのポインタ
HitShere g_aHitShere[HITSHERE_MAX];

//----------------------
//ポリゴンの初期化処理
//----------------------
void InitHitShere(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_3D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VT_MAX_HS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHitShere,
		NULL
	);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * INDEX_HS_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffHitShere,
		NULL
	);

	int nCntHitShere, nCntHitShere2;
	for (nCntHitShere = 0; nCntHitShere < HITSHERE_MAX; nCntHitShere++)
	{
		g_aHitShere[nCntHitShere].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aHitShere[nCntHitShere].scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aHitShere[nCntHitShere].nLife = 0;
		g_aHitShere[nCntHitShere].Master = MAX;
		g_aHitShere[nCntHitShere].bUse = false;
	}
	//1つ目

	g_pVtxBuffHitShere->Lock(0, 0, (void**)&pVtx, 0);

	//極頂点
	pVtx[0].pos = D3DXVECTOR3(0.0f, HITSHERE_RADIUS, 0.0f);

	D3DXVECTOR3 npos = D3DXVECTOR3(0.0f, HITSHERE_RADIUS, 0.0f), dpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	npos = npos - dpos;
	D3DXVec3Normalize(&npos, &npos);
	pVtx[0].nor = npos;

	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	for (nCntHitShere = 0; nCntHitShere < HITSHERE_VNUM - 1; nCntHitShere++)
	{//1枚分のZ軸のループ
		for (nCntHitShere2 = 0; nCntHitShere2 < HITSHERE_HNUM + 1; nCntHitShere2++)
		{//1枚分のX軸のループ

			//円形の角度
			float fangleH = (360.0f / HITSHERE_HNUM) * nCntHitShere2 * (D3DX_PI / 180.0f), fangleV = (HITSHERE_ANG / HITSHERE_VNUM) * (nCntHitShere + 1) * (D3DX_PI / 180.0f);

			//座標設定
			pVtx[nCntHitShere * (HITSHERE_HNUM + 1) + nCntHitShere2 + 1].pos = D3DXVECTOR3(sinf(fangleH) * sinf(fangleV) * HITSHERE_RADIUS, cosf(fangleV) * HITSHERE_RADIUS, cosf(fangleH) * sinf(fangleV) * HITSHERE_RADIUS);

			//nor
			D3DXVECTOR3 npos = D3DXVECTOR3(sinf(fangleH) * sinf(fangleV) * HITSHERE_RADIUS, cosf(fangleV) * HITSHERE_RADIUS, cosf(fangleH) * sinf(fangleV) * HITSHERE_RADIUS), dpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			npos = npos - dpos;
			D3DXVec3Normalize(&npos, &npos);
			pVtx[nCntHitShere * (HITSHERE_HNUM + 1) + nCntHitShere2 + 1].nor = npos;

			//カラー
			pVtx[nCntHitShere * (HITSHERE_HNUM + 1) + nCntHitShere2 + 1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);

			//テクスチャ
			pVtx[nCntHitShere * (HITSHERE_HNUM + 1) + nCntHitShere2 + 1].tex = D3DXVECTOR2(0.0f,0.0f);
		}
	}

	//極頂点
	pVtx[VT_MAX_HS - 1].pos = D3DXVECTOR3(0.0f, -HITSHERE_RADIUS, 0.0f);

	npos = D3DXVECTOR3(0.0f, -HITSHERE_RADIUS, 0.0f), dpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	npos = npos - dpos;
	D3DXVec3Normalize(&npos, &npos);
	pVtx[VT_MAX_HS - 1].nor = npos;

	pVtx[VT_MAX_HS - 1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);

	pVtx[VT_MAX_HS - 1].tex = D3DXVECTOR2(0.0f,0.0f);

	g_pVtxBuffHitShere->Unlock();

	WORD* pIdx;
	int nIdx[INDEX_HS_NUM];
	g_pIdxBuffHitShere->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	nIdx[0] = 0;
	for (nCntHitShere = 1; nCntHitShere < HITSHERE_HNUM + 2; nCntHitShere++)
	{//FAN部分
		pIdx[nCntHitShere] = nCntHitShere;
		nIdx[nCntHitShere] = nCntHitShere;
	}
	for (nCntHitShere = HITSHERE_HNUM + 2; nCntHitShere < INDEX_HS_NUM - (HITSHERE_VNUM - 2) * 2 - HITSHERE_HNUM; nCntHitShere++)
	{//HSRIP部分
		if ((nCntHitShere - (HITSHERE_HNUM + 2)) % (((HITSHERE_HNUM + 1) * 2)) == 0 && (nCntHitShere - (HITSHERE_HNUM + 2)) != 0)
		{//縮退ポリゴン
			//インデックス指定
			pIdx[nCntHitShere - 2 + (((nCntHitShere - (HITSHERE_HNUM + 2)) / ((HITSHERE_HNUM + 1) * 2)) * 2)] = (HITSHERE_HNUM + 1) - (HITSHERE_HNUM + 1) * (((nCntHitShere - (HITSHERE_HNUM + 2)) - 1) % 2) + (VT_MAX_HS - (HITSHERE_HNUM + 3)) - (((nCntHitShere - (HITSHERE_HNUM + 2)) - 1) / 2);
			nIdx[nCntHitShere - 2 + (((nCntHitShere - (HITSHERE_HNUM + 2)) / ((HITSHERE_HNUM + 1) * 2)) * 2)] = (HITSHERE_HNUM + 1) - (HITSHERE_HNUM + 1) * (((nCntHitShere - (HITSHERE_HNUM + 2)) - 1) % 2) + (VT_MAX_HS - (HITSHERE_HNUM + 3)) - (((nCntHitShere - (HITSHERE_HNUM + 2)) - 1) / 2);

			//インデックス指定
			pIdx[nCntHitShere - 1 + (((nCntHitShere - (HITSHERE_HNUM + 2)) / ((HITSHERE_HNUM + 1) * 2)) * 2)] = (HITSHERE_HNUM + 1) - (HITSHERE_HNUM + 1) * ((nCntHitShere - (HITSHERE_HNUM + 2)) % 2) + (VT_MAX_HS - (HITSHERE_HNUM + 3)) - ((nCntHitShere - (HITSHERE_HNUM + 2)) / 2);
			nIdx[nCntHitShere - 1 + (((nCntHitShere - (HITSHERE_HNUM + 2)) / ((HITSHERE_HNUM + 1) * 2)) * 2)] = (HITSHERE_HNUM + 1) - (HITSHERE_HNUM + 1) * ((nCntHitShere - (HITSHERE_HNUM + 2)) % 2) + (VT_MAX_HS - (HITSHERE_HNUM + 3)) - ((nCntHitShere - (HITSHERE_HNUM + 2)) / 2);
		}

		//インデックス指定
		pIdx[nCntHitShere + (((nCntHitShere - (HITSHERE_HNUM + 2)) / ((HITSHERE_HNUM + 1) * 2)) * 2)] = (HITSHERE_HNUM + 1) - (HITSHERE_HNUM + 1) * ((nCntHitShere - (HITSHERE_HNUM + 2)) % 2) + (VT_MAX_HS - (HITSHERE_HNUM + 3)) - ((nCntHitShere - (HITSHERE_HNUM + 2)) / 2);
		nIdx[nCntHitShere + (((nCntHitShere - (HITSHERE_HNUM + 2)) / ((HITSHERE_HNUM + 1) * 2)) * 2)] = (HITSHERE_HNUM + 1) - (HITSHERE_HNUM + 1) * ((nCntHitShere - (HITSHERE_HNUM + 2)) % 2) + (VT_MAX_HS - (HITSHERE_HNUM + 3)) - ((nCntHitShere - (HITSHERE_HNUM + 2)) / 2);
	}

	for (nCntHitShere = 1; nCntHitShere < HITSHERE_HNUM + 2; nCntHitShere++)
	{//FAN部分
		pIdx[INDEX_HS_NUM - nCntHitShere] = VT_MAX_HS - (HITSHERE_HNUM + 2) + (nCntHitShere - 1);
		nIdx[INDEX_HS_NUM - nCntHitShere] = VT_MAX_HS - (HITSHERE_HNUM + 2) + (nCntHitShere - 1);
	}
	pIdx[INDEX_HS_NUM - (HITSHERE_HNUM + 2)] = VT_MAX_HS - 1;
	nIdx[INDEX_HS_NUM - (HITSHERE_HNUM + 2)] = VT_MAX_HS - 1;

	g_pIdxBuffHitShere->Unlock();
}

//-------------------
//ポリゴン終了処理
//-------------------
void UninitHitShere(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffHitShere != NULL)
	{
		g_pVtxBuffHitShere->Release();
		g_pVtxBuffHitShere = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffHitShere != NULL)
	{
		g_pIdxBuffHitShere->Release();
		g_pIdxBuffHitShere = NULL;
	}
}

//-------------------
//ポリゴン更新処理
//-------------------
void UpdateHitShere(void)
{
	int nCntHitShere;
	for (nCntHitShere = 0; nCntHitShere < HITSHERE_MAX; nCntHitShere++)
	{//球の数
		if (g_aHitShere[nCntHitShere].bUse)
		{//使用している
			g_aHitShere[nCntHitShere].nLife--;
			if (g_aHitShere[nCntHitShere].nLife <= 0)
			{
				g_aHitShere[nCntHitShere].bUse = false;
			}
		}
	}
}

//-------------------
//ポリゴン描画処理
//-------------------
void DrawHitShere(void)
{
#ifdef _DEBUG

	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;//計算マトリックス

	//デバイスの取得
	pDevice = GetDevice();

	int nCntHitShere;
	for (nCntHitShere = 0; nCntHitShere < HITSHERE_MAX; nCntHitShere++)
	{
		if (g_aHitShere[nCntHitShere].bUse)
		{
			//マトリックス初期化
			D3DXMatrixIdentity(&g_aHitShere[nCntHitShere].mtxWorld);

			//大きさの反映
			D3DXMatrixScaling(&mtxScale, g_aHitShere[nCntHitShere].scale.x, g_aHitShere[nCntHitShere].scale.y, g_aHitShere[nCntHitShere].scale.z);
			D3DXMatrixMultiply(&g_aHitShere[nCntHitShere].mtxWorld, &g_aHitShere[nCntHitShere].mtxWorld, &mtxScale);

			//位置の計算
			D3DXMatrixTranslation(&mtxTrans, g_aHitShere[nCntHitShere].pos.x, g_aHitShere[nCntHitShere].pos.y, g_aHitShere[nCntHitShere].pos.z);
			D3DXMatrixMultiply(&g_aHitShere[nCntHitShere].mtxWorld, &g_aHitShere[nCntHitShere].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aHitShere[nCntHitShere].mtxWorld);

			//頂点バッファ
			pDevice->SetStreamSource(0, g_pVtxBuffHitShere, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffHitShere);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			//極ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLEFAN,//タイプ
				0,
				0,
				HITSHERE_HNUM + 2,//頂点数
				0,
				HITSHERE_HNUM//ポリゴンの個数
			);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				0,
				0,
				VT_MAX_HS - (HITSHERE_HNUM + 2) * 2,//頂点数
				HITSHERE_HNUM + 2,
				POLYGON_HS_NUM - HITSHERE_HNUM * 2//ポリゴンの個数
			);

			//極ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLEFAN,//タイプ
				0,
				0,
				HITSHERE_HNUM + 2,//頂点数
				INDEX_HS_NUM - (HITSHERE_HNUM + 2),
				HITSHERE_HNUM//ポリゴンの個数
			);
		}
	}

#endif
}

//----------
//設定
//----------
void SetHitShere(D3DXVECTOR3 pos, D3DXVECTOR3 scale, int Life, MASTER Master)
{
	int nCntHitShere;
	for (nCntHitShere = 0; nCntHitShere < HITSHERE_MAX; nCntHitShere++)
	{//球の数
		if (!g_aHitShere[nCntHitShere].bUse)
		{//使用していない
			//有効化
			g_aHitShere[nCntHitShere].pos = pos;
			g_aHitShere[nCntHitShere].scale = scale;
			g_aHitShere[nCntHitShere].nLife = Life;
			g_aHitShere[nCntHitShere].Master = Master;
			g_aHitShere[nCntHitShere].bUse = true;
			break;
		}
	}
}

//--------------------------
//当たり判定
//--------------------------
bool TriggerSphere(MASTER Master, D3DXVECTOR3 pos, float fWidth)
{
	for (int nCntSphere = 0; nCntSphere < HITSHERE_MAX; nCntSphere++)
	{
		if (g_aHitShere[nCntSphere].Master==Master)
		{
			D3DXVECTOR3 Space = pos - g_aHitShere[nCntSphere].pos;
			float Length = D3DXVec3Length(&Space);
			if (Length < fWidth + g_aHitShere[nCntSphere].scale.x)
			{
				return true;
			}
		}
	}
	return false;
}