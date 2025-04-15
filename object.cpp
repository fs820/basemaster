//----------------------------------------
//
//オブジェクト処理[Object.cpp]
//Author fuma sato
//
//----------------------------------------

#include"object.h"
#include"shadow.h"
#include"player.h"
#include"camera.h"
#include"meshfield.h"
#include"barrier.h"
#include"rotation.h"
#include"input.h"
#include"vectorcollision.h"

//グローバル変数宣言
Object g_aObject[OBJECT_MAX] = {}; //オブジェクト情報体
ObjectManager g_ObjectManager;     //ファイル情報体
OTYPE g_Type;                      //オブジェクト番号管理
int g_DelObject;                   //オブジェクト消去用
//----------------------
//初期化処理
//----------------------
void InitObject(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	int nCntObject, nCntTex;
	for (nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//最大数繰り返す

		//情報の初期化
		g_aObject[nCntObject].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObject[nCntObject].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObject[nCntObject].fLength = 0.0f;
		g_aObject[nCntObject].fHeight = 0.0f;
		g_aObject[nCntObject].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		for (int nCntShadow = 0; nCntShadow < 2; nCntShadow++)
		{
			g_aObject[nCntObject].nIdxShadow[nCntShadow] = -1;
		}
		g_aObject[nCntObject].Type = OTYPE_ROKET;
		g_aObject[nCntObject].bAlpha = false;
		g_aObject[nCntObject].bUse = false;
		g_aObject[nCntObject].nIdxRadar = -1;	// レーダーインデックス tetsuji
	}

	//ファイル情報の初期化
	g_ObjectManager.ObjectNum = 0;
	for (nCntTex = 0; nCntTex < OBJECT_TEXTURE_MAX; nCntTex++)
	{//テクスチャ数繰り返す
		g_ObjectManager.apTexture[nCntTex] = NULL;
	}
	int nCntModel, nCntMat;
	for (nCntModel = 0; nCntModel < OTYPE_MAX; nCntModel++)
	{//種類数回す
		*g_ObjectManager.ObjectModel[nCntModel].Pass.aName = '\0';
		g_ObjectManager.ObjectModel[nCntModel].pMesh = NULL;
		g_ObjectManager.ObjectModel[nCntModel].pBuffMat = NULL;
		g_ObjectManager.ObjectModel[nCntModel].dwNumMat = 0;
		for (nCntMat = 0; nCntMat < OBJECT_SUBSET_MAX; nCntMat++)
		{//マテリアル数繰り返す
			g_ObjectManager.ObjectModel[nCntModel].TexIdx[nCntMat] = -1;
		}
		g_ObjectManager.ObjectModel[nCntModel].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ObjectManager.ObjectModel[nCntModel].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ObjectManager.ObjectModel[nCntModel].nNumVtx = 0;
		g_ObjectManager.ObjectModel[nCntModel].sizeFVF = 0;
		g_ObjectManager.ObjectModel[nCntModel].pVtxBuff = NULL;
	}

	//オブジェクトファイル情報取得
	LoadObjectManager();

	//番号の初期化
	g_Type = OTYPE_ROKET;
	g_DelObject = -1;
}

