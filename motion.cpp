#include "motion.h"

// グローバル変数宣言
SET_MOTION_MANAGER g_SetMotiMane;	// モーションカウント情報

//************************************************
// モーションの初期化
//************************************************
void InitMotion(void)
{
	g_SetMotiMane.nCntMotionInfo = 0;
	g_SetMotiMane.nCntKeyInfo = 0;
	g_SetMotiMane.nCntKey = 0;
}

//************************************************
// モーションテキストの読み込み	SCRIPT
//************************************************
void ReadMotionText(const char* pFileName, Model* pModel, LPDIRECT3DTEXTURE9* pTex, MOTION_SOURCE* motion, CHARPARAM_SOURCE* charParam)
{
	char astrA[256] = {};		// 文字列格納用変数
	char astrB[256] = {};		// 古い格納用変数
	char* aResult = 0;
	int nResult = 0;
	int nCnt = 0;
	// ファイルを開く
	FILE* pFile = fopen(pFileName, "r");

	if (pFile == NULL)
	{
		return;
	}

	while (1)
	{	
		// 一文字読み取る
		aResult = fgets(&astrA[0],2, pFile);
		
		// 一文字ずつ代入
		astrB[nCnt] = astrA[0];

		// カウントを進める,上限超えたらゼロ
		nCnt = (nCnt + 1) % 256;
		
		if (strcmp(&astrA[0], "#") == 0)
		{// コメント文字を読んだらそのあとの文字は読み飛ばす
			ReadComment(pFile, aResult);	// コメントを読み飛ばす
			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrA[0], "\n") == 0)
		{// 改行よんだら
			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrA[0], "\t") == 0)
		{// 改行よんだら

			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "SCRIPT") == 0)
		{// SCRIPT読み取り
			ReadScript(pFile, pModel, motion, pTex, charParam);	// スクリプトの読み込みに以降
		}

		if (aResult == NULL)
		{// テキストファイル全部読み切っちゃった
			break;
		}
		else if (nResult == EOF)
		{// テキストファイル全部読み切っちゃった
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);
}

