//------------------------------------------
//
//メインの処理[main.cpp]
//Author fuma sato
//
//------------------------------------------

#include "main.h"
#include "resource.h"
#include "camera.h"
#include "fade.h"
#include "light.h"
#include "sound.h"
#include "input.h"
#include "pause.h"
#include "game.h"
#include "result.h"
#include "title.h"
#include "stddxbuff.h"

LPDIRECT3D9 g_pD3D = NULL;//ダイレクトXオブジェクトのグローバルポインタを宣言
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;//ダイレクトXデバイスのグローバルポインタを宣言
MODE g_mode = MODE_TITLE;
HINSTANCE g_hInstance;
HWND g_hWnd;
bool g_bStop = false;
bool g_bFullScreen = false;
LPD3DXFONT g_pFont = NULL;//フォントポインタ
int g_nCountFPS = 0;//FPSカウンタ
bool g_bDebug = true;

//------------------------
//メイン関数
//------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//ウインドウ設定
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),//メモリサイズ
		CS_CLASSDC,//スタイル
		WindowProc,//プロシージャ
		0,//0
		0,//0
		hInstance,//インスタンスハンドル
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),//タスクバーアイコン
		LoadCursor(hInstance,MAKEINTRESOURCE(IDC_CURSOR1)),//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 3),//背景色
		MAKEINTRESOURCE(IDR_MENU1),//メニューバー
		CLASS_NAME,//クラスの名前
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1))//アイコン
	};

	srand((unsigned int)time(NULL));//乱数設定
	HWND hWnd = nullptr;//ハンドル
	MSG msg = { 0 };//メッセージ
	RECT rect = { 0,0,(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };//ウインドウサイズの設定
	DWORD dwCurrntTime;//現在のシステム時刻
	DWORD dwExceLastTime;//前回のシステム時刻

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// DPIスケーリング対応
	if (FAILED(SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE)))
	{
		SetProcessDPIAware();  // Windows 7 互換用
	}

	//クライアント領域のサイズ調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

	//ウインドウを生成
	hWnd = CreateWindowEx
	(
		0,//スタイル
		CLASS_NAME,//ウインドウクラスの名前
		WINDOW_NAME,//ウインドウの名前
		WS_OVERLAPPEDWINDOW,//スタイル
		CW_USEDEFAULT,//左上座標
		CW_USEDEFAULT,//右下座標
		(rect.right - rect.left),//幅
		(rect.bottom - rect.top),//高さ
		NULL,//親
		NULL,//メニューID
		hInstance,//インスタンスハンドル
		NULL//作成データ
	);

	g_hInstance = hInstance;
	g_hWnd = hWnd;

	//表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (FAILED(Init(hInstance, hWnd, TRUE)))//初期化処理
	{
		return E_FAIL;
	}

	RECT Rect;
	GetWindowRect(hWnd, &Rect);

	timeBeginPeriod(1);//分解能の設定
	dwCurrntTime = 0;//初期時間
	dwExceLastTime = timeGetTime();//開始時刻を記録

	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)//終了メッセージ
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (!g_bStop)
			{
				dwCurrntTime = timeGetTime();//時刻を取得
				if ((dwCurrntTime-dwFPSLastTime)>=FPS_TIME)
				{//0.5秒
					//FPS算出
					g_nCountFPS = (dwFrameCount * 1000) / (dwCurrntTime - dwFPSLastTime);
					dwFPSLastTime = dwCurrntTime;
					dwFrameCount = 0;
				}

				if ((dwCurrntTime - dwExceLastTime) >= 1000 / FPS)//設定した間隔が空いたら
				{//60分の1
					dwExceLastTime = dwCurrntTime;//今回の更新時刻を記録

					Update();//更新処理
					Render();//描画処理

					dwFrameCount++;
				}
			}
		}
	}

	Uninit();//終了処理

	//ウインドウクラスの登録解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	timeEndPeriod(1);//分解能を戻す

	CursorSwitch(ON);

	//終了
	return (int)msg.wParam;
}

