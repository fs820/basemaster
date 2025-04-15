//---------------------------------------
//
//アタックゾーン処理[AttackZone.cpp]
//Author fuma sato
//
//---------------------------------------
#include "AttackZone.h"
#include "camera.h"
#include "stddx.h"
#include "light.h"

AttackZone g_aAttackZone[ATTACK_ZONE_MAX];	// アタックゾーン情報
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAttackZone = NULL; //バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureAttackZone = NULL;      //テクスチャのポインタ
//******************************************************
// アタックゾーン初期化
//******************************************************
void InitAttackZone(void)
{
	for (int nCntAttackZone = 0; nCntAttackZone < ATTACK_ZONE_MAX; nCntAttackZone++)
	{// 最大アタックゾーン分回す
		// 各種変数の初期化
		g_aAttackZone[nCntAttackZone].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	    // 位置
		g_aAttackZone[nCntAttackZone].fRadius = 0.0f;						    // 半径
		g_aAttackZone[nCntAttackZone].bLess = 0.0f;						        // 半径
		g_aAttackZone[nCntAttackZone].bUse = false;						        // 使用
		g_aAttackZone[nCntAttackZone].col = D3DXCOLOR(0.3f, 0.0f, 0.0f, 0.1f);	// 色
		g_aAttackZone[nCntAttackZone].idexLight = -1;					        // ライトのインデックス
	}

	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//頂点バッファの生成
	CHECK_HR(CreateVertex3D
	(
		&g_pVtxBuffAttackZone,   // 頂点バッファのポインタ
		VT_MAX, ATTACK_ZONE_MAX, // 頂点数,バッファ数
		D3DUSAGE_WRITEONLY,      // 使用法
		D3DPOOL_DEFAULT          // プールの種類
	))

	//テクスチャの読み込み
	CHECK_HR(CreateTexture
	(
		&g_pTextureAttackZone,  // テクスチャのポインタ
		&ATTACK_ZONE_TEX,       // テクスチャのパス
		1,                      // テクスチャ数
		nullptr,                // 画像情報（NULLなら取得しない）
		D3DX_FILTER_TRIANGLE,   // フィルタリング
		D3DPOOL_DEFAULT         // プールの種類
	))

	//頂点の設定
	CHECK_HR(SetVertex3D
	(
		&g_pVtxBuffAttackZone,                                // 頂点バッファのポインタ
		0,                                                    // ロックフラグ
		0, ATTACK_ZONE_MAX,                                   // バッファの開始オフセット,バッファ数
		1, 1,                                                 // y分割数, x分割数
		1.0f, 1.0f,                                           // 幅,高さ
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),                    // 色
		1, nullptr,                                           // テクスチャ数,画像情報(nullの時画像はポリゴンの大きさに調節される)
		true											      // センターポジション
	))
}

//-----------------------------------------------------
//終了処理
//-----------------------------------------------------
void UninitAttackZone(void)
{
	for (int nCntAttackZone = 0; nCntAttackZone < ATTACK_ZONE_MAX; nCntAttackZone++)
	{// 最大アタックゾーン分回す
		EndAttackZone(nCntAttackZone); // アタックゾーンの終了
	}

	SAFE_RELEASE(g_pTextureAttackZone); // テクスチャの破棄

	SAFE_RELEASE(g_pVtxBuffAttackZone); // 頂点バッファの破棄
}

