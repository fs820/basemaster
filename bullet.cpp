//----------------------------------------
//
//ポリゴン表示処理[bullet.cpp]
//Author fuma sato
//
//----------------------------------------

#include"bullet.h"
#include"camera.h"
#include"shadow.h"
#include"explosion.h"
#include"effect.h"
#include"particle.h"
#include"meshfield.h"
#include"sound.h"
#include"hitshere.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;
Bullet g_aBullet[BULLET_MAX];

//----------------------
//ポリゴンの初期化処理
//----------------------
void InitBullet(void)
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
		&g_pVtxBuffBullet,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		BULLET_TEX,
		&g_pTextureBullet
	);

	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].nIdxShadow = -1;
	}
	//1つ目


	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	//座標設定
	pVtx[0].pos = D3DXVECTOR3(-BULLET_WIDTH * 0.5f, BULLET_HEIGHT * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BULLET_WIDTH * 0.5f, BULLET_HEIGHT * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-BULLET_WIDTH * 0.5f, -BULLET_HEIGHT * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BULLET_WIDTH * 0.5f, -BULLET_HEIGHT * 0.5f, 0.0f);

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

	g_pVtxBuffBullet->Unlock();
}

//-------------------
//ポリゴン終了処理
//-------------------
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//-------------------
//ポリゴン更新処理
//-------------------
void UpdateBullet(void)
{
	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{
			g_aBullet[nCntBullet].nLife--;

			D3DXVec3Normalize(&g_aBullet[nCntBullet].dir, &g_aBullet[nCntBullet].dir);

			g_aBullet[nCntBullet].move.x = g_aBullet[nCntBullet].dir.x * BULLET_SPEED;
			g_aBullet[nCntBullet].move.y = g_aBullet[nCntBullet].dir.y * BULLET_SPEED;
			g_aBullet[nCntBullet].move.z = g_aBullet[nCntBullet].dir.z * BULLET_SPEED;

			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.1f, 0.5f, 0.4f, 0.1f), g_aBullet[nCntBullet].scale, 1000, EFFECT_TYPE_NORMAL);

			if (g_aBullet[nCntBullet].nLife < 0)
			{
				g_aBullet[nCntBullet].bUse = false;
				NullShadow(g_aBullet[nCntBullet].nIdxShadow);
				SetParticle(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].scale);
			}
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].scale, BULLET_HEIGHT);

			SetHitShere(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].scale * BULLET_WIDTH * 0.5f*2.0f, 2, PLAYER);
		}
	}
}

//-------------------
//ポリゴン描画処理
//-------------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATRIX mtxScale, mtxTrans;//計算マトリックス

	//デバイスの取得
	pDevice = GetDevice();
	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//アルファテストオン
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{
			//マトリックス初期化
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			//ビューマトリックス取得
			D3DXMATRIX mtxView;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			g_aBullet[nCntBullet].mtxWorld._11 = mtxView._11;
			g_aBullet[nCntBullet].mtxWorld._12 = mtxView._21;
			g_aBullet[nCntBullet].mtxWorld._13 = mtxView._31;
			g_aBullet[nCntBullet].mtxWorld._21 = mtxView._12;
			g_aBullet[nCntBullet].mtxWorld._22 = mtxView._22;
			g_aBullet[nCntBullet].mtxWorld._23 = mtxView._32;
			g_aBullet[nCntBullet].mtxWorld._31 = mtxView._13;
			g_aBullet[nCntBullet].mtxWorld._32 = mtxView._23;
			g_aBullet[nCntBullet].mtxWorld._33 = mtxView._33;

			//大きさの反映
			D3DXMatrixScaling(&mtxScale, g_aBullet[nCntBullet].scale.x, g_aBullet[nCntBullet].scale.y, g_aBullet[nCntBullet].scale.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxScale);

			//位置の計算
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			//頂点バッファ
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);

			//ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				0,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}

	//アルファテストオフ
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//----------
//設定
//----------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 dir, D3DXVECTOR3 scale)
{
	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (!g_aBullet[nCntBullet].bUse)
		{
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aBullet[nCntBullet].dir = dir;
			g_aBullet[nCntBullet].scale = scale;
			g_aBullet[nCntBullet].nLife = BULEET_LIFE;
			g_aBullet[nCntBullet].bUse = true;
			g_aBullet[nCntBullet].nIdxShadow = SetShadow(g_aBullet[nCntBullet].pos);
			break;
		}
	}
}