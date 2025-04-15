//---------------------------------------
//
//隔離バリア処理の定義・宣言[isolationfield.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ISOLATION_FIELD_H_
#define _ISOLATION_FIELD_H_

#include"main.h"
#include"barrier.h"

#define ISOLATION_FIELD_FILE "data\\txt\\sato\\Isolation.txt" //最大数
#define ISOLATION_FIELD_MAX (16) //最大数
#define ISOLATION_FACE_MAX (8)   //面の最大数

#define ISOLATION_WIDTH_MIN (10.0f)
#define ISOLATION_WIDTH_MAX (1000.0f)
#define ISOLATION_HEIGHT_MIN (10.0f)
#define ISOLATION_HEIGHT_MAX (1000.0f)
#define ISOLATION_DEPTH_MIN (10.0f)
#define ISOLATION_DEPTH_MAX (1000.0f)

//隔離バリアの列挙型定義
typedef enum
{
	ISO_TYPE_1 = 0,
	ISO_TYPE_MAX
}ISO_TYPE;

//隔離バリア構造体
typedef struct
{
	D3DXVECTOR3 pos;                    //位置
	D3DXVECTOR3 rot;                    //向き
	float Width;                        //幅
	float Height;                       //高さ
	float Depth;                        //奥行
	float Corner;                       //角
	ISO_TYPE Type;                      //タイプ
	SWITCH Switch;                      //Switch
	bool bUse;                          //使用
}IsolationField;

void InitIsolationField(void);                                                                                                 //初期化処理
void UninitIsolationField(void);                                                                                               //終了処理
void UpdateIsolationField(void);                                                                                               //更新処理
void DrawIsolationField(void);                                                                                                 //描画処理
int SetIsolationField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, float Depth, float Corner, ISO_TYPE Type);  //設定
IsolationField* GetIsolationField(void);                                                                                       //取得
void IsolationFieldSwitch(ISO_TYPE Type, SWITCH Switch);                                                                       //オンオフ

void UpdateIsolationFieldEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot); //隔離バリアエディター
void SaveIsolationField(void); //書き込み
void LoadIsolationField(void); //読み込み

int GetIsonumber(void);    //タイプナンバー
int GetIsoDelnumber(void); //デリートナンバー
#endif _ISOLATION_FIELD_H_