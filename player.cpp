//----------------------------------------
//
//モデル表示処理[player.cpp]
//Author fuma sato
//
//----------------------------------------

#include"player.h"
#include"game.h"
#include"shadow.h"
#include"camera.h"
#include"input.h"
#include"bullet.h"
#include"file.h"
#include"particle.h"

//グローバル変数宣言
Player g_Player;
//----------------------
//ポリゴンの初期化処理
//----------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	g_Player.pos = D3DXVECTOR3(START_X, 0.1f, START_Z);
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_Player.Destrot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_Player.scale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	g_Player.bJump = false;
	g_Player.nLife = PLAYER_LIFE;
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.nIdxShadow = -1;
	g_Player.nIdxLife = -1;
	g_Player.pObject = NULL;

	g_Player.bLoopMotion = false;
	g_Player.motionType = MOTIONTYPE_NEUTRAL;
	g_Player.nCounterMotion = 0;
	g_Player.nKey = -1;
	g_Player.nNumKey = 0;
	g_Player.nNumMotion = NUM_MOTION;
	g_Player.bUse = true;

	//Xファイル読み込み
	D3DXLoadMeshFromX
	(
		PLAYER_BODY_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[0].pBuffMat,
		NULL,
		&g_Player.aModel[0].dwNumMat,
		&g_Player.aModel[0].pMesh
	);

	//Xファイル読み込み
	D3DXLoadMeshFromX
	(
		PLAYER_HEAD_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[1].pBuffMat,
		NULL,
		&g_Player.aModel[1].dwNumMat,
		&g_Player.aModel[1].pMesh
	);

	//Xファイル読み込み
	D3DXLoadMeshFromX
	(
		PLAYER_RIGHTARM_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[2].pBuffMat,
		NULL,
		&g_Player.aModel[2].dwNumMat,
		&g_Player.aModel[2].pMesh
	);

	//Xファイル読み込み
	D3DXLoadMeshFromX
	(
		PLAYER_RIGHTHAND_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[3].pBuffMat,
		NULL,
		&g_Player.aModel[3].dwNumMat,
		&g_Player.aModel[3].pMesh
	);

	//Xファイル読み込み
	D3DXLoadMeshFromX
	(
		PLAYER_LEFTARM_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[4].pBuffMat,
		NULL,
		&g_Player.aModel[4].dwNumMat,
		&g_Player.aModel[4].pMesh
	);

	//Xファイル読み込み
	D3DXLoadMeshFromX
	(
		PLAYER_LEFTHAND_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[5].pBuffMat,
		NULL,
		&g_Player.aModel[5].dwNumMat,
		&g_Player.aModel[5].pMesh
	);

	//Xファイル読み込み
	D3DXLoadMeshFromX
	(
		PLAYER_RIGHTLEG_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[6].pBuffMat,
		NULL,
		&g_Player.aModel[6].dwNumMat,
		&g_Player.aModel[6].pMesh
	);

	//Xファイル読み込み
	D3DXLoadMeshFromX
	(
		PLAYER_RIGHTFOOT_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[7].pBuffMat,
		NULL,
		&g_Player.aModel[7].dwNumMat,
		&g_Player.aModel[7].pMesh
	);

	//Xファイル読み込み
	D3DXLoadMeshFromX
	(
		PLAYER_LEFTLEG_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[8].pBuffMat,
		NULL,
		&g_Player.aModel[8].dwNumMat,
		&g_Player.aModel[8].pMesh
	);

	//Xファイル読み込み
	D3DXLoadMeshFromX
	(
		PLAYER_LEFTFOOT_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[9].pBuffMat,
		NULL,
		&g_Player.aModel[9].dwNumMat,
		&g_Player.aModel[9].pMesh
	);

	//Xファイル読み込み
	D3DXLoadMeshFromX
	(
		PLAYER_HAT_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[10].pBuffMat,
		NULL,
		&g_Player.aModel[10].dwNumMat,
		&g_Player.aModel[10].pMesh
	);

	g_Player.nNumModel = NUM_MODEL;

	int nCntModel;
	for (nCntModel = 0;  nCntModel < g_Player.nNumModel; nCntModel++)
	{
		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Player.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャがある
				//テクスチャの読み込み
				D3DXCreateTextureFromFile
				(
					pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_Player.aModel[nCntModel].apTexture[nCntMat]
				);
			}
		}

		//頂点数
		g_Player.aModel[nCntModel].nNumVtx = g_Player.aModel[nCntModel].pMesh->GetNumVertices();
		//頂点サイズ
		g_Player.aModel[nCntModel].sizeFVF = D3DXGetFVFVertexSize(g_Player.aModel[nCntModel].pMesh->GetFVF());
		//頂点バッファの取得
		g_Player.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_Player.aModel[nCntModel].pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < g_Player.aModel[nCntModel].nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_Player.aModel[nCntModel].pVtxBuff;

			if (vtx.x > g_Player.aModel[nCntModel].vtxMax.x)
			{
				g_Player.aModel[nCntModel].vtxMax.x = vtx.x;
			}
			if (vtx.x < g_Player.aModel[nCntModel].vtxMin.x)
			{
				g_Player.aModel[nCntModel].vtxMin.x = vtx.x;
			}
			if (vtx.y > g_Player.aModel[nCntModel].vtxMax.y)
			{
				g_Player.aModel[nCntModel].vtxMax.y = vtx.y;
			}
			if (vtx.y < g_Player.aModel[nCntModel].vtxMin.y)
			{
				g_Player.aModel[nCntModel].vtxMin.y = vtx.y;
			}
			if (vtx.z > g_Player.aModel[nCntModel].vtxMax.z)
			{
				g_Player.aModel[nCntModel].vtxMax.z = vtx.z;
			}
			if (vtx.z < g_Player.aModel[nCntModel].vtxMin.z)
			{
				g_Player.aModel[nCntModel].vtxMin.z = vtx.z;
			}

			g_Player.aModel[nCntModel].pVtxBuff += g_Player.aModel[nCntModel].sizeFVF;
		}

		g_Player.aModel[nCntModel].vtxMinDef = g_Player.aModel[nCntModel].vtxMin;
		g_Player.aModel[nCntModel].vtxMaxDef = g_Player.aModel[nCntModel].vtxMax;
	}

	g_Player.aModel[0].nIdxModelParent = -1;
	g_Player.aModel[0].pos = D3DXVECTOR3(0.0f, 21.0f, 0.0f);
	g_Player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.aModel[0].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Player.aModel[1].nIdxModelParent = 0;
	g_Player.aModel[1].pos = D3DXVECTOR3(0.0f, 24.0f, 0.0f);
	g_Player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.aModel[1].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Player.aModel[2].nIdxModelParent = 0;
	g_Player.aModel[2].pos = D3DXVECTOR3(-7.5f, 19.5f, 0.0f);
	g_Player.aModel[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.aModel[2].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Player.aModel[3].nIdxModelParent = 2;
	g_Player.aModel[3].pos = D3DXVECTOR3(-7.5f, 0.0f, 0.0f);
	g_Player.aModel[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.aModel[3].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Player.aModel[4].nIdxModelParent = 0;
	g_Player.aModel[4].pos = D3DXVECTOR3(7.5f, 19.5f, 0.0f);
	g_Player.aModel[4].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.aModel[4].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Player.aModel[5].nIdxModelParent = 4;
	g_Player.aModel[5].pos = D3DXVECTOR3(7.5f, 0.0f, 0.0f);
	g_Player.aModel[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.aModel[5].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Player.aModel[6].nIdxModelParent = 0;
	g_Player.aModel[6].pos = D3DXVECTOR3(-6.0f, 0.0f, 0.0f);
	g_Player.aModel[6].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.aModel[6].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Player.aModel[7].nIdxModelParent = 6;
	g_Player.aModel[7].pos = D3DXVECTOR3(0.0f, -6.0f, 0.0f);
	g_Player.aModel[7].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.aModel[7].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Player.aModel[8].nIdxModelParent = 0;
	g_Player.aModel[8].pos = D3DXVECTOR3(6.0f, 0.0f, 0.0f);
	g_Player.aModel[8].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.aModel[8].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Player.aModel[9].nIdxModelParent = 8;
	g_Player.aModel[9].pos = D3DXVECTOR3(0.0f, -6.0f, 0.0f);
	g_Player.aModel[9].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.aModel[9].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Player.aModel[10].nIdxModelParent = 1;
	g_Player.aModel[10].pos = D3DXVECTOR3(0.0f, 15.0f, 0.0f);
	g_Player.aModel[10].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.aModel[10].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		g_Player.aModel[nCnt].posDef = g_Player.aModel[nCnt].pos;
	}

	g_Player.nIdxShadow = SetShadow(g_Player.pos);
}

//-------------------
//ポリゴン終了処理
//-------------------
void UninitPlayer(void)
{
	int nCntModel;
	for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		for (unsigned int i = 0; i < g_Player.aModel[nCntModel].dwNumMat; i++)
		{
			if (g_Player.aModel[nCntModel].apTexture[i] != NULL)
			{
				g_Player.aModel[nCntModel].apTexture[i]->Release();
				g_Player.aModel[nCntModel].apTexture[i] = NULL;
			}
		}
		//メッシュの破棄
		if (g_Player.aModel[nCntModel].pMesh != NULL)
		{
			g_Player.aModel[nCntModel].pMesh->Release();
			g_Player.aModel[nCntModel].pMesh = NULL;
		}
		//マテリアルの破棄
		if (g_Player.aModel[nCntModel].pBuffMat != NULL)
		{
			g_Player.aModel[nCntModel].pBuffMat->Release();
			g_Player.aModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//-------------------
//ポリゴン更新処理
//-------------------
void UpdatePlayer(void)
{
	if (g_Player.bUse)
	{
		Camera* pCamera = GetCamera();
		float* pStick;

		//移動処理

		//X
		if (IsXInputControllerConnected(CONTROLLER_1))
		{
			//左スティック処理
			pStick = GetJoyStick(STICK_LEFT, CONTROLLER_1);
			if (sqrtf(*pStick * *pStick + *(pStick + 1) * *(pStick + 1)) >= STICK_DED)
			{
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f + -atan2f(*(pStick + 1), *pStick)) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f + -atan2f(*(pStick + 1), *pStick)) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * -0.5f + -atan2f(*(pStick + 1), *pStick);
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
		}
		//d
		else if (IsDirectInputControllerConnected(CONTROLLER_1))
		{

			//左スティック処理
			pStick = GetdJoyStick(STICK_LEFT, CONTROLLER_1);
			if (sqrtf(*pStick * *pStick + *(pStick + 1) * *(pStick + 1)) >= STICK_DED)
			{
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f + atan2f(*(pStick + 1), *pStick)) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f + atan2f(*(pStick + 1), *pStick)) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * -0.5f + atan2f(*(pStick + 1), *pStick);
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
		}

		if (GetKeyboardPress(DIK_A) == true || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) == true)//Aキー
		{
			if (GetKeyboardPress(DIK_W) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//Wキー
			{//左上
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * -0.25f) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * -0.25f) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * 0.75f;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//Sキー
			{//左下
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * -0.75f) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * -0.75f) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * 0.25f;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
			else
			{//左
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * -0.5f) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * -0.5f) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * 0.5f;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
		}
		else if (GetKeyboardPress(DIK_D) == true || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) == true)//Dキー
		{
			if (GetKeyboardPress(DIK_W) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//Wキー
			{//右上
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * -0.75f;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//Sキー
			{//右下
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * -0.25f;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
			else
			{//右
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * -0.5f;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
		}
		else if (GetKeyboardPress(DIK_W) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//Wキー
		{//上
			g_Player.move.x += sinf(pCamera->rot.y) * SPEED;
			g_Player.move.z += cosf(pCamera->rot.y) * SPEED;
			g_Player.Destrot.y = pCamera->rot.y + D3DX_PI;
			if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
			{
				g_Player.motionType = MOTIONTYPE_MOVE;
			}
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//Sキー
		{//下
			g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI) * SPEED;
			g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI) * SPEED;
			g_Player.Destrot.y = pCamera->rot.y;
			if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
			{
				g_Player.motionType = MOTIONTYPE_MOVE;
			}
		}

		if (GetKeyboardTrigger(DIK_SPACE) == true || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_1) == true)//Sキー
		{//上
			if (!g_Player.bJump)
			{
				g_Player.move.y += JUMP;
				g_Player.bJump = true;
				g_Player.motionType = MOTIONTYPE_JUMP;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetMouseTrigger(MOUSE_LEFT) == true || GetJoykeyTrigger(JOYKEY_X, CONTROLLER_1) == true)
		{
			g_Player.motionType = MOTIONTYPE_ACTION;
		}

		if (!IsXInputControllerConnected(CONTROLLER_1) && IsDirectInputControllerConnected(CONTROLLER_1))
		{
			if (GetdJoyPov(POV_UP, 0, CONTROLLER_1) == true)
			{
				g_Player.move.x += sinf(pCamera->rot.y) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
			else if (GetdJoyPov(POV_RIGHTUP, 0, CONTROLLER_1) == true)
			{
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * -0.75f;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
			else if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_1) == true)
			{
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * -0.5f;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
			else if (GetdJoyPov(POV_RIGHTDOWN, 0, CONTROLLER_1) == true)
			{
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * -0.25f;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
			else if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_1) == true)
			{
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
			else if (GetdJoyPov(POV_LEFTDOWN, 0, CONTROLLER_1) == true)
			{
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * -0.75f) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * -0.75f) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * 0.25f;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
			else if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_1) == true)
			{
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * -0.5f) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * -0.5f) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * 0.5f;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
			else if (GetdJoyPov(POV_LEFTUP, 0, CONTROLLER_1) == true)
			{
				g_Player.move.x += sinf(pCamera->rot.y + D3DX_PI * -0.25f) * SPEED;
				g_Player.move.z += cosf(pCamera->rot.y + D3DX_PI * -0.25f) * SPEED;
				g_Player.Destrot.y = pCamera->rot.y + D3DX_PI * 0.75f;
				if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionType != MOTIONTYPE_LANDING && g_Player.motionType != MOTIONTYPE_ACTION)
				{
					g_Player.motionType = MOTIONTYPE_MOVE;
				}
			}
		}

		if (!strcmp(ControllerName(CONTROLLER_1), ELE_CON))
		{
			if (GetdJoykeyPress(ELEKEY_A, CONTROLLER_1) == true)
			{//上
				if (!g_Player.bJump)
				{
					g_Player.move.y += JUMP;
					g_Player.bJump = true;
					g_Player.motionType = MOTIONTYPE_JUMP;
				}
			}

			if (GetdJoykeyTrigger(ELEKEY_X, CONTROLLER_1) == true)
			{
				g_Player.motionType = MOTIONTYPE_ACTION;
			}
		}
		else if (!strcmp(ControllerName(CONTROLLER_1), PS_CON))
		{
			if (GetdJoykeyPress(PSKEY_CR, CONTROLLER_1) == true)
			{//上
				if (!g_Player.bJump)
				{
					g_Player.move.y += JUMP;
					g_Player.bJump = true;
					g_Player.motionType = MOTIONTYPE_JUMP;
				}
			}

			if (GetdJoykeyTrigger(PSKEY_SQ, CONTROLLER_1) == true)
			{
				g_Player.motionType = MOTIONTYPE_ACTION;
			}
		}
		else if (!strcmp(ControllerName(CONTROLLER_1), NIN_CON))
		{
			if (GetdJoykeyPress(NINKEY_B, CONTROLLER_1) == true)
			{//上
				if (!g_Player.bJump)
				{
					g_Player.move.y += JUMP;
					g_Player.bJump = true;
					g_Player.motionType = MOTIONTYPE_JUMP;
				}
			}

			if (GetdJoykeyTrigger(NINKEY_Y, CONTROLLER_1) == true)
			{
				g_Player.motionType = MOTIONTYPE_ACTION;
			}
		}
		else if (!IsXInputControllerConnected(CONTROLLER_1) && IsDirectInputControllerConnected(CONTROLLER_1))
		{
			if (GetdJoykeyPress(DKEY_A, CONTROLLER_1) == true)
			{//上
				if (!g_Player.bJump)
				{
					g_Player.move.y += JUMP;
					g_Player.bJump = true;
					g_Player.motionType = MOTIONTYPE_JUMP;
				}
			}

			if (GetdJoykeyTrigger(DKEY_X, CONTROLLER_1) == true)
			{
				g_Player.motionType = MOTIONTYPE_ACTION;
			}
		}

		g_Player.move.y += GRAVITY;

		if (g_Player.bJump)
		{
			g_Player.move.x *= JUMP_INA;
			g_Player.move.z *= JUMP_INA;
		}

		g_Player.posOld = g_Player.pos;

		if (g_Player.motionType == MOTIONTYPE_ACTION)
		{
			g_Player.move.x = 0.0f;
			g_Player.move.z = 0.0f;
		}

		//位置の更新
		g_Player.pos.x += g_Player.move.x;
		g_Player.pos.y += g_Player.move.y;
		g_Player.pos.z += g_Player.move.z;

		//移動量の更新(減衰)
		g_Player.move.x += (SPEED_DEF - g_Player.move.x) * PLAYER_INA;
		g_Player.move.z += (SPEED_DEF - g_Player.move.z) * PLAYER_INA;

		g_Player.pObject = NULL;

		if (CollisionObject(&g_Player.pos, &g_Player.posOld, &g_Player.move, 50.0f, 100.0f, &g_Player.pObject) == true && g_Player.bJump == true)
		{
			g_Player.bJump = false;
		}

		if (g_Player.pObject != NULL)
		{
			
		}

		if (g_Player.pos.x < -GAME_WALL)
		{
			g_Player.pos.x = -GAME_WALL;
		}
		if (g_Player.pos.x > GAME_WALL)
		{
			g_Player.pos.x = GAME_WALL;
		}
		if (g_Player.pos.z < -GAME_WALL)
		{
			g_Player.pos.z = -GAME_WALL;
		}
		if (g_Player.pos.z > GAME_WALL)
		{
			g_Player.pos.z = GAME_WALL;
		}

		if (sqrtf(g_Player.move.x * g_Player.move.x + g_Player.move.z * g_Player.move.z) < 0.1f && g_Player.motionType == MOTIONTYPE_MOVE)
		{
			g_Player.move.x = 0.0f;
			g_Player.move.z = 0.0f;
			g_Player.motionType = MOTIONTYPE_NEUTRAL;
		}

		//正規化
		if (g_Player.Destrot.y > D3DX_PI)
		{
			g_Player.Destrot.y -= D3DX_PI * 2.0f;
		}
		else if (g_Player.Destrot.y < -D3DX_PI)
		{
			g_Player.Destrot.y += D3DX_PI * 2.0f;
		}

		//正規化
		if (g_Player.Destrot.y - g_Player.rot.y > D3DX_PI)
		{
			g_Player.Destrot.y = g_Player.rot.y + ((g_Player.Destrot.y - g_Player.rot.y) - D3DX_PI * 2.0f);
		}
		else if (g_Player.Destrot.y - g_Player.rot.y < -D3DX_PI)
		{
			g_Player.Destrot.y = g_Player.rot.y + ((g_Player.Destrot.y - g_Player.rot.y) + D3DX_PI * 2.0f);
		}

		g_Player.rot.y += (g_Player.Destrot.y - g_Player.rot.y) * ROT_SPEED;

		//正規化
		if (g_Player.rot.y > D3DX_PI)
		{
			g_Player.rot.y -= D3DX_PI * 2.0f;
		}
		else if (g_Player.rot.y < -D3DX_PI)
		{
			g_Player.rot.y += D3DX_PI * 2.0f;
		}

		//正規化
		if (g_Player.Destrot.x > D3DX_PI)
		{
			g_Player.Destrot.x -= D3DX_PI * 2.0f;
		}
		else if (g_Player.Destrot.x < -D3DX_PI)
		{
			g_Player.Destrot.x += D3DX_PI * 2.0f;
		}

		//正規化
		if (g_Player.Destrot.x - g_Player.rot.x > D3DX_PI)
		{
			g_Player.Destrot.x = g_Player.rot.x + ((g_Player.Destrot.x - g_Player.rot.x) - D3DX_PI * 2.0f);
		}
		else if (g_Player.Destrot.x - g_Player.rot.x < -D3DX_PI)
		{
			g_Player.Destrot.x = g_Player.rot.x + ((g_Player.Destrot.x - g_Player.rot.x) + D3DX_PI * 2.0f);
		}

		g_Player.rot.x += (g_Player.Destrot.x - g_Player.rot.x) * ROT_SPEED;

		//正規化
		if (g_Player.rot.x > D3DX_PI)
		{
			g_Player.rot.x -= D3DX_PI * 2.0f;
		}
		else if (g_Player.rot.x < -D3DX_PI)
		{
			g_Player.rot.x += D3DX_PI * 2.0f;
		}

		//スケール制限
		g_Player.scale.x = max(SCALE_MIN, min(SCALE_MAX, g_Player.scale.x));
		g_Player.scale.y = max(SCALE_MIN, min(SCALE_MAX, g_Player.scale.y));
		g_Player.scale.z = max(SCALE_MIN, min(SCALE_MAX, g_Player.scale.z));

		//パーツ分回す
		FOR_NAME (nCntModel,g_Player.nNumModel)
		{
			g_Player.aModel[nCntModel].vtxMax.x = g_Player.aModel[nCntModel].vtxMaxDef.x * g_Player.scale.x;
			g_Player.aModel[nCntModel].vtxMax.y = g_Player.aModel[nCntModel].vtxMaxDef.y * g_Player.scale.y;
			g_Player.aModel[nCntModel].vtxMax.z = g_Player.aModel[nCntModel].vtxMaxDef.z * g_Player.scale.z;
			g_Player.aModel[nCntModel].vtxMin.x = g_Player.aModel[nCntModel].vtxMinDef.x * g_Player.scale.x;
			g_Player.aModel[nCntModel].vtxMin.y = g_Player.aModel[nCntModel].vtxMinDef.y * g_Player.scale.y;
			g_Player.aModel[nCntModel].vtxMin.z = g_Player.aModel[nCntModel].vtxMinDef.z * g_Player.scale.z;
		}

		if (g_Player.motionType == MOTIONTYPE_ACTION && g_Player.nKey == 3 && g_Player.nCounterMotion == 11)
		{
			SetBullet(g_Player.pos + g_Player.aModel[0].pos, D3DXVECTOR3(sinf(g_Player.rot.y + g_Player.aModel[0].rot.y + g_Player.aModel[2].rot.y + g_Player.aModel[3].rot.y) * cosf(g_Player.rot.x + g_Player.aModel[0].rot.x + g_Player.aModel[2].rot.x + g_Player.aModel[3].rot.x), sinf(g_Player.rot.x + g_Player.aModel[0].rot.x + g_Player.aModel[2].rot.x + g_Player.aModel[3].rot.x), cosf(g_Player.rot.y + g_Player.aModel[0].rot.y + g_Player.aModel[2].rot.y + g_Player.aModel[3].rot.y) * cosf(g_Player.rot.x + g_Player.aModel[0].rot.x + g_Player.aModel[2].rot.x + g_Player.aModel[3].rot.x)), g_Player.aModel[3].scale);
		}

		switch (g_Player.state)
		{
		case PLAYERSTATE_APPEAR:
			g_Player.bUse = false;
			break;
		case PLAYERSTATE_NORMAL:
			if (g_Player.nLife <= 0)
			{
				g_Player.state = PLAYERSTATE_DIE;
			}
			SetPositionShadow(g_Player.nIdxShadow, g_Player.pos, g_Player.scale, 200.0f);
			break;
		case PLAYERSTATE_DIE:
			g_Player.Destrot.x = D3DX_PI * 0.5f;
			static int nCnt = 0;
			nCnt++;
			if (nCnt >= 20)
			{
				NullShadow(g_Player.nIdxShadow);
				SetParticle(g_Player.pos, g_Player.scale);
				g_Player.state = PLAYERSTATE_APPEAR;
			}
			break;
		}
	}
}

