//---------------------------------------
//
//オブジェクト処理の定義・宣言[object.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include"main.h"
#include"game.h"

#define OMANAGER_FILE "data\\txt\\sato\\objectManager.txt"
#define OBJECT_FILE   "data\\txt\\sato\\object.txt"

#define OBJECT_MAX (64)          //最大数
#define OBJECT_SUBSET_MAX (4)    //サブセット数
#define OBJECT_TEXTURE_MAX (9)   //テクスチャ数
#define OBJECT_SCALE_MAX (20.0f) //大きさの最大
#define OBJECT_SCALE_MIN (0.5f)  //大きさの最小

#define ROKET_TRIGGER_SPACE (300.0f) //トリガースペース

//オブジェクトタイプの列挙型定義
typedef enum
{
	OTYPE_TEST = 0,
	OTYPE_MAX
}OTYPE;

//オブジェクトモデル構造体
typedef struct
{
	String Pass;                                      //ファイルパスの格納
	LPD3DXMESH pMesh;                                 //メッシュ
	LPD3DXBUFFER pBuffMat;                            //マテリアル
	DWORD dwNumMat;                                   //マテリアル数
	int TexIdx[OBJECT_SUBSET_MAX];                  //マテリアルインデックス
	D3DXVECTOR3 vtxMin;                               //最小
	D3DXVECTOR3 vtxMax;                               //最大
	int nNumVtx;                                      //頂点数
	DWORD sizeFVF;                                    //サイズ
	BYTE* pVtxBuff;                                   //バッファのポインタ
}ObjectModel;

//オブジェクトマネージャー構造体
typedef struct
{
	int ObjectNum;                      //読み込むファイル数
	ObjectModel ObjectModel[OTYPE_MAX]; //ファイルの格納
	LPDIRECT3DTEXTURE9 apTexture[OBJECT_TEXTURE_MAX]; //テクスチャ
}ObjectManager;

//オブジェクト構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//回転
	float fLength;			//長さ
	float fHeight;			//長さ
	D3DXVECTOR3 scale;		//大きさ
	D3DXMATRIX mtxWorld;	//マトリックス
	int nIdxShadow[2];		//影
	OTYPE Type;				//タイプ識別子
	bool bAlpha;			//透明
	bool bUse;				//使用
	int nIdxRadar;			// レーダー
}Object;

void InitObject(void);    //初期化処理
void UninitObject(void);  //終了処理
void UpdateObject(void);  //更新処理
void DrawObject(void);    //描画処理
void SetObjectManager(ObjectManager ObjectManager);                             //読み込んだファイル情報を格納する
int SetObject(OTYPE Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale); //オブジェクトを設置する
Object* GetObject_EX(void);       //オブジェクトの取得
int GetObjectNum(void);           //種類数の取得
int GetObjectIdx(OTYPE Type, int Number);  //インデックスの取得

bool CollisionObject(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius, float fHeight, Object** pObject); //当たり判定
bool TriggerObject(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld,  float fRadius);                                                       //当たり判定
bool InsideObject(OTYPE Type, D3DXVECTOR3 pos, Object** pObject);                                                                                    //当たり判定

void LoadObjectManager(void); //ファイル情報読み込み
void LoadObject(void);        //設置情報読み込み
void SaveObject(void);        //設置情報書き出し

char* GetObjectName(int nNumber);                          //ファイルパスの取得(Debug表示用)
void UpdateObjectEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot); //オブジェクトエディター
int GetOnumber(void);                                      //現在のXファイルナンバー
int GetODelnumber(void);                                   //現在のデリートナンバー

#endif //_OBJECT_H_