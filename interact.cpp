//----------------------------------------
//
//インタラクト処理[interact.cpp]
//Author fuma sato
//
//----------------------------------------
#include"interact.h"
#include"stddxbuff.h"
#include"camera.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffInteract = NULL;                   //バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureInteract[INTERACT_TYPE_MAX] = { NULL }; //テクスチャのポインタ
Interact g_Interact;                                                 //情報体
//----------------------
//ポリゴンの初期化処理
//----------------------
void InitInteract(void)
{
	//頂点バッファの生成
	CHECK_HR(CreateVertex3D
	(
		&g_pVtxBuffInteract, // 頂点バッファのポインタ
		VT_MAX, 1,           // 頂点数,バッファ数
		D3DUSAGE_WRITEONLY,  // 使用法
		D3DPOOL_DEFAULT      // プールの種類
	))

	//テクスチャバッファの生成
	CHECK_HR(CreateTexture
	(
		g_pTextureInteract,   // テクスチャのポインタ
		INTERACT_TEX,         // テクスチャのパス
		INTERACT_TYPE_MAX,    // テクスチャ数
		nullptr,              // 画像情報（NULLなら取得しない）
		D3DX_FILTER_TRIANGLE, // フィルタリング
		D3DPOOL_DEFAULT       // プールの種類
	))

	//頂点の設定
	CHECK_HR(SetVertex3D
	(
		&g_pVtxBuffInteract,                // 頂点バッファのポインタ
		0,                                  // ロックフラグ
		0, 1,                               // バッファの開始オフセット,バッファ数
		1, 1,                               // y分割数, x分割数
		1.0f, 1.0f,                         // 幅,高さ
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  // 色
		1, nullptr,                         // テクスチャ数,画像情報(nullの時画像はポリゴンの大きさに調節される)
		false                               // センターポジション
	))

	//情報体の初期化
	g_Interact.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Interact.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Interact.Type = INTERACT_TYPE_KEY;
	g_Interact.bUse = false;
}

//-------------------
//ポリゴン終了処理
//-------------------
void UninitInteract(void)
{
	g_Interact.bUse = false;

	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < INTERACT_TYPE_MAX; nCntTex++)
	{
		SAFE_RELEASE(g_pTextureInteract[nCntTex]);
	}
	//頂点バッファの破棄
	SAFE_RELEASE(g_pVtxBuffInteract);
}

//-------------------
//ポリゴン更新処理
//-------------------
void UpdateInteract(void)
{

}

//-------------------
//ポリゴン描画処理
//-------------------
void DrawInteract(void)
{
	RSOP rsOp, rsOpOld;                 //レンダーステートの構造体
	CHECK_HR(GetRenderOperation(&rsOp)) //レンダーステートの取得
	rsOpOld = rsOp;                     //現在のレンダーステート

	//レンダーステートの変更
	rsOp.Lighting = FALSE; //ライトを無効にする
	rsOp.zEnable = TRUE;  //Zテストを有効にする
	rsOp.zWriteEnable = FALSE; //Z書き込みを無効にする
	rsOp.zFunc = D3DCMP_ALWAYS; //Zテストの比較関数を常に通す

	//レンダーステートの設定
	CHECK_HR(SetRenderOperation(&rsOp))

	if (g_Interact.bUse)
	{//使っている
		//描画
		CHECK_HR(DrawVertex3D
		(
			&g_pVtxBuffInteract, nullptr, &g_pTextureInteract[(unsigned int)g_Interact.Type], //バッファ(頂点,インデックス,テクスチャ)
			&g_Interact.pos, nullptr, &g_Interact.scale, &g_Interact.mtxWorld,                //位置,回転,大きさ,ワールドマトリックス
			BILLBOARD_FULL, 0.0f,                                                             //ビルボードのタイプ、Zバイアス値
			D3DPT_TRIANGLESTRIP,                                                              //描画のタイプ
			VT_MAX, 0, 2                                                                      //頂点数、開始インデックス、ポリゴン数
		))
	}

	CHECK_HR(SetRenderOperation(&rsOpOld)) //レンダーステートを戻す
}

//----------
//設定
//----------
void SetInteract(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	g_Interact.pos = pos;      //位置
	g_Interact.pos.y += 50.0f; //少し上げる
	g_Interact.scale = scale;  //大きさ
	g_Interact.bUse = true;    //使用
}

//----------------------
//破棄
//----------------------
void EndInteract(void)
{
	g_Interact.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置
	g_Interact.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //大きさ
	g_Interact.bUse = false;                          //使用
}

//---------------------
//タイプ変更
//---------------------
void SetInteractType(INTERACT_TYPE Type)
{
	g_Interact.Type = Type;
}