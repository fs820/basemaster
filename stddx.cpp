//---------------------------------------
//
//DirectX処理[stddx.cpp]
//Author fuma sato
//
//---------------------------------------
#include "stddx.h"
#include <bit>
#include "camera.h"
using std::bit_cast;

// レンダーステートのテンプレートの定義
constexpr RSOP RSOP_TEMP[DRAW_TEMP_MAX] =
{
	// DRAW_NONE
	{
		false,              // Lighting
		D3DCULL_NONE,       // cullMode
		false,              // alphaBlendEnable
		D3DBLENDOP_ADD,     // blendOp
		D3DBLEND_ZERO,      // srcBlend
		D3DBLEND_ZERO,      // destBlend
		false,              // zEnable
		false,              // zWriteEnable
		D3DCMP_NEVER,       // zFunc
		0.0f,               // zBias
		false,              // alphaTestEnable
		0,                  // alphaRef
		D3DCMP_NEVER,       // alphaFunc
		false,              // antialiaseLineEnable
		false               // stencilEnable
	},

	// DRAW_NORMAL
	{
		true,               // Lighting
		D3DCULL_CCW,        // cullMode
		false,              // alphaBlendEnable
		D3DBLENDOP_ADD,     // blendOp
		D3DBLEND_ONE,       // srcBlend
		D3DBLEND_ZERO,      // destBlend
		true,               // zEnable
		true,               // zWriteEnable
		D3DCMP_LESSEQUAL,   // zFunc
		0.0f,               // zBias
		false,              // alphaTestEnable
		0,                  // alphaRef
		D3DCMP_ALWAYS,      // alphaFunc
		false,              // antialiaseLineEnable
		false               // stencilEnable
	},

	// DRAW_BILLBOARD (他のテンプレートも同様に定義)
	{
		false,              // Lighting
		D3DCULL_NONE,       // cullMode
		true,               // alphaBlendEnable
		D3DBLENDOP_ADD,     // blendOp
		D3DBLEND_SRCALPHA,  // srcBlend
		D3DBLEND_INVSRCALPHA, // destBlend
		true,               // zEnable
		true,               // zWriteEnable
		D3DCMP_LESSEQUAL,   // zFunc
		0.0f,               // zBias
		false,              // alphaTestEnable
		0,                  // alphaRef
		D3DCMP_ALWAYS,      // alphaFunc
		false,              // antialiaseLineEnable
		false               // stencilEnable
	},

	// 他のテンプレートの定義...
};

// サンプラーステートのテンプレートの定義
constexpr SSOP SSOP_TEMP[DRAW_TEMP_MAX] =
{
	// DRAW_NONE
	{
		D3DTEXF_POINT,      // minFilter
		D3DTEXF_POINT,      // magFilter
		D3DTEXF_NONE,       // mipFilter
		D3DTADDRESS_WRAP,   // addressU
		D3DTADDRESS_WRAP    // addressV
	},

	// DRAW_NORMAL
	{
		D3DTEXF_LINEAR,     // minFilter
		D3DTEXF_LINEAR,     // magFilter
		D3DTEXF_LINEAR,     // mipFilter
		D3DTADDRESS_WRAP,   // addressU
		D3DTADDRESS_WRAP    // addressV
	},

	// 他のテンプレートの定義...
};

// テクスチャステージステートのテンプレートの定義
constexpr TSSOP TSSOP_TEMP[DRAW_TEMP_MAX] =
{
	// DRAW_NONE
	{
		D3DTOP_DISABLE,     // colorOp
		D3DTOP_DISABLE,     // alphaOp
		D3DTA_TEXTURE,      // colorArg1
		D3DTA_CURRENT       // colorArg2
	},

	// DRAW_NORMAL
	{
		D3DTOP_MODULATE,    // colorOp
		D3DTOP_MODULATE,    // alphaOp
		D3DTA_TEXTURE,      // colorArg1
		D3DTA_CURRENT       // colorArg2
	},

	// 他のテンプレートの定義...
};

//----------------------------------------------------
//2Dの頂点バッファを生成する
//----------------------------------------------------
HRESULT CreateVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, UINT VertexSize, UINT VertexCount, DWORD Usage, D3DPOOL Pool)
{
	//頂点バッファの生成
	RETURN_HR(GetDevice()->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VertexSize * VertexCount,
		Usage,
		FVF_VERTEX_2D,
		Pool,
		ppVtxBuff,
		nullptr
	))

	return S_OK;
}

//----------------------------------------------------
//3Dの頂点バッファを生成する
//----------------------------------------------------
HRESULT CreateVertex3D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, UINT VertexSize, UINT VertexCount, DWORD Usage, D3DPOOL Pool)
{
	//頂点バッファの生成
	RETURN_HR(GetDevice()->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VertexSize * VertexCount,
		Usage,
		FVF_VERTEX_3D,
		Pool,
		ppVtxBuff,
		nullptr
	))

	return S_OK;
}

