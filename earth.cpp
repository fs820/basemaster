//----------------------------------------
//
//地球処理[earth.cpp]
//Author fuma sato
//
//----------------------------------------

#include"earth.h"
#include"stddxbuff.h"
#include "camera.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEarth = NULL;//バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEarth = NULL;     //テクスチャのポインタ
Earth g_Earth;                                 //情報体

//----------------------
//初期化処理
//----------------------
void InitEarth(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_DEFAULT,
		&g_pVtxBuffEarth,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFileEx
	(
		pDevice,
		EARTH_TEX,
		D3DX_DEFAULT,              // テクスチャの幅（D3DX_DEFAULTなら自動調整）
		D3DX_DEFAULT,              // テクスチャの高さ（D3DX_DEFAULTなら自動調整）
		D3DX_DEFAULT,              // ミップマップレベル（D3DX_DEFAULTで自動設定）
		0,                         // 使用しない場合は0（動的テクスチャならD3DUSAGE_DYNAMIC）
		D3DFMT_A8R8G8B8,           // フォーマット（A8R8G8B8 png X8R8G8B8 jpg）
		D3DPOOL_DEFAULT,           // メモリプール（通常は D3DPOOL_MANAGED）
		D3DX_FILTER_TRIANGLE,      // 画像の拡縮フィルタ（D3DX_FILTER_LINEAR など）
		D3DX_FILTER_TRIANGLE,      // ミップマップのフィルタ
		0,                         // カラーキー（透明にしたい色）
		NULL,                      // `D3DXIMAGE_INFO`（画像情報を取得する場合）
		NULL,                      // パレットデータ（パレットなしならNULL）
		&g_pTextureEarth
	);

	g_Earth.bUse = false;

	//頂点の設定
	SetVertex3DYX
	(
		&g_pVtxBuffEarth,
		0,
		1,
		1,
		1,
		EARTH_WIDTH,
		EARTH_HEIGHT,
		D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		EARTH_WIDTH,
		EARTH_HEIGHT,
		true, false
	);
}

//-------------------
//ポリゴン終了処理
//-------------------
void UninitEarth(void)
{
	g_Earth.bUse = false;

	//テクスチャの破棄
	if (g_pTextureEarth != NULL)
	{
		g_pTextureEarth->Release();
		g_pTextureEarth = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEarth != NULL)
	{
		g_pVtxBuffEarth->Release();
		g_pVtxBuffEarth = NULL;
	}
}

//-------------------
//ポリゴン更新処理
//-------------------
void UpdateEarth(void)
{

}

//-------------------
//ポリゴン描画処理
//-------------------
void DrawEarth(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATRIX mtxTrans;//計算マトリックス

	//デバイスの取得
	pDevice = GetDevice();
	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテスト
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	if (g_Earth.bUse)
	{
		//マトリックス初期化
		D3DXMatrixIdentity(&g_Earth.mtxWorld);

		//ビューマトリックス取得
		D3DXMATRIX mtxView;
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//カメラの逆行列を設定
		g_Earth.mtxWorld._11 = mtxView._11;
		g_Earth.mtxWorld._12 = mtxView._21;
		g_Earth.mtxWorld._13 = mtxView._31;
		g_Earth.mtxWorld._21 = mtxView._12;
		g_Earth.mtxWorld._22 = mtxView._22;
		g_Earth.mtxWorld._23 = mtxView._32;
		g_Earth.mtxWorld._31 = mtxView._13;
		g_Earth.mtxWorld._32 = mtxView._23;
		g_Earth.mtxWorld._33 = mtxView._33;

		//位置の計算
		extern  Camera g_camera[1];
		D3DXVECTOR3 Pos;
		SpherePos(Pos, g_camera[0].posV, D3DXVECTOR3(D3DX_PI * 0.3f, D3DX_PI, 0.0f), 10000.0f);
		D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);
		D3DXMatrixMultiply(&g_Earth.mtxWorld, &g_Earth.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Earth.mtxWorld);

		//頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffEarth, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEarth);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			0,//始まりの番号
			2//ポリゴンの個数
		);
	}

	// Zテストを戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//----------
//設定
//----------
void SetEarth(void)
{
	if (!g_Earth.bUse)
	{
		g_Earth.bUse = true;
	}
}