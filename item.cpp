//----------------------------------------
//
// オブジェクト処理[Item.cpp]
// Author fuma sato
//
//----------------------------------------
#include"Item.h"
#include"shadow.h"
#include"player.h"
#include"camera.h"
#include"meshfield.h"
#include"input.h"

// グローバル変数宣言
Item g_aItem[ITEM_MAX] = {};								// オブジェクト情報体
ItemManager g_ItemManager;									// ファイル情報体
ITYPE g_Type;												// オブジェクト番号管理
int g_DelItem;												// オブジェクト消去用
//----------------------
// 初期化処理
//----------------------
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへポインタ

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{// 最大数繰り返す

		// オブジェクト情報の初期化
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置を初期化する
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向きを初期化する
		g_aItem[nCntItem].fLength = 0.0f;							// 長さを初期化する
		g_aItem[nCntItem].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 大きさを初期化する
		g_aItem[nCntItem].nIdxShadow = -1;						    // 影の番号を初期化する
		g_aItem[nCntItem].Type = ITYPE_TEST;							// タイプ識別子を初期化する
		g_aItem[nCntItem].bAlpha = false;							// 透明かどうかを初期化する
		g_aItem[nCntItem].bUse = false;								// 使用しているかどうかを初期化する
		g_aItem[nCntItem].nRadarIdx = -1;							// レーダーインデックス
	}

	// ファイル情報の初期化
	g_ItemManager.ItemNum = 0;
	for (int nCntTex = 0; nCntTex < ITEM_TEXTURE_MAX; nCntTex++)
	{//テクスチャ数繰り返す
		g_ItemManager.apTexture[nCntTex] = NULL;
	}
	for (int nCntModel = 0; nCntModel < ITYPE_MAX; nCntModel++)
	{
		*g_ItemManager.ItemModel[nCntModel].Pass.aName = '\0';						// ファイルパスを初期化する
		g_ItemManager.ItemModel[nCntModel].pMesh = NULL;							// メッシュを初期化する
		g_ItemManager.ItemModel[nCntModel].pBuffMat = NULL;							// マテリアルを初期化する
		g_ItemManager.ItemModel[nCntModel].dwNumMat = 0;							// マテリアル数を初期化する
		for (int nCntMat = 0; nCntMat < ITEM_SUBSET_MAX; nCntMat++)
		{//マテリアル数繰り返す
			g_ItemManager.ItemModel[nCntModel].TexIdx[nCntMat] = -1;
		}
		g_ItemManager.ItemModel[nCntModel].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 最小値を初期化する
		g_ItemManager.ItemModel[nCntModel].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 最大値を初期化する
		g_ItemManager.ItemModel[nCntModel].nNumVtx = 0;								// 頂点数を初期化する
		g_ItemManager.ItemModel[nCntModel].sizeFVF = 0;								// サイズを初期化する
		g_ItemManager.ItemModel[nCntModel].pVtxBuff = NULL;							// バッファのポインタを初期化する
	}

	//オブジェクトファイル情報取得
	LoadItemManager();

	//番号の初期化
	g_Type = ITYPE_TEST;
	g_DelItem = -1;
}