//-------------------
//終了処理
//-------------------
void UninitObject(void)
{
	for (int nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//最大数繰り返す
		//情報の初期化
		g_aObject[nCntObject].bUse = false;
	}

	for (int nCntTex = 0; nCntTex < OBJECT_TEXTURE_MAX; nCntTex++)
	{//テクスチャの最大数繰り返す tetsuji
		//テクスチャの破棄
		if (g_ObjectManager.apTexture[nCntTex] != NULL)
		{
			g_ObjectManager.apTexture[nCntTex]->Release();
			g_ObjectManager.apTexture[nCntTex] = NULL;
		}
	}
	for (int nCntModel = 0; nCntModel < OTYPE_MAX; nCntModel++)
	{//最大数繰り返す
		//頂点バッファの破棄
		if (g_ObjectManager.ObjectModel[nCntModel].pVtxBuff != NULL)
		{
			g_ObjectManager.ObjectModel[nCntModel].pVtxBuff = NULL;
		}
		//メッシュの破棄
		if (g_ObjectManager.ObjectModel[nCntModel].pMesh != NULL)
		{
			g_ObjectManager.ObjectModel[nCntModel].pMesh->Release();
			g_ObjectManager.ObjectModel[nCntModel].pMesh = NULL;
		}
		//マテリアルの破棄
		if (g_ObjectManager.ObjectModel[nCntModel].pBuffMat != NULL)
		{
			g_ObjectManager.ObjectModel[nCntModel].pBuffMat->Release();
			g_ObjectManager.ObjectModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//-------------------
//更新処理
//-------------------
void UpdateObject(void)
{
	for (int nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//最大数繰り返す
		if (g_aObject[nCntObject].bUse)
		{//使用時
			switch (g_aObject[nCntObject].Type)
			{
			case OTYPE_ROKET:
				SetPositionShadow(g_aObject[nCntObject].nIdxShadow[0], g_aObject[nCntObject].pos, g_aObject[nCntObject].scale, g_aObject[nCntObject].fLength);
				break;
			}
		}
	}
}

//-------------------
//描画処理
//-------------------
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;//計算マトリックス
	D3DMATERIAL9 matDef;//現在のマテリアル保存用
	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//デバイスの取得
	pDevice = GetDevice();
	int nCntObject;
	for (nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//最大数繰り返す
		if (g_aObject[nCntObject].bUse)
		{//使用時
			//マトリックス初期化
			D3DXMatrixIdentity(&g_aObject[nCntObject].mtxWorld);

			//大きさの反映
			D3DXMatrixScaling(&mtxScale, g_aObject[nCntObject].scale.x, g_aObject[nCntObject].scale.y, g_aObject[nCntObject].scale.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxScale);

			//向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObject[nCntObject].rot.y, g_aObject[nCntObject].rot.x, g_aObject[nCntObject].rot.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxRot);

			//位置の反映
			D3DXMatrixTranslation(&mtxTrans, g_aObject[nCntObject].pos.x, g_aObject[nCntObject].pos.y, g_aObject[nCntObject].pos.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aObject[nCntObject].mtxWorld);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_ObjectManager.ObjectModel[(int)g_aObject[nCntObject].Type].pBuffMat->GetBufferPointer();

			// 各サブセットのフェイス数を取得
			DWORD numSubsets = 0;
			g_ObjectManager.ObjectModel[(int)g_aObject[nCntObject].Type].pMesh->GetAttributeTable(nullptr, &numSubsets);

			D3DXATTRIBUTERANGE* pAttributeTable = new D3DXATTRIBUTERANGE[numSubsets];
			g_ObjectManager.ObjectModel[(int)g_aObject[nCntObject].Type].pMesh->GetAttributeTable(pAttributeTable, &numSubsets);

			for (int nCntSubset = 0; nCntSubset < (int)numSubsets; nCntSubset++)
			{//サブセット数回す
				if ((int)pAttributeTable[nCntSubset].FaceCount > 0) // 使われているマテリアルのみ描画
				{
					//現在のマテリアル取得
					pDevice->GetMaterial(&matDef);

					// 透明度の設定
					D3DMATERIAL9 mat = pMat[pAttributeTable[nCntSubset].AttribId].MatD3D;
					mat.Diffuse.a = 1.0f - 0.5f * g_aObject[nCntObject].bAlpha;
					// マテリアルの設定
					pDevice->SetMaterial(&mat);

					if (g_ObjectManager.ObjectModel[(int)g_aObject[nCntObject].Type].TexIdx[nCntSubset] != -1)
					{//テクスチャがある
						// テクスチャ設定
						pDevice->SetTexture(0, g_ObjectManager.apTexture[g_ObjectManager.ObjectModel[(int)g_aObject[nCntObject].Type].TexIdx[nCntSubset]]);
					}
					else
					{
						pDevice->SetTexture(0, nullptr);
					}

					// サブセット描画
					g_ObjectManager.ObjectModel[(int)g_aObject[nCntObject].Type].pMesh->DrawSubset(pAttributeTable[nCntSubset].AttribId);

					//マテリアル保存
					pDevice->SetMaterial(&matDef);
				}
			}
			delete[] pAttributeTable; // メモリ解放
		}
	}
}

//------------------------------
//読み込んだファイル情報の取得
//------------------------------
void SetObjectManager(ObjectManager ObjectManager)
{
	g_ObjectManager = ObjectManager;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ
	int nCountModel, nCntTex = 0, nCntSubset;
	//デバイスの取得
	pDevice = GetDevice();

	nCntTex = 0;
	LPSTR Filename[OBJECT_TEXTURE_MAX + 1]; //テクスチャパス一時保存用
	for (nCountModel = 0; nCountModel < g_ObjectManager.ObjectNum; nCountModel++)
	{
		//Xファイル読み込み
		D3DXLoadMeshFromX
		(
			g_ObjectManager.ObjectModel[nCountModel].Pass.aName,
			D3DXMESH_MANAGED,
			pDevice,
			NULL,
			&g_ObjectManager.ObjectModel[nCountModel].pBuffMat,
			NULL,
			&g_ObjectManager.ObjectModel[nCountModel].dwNumMat,
			&g_ObjectManager.ObjectModel[nCountModel].pMesh
		);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_ObjectManager.ObjectModel[nCountModel].pBuffMat->GetBufferPointer();

		// 各サブセットのフェイス数を取得
		DWORD numSubsets = 0;
		g_ObjectManager.ObjectModel[nCountModel].pMesh->GetAttributeTable(nullptr, &numSubsets);

		D3DXATTRIBUTERANGE* pAttributeTable = new D3DXATTRIBUTERANGE[numSubsets];
		g_ObjectManager.ObjectModel[nCountModel].pMesh->GetAttributeTable(pAttributeTable, &numSubsets);

		for (nCntSubset = 0; nCntSubset < (int)numSubsets; nCntSubset++)
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
						g_ObjectManager.ObjectModel[nCountModel].TexIdx[nCntSubset] = nCntTex2; //同じテクスチャを登録
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
						&g_ObjectManager.apTexture[nCntTex]
					);

					//読み込んだテクスチャを登録
					g_ObjectManager.ObjectModel[nCountModel].TexIdx[nCntSubset] = nCntTex;
					//テクスチャ番号進行
					nCntTex++;
				}
			}
			else
			{//テクスチャがない
				g_ObjectManager.ObjectModel[nCountModel].TexIdx[nCntSubset] = -1;
			}
		}
		delete[] pAttributeTable; // メモリ解放

		//頂点数
		g_ObjectManager.ObjectModel[nCountModel].nNumVtx = g_ObjectManager.ObjectModel[nCountModel].pMesh->GetNumVertices();
		//頂点サイズ
		g_ObjectManager.ObjectModel[nCountModel].sizeFVF = D3DXGetFVFVertexSize(g_ObjectManager.ObjectModel[nCountModel].pMesh->GetFVF());
		//頂点バッファの取得
		g_ObjectManager.ObjectModel[nCountModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_ObjectManager.ObjectModel[nCountModel].pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < g_ObjectManager.ObjectModel[nCountModel].nNumVtx; nCntVtx++)
		{//頂点数繰り返す
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_ObjectManager.ObjectModel[nCountModel].pVtxBuff;

			//頂点の大きさの反映
			if (vtx.x > g_ObjectManager.ObjectModel[nCountModel].vtxMax.x)
			{
				g_ObjectManager.ObjectModel[nCountModel].vtxMax.x = vtx.x;
			}
			if (vtx.x < g_ObjectManager.ObjectModel[nCountModel].vtxMin.x)
			{
				g_ObjectManager.ObjectModel[nCountModel].vtxMin.x = vtx.x;
			}
			if (vtx.y > g_ObjectManager.ObjectModel[nCountModel].vtxMax.y)
			{
				g_ObjectManager.ObjectModel[nCountModel].vtxMax.y = vtx.y;
			}
			if (vtx.y < g_ObjectManager.ObjectModel[nCountModel].vtxMin.y)
			{
				g_ObjectManager.ObjectModel[nCountModel].vtxMin.y = vtx.y;
			}
			if (vtx.z > g_ObjectManager.ObjectModel[nCountModel].vtxMax.z)
			{
				g_ObjectManager.ObjectModel[nCountModel].vtxMax.z = vtx.z;
			}
			if (vtx.z < g_ObjectManager.ObjectModel[nCountModel].vtxMin.z)
			{
				g_ObjectManager.ObjectModel[nCountModel].vtxMin.z = vtx.z;
			}

			g_ObjectManager.ObjectModel[nCountModel].pVtxBuff += g_ObjectManager.ObjectModel[nCountModel].sizeFVF;
		}

		// 頂点バッファのアンロック
        g_ObjectManager.ObjectModel[nCountModel].pMesh->UnlockVertexBuffer();
	}
}