//************************************************
// スクリプトの読み取り
//************************************************
void ReadScript(FILE* pFile, Model* pModel, MOTION_SOURCE* motion, LPDIRECT3DTEXTURE9* pTex, CHARPARAM_SOURCE* charParam)
{
	char astrA[256] = {};		// 文字列格納用変数
	char astrB[256] = {};	// 古い格納用変数
	char *aResult = 0;
	int nResult = 0;
	int nCnt = 0;
	int nCntModel = 0;

	while (1)
	{
		// 一文字読み取る
		aResult = fgets(&astrA[0], 2, pFile);

		// 一文字ずつ代入
		astrB[nCnt] = astrA[0];

		// カウントを進める,上限超えたらゼロ
		nCnt = (nCnt + 1) % 256;

		if (strcmp(&astrA[0], "#") == 0)
		{// コメント文字を読んだらそのあとの文字は読み飛ばす
			ReadComment(pFile, aResult);	// コメントを読み飛ばす
			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrA[0], "\n") == 0)
		{// 改行よんだら
			
			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrA[0], "\t") == 0)
		{// 改行よんだら

			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "NUM_MODEL") == 0)
		{// モデル数の読み込み
			fgets(&astrA[0],3,pFile);								// =読み込み
			nResult = fscanf(pFile, "%d", &charParam->nNumModel);	// モデル数に代入
			StringClear(&astrB[0], 256);							// 保存用文字列のクリア
			nCnt = 0;												// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "MODEL_FILENAME") == 0)
		{// MODEL_FILENAME読み取り
			fgets(&astrA[0], 3, pFile);					// =読み込み
			nResult = fscanf(pFile, "%s", &astrA[0]);	// ファイルパス読み込み
			SetModel(&astrA[0], pModel, pTex, nCntModel);		// Xフィルのパスの読み込み
			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;									// 保存用文字列のカウントゼロ
			nCntModel++;								// モデルカウントを進める
		}
		else if (strcmp(&astrB[0], "CHARACTERSET") == 0)
		{// CHARACTERSET読み取り
			ReadCharacterSet(pFile, pModel, motion, charParam);
			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;									// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "MOTIONSET") == 0)
		{// MOTIONSET読み取り
			MostionSet(pFile, motion);		// モーションセットへ以降
			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;									// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "END_SCRIPT") == 0)
		{// END_SCRIPT読み取り
			break;
		}

		if (aResult == NULL)
		{// テキストファイル全部読み切っちゃった
			break;
		}
		else if (nResult == EOF)
		{// テキストファイル全部読み切っちゃった
			break;
		}
	}

	// カウントしている変数を初期化
	g_SetMotiMane.nCntMotionInfo = 0;
	g_SetMotiMane.nCntKeyInfo = 0;
	g_SetMotiMane.nCntKey = 0;
}

//************************************************
// キャラクターセットの読み込み
//************************************************
void ReadCharacterSet(FILE* pFile, Model* pModel, MOTION_SOURCE* motion, CHARPARAM_SOURCE* charParam)
{
	char astrA[256] = {};	// 文字列格納用変数
	char astrB[256] = {};	// 古い格納用変数
	char* aResult = 0;
	int nResult = 0;
	int nCnt = 0;

	while (1)
	{
		// 一文字読み取る
		aResult = fgets(&astrA[0], 2, pFile);

		// 一文字ずつ代入
		astrB[nCnt] = astrA[0];

		// カウントを進める,上限超えたらゼロ
		nCnt = (nCnt + 1) % 256;

		if (strcmp(&astrA[0], "#") == 0)
		{// コメント文字を読んだらそのあとの文字は読み飛ばす
			ReadComment(pFile, aResult);	// コメントを読み飛ばす
			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrA[0], "\n") == 0)
		{// 改行よんだら

			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrA[0], "\t") == 0)
		{// 改行よんだら

			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "NUM_PARTS") == 0)
		{// パーツ数の読み込み
			fgets(&astrA[0], 3, pFile);					// =読み込み
			nResult = fscanf(pFile, "%d", &charParam->nNumModel);	// モデル数に代入
			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;									// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "RADIUS") == 0)
		{// モデル数の読み込み
			fgets(&astrA[0], 3, pFile);							// =読み込み
			nResult = fscanf(pFile, "%f", &charParam->fWidth);	// モデル数に代入
			StringClear(&astrB[0], 256);						// 保存用文字列のクリア
			nCnt = 0;											// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "HEIGHT") == 0)
		{// モデル数の読み込み
			fgets(&astrA[0], 3, pFile);							// =読み込み
			nResult = fscanf(pFile, "%f", &charParam->fHeigt);	// モデル数に代入
			StringClear(&astrB[0], 256);						// 保存用文字列のクリア
			nCnt = 0;											// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "PARTSSET") == 0)
		{// パーツを設定
			PartsSet(pFile,pModel);
			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "END_CHARACTERSET") == 0)
		{// パーツを設定
			break;
		}

		if (aResult == NULL)
		{// テキストファイル全部読み切っちゃった
			break;
		}
		else if (nResult == EOF)
		{// テキストファイル全部読み切っちゃった
			break;
		}
	}
}

