#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"
#include "model.h"

#define MOTION_KEY_MAX (16)			// キーの最大数
#define MOTION_KEYINFO_MAX (32)		// キーインフォの最大数
#define MOTION_MOTION_INFO_MAX (15)	// モーションインフォ最大数

// キーの構造体
typedef struct
{
	float fPosX;	// 位置X
	float fPosY;	// 位置Y
	float fPosZ;	// 位置Z
	float fRotX;	// 向きX
	float fRotY;	// 向きY
	float fRotZ;	// 向きZ
}KEY;

// キー情報の構造体
typedef struct
{
	int nFrame;					// 再生フレーム
	KEY aKey[MOTION_KEY_MAX];	// 各パーツのキー要素
}KEY_INFO;

// キーモーション情報構造体
typedef struct
{
	bool bLoop;				                // ループするかどうか
	int nNumKey;			                // キーの総数
	KEY_INFO aKeyinfo[MOTION_KEYINFO_MAX];	// キー情報
}MOTION_INFO;

typedef struct
{
	MOTION_INFO aMotionInfo[MOTION_MOTION_INFO_MAX];	// モーション情報
	int nNumMotion;					                    // モーションの総数
}MOTION_SOURCE;

typedef struct
{
	MOTION_INFO MotionInfo;         // 今のモーション情報

	int nNowMotiType;				// 現在のモーションタイプ
	int nNumKey;					// キーの総数
	bool bLoop;                     // ループするかどうか
	int nKey;						// 現在のキーNo.
	int nNextKey;	                // 次のキーNo.
	int nCounterKey;				// キーカウンター
	int nCounterMotion;				// モーションカウンター
	bool MotionEnd;                 // モーションが終わっているか


	MOTION_INFO MotionInfoBlend;    // ブレンド先のモーション情報

	int nNowMotiTypeBlend;		    // ブレンド先のモーションタイプ
	int nNumKeyBlend;				// ブレンド先のキーの総数
	bool bLoopBlend;                // ブレンド先がループするかどうか
	int nKeyBlend;					// ブレンド先の現在のキーNo.
	int nNextKeyBlend;	            // ブレンド先の次のキーNo.
	int nCounterKeyBlend;		    // ブレンドキーカウンター
	int nCounterMotionBlend;		// ブレンドモーションカウンター
	bool MotionEndBlend;            // ブレンド先のモーションが終わっているか

	bool bBlend;                    // ブレンドするか
	int nBlendFrame;                // ブレンドフレーム
	int nBlendCounter;              // ブレンドカウンター
}MOTION;

// キャラクターパラメーター
typedef struct
{
	float fWidth;	  // 幅
	float fHeigt;	  // 高さ
	float fJamp;	  // ジャンプ力
	float fMovement;  // 移動量
	float fgraviment; // 重力
	int nNumModel;	  // モデル数
}CHARPARAM_SOURCE;

// キャラクターパラメーター
typedef struct
{
	int nLife;		   // 体力
	int nCondition;	   // 状態
	int nCntCondition; // 状態管理カウンター
	bool bJanp;		   // ジャンプしているかどうか
	bool bDisp;		   // 表示しますか
}CHARPARAM;

typedef struct
{
	int nCntMotionInfo;		//	モーション情報をカウントするもの
	int nCntKeyInfo;		//	キーモーションの情報をカウントするもの
	int nCntKey;			//	キーをカウントするもの
}SET_MOTION_MANAGER;

// プロトタイプ宣言
void InitMotion(void);	// モーション初期化
void ReadMotionText(const char* pFileName,Model *pModel, LPDIRECT3DTEXTURE9* pTex, MOTION_SOURCE* motion, CHARPARAM_SOURCE*charParam);	// モーションテキスト読み込み
void ReadScript(FILE* pFile, Model* pModel, MOTION_SOURCE* motion, LPDIRECT3DTEXTURE9* pTex, CHARPARAM_SOURCE* charParam);				// スクリプトの読み込み
void ReadCharacterSet(FILE* pFile, Model* pModel, MOTION_SOURCE* motion, CHARPARAM_SOURCE* charParam);
bool UpdateMotion(MOTION* motion, ModelMatrix* modelmtx, Model* model, int nNumModel, float Angle);
void SetMotion(MOTION* motion, MOTION_SOURCE* motionsource, int nType, bool bBlend, int BlendFrame);	// モーションの切替

void MostionSet(FILE* pFile, MOTION_SOURCE* motion);
void MostionSet(MOTION_SOURCE* motion, MOTION_INFO setMotInfo);
void KeySet(FILE* pFile, MOTION_SOURCE* motion);
void EndKeySet(MOTION_SOURCE* motion, KEY_INFO keyInfo);
void Key(FILE* pFile, MOTION_SOURCE* motion);
void EndKey(MOTION_SOURCE* motion, KEY key);

void StringClear(char* astr, int nMaxCunt);
void ReadComment(FILE *pFile, char* aResult);
#endif // !_MOTION_H_