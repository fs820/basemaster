//----------------------------------------
//
//ポリゴン表示処理[meshfield.cpp]
//Author fuma sato
//
//----------------------------------------

#include"meshfield.h"
#include"player.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;//バッファのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;//インデックスバッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshField[MESHFIELD_TEX_MAX] = { NULL };
MeshField g_aMeshField[MESHFIELD_MAX];

//----------------------
//ポリゴンの初期化処理
//----------------------
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_3D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VT_MAX_MESH,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		MESHFIELD_TEX,
		&g_pTextureMeshField[0]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		MESHFIELD_TEX2,
		&g_pTextureMeshField[1]
	);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL
	);

	int nCntMeshField, nCntMeshField2;
	for (nCntMeshField = 0; nCntMeshField < MESHFIELD_MAX; nCntMeshField++)
	{
		g_aMeshField[nCntMeshField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshField[nCntMeshField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshField[nCntMeshField].bAlpha = false;
		g_aMeshField[nCntMeshField].bUse = false;
	}
	//1つ目


	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshField = 0; nCntMeshField < MESHFIELD_ZNUM + 1; nCntMeshField++)
	{//1枚分のZ軸のループ
		for (nCntMeshField2 = 0; nCntMeshField2 < MESHFIELD_XNUM + 1; nCntMeshField2++)
		{//1枚分のX軸のループ
			//座標設定
			pVtx[nCntMeshField * (MESHFIELD_XNUM + 1) + nCntMeshField2].pos = D3DXVECTOR3(-MESHFIELD_WIDTH * 0.5f + (MESHFIELD_WIDTH / MESHFIELD_XNUM) * nCntMeshField2, MESHFIELD_HEIGHT, MESHFIELD_Z * 0.5f - (MESHFIELD_Z / MESHFIELD_ZNUM) * nCntMeshField);

			//nor
			pVtx[nCntMeshField * (MESHFIELD_XNUM + 1) + nCntMeshField2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//カラー
			pVtx[nCntMeshField * (MESHFIELD_XNUM + 1) + nCntMeshField2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ
			pVtx[nCntMeshField * (MESHFIELD_XNUM + 1) + nCntMeshField2].tex = D3DXVECTOR2(((MESHFIELD_WIDTH / MESHFIELD_WIDTH_DEF) / MESHFIELD_XNUM) * nCntMeshField2, ((MESHFIELD_Z / MESHFIELD_Z_DEF) / MESHFIELD_ZNUM) * nCntMeshField);
		}
	}

	g_pVtxBuffMeshField->Unlock();

	WORD* pIdx;
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshField = 0; nCntMeshField < INDEX_NUM - (MESHFIELD_ZNUM - 1) * 2; nCntMeshField++)
	{
		if (nCntMeshField % ((MESHFIELD_XNUM + 1) * 2) == 0 && nCntMeshField != 0)
		{
			//インデックス指定
			pIdx[nCntMeshField - 2 + ((nCntMeshField / ((MESHFIELD_XNUM + 1) * 2)) * 2)] = (MESHFIELD_XNUM + 1) - (MESHFIELD_XNUM + 1) * ((nCntMeshField - 1) % 2) + ((nCntMeshField - 1) / 2);
			//インデックス指定
			pIdx[nCntMeshField - 1 + ((nCntMeshField / ((MESHFIELD_XNUM + 1) * 2)) * 2)] = (MESHFIELD_XNUM + 1) - (MESHFIELD_XNUM + 1) * (nCntMeshField % 2) + (nCntMeshField / 2);
		}

		//インデックス指定
		pIdx[nCntMeshField + ((nCntMeshField / ((MESHFIELD_XNUM + 1) * 2)) * 2)] = (MESHFIELD_XNUM + 1) - (MESHFIELD_XNUM + 1) * (nCntMeshField % 2) + (nCntMeshField / 2);
	}

	g_pIdxBuffMeshField->Unlock();
}

//-------------------
//ポリゴン終了処理
//-------------------
void UninitMeshField(void)
{
	for (int nCnt = 0; nCnt < MESHFIELD_TEX_MAX; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureMeshField[nCnt] != NULL)
		{
			g_pTextureMeshField[nCnt]->Release();
			g_pTextureMeshField[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//-------------------
//ポリゴン更新処理
//-------------------
void UpdateMeshField(void)
{

}

//-------------------
//ポリゴン描画処理
//-------------------
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATRIX mtxRot, mtxTrans;//計算マトリックス

	//デバイスの取得
	pDevice = GetDevice();

	int nCntMeshField;
	for (nCntMeshField = 0; nCntMeshField < MESHFIELD_MAX; nCntMeshField++)
	{
		if (g_aMeshField[nCntMeshField].bUse && !g_aMeshField[nCntMeshField].bAlpha)
		{
			//マトリックス初期化
			D3DXMatrixIdentity(&g_aMeshField[nCntMeshField].mtxWorld);

			//向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField[nCntMeshField].rot.y, g_aMeshField[nCntMeshField].rot.x, g_aMeshField[nCntMeshField].rot.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntMeshField].mtxWorld, &g_aMeshField[nCntMeshField].mtxWorld, &mtxRot);

			//位置の計算
			D3DXMatrixTranslation(&mtxTrans, g_aMeshField[nCntMeshField].pos.x, g_aMeshField[nCntMeshField].pos.y, g_aMeshField[nCntMeshField].pos.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntMeshField].mtxWorld, &g_aMeshField[nCntMeshField].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField[nCntMeshField].mtxWorld);

			//頂点バッファ
			pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshField);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshField[GetbSnow()]);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				0,
				0,
				VT_MAX_MESH,//頂点数
				0,
				POLYGON_NUM//ポリゴンの個数
			);
		}
	}
}

