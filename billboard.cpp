//----------------------------------------
//
//ポリゴン表示処理[billboard.cpp]
//Author fuma sato
//
//----------------------------------------

#include"billboard.h"
#include"camera.h"
#include"shadow.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;//バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;
Billboard g_aBillboard[BILLBOARD_MAX];

//----------------------
//ポリゴンの初期化処理
//----------------------
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_3D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		BILLBOARD_TEX,
		&g_pTextureBillboard
	);

	int nCntBillboard;
	for (nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		g_aBillboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBillboard[nCntBillboard].bUse = false;
		g_aBillboard[nCntBillboard].nIdxShadow = -1;
	}
	//1つ目


	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//座標設定
	pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_WIDTH * 0.5f, BILLBOARD_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BILLBOARD_WIDTH * 0.5f, BILLBOARD_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_WIDTH * 0.5f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BILLBOARD_WIDTH * 0.5f, 0.0f, 0.0f);

	//nor
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

	g_pVtxBuffBillboard->Unlock();
}

//-------------------
//ポリゴン終了処理
//-------------------
void UninitBillboard(void)
{
	//テクスチャの破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//-------------------
//ポリゴン更新処理
//-------------------
void UpdateBillboard(void)
{
	int nCntBillboard;
	for (nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse)
		{
			SetPositionShadow(g_aBillboard[nCntBillboard].nIdxShadow, g_aBillboard[nCntBillboard].pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), BILLBOARD_HEIGHT);
		}
	}
}

//-------------------
//ポリゴン描画処理
//-------------------
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATRIX mtxScale, mtxTrans;//計算マトリックス

	//デバイスの取得
	pDevice = GetDevice();
	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zテストオフ
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	int nCntBillboard;
	for (nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse)
		{
			//マトリックス初期化
			D3DXMatrixIdentity(&g_aBillboard[nCntBillboard].mtxWorld);

			//ビューマトリックス取得
			D3DXMATRIX mtxView;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			g_aBillboard[nCntBillboard].mtxWorld._11 = mtxView._11;
			//g_aBillboard[nCntBillboard].mtxWorld._12 = mtxView._21;
			g_aBillboard[nCntBillboard].mtxWorld._13 = mtxView._31;
			//g_aBillboard[nCntBillboard].mtxWorld._21 = mtxView._12;
			//g_aBillboard[nCntBillboard].mtxWorld._22 = mtxView._22;
			//g_aBillboard[nCntBillboard].mtxWorld._23 = mtxView._32;
			g_aBillboard[nCntBillboard].mtxWorld._31 = mtxView._13;
			//g_aBillboard[nCntBillboard].mtxWorld._32 = mtxView._23;
			g_aBillboard[nCntBillboard].mtxWorld._33 = mtxView._33;

			//大きさの反映
			D3DXMatrixScaling(&mtxScale, g_aBillboard[nCntBillboard].scale.x, g_aBillboard[nCntBillboard].scale.y, g_aBillboard[nCntBillboard].scale.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxScale);

			//位置の計算
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBillboard].pos.x, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBillboard].mtxWorld);

			//頂点バッファ
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBillboard);

			//ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				0,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}

	//Zテストオン
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//----------
//設定
//----------
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	int nCntBillboard;
	for (nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		if (!g_aBillboard[nCntBillboard].bUse)
		{
			g_aBillboard[nCntBillboard].pos = pos;
			g_aBillboard[nCntBillboard].scale = scale;
			g_aBillboard[nCntBillboard].bUse = true;
			g_aBillboard[nCntBillboard].nIdxShadow = SetShadow(g_aBillboard[nCntBillboard].pos);
			break;
		}
	}
}