//----------------------------------------------------
//テクスチャバッファを生成する
//----------------------------------------------------
HRESULT CreateTexture(LPDIRECT3DTEXTURE9* ppTextureBuff, const String* TexPass, int TextureCount, D3DXIMAGE_INFO* pImageInfo, DWORD Filter, D3DPOOL Pool)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (pImageInfo == nullptr)
	{
		for (int nCntTexture = 0; nCntTexture < TextureCount; nCntTexture++)
		{
			D3DXIMAGE_INFO ImageInfo;
			//テクスチャ情報の取得
			RETURN_HR(D3DXGetImageInfoFromFile
			(
				TexPass[nCntTexture].aName,
				&ImageInfo
			))

			//テクスチャの読み込み
			RETURN_HR(D3DXCreateTextureFromFileEx
			(
				pDevice,                    // デバイスのポインタ
				TexPass[nCntTexture].aName, // テクスチャのパス
				ImageInfo.Width,            // テクスチャの幅
				ImageInfo.Height,           // テクスチャの高さ
				ImageInfo.MipLevels,        // ミップマップレベル
				0,                          // 0（動的テクスチャならD3DUSAGE_DYNAMIC）
				ImageInfo.Format,           // フォーマット
				Pool,                       // メモリプール
				Filter,                     // 画像の拡縮フィルタ
				Filter,                     // ミップマップのフィルタ
				0,                          // カラーキー（透明にしたい色）
				nullptr,                    // `D3DXIMAGE_INFO`（画像情報を取得する場合）
				nullptr,                    // パレットデータ（パレットなしならNULL）
				&ppTextureBuff[nCntTexture] // テクスチャのポインタ
			))
		}
	}
	else
	{
		for (int nCntTexture = 0; nCntTexture < TextureCount; nCntTexture++)
		{
			//テクスチャ情報の取得
			RETURN_HR(D3DXGetImageInfoFromFile
			(
				TexPass[nCntTexture].aName,
				&pImageInfo[nCntTexture]
			))

			//テクスチャの読み込み
			RETURN_HR(D3DXCreateTextureFromFileEx
			(
				pDevice,                             // デバイスのポインタ
				TexPass[nCntTexture].aName,          // テクスチャのパス
				pImageInfo[nCntTexture].Width,       // テクスチャの幅
				pImageInfo[nCntTexture].Height,      // テクスチャの高さ
				pImageInfo[nCntTexture].MipLevels,   // ミップマップレベル
				0,                                   // 0（動的テクスチャならD3DUSAGE_DYNAMIC）
				pImageInfo[nCntTexture].Format,      // フォーマット
				Pool,                                // メモリプール
				Filter,                              // 画像の拡縮フィルタ
				Filter,                              // ミップマップのフィルタ
				0,                                   // カラーキー（透明にしたい色）
				&pImageInfo[nCntTexture],            // `D3DXIMAGE_INFO`（画像情報を取得する場合）
				nullptr,                             // パレットデータ（パレットなしならNULL）
				&ppTextureBuff[nCntTexture]          // テクスチャのポインタ
			))
		}
	}
	return S_OK;
}

//----------------------------------------------------
//2Dの頂点バッファを設定する
//----------------------------------------------------
HRESULT SetVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, DWORD LockFlags, int OffsetIdx, int BuffMax, int yDivisions, int xDivisions, float Width, float Height, float rhw, D3DXCOLOR col, int TexCount, D3DXIMAGE_INFO* pImageInfo, bool PosSenter)
{
	HRESULT hr; //成功確認
	VERTEX_2D* pVtx;//頂点情報ポインタ
	int nCnt, nCnt2, nCnt3; //カウント変数

	//頂点バッファをロック
	hr = (*ppVtxBuff)->Lock(sizeof(VERTEX_2D) * (xDivisions + 1) * (yDivisions + 1) * OffsetIdx, sizeof(VERTEX_2D) * (xDivisions + 1) * (yDivisions + 1) * BuffMax, (void**)&pVtx, LockFlags);
	RETURN_HR(hr)

	for (nCnt = 0; nCnt < BuffMax; nCnt++)
	{//指定数ループ
		for (nCnt2 = 0; nCnt2 < yDivisions + 1; nCnt2++)
		{//1枚分のZ軸のループ
			for (nCnt3 = 0; nCnt3 < xDivisions + 1; nCnt3++)
			{//1枚分のX軸のループ
				//座標設定
				if (PosSenter)
				{
					pVtx[nCnt2 * (xDivisions + 1) + nCnt3].pos = D3DXVECTOR3(-Width * 0.5f + (Width / (float)xDivisions) * nCnt3, -Height * 0.5f + (Height / (float)yDivisions) * nCnt2, 0.0f);
				}
				else
				{
					pVtx[nCnt2 * (xDivisions + 1) + nCnt3].pos = D3DXVECTOR3(-Width * 0.5f + (Width / (float)xDivisions) * nCnt3, -Height + (Height / (float)yDivisions) * nCnt2, 0.0f);
				}

				//rhw
				pVtx[nCnt2 * (xDivisions + 1) + nCnt3].rhw = rhw;

				//カラー
				pVtx[nCnt2 * (xDivisions + 1) + nCnt3].col = col;

				//テクスチャ
				if (pImageInfo == nullptr)
				{
					for (int nCntTex = 0; nCntTex < TexCount; nCntTex++)
					{
						pVtx[nCnt2 * (xDivisions + 1) + nCnt3].tex[nCntTex] = D3DXVECTOR2((Width / (float)xDivisions) * nCnt3, (Height / (float)yDivisions) * nCnt2);
					}
				}
				else
				{
					for (int nCntTex = 0; nCntTex < TexCount; nCntTex++)
					{
						pVtx[nCnt2 * (xDivisions + 1) + nCnt3].tex[nCntTex] = D3DXVECTOR2(((Width / (float)pImageInfo[nCntTex].Width) / (float)xDivisions) * nCnt3, ((Height / (float)pImageInfo[nCntTex].Height) / (float)yDivisions) * nCnt2);
					}
				}
			}
		}
		pVtx += (xDivisions + 1) * (yDivisions + 1);
	}

	//頂点バッファをアンロック
	hr = (*ppVtxBuff)->Unlock();
	RETURN_HR(hr)

	return hr;
}

