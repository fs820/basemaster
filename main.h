//------------------------------------------
//
//メインの処理の定義・宣言[main.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_

#define DIRECTINPUT_VERSION 0x0800
#define NOMINMAX  // 旧max,min を無効化 sato
#include <windows.h>
#include <algorithm>      // 新min,maxを有効化 sato
using std::clamp;         // clamp
using std::max;           // max
using std::min;           // min
#include<stdio.h>
#include<time.h>
#include <strsafe.h>
#include <shellscalingapi.h>
#include <shlwapi.h>
#include"d3dx9.h"
#include"dinput.h" // 入力に必要なファイル
#include"Xinput.h" // Xinput(パッド処理)
#include"xaudio2.h"

// Debug sato
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

// ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"uuid.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Shcore.lib")


// マクロ定義
#define FPS (60) // 更新スピード

// 名前
#define CLASS_NAME "WindowClass" // クラス名
#define WINDOW_NAME "C_Bese_Master" // ウインドウ名
#define DATA_FILE "data\\DATA\\data.bin"//スコア保存用ファイル

// 大きさ
#define SCREEN_WIDTH (2560.0f)  // 幅
#define SCREEN_HEIGHT (1440.0f) // 高さ

// 頂点
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX8) // 頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX8) // 頂点フォーマット
#define VT_MAX (4)//頂点数
#define UV_DEF (1.0f) // テクスチャのデフォルトのサイズ
#define TEXTURE_MAX (8) // テクスチャの最大数

#define ROT_RAND ((float)(rand() % 629) - 314.0f) * 0.01f

// コントローラー
#define	ELE_CON "GP30X-Dinput"//学校支給コントローラ
#define PS_CON "Wireless Controller"//プレステ4
#define NIN_CON "Pro Controller"//プロコン

//デバッグ用
#define FPS_TIME (500)
#define FONT_SIZE (20)

// 基本値
#define NOR D3DXVECTOR3(0.0f, 0.0f, -1.0f) // 基本法線ベクトル

// 処理マクロ
#define FOR(Max) for(int i = 0; i < Max; i++)
#define FOR_NAME(Name, Max) for(int Name = 0; Name < Max; Name++)
#define FOR_SIZE(Size, Max) for(Size i = 0; i < Max; i++)
#define FOR_SIZE_NAME(Size, Name, Max) for(Size Name = 0; Name < Max; Name++)
#define FOR_MIN(Min, Max) for(int i = Min; i < Max; i++)
#define FOR_NAME_MIN(Name, Min, Max) for(int Name = Min; Name < Max; Name++)
#define FOR_SIZE_MIN(Size, Min, Max) for(Size i = Min; i < Max; i++)
#define FOR_SIZE_NAME_MIN(Size, Name, Min, Max) for(Size Name = Min; Name < Max; Name++)

// エラーチェック用マクロ
#define RETURN_HR(a) HRESULT hr = a; if (FAILED(hr)) { Error(hr); return hr; }
#define CHECK_HR(a) HRESULT hr = a; if (FAILED(hr)) { Error(hr); }
#define SAFE_RELEASE(p) { if ((p) != nullptr) { (p)->Release(); (p) = nullptr; } }

// 画面の種類
typedef enum
{
	MODE_TITLE = 0,//タイトル
	MODE_TUTO,//チュートリアル
	MODE_GAME,//ゲーム
	MODE_RESULT,//リザルト
	MODE_RANK,//ランキング
	MODE_OPTION,//オプション
	MODE_MAX//モード数
} MODE;

//Switch
typedef enum
{
	OFF = 0,
	ON,
	SWITCH_MAX
}SWITCH;

//文字列構造体 sato
typedef struct
{
	char aName[64]; //ファイルパス
}String;

// 頂点情報の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;    // 座標
	float rhw;          // 座標変換係数(1.0f)
	D3DCOLOR col;       // カラー
	D3DXVECTOR2 tex[8]; // テクスチャ座標
} VERTEX_2D;

// 頂点情報の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;    // 座標
	D3DXVECTOR3 nor;    //法線ベクトル
	D3DCOLOR col;       // カラー
	D3DXVECTOR2 tex[8]; // テクスチャ座標
} VERTEX_3D;

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // ウインドウプロシージャ
HRESULT Init(HINSTANCE hInstanse, HWND hWnd, BOOL bWindow = TRUE); // 初期化処理
void Uninit(void); // 終了処理
void Update(void); // 更新処理
void Render(void); // 描画処理
LPDIRECT3DDEVICE9 GetDevice(void); // デバイスの取得
void SetMode(MODE mode);//モード設定
MODE GetMode(void);//モード取得
HWND GethWnd(void);//ハンドル取得
HINSTANCE GethInstanse(void);//インスタンス取得

void Error(HRESULT hr);// エラー

void ConfineCursorToWindow(HWND hwnd);//カーソル制御
void ReleaseCursor(void);//カーソル制御解除
void CursorSwitch(SWITCH Switch); // カーソルの切替

POINT GetCursorPoint(void); // カーソルの位置を取得する

// 汎用関数
float NormalizeAngle(float angle);
void SpherePos(D3DXVECTOR3& OutPos, D3DXVECTOR3 BasePos, D3DXVECTOR3 rot, float Length);

//デバッグ用
void DrawDebug(void);

// ログにデバック表示する関数郡
void DebugPrintVector(const char* label, D3DXVECTOR3 vec);         // ベクトルをデバッグ表示する関数
void DebugPrintNumber(const char* label, int nNumber);             // 数値をデバッグ表示する関数
void DebugPrintBool(const char* label, bool bTrue);                // 真偽をデバック表示する関数
void DebugPrintFloat(const char* label, float fNumber);            // floatをデバッグ表示する関数
void DebugPrintMatrix(const char* label, D3DXMATRIX mat);          // D3DXMATRIXをデバッグ表示する関数
void DebugPrintString(const char* label, const char* str);         // 文字列をデバッグ表示する関数
void DebugPrintVector2(const char* label, D3DXVECTOR2 vec);        // 2Dベクトルをデバッグ表示する関数
void DebugPrintVector4(const char* label, D3DXVECTOR4 vec);        // 4Dベクトルをデバッグ表示する関数
void DebugPrintColor(const char* label, D3DXCOLOR color);          // カラーをデバッグ表示する関数
void DebugPrintQuaternion(const char* label, D3DXQUATERNION quat); // D3DXQUATERNIONをデバッグ表示する関数
void DebugPrintHRESULT(const char* label, HRESULT hr);             // HRESULTをデバッグ表示する関数

#endif // _MAIN_H_