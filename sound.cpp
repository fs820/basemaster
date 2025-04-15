//=============================================================================
//
// �T�E���h���� [sound.cpp]
//
//=============================================================================
#include "sound.h"
#include <thread>                     // �X���b�h
using std::thread;                    // �X���b�h�̏ȗ� sato
using std::chrono::milliseconds;      // �~���b�ϊ��̏ȗ� sato
using std::this_thread::sleep_for;    // �X���[�v�̏ȗ� sato

//*****************************************************************************
// �}�N����` sato
//*****************************************************************************
constexpr float FADE_STEP = 0.016f;              // �t�F�[�h�A�E�g�X�s�[�h
constexpr auto FADE_INTERVAL = milliseconds(50); // �t�F�[�h�A�E�g�Ԋu ms

//*****************************************************************************
// �T�E���h���̍\���̒�`
//*****************************************************************************
typedef struct
{	
	const char *pFilename;	// �t�@�C����
	int nCntLoop;			// ���[�v�J�E���g
} SOUNDINFO;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
IXAudio2 *g_pXAudio2 = nullptr;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice *g_pMasteringVoice = nullptr;        // �}�X�^�[�{�C�X
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

// �T�E���h�̏��
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] =
{
	{"data/SOUND/Test.wav", -1}, // Test BGM
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "COM���C�u�����̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		UninitSound();
		return E_FAIL;
	}

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		UninitSound();
		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(g_pXAudio2 != NULL)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		UninitSound();
		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);

			UninitSound();
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);

			UninitSound();
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = new BYTE[g_aSizeAudio[nCntSound]];
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

			UninitSound();
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aSoundInfo[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		// �\�[�X�{�C�X�̔j��
		if(g_apSourceVoice[nCntSound] != nullptr)
		{
			g_apSourceVoice[nCntSound]->Stop(0);
			g_apSourceVoice[nCntSound]->FlushSourceBuffers();
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = nullptr;
		}

		// �I�[�f�B�I�f�[�^�̊J��
		if (g_apDataAudio[nCntSound] != nullptr)
		{
			delete[] g_apDataAudio[nCntSound];
			g_apDataAudio[nCntSound] = nullptr;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	if (g_pMasteringVoice != nullptr)
	{
		g_pMasteringVoice->DestroyVoice();
		g_pMasteringVoice = nullptr;
	}
	
	// XAudio2�I�u�W�F�N�g�̊J��
	if(g_pXAudio2 != nullptr)
	{
		g_pXAudio2->Release();
		g_pXAudio2 = nullptr;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label, float volume)
{
	// `label` �͈̔̓`�F�b�N
	if (label < static_cast<SOUND_LABEL>(0) || label >= SOUND_LABEL_MAX)
	{
		return E_INVALIDARG;
	}

	// ���y�����݂���
	if (g_apSourceVoice[label] == nullptr)
	{
		return E_POINTER; // ���s���̃��^�[��
	}

	// ��Ԃ̎擾
	XAUDIO2_VOICE_STATE xa2state;
	g_apSourceVoice[label]->GetState(&xa2state);

	// �����̍Đ����~
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

	// �͈͐���
	volume = clamp(volume, 0.0f, 1.0f);

	// buffer�̐ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aSoundInfo[label].nCntLoop;

	// ����
	if (FAILED(g_apSourceVoice[label]->SetVolume(volume)))
	{
		DebugPrintString("E", "Volume set failed");
		return E_FAIL;
	}

	// buffer�̓o�^
	if (FAILED(g_apSourceVoice[label]->SubmitSourceBuffer(&buffer)))
	{
		DebugPrintString("E", "Buffer submission failed");
		return E_FAIL;
	}

	// �Đ�
	if (FAILED(g_apSourceVoice[label]->Start(0)))
	{
		DebugPrintString("E", "Playback start failed");
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �Z�O�����g��~(���x���w��)
//=============================================================================
HRESULT StopSound(SOUND_LABEL label)
{
	// `label` �͈̔̓`�F�b�N
	if (label < static_cast<SOUND_LABEL>(0) || label >= SOUND_LABEL_MAX)
	{
		return E_INVALIDARG;
	}

	// ���y�����݂���
	if (g_apSourceVoice[label] == nullptr)
	{
		return E_POINTER; // ���s���̃��^�[��
	}

	// �t�F�[�h�A�E�g�X���b�h
	thread fadeOutThread([label]()
		{
			IXAudio2SourceVoice*& pVoice = g_apSourceVoice[label]; // ���[�J���Q��

			if (pVoice != nullptr)
			{
				// ��Ԃ̎擾
				XAUDIO2_VOICE_STATE xa2state;
				pVoice->GetState(&xa2state);
				if (xa2state.BuffersQueued != 0)
				{
					// ���ʂ̎擾
					float volume;
					pVoice->GetVolume(&volume);

					// ���ʂ�0�ɂȂ�܂ŉ��ʂ�������������
					while (volume > 0.0f)
					{
						if (pVoice == nullptr)
						{// �G���[���
							break;
						}
						pVoice->GetVolume(&volume);
						volume -= FADE_STEP; // ������������
						volume = max(0.0f, volume); // �����l
						pVoice->SetVolume(volume); //���ʕύX
						sleep_for(FADE_INTERVAL); // �����҂�
					}

					if (pVoice != nullptr)
					{// �G���[���
						// ��~
						pVoice->Stop(0);

						// �J��
						pVoice->FlushSourceBuffers();
					}
				}
			}
		});

	// �X���b�h�����
	fadeOutThread.detach();

	return S_OK;
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
HRESULT StopSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{// ���ׂẲ��y
		if(g_apSourceVoice[static_cast<SOUND_LABEL>(nCntSound)] != nullptr)
		{// ���݂��Ă���
			if (FAILED(StopSound(static_cast<SOUND_LABEL>(nCntSound))))
			{// ��~�Ɏ��s
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

//=============================================================================
// ���ʕύX
//=============================================================================
HRESULT SetSoundVolume(SOUND_LABEL label, float volume)
{
	// `label` �͈̔̓`�F�b�N
	if (label < static_cast<SOUND_LABEL>(0) || label >= SOUND_LABEL_MAX)
	{
		return E_INVALIDARG;
	}

	// ���y�����݂���
	if (g_apSourceVoice[label] == nullptr)
	{
		return E_POINTER; // ���s���̃��^�[��
	}

	// �͈͐���
	volume = clamp(volume, 0.0f, 1.0f);

	// ���ʂ�ύX
	return g_apSourceVoice[label]->SetVolume(volume);
}

//=============================================================================
// �`�����N�̃`�F�b�N
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
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}