//************************************************
// モーションセット
//************************************************
void MostionSet(FILE* pFile, MOTION_SOURCE* motion)
{
	char astrA[256] = {};	// 文字列格納用変数
	char astrB[256] = {};	// 古い格納用変数
	char* aResult = 0;
	int nResult = 0;
	int nCnt = 0;

	MOTION_INFO setMotInfo;
	setMotInfo.bLoop = false;
	setMotInfo.nNumKey = 0;

	while (1)
	{
		// 一文字読み取る
		aResult = fgets(&astrA[0], 2, pFile);

		// 一文字ずつ代入
		astrB[nCnt] = astrA[0];

		// カウントを進める,上限超えたらゼロ
		nCnt = (nCnt + 1) % 256;

		if (strcmp(&astrA[0], "#") == 0)
		{// コメント文字を読んだらそのあとの文字は読み飛ばす
			ReadComment(pFile, aResult);	// コメントを読み飛ばす
			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrA[0], "\n") == 0)
		{// 改行よんだら

			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrA[0], "\t") == 0)
		{// 改行よんだら

			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "LOOP") == 0)
		{// ループ情報の読み込み
			int nFlag = 0;
			fgets(&astrA[0], 3, pFile);					// =読み込み
			nResult = fscanf(pFile, "%d", &nFlag);		// モデル数に代入

			if (nFlag > 0)
			{// ゼロより上だったらトゥルー
				setMotInfo.bLoop = true;
			}

			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;									// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "NUM_KEY") == 0)
		{// キー総数の読み込み
			fgets(&astrA[0], 3, pFile);					// =読み込み
			nResult = fscanf(pFile, "%d", &setMotInfo.nNumKey);		// モデル数に代入
			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;
		}
		else if (strcmp(&astrB[0], "KEYSET") == 0)
		{// キーセットの読み込み
			KeySet(pFile, motion);	// キーセット開始
			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;
		}
		else if (strcmp(&astrB[0], "END_MOTIONSET") == 0)
		{// モーションセット終わり
			MostionSet(motion,setMotInfo);	// 取得した情報を全部ぶち込む
			g_SetMotiMane.nCntMotionInfo++;	// キーセット番号追加

			g_SetMotiMane.nCntKeyInfo = 0;	// キー情報カウントリセット
			break;
		}

		if (aResult == NULL)
		{// テキストファイル全部読み切っちゃった
			break;
		}
		else if (nResult == EOF)
		{// テキストファイル全部読み切っちゃった
			break;
		}
	}
}

//************************************************
// モーション終わりー
//************************************************
void MostionSet(MOTION_SOURCE* motion, MOTION_INFO setMotInfo)
{
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].bLoop = setMotInfo.bLoop;
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].nNumKey = setMotInfo.nNumKey;
}

//************************************************
// キーセットの設定
//************************************************
void KeySet(FILE* pFile, MOTION_SOURCE* motion)
{
	char astrA[256] = {};	// 文字列格納用変数
	char astrB[256] = {};	// 古い格納用変数
	char* aResult = 0;
	int nResult = 0;
	int nCnt = 0;

	KEY_INFO keyInfo;
	keyInfo.nFrame = 1;

	while (1)
	{
		// 一文字読み取る
		aResult = fgets(&astrA[0], 2, pFile);

		// 一文字ずつ代入
		astrB[nCnt] = astrA[0];

		// カウントを進める,上限超えたらゼロ
		nCnt = (nCnt + 1) % 256;

		if (strcmp(&astrA[0], "#") == 0)
		{// コメント文字を読んだらそのあとの文字は読み飛ばす
			ReadComment(pFile, aResult);	// コメントを読み飛ばす
			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrA[0], "\n") == 0)
		{// 改行よんだら

			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrA[0], "\t") == 0)
		{// 改行よんだら

			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "FRAME") == 0)
		{// フレーム数の読み込み
			fgets(&astrA[0], 3, pFile);					// =読み込み
			nResult = fscanf(pFile, "%d", &keyInfo.nFrame);	// モデル数に代入
			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;									// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "KEY") == 0)
		{// キーの読み込み
			Key(pFile, motion);
			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;									// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "END_KEYSET") == 0)
		{// キーの読み込み
			EndKeySet(motion,keyInfo);		// 取得した情報をすべてぶち込む
			g_SetMotiMane.nCntKeyInfo++;	// キーを一つ進める
			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;

			g_SetMotiMane.nCntKey = 0;		// キーカウント数リセット
			break;
		}

		if (aResult == NULL)
		{// テキストファイル全部読み切っちゃった
			break;
		}
		else if (nResult == EOF)
		{// テキストファイル全部読み切っちゃった
			break;
		}
	}
}

//************************************************
// キーセット終わりー
//************************************************
void EndKeySet(MOTION_SOURCE* motion, KEY_INFO keyInfo)
{
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].nFrame = keyInfo.nFrame;
}

