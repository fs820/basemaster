//=============================================================================
//
// サウンド処理 [sound.cpp]
//
//=============================================================================
#include "sound.h"
#include <thread>                     // スレッド
using std::thread;                    // スレッドの省略 sato
using std::chrono::milliseconds;      // ミリ秒変換の省略 sato
using std::this_thread::sleep_for;    // スリープの省略 sato

//*****************************************************************************
// マクロ定義 sato
//*****************************************************************************
constexpr float FADE_STEP = 0.016f;              // フェードアウトスピード
constexpr auto FADE_INTERVAL = milliseconds(50); // フェードアウト間隔 ms

//*****************************************************************************
// サウンド情報の構造体定義
//*****************************************************************************
typedef struct
{	
	const char *pFilename;	// ファイル名
	int nCntLoop;			// ループカウント
} SOUNDINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
IXAudio2 *g_pXAudio2 = nullptr;								// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *g_pMasteringVoice = nullptr;        // マスターボイス
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

// サウンドの情報
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] =
{
	{"data/SOUND/Test.wav", -1}, // Test BGM
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "COMライブラリの初期化に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		UninitSound();
		return E_FAIL;
	}

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		UninitSound();
		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(g_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		UninitSound();
		return E_FAIL;
	}

	// サウンドデータの初期化
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);

			UninitSound();
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);

			UninitSound();
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = new BYTE[g_aSizeAudio[nCntSound]];
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}
	
		// ソースボイスの生成
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aSoundInfo[nCntSound].nCntLoop;

		// オーディオバッファの登録
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		// ソースボイスの破棄
		if(g_apSourceVoice[nCntSound] != nullptr)
		{
			g_apSourceVoice[nCntSound]->Stop(0);
			g_apSourceVoice[nCntSound]->FlushSourceBuffers();
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = nullptr;
		}

		// オーディオデータの開放
		if (g_apDataAudio[nCntSound] != nullptr)
		{
			delete[] g_apDataAudio[nCntSound];
			g_apDataAudio[nCntSound] = nullptr;
		}
	}
	
	// マスターボイスの破棄
	if (g_pMasteringVoice != nullptr)
	{
		g_pMasteringVoice->DestroyVoice();
		g_pMasteringVoice = nullptr;
	}
	
	// XAudio2オブジェクトの開放
	if(g_pXAudio2 != nullptr)
	{
		g_pXAudio2->Release();
		g_pXAudio2 = nullptr;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label, float volume)
{
	// `label` の範囲チェック
	if (label < static_cast<SOUND_LABEL>(0) || label >= SOUND_LABEL_MAX)
	{
		return E_INVALIDARG;
	}

	// 音楽が存在する
	if (g_apSourceVoice[label] == nullptr)
	{
		return E_POINTER; // 失敗時のリターン
	}

	// 状態の取得
	XAUDIO2_VOICE_STATE xa2state;
	g_apSourceVoice[label]->GetState(&xa2state);

	// 既存の再生を停止
	if (xa2state.BuffersQueued > 0)
	{
		if (FAILED(g_apSourceVoice[label]->Stop(0)))
		{
			DebugPrintString("E", "Failed to stop voice");
			return E_FAIL;
		}
		if (FAILED(g_apSourceVoice[label]->FlushSourceBuffers()))
		{
			DebugPrintString("E", "Failed to flush buffers");
			return E_FAIL;
		}
	}

	XAUDIO2_BUFFER buffer;

	// 範囲制限
	volume = clamp(volume, 0.0f, 1.0f);

	// bufferの設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aSoundInfo[label].nCntLoop;

	// 音量
	if (FAILED(g_apSourceVoice[label]->SetVolume(volume)))
	{
		DebugPrintString("E", "Volume set failed");
		return E_FAIL;
	}

	// bufferの登録
	if (FAILED(g_apSourceVoice[label]->SubmitSourceBuffer(&buffer)))
	{
		DebugPrintString("E", "Buffer submission failed");
		return E_FAIL;
	}

	// 再生
	if (FAILED(g_apSourceVoice[label]->Start(0)))
	{
		DebugPrintString("E", "Playback start failed");
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
HRESULT StopSound(SOUND_LABEL label)
{
	// `label` の範囲チェック
	if (label < static_cast<SOUND_LABEL>(0) || label >= SOUND_LABEL_MAX)
	{
		return E_INVALIDARG;
	}

	// 音楽が存在する
	if (g_apSourceVoice[label] == nullptr)
	{
		return E_POINTER; // 失敗時のリターン
	}

	// フェードアウトスレッド
	thread fadeOutThread([label]()
		{
			IXAudio2SourceVoice*& pVoice = g_apSourceVoice[label]; // ローカル参照

			if (pVoice != nullptr)
			{
				// 状態の取得
				XAUDIO2_VOICE_STATE xa2state;
				pVoice->GetState(&xa2state);
				if (xa2state.BuffersQueued != 0)
				{
					// 音量の取得
					float volume;
					pVoice->GetVolume(&volume);

					// 音量が0になるまで音量を少しずつ下げる
					while (volume > 0.0f)
					{
						if (pVoice == nullptr)
						{// エラー回避
							break;
						}
						pVoice->GetVolume(&volume);
						volume -= FADE_STEP; // 少しずつ下げる
						volume = max(0.0f, volume); // 下限値
						pVoice->SetVolume(volume); //音量変更
						sleep_for(FADE_INTERVAL); // 少し待つ
					}

					if (pVoice != nullptr)
					{// エラー回避
						// 停止
						pVoice->Stop(0);

						// 開放
						pVoice->FlushSourceBuffers();
					}
				}
			}
		});

	// スレッドを放す
	fadeOutThread.detach();

	return S_OK;
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
HRESULT StopSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{// すべての音楽
		if(g_apSourceVoice[static_cast<SOUND_LABEL>(nCntSound)] != nullptr)
		{// 存在している
			if (FAILED(StopSound(static_cast<SOUND_LABEL>(nCntSound))))
			{// 停止に失敗
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

//=============================================================================
// 音量変更
//=============================================================================
HRESULT SetSoundVolume(SOUND_LABEL label, float volume)
{
	// `label` の範囲チェック
	if (label < static_cast<SOUND_LABEL>(0) || label >= SOUND_LABEL_MAX)
	{
		return E_INVALIDARG;
	}

	// 音楽が存在する
	if (g_apSourceVoice[label] == nullptr)
	{
		return E_POINTER; // 失敗時のリターン
	}

	// 範囲制限
	volume = clamp(volume, 0.0f, 1.0f);

	// 音量を変更
	return g_apSourceVoice[label]->SetVolume(volume);
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}