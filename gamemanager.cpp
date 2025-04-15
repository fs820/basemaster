//----------------------------------------
//
//ゲーム管理処理[gamemanager.cpp]
//Author fuma sato
//
//----------------------------------------
#include"gamemanager.h"
#include"game.h"
#include"player.h"
#include"item.h"
#include"interact.h"
#include"barrier.h"
#include"isolationfield.h"
#include"object.h"
#include"virtualcollision.h"
#include "sound.h"
#include <chrono>
#include "meshfield.h"
#include "wall.h"
#include "light.h"
#include "input.h"

// グローバル変数
EVENTSTATE g_EventState = EVENT_START; // ゲーム進行管理
SOUND_LABEL g_NowBgm;
int g_CountEvent = 0; // イベントカウント
bool g_bStage[3] = { false }; // ステージの到着フラグ
//------------------------
//ゲームマネージャー
//------------------------
void GameManager(void)
{
	extern MODE g_mode; // モード参照
	extern Player g_player; // プレイヤー参照

	//インタラクトタイプ 切替
	if (IsJoyPadUsed())
	{//コントローラー入力
		SetInteractType(INTERACT_TYPE_JOY);
	}
	else if (IsKeyboardUsed() || IsMouseUsed())
	{//キーボード入力
		SetInteractType(INTERACT_TYPE_KEY);
	}

	switch (g_mode)
	{
	case MODE_GAME:
		//ゲーム進行管理
		switch (g_EventState)
		{
		case EVENT_START:
			break;
		}
	}
}

//---------------------
//イベントスタート
//---------------------
void StartEvent(void)
{
	SetEvent(EVENT_START);
}

//---------------------
//イベントスタート
//---------------------
void EndEvent(void)
{
	SetEvent(EVENT_START);
}

//--------------------------------
//イベント切り替え
//--------------------------------
void SetEvent(EVENTSTATE Event)
{
	g_EventState = Event;
}

//-----------------------
//イベント取得
//-----------------------
EVENTSTATE GetEvent(void)
{
	return g_EventState;
}

//---------------------
//バリアの設定
//---------------------
void LinkBarrier(void)
{
	extern Object g_aObject[OBJECT_MAX]; //オブジェクト参照

}

//------------------------------------
//オブジェクトに当たり判定を付与する
//------------------------------------
void LinkVirtualObjectCollision(void)
{
	extern Object g_aObject[OBJECT_MAX]; //オブジェクト参照

}