//----------------------------------------------------
//3Dの頂点バッファを設定する
//----------------------------------------------------
HRESULT SetVertex3D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, DWORD LockFlags, int OffsetIdx, int BuffMax, int yDivisions, int xDivisions, float Width, float Height, D3DXCOLOR col, int TexCount, D3DXIMAGE_INFO* pImageInfo, bool PosSenter)
{
	HRESULT hr; //成功確認
	int nCnt, nCnt2, nCnt3; //カウント変数

	VERTEX_3D* pVtx;//頂点情報ポインタ

	//頂点バッファをロック
	hr = (*ppVtxBuff)->Lock(sizeof(VERTEX_3D) * (xDivisions + 1) * (yDivisions + 1) * OffsetIdx, sizeof(VERTEX_3D) * (xDivisions + 1) * (yDivisions + 1) * BuffMax, (void**)&pVtx, LockFlags);
	RETURN_HR(hr)


	for (nCnt = 0; nCnt < BuffMax; nCnt++)
	{//指定数ループ
		for (nCnt2 = 0; nCnt2 < yDivisions + 1; nCnt2++)
		{//1枚分のZ軸のループ
			for (nCnt3 = 0; nCnt3 < xDivisions + 1; nCnt3++)
			{//1枚分のX軸のループ
				//座標設定
				if (PosSenter)
				{
					pVtx[nCnt2 * (xDivisions + 1) + nCnt3].pos = D3DXVECTOR3(-Width * 0.5f + (Width / (float)xDivisions) * nCnt3, Height * 0.5f - (Height / (float)yDivisions) * nCnt2, 0.0f);
				}
				else
				{
					pVtx[nCnt2 * (xDivisions + 1) + nCnt3].pos = D3DXVECTOR3(-Width * 0.5f + (Width / (float)xDivisions) * nCnt3, Height - (Height / (float)yDivisions) * nCnt2, 0.0f);
				}

				//nor
				pVtx[nCnt2 * (xDivisions + 1) + nCnt3].nor = NOR;

				//カラー
				pVtx[nCnt2 * (xDivisions + 1) + nCnt3].col = col;

				//テクスチャ
				if (pImageInfo == nullptr)
				{
					for (int nCntTex = 0; nCntTex < TexCount; nCntTex++)
					{
						pVtx[nCnt2 * (xDivisions + 1) + nCnt3].tex[nCntTex] = D3DXVECTOR2((Width / (float)xDivisions) * nCnt3, (Height / (float)yDivisions) * nCnt2);
					}
				}
				else
				{
					for (int nCntTex = 0; nCntTex < TexCount; nCntTex++)
					{
						pVtx[nCnt2 * (xDivisions + 1) + nCnt3].tex[nCntTex] = D3DXVECTOR2(((Width / (float)pImageInfo[nCntTex].Width) / (float)xDivisions) * nCnt3, ((Height / (float)pImageInfo[nCntTex].Height) / (float)yDivisions) * nCnt2);
					}
				}
			}
		}
		pVtx += (xDivisions + 1) * (yDivisions + 1);
	}

	//頂点バッファをアンロック
	hr = (*ppVtxBuff)->Unlock();
	RETURN_HR(hr)

	return hr;
}

//----------------------------------------------------
//インデックスバッファを設定する
//----------------------------------------------------
HRESULT SetIndex(LPDIRECT3DINDEXBUFFER9* ppIdxBuff, int yzDivisions, int xDivisions)
{
	HRESULT hr; //成功確認
	WORD* pIdx;
	int nCnt;

	hr = (*ppIdxBuff)->Lock(0, 0, (void**)&pIdx, 0);
	RETURN_HR(hr)


	for (nCnt = 0; nCnt < (((xDivisions + 1) * 2) * yzDivisions + (yzDivisions - 1) * 2) - (yzDivisions - 1) * 2; nCnt++)
	{//インデックス数回す
		if (nCnt % ((xDivisions + 1) * 2) == 0 && nCnt != 0)
		{
			//インデックス指定
			pIdx[nCnt - 2 + ((nCnt / ((xDivisions + 1) * 2)) * 2)] = (xDivisions + 1) - (xDivisions + 1) * ((nCnt - 1) % 2) + ((nCnt - 1) / 2);
			//インデックス指定
			pIdx[nCnt - 1 + ((nCnt / ((xDivisions + 1) * 2)) * 2)] = (xDivisions + 1) - (xDivisions + 1) * (nCnt % 2) + (nCnt / 2);
		}

		//インデックス指定
		pIdx[nCnt + ((nCnt / ((xDivisions + 1) * 2)) * 2)] = (xDivisions + 1) - (xDivisions + 1) * (nCnt % 2) + (nCnt / 2);
	}

	hr = (*ppIdxBuff)->Unlock();
	RETURN_HR(hr)

	return hr;
}

//----------------------------------------------------
//描画情報を取得する
//----------------------------------------------------
HRESULT GetDrawOperation(RSOP* rsOp, SSOP* ssOp, unsigned int SamplerCount, TSSOP* tssOp, unsigned int TextureStateCount)
{
	RETURN_HR(GetRenderOperation(rsOp))                           // レンダーステート情報
	RETURN_HR(GetSamplerOperation(ssOp, SamplerCount))            // サンプラーステート情報
	RETURN_HR(GetTextureStageOperation(tssOp, TextureStateCount)) // テクスチャステージステート情報

	return S_OK;
}

//----------------------------------------------------
//描画情報を変更する
//----------------------------------------------------
HRESULT SetDrawOperation(RSOP* rsOp, SSOP* ssOp, unsigned int SamplerCount, TSSOP* tssOp, unsigned int TextureStateCount)
{
	RETURN_HR(SetRenderOperation(rsOp))                           // レンダーステート情報
	RETURN_HR(SetSamplerOperation(ssOp, SamplerCount))            // サンプラーステート情報
	RETURN_HR(SetTextureStageOperation(tssOp, TextureStateCount)) // テクスチャステージステート情報

	return S_OK;
}