//-----------
//配置
//-----------
int SetObject(OTYPE Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	int nCntObject;
	for (nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//最大数繰り返す
		if (!g_aObject[nCntObject].bUse)
		{//使用時
			//オブジェクト情報の設定
			g_aObject[nCntObject].pos = pos;
			g_aObject[nCntObject].rot = rot;
			g_aObject[nCntObject].scale = scale;
			g_aObject[nCntObject].fLength = sqrtf((g_ObjectManager.ObjectModel[Type].vtxMax.x * g_aObject[nCntObject].scale.x - g_ObjectManager.ObjectModel[Type].vtxMin.x * g_aObject[nCntObject].scale.x) * (g_ObjectManager.ObjectModel[Type].vtxMax.x * g_aObject[nCntObject].scale.x - g_ObjectManager.ObjectModel[Type].vtxMin.x * g_aObject[nCntObject].scale.x) + (g_ObjectManager.ObjectModel[Type].vtxMax.z * g_aObject[nCntObject].scale.z - g_ObjectManager.ObjectModel[Type].vtxMin.z * g_aObject[nCntObject].scale.z) * (g_ObjectManager.ObjectModel[Type].vtxMax.z * g_aObject[nCntObject].scale.z - g_ObjectManager.ObjectModel[Type].vtxMin.z * g_aObject[nCntObject].scale.z)) / 2.0f;
			g_aObject[nCntObject].fHeight = g_ObjectManager.ObjectModel[Type].vtxMax.y * g_aObject[nCntObject].scale.y - g_ObjectManager.ObjectModel[Type].vtxMin.y * g_aObject[nCntObject].scale.y;
			switch (Type)
			{
			case OTYPE_ROKET:
				g_aObject[nCntObject].nIdxShadow[0] = SetShadow(g_aObject[nCntObject].pos);
				g_aObject[nCntObject].nIdxShadow[1] = -1;
				break;
			}
			g_aObject[nCntObject].Type = Type;
			g_aObject[nCntObject].bAlpha = false;
			g_aObject[nCntObject].bUse = true;
			break;
		}
	}
	return nCntObject;
}

