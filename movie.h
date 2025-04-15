//------------------------------------
//
//動画再生処理の定義・宣言[movie.h]
//Author
//
//------------------------------------

#include"main.h"

#define MOVIE_TEST L"data/MOVIE/Test.mp4"

//プロトタイプ宣言
void InitMovie(void);   // 初期化処理
void UninitMovie(void); // 終了処理
void UpdateMovie(void); // 更新処理
void DrawMovie(void);   // 描画処理
HRESULT PlayMovie(HWND hwnd, const wchar_t* videoFilePath);