//-------------------
// 終了処理
//-------------------
void UninitItem(void)
{
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{// 最大数繰り返す
		// オブジェクト情報の初期化
		g_aItem[nCntItem].bUse = false;								// 使用しているかどうかを初期化する
	}

	for (int nCount = 0; nCount < ITYPE_MAX; nCount++)
	{// UIの最大数分繰り返す
		g_itemUI[nCount].bUse = false;																										// アイテムを所持しているかを初期化する
	}

	// UIテクスチャの処理
	for (int nCount = 0; nCount < ITEMUITYPE_MAX; nCount++)
	{// タイプ分回す tetsuji
		// テクスチャの破棄
		if (g_pTextureitemUI[nCount] != NULL)
		{
			g_pTextureitemUI[nCount]->Release();
			g_pTextureitemUI[nCount] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffitemUI != NULL)
	{
		g_pVtxBuffitemUI->Release();
		g_pVtxBuffitemUI = NULL;
	}

	for (int nCntTex = 0; nCntTex < ITEM_TEXTURE_MAX; nCntTex++)
	{// テクスチャ数繰り返す
		// テクスチャの破棄
		if (g_ItemManager.apTexture[nCntTex] != NULL)
		{
			g_ItemManager.apTexture[nCntTex]->Release();
			g_ItemManager.apTexture[nCntTex] = NULL;
		}
	}
	for (int nCntModel = 0; nCntModel < ITYPE_MAX; nCntModel++)
	{// 最大数繰り返す
				//頂点バッファの破棄
		if (g_ItemManager.ItemModel[nCntModel].pVtxBuff != NULL)
		{
			g_ItemManager.ItemModel[nCntModel].pVtxBuff = NULL;
		}
		// メッシュの破棄
		if (g_ItemManager.ItemModel[nCntModel].pMesh != NULL)
		{
			g_ItemManager.ItemModel[nCntModel].pMesh->Release();
			g_ItemManager.ItemModel[nCntModel].pMesh = NULL;
		}
		// マテリアルの破棄
		if (g_ItemManager.ItemModel[nCntModel].pBuffMat != NULL)
		{
			g_ItemManager.ItemModel[nCntModel].pBuffMat->Release();
			g_ItemManager.ItemModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//-------------------
// 更新処理
//-------------------
void UpdateItem(void)
{
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{//最大数繰り返す
		if (g_aItem[nCntItem].bUse == true)
		{// アイテムを使用している時
			SetPositionShadow(g_aItem[nCntItem].nIdxShadow, g_aItem[nCntItem].pos, g_aItem[nCntItem].scale, g_aItem[nCntItem].fLength);
		}
	}
}

//-------------------
// 描画処理
//-------------------
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへポインタ
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// 計算マトリックス
	D3DMATERIAL9 matDef;					// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;						// マテリアルデータへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{//最大数繰り返す
		if (g_aItem[nCntItem].bUse == true)
		{// アイテムを使用している時
			// マトリックス初期化
			D3DXMatrixIdentity(&g_aItem[nCntItem].mtxWorld);

			// 大きさの反映
			D3DXMatrixScaling(&mtxScale, g_aItem[nCntItem].scale.x, g_aItem[nCntItem].scale.y, g_aItem[nCntItem].scale.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxScale);

			// 向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxRot);

			// 位置の反映
			D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].mtxWorld);
			
			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].pBuffMat->GetBufferPointer();

			// 各サブセットのフェイス数を取得
			DWORD numSubsets = 0;
			g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].pMesh->GetAttributeTable(nullptr, &numSubsets);

			D3DXATTRIBUTERANGE* pAttributeTable = new D3DXATTRIBUTERANGE[numSubsets];
			g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].pMesh->GetAttributeTable(pAttributeTable, &numSubsets);

			for (int nCntSubset = 0; nCntSubset < (int)numSubsets; nCntSubset++)
			{//サブセット数回す
				if (pAttributeTable[nCntSubset].FaceCount > 0) // 使われているマテリアルのみ描画
				{
					// 現在のマテリアル取得
					pDevice->GetMaterial(&matDef);

					// 透明度の設定
					D3DMATERIAL9 mat = pMat[pAttributeTable[nCntSubset].AttribId].MatD3D;
					if (nCntSubset == 0)
					{// 鉱石部分
						mat.Emissive = mat.Diffuse;
						mat.Emissive.a *= 0.5f;
						mat.Emissive.r *= 0.3f;
						mat.Emissive.g *= 0.3f;
						mat.Emissive.b *= 0.3f;
					}
					// マテリアルの設定
					pDevice->SetMaterial(&mat);

					if (g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].TexIdx[nCntSubset] != -1)
					{//テクスチャがある
						// テクスチャ設定
						pDevice->SetTexture(0, g_ItemManager.apTexture[g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].TexIdx[nCntSubset]]);
					}
					else
					{
						pDevice->SetTexture(0, nullptr);
					}

					if (nCntSubset == 0)
					{// 鉱石部分
						//αブレンディングを加算合成に設定
						pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
						pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
						pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
					}
					else
					{
						//αブレンディングを元に戻す
						pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
						pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
						pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
					}

					if (nCntSubset == 0)
					{// 鉱石部分
						for (int nCnt = 0; nCnt < 5; nCnt++)
						{// 光る
							// サブセット
							g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].pMesh->DrawSubset(pAttributeTable[nCntSubset].AttribId);
						}
					}
					else
					{
						// サブセット
						g_ItemManager.ItemModel[(int)g_aItem[nCntItem].Type].pMesh->DrawSubset(pAttributeTable[nCntSubset].AttribId);
					}

					// マテリアル保存
					pDevice->SetMaterial(&matDef);
				}
			}
			delete[] pAttributeTable; // メモリ解放
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================
// アイテムUIの描画処理
//=============================================================
void DrawItemUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// アイテムUIの処理
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffitemUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < ITYPE_MAX; nCount++)
	{
		if (g_itemUI[nCount].bUse == true)
		{// アイテムを所持している時
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureitemUI[ITYPE_MAX + nCount]);
			// アイテムUIの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCount, 2);

		}
		else if (g_itemUI[nCount].bUse == false)
		{// アイテムを所持していない時
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureitemUI[nCount]);
			// アイテムUIの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCount, 2);
		}
	}
}