//----------------------------------------------------
//描画情報のテンプレートを取得する
//----------------------------------------------------
HRESULT GetDrawOperationTemp(RSOP* rsOp, SSOP* ssOp, unsigned int SamplerCount, TSSOP* tssOp, unsigned int TextureStateCount, DRAW_TEMP DrawTemp)
{
	// 指定されたテンプレートに基づいて描画情報を取得
	switch (DrawTemp)
	{
	case DRAW_NONE:
		break;
	}
}

//----------------------------------------------------
// 描画情報をテンプレートに変更する _STD_UNSECURE_USE
//----------------------------------------------------
HRESULT SetDrawOperationTemp(unsigned int SamplerCount, unsigned int TextureStateCount, DRAW_TEMP DrawTemp)
{

}

//----------------------------------------------------
//レンダーステート情報を取得する
//----------------------------------------------------
HRESULT GetRenderOperation(RSOP* rsOp)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DWORD Ram;

	// ライティング
	RETURN_HR(pDevice->GetRenderState(D3DRS_LIGHTING, &Ram))
	rsOp->Lighting = (Ram != 0);

	// カリングモード
	RETURN_HR(pDevice->GetRenderState(D3DRS_CULLMODE, &Ram))
	rsOp->cullMode = static_cast<D3DCULL>(Ram);

	// アルファブレンド
	RETURN_HR(pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &Ram))
	rsOp->alphaBlendEnable = (Ram != 0);

	// ブレンド演算
	RETURN_HR(pDevice->GetRenderState(D3DRS_BLENDOP, &Ram))
	rsOp->blendOp = static_cast<D3DBLENDOP>(Ram);

	// ソースブレンド
	RETURN_HR(pDevice->GetRenderState(D3DRS_SRCBLEND, &Ram))
	rsOp->srcBlend = static_cast<D3DBLEND>(Ram);

	// デスティネーションブレンド
	RETURN_HR(pDevice->GetRenderState(D3DRS_DESTBLEND, &Ram))
	rsOp->destBlend = static_cast<D3DBLEND>(Ram);

	// Zバッファ
	RETURN_HR(pDevice->GetRenderState(D3DRS_ZENABLE, &Ram))
	rsOp->zEnable = (Ram != 0);

	// Zバッファ書き込み
	RETURN_HR(pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &Ram))
	rsOp->zWriteEnable = (Ram != 0);

	// Zバッファ比較関数
	RETURN_HR(pDevice->GetRenderState(D3DRS_ZFUNC, &Ram))
	rsOp->zFunc = static_cast<D3DCMPFUNC>(Ram);

	// Zバイアス
	RETURN_HR(pDevice->GetRenderState(D3DRS_DEPTHBIAS, &Ram))
	rsOp->zBias = bit_cast<float>(Ram); //float bit変換


	// アルファテスト
	RETURN_HR(pDevice->GetRenderState(D3DRS_ALPHATESTENABLE, &Ram))
	rsOp->alphaTestEnable = (Ram != 0);

	// アルファテスト参照値
	RETURN_HR(pDevice->GetRenderState(D3DRS_ALPHAREF, &Ram))
	rsOp->alphaRef = Ram;

	// アルファテスト比較関数
	RETURN_HR(pDevice->GetRenderState(D3DRS_ALPHAFUNC, &Ram))
	rsOp->alphaFunc = static_cast<D3DCMPFUNC>(Ram);

	// アンチエイリアス
	RETURN_HR(pDevice->GetRenderState(D3DRS_ANTIALIASEDLINEENABLE, &Ram))
	rsOp->antialiaseLineEnable = (Ram != 0);

	// ステンシルテスト
	RETURN_HR(pDevice->GetRenderState(D3DRS_STENCILENABLE, &Ram))
	rsOp->stencilEnable = (Ram != 0);

	return S_OK;
}

//----------------------------------------------------
//レンダーステート情報を取得する
//----------------------------------------------------
RSOP GetRenderOperation(void)
{
	RSOP rsOp;
	CHECK_HR(GetRenderOperation(&rsOp)) // レンダーステート情報を取得
	return rsOp;
}