//************************************************
// キーを設定
//************************************************
void Key(FILE* pFile, MOTION_SOURCE* motion)
{
	char astrA[256] = {};	// 文字列格納用変数
	char astrB[256] = {};	// 古い格納用変数
	char* aResult = 0;
	int nResult = 0;
	int nCnt = 0;

	KEY key;
	key.fPosX = 0.0f;
	key.fPosY = 0.0f;
	key.fPosZ = 0.0f;
	key.fRotX = 0.0f;
	key.fRotY = 0.0f;
	key.fRotZ = 0.0f;

	while (1)
	{
		// 一文字読み取る
		aResult = fgets(&astrA[0], 2, pFile);

		// 一文字ずつ代入
		astrB[nCnt] = astrA[0];

		// カウントを進める,上限超えたらゼロ
		nCnt = (nCnt + 1) % 256;

		if (strcmp(&astrA[0], "#") == 0)
		{// コメント文字を読んだらそのあとの文字は読み飛ばす
			ReadComment(pFile, aResult);	// コメントを読み飛ばす
			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrA[0], "\n") == 0)
		{// 改行よんだら

			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrA[0], "\t") == 0)
		{// 改行よんだら

			StringClear(&astrB[0], 256);	// 保存用文字列のクリア
			nCnt = 0;						// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "POS") == 0)
		{// POSの読み込み
			fgets(&astrA[0], 3, pFile);					// =読み込み
			nResult = fscanf(pFile, "%f", &key.fPosX);	// 位置X
			nResult = fscanf(pFile, "%f", &key.fPosY);	// 位置Y
			nResult = fscanf(pFile, "%f", &key.fPosZ);	// 位置Z
			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;									// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "ROT") == 0)
		{// ROTの読み込み
			fgets(&astrA[0], 3, pFile);					// =読み込み
			nResult = fscanf(pFile, "%f", &key.fRotX);	// 向きX
			nResult = fscanf(pFile, "%f", &key.fRotY);	// 向きY
			nResult = fscanf(pFile, "%f", &key.fRotZ);	// 向きZ
			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;									// 保存用文字列のカウントゼロ
		}
		else if (strcmp(&astrB[0], "END_KEY") == 0)
		{// キー読み込みおーわり
			EndKey(motion,key);			// 取得した情報をすべてぶち込む
			g_SetMotiMane.nCntKey++;	// キーセット番目追加
			StringClear(&astrB[0], 256);				// 保存用文字列のクリア
			nCnt = 0;
			break;
		}

		if (aResult == NULL)
		{// テキストファイル全部読み切っちゃった
			break;
		}
		else if (nResult == EOF)
		{// テキストファイル全部読み切っちゃった
			break;
		}
	}
}

//************************************************
// キーの終わりー
//************************************************
void EndKey(MOTION_SOURCE* motion, KEY key)
{
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].aKey[g_SetMotiMane.nCntKey].fPosX = key.fPosX;
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].aKey[g_SetMotiMane.nCntKey].fPosY = key.fPosY;
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].aKey[g_SetMotiMane.nCntKey].fPosZ = key.fPosZ;

	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].aKey[g_SetMotiMane.nCntKey].fRotX = key.fRotX;
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].aKey[g_SetMotiMane.nCntKey].fRotY = key.fRotY;
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].aKey[g_SetMotiMane.nCntKey].fRotZ = key.fRotZ;
}

//************************************************
// 文字列のクリア
//************************************************
void StringClear(char* astr,int nMaxCunt)
{
	for (int nCntB = 0; nCntB < nMaxCunt; nCntB++)
	{// 保存している文字列をすべてクリア
		if (astr[nCntB] == '\0')
		{
			break;
		}

		astr[nCntB] = '\0';
	}
}

//************************************************
// コメント
//************************************************
void ReadComment(FILE *pFile,char*aResult)
{
	char astr[2];

	while (1)
	{
		// 一文字ずつ読む
		fgets(&astr[0], 2, pFile);

		if (*aResult = strcmp(&astr[0], "\n") == 0)
		{// 改行文字読んだら
			break;
		}

		if (*aResult == EOF)
		{// テキストファイル全部読み切っちゃった
			break;
		}
	}
}