//------------------------------
//位置取得
//------------------------------
Object* GetObject_EX(void)
{
	return &g_aObject[0]; //オブジェクト情報
}

//------------------------------
//種類数取得
//------------------------------
int GetObjectNum(void)
{
	return g_ObjectManager.ObjectNum; //種類数取得
}

//----------------------
//インデックスの取得
//----------------------
int GetObjectIdx(OTYPE Type, int Number)
{
	int nCnt = 0;
	for (int nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//最大数回す
		if (g_aObject[nCntObject].bUse&& g_aObject[nCntObject].Type==Type)
		{//指定のタイプの時
			if (Number == nCnt)
			{
				return nCntObject;
			}
			nCnt++;
		}
	}
	return 0;
}

//------------------------------
//当たり判定
//------------------------------
bool CollisionObject(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius, float fHeight, Object** pObject)
{
	bool bRanding = false;
	int nCntObject = 0;

	for (nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//最大数回す
		if (g_aObject[nCntObject].bUse && (g_aObject[nCntObject].Type == OTYPE_ROKET || (g_aObject[nCntObject].Type >= OTYPE_STONE1 && g_aObject[nCntObject].Type <= OTYPE_STONE4) || g_aObject[nCntObject].Type == OTYPE_TUTO_BLOCK || g_aObject[nCntObject].Type == OTYPE_TUTO_GATE))
		{
			// 4頂点のローカル座標を作る
			D3DXVECTOR3 RightUp = D3DXVECTOR3(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.x * g_aObject[nCntObject].scale.x, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.z * g_aObject[nCntObject].scale.z),
				         LeftUp = D3DXVECTOR3(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.x * g_aObject[nCntObject].scale.x, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.z * g_aObject[nCntObject].scale.z),
				       LeftDown = D3DXVECTOR3(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.x * g_aObject[nCntObject].scale.x, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.z * g_aObject[nCntObject].scale.z),
				      RightDown = D3DXVECTOR3(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.x * g_aObject[nCntObject].scale.x, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y, g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.z * g_aObject[nCntObject].scale.z);

			// 4頂点に回転を適応する
			RotationofShaftbyMatrix(RightUp, RightUp, g_aObject[nCntObject].rot, TYPE_Y);
			RotationofShaftbyMatrix(LeftUp, LeftUp, g_aObject[nCntObject].rot, TYPE_Y);
			RotationofShaftbyMatrix(LeftDown, LeftDown, g_aObject[nCntObject].rot, TYPE_Y);
			RotationofShaftbyMatrix(RightDown, RightDown, g_aObject[nCntObject].rot, TYPE_Y);

			// 2頂点をワールド座標にする
			RightUp += g_aObject[nCntObject].pos;
			LeftUp += g_aObject[nCntObject].pos;
			LeftDown += g_aObject[nCntObject].pos;
			RightDown += g_aObject[nCntObject].pos;

			D3DXVECTOR3 PosBox[4] = { RightUp,RightDown, LeftDown ,LeftUp }; // エリアの生成

			//y軸
			if (VectorInside(*pPos, fRadius, PosBox))
			{// 内側にいる
				//下から上
				if (pPosOld->y + fHeight <= g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y* g_aObject[nCntObject].scale.y && pPos->y + fHeight > g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y* g_aObject[nCntObject].scale.y)
				{
					pPos->y = g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y* g_aObject[nCntObject].scale.y - fHeight;
					pMove->y = 0.0f;
				}
				//上から下
				if (pPosOld->y >= g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y* g_aObject[nCntObject].scale.y && pPos->y < g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y* g_aObject[nCntObject].scale.y)
				{
					pPos->y = g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y * g_aObject[nCntObject].scale.y;
					pMove->y = 0.0f;
					bRanding = true;
					if (pObject != NULL)
					{
						*pObject = &g_aObject[nCntObject];
					}
				}
			}

			//x,z軸
			VectorCollision(*pPos, *pPosOld, fRadius, fHeight, RightUp, LeftUp, fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y * g_aObject[nCntObject].scale.y) + fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y));
			VectorCollision(*pPos, *pPosOld, fRadius, fHeight, LeftUp, LeftDown, fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y * g_aObject[nCntObject].scale.y) + fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y));
			VectorCollision(*pPos, *pPosOld, fRadius, fHeight, LeftDown, RightDown, fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y * g_aObject[nCntObject].scale.y) + fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y));
			VectorCollision(*pPos, *pPosOld, fRadius, fHeight, RightDown, RightUp, fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y * g_aObject[nCntObject].scale.y) + fabsf(g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y * g_aObject[nCntObject].scale.y));
		}
	}
	return bRanding;
}

