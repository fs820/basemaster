//----------------------------------------
//
//壁処理[Wall.cpp]
//Author fuma sato
//
//----------------------------------------
#include"Wall.h"
#include"camera.h"
#include"stddxbuff.h"
#include"input.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;//バッファのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffWall = NULL; //インデックスバッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;     //テクスチャのポインタ
#ifdef _DEBUG
WALL_TYPE g_Type;                             //タイプ
int g_DelNumberWall;                          //設置番号
#endif // _DEBUG
bool g_bDisp = false;                         //表示状態
Wall g_aWall[WALL_MAX];     //情報体
//----------------------
//初期化処理
//----------------------
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VT_MAX_WALL * WALL_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_DEFAULT,
		&g_pVtxBuffWall,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFileEx
	(
		pDevice,
		WALL_TEX,
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
		&g_pTextureWall
	);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * INDEX_NUM_WALL,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&g_pIdxBuffWall,
		NULL
	);

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//初期化
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].mtxWorld = D3DXMATRIX();
		g_aWall[nCntWall].Width = 0.0f;
		g_aWall[nCntWall].Height = 0.0f;
		g_aWall[nCntWall].bAlpha = false;
		g_aWall[nCntWall].bBack = false;
		g_aWall[nCntWall].bUse = false;
		g_aWall[nCntWall].Type = WTYPE_NORMAL;
	}
	//1つ目

	//頂点の設定
	if(FAILED(SetVertex3DYX
	(
		&g_pVtxBuffWall,
		0, WALL_MAX,
		WALL_YNUM, WALL_XNUM,
		WALL_WIDTH, WALL_HEIGHT,
		D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		WALL_WIDTH_DEF, WALL_HEIGHT_DEF,
		false, false
	)))
	{
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "頂点エラー", "WALL", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}

	//インデックスの設定
	if (FAILED(SetIndex
	(
		&g_pIdxBuffWall,
		WALL_YNUM, WALL_XNUM
	)))
	{
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "インデックスエラー", "WALL", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}

#ifdef _DEBUG
	//設置番号
	g_DelNumberWall = -1;
#endif // _DEBUG
	//表示状態
	g_bDisp = false;
}

//-------------------
//終了処理
//-------------------
void UninitWall(void)
{
	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//初期化
		g_aWall[nCntWall].bUse = false;
	}

	//テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffWall != NULL)
	{
		g_pIdxBuffWall->Release();
		g_pIdxBuffWall = NULL;
	}
}

//-------------------
//更新処理
//-------------------
void UpdateWall(void)
{

}

//-------------------
//描画処理
//-------------------
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATRIX mtxRot, mtxTrans;//計算マトリックス

	//デバイスの取得
	pDevice = GetDevice();

	int nCntWall;
	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//最大数回す
		if (g_aWall[nCntWall].bUse && (g_aWall[nCntWall].Type == WTYPE_NORMAL || g_bDisp ))
		{//使っていたら
			//マトリックス初期化
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			//向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

			//位置の計算
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			//頂点バッファ
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffWall);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				VT_MAX_WALL * nCntWall,
				0,
				VT_MAX_WALL,//頂点数
				0,
				POLYGON_NUM_WALL//ポリゴンの個数
			);
		}
	}
}

//----------
//設定
//----------
int SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, WALL_TYPE Type)
{
	int nCntWall, nIdx = 0;
	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//最大数回す
		if (!g_aWall[nCntWall].bUse)
		{//使っていない
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = rot;
			g_aWall[nCntWall].Width = Width;
			g_aWall[nCntWall].Height = Height;

			//頂点の設定
			if (FAILED(SetVertex3DYX
			(
				&g_pVtxBuffWall,
				nCntWall, 1,
				WALL_YNUM, WALL_XNUM,
				g_aWall[nCntWall].Width, g_aWall[nCntWall].Height,
				D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				WALL_WIDTH_DEF, WALL_HEIGHT_DEF,
				false, false
			)))
			{
				HWND hWnd;
				hWnd = GethWnd();
				MessageBox(hWnd, "頂点エラー", "WALL", MB_OK | MB_ICONERROR);
				PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
			}

			g_aWall[nCntWall].bAlpha = false;
			g_aWall[nCntWall].bBack = false;
			g_aWall[nCntWall].bUse = true;
			g_aWall[nCntWall].Type = Type;
			if (g_aWall[nCntWall].Type == WTYPE_INVISIBLE)
			{
				return nCntWall;
			}
			break;
		}
	}

	nIdx = nCntWall;