//************************************************
// モーションの更新
//************************************************
bool UpdateMotion(MOTION* motion, ModelMatrix* modelmtx, Model* model, int nNumModel, float Angle)
{
	//今のモーション
	KEY nextKey[16] = {};                // 次のキー
	KEY nowKey[16] = {};                 // 今のキー
	KEY diffKey[16] = {};	             // キーの差分
	KEY Hope[16] = {};		             // 希望の値

	//ブレンド先のモーション
	KEY nextKeyBlend[16] = {};            // 次のキー
	KEY nowKeyBlend[16] = {};             // 今のキー
	KEY diffKeyBlend[16] = {};            // キーの差分
	KEY HopeBlend[16] = {};		          // 希望の値

	//結果
	KEY diffKeyDest[16] = {};	          // キーの差分
	KEY HopeDest[16] = {};		          // 希望の値

	if (motion->nNextKey < motion->nNumKey)
	{// モーションの全体キーより値が低い場合

		motion->nCounterMotion++;	// モーションカウント
		motion->nCounterKey++;		// キーカウント

		// 全モデルパーツの更新
		for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
		{
			nextKey[nCntModel] = motion->MotionInfo.aKeyinfo[motion->nNextKey].aKey[nCntModel]; // 次のキー
			nowKey[nCntModel] = motion->MotionInfo.aKeyinfo[motion->nKey].aKey[nCntModel];      // 今のキー

			// キーの差分(位置)を求める
			diffKey[nCntModel].fPosX = (nextKey[nCntModel].fPosX - nowKey[nCntModel].fPosX);
			diffKey[nCntModel].fPosY = (nextKey[nCntModel].fPosY - nowKey[nCntModel].fPosY);
			diffKey[nCntModel].fPosZ = (nextKey[nCntModel].fPosZ - nowKey[nCntModel].fPosZ);

			// キーの差分(向き)を求める
			diffKey[nCntModel].fRotX = (nextKey[nCntModel].fRotX - nowKey[nCntModel].fRotX);
			diffKey[nCntModel].fRotY = (nextKey[nCntModel].fRotY - nowKey[nCntModel].fRotY);
			diffKey[nCntModel].fRotZ = (nextKey[nCntModel].fRotZ - nowKey[nCntModel].fRotZ);

			// オフセットに足す値を求める
			Hope[nCntModel].fPosX = nowKey[nCntModel].fPosX + diffKey[nCntModel].fPosX * ((float)motion->nCounterKey / (float)motion->MotionInfo.aKeyinfo[motion->nKey].nFrame);
			Hope[nCntModel].fPosY = nowKey[nCntModel].fPosY + diffKey[nCntModel].fPosY * ((float)motion->nCounterKey / (float)motion->MotionInfo.aKeyinfo[motion->nKey].nFrame);
			Hope[nCntModel].fPosZ = nowKey[nCntModel].fPosZ + diffKey[nCntModel].fPosZ * ((float)motion->nCounterKey / (float)motion->MotionInfo.aKeyinfo[motion->nKey].nFrame);

			// 向きを求める
			Hope[nCntModel].fRotX = nowKey[nCntModel].fRotX + diffKey[nCntModel].fRotX * ((float)motion->nCounterKey / (float)motion->MotionInfo.aKeyinfo[motion->nKey].nFrame);
			Hope[nCntModel].fRotY = nowKey[nCntModel].fRotY + diffKey[nCntModel].fRotY * ((float)motion->nCounterKey / (float)motion->MotionInfo.aKeyinfo[motion->nKey].nFrame);
			Hope[nCntModel].fRotZ = nowKey[nCntModel].fRotZ + diffKey[nCntModel].fRotZ * ((float)motion->nCounterKey / (float)motion->MotionInfo.aKeyinfo[motion->nKey].nFrame);
		}

		if (motion->nCounterKey >= motion->MotionInfo.aKeyinfo[motion->nKey].nFrame)
		{// フレーム全部数え終わったら
			motion->nCounterKey = 0;          	    // キーカウントゼロ
			motion->nNextKey++;	                    // 次のキーへカウント
			motion->nKey++;		                    // 次のキーへカウント
			motion->nNextKey %= motion->nNumKey;	// キーの総数分に制限する
			motion->nKey %= motion->nNumKey;		// キーの総数分に制限する

			if (motion->nNextKey == 0 && motion->bLoop == false)
			{// ループしないモーションは終わらせる
				motion->nNextKey = motion->nNumKey;	// 計算の条件式に入らないようにする
				motion->nCounterMotion = 0;			// モーションカウントゼロ
				motion->MotionEnd = true;		    // モーション終わり
			}
		}
	}

	if (motion->bBlend == true && motion->nNextKeyBlend < motion->nNumKeyBlend)
	{// ブレンドするかつモーションの全体キーより値が低い場合
		motion->nCounterMotionBlend++;	// モーションカウント
		motion->nCounterKeyBlend++;		// キーカウント

		// 全モデルパーツの更新
		for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
		{
			nextKeyBlend[nCntModel] = motion->MotionInfoBlend.aKeyinfo[motion->nNextKeyBlend].aKey[nCntModel]; // 次のキー
			nowKeyBlend[nCntModel] = motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].aKey[nCntModel];      // 今のキー

			// キーの差分(位置)を求める
			diffKeyBlend[nCntModel].fPosX = (nextKeyBlend[nCntModel].fPosX - nowKeyBlend[nCntModel].fPosX);
			diffKeyBlend[nCntModel].fPosY = (nextKeyBlend[nCntModel].fPosY - nowKeyBlend[nCntModel].fPosY);
			diffKeyBlend[nCntModel].fPosZ = (nextKeyBlend[nCntModel].fPosZ - nowKeyBlend[nCntModel].fPosZ);

			// キーの差分(向き)を求める
			diffKeyBlend[nCntModel].fRotX = (nextKeyBlend[nCntModel].fRotX - nowKeyBlend[nCntModel].fRotX);
			diffKeyBlend[nCntModel].fRotY = (nextKeyBlend[nCntModel].fRotY - nowKeyBlend[nCntModel].fRotY);
			diffKeyBlend[nCntModel].fRotZ = (nextKeyBlend[nCntModel].fRotZ - nowKeyBlend[nCntModel].fRotZ);

			// オフセットに足す値を求める
			HopeBlend[nCntModel].fPosX = nowKeyBlend[nCntModel].fPosX + diffKeyBlend[nCntModel].fPosX * ((float)motion->nCounterKeyBlend / (float)motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame);
			HopeBlend[nCntModel].fPosY = nowKeyBlend[nCntModel].fPosY + diffKeyBlend[nCntModel].fPosY * ((float)motion->nCounterKeyBlend / (float)motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame);
			HopeBlend[nCntModel].fPosZ = nowKeyBlend[nCntModel].fPosZ + diffKeyBlend[nCntModel].fPosZ * ((float)motion->nCounterKeyBlend / (float)motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame);

			// 向きを求める
			HopeBlend[nCntModel].fRotX = nowKeyBlend[nCntModel].fRotX + diffKeyBlend[nCntModel].fRotX * ((float)motion->nCounterKeyBlend / (float)motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame);
			HopeBlend[nCntModel].fRotY = nowKeyBlend[nCntModel].fRotY + diffKeyBlend[nCntModel].fRotY * ((float)motion->nCounterKeyBlend / (float)motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame);
			HopeBlend[nCntModel].fRotZ = nowKeyBlend[nCntModel].fRotZ + diffKeyBlend[nCntModel].fRotZ * ((float)motion->nCounterKeyBlend / (float)motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame);
		}

		if (motion->nCounterKeyBlend >= motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame)
		{// フレーム全部数え終わったら
			motion->nCounterKeyBlend = 0;          	        // キーカウントゼロ
			motion->nNextKeyBlend++;	                    // 次のキーへカウント
			motion->nKeyBlend++;		                    // 次のキーへカウント
			motion->nNextKeyBlend %= motion->nNumKeyBlend;	// キーの総数分に制限する
			motion->nKeyBlend %= motion->nNumKeyBlend;		// キーの総数分に制限する

			if (motion->nNextKeyBlend == 0 && motion->bLoopBlend == false)
			{// ループしないモーションは終わらせる
				motion->nNextKeyBlend = motion->nNumKeyBlend;	// 計算の条件式に入らないようにする
				motion->nCounterMotionBlend = 0;			    // モーションカウントゼロ
				motion->MotionEndBlend = true;		            // モーション終わり
			}
		}
	}

	// 全モデルパーツの更新
	for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
	{
		if (motion->bBlend == true)
		{
			motion->nBlendCounter++; //ブレンドカウンター

			// キーの差分(位置)を求める
			diffKeyDest[nCntModel].fPosX = (HopeBlend[nCntModel].fPosX - Hope[nCntModel].fPosX);
			diffKeyDest[nCntModel].fPosY = (HopeBlend[nCntModel].fPosY - Hope[nCntModel].fPosY);
			diffKeyDest[nCntModel].fPosZ = (HopeBlend[nCntModel].fPosZ - Hope[nCntModel].fPosZ);

			// キーの差分(向き)を求める
			diffKeyDest[nCntModel].fRotX = (HopeBlend[nCntModel].fRotX - Hope[nCntModel].fRotX);
			diffKeyDest[nCntModel].fRotY = (HopeBlend[nCntModel].fRotY - Hope[nCntModel].fRotY);
			diffKeyDest[nCntModel].fRotZ = (HopeBlend[nCntModel].fRotZ - Hope[nCntModel].fRotZ);

			// オフセットに足す値を求める
			HopeDest[nCntModel].fPosX = Hope[nCntModel].fPosX + diffKeyDest[nCntModel].fPosX * ((float)motion->nBlendCounter / (float)motion->nBlendFrame);
			HopeDest[nCntModel].fPosY = Hope[nCntModel].fPosY + diffKeyDest[nCntModel].fPosY * ((float)motion->nBlendCounter / (float)motion->nBlendFrame);
			HopeDest[nCntModel].fPosZ = Hope[nCntModel].fPosZ + diffKeyDest[nCntModel].fPosZ * ((float)motion->nBlendCounter / (float)motion->nBlendFrame);

			// 向きを求める
			HopeDest[nCntModel].fRotX = Hope[nCntModel].fRotX + diffKeyDest[nCntModel].fRotX * ((float)motion->nBlendCounter / (float)motion->nBlendFrame);
			HopeDest[nCntModel].fRotY = Hope[nCntModel].fRotY + diffKeyDest[nCntModel].fRotY * ((float)motion->nBlendCounter / (float)motion->nBlendFrame);
			HopeDest[nCntModel].fRotZ = Hope[nCntModel].fRotZ + diffKeyDest[nCntModel].fRotZ * ((float)motion->nBlendCounter / (float)motion->nBlendFrame);

			// モデルの位置、向きを設定する
			modelmtx[nCntModel].pos = model[nCntModel].offSet + D3DXVECTOR3(HopeDest[nCntModel].fPosX, HopeDest[nCntModel].fPosY, HopeDest[nCntModel].fPosZ);
			modelmtx[nCntModel].rot = D3DXVECTOR3(HopeDest[nCntModel].fRotX, HopeDest[nCntModel].fRotY, HopeDest[nCntModel].fRotZ);
		}
		else if (motion->MotionEnd == false)
		{
			// モデルの位置、向きを設定する
			modelmtx[nCntModel].pos = model[nCntModel].offSet + D3DXVECTOR3(Hope[nCntModel].fPosX, Hope[nCntModel].fPosY, Hope[nCntModel].fPosZ);
			modelmtx[nCntModel].rot = D3DXVECTOR3(Hope[nCntModel].fRotX, Hope[nCntModel].fRotY, Hope[nCntModel].fRotZ);
		}

		if ((motion->bBlend == true || motion->MotionEnd == false) && model[nCntModel].nIdxModelParent == -1)
		{// 本体のポスが結びついてるやつがいたら
			modelmtx[nCntModel].rot.y += Angle;
		}
	}

	if (motion->bBlend == true && motion->nBlendCounter >= motion->nBlendFrame)
	{// ブレンドフレーム全部数え終わったら
		motion->nBlendCounter = 0;                      // ブレンドカウンターゼロ

		// ブレンド先を今のモーションにする
		motion->nNowMotiType = motion->nNowMotiTypeBlend;					  // タイプ
		motion->MotionInfo = motion->MotionInfoBlend;                         // モーション情報
		motion->nNumKey = motion->MotionInfo.nNumKey;	                      // キーの最大数
		motion->bLoop = motion->MotionInfo.bLoop;	                          // ループ
		motion->nKey = motion->nKeyBlend;									  // 今数えているキー
		motion->nNextKey = motion->nNextKeyBlend;                             // 次のキー
		motion->nCounterMotion = motion->nCounterMotionBlend;				  // フレームカウンター
		motion->nCounterKey = motion->nCounterKeyBlend;						  // キーカウンター
		motion->MotionEnd = motion->MotionEndBlend;                           // モーション

		motion->bBlend = false;                         // ブレンド終了
	}

	return motion->MotionEnd;
}