//----------------------------------------------------
//レンダーステート情報を変更する
//----------------------------------------------------
HRESULT SetRenderOperation(RSOP* rsOp)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	static RSOP rsOpOld = GetRenderOperation();

	if (rsOp->Lighting != rsOpOld.Lighting)
	{
		// ライティング
		RETURN_HR(pDevice->SetRenderState(D3DRS_LIGHTING, rsOp->Lighting))
	}

	if (rsOp->cullMode != rsOpOld.cullMode)
	{
		// カリングモード
		RETURN_HR(pDevice->SetRenderState(D3DRS_CULLMODE, rsOp->cullMode))
	}

	if (rsOp->alphaBlendEnable != rsOpOld.alphaBlendEnable)
	{
		// アルファブレンド
		RETURN_HR(pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, rsOp->alphaBlendEnable))
	}

	if (rsOp->blendOp != rsOpOld.blendOp)
	{
		// ブレンド演算
		RETURN_HR(pDevice->SetRenderState(D3DRS_BLENDOP, rsOp->blendOp))
	}

	if (rsOp->srcBlend != rsOpOld.srcBlend)
	{
		// ソースブレンド
		RETURN_HR(pDevice->SetRenderState(D3DRS_SRCBLEND, rsOp->srcBlend))
	}

	if (rsOp->destBlend != rsOpOld.destBlend)
	{
		// デスティネーションブレンド
		RETURN_HR(pDevice->SetRenderState(D3DRS_DESTBLEND, rsOp->destBlend))
	}

	if (rsOp->zEnable != rsOpOld.zEnable)
	{
		// Zバッファ
		RETURN_HR(pDevice->SetRenderState(D3DRS_ZENABLE, rsOp->zEnable))
	}

	if (rsOp->zWriteEnable != rsOpOld.zWriteEnable)
	{
		// Zバッファ書き込み
		RETURN_HR(pDevice->SetRenderState(D3DRS_ZWRITEENABLE, rsOp->zWriteEnable))
	}

	if (rsOp->zFunc != rsOpOld.zFunc)
	{
		// Zバッファ比較関数
		RETURN_HR(pDevice->SetRenderState(D3DRS_ZFUNC, rsOp->zFunc))
	}

	if (fabsf(rsOp->zBias - rsOpOld.zBias) > 0.0001f)
	{
		// Zバイアス
		RETURN_HR(pDevice->SetRenderState(D3DRS_DEPTHBIAS, bit_cast<DWORD>(rsOp->zBias))) //DWORD bit変換
	}

	if (rsOp->alphaTestEnable != rsOpOld.alphaTestEnable)
	{
		// アルファテスト
		RETURN_HR(pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, rsOp->alphaTestEnable))
	}

	if (rsOp->alphaRef != rsOpOld.alphaRef)
	{
		// アルファテスト参照値
		RETURN_HR(pDevice->SetRenderState(D3DRS_ALPHAREF, rsOp->alphaRef))
	}

	if (rsOp->alphaFunc != rsOpOld.alphaFunc)
	{
		// アルファテスト比較関数
		RETURN_HR(pDevice->SetRenderState(D3DRS_ALPHAFUNC, rsOp->alphaFunc))
	}

	if (rsOp->antialiaseLineEnable != rsOpOld.antialiaseLineEnable)
	{
		// アンチエイリアス
		RETURN_HR(pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, rsOp->antialiaseLineEnable))
	}

	if (rsOp->stencilEnable != rsOpOld.stencilEnable)
	{
		// ステンシルテスト
		RETURN_HR(pDevice->SetRenderState(D3DRS_STENCILENABLE, rsOp->stencilEnable))
	}

	rsOpOld = *rsOp; // 現在の設定を保存する
	return S_OK;
}

//----------------------------------------------------
//サンプラーステート情報を取得する
//----------------------------------------------------
HRESULT GetSamplerOperation(SSOP* ssOp, unsigned int SamplerCount)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DWORD Ram;
	SamplerCount = static_cast<unsigned int>(min(static_cast<int>(SamplerCount), TEXTURE_MAX)); // 最大数を超えないようにする

	for (unsigned int nCnt = 0; nCnt < SamplerCount; nCnt++)
	{
		// 異方性フィルタの最大値
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_MAXANISOTROPY, &Ram))
		ssOp[nCnt].maxAnisotropy = static_cast<DWORD>(Ram);

		// テクスチャミニフィケーションフィルタ
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_MINFILTER, &Ram))
		ssOp[nCnt].minFilter = static_cast<D3DTEXTUREFILTERTYPE>(Ram);

		// テクスチャマグニフィケーションフィルタ
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_MAGFILTER, &Ram))
		ssOp[nCnt].magFilter = static_cast<D3DTEXTUREFILTERTYPE>(Ram);

		// ミップマップレベル
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_MAXMIPLEVEL, &Ram))
		ssOp[nCnt].maxMipLevel = Ram;

		// ミップマップLODバイアス
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_MIPMAPLODBIAS, &Ram))

		//ssOp[nCnt].mipMapLODBias = *reinterpret_cast<float*>(&Ram); // DWORDをfloatに変換
		//memcpy_s(&ssOp[nCnt].mipMapLODBias, sizeof(float), &Ram, sizeof(DWORD)); // memcpy_sを使用して安全にコピーする

		TYPE_CONVERSION TypeConversion;                  // 型変換用の共用体
		TypeConversion.Dword = Ram;                      // DWORDを共用体に格納
		ssOp[nCnt].mipMapLODBias = TypeConversion.Float; // 共用体からfloatを取得する

		ssOp[nCnt].mipMapLODBias = bit_cast<float>(Ram); // float bit変換

		// ミップマップフィルタ
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_MIPFILTER, &Ram))
		ssOp[nCnt].mipFilter = static_cast<D3DTEXTUREFILTERTYPE>(Ram);

		// U方向のアドレスモード
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_ADDRESSU, &Ram))
		ssOp[nCnt].addressU = static_cast<D3DTEXTUREADDRESS>(Ram);

		// V方向のアドレスモード
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_ADDRESSV, &Ram))
		ssOp[nCnt].addressV = static_cast<D3DTEXTUREADDRESS>(Ram);

		// W方向のアドレスモード (3Dテクスチャ用)
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_ADDRESSW, &Ram))
		ssOp[nCnt].addressW = static_cast<D3DTEXTUREADDRESS>(Ram);

		// ボーダーカラー (Clamp時の境界色)
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_BORDERCOLOR, &Ram))
		ssOp[nCnt].borderColor = static_cast<D3DCOLOR>(Ram);
	}

	return S_OK;
}

//----------------------------------------------------
//レンダーステート情報を取得する
//----------------------------------------------------
SSOP* GetSamplerOperation(unsigned int SamplerCount)
{
	SSOP ssOp[TEXTURE_MAX];
	CHECK_HR(GetSamplerOperation(&ssOp[0], SamplerCount)) // サンプラーステート情報を取得
	return &ssOp[0];
}