#ifndef _DEBUG
	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//最大数回す
		if (!g_aWall[nCntWall].bUse)
		{//使っていない
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = rot;
			g_aWall[nCntWall].rot.y -= D3DX_PI;
			//正規化
			if (g_aWall[nCntWall].rot.y > D3DX_PI)
			{
				g_aWall[nCntWall].rot.y -= D3DX_PI * 2.0f;
			}
			else if (g_aWall[nCntWall].rot.y < -D3DX_PI)
			{
				g_aWall[nCntWall].rot.y += D3DX_PI * 2.0f;
			}
			g_aWall[nCntWall].Width = Width;
			g_aWall[nCntWall].Height = Height;

			//頂点の設定
			if (FAILED(SetVertex3DYX
			(
				&g_pVtxBuffWall,
				nCntWall, 1,
				WALL_YNUM, WALL_XNUM,
				g_aWall[nCntWall].Width, g_aWall[nCntWall].Height,
				D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, BACK_WALL_ALPHA),
				WALL_WIDTH_DEF, WALL_HEIGHT_DEF,
				false, false
			)))
			{
				HWND hWnd;
				hWnd = GethWnd();
				MessageBox(hWnd, "頂点エラー", "WALL", MB_OK | MB_ICONERROR);
				PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
			}

			g_aWall[nCntWall].bAlpha = false;
			g_aWall[nCntWall].bBack = true;
			g_aWall[nCntWall].bUse = true;
			g_aWall[nCntWall].Type = WTYPE_NORMAL;
			break;
		}
	}
#endif // !_DEBUG

	return nIdx; //セットした番号を返す
}

//--------------------------
//情報取得
//--------------------------
Wall* GetWall(void)
{
	return &g_aWall[0];
}