//--------------------------
//ウインドウプロシージャ
//--------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID = 0;//返り値の格納
	static bool bFreeMause=false;

	switch (uMsg)
	{
	case WM_DESTROY://破棄
		//WM_QUITメッセージ
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN://キー押下
		switch (wParam)
		{
		case VK_ESCAPE://ESCキー
			CursorSwitch(ON);
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
			if (nID == IDYES)
			{
				//破棄(WM_DESTROY)
				DestroyWindow(hWnd);
			}
			break;
		case VK_F2:
			g_bDebug = !g_bDebug;
			break;
		case VK_F4:
			if (bFreeMause)
			{
				bFreeMause = false;

				CursorSwitch(OFF);
			}
			else
			{
				bFreeMause = true;

				CursorSwitch(ON);
			}
			break;
		}
		break;
	case WM_SETFOCUS:
		g_bStop = false;
		break;
	case WM_KILLFOCUS:
		g_bStop = true;
		break;
	case WM_COMMAND:
		// メニュー項目のIDを取得
		switch (LOWORD(wParam))
		{
		case IDM_FILE_NEW:
			// 新規作成の処理
			break;
		case IDM_FILE_OPEN:
			// ファイルを開く処理
			break;
		case IDM_FILE_SAVE:
			// 保存処理
			break;
		case IDM_FILE_SAVEAS:
			// 名前を付けて保存処理
			break;
		case IDM_FILE_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_EDIT_UNDO:
			// 元に戻す処理
			break;
		case IDM_EDIT_CUT:
			// 切り取り処理
			break;
		case IDM_EDIT_COPY:
			// コピー処理
			break;
		case IDM_EDIT_PASTE:
			// 貼り付け処理
			break;
		case IDM_HELP_ABOUT:
			MessageBox(hWnd, "アプリケーションのバージョン情報", "バージョン情報", MB_OK | MB_ICONINFORMATION);
			break;
		}
		break;
	}
	//必要データを返す
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//------------------------------
//初期化処理
//------------------------------
HRESULT Init(HINSTANCE hInstanse, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;//ダイレクトXディスプレイモードの変数宣言
	D3DPRESENT_PARAMETERS d3dpp;//ダイレクトXパラメータの変数宣言

	//オブジェクト生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//ディスプレイモード
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// クライアント領域のサイズを取得
	RECT clientRect;
	GetClientRect(g_hWnd, &clientRect);

	ZeroMemory(&d3dpp, sizeof(d3dpp));//0を敷き詰める

	//バックバッファの設定
	d3dpp.BackBufferWidth = clientRect.right - clientRect.left;
	d3dpp.BackBufferHeight = clientRect.bottom - clientRect.top;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;// 描画の乱れを減らす メモリ負荷が増えるので使わないときは戻す
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL; // 描画の乱れを減らす 深度バッファを毎フレーム破棄する メモリ負荷を減らすがシャドウマッピングができなくなる
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//デバイス生成
	if (FAILED(g_pD3D->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice
	)))
	{
		if (FAILED(g_pD3D->CreateDevice
		(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice
		)))
		{
			if (FAILED(g_pD3D->CreateDevice
			(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice
			)))
			{
				return E_FAIL;
			}
		}
	}

	//描画設定
	RSOP rsOp;                //レンダーステートの構造体
	SSOP ssOp[TEXTURE_MAX];   //サンプラーステートの構造体
	TSSOP tssOp[TEXTURE_MAX]; //テクスチャステージの構造体
	ZeroMemory(&rsOp, sizeof(rsOp)); //0を敷き詰める
	RETURN_HR(GetDrawOperation(&rsOp, &ssOp[0], TEXTURE_MAX, &tssOp[0], TEXTURE_MAX)) //描画情報を取得

	//レンダーステートの設定
	rsOp.Lighting = TRUE;                  // ライティング
	rsOp.cullMode = D3DCULL_CCW;           // カリングの設定
	rsOp.alphaBlendEnable = TRUE;          // プレイヤーの中に透明度を加える
	rsOp.srcBlend = D3DBLEND_SRCALPHA;     // 合成法
	rsOp.destBlend = D3DBLEND_INVSRCALPHA; // 合成法
	rsOp.blendOp = D3DBLENDOP_ADD;         // ブレンド演算
	rsOp.zEnable = TRUE;                   // Zバッファ
	rsOp.zWriteEnable = TRUE;              // Zバッファ書き込み
	rsOp.zFunc = D3DCMP_LESSEQUAL;         // Zバッファ比較関数
	rsOp.zBias = 0.0f;                     // Zバイアス
	rsOp.alphaTestEnable = FALSE;          // アルファテスト
	rsOp.alphaRef = 0;                     // アルファテスト参照値
	rsOp.alphaFunc = D3DCMP_GREATER;       // アルファテスト比較関数
	rsOp.antialiaseLineEnable = TRUE;      // アンチエイリアス
	rsOp.stencilEnable = FALSE;            // ステンシルテスト

	// GPU に合わせた異方性フィルタ設定
	D3DCAPS9 caps;
	g_pD3DDevice->GetDeviceCaps(&caps);
	DWORD maxAniso = min(caps.MaxAnisotropy, static_cast<DWORD>(16));

	// サンブラーステートの設定
	for (unsigned int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		ssOp[nCntTex].maxAnisotropy = maxAniso; // 異方性フィルタの最大値
		ssOp[nCntTex].minFilter = D3DTEXF_ANISOTROPIC; // テクスチャミニフィケーションフィルタ
		ssOp[nCntTex].magFilter = D3DTEXF_ANISOTROPIC; // テクスチャマグニフィケーションフィルタ
		ssOp[nCntTex].maxMipLevel = 0; // ミップマップレベル
		ssOp[nCntTex].mipMapLODBias = -0.75f; // ミップマップLODバイアス
		ssOp[nCntTex].mipFilter = D3DTEXF_ANISOTROPIC; // ミップマップフィルタ
		ssOp[nCntTex].addressU = D3DTADDRESS_WRAP; // U方向のアドレスモード
		ssOp[nCntTex].addressV = D3DTADDRESS_WRAP; // V方向のアドレスモード
		ssOp[nCntTex].addressW = D3DTADDRESS_WRAP; // W方向のアドレスモード (3Dテクスチャ用)
		ssOp[nCntTex].borderColor = D3DCOLOR_ARGB(255, 55, 100, 100); // ボーダーカラー (Clamp時の境界色)
	}

	// テクスチャステージステートの設定
    // ステージ 0 のみ使用（通常のオブジェクト）
	//カラー
	tssOp[0].colorOp = D3DTOP_MODULATE; // カラーオペレーション
	tssOp[0].colorArg0 = D3DTA_CURRENT; // オペレーションの引数0
	tssOp[0].colorArg1 = D3DTA_TEXTURE; // オペレーションの引数1
	tssOp[0].colorArg2 = D3DTA_DIFFUSE; // オペレーションの引数2
	//透明度
	tssOp[0].alphaOp = D3DTOP_MODULATE; // アルファオペレーション
	tssOp[0].alphaArg0 = D3DTA_CURRENT; // オペレーションの引数0
	tssOp[0].alphaArg1 = D3DTA_TEXTURE; // オペレーションの引数1
	tssOp[0].alphaArg2 = D3DTA_DIFFUSE; // オペレーションの引数2
	//その他
	tssOp[0].texCoordIndex = 0; // テクスチャのインデックス
	tssOp[0].texTureTransFormFlags = D3DTTFF_DISABLE; // テクスチャ変換
	tssOp[0].resultArg = D3DTA_CURRENT; // 結果の格納先
	tssOp[0].constant = D3DCOLOR_ARGB(255, 55, 100, 100); // 定数値 (D3DTA_CONSTANTを使用時)

	// ステージ 1 以降を無効化
	for (unsigned int nCnt = 1; nCnt < TEXTURE_MAX; nCnt++)
	{
		tssOp[nCnt].colorOp = D3DTOP_DISABLE; // カラーオペレーション
		tssOp[nCnt].alphaOp = D3DTOP_DISABLE; // アルファオペレーション
	}

	RETURN_HR(SetDrawOperation(&rsOp, &ssOp[0], TEXTURE_MAX, &tssOp[0], TEXTURE_MAX)) //描画情報を設定

	// デバッグ用表示フォントの生成
	D3DXCreateFont
	(
		g_pD3DDevice, 18, 0, FW_NORMAL, 1,             // 高さ, 幅, 太さ
		FALSE, DEFAULT_CHARSET,                        // 日本語環境なら SHIFTJIS_CHARSET でも可
		OUT_TT_PRECIS,                                 // TrueTypeフォント優先
		CLEARTYPE_QUALITY,                             // アンチエイリアス有効
		DEFAULT_PITCH | FF_DONTCARE,                   // 通常
		"MS Gothic", &g_pFont                          // フォント名をより確実なものに
	);

	//オブジェクトの初期化処理
	RETURN_HR(InitKeyboard(hInstanse, hWnd));
	RETURN_HR(InitMouse(hInstanse, hWnd));
	RETURN_HR(InitdJoypad(hInstanse, hWnd));
	RETURN_HR(InitJoypad());
	RETURN_HR(InitSound(hWnd));
	InitCamera();
	InitLight();
	InitFade(g_mode);

	return S_OK;
}