//------------------------------
// 読み込んだファイル情報の取得
//------------------------------
void SetItemManager(ItemManager ItemManager)
{
	g_ItemManager = ItemManager; //読み込んだファイル情報をセット

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへポインタ
	D3DXMATERIAL* pMat;			// マテリアルデータへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	int nCntTex = 0;
	LPSTR Filename[ITEM_TEXTURE_MAX + 1]; //テクスチャパス一時保存用
	for (int nCntModel = 0; nCntModel < g_ItemManager.ItemNum; nCntModel++)
	{// 最大数繰り返す
		// Xファイル読み込み
		D3DXLoadMeshFromX
		(
			g_ItemManager.ItemModel[nCntModel].Pass.aName,
			D3DXMESH_MANAGED,
			pDevice,
			NULL,
			&g_ItemManager.ItemModel[nCntModel].pBuffMat,
			NULL,
			&g_ItemManager.ItemModel[nCntModel].dwNumMat,
			&g_ItemManager.ItemModel[nCntModel].pMesh
		);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_ItemManager.ItemModel[nCntModel].pBuffMat->GetBufferPointer();

		// 各サブセットのフェイス数を取得
		DWORD numSubsets = 0;
		g_ItemManager.ItemModel[nCntModel].pMesh->GetAttributeTable(nullptr, &numSubsets);

		D3DXATTRIBUTERANGE* pAttributeTable = new D3DXATTRIBUTERANGE[numSubsets];
		g_ItemManager.ItemModel[nCntModel].pMesh->GetAttributeTable(pAttributeTable, &numSubsets);

		for (int nCntSubset = 0; nCntSubset < (int)numSubsets; nCntSubset++)
		{//マテリアル数繰り返す
			if (pMat[pAttributeTable[nCntSubset].AttribId].pTextureFilename != NULL)
			{//テクスチャがある

				//同じテクスチャは読み込まない
				Filename[nCntTex] = pMat[pAttributeTable[nCntSubset].AttribId].pTextureFilename; //テクスチャパス一時保存
				bool bLoad = true; //読み込みの確認
				for (int nCntTex2 = nCntTex - 1; nCntTex2 >= 0; nCntTex2--)
				{//前のテクスチャを見る
					if (!strcmp(Filename[nCntTex], Filename[nCntTex2]))
					{//同じくテクスチャを発見
						g_ItemManager.ItemModel[nCntModel].TexIdx[nCntSubset] = nCntTex2; //同じテクスチャを登録
						bLoad = false; //テクスチャは読み込まない
						break;
					}
				}

				if (bLoad)
				{//読み込む
					//テクスチャの読み込み
					D3DXCreateTextureFromFileEx
					(
						pDevice,
						pMat[pAttributeTable[nCntSubset].AttribId].pTextureFilename,
						D3DX_DEFAULT,              // テクスチャの幅（D3DX_DEFAULTなら自動調整）
						D3DX_DEFAULT,              // テクスチャの高さ（D3DX_DEFAULTなら自動調整）
						D3DX_DEFAULT,              // ミップマップレベル（D3DX_DEFAULTで自動設定）
						0,                         // 使用しない場合は0（動的テクスチャならD3DUSAGE_DYNAMIC）
						D3DFMT_X8R8G8B8,           // フォーマット（A8R8G8B8 png X8R8G8B8 jpg）
						D3DPOOL_DEFAULT,           // メモリプール（通常は D3DPOOL_MANAGED）
						D3DX_FILTER_TRIANGLE,      // 画像の拡縮フィルタ（D3DX_FILTER_LINEAR など）
						D3DX_FILTER_TRIANGLE,      // ミップマップのフィルタ
						0,                         // カラーキー（透明にしたい色）
						NULL,                      // `D3DXIMAGE_INFO`（画像情報を取得する場合）
						NULL,                      // パレットデータ（パレットなしならNULL）
						&g_ItemManager.apTexture[nCntTex]
					);

					//読み込んだテクスチャを登録
					g_ItemManager.ItemModel[nCntModel].TexIdx[nCntSubset] = nCntTex;
					//テクスチャ番号進行
					nCntTex++;
				}
			}
			else
			{//テクスチャがない
				g_ItemManager.ItemModel[nCntModel].TexIdx[nCntSubset] = -1;
			}
		}
		delete[] pAttributeTable; // メモリ解放

		// 頂点数
		g_ItemManager.ItemModel[nCntModel].nNumVtx = g_ItemManager.ItemModel[nCntModel].pMesh->GetNumVertices();
		// 頂点サイズ
		g_ItemManager.ItemModel[nCntModel].sizeFVF = D3DXGetFVFVertexSize(g_ItemManager.ItemModel[nCntModel].pMesh->GetFVF());
		// 頂点バッファの取得
		g_ItemManager.ItemModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_ItemManager.ItemModel[nCntModel].pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < g_ItemManager.ItemModel[nCntModel].nNumVtx; nCntVtx++)
		{// 頂点数繰り返す
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_ItemManager.ItemModel[nCntModel].pVtxBuff;

			// 頂点の大きさの反映
			if (vtx.x > g_ItemManager.ItemModel[nCntModel].vtxMax.x)
			{
				g_ItemManager.ItemModel[nCntModel].vtxMax.x = vtx.x;
			}
			if (vtx.x < g_ItemManager.ItemModel[nCntModel].vtxMin.x)
			{
				g_ItemManager.ItemModel[nCntModel].vtxMin.x = vtx.x;
			}
			if (vtx.y > g_ItemManager.ItemModel[nCntModel].vtxMax.y)
			{
				g_ItemManager.ItemModel[nCntModel].vtxMax.y = vtx.y;
			}
			if (vtx.y < g_ItemManager.ItemModel[nCntModel].vtxMin.y)
			{
				g_ItemManager.ItemModel[nCntModel].vtxMin.y = vtx.y;
			}
			if (vtx.z > g_ItemManager.ItemModel[nCntModel].vtxMax.z)
			{
				g_ItemManager.ItemModel[nCntModel].vtxMax.z = vtx.z;
			}
			if (vtx.z < g_ItemManager.ItemModel[nCntModel].vtxMin.z)
			{
				g_ItemManager.ItemModel[nCntModel].vtxMin.z = vtx.z;
			}

			g_ItemManager.ItemModel[nCntModel].pVtxBuff += g_ItemManager.ItemModel[nCntModel].sizeFVF;
		}

		// 頂点バッファのアンロック
		g_ItemManager.ItemModel[nCntModel].pMesh->UnlockVertexBuffer();

	}
}