//----------------------------------------------------
//サンプラーステート情報を変更する
//----------------------------------------------------
HRESULT SetSamplerOperation(SSOP* ssOp, unsigned int SamplerCount)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	static SSOP ssOpOld[TEXTURE_MAX]; // サンプラーステート情報を取得
	static bool ssOpOldFlag = false; // 初期化フラグ
	if (!ssOpOldFlag)
	{
		// 初回のみ、サンプラーステート情報を取得
		CHECK_HR(GetSamplerOperation(ssOpOld, TEXTURE_MAX))
		ssOpOldFlag = true; // フラグを立てる
	}
	SamplerCount = static_cast<unsigned int>(min(static_cast<int>(SamplerCount), TEXTURE_MAX)); // 最大数を超えないようにする

	for (unsigned int nCnt = 0; nCnt < SamplerCount; nCnt++)
	{
		if (ssOp[nCnt].maxAnisotropy != ssOpOld[nCnt].maxAnisotropy)
		{
			// 異方性フィルタの最大値
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_MAXANISOTROPY, ssOp[nCnt].maxAnisotropy))
		}

		if (ssOp[nCnt].minFilter != ssOpOld[nCnt].minFilter)
		{
			// テクスチャミニフィケーションフィルタ
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_MINFILTER, ssOp[nCnt].minFilter))
		}

		if (ssOp[nCnt].magFilter != ssOpOld[nCnt].magFilter)
		{
			// テクスチャマグニフィケーションフィルタ
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_MAGFILTER, ssOp[nCnt].magFilter))
		}

		if (ssOp[nCnt].maxMipLevel != ssOpOld[nCnt].maxMipLevel)
		{
			// ミップマップレベル
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_MAXMIPLEVEL, ssOp[nCnt].maxMipLevel))
		}

		if (fabsf(ssOp[nCnt].mipMapLODBias - ssOpOld[nCnt].mipMapLODBias) > 0.0001f)
		{
			// ミップマップLODバイアス
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_MIPMAPLODBIAS, bit_cast<DWORD>(ssOp[nCnt].mipMapLODBias))) // DWORD bit変換
		}

		if (ssOp[nCnt].mipFilter != ssOpOld[nCnt].mipFilter)
		{
			// ミップマップフィルタ
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_MIPFILTER, ssOp[nCnt].mipFilter))
		}

		if (ssOp[nCnt].addressU != ssOpOld[nCnt].addressU)
		{
			// U方向のアドレスモード
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_ADDRESSU, ssOp[nCnt].addressU))
		}

		if (ssOp[nCnt].addressV != ssOpOld[nCnt].addressV)
		{
			// V方向のアドレスモード
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_ADDRESSV, ssOp[nCnt].addressV))
		}

		if (ssOp[nCnt].addressW != ssOpOld[nCnt].addressW)
		{
			// W方向のアドレスモード (3Dテクスチャ用)
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_ADDRESSW, ssOp[nCnt].addressW))
		}

		if (ssOp[nCnt].borderColor != ssOpOld[nCnt].borderColor)
		{
			// ボーダーカラー (Clamp時の境界色)
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_BORDERCOLOR, ssOp[nCnt].borderColor))
		}

		ssOpOld[nCnt] = ssOp[nCnt]; // 現在の設定を保存する
	}

	return S_OK;
}

//----------------------------------------------------
//テクスチャステージステート情報を取得する
//----------------------------------------------------
HRESULT GetTextureStageOperation(TSSOP* tssOp, unsigned int TextureStateCount)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DWORD Ram;
	TextureStateCount = static_cast<unsigned int>(min(static_cast<int>(TextureStateCount), TEXTURE_MAX)); // 最大数を超えないようにする

	for (unsigned int nCnt = 0; nCnt < TextureStateCount; nCnt++)
	{
		// カラーオペレーション
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_COLOROP, &Ram))
		tssOp[nCnt].colorOp = static_cast<D3DTEXTUREOP>(Ram);

		// オペレーションの引数0 (一部のオペレーション用)
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_COLORARG0, &Ram))
		tssOp[nCnt].colorArg0 = static_cast<DWORD>(Ram);

		// オペレーションの引数1
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_COLORARG1, &Ram))
		tssOp[nCnt].colorArg1 = static_cast<DWORD>(Ram);

		// オペレーションの引数2
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_COLORARG2, &Ram))
		tssOp[nCnt].colorArg2 = static_cast<DWORD>(Ram);

		// アルファオペレーション
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_ALPHAOP, &Ram))
		tssOp[nCnt].alphaOp = static_cast<D3DTEXTUREOP>(Ram);

		// オペレーションの引数0 (一部のオペレーション用)
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_ALPHAARG0, &Ram))
		tssOp[nCnt].alphaArg0 = static_cast<DWORD>(Ram);

		// オペレーションの引数1
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_ALPHAARG1, &Ram))
		tssOp[nCnt].alphaArg1 = static_cast<DWORD>(Ram);

		// オペレーションの引数2
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_ALPHAARG2, &Ram))
		tssOp[nCnt].alphaArg2 = static_cast<DWORD>(Ram);

		// テクスチャのインデックス
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_TEXCOORDINDEX, &Ram))
		tssOp[nCnt].texCoordIndex = static_cast<DWORD>(Ram);

		// テクスチャ変換
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_TEXTURETRANSFORMFLAGS, &Ram))
		tssOp[nCnt].texTureTransFormFlags = static_cast<D3DTEXTURETRANSFORMFLAGS>(Ram);

		// 結果の格納先
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_RESULTARG, &Ram))
		tssOp[nCnt].resultArg = static_cast<DWORD>(Ram);

		// 定数値 (D3DTA_CONSTANTを使用時)
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_CONSTANT, &Ram))
		tssOp[nCnt].constant = static_cast<D3DCOLOR>(Ram);
	}

	return S_OK;
}

//----------------------------------------------------
//レンダーステート情報を取得する
//----------------------------------------------------
TSSOP* GetTextureStageOperation(unsigned int TextureStateCount)
{
	TSSOP tssOp[TEXTURE_MAX];
	CHECK_HR(GetTextureStageOperation(&tssOp[0], TextureStateCount)) // テクスチャステージステート情報を取得
	return &tssOp[0];
}