//------------------------------
//終了処理
//------------------------------
void Uninit(void)
{
	UninitKeyboard();
	UninitMouse();
	UninitdJoypad();
	UninitJoypad();
	UninitSound();
	UninitCamera();
	UninitLight();
	UninitFade();

	//デバッグフォント破棄
	SAFE_RELEASE(g_pFont);

	//デバイス破棄
	SAFE_RELEASE(g_pD3DDevice);

	//オブジェクト破棄
	SAFE_RELEASE(g_pD3D);
}

//------------------------------
//更新処理
//------------------------------
void Update(void)
{
	UpdateKeyboard();
	UpdateMouse();
	UpdatedJoypad();
	UpdateJoypad();
	UpdateCamera();
	UpdateLight();
	UpdateFade();

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;
	case MODE_GAME:
		UpdateGame();
		break;
	case MODE_RESULT:
		UpdateResult();
		break;
	case MODE_RANK:
		break;
	}
}

//------------------------------
//描画処理
//------------------------------
void Render(void)
{
	Camera* pCamera = GetCamera();

	for (int nCount = 0; nCount < CAMERA_XNUM * CAMERA_YNUM; nCount++, pCamera++)
	{
		g_pD3DDevice->SetViewport(&pCamera->viewport);

		g_pD3DDevice->Clear//フロントバッファのクリア
		(
			0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0
		);

		if (SUCCEEDED(g_pD3DDevice->BeginScene()))//描画開始
		{
			SetCamera(nCount);

			//オブジェクト描画
			switch (g_mode)
			{
			case MODE_TITLE:
				DrawTitle();
				break;
			case MODE_GAME:
				DrawGame();
				break;
			case MODE_RESULT:
				DrawResult();
				break;
			case MODE_RANK:
				break;
			}
			DrawFade();

#ifdef _DEBUG

			if (g_bDebug)
			{
				DrawDebug();
			}

#endif

			g_pD3DDevice->EndScene();//描画終了
		}
		//バックバッファに表示を切り替える
		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}
}

