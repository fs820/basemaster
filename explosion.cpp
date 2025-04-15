//---------------------------------------
//
//弾表示処理[explosion.cpp]
//Author fuma sato
//
//---------------------------------------

#include"explosion.h"

#define MAX_EXPLOSION (256)
#define EXPLOSION_WIDTH (50.0f)
#define EXPLOSION_HEIGHT (50.0f)

//弾構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXCOLOR col;
	int nCntAnim;//移動量
	int nPatternAnim;//寿命
	D3DXVECTOR3 scale;
	bool bUse;//使用しているかどうか
	D3DXMATRIX mtxWorld;
}Explosion;

//グローバル
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;//バッファのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];//弾の情報

//-----------------
//弾の初期化処理
//-----------------
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_3D* pVtx;

	//デバイスを取得
	pDevice = GetDevice();

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		g_aExplosion[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[i].nCntAnim = 0;
		g_aExplosion[i].nPatternAnim = 0;
		g_aExplosion[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aExplosion[i].bUse = false;//使用していない状態にする
	}

	//バッファの作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VT_MAX * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		EXPLOSION_TEX,
		&g_pTextureExplosion
	);

	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH * 0.5f, g_aExplosion[i].pos.y + EXPLOSION_HEIGHT * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH * 0.5f, g_aExplosion[i].pos.y + EXPLOSION_HEIGHT * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH * 0.5f, g_aExplosion[i].pos.y - EXPLOSION_HEIGHT * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH * 0.5f, g_aExplosion[i].pos.y - EXPLOSION_HEIGHT * 0.5f, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim + UV_DEF / U_MAX_E, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E) + UV_DEF / V_MAX_E);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim + UV_DEF / U_MAX_E, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E) + UV_DEF / V_MAX_E);

		pVtx += VT_MAX;//ポリゴン1つ分進める
	}
	g_pVtxBuffExplosion->Unlock();//バッファのアンロック
}

//----------------
//弾の終了処理
//----------------
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//----------------
//弾の更新処理
//----------------
void UpdateExplosion(void)
{
	VERTEX_3D* pVtx;

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (g_aExplosion[i].bUse == true)
		{//弾が使用されている

			g_aExplosion[i].nCntAnim++;
			if ((g_aExplosion[i].nCntAnim % 5) == 0)//更新期間
			{
				g_aExplosion[i].nCntAnim = (g_aExplosion[i].nCntAnim + 1) % (U_MAX_E * V_MAX_E);

				g_aExplosion[i].nPatternAnim++;
				if (g_aExplosion[i].nPatternAnim >= (U_MAX_E * V_MAX_E))
				{
					g_aExplosion[i].bUse = false;
				}

				g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);//バッファのロック
				pVtx += i * VT_MAX;

				//テクスチャ
				pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E));
				pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim + UV_DEF / U_MAX_E, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E));
				pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E) + UV_DEF / V_MAX_E);
				pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim + UV_DEF / U_MAX_E, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E) + UV_DEF / V_MAX_E);

				g_pVtxBuffExplosion->Unlock();//バッファのアンロック
			}
		}
	}
}

//----------------
//弾の描画処理
//----------------
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATRIX mtxScale, mtxTrans;//計算マトリックス

	//デバイスの取得
	pDevice = GetDevice();
	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	int nCntExplosion;
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse)
		{
			//マトリックス初期化
			D3DXMatrixIdentity(&g_aExplosion[nCntExplosion].mtxWorld);

			//ビューマトリックス取得
			D3DXMATRIX mtxView;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			g_aExplosion[nCntExplosion].mtxWorld._11 = mtxView._11;
			g_aExplosion[nCntExplosion].mtxWorld._12 = mtxView._21;
			g_aExplosion[nCntExplosion].mtxWorld._13 = mtxView._31;
			g_aExplosion[nCntExplosion].mtxWorld._21 = mtxView._12;
			g_aExplosion[nCntExplosion].mtxWorld._22 = mtxView._22;
			g_aExplosion[nCntExplosion].mtxWorld._23 = mtxView._32;
			g_aExplosion[nCntExplosion].mtxWorld._31 = mtxView._13;
			g_aExplosion[nCntExplosion].mtxWorld._32 = mtxView._23;
			g_aExplosion[nCntExplosion].mtxWorld._33 = mtxView._33;

			//大きさの反映
			D3DXMatrixScaling(&mtxScale, g_aExplosion[nCntExplosion].scale.x, g_aExplosion[nCntExplosion].scale.y, g_aExplosion[nCntExplosion].scale.z);
			D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxScale);

			//位置の計算
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntExplosion].mtxWorld);

			//頂点バッファ
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureExplosion);

			//ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				nCntExplosion * VT_MAX,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//-------------------
//爆発設定
//-------------------
void SetExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (g_aExplosion[i].bUse == false)
		{//弾が使用されていない
			g_aExplosion[i].pos = pos;
			g_aExplosion[i].scale = scale;
			g_aExplosion[i].nCntAnim = 0;
			g_aExplosion[i].nPatternAnim = 0;
			g_aExplosion[i].bUse = true;
			break;
		}
	}
}