//----------------------------------------------------
//テクスチャステージステート情報を変更する
//----------------------------------------------------
HRESULT SetTextureStageOperation(TSSOP* tssOp, unsigned int TextureStateCount)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	static TSSOP tssOpOld[TEXTURE_MAX];
	static bool tssOpOldFlag = false; // 初期化フラグ
	if (!tssOpOldFlag)
	{
		// 初回のみ、テクスチャステージステート情報を取得
		CHECK_HR(GetTextureStageOperation(tssOpOld, TEXTURE_MAX))
		tssOpOldFlag = true; // フラグを立てる
	}
	TextureStateCount = static_cast<unsigned int>(min(static_cast<int>(TextureStateCount), TEXTURE_MAX)); // 最大数を超えないようにする

	for (unsigned int nCnt = 0; nCnt < TextureStateCount; nCnt++)
	{
		if (tssOp[nCnt].colorOp != tssOpOld[nCnt].colorOp)
		{
			// カラーオペレーション
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_COLOROP, tssOp[nCnt].colorOp))
		}

		if (tssOp[nCnt].colorArg0 != tssOpOld[nCnt].colorArg0)
		{
			// オペレーションの引数0 (一部のオペレーション用)
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_COLORARG0, tssOp[nCnt].colorArg0))
		}

		if (tssOp[nCnt].colorArg1 != tssOpOld[nCnt].colorArg1)
		{
			// オペレーションの引数1
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_COLORARG1, tssOp[nCnt].colorArg1))
		}

		if (tssOp[nCnt].colorArg2 != tssOpOld[nCnt].colorArg2)
		{
			// オペレーションの引数2
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_COLORARG2, tssOp[nCnt].colorArg2))
		}

		if (tssOp[nCnt].alphaOp != tssOpOld[nCnt].alphaOp)
		{
			// アルファオペレーション
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_ALPHAOP, tssOp[nCnt].alphaOp))
		}

		if (tssOp[nCnt].alphaArg0 != tssOpOld[nCnt].alphaArg0)
		{
			// オペレーションの引数0 (一部のオペレーション用)
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_ALPHAARG0, tssOp[nCnt].alphaArg0))
		}

		if (tssOp[nCnt].alphaArg1 != tssOpOld[nCnt].alphaArg1)
		{
			// オペレーションの引数1
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_ALPHAARG1, tssOp[nCnt].alphaArg1))
		}

		if (tssOp[nCnt].alphaArg2 != tssOpOld[nCnt].alphaArg2)
		{
			// オペレーションの引数2
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_ALPHAARG2, tssOp[nCnt].alphaArg2))
		}

		if (tssOp[nCnt].texCoordIndex != tssOpOld[nCnt].texCoordIndex)
		{
			// テクスチャのインデックス
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_TEXCOORDINDEX, tssOp[nCnt].texCoordIndex))
		}

		if (tssOp[nCnt].texTureTransFormFlags != tssOpOld[nCnt].texTureTransFormFlags)
		{
			// テクスチャ変換
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_TEXTURETRANSFORMFLAGS, tssOp[nCnt].texTureTransFormFlags))
		}

		if (tssOp[nCnt].resultArg != tssOpOld[nCnt].resultArg)
		{
			// 結果の格納先
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_RESULTARG, tssOp[nCnt].resultArg))
		}

		if (tssOp[nCnt].constant != tssOpOld[nCnt].constant)
		{
			// 定数値 (D3DTA_CONSTANTを使用時)
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_CONSTANT, tssOp[nCnt].constant))
		}

		tssOpOld[nCnt] = tssOp[nCnt]; // 現在の設定を保存する
	}

	return S_OK;
}