//------------------------
//モードの切り替え
//------------------------
void SetMode(MODE mode)
{
	//現在の画面の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;
	case MODE_GAME:
		UninitGame();
		break;
	case MODE_RESULT:
		UninitResult();
		break;
	case MODE_RANK:
		break;
	}

	g_mode = mode;

	//新しい画面の初期化処理
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		break;
	case MODE_GAME:
		InitGame();
		break;
	case MODE_RESULT:
		InitResult();
		break;
	case MODE_RANK:
		break;
	}
}

//--------------------------------
// 角度の正規化
//--------------------------------
float NormalizeAngle(float angle)
{
	while (angle > D3DX_PI)
	{
		angle -= D3DX_PI * 2.0f;
	}
	while (angle < -D3DX_PI)
	{
		angle += D3DX_PI * 2.0f;
	}
	return angle;
}

//---------------------------------
// 球座標
//---------------------------------
void SpherePos(D3DXVECTOR3& OutPos, D3DXVECTOR3 BasePos, D3DXVECTOR3 rot, float Length)
{
	OutPos.x = BasePos.x + sinf(rot.x) * sinf(rot.y) * Length;
	OutPos.y = BasePos.y + cosf(rot.x) * Length;
	OutPos.z = BasePos.z + sinf(rot.x) * cosf(rot.y) * Length;
}