//----------------------
//当たり判定
//----------------------
void CollisionWall(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float Width, float fHeight)
{
	D3DXVECTOR3 aPos[2] = {}, Wallvec = {}, Posvec = {}, PosOldvec = {}, movevec = {}, Norvec = {}, Dovec = {}, Hit = {};
	int nCntWall;
	float WallCross, PosCross;
	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//壁の数
		if (g_aWall[nCntWall].bUse && g_aWall[nCntWall].bBack == false)
		{//使っている壁
			aPos[0] = D3DXVECTOR3(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].Width * 0.5f * sinf(g_aWall[nCntWall].rot.y + D3DX_PI * -0.5f), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z + g_aWall[nCntWall].Width * 0.5f * cosf(g_aWall[nCntWall].rot.y + D3DX_PI * -0.5f));
			aPos[1] = D3DXVECTOR3(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].Width * 0.5f * sinf(g_aWall[nCntWall].rot.y + D3DX_PI * 0.5f), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z + g_aWall[nCntWall].Width * 0.5f * cosf(g_aWall[nCntWall].rot.y + D3DX_PI * 0.5f));

			if (g_aWall[nCntWall].Type==WTYPE_NORMAL)
			{
				Wallvec = aPos[1] - aPos[0];		//壁のベクトル

				Norvec = D3DXVECTOR3(Wallvec.z, Wallvec.y, -Wallvec.x);
				D3DXVec3Normalize(&Norvec, &Norvec);

				// 体分の計算
				D3DXVECTOR3 Pos = *pPos + -Norvec * Width, PosOld = *pPosOld + -Norvec * Width;

				Posvec = Pos - aPos[0];			//壁に対するプレイヤーのベクトル
				PosOldvec = PosOld - aPos[0];		//壁に対するプレイヤーの旧ベクトル
				movevec = Pos - PosOld;			//プレイヤーの移動ベクトル
				if ((Wallvec.z * Posvec.x) - (Wallvec.x * Posvec.z) <= 0.0f && (Wallvec.z * PosOldvec.x) - (Wallvec.x * PosOldvec.z) >= 0.0f && pPos->y < g_aWall[nCntWall].pos.y + g_aWall[nCntWall].Height && pPos->y + fHeight > g_aWall[nCntWall].pos.y)
				{//壁の外側
					WallCross = (Wallvec.z * movevec.x) - (Wallvec.x * movevec.z);
					PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
					PosCross /= WallCross;
					if (PosCross >= -0.05f && PosCross < 1.05f)
					{//壁の範囲
						Hit = aPos[0] + Wallvec * PosCross;
						movevec = Pos - Hit;//プレイヤーの移動ベクトル
						Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
						*pPos += Dovec * 1.1f;
					}
				}
			}
			else
			{
				Wallvec = aPos[1] - aPos[0];		//壁のベクトル
				Posvec = *pPos - aPos[0];			//壁に対するプレイヤーのベクトル
				PosOldvec = *pPosOld - aPos[0];		//壁に対するプレイヤーの旧ベクトル
				movevec = *pPos - *pPosOld;			//プレイヤーの移動ベクトル
				if ((Wallvec.z * Posvec.x) - (Wallvec.x * Posvec.z) <= 0.0f && (Wallvec.z * PosOldvec.x) - (Wallvec.x * PosOldvec.z) >= 0.0f && pPos->y < g_aWall[nCntWall].pos.y + g_aWall[nCntWall].Height && pPos->y + fHeight > g_aWall[nCntWall].pos.y)
				{//壁の外側
					WallCross = (Wallvec.z * movevec.x) - (Wallvec.x * movevec.z);
					PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
					PosCross /= WallCross;
					if (PosCross >= -0.05f && PosCross < 1.05f)
					{//壁の範囲
						Hit = aPos[0] + Wallvec * PosCross;
						movevec = *pPos - Hit;//プレイヤーの移動ベクトル
						Norvec = D3DXVECTOR3(Wallvec.z, Wallvec.y, -Wallvec.x);
						D3DXVec3Normalize(&Norvec, &Norvec);
						Dovec = Norvec * ((-movevec.x * Norvec.x) + (-movevec.z * Norvec.z));
						*pPos += Dovec * 1.1f;
					}
				}
			}
		}
	}
}

//----------------------
//当たり判定
//----------------------
bool TriggerWall(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fHeight)
{
	bool bTrigger = false;

	D3DXVECTOR3 aPos[2] = {}, Wallvec = {}, Posvec = {}, PosOldvec = {}, movevec = {}, Norvec = {}, Dovec = {}, Hit = {};
	int nCntWall;
	float WallCross, PosCross;
	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{//壁の数
		if (g_aWall[nCntWall].bUse && g_aWall[nCntWall].bBack == false)
		{//使っている壁
			aPos[0] = D3DXVECTOR3(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].Width * 0.5f * sinf(g_aWall[nCntWall].rot.y + D3DX_PI * -0.5f), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z + g_aWall[nCntWall].Width * 0.5f * cosf(g_aWall[nCntWall].rot.y + D3DX_PI * -0.5f));
			aPos[1] = D3DXVECTOR3(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].Width * 0.5f * sinf(g_aWall[nCntWall].rot.y + D3DX_PI * 0.5f), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z + g_aWall[nCntWall].Width * 0.5f * cosf(g_aWall[nCntWall].rot.y + D3DX_PI * 0.5f));

			Wallvec = aPos[1] - aPos[0];		//壁のベクトル
			Posvec = *pPos - aPos[0];			//壁に対するプレイヤーのベクトル
			PosOldvec = *pPosOld - aPos[0];		//壁に対するプレイヤーの旧ベクトル
			movevec = *pPos - *pPosOld;			//プレイヤーの移動ベクトル
			if ((Wallvec.z * Posvec.x) - (Wallvec.x * Posvec.z) <= 0.0f && (Wallvec.z * PosOldvec.x) - (Wallvec.x * PosOldvec.z) >= 0.0f && pPos->y < g_aWall[nCntWall].pos.y + g_aWall[nCntWall].Height && pPos->y + fHeight > g_aWall[nCntWall].pos.y)
			{//壁の外側
				WallCross = (Wallvec.z * movevec.x) - (Wallvec.x * movevec.z);
				PosCross = (Posvec.z * movevec.x) - (Posvec.x * movevec.z);
				PosCross /= WallCross;
				if (PosCross >= -0.05f && PosCross < 1.05f)
				{//壁の範囲
					bTrigger = true;
					break;
				}
			}
		}
	}

	return bTrigger;
}