//------------------------------
//当たり判定
//------------------------------
bool TriggerObject(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fRadius)
{
	bool bTrigger = false;
	int nCntObject = 0;

	for (nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//最大数回す
		if (g_aObject[nCntObject].bUse && (g_aObject[nCntObject].Type == OTYPE_ROKET || (g_aObject[nCntObject].Type >= OTYPE_STONE1 && g_aObject[nCntObject].Type <= OTYPE_STONE4) || g_aObject[nCntObject].Type == OTYPE_TUTO_BLOCK))
		{
			//y軸
			if (pPos->x + fRadius > g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.x* g_aObject[nCntObject].scale.x && pPos->x - fRadius < g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.x* g_aObject[nCntObject].scale.x && pPos->z + fRadius > g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.z* g_aObject[nCntObject].scale.z && pPos->z - fRadius < g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.z* g_aObject[nCntObject].scale.z)
			{
				//下から上
				if (pPosOld->y + fRadius <= g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y* g_aObject[nCntObject].scale.y && pPos->y + fRadius > g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y* g_aObject[nCntObject].scale.y)
				{
					bTrigger = true;
				}
				//上から下
				if (pPosOld->y - fRadius >= g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y* g_aObject[nCntObject].scale.y && pPos->y - fRadius < g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y* g_aObject[nCntObject].scale.y)
				{
					bTrigger = true;
				}
			}
			//x,z軸
			if (pPos->y + fRadius > g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.y* g_aObject[nCntObject].scale.y && pPos->y - fRadius < g_aObject[nCntObject].pos.y + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.y* g_aObject[nCntObject].scale.y)
			{
				//x軸
				if (pPos->z + fRadius > g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.z* g_aObject[nCntObject].scale.z && pPos->z - fRadius < g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.z* g_aObject[nCntObject].scale.z)
				{
					//左から右
					if (pPosOld->x + fRadius <= g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.x* g_aObject[nCntObject].scale.x && pPos->x + fRadius > g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.x* g_aObject[nCntObject].scale.x)
					{
						bTrigger = true;
					}
					//右から左
					if (pPosOld->x - fRadius >= g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.x* g_aObject[nCntObject].scale.x && pPos->x - fRadius < g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.x* g_aObject[nCntObject].scale.x)
					{
						bTrigger = true;
					}
				}
				//z軸
				if (pPos->x + fRadius > g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.x* g_aObject[nCntObject].scale.x && pPos->x - fRadius < g_aObject[nCntObject].pos.x + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.x* g_aObject[nCntObject].scale.x)
				{
					//手前から奥
					if (pPosOld->z + fRadius <= g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.z* g_aObject[nCntObject].scale.z && pPos->z + fRadius > g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMin.z* g_aObject[nCntObject].scale.z)
					{
						bTrigger = true;
					}
					//奥から手前
					if (pPosOld->z - fRadius >= g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.z* g_aObject[nCntObject].scale.z && pPos->z - fRadius < g_aObject[nCntObject].pos.z + g_ObjectManager.ObjectModel[g_aObject[nCntObject].Type].vtxMax.z* g_aObject[nCntObject].scale.z)
					{
						bTrigger = true;
					}
				}
			}
		}
	}
	return bTrigger;
}