//------------------------
//デバイスの取得
//------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//-----------------
//モードの取得
//-----------------
MODE GetMode(void)
{
	return g_mode;
}

//-----------------
//ハンドルの取得
//-----------------
HWND GethWnd(void)
{
	return g_hWnd;
}

//-----------------
//インスタンスハンドル取得
//-----------------
HINSTANCE GethInstanse(void)
{
	return g_hInstance;
}

//-----------------
// エラー
//-----------------
void Error(HRESULT hr)
{
	char msg[256];
	snprintf(msg, sizeof(msg), "失敗しました。\nHRESULT: 0x%08X", hr);
	MessageBox(g_hWnd, msg, "エラー", MB_OK | MB_ICONERROR);
	PostMessage(g_hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
}

//------------------
//マウス制限
//------------------
void ConfineCursorToWindow(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	rect.left += 10;
	rect.top += 10;
	rect.right -= 10;
	rect.bottom -= 10;

	MapWindowPoints(hWnd, NULL, (POINT*)&rect, 2);
	ClipCursor(&rect);
}

//-----------------
//マウス解放
//-----------------
void ReleaseCursor(void)
{
	ClipCursor(NULL); // 制限を解除
}

//-----------------
// マウス切替
//-----------------
void CursorSwitch(SWITCH Switch)
{
	if (Switch==ON)
	{
		ReleaseCursor();
		while (ShowCursor(TRUE) < 0);
	}
	else
	{
		while (ShowCursor(FALSE) >= 0);
		ConfineCursorToWindow(GethWnd());
	}
}

//-------------------------
// マウスカーソルの位置を取得するs
//-------------------------
POINT GetCursorPoint(void)
{
	POINT point;
	GetCursorPos(&point);
	// スクリーン座標をクライアント座標に変換する
	if (ScreenToClient(GethWnd(), &point))
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// DirectXのサイズを取得する
		float width = 0.0f;
		float height = 0.0f;
		IDirect3DSurface9* pBackBuffer = nullptr;
		if (SUCCEEDED(pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer)))
		{// バックバッファの取得
			D3DSURFACE_DESC desc;
			pBackBuffer->GetDesc(&desc);
			width = (float)desc.Width;
			height = (float)desc.Height;
			pBackBuffer->Release();  // 取得したら解放
		}
		else
		{
			width = (float)SCREEN_WIDTH;
			height = (float)SCREEN_HEIGHT;
		}

		// クライアント領域のサイズを取得
		RECT clientRect;
		GetClientRect(GethWnd(), &clientRect);

		// クライアント座標をDirectXの座標にスケーリング
		float scaleX = width / (clientRect.right - clientRect.left);
		float scaleY = height / (clientRect.bottom - clientRect.top);

		int fixedX = static_cast<int>(point.x * scaleX);
		int fixedY = static_cast<int>(point.y * scaleY);

		// 座標を範囲内に固定
		fixedX = max(0, min((int)width, fixedX));
		fixedY = max(0, min((int)height, fixedY));

		point.x = fixedX;
		point.y = fixedY;
	}

	return point;
}

//--------------------
//
//デバッグ用
//
//--------------------