//-------------------
//ポリゴン描画処理
//-------------------
void DrawPlayer(void)
{
	if (g_Player.bUse)
	{
		LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
		D3DXMATRIX mtxRot, mtxTrans, mtxScale;//計算マトリックス
		D3DMATERIAL9 matDef;//現在のマテリアル保存用
		D3DXMATERIAL* pMat;//マテリアルデータへのポインタ


		//デバイスの取得
		pDevice = GetDevice();

		//マトリックス初期化
		D3DXMatrixIdentity(&g_Player.mtxWorld);

		//大きさの反映
		D3DXMatrixScaling(&mtxScale, g_Player.scale.x, g_Player.scale.y, g_Player.scale.z);
		D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxScale);

		//向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
		D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

		//位置の反映
		D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
		D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

		int nCntModel;
		//パーツ分回す
		for (nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel, mtxScaleModel;//計算マトリックス
			D3DXMATRIX mtxParent;
			//マトリックス初期化
			D3DXMatrixIdentity(&g_Player.aModel[nCntModel].mtxWorld);

			//大きさの反映
			D3DXMatrixScaling(&mtxScaleModel, g_Player.aModel[nCntModel].scale.x, g_Player.aModel[nCntModel].scale.y, g_Player.aModel[nCntModel].scale.z);
			D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld, &g_Player.aModel[nCntModel].mtxWorld, &mtxScaleModel);

			//向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Player.aModel[nCntModel].rot.y, g_Player.aModel[nCntModel].rot.x, g_Player.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld, &g_Player.aModel[nCntModel].mtxWorld, &mtxRotModel);

			//位置の反映
			D3DXMatrixTranslation(&mtxTransModel, g_Player.aModel[nCntModel].pos.x, g_Player.aModel[nCntModel].pos.y, g_Player.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld, &g_Player.aModel[nCntModel].mtxWorld, &mtxTransModel);

			//パーツの親のマトリックスの設定
			if (g_Player.aModel[nCntModel].nIdxModelParent != -1)
			{//親モデルがある
				mtxParent = g_Player.aModel[g_Player.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{//親モデルがない
				mtxParent = g_Player.mtxWorld;
			}

			D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld, &g_Player.aModel[nCntModel].mtxWorld, &mtxParent);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntModel].mtxWorld);

			//現在のマテリアル取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Player.aModel[nCntModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				//テクスチャ
				pDevice->SetTexture(0, g_Player.aModel[nCntModel].apTexture[nCntMat]);

				//モデル描画
				g_Player.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}

			pDevice->SetMaterial(&matDef);
		}
	}
}

//------------------------------
//位置取得
//------------------------------
Player* GetPlayer(void)
{
	return &g_Player;
}