//-------------------------------------------------
//当たり判定
//-------------------------------------------------
bool InsideObject(OTYPE Type, D3DXVECTOR3 pos, Object** pObject)
{
	bool bGeting = false;
	int nCntObject = 0;

	for (nCntObject = 0; nCntObject < OBJECT_MAX; nCntObject++)
	{//最大数回す
		if (g_aObject[nCntObject].bUse && g_aObject[nCntObject].Type == Type)
		{//指定されたタイプのオブジェクト
			float TriggerSpace = 0.0f;
			if (g_aObject[nCntObject].Type == OTYPE_ROKET || g_aObject[nCntObject].Type == OTYPE_TUTO_GATE)
			{
				TriggerSpace = ROKET_TRIGGER_SPACE;
			}
			else if (g_aObject[nCntObject].Type == OTYPE_TUTO_BLOCK)
			{
				TriggerSpace = g_aObject[nCntObject].fLength;
			}

			if (sqrtf((g_aObject[nCntObject].pos.x - pos.x) * (g_aObject[nCntObject].pos.x - pos.x) + (g_aObject[nCntObject].pos.y - pos.y) * (g_aObject[nCntObject].pos.y - pos.y) + (g_aObject[nCntObject].pos.z - pos.z) * (g_aObject[nCntObject].pos.z - pos.z)) <= TriggerSpace)
			{//近づいたら
				if (pObject!=NULL)
				{
					*pObject = &g_aObject[nCntObject];
				}
				bGeting = true; //取得可能
				break;
			}
		}
	}
	return bGeting;
}

//------------------------------
//Xファイル名取得
//------------------------------
char* GetObjectName(int nNumber)
{
	return (nNumber < OTYPE_MAX&& nNumber >= 0) ? g_ObjectManager.ObjectModel[nNumber].Pass.aName : NULL; //パスを返す
}