//--------------------
//デバッグ表示
//--------------------
void DrawDebug(void)
{
	RECT rect = { 0,0,(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };
	char aStr[1024] = { '\0' };
	extern Camera g_camera[CAMERA_XNUM * CAMERA_YNUM];//カメラ参照

	//文字列の代入
	snprintf(&aStr[0], sizeof(aStr), "FPS:%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//文字列の代入
	snprintf(&aStr[0], sizeof(aStr), "カメラアングルY:%f\n", g_camera[0].rot.y);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//文字列の代入
	snprintf(&aStr[0], sizeof(aStr), "カメラアングルX:%f\n", g_camera[0].rot.x);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//文字列の代入
	snprintf(&aStr[0], sizeof(aStr), "視点:%f %f %f\n", g_camera[0].posV.x, g_camera[0].posV.y, g_camera[0].posV.z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//文字列の代入
	snprintf(&aStr[0], sizeof(aStr), "目標視点:%f %f %f\n", g_camera[0].posVDest.x, g_camera[0].posVDest.y, g_camera[0].posVDest.z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//文字列の代入
	snprintf(&aStr[0], sizeof(aStr), "注視点:%f %f %f\n", g_camera[0].posR.x, g_camera[0].posR.y, g_camera[0].posR.z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//文字列の代入
	snprintf(&aStr[0], sizeof(aStr), "目標注視点:%f %f %f\n", g_camera[0].posRDest.x, g_camera[0].posRDest.y, g_camera[0].posRDest.z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//文字列の代入
	snprintf(&aStr[0], sizeof(aStr), "距離:%f\n", g_camera[0].fDistance);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;
}

//*********************************************************************************************
// ベクトルをデバッグ表示する関数
//*********************************************************************************************
void DebugPrintVector(const char* label, D3DXVECTOR3 vec)
{
	char buffer[128];
	snprintf(buffer, sizeof(buffer), "%s: x=%.2f, y=%.2f, z=%.2f\n", label, vec.x, vec.y, vec.z);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// 数値をデバッグ表示する関数
//*********************************************************************************************
void DebugPrintNumber(const char* label, int nNumber)
{
	char buffer[128];
	snprintf(buffer, sizeof(buffer), "%s: %d\n", label, nNumber);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// 数値をデバッグ表示する関数
//*********************************************************************************************
void DebugPrintBool(const char* label, bool bTrue)
{
	char buffer[128];
	snprintf(buffer, sizeof(buffer), "%s: %s\n", label, bTrue ? "true" : "false");
	OutputDebugString(buffer);
}

//*********************************************************************************************
// floatをデバッグ表示する関数
//*********************************************************************************************
void DebugPrintFloat(const char* label, float fNumber)
{
	char buffer[128];
	snprintf(buffer, sizeof(buffer), "%s: %.2f\n", label, fNumber);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// D3DXMATRIXをデバッグ表示する関数
//*********************************************************************************************
void DebugPrintMatrix(const char* label, D3DXMATRIX mat)
{
	char buffer[512];
	snprintf(buffer, sizeof(buffer),
		"%s:\n"
		"[%.2f, %.2f, %.2f, %.2f]\n"
		"[%.2f, %.2f, %.2f, %.2f]\n"
		"[%.2f, %.2f, %.2f, %.2f]\n"
		"[%.2f, %.2f, %.2f, %.2f]\n",
		label,
		mat._11, mat._12, mat._13, mat._14,
		mat._21, mat._22, mat._23, mat._24,
		mat._31, mat._32, mat._33, mat._34,
		mat._41, mat._42, mat._43, mat._44);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// 文字列をデバッグ表示する関数
//*********************************************************************************************
void DebugPrintString(const char* label, const char* str)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s: %s\n", label, str);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// 2Dベクトルをデバッグ表示する関数
//*********************************************************************************************
void DebugPrintVector2(const char* label, D3DXVECTOR2 vec)
{
	char buffer[128];
	snprintf(buffer, sizeof(buffer), "%s: x=%.2f, y=%.2f\n", label, vec.x, vec.y);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// 4Dベクトルをデバッグ表示する関数
//*********************************************************************************************
void DebugPrintVector4(const char* label, D3DXVECTOR4 vec)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s: x=%.2f, y=%.2f, z=%.2f, w=%.2f\n", label, vec.x, vec.y, vec.z, vec.w);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// カラーをデバッグ表示する関数
//*********************************************************************************************
void DebugPrintColor(const char* label, D3DXCOLOR color)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s: r=%.2f, g=%.2f, b=%.2f, a=%.2f\n", label, color.r, color.g, color.b, color.a);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// D3DXQUATERNIONをデバッグ表示する関数
//*********************************************************************************************
void DebugPrintQuaternion(const char* label, D3DXQUATERNION quat)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s: x=%.2f, y=%.2f, z=%.2f, w=%.2f\n", label, quat.x, quat.y, quat.z, quat.w);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// HRESULTをデバッグ表示する関数
//*********************************************************************************************
void DebugPrintHRESULT(const char* label, HRESULT hr)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s: 0x%08X\n", label, hr);
	OutputDebugString(buffer);
}