//---------------------------
//読み込み
//---------------------------
void LoadWall(void)
{
	FILE* pFile;//ファイルポインタを宣言
	MODE Mode = GetMode();
	int nCount = 0;
	int nNumber = 0;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Width = 0.0f, Height = 0.0f;

	//Xファイル
	switch (Mode)
	{
	case MODE_GAME:
		pFile = fopen(WALL_FILE, "r");//ファイルを開く
		break;
	case MODE_TUTO:
		pFile = fopen(WALL_FILE_TUTORIAL, "r");//ファイルを開く
		break;
	default:
		pFile = fopen(nullptr, "r");//ファイルを開く
		break;
	}

	if (pFile != NULL)
	{//開けたら
		for (nCount = 0; nCount < WALL_MAX; nCount++)
		{
			if (fscanf(pFile, "%d", &nNumber) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &pos.x) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &pos.y) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &pos.z) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &rot.x) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &rot.y) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &rot.z) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &Width) == EOF)break;//数値を書き入れ
			if (fscanf(pFile, "%f", &Height) == EOF)break;//数値を書き入れ

			//角度変換
			rot = D3DXToRadian(rot);

			//オブジェクト設置
			SetWall(pos, rot, Width, Height, (WALL_TYPE)nNumber);
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

//---------------------------
//書き込み
//---------------------------
void SaveWall(void)
{
	FILE* pFile;//ファイルポインタを宣言
	MODE Mode = GetMode();
	D3DXVECTOR3 rot;
	int nCount = 0;

	//Xファイル
	switch (Mode)
	{
	case MODE_GAME:
		pFile = fopen(WALL_FILE, "w");//ファイルを開く
		break;
	case MODE_TUTO:
		pFile = fopen(WALL_FILE_TUTORIAL, "w");//ファイルを開く
		break;
	default:
		pFile = fopen(nullptr, "w");//ファイルを開く
		break;
	}

	if (pFile != NULL)
	{//開けたら
		for (nCount = 0; nCount < WALL_MAX; nCount++)
		{
			if (g_aWall[nCount].bUse && g_aWall[nCount].bBack == false)
			{
				//角度変換
				rot = D3DXToDegree(g_aWall[nCount].rot);

				fprintf(pFile, "%d\n", (int)g_aWall[nCount].Type);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aWall[nCount].pos.x);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aWall[nCount].pos.y);//数値を書き入れ
				fprintf(pFile, "%.1f\n", g_aWall[nCount].pos.z);//数値を書き入れ
				fprintf(pFile, "%.1f ", rot.x);//数値を書き入れ
				fprintf(pFile, "%.1f ", rot.y);//数値を書き入れ
				fprintf(pFile, "%.1f\n", rot.z);//数値を書き入れ
				fprintf(pFile, "%.1f ", g_aWall[nCount].Width);   //数値を書き入れ
				fprintf(pFile, "%.1f\n\n", g_aWall[nCount].Height);//数値を書き入れ
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

//------------------------
//エディター
//------------------------
void UpdateWallEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot)
{
#ifdef _DEBUG
	Camera* pCamera = GetCamera();
	static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static float Width = 0.0f, Height = 0.0f;
	static int HoldNumber = -1, EndNumber = -1;

	if (GetKeyboardTrigger(DIK_9))
	{//表示の切替
		g_bDisp = !g_bDisp;
	}

	if (GetKeyboardTrigger(DIK_RIGHT))
	{// タイプ切替
		g_Type = (WALL_TYPE)(g_Type + 1);
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{// タイプ切替
		g_Type = (WALL_TYPE)(g_Type - 1);
	}

	//範囲制限
	g_Type = (WALL_TYPE)(g_Type % (WTYPE_MAX));

	if (g_Type < 0)
	{//ループ
		g_Type = (WALL_TYPE)(WTYPE_MAX - 1);
	}

	if (GetKeyboardTrigger(DIK_UP))
	{// DELオブジェクト切替
		g_DelNumberWall++;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{// DELオブジェクト切替
		g_DelNumberWall--;
	}

	for (int nCount = 0; nCount < WALL_MAX; nCount++)
	{//最大数回す

		//範囲制限
		if (g_DelNumberWall < 0)
		{
			g_DelNumberWall = WALL_MAX - 1;
		}
		g_DelNumberWall %= WALL_MAX;

		if (g_aWall[g_DelNumberWall].bUse && g_aWall[g_DelNumberWall].bBack == false)
		{//使っているオブジェクトの選択
			break;
		}
		else
		{//使っていない番号はスキップ

			if (GetKeyboardTrigger(DIK_DOWN))
			{// DELオブジェクト切替
				g_DelNumberWall--;
			}
			else
			{// DELオブジェクト切替
				g_DelNumberWall++;
			}
		}

		if (nCount >= WALL_MAX - 1)
		{//どれも使っていないとき
			g_DelNumberWall = -1;
		}
	}

	if (GetKeyboardPress(DIK_F) == true || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) == true)//Aキー
	{
		if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//Wキー
		{//左上
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.25f) * 5.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.25f) * 5.0f;
		}
		else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//Sキー
		{//左下
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.75f) * 5.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.75f) * 5.0f;
		}
		else
		{//左
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.5f) * 5.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.5f) * 5.0f;
		}
	}
	else if (GetKeyboardPress(DIK_H) == true || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) == true)//Dキー
	{
		if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//Wキー
		{//右上
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * 5.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * 5.0f;
		}
		else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//Sキー
		{//右下
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * 5.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * 5.0f;
		}
		else
		{//右
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * 5.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * 5.0f;
		}
	}
	else if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//Wキー
	{//上
		pos.x += sinf(pCamera->rot.y) * 5.0f;
		pos.z += cosf(pCamera->rot.y) * 5.0f;
	}
	else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//Sキー
	{//下
		pos.x += sinf(pCamera->rot.y + D3DX_PI) * 5.0f;
		pos.z += cosf(pCamera->rot.y + D3DX_PI) * 5.0f;
	}

	if (GetKeyboardPress(DIK_V) == true)//Wキー
	{//上
		pos.y -= 5.0f;
	}
	if (GetKeyboardPress(DIK_B) == true)//Sキー
	{//下
		pos.y += 5.0f;
	}

	if (GetKeyboardPress(DIK_J) || GetJoykeyPress(JOYKEY_LT, CONTROLLER_1))
	{// 押している間
		rot.y -= 0.05f;
	}
	if (GetKeyboardPress(DIK_K) || GetJoykeyPress(JOYKEY_RT, CONTROLLER_1))
	{// 押している間
		rot.y += 0.05f;
	}
	if (GetKeyboardPress(DIK_7) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// 押している間
		Width -= 10.0f;
	}
	if (GetKeyboardPress(DIK_8) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// 押している間
		Width += 10.0f;
	}
	if (GetKeyboardPress(DIK_U) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// 押している間
		Height -= 10.0f;
	}
	if (GetKeyboardPress(DIK_I) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// 押している間
		Height += 10.0f;
	}

	if (GetKeyboardTrigger(DIK_0) || GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1))
	{// 初期化
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Width = 0.0f;
		Height = 0.0f;
	}

	//正規化
	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	//スケール制限
	Width = max(WALL_WIDTH_MIN, min(WALL_WIDTH_MAX, Width));
	Height = max(WALL_HEIGHT_MIN, min(WALL_HEIGHT_MAX, Height));


	if (GetKeyboardTrigger(DIK_RSHIFT) || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1))
	{// 押したとき
		HoldNumber = SetWall(Epos + pos, Erot + rot, Width, Height, g_Type);
		EndNumber = HoldNumber;
	}
	if (GetKeyboardPress(DIK_RSHIFT) || GetJoykeyPress(JOYKEY_RB, CONTROLLER_1))
	{// 押している間
		if (HoldNumber != -1)
		{
			g_aWall[HoldNumber].pos = Epos + pos;
			g_aWall[HoldNumber].rot = Erot + rot;
			g_aWall[HoldNumber].Width = Width;
			g_aWall[HoldNumber].Height = Height;

			//頂点の設定
			if (FAILED(SetVertex3DYX
			(
				&g_pVtxBuffWall,
				HoldNumber, 1,
				WALL_YNUM, WALL_XNUM,
				g_aWall[HoldNumber].Width, g_aWall[HoldNumber].Height,
				D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				WALL_WIDTH_DEF, WALL_HEIGHT_DEF,
				false, false
			)))
			{
				HWND hWnd;
				hWnd = GethWnd();
				MessageBox(hWnd, "頂点エラー", "WALL", MB_OK | MB_ICONERROR);
				PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
			}
		}
	}
	else
	{
		HoldNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_BACK) || GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1))
	{// 押したとき
		g_aWall[EndNumber].bUse = false;
	}

	if (GetKeyboardTrigger(DIK_F8))
	{// セーブ
		SaveWall();
	}

	if (GetKeyboardTrigger(DIK_DELETE) || (GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1) && GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1)))
	{// 押したとき
		g_aWall[g_DelNumberWall].bUse = false;
	}

	static int DelNumber = -1; //一時保存用ナンバー

	if (DelNumber != g_DelNumberWall)
	{//デリートナンバーが変化した
		if (DelNumber != -1)
		{//前回のオブジェクトが存在する
			g_aWall[DelNumber].bAlpha = false; //元に戻す

			//頂点の設定
			if (FAILED(SetVertex3DYX
			(
				&g_pVtxBuffWall,
				DelNumber, 1,
				WALL_YNUM, WALL_XNUM,
				g_aWall[DelNumber].Width, g_aWall[DelNumber].Height,
				D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				WALL_WIDTH_DEF, WALL_HEIGHT_DEF,
				false, false
			)))
			{
				HWND hWnd;
				hWnd = GethWnd();
				MessageBox(hWnd, "頂点エラー", "WALL", MB_OK | MB_ICONERROR);
				PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
			}
		}
		DelNumber = g_DelNumberWall; //選択オブジェクトの更新
		if (DelNumber != -1)
		{//オブジェクトが存在する
		g_aWall[DelNumber].bAlpha = true; //元の値の保持

			//頂点の設定
			if (FAILED(SetVertex3DYX
			(
				&g_pVtxBuffWall,
				DelNumber, 1,
				WALL_YNUM, WALL_XNUM,
				g_aWall[DelNumber].Width, g_aWall[DelNumber].Height,
				D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f),
				WALL_WIDTH_DEF, WALL_HEIGHT_DEF,
				false, false
			)))
			{
				HWND hWnd;
				hWnd = GethWnd();
				MessageBox(hWnd, "頂点エラー", "WALL", MB_OK | MB_ICONERROR);
				PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
			}
		}
	}
#endif // _DEBUG
}

//--------------------------------
//数の取得
//--------------------------------
int GetWallNum(WALL_TYPE Type)
{
	int Num = 0;
	if (Type == WTYPE_MAX)
	{
		for (int nCnt = 0; nCnt < WALL_MAX; nCnt++)
		{
			if (g_aWall[nCnt].bUse)
			{
				Num++;
			}
		}
		return Num;
	}

	for (int nCnt = 0; nCnt < WALL_MAX; nCnt++)
	{
		if (g_aWall[nCnt].bUse && g_aWall[nCnt].Type==Type)
		{
			Num++;
		}
	}

	return Num;
}

//-----------------------
//タイプ
//-----------------------
int GetWallType(void)
{
#ifdef _DEBUG
	return g_Type;
#endif // _DEBUG
}

//-----------------------
//タイプ
//-----------------------
int GetDelWall(void)
{
#ifdef _DEBUG
	return g_DelNumberWall;
#endif // _DEBUG
}