//----------
//設定
//----------
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntMeshField;
	for (nCntMeshField = 0; nCntMeshField < MESHFIELD_MAX; nCntMeshField++)
	{
		if (!g_aMeshField[nCntMeshField].bUse)
		{
			g_aMeshField[nCntMeshField].pos = pos;
			g_aMeshField[nCntMeshField].rot = rot;
			g_aMeshField[nCntMeshField].bAlpha = false;
			g_aMeshField[nCntMeshField].bUse = true;
			break;
		}
	}
}

//---------------------------
//取得
//---------------------------
MeshField* GetMeshField(void)
{
	return &g_aMeshField[0];
}

//----------------------
//当たり判定
//----------------------
bool CollisionMeshField(D3DXVECTOR3& pos, D3DXVECTOR3& posOld, D3DXVECTOR3& move,float fWidth)
{
	D3DXVECTOR3 aPos[2] = {}, Fieldvec = {}, Posvec = {}, PosOldvec = {}, movevec = {}, Norvec = {}, Dovec = {}, Hit = {};
	int nCntMeshField;
	float FieldCross, PosCross;
	for (nCntMeshField = 0; nCntMeshField < MESHFIELD_MAX; nCntMeshField++)
	{//地面の数
		if (g_aMeshField[nCntMeshField].bUse && !g_aMeshField[nCntMeshField].bAlpha)
		{//使っている透明でない壁
			aPos[0] = D3DXVECTOR3(g_aMeshField[nCntMeshField].pos.x + MESHFIELD_WIDTH * 0.5f * sinf(g_aMeshField[nCntMeshField].rot.z + D3DX_PI * -0.5f), g_aMeshField[nCntMeshField].pos.y + MESHFIELD_WIDTH * 0.5f * cosf(g_aMeshField[nCntMeshField].rot.z + D3DX_PI * 0.5f), g_aMeshField[nCntMeshField].pos.z);
			aPos[1] = D3DXVECTOR3(g_aMeshField[nCntMeshField].pos.x + MESHFIELD_WIDTH * 0.5f * sinf(g_aMeshField[nCntMeshField].rot.z + D3DX_PI * 0.5f), g_aMeshField[nCntMeshField].pos.y + MESHFIELD_WIDTH * 0.5f * cosf(g_aMeshField[nCntMeshField].rot.z + D3DX_PI * -0.5f), g_aMeshField[nCntMeshField].pos.z);

			Fieldvec = aPos[1] - aPos[0];//壁のベクトル
			Posvec = pos - aPos[0];//壁に対するプレイヤーのベクトル
			PosOldvec = posOld - aPos[0];//壁に対するプレイヤーの旧ベクトル
			movevec = pos - posOld;//プレイヤーの移動ベクトル
			if ((Fieldvec.x * Posvec.y) - (Fieldvec.y * Posvec.x) <= 0.0f && (Fieldvec.x * PosOldvec.y) - (Fieldvec.y * PosOldvec.x) >= 0.0f && pos.z - fWidth < g_aMeshField[nCntMeshField].pos.z + MESHFIELD_WIDTH * 0.5f && pos.z + fWidth > g_aMeshField[nCntMeshField].pos.z - MESHFIELD_WIDTH * 0.5f)
			{//地面の下
				FieldCross = (Fieldvec.x * movevec.y) - (Fieldvec.y * movevec.x);
				PosCross = (Posvec.x * movevec.y) - (Posvec.y * movevec.x);
				PosCross /= FieldCross;
				if (PosCross >= -0.01f && PosCross < 1.01f)
				{
					Hit = aPos[0] + Fieldvec * PosCross;
					movevec = pos - Hit;//プレイヤーの移動ベクトル
					Norvec = D3DXVECTOR3(Fieldvec.y, -Fieldvec.x, Fieldvec.z);
					D3DXVec3Normalize(&Norvec, &Norvec);
					Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.y * Norvec.y));
					pos += Dovec * 1.001f;
					move.y = 0.0f;
					return true;
				}
			}
		}
	}

	return false;
}