//-----------
// 配置
//-----------
int SetItem(ITYPE Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	int nCntItem;

	for (nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{// 最大数繰り返す
		if (g_aItem[nCntItem].bUse == false)
		{// アイテムを使用している時
			// オブジェクト情報の設定
			g_aItem[nCntItem].pos = pos;		// 位置を設定
			g_aItem[nCntItem].rot = rot;		// 向きを設定
			g_aItem[nCntItem].scale = scale;	// 大きさを設定
			// 長さを設定
			g_aItem[nCntItem].fLength = sqrtf((g_ItemManager.ItemModel[Type].vtxMax.x * g_aItem[nCntItem].scale.x - g_ItemManager.ItemModel[Type].vtxMin.x * g_aItem[nCntItem].scale.x) * (g_ItemManager.ItemModel[Type].vtxMax.x * g_aItem[nCntItem].scale.x - g_ItemManager.ItemModel[Type].vtxMin.x * g_aItem[nCntItem].scale.x) + (g_ItemManager.ItemModel[Type].vtxMax.z * g_aItem[nCntItem].scale.z - g_ItemManager.ItemModel[Type].vtxMin.z * g_aItem[nCntItem].scale.z) * (g_ItemManager.ItemModel[Type].vtxMax.z * g_aItem[nCntItem].scale.z - g_ItemManager.ItemModel[Type].vtxMin.z * g_aItem[nCntItem].scale.z)) / 2.0f;
			// 影の番号を設定
			g_aItem[nCntItem].nIdxShadow = SetShadow(g_aItem[nCntItem].pos);
			g_aItem[nCntItem].Type = Type;		// タイプ識別子を設定
			g_aItem[nCntItem].bAlpha = false;	// 透明かどうかを設定する
			g_aItem[nCntItem].bUse = true;		// 使用しているかどうか設定する
			break;
		}
	}

	return nCntItem;
}

//------------------------------
// 位置取得
//------------------------------
Item* GetItem(void)
{
	return &g_aItem[0]; //アイテム情報
}

//------------------------------
// 種類数取得
//------------------------------
int GetItemNum(void)
{
	return g_ItemManager.ItemNum; //アイテムの種類数
}

//------------------------------
// 当たり判定
//------------------------------
bool TriggerItem(D3DXVECTOR3 pos, Item** pItem)
{
	bool bGeting = false;
	int nCntItem = 0;

	for (nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{//最大数回す
		if (g_aItem[nCntItem].bUse)
		{//使用している
			if (sqrtf((g_aItem[nCntItem].pos.x - pos.x) * (g_aItem[nCntItem].pos.x - pos.x) + (g_aItem[nCntItem].pos.y - pos.y) * (g_aItem[nCntItem].pos.y - pos.y) + (g_aItem[nCntItem].pos.z - pos.z) * (g_aItem[nCntItem].pos.z - pos.z)) <= ITEM_TRIGGER_SPACE)
			{//近づいたら
				bGeting = true; //ゲット可能
				if (pItem != NULL)
				{// アイテム情報を渡す
					*pItem = &g_aItem[nCntItem];
				}
				break;
			}
		}
	}
	return bGeting;
}

//------------------------------
// Xファイル名取得
//------------------------------
char* GetItemName(int nNumber)
{
	return (nNumber < (int)ITYPE_MAX&& nNumber >= 0) ? g_ItemManager.ItemModel[nNumber].Pass.aName : NULL; //名前を渡す
}

//---------------------------------------
// ロード処理
//---------------------------------------
void LoadItemManager(void)
{
	FILE* pFile;	// ファイルポインタを宣言
	ItemManager ItemManager;
	int nCount = 0;

	//初期化
	ItemManager.ItemNum = 0;
	for (int nCntTex = 0; nCntTex < ITEM_TEXTURE_MAX; nCntTex++)
	{//テクスチャ数繰り返す
		ItemManager.apTexture[nCntTex] = NULL;
	}

	for (int nCntModel = 0; nCntModel < ITYPE_MAX; nCntModel++)
	{// 種類数繰り返す
		//すべて初期化
		*ItemManager.ItemModel[nCntModel].Pass.aName = '\0';
		ItemManager.ItemModel[nCntModel].pMesh = NULL;
		ItemManager.ItemModel[nCntModel].pBuffMat = NULL;
		ItemManager.ItemModel[nCntModel].dwNumMat = 0;
		for (int nCntMat = 0; nCntMat < ITEM_SUBSET_MAX; nCntMat++)
		{//マテリアル数繰り返す
			ItemManager.ItemModel[nCntModel].TexIdx[nCntMat] = -1;
		}
		ItemManager.ItemModel[nCntModel].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ItemManager.ItemModel[nCntModel].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ItemManager.ItemModel[nCntModel].nNumVtx = 0;
		ItemManager.ItemModel[nCntModel].sizeFVF = 0;
		ItemManager.ItemModel[nCntModel].pVtxBuff = NULL;
	}

	// Xファイルを開く
	if (fopen_s(&pFile, IMANAGER_FILE, "r") == 0 && pFile != NULL)
	{//開けたら
		if (fscanf(pFile, "%d", &ItemManager.ItemNum) == EOF)//アイテム数
		{
			fclose(pFile);//ファイルを閉じる
			return;
		}

		if (ItemManager.ItemNum > ITYPE_MAX)
		{//タイプ以上のファイル数
			HWND hWnd;
			hWnd = GethWnd();
			MessageBox(hWnd, "ロードエラー", "Xファイル数が多すぎます", MB_OK | MB_ICONERROR);
			PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
		}

		for (nCount = 0; nCount < ItemManager.ItemNum; nCount++)
		{//パスの取得
			if (fscanf(pFile, "%63s", ItemManager.ItemModel[nCount].Pass.aName) == EOF)break;//数値を書き入れ
		}

		//設定
		SetItemManager(ItemManager);

		fclose(pFile);//ファイルを閉じる
	}
	else
	{//開けなかった
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "ロードエラー", "ItemManager", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//---------------------------------------
// ロード処理
//---------------------------------------
void LoadItem(void)
{
	FILE* pFile;	// ファイルポインタを宣言
	MODE Mode = GetMode();
	int nCount = 0;
	int nNumber = 0;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//Xファイル
	switch (Mode)
	{
	case MODE_GAME:
		pFile = fopen(ITEM_FILE, "r");	// ファイルを開く
		break;
	case MODE_TUTO:
		pFile = fopen(ITEM_TUTO_FILE, "r");	// ファイルを開く
		break;
	default:
		pFile = fopen(nullptr, "r");//ファイルを開く
		break;
	}

	if (pFile != NULL)
	{// 開けたら
		for (nCount = 0; nCount < ITEM_MAX; nCount++)
		{//アイテム数回す
			if (fscanf(pFile, "%d", &nNumber) == EOF)break;		// 数値を書き入れ
			if (fscanf(pFile, "%f", &pos.x) == EOF)break;		// 数値を書き入れ
			if (fscanf(pFile, "%f", &pos.y) == EOF)break;		// 数値を書き入れ
			if (fscanf(pFile, "%f", &pos.z) == EOF)break;		// 数値を書き入れ
			if (fscanf(pFile, "%f", &rot.x) == EOF)break;		// 数値を書き入れ
			if (fscanf(pFile, "%f", &rot.y) == EOF)break;		// 数値を書き入れ
			if (fscanf(pFile, "%f", &rot.z) == EOF)break;		// 数値を書き入れ
			if (fscanf(pFile, "%f", &scale.x) == EOF)break;		// 数値を書き入れ
			if (fscanf(pFile, "%f", &scale.y) == EOF)break;		// 数値を書き入れ
			if (fscanf(pFile, "%f", &scale.z) == EOF)break;		// 数値を書き入れ

			// 角度変換
			rot = D3DXToRadian(rot);

			//異常番号
			switch (Mode)
			{
			case MODE_GAME:
				if (nNumber >= g_ItemManager.ItemNum - ITYPE_TUTO_MAX || nNumber < 0)
				{
					continue;
				}
				break;
			case MODE_TUTO:
				if (nNumber < g_ItemManager.ItemNum - ITYPE_TUTO_MAX || nNumber >= g_ItemManager.ItemNum)
				{
					continue;
				}
				break;
			}

			// オブジェクト設置
			SetItem((ITYPE)nNumber, pos, rot, scale);
		}
		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// 開けなかった
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "ロードエラー", "Item", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//---------------------------------------
//ロード処理
//---------------------------------------
void SaveItem(void)
{
	FILE* pFile;	// ファイルポインタを宣言
	MODE Mode = GetMode();
	D3DXVECTOR3 rot;

	// Xファイル
	switch (Mode)
	{
	case MODE_GAME:
		pFile = fopen(ITEM_FILE, "w");	// ファイルを開く
		break;
	case MODE_TUTO:
		pFile = fopen(ITEM_TUTO_FILE, "w");	// ファイルを開く
		break;
	default:
		pFile = fopen(nullptr, "w");//ファイルを開く
		break;
	}

	if (pFile != NULL)
	{// 開けたら
		for (int nCount = 0; nCount < ITEM_MAX; nCount++)
		{//アイテム数回す
			if (g_aItem[nCount].bUse == true)
			{//使っている
				// 角度変換
				rot = D3DXToDegree(g_aItem[nCount].rot);

				fprintf(pFile, "%d\n", (int)g_aItem[nCount].Type);	// 数値を書き入れ
				fprintf(pFile, "%.1f ", g_aItem[nCount].pos.x);		// 数値を書き入れ
				fprintf(pFile, "%.1f ", g_aItem[nCount].pos.y);		// 数値を書き入れ
				fprintf(pFile, "%.1f\n", g_aItem[nCount].pos.z);	// 数値を書き入れ
				fprintf(pFile, "%.1f ", rot.x);						// 数値を書き入れ
				fprintf(pFile, "%.1f ", rot.y);						// 数値を書き入れ
				fprintf(pFile, "%.1f\n", rot.z);					// 数値を書き入れ
				fprintf(pFile, "%.1f ", g_aItem[nCount].scale.x);	// 数値を書き入れ
				fprintf(pFile, "%.1f ", g_aItem[nCount].scale.y);	// 数値を書き入れ
				fprintf(pFile, "%.1f\n", g_aItem[nCount].scale.z);	// 数値を書き入れ
			}
		}
		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// 開けなかった
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "ロードエラー", "ロードできなかったよ", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//--------------------------
// オブジェクトエディター
//--------------------------
void UpdateItemEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot)
{
	Camera* pCamera = GetCamera();
	static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	static int HoldNumber = -1, EndNumber = -1;
	MODE Mode = GetMode();

	if (GetKeyboardTrigger(DIK_RIGHT))
	{// オブジェクト切替
		g_Type = (ITYPE)(g_Type + 1);
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{// オブジェクト切替
		g_Type = (ITYPE)(g_Type - 1);
	}

	switch (Mode)
	{
	case MODE_GAME:
		//範囲制限
		g_Type = (ITYPE)(g_Type % (g_ItemManager.ItemNum - ITYPE_TUTO_MAX));

		if (g_Type < 0)
		{//ループ
			g_Type = (ITYPE)(g_ItemManager.ItemNum - 1 - ITYPE_TUTO_MAX);
		}	
		break;
	case MODE_TUTO:
		//範囲制限
		if (g_Type > g_ItemManager.ItemNum - 1)
		{//ループ
			g_Type = (ITYPE)(g_ItemManager.ItemNum - ITYPE_TUTO_MAX);
		}

		if (g_Type < g_ItemManager.ItemNum - ITYPE_TUTO_MAX)
		{//ループ
			g_Type = (ITYPE)(g_ItemManager.ItemNum - 1);
		}
		break;
	}

	if (GetKeyboardTrigger(DIK_UP))
	{// DELオブジェクト切替
		g_DelItem++;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{// DELオブジェクト切替
		g_DelItem--;
	}

	for (int nCount = 0; nCount < ITEM_MAX; nCount++)
	{//最大数回す

		//範囲制限
		if (g_DelItem < 0)
		{
			g_DelItem = ITEM_MAX - 1;
		}
		g_DelItem %= ITEM_MAX;

		if (g_aItem[g_DelItem].bUse)
		{//使っているオブジェクトの選択
			break;
		}
		else
		{//使っていない番号はスキップ

			if (GetKeyboardTrigger(DIK_DOWN))
			{// DELオブジェクト切替
				g_DelItem--;
			}
			else
			{// DELオブジェクト切替
				g_DelItem++;
			}
		}

		if (nCount == ITEM_MAX - 1)
		{//オブジェクトがない
			g_DelItem = -1;
		}
	}

	if (GetKeyboardPress(DIK_F) == true || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) == true)//Aキー
	{
		if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//Wキー
		{//左上
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.25f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.25f) * 10.0f;
		}
		else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//Sキー
		{//左下
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.75f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.75f) * 10.0f;
		}
		else
		{//左
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.5f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.5f) * 10.0f;
		}
	}
	else if (GetKeyboardPress(DIK_H) == true || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) == true)//Dキー
	{
		if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//Wキー
		{//右上
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * 10.0f;
		}
		else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//Sキー
		{//右下
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * 10.0f;
		}
		else
		{//右
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * 10.0f;
		}
	}
	else if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//Wキー
	{//上
		pos.x += sinf(pCamera->rot.y) * 10.0f;
		pos.z += cosf(pCamera->rot.y) * 10.0f;
	}
	else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//Sキー
	{//下
		pos.x += sinf(pCamera->rot.y + D3DX_PI) * 10.0f;
		pos.z += cosf(pCamera->rot.y + D3DX_PI) * 10.0f;
	}

	if (GetKeyboardPress(DIK_V) == true)//Wキー
	{//上
		pos.y -= 10.0f;
	}
	if (GetKeyboardPress(DIK_B) == true)//Sキー
	{//下
		pos.y += 10.0f;
	}

	if (GetKeyboardPress(DIK_U) || GetJoykeyPress(JOYKEY_A, CONTROLLER_1))
	{// 押している間
		rot.x -= 0.1f;
	}
	if (GetKeyboardPress(DIK_I) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// 押している間
		rot.x += 0.1f;
	}
	if (GetKeyboardPress(DIK_J) || GetJoykeyPress(JOYKEY_LT, CONTROLLER_1))
	{// 押している間
		rot.y -= 0.1f;
	}
	if (GetKeyboardPress(DIK_K) || GetJoykeyPress(JOYKEY_RT, CONTROLLER_1))
	{// 押している間
		rot.y += 0.1f;
	}
	if (GetKeyboardPress(DIK_M) || GetJoykeyPress(JOYKEY_X, CONTROLLER_1))
	{// 押している間
		rot.z -= 0.1f;
	}
	if (GetKeyboardPress(DIK_COMMA) || GetJoykeyPress(JOYKEY_Y, CONTROLLER_1))
	{// 押している間
		rot.z += 0.1f;
	}
	if (GetKeyboardPress(DIK_7) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// 押している間
		scale.x -= 0.2f;
		scale.y -= 0.2f;
		scale.z -= 0.2f;
	}
	if (GetKeyboardPress(DIK_8) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// 押している間
		scale.x += 0.2f;
		scale.y += 0.2f;
		scale.z += 0.2f;
	}

	if (GetKeyboardTrigger(DIK_0) || GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1))
	{// 初期化
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

	//正規化
	if (rot.x > D3DX_PI)
	{
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x < -D3DX_PI)
	{
		rot.x += D3DX_PI * 2.0f;
	}
	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}
	if (rot.z > D3DX_PI)
	{
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z < -D3DX_PI)
	{
		rot.z += D3DX_PI * 2.0f;
	}

	//スケール制限
	scale.x = max(ITEM_SCALE_MIN, min(ITEM_SCALE_MAX, scale.x));
	scale.y = max(ITEM_SCALE_MIN, min(ITEM_SCALE_MAX, scale.y));
	scale.z = max(ITEM_SCALE_MIN, min(ITEM_SCALE_MAX, scale.z));

	if (GetKeyboardTrigger(DIK_RSHIFT) || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1))
	{// 押したとき
		HoldNumber = SetItem(g_Type, Epos + pos, Erot + rot, scale);
		EndNumber = HoldNumber;
	}
	if (GetKeyboardPress(DIK_RSHIFT) || GetJoykeyPress(JOYKEY_RB, CONTROLLER_1))
	{// 押している間
		g_aItem[HoldNumber].pos = Epos + pos;
		g_aItem[HoldNumber].rot = Erot + rot;
		g_aItem[HoldNumber].scale = scale;

		static ITYPE Type = ITYPE_1; //一時保存用ナンバー

		if (Type != g_Type)
		{//ナンバーが変化した
			//オブジェクトの消去
			NullShadow(g_aItem[HoldNumber].nIdxShadow);
			g_aItem[HoldNumber].bUse = false;
			//オブジェクトの切り替え
			HoldNumber = SetItem(g_Type, Epos + pos, Erot + rot, scale);
			EndNumber = HoldNumber;
			Type = g_Type; //ナンバーの更新
		}
	}
	else
	{
		HoldNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_BACK) || GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1))
	{// 押したとき
		NullShadow(g_aItem[EndNumber].nIdxShadow);
		g_aItem[EndNumber].bUse = false;
		EndNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_F8))
	{// セーブ
		SaveItem();
	}

	if (GetKeyboardTrigger(DIK_DELETE) || (GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1) && GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1)))
	{// 押したとき
		if (g_DelItem != -1)
		{
			NullShadow(g_aItem[g_DelItem].nIdxShadow);
			g_aItem[g_DelItem].bUse = false;
		}
	}

	static int DelNumber = -1; //一時保存用ナンバー

	if (DelNumber != g_DelItem)
	{//デリートナンバーが変化した
		if (DelNumber != -1)
		{//前回のオブジェクトが存在する
			g_aItem[DelNumber].bAlpha = false; //元に戻す
		}
		DelNumber = g_DelItem; //選択オブジェクトの更新
		if (DelNumber != -1)
		{//オブジェクトが存在する
			g_aItem[DelNumber].bAlpha = true; //元の値の保持
		}
	}
}

//--------------------
//現在のXファイルナンバー
//--------------------
int GetInumber(void)
{
	return (int)g_Type; //現在の種類(Edit)
}

//--------------------
//現在のデリートナンバー
//--------------------
int GetIDelnumber(void)
{
	return g_DelItem; //DELキーで消えるインデックス番号
}

//==================================================
// アイテムUIを表示する処理 misaki
//==================================================
void DisplayItem(bool bDisp, int nType)
{
	if (g_itemUI[nType].bUse == false)
	{
		g_itemUI[nType].bUse = bDisp;
	}
}