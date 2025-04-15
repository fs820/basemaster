//------------------------------------------
//
//ポーズ処理の定義・宣言[pause.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _PAUSE_H_
#define _PAUSE_H_

#include"main.h"

#define TEXTURE_RESTART "data\\TEXTURE\\Base.jpg"
#define TEXTURE_RETRY "data\\TEXTURE\\Base.jpg"
#define TEXTURE_BACKTITLE "data\\TEXTURE\\Base.jpg"
#define TEXTURE_EXIT "data\\TEXTURE\\Base.jpg"

void InitPause(void);//ゲームの初期化処理
void UninitPause(void);//ゲームの終了処理
void UpdatePause(void);//ゲームの更新処理
void DrawPause(void);//ゲームの描画処理
void Pause(void);//ポーズ
bool bPause(void);//ポーズかどうか

#endif _PAUSE_H_