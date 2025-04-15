//-----------------------------------------
//
//見えない壁処理の定義・宣言[gamemanager.h
//
//-----------------------------------------

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include"main.h"

// ゲーム進行管理
typedef enum
{
	EVENT_START = 0,          // 始まり
	EVENTSTATE_MAX
}EVENTSTATE;

//プロトタイプ宣言
void GameManager(void);                // ゲームマネージャー
void StartEvent(void);                 // イベントスタート
void EndEvent(void);                   // イベント終了
void SetEvent(EVENTSTATE Event);       // イベント切り替え
EVENTSTATE GetEvent(void);             // イベント取得
void LinkBarrier(void);                // バリア設定
void LinkVirtualObjectCollision(void); // オブジェクトに当たり判定を付与する

#endif _GAMEMANAGER_H_