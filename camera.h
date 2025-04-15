//---------------------------------------
//
//カメラの定義・宣言[camera.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include"main.h"

#define CAMERA_XNUM (1) //カメラの数X
#define CAMERA_YNUM (1) //カメラの数Y
#define CAMERA_VIEW_SPEED (0.01f)//回転スピード
#define CAMERA_ROT_SPEED (0.05f)//回転スピード
#define CAMERA_SPEED (2.0f)//移動スピード
#define CAMERA_INA (0.05f)//追従倍率
#define CAMERA_ROTX (0.2f)//縦回転範囲
#define CAMERA_ROTX_GRA (0.01f)//縦回転地面スペース
#define CAMERA_DISTANCE_SPEED (5.0f)
#define CAMERA_DISTANCE_MIN (5.0f)//ズーム制限
#define CAMERA_DISTANCE_MAX (100.0f)//ズーム制限
#define CAMERA_DISTANCE_VIEW_MAX (1000.0f)
#define CAMERA_WIDTH (90.0f)//視野の広さ
#define CAMERA_MIN (1.0f)//視野の最小
#define CAMERA_MAX (1000.0f)//視野の最大

//カメラ構造体
typedef struct
{
	D3DXVECTOR3 posV;
	D3DXVECTOR3 posVDest;
	D3DXVECTOR3 posR;
	D3DXVECTOR3 posRDest;
	D3DXVECTOR3 posU;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	float fDistance;
	float fDistanceMin;
	float fDistanceMax;
	D3DVIEWPORT9 viewport;
}Camera;

void InitCamera(void);//初期化処理
void UninitCamera(void);//終了処理
void UpdateCamera(void);//更新処理
void SetCamera(int nNumber);//描画処理
Camera* GetCamera(void);
void ViewCamera(int nNumber);
void GameCamera(int nNumber);

HRESULT DeptSort(int* nIdx, D3DXVECTOR3* pPos, int IdxNum);	// 遠い順インデックス作成関数
bool DrawCameraCollision(D3DXVECTOR3 pos);		            // カメラから見てさえぎられているか
#endif // _CAMERA_H_