//------------------------------------------------------
//描画処理
//------------------------------------------------------
void DrawAttackZone(void)
{
	RSOP rsop, rsOpOld;                 // レンダーステート
	CHECK_HR(GetRenderOperation(&rsop)) // レンダーステートの取得
	rsOpOld = rsop;                     // 現在のレンダーステート

	// レンダーステートの変更
	rsop.Lighting = FALSE;             // ライトを無効にする
	rsop.zEnable = TRUE;               // Zテストを有効にする
	rsop.zWriteEnable = FALSE;         // Zバッファ書き込み
	rsop.zFunc = D3DCMP_LESSEQUAL;     // Zバッファ比較関数
	rsop.blendOp = D3DBLENDOP_ADD;     // ブレンド演算
	rsop.srcBlend = D3DBLEND_SRCALPHA; // ソースブレンド
	rsop.destBlend = D3DBLEND_ONE;     // デスティネーションブレンド
	rsop.alphaTestEnable = TRUE;       // アルファテストを有効にする
	rsop.alphaRef = 0;                 // アルファテスト参照値
	rsop.alphaFunc = D3DCMP_GREATER;   // アルファテスト比較関数

	CHECK_HR(SetRenderOperation(&rsop)) // レンダーステートの変更

	// 遠いほうから描画する
	static int nIdx[ATTACK_ZONE_MAX]; // 遠い順格納用
	static int nLimit = 0;
	if (nLimit % 60 == 0)
	{// フレームごとに処理 (毎フレームやると重くなる可能性があるため)
		D3DXVECTOR3 PosList[ATTACK_ZONE_MAX]; // ポスリスト
		Camera* pCamera = GetCamera();
		for (int nCnt = 0; nCnt < ATTACK_ZONE_MAX; nCnt++)
		{// ポスリスト作成
			if (g_aAttackZone[nCnt].bUse)
			{
				PosList[nCnt] = g_aAttackZone[nCnt].pos;
			}
			else
			{
				PosList[nCnt] = pCamera->posV;
			}
		}
		CHECK_HR(DeptSort(nIdx, PosList, ATTACK_ZONE_MAX)) //遠い順にソート
	}
	nLimit++;

	for (int nCntAttackZone = 0; nCntAttackZone < ATTACK_ZONE_MAX; nCntAttackZone++)
	{
		if (g_aAttackZone[nIdx[nCntAttackZone]].bUse == false)
		{// 使っていないものが出てきたら終了
			break;
		}

		CHECK_HR(GetRenderOperation(&rsop)) // レンダーステートの取得

		if (g_aAttackZone[nIdx[nCntAttackZone]].bLess)
		{
			// レンダーステートの変更
			rsop.blendOp = D3DBLENDOP_REVSUBTRACT;     // ブレンド演算
		}
		else
		{
			// レンダーステートの変更
			rsop.blendOp = D3DBLENDOP_ADD;     // ブレンド演算
		}

		CHECK_HR(SetRenderOperation(&rsop)) // レンダーステートの変更

		//描画
		CHECK_HR(DrawVertex3D
		(
			&g_pVtxBuffAttackZone, nullptr, &g_pTextureAttackZone,                                                     //バッファ(頂点,インデックス,テクスチャ)
			&g_aAttackZone[nIdx[nCntAttackZone]].pos, nullptr, nullptr, &g_aAttackZone[nIdx[nCntAttackZone]].mtxWorld, //位置,回転,大きさ,ワールドマトリックス
			BILLBOARD_XZ_AXIS, 0.0001f,                                                                                //ビルボードのタイプ、Zバイアス値
			D3DPT_TRIANGLESTRIP,                                                                                       //描画のタイプ
			VT_MAX, nIdx[nCntAttackZone], 2                                                                            //頂点数、開始インデックス、ポリゴン数
		))
	}

	CHECK_HR(SetRenderOperation(&rsOpOld)) // レンダーステートを戻す
}

//******************************************************
// アタックゾーン設定
//******************************************************
int SetAttackZone(D3DXVECTOR3 pos, float fRadius, D3DXCOLOR col, bool Less)
{
	for (int nCntAttackZone = 0; nCntAttackZone < ATTACK_ZONE_MAX; nCntAttackZone++)
	{// 最大アタックゾーン分回す
		if (g_aAttackZone[nCntAttackZone].bUse == false)
		{// 使っていないアタックゾーン
			g_aAttackZone[nCntAttackZone].pos = pos;		    // 位置
			g_aAttackZone[nCntAttackZone].fRadius = fRadius;	// 半径
			g_aAttackZone[nCntAttackZone].col = col;	        // 半径
			g_aAttackZone[nCntAttackZone].bLess = Less;		// 減算
			g_aAttackZone[nCntAttackZone].bUse = true;		// 使用

			//頂点の設定
			CHECK_HR(SetVertex3D
			(
				&g_pVtxBuffAttackZone,
				0,
				nCntAttackZone, 1,
				1, 1,
				g_aAttackZone[nCntAttackZone].fRadius * 2.0f, g_aAttackZone[nCntAttackZone].fRadius * 2.0f,
				g_aAttackZone[nCntAttackZone].col,
				1, nullptr,
				true
			))

			D3DLIGHT9 light;
			//種類
			light.Type = D3DLIGHT_SPOT;
			//拡散光
			light.Diffuse = g_aAttackZone[nCntAttackZone].col * 20.0f;
			//環境光
			light.Ambient = g_aAttackZone[nCntAttackZone].col * 20.0f;
			//鏡面反射光
			light.Specular = g_aAttackZone[nCntAttackZone].col * 20.0f;
			//位置
			light.Position = g_aAttackZone[nCntAttackZone].pos + D3DXVECTOR3(0.0f, g_aAttackZone[nCntAttackZone].fRadius * 0.5f, 0.0f);
			//方向
			light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			//範囲
			light.Range = g_aAttackZone[nCntAttackZone].fRadius;
			//コーン減衰率
			light.Falloff = 0.01f;
			//距離減衰率
			light.Attenuation0 = 0.1f;
			//コーン角度(内)
			light.Theta = D3DXToRadian(90.0f);
			//コーン角度(外)
			light.Phi = D3DXToRadian(360.0f);

			g_aAttackZone[nCntAttackZone].idexLight = SetLight(&light);
			return nCntAttackZone;
		}
	}
	return -1;
}

//******************************************************
// アタックゾーン破棄
//******************************************************
void EndAttackZone(int nIdx)
{
	EndLight(g_aAttackZone[nIdx].idexLight); // ライトの終了
	g_aAttackZone[nIdx].idexLight = -1;	     // ライトのインデックス
	g_aAttackZone[nIdx].bUse = false;		 // 使用
}