//----------------------------------------------------
//2Dのバッファを描画する
//----------------------------------------------------
HRESULT DrawVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, LPDIRECT3DINDEXBUFFER9* ppIdxBuff, LPDIRECT3DTEXTURE9* ppTextureBuff, D3DPRIMITIVETYPE PrimitiveType, UINT VertexSize, UINT OffsetIdx, UINT PrimitiveCount)
{
	if (ppVtxBuff == nullptr || *ppVtxBuff == nullptr)
	{
		return E_POINTER;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファ
	RETURN_HR(pDevice->SetStreamSource(0, ppVtxBuff[0], 0, sizeof(VERTEX_2D)))

	if (ppIdxBuff != nullptr)
	{
		//インデックスバッファをデータストリームに設定
		RETURN_HR(pDevice->SetIndices(ppIdxBuff[0]))
	}

	//頂点フォーマットの設定
	RETURN_HR(pDevice->SetFVF(FVF_VERTEX_2D))

	if (ppTextureBuff != nullptr)
	{
		//テクスチャの設定
		RETURN_HR(pDevice->SetTexture(0, ppTextureBuff[0]))
	}

	if (ppIdxBuff != nullptr)
	{
		//ポリゴンの描画
		RETURN_HR(pDevice->DrawIndexedPrimitive
		(
			PrimitiveType,//タイプ
			VertexSize * OffsetIdx,
			0,
			VertexSize,//頂点数
			0,
			PrimitiveCount//ポリゴンの個数
		))
	}
	else
	{
		//ポリゴンの描画
		RETURN_HR(pDevice->DrawPrimitive
		(
			PrimitiveType,//タイプ
			VertexSize * OffsetIdx,//始まりの番号
			PrimitiveCount//ポリゴンの個数
		))
	}

	return S_OK;
}

//----------------------------------------------------
//3Dのバッファを描画する
//----------------------------------------------------
HRESULT DrawVertex3D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, LPDIRECT3DINDEXBUFFER9* ppIdxBuff, LPDIRECT3DTEXTURE9* ppTextureBuff, D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, D3DXVECTOR3* pScale, D3DXMATRIX* pMtxWorld, BILLBOARD_TYPE BillboardType, float zBias, D3DPRIMITIVETYPE PrimitiveType, UINT VertexSize, UINT OffsetIdx, UINT PrimitiveCount)
{
	if (ppVtxBuff == nullptr || *ppVtxBuff == nullptr)
	{
		return E_POINTER;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;//計算マトリックス

	if (zBias >= 0.00001f)
	{// Zバイアス設定
		RSOP rsOp;
		GetRenderOperation(&rsOp);
		rsOp.zBias += zBias; // レンダーステート情報を元に戻すまで足し続ける
		SetRenderOperation(&rsOp);
	}

	if (pMtxWorld != nullptr)
	{
		//マトリックス初期化
		D3DXMatrixIdentity(pMtxWorld);

		//ビューマトリックス取得
		D3DXMATRIX mtxView;
		switch (BillboardType)
		{
		case BILLBOARD_NONE:
			break;
		case BILLBOARD_FULL:
			RETURN_HR(pDevice->GetTransform(D3DTS_VIEW, &mtxView))
			//カメラの逆行列を設定
			pMtxWorld->_11 = mtxView._11;
			pMtxWorld->_12 = mtxView._21;
			pMtxWorld->_13 = mtxView._31;
			pMtxWorld->_21 = mtxView._12;
			pMtxWorld->_22 = mtxView._22;
			pMtxWorld->_23 = mtxView._32;
			pMtxWorld->_31 = mtxView._13;
			pMtxWorld->_32 = mtxView._23;
			pMtxWorld->_33 = mtxView._33;
			break;
		case BILLBOARD_Y_AXIS:
			RETURN_HR(pDevice->GetTransform(D3DTS_VIEW, &mtxView))
			//カメラの逆行列を設定
			D3DXVECTOR3 zAxis = D3DXVECTOR3(mtxView._31, 0.0f, mtxView._33); // Y成分をゼロに
			D3DXVec3Normalize(&zAxis, &zAxis);

			D3DXVECTOR3 yAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // 固定

			D3DXVECTOR3 xAxis;
			D3DXVec3Cross(&xAxis, &yAxis, &zAxis);
			D3DXVec3Normalize(&xAxis, &xAxis);

			pMtxWorld->_11 = xAxis.x;
			pMtxWorld->_12 = xAxis.y;
			pMtxWorld->_13 = xAxis.z;

			pMtxWorld->_21 = yAxis.x;
			pMtxWorld->_22 = yAxis.y;
			pMtxWorld->_23 = yAxis.z;

			pMtxWorld->_31 = zAxis.x;
			pMtxWorld->_32 = zAxis.y;
			pMtxWorld->_33 = zAxis.z;
			break;
		case BILLBOARD_XZ_AXIS:
			RETURN_HR(pDevice->GetTransform(D3DTS_VIEW, &mtxView))
			//カメラの逆行列を設定
			D3DXVECTOR3 xAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f); // X軸を固定
			D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, 0.0f, 1.0f); // Z軸を固定

			D3DXVECTOR3 yAxis;
			D3DXVec3Cross(&yAxis, &zAxis, &xAxis);
			D3DXVec3Normalize(&yAxis, &yAxis);

			pMtxWorld->_11 = xAxis.x;
			pMtxWorld->_12 = xAxis.y;
			pMtxWorld->_13 = xAxis.z;

			pMtxWorld->_21 = yAxis.x;
			pMtxWorld->_22 = yAxis.y;
			pMtxWorld->_23 = yAxis.z;

			pMtxWorld->_31 = zAxis.x;
			pMtxWorld->_32 = zAxis.y;
			pMtxWorld->_33 = zAxis.z;
			break;
		}

		if (pScale != nullptr)
		{
			//大きさの反映
			D3DXMatrixScaling(&mtxScale, pScale->x, pScale->y, pScale->z);
			D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxScale);
		}

		if (pRot != nullptr)
		{
			//向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pRot->y, pRot->x, pRot->z);
			D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxRot);
		}

		if (pPos != nullptr)
		{
			//位置の計算
			D3DXMatrixTranslation(&mtxTrans, pPos->x, pPos->y, pPos->z);
			D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxTrans);
		}

		//ワールドマトリックスの設定
		RETURN_HR(pDevice->SetTransform(D3DTS_WORLD, pMtxWorld))
	}

	//頂点バッファ
	RETURN_HR(pDevice->SetStreamSource(0, ppVtxBuff[0], 0, sizeof(VERTEX_3D)))

	if (ppIdxBuff != nullptr)
	{
		//インデックスバッファをデータストリームに設定
		RETURN_HR(pDevice->SetIndices(ppIdxBuff[0]))
	}

	//頂点フォーマットの設定
	RETURN_HR(pDevice->SetFVF(FVF_VERTEX_3D))

	if (ppTextureBuff != nullptr)
	{
		//テクスチャの設定
		RETURN_HR(pDevice->SetTexture(0, ppTextureBuff[0]))
	}

	if (ppIdxBuff != nullptr)
	{
		//ポリゴンの描画
		RETURN_HR(pDevice->DrawIndexedPrimitive
		(
			PrimitiveType,//タイプ
			VertexSize * OffsetIdx,
			0,
			VertexSize,//頂点数
			0,
			PrimitiveCount//ポリゴンの個数
		))
	}
	else
	{
		//ポリゴンの描画
		RETURN_HR(pDevice->DrawPrimitive
		(
			PrimitiveType,//タイプ
			VertexSize * OffsetIdx,//始まりの番号
			PrimitiveCount//ポリゴンの個数
		))
	}

	return S_OK;
}