//************************************************
// モーションの切替
//************************************************
void SetMotion(MOTION* motion, MOTION_SOURCE* motionsource, int nType, bool bBlend, int BlendFrame)
{
	if (bBlend == true && motion->MotionInfo.nNumKey > 1)
	{// ブレンドする
		motion->nNowMotiTypeBlend = nType;							                    // タイプを変える
		motion->MotionInfoBlend = motionsource->aMotionInfo[motion->nNowMotiTypeBlend]; // モーション情報をもらう
		motion->nNumKeyBlend = motion->MotionInfoBlend.nNumKey;	                        // キーの最大数を代入
		motion->bLoopBlend = motion->MotionInfoBlend.bLoop;	                            // ループの代入
		motion->nKeyBlend = 0;										                    // 今数えているキーを初期化
		if (motion->nNumKeyBlend > 1)
		{// 一キー以上あるなら
			motion->nNextKeyBlend = motion->nKeyBlend + 1;                              // 次のキーを設定
		}
		else
		{// ないならば
			motion->nNextKeyBlend = motion->nKeyBlend;								    // 次のキーを設定
		}
		motion->nCounterMotionBlend = 0;								                // フレームカウンターをゼロ
		motion->nCounterKeyBlend = 0;									                // キーカウンターをゼロ
		motion->MotionEndBlend = false;                                                 // モーションは終わっていない

		motion->nBlendFrame = BlendFrame;                                               // ブレンドフレーム
		motion->nBlendCounter = 0;                                                      // ブレンドカウンター
		motion->bBlend = true;                                                          // ブレンドする
	}
	else
	{// ブレンドしない
		motion->nNowMotiType = nType;							              // タイプを変える
		motion->MotionInfo = motionsource->aMotionInfo[motion->nNowMotiType]; // モーション情報をもらう
		motion->nNumKey = motion->MotionInfo.nNumKey;	                      // キーの最大数を代入
		motion->bLoop = motion->MotionInfo.bLoop;	                          // ループの代入
		motion->nKey = 0;										              // 今数えているキーを初期化
		if (motion->nNumKey > 1)
		{// 一キー以上あるなら
			motion->nNextKey = motion->nKey + 1;                            // 次のキーを設定
		}
		else
		{// ないならば
			motion->nNextKey = motion->nKey;								// 次のキーを設定
		}
		motion->nCounterMotion = 0;								            // フレームカウンターをゼロ
		motion->nCounterKey = 0;									        // キーカウンターをゼロ
		motion->MotionEnd = false;                                          // モーションは終わっていない
		motion->bBlend = false;                                             // ブレンドしない

		//ブレンド用の変数も初期化しておく
		motion->nNowMotiTypeBlend = -1;							                        // タイプを変える
		motion->MotionInfoBlend = motionsource->aMotionInfo[0];                        // モーション情報をもらう
		motion->nNumKeyBlend = 0;	                                                    // キーの最大数を代入
		motion->bLoopBlend = false;	                                                    // ループの代入
		motion->nKeyBlend = -1;										                    // 今数えているキーを初期化
		motion->nNextKeyBlend = -1;                                                     // 次のキーを設定
		motion->nCounterMotionBlend = -1;								                // フレームカウンターをゼロ
		motion->nCounterKeyBlend = -1;									                // キーカウンターをゼロ
		motion->MotionEndBlend = false;                                                 // モーションは終わっていない

		motion->nBlendFrame = 0;                                                        // ブレンドフレーム
		motion->nBlendCounter = 0;                                                      // ブレンドカウンター
	}
}