//---------------------------------------
//ロード処理
//---------------------------------------
void LoadObjectManager(void)
{
	FILE* pFile;//ファイルポインタを宣言
	ObjectManager ObjectManager;
	int nCount = 0;

	//初期化
	int nCntModel, nCntTex, nCntMat;
	ObjectManager.ObjectNum = 0;
	for (nCntTex = 0; nCntTex < OBJECT_TEXTURE_MAX; nCntTex++)
	{//テクスチャ数繰り返す
		ObjectManager.apTexture[nCntTex] = NULL;
	}
	for (nCntModel = 0; nCntModel < OTYPE_MAX; nCntModel++)
	{//最大数回す
		*ObjectManager.ObjectModel[nCntModel].Pass.aName = '\0';
		ObjectManager.ObjectModel[nCntModel].pMesh = NULL;
		ObjectManager.ObjectModel[nCntModel].pBuffMat = NULL;
		ObjectManager.ObjectModel[nCntModel].dwNumMat = 0;
		for (nCntMat = 0; nCntMat < OBJECT_SUBSET_MAX; nCntMat++)
		{//マテリアル数繰り返す
			ObjectManager.ObjectModel[nCntModel].TexIdx[nCntMat] = -1;
		}
		ObjectManager.ObjectModel[nCntModel].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ObjectManager.ObjectModel[nCntModel].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ObjectManager.ObjectModel[nCntModel].nNumVtx = 0;
		ObjectManager.ObjectModel[nCntModel].sizeFVF = 0;
		ObjectManager.ObjectModel[nCntModel].pVtxBuff = NULL;
	}

	//Xファイル
	pFile = fopen(OMANAGER_FILE, "r");//ファイルを開く
	if (pFile != NULL)
	{//開けたら
		if (fscanf(pFile, "%d", &ObjectManager.ObjectNum) == EOF)//数値を書き入れ
		{//テキストがない
			fclose(pFile);//ファイルを閉じる
			return;
		}

		if (ObjectManager.ObjectNum > OTYPE_MAX)
		{//タイプ以上のファイル数
			HWND hWnd;
			hWnd = GethWnd();
			MessageBox(hWnd, "ロードエラー", "Xファイル数が多すぎます", MB_OK | MB_ICONERROR);
			PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
		}

		for (nCount = 0; nCount < ObjectManager.ObjectNum; nCount++)
		{//指定数回す
			if (fscanf(pFile, "%63s", ObjectManager.ObjectModel[nCount].Pass.aName) == EOF)break;//数値を書き入れ
		}

		//設定
		SetObjectManager(ObjectManager);

		fclose(pFile);//ファイルを閉じる
	}
	else
	{//開けなかった
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "ロードエラー", "ObjectManager", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//---------------------------------------
//ロード処理
//---------------------------------------
void LoadObject(void)
{
	FILE* pFile;//ファイルポインタを宣言
	MODE Mode = GetMode();
	int nCount = 0;
	int nNumber = 0;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//Xファイル
	switch (Mode)
	{
	case MODE_GAME:
		pFile = fopen(OBJECT_FILE, "r");//ファイルを開く
		break;
	case MODE_TUTO:
		pFile = fopen(OBJECT_TUTO_FILE, "r");//ファイルを開く
		break;
	default:
		pFile = fopen(nullptr, "r");//ファイルを開く
		break;
	}

	if (pFile != NULL)
	{//開けたら
		for (nCount = 0; nCount < OBJECT_MAX; nCount++)
		{//最大数回す
			if (fscanf(pFile, "%d", &nNumber) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &pos.x) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &pos.y) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &pos.z) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &rot.x) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &rot.y) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &rot.z) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &scale.x) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &scale.y) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &scale.z) == EOF)break;//数値を書き入れ

			//角度変換
			rot = D3DXToRadian(rot);

			//異常番号
			switch (Mode)
			{
			case MODE_GAME:
				if (nNumber >= g_ObjectManager.ObjectNum - OTYPE_TUTO_MAX || nNumber < 0)
				{
					continue;
				}
				break;
			case MODE_TUTO:
				if (nNumber < g_ObjectManager.ObjectNum - OTYPE_TUTO_MAX || nNumber >= g_ObjectManager.ObjectNum)
				{
					continue;
				}
				break;
			}

			//オブジェクト設置
			SetObject((OTYPE)nNumber, pos, rot, scale);
		}
		fclose(pFile);//ファイルを閉じる
	}
	else
	{//開けなかった
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "ロードエラー", "Object", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//---------------------------------------
//ロード処理
//---------------------------------------
void SaveObject(void)
{
	FILE* pFile;//ファイルポインタを宣言
	MODE Mode = GetMode();
	D3DXVECTOR3 rot;
	int nCount = 0;

	//Xファイル
	switch (Mode)
	{
	case MODE_GAME:
		pFile = fopen(OBJECT_FILE, "w");//ファイルを開く
		break;
	case MODE_TUTO:
		pFile = fopen(OBJECT_TUTO_FILE, "w");//ファイルを開く
		break;
	default:
		pFile = fopen(nullptr, "w");//ファイルを開く
		break;
	}

	if (pFile != NULL)
	{//開けたら
		for (nCount = 0; nCount < OBJECT_MAX; nCount++)
		{//最大数回す
			if (g_aObject[nCount].bUse)
			{//使用している
				//角度変換
				rot = D3DXToDegree(g_aObject[nCount].rot);

				fprintf(pFile, "%d\n", (int)g_aObject[nCount].Type);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aObject[nCount].pos.x);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aObject[nCount].pos.y);//数値を書き入れ
				fprintf(pFile, "%.1f\n", g_aObject[nCount].pos.z);//数値を書き入れ
				fprintf(pFile, "%.1f ", rot.x);//数値を書き入れ
				fprintf(pFile, "%.1f ", rot.y);//数値を書き入れ
				fprintf(pFile, "%.1f\n", rot.z);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aObject[nCount].scale.x);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aObject[nCount].scale.y);//数値を書き入れ
				fprintf(pFile, "%.1f\n", g_aObject[nCount].scale.z);//数値を書き入れ
			}
		}
		fclose(pFile);//ファイルを閉じる
	}
	else
	{//開けなかった
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "ロードエラー", "ロードできなかったよ", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//--------------------------
//オブジェクトエディター
//--------------------------
void UpdateObjectEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot)
{
	Camera* pCamera = GetCamera();
	static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	static int HoldNumber = -1 , EndNumber = -1;
	MODE Mode = GetMode();

	if (GetKeyboardTrigger(DIK_RIGHT))
	{// オブジェクト切替
		g_Type = (OTYPE)(g_Type + 1);
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{// オブジェクト切替
		g_Type = (OTYPE)(g_Type - 1);
	}

	switch (Mode)
	{
	case MODE_GAME:
		//範囲制限
		g_Type = (OTYPE)(g_Type % (g_ObjectManager.ObjectNum - OTYPE_TUTO_MAX));

		if (g_Type < 0)
		{//ループ
			g_Type = (OTYPE)(g_ObjectManager.ObjectNum - 1 - OTYPE_TUTO_MAX);
		}
		break;
	case MODE_TUTO:
		//範囲制限
		if (g_Type > g_ObjectManager.ObjectNum - 1)
		{//ループ
			g_Type = (OTYPE)(g_ObjectManager.ObjectNum - OTYPE_TUTO_MAX);
		}

		if (g_Type < g_ObjectManager.ObjectNum - OTYPE_TUTO_MAX)
		{//ループ
			g_Type = (OTYPE)(g_ObjectManager.ObjectNum - 1);
		}
		break;
	}

	if (GetKeyboardTrigger(DIK_UP))
	{// DELオブジェクト切替
		g_DelObject++;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{// DELオブジェクト切替
		g_DelObject--;
	}

	for (int nCount = 0; nCount < OBJECT_MAX; nCount++)
	{//最大数回す

		//範囲制限
		if (g_DelObject < 0)
		{
			g_DelObject = OBJECT_MAX - 1;
		}
		g_DelObject %= OBJECT_MAX;

		if (g_aObject[g_DelObject].bUse)
		{//使っているオブジェクトの選択
			break;
		}
		else
		{//使っていない番号はスキップ

			if (GetKeyboardTrigger(DIK_DOWN))
			{// DELオブジェクト切替
				g_DelObject--;
			}
			else
			{// DELオブジェクト切替
				g_DelObject++;
			}
		}

		if (nCount == OBJECT_MAX - 1)
		{//オブジェクトがない
			g_DelObject = -1;
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
		rot.x -= 0.01f;
	}
	if (GetKeyboardPress(DIK_I) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// 押している間
		rot.x += 0.01f;
	}
	if (GetKeyboardPress(DIK_J) || GetJoykeyPress(JOYKEY_LT, CONTROLLER_1))
	{// 押している間
		rot.y -= 0.01f;
	}
	if (GetKeyboardPress(DIK_K) || GetJoykeyPress(JOYKEY_RT, CONTROLLER_1))
	{// 押している間
		rot.y += 0.01f;
	}
	if (GetKeyboardPress(DIK_M) || GetJoykeyPress(JOYKEY_X, CONTROLLER_1))
	{// 押している間
		rot.z -= 0.01f;
	}
	if (GetKeyboardPress(DIK_COMMA) || GetJoykeyPress(JOYKEY_Y, CONTROLLER_1))
	{// 押している間
		rot.z += 0.01f;
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
	scale.x = max(OBJECT_SCALE_MIN, min(OBJECT_SCALE_MAX, scale.x));
	scale.y = max(OBJECT_SCALE_MIN, min(OBJECT_SCALE_MAX, scale.y));
	scale.z = max(OBJECT_SCALE_MIN, min(OBJECT_SCALE_MAX, scale.z));

	if (GetKeyboardTrigger(DIK_RSHIFT) || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1))
	{// 押したとき
		HoldNumber = SetObject(g_Type, Epos + pos, Erot + rot, scale);
		EndNumber = HoldNumber;
	}
	if (GetKeyboardPress(DIK_RSHIFT) || GetJoykeyPress(JOYKEY_RB, CONTROLLER_1))
	{// 押している間
		g_aObject[HoldNumber].pos = Epos + pos;
		g_aObject[HoldNumber].rot = Erot + rot;
		g_aObject[HoldNumber].scale = scale;

		static OTYPE Type = OTYPE_ROKET; //一時保存用ナンバー

		if (Type != g_Type)
		{//ナンバーが変化した
			//オブジェクトの消去
			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				if (g_aObject[HoldNumber].nIdxShadow[nCnt] != -1)
				{
					NullShadow(g_aObject[HoldNumber].nIdxShadow[nCnt]);
				}
			}
			g_aObject[HoldNumber].bUse = false;
			//オブジェクトの切り替え
			HoldNumber = SetObject(g_Type, Epos + pos, Erot + rot, scale);
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
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			if (g_aObject[EndNumber].nIdxShadow[nCnt] != -1)
			{
				NullShadow(g_aObject[EndNumber].nIdxShadow[nCnt]);
			}
		}
		g_aObject[EndNumber].bUse = false;
		EndNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_F8))
	{// セーブ
		SaveObject();
	}

	if (GetKeyboardTrigger(DIK_DELETE) || (GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1) && GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1)))
	{// 押したとき
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			if (g_aObject[g_DelObject].nIdxShadow[nCnt] != -1)
			{
				NullShadow(g_aObject[g_DelObject].nIdxShadow[nCnt]);
			}
		}
		g_aObject[g_DelObject].bUse = false;
	}

	static int DelNumber = -1; //一時保存用ナンバー

	if (DelNumber != g_DelObject)
	{//デリートナンバーが変化した
		if (DelNumber != -1)
		{//前回のオブジェクトが存在する
			g_aObject[DelNumber].bAlpha = false;
		}
		DelNumber = g_DelObject; //選択オブジェクトの更新
		if (DelNumber != -1)
		{//オブジェクトが存在する
			//マテリアルデータへのポインタを取得
			g_aObject[DelNumber].bAlpha = true;
		}
	}
}

//--------------------
//現在のXファイルナンバー
//--------------------
int GetOnumber(void)
{
	return (int)g_Type;
}

//--------------------
//現在のデリートナンバー
//--------------------
int GetODelnumber(void)
{
	return g_DelObject;
}