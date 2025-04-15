//------------------------------------
//
//����Đ�����[movie.cpp]
//Author:NULL
//
//------------------------------------

#include <thread>
#include <mfapi.h>
#include <mfidl.h>
#include <mfplay.h>
#include <mfobjects.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <evr.h>
#include <assert.h>
#include <Windows.h>
#include "movie.h"
#include "input.h"
#include "fade.h"
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplay.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "evr.lib")

// �����Ɖf���̃��f�B�A�^�C�v���`
const GUID MFMediaType_Audio = { 0x73647561, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };
const GUID MFMediaType_Video = { 0x73646976, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };

// �T�[�t�F�C�X�̒�`
const GUID MR_BUFFER_SERVICE = { 0xa562248c, 0x9ac6, 0x4ffc, { 0x9f, 0xba, 0x3a, 0xf8, 0xf8, 0xad, 0x1a, 0x4d } };

// �O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMovie = nullptr;
LPDIRECT3DTEXTURE9 g_pTextureMovie = nullptr;
LPDIRECT3DSURFACE9 g_pMovieSurface = nullptr;
//---------------------------------------------------------
// �g�|���W�̍쐬�w���p�[�֐�
//---------------------------------------------------------
HRESULT AddStreamToTopology(IMFTopology* pTopology, IMFMediaSource* pSource, IMFPresentationDescriptor* pPD, DWORD iStream, HWND hwnd)
{
    IMFStreamDescriptor* pSD = NULL;
    IMFActivate* pRendererActivate = NULL;
    BOOL fSelected = FALSE;
    HRESULT hr = S_OK;

    // �X�g���[���f�B�X�N���v�^���擾
    hr = pPD->GetStreamDescriptorByIndex(iStream, &fSelected, &pSD);
    RETURN_HR(hr);

    if (fSelected)
    {
        // �X�g���[���^�C�v�𔻕�
        GUID majorType;
        IMFMediaTypeHandler* pHandler = NULL;
        hr = pSD->GetMediaTypeHandler(&pHandler);
        RETURN_HR(hr);

        hr = pHandler->GetMajorType(&majorType);
        pHandler->Release();
        RETURN_HR(hr);

        // �f���X�g���[���Ȃ�EVR���g���A�����X�g���[���Ȃ�SAR���g��
        if (majorType == MFMediaType_Video)
        {
            // EVR�̃A�N�e�B�x�[�^���쐬
            hr = MFCreateVideoRendererActivate(hwnd, &pRendererActivate);
            RETURN_HR(hr);
        }
        else if (majorType == MFMediaType_Audio)
        {
            // SAR�̃A�N�e�B�x�[�^���쐬
            hr = MFCreateAudioRendererActivate(&pRendererActivate);
            RETURN_HR(hr);
        }

        // �g�|���W�Ƀ\�[�X�m�[�h��ǉ�
        IMFTopologyNode* pSourceNode = NULL;
        hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pSourceNode);
        RETURN_HR(hr);

        hr = pSourceNode->SetUnknown(MF_TOPONODE_SOURCE, pSource);
        RETURN_HR(hr);

        hr = pSourceNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD);
        RETURN_HR(hr);

        hr = pSourceNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD);
        RETURN_HR(hr);

        hr = pTopology->AddNode(pSourceNode);
        RETURN_HR(hr);

        // �g�|���W�Ƀ����_���[�m�[�h��ǉ�
        IMFTopologyNode* pOutputNode = NULL;
        hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pOutputNode);
        RETURN_HR(hr);

        hr = pOutputNode->SetObject(pRendererActivate);
        RETURN_HR(hr);

        hr = pTopology->AddNode(pOutputNode);
        RETURN_HR(hr);

        // �\�[�X�m�[�h�ƃ����_���[�m�[�h��ڑ�
        hr = pSourceNode->ConnectOutput(0, pOutputNode, 0);
        RETURN_HR(hr);

        pSourceNode->Release();
        pOutputNode->Release();
    }

    pSD->Release();
    if (pRendererActivate) pRendererActivate->Release();
    return hr;
}

//---------------------------------------------------------
// �g�|���W�̍쐬�֐�
//---------------------------------------------------------
HRESULT CreatePlaybackTopology(IMFMediaSource* pSource, HWND hwnd, IMFTopology** ppTopology)
{
    IMFTopology* pTopology = NULL;
    IMFPresentationDescriptor* pPD = NULL;
    HRESULT hr = S_OK;

    // �g�|���W���쐬
    hr = MFCreateTopology(&pTopology);
    RETURN_HR(hr);

    // �v���[���e�[�V�����f�B�X�N���v�^�̎擾
    hr = pSource->CreatePresentationDescriptor(&pPD);
    RETURN_HR(hr);

    // �e�X�g���[���ɑ΂��ăm�[�h���쐬���A�g�|���W�ɒǉ�
    DWORD cStreams = 0;
    hr = pPD->GetStreamDescriptorCount(&cStreams);
    RETURN_HR(hr);

    for (DWORD i = 0; i < cStreams; i++)
    {
        hr = AddStreamToTopology(pTopology, pSource, pPD, i, hwnd);
        RETURN_HR(hr);
    }

    *ppTopology = pTopology;
    pPD->Release();
    return hr;
}

//---------------------------
// ���[�r�[�̏�����
//---------------------------
void InitMovie(void)
{
    LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
    VERTEX_2D* pVtx;//���_���|�C���^
    D3DXVECTOR3 posSelect;//�X�R�A�̈ʒu

    //�f�o�C�X�̎擾
    pDevice = GetDevice();

    //�o�b�t�@�[�̐ݒ�
    pDevice->CreateVertexBuffer
    (
        sizeof(VERTEX_2D) * VT_MAX,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_2D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffMovie,
        NULL
    );

    // �e�N�X�`��������
    D3DXCreateTexture
    (
        pDevice,
        (UINT)SCREEN_WIDTH, (UINT)SCREEN_HEIGHT,
        1, 0,
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
        &g_pTextureMovie
    );

    // �e�N�X�`���̃T�[�t�F�X���擾
    g_pTextureMovie->GetSurfaceLevel(0, &g_pMovieSurface);

    // ���_�f�[�^�ݒ�
    g_pVtxBuffMovie->Lock(0, 0, (void**)&pVtx, 0);
    // ��ʒ����ɕ\�������l�p�`�̐ݒ�
    
    //���W�ݒ�
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    //rhw
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    //�J���[
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //�e�N�X�`��
    pVtx[0].tex[0] = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex[0] = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex[0] = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex[0] = D3DXVECTOR2(1.0f, 1.0f);

    g_pVtxBuffMovie->Unlock();
}

//---------------------------
// ���[�r�[�̔j��
//---------------------------
void UninitMovie(void)
{
    SAFE_RELEASE(g_pMovieSurface);
    SAFE_RELEASE(g_pTextureMovie);
    SAFE_RELEASE(g_pVtxBuffMovie);
}

//---------------------------
// ���[�r�[�̍X�V
//---------------------------
void UpdateMovie(void)
{
    if (!g_pTextureMovie || !g_pMovieSurface)
        return;

    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    if (!pDevice)
        return;

    // �e�N�X�`���̃T�[�t�F�X���擾
    IDirect3DSurface9* pTextureSurface = nullptr;
    if (SUCCEEDED(g_pTextureMovie->GetSurfaceLevel(0, &pTextureSurface)))
    {
        // ���[�r�[�t���[�����e�N�X�`���ɃR�s�[
        pDevice->StretchRect(g_pMovieSurface, NULL, pTextureSurface, NULL, D3DTEXF_LINEAR);

        // �T�[�t�F�X�����
        pTextureSurface->Release();
    }

    UpdateKeyboard();//�L�[����
    UpdateMouse();//�}�E�X����
    UpdateJoypad();//�p�b�h����
    UpdatedJoypad();//d�p�b�h����

    UpdateFade();
    // �L�[�{�[�h��R���g���[���[�̓��͂��`�F�b�N
    if (GetKeyboardTrigger(DIK_RETURN) || GetJoykeyTrigger(JOYKEY_START, CONTROLLER_1))
    {
        if (GetFade() == FADE_NONE)
        {
            SetFade(MODE(), false);
        }
    }
}

//---------------------------
// ���[�r�[�̕`��
//---------------------------
void DrawMovie(void)
{
    if (!g_pTextureMovie) return;

    LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

    //�f�o�C�X�̎擾
    pDevice = GetDevice();

    pDevice->SetTexture(0, g_pTextureMovie);
    pDevice->SetStreamSource(0, g_pVtxBuffMovie, 0, sizeof(VERTEX_2D));
    pDevice->SetFVF(FVF_VERTEX_2D);
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//---------------------------------------------------------
// �r�f�I�t���[���̎擾
//---------------------------------------------------------
HRESULT ProcessFrame(IMFSample* pSample)
{
    if (!pSample || !g_pMovieSurface)
        return E_POINTER;

    IMFMediaBuffer* pBuffer = nullptr;
    IDirect3DSurface9* pSurface = nullptr;

    // �T���v������o�b�t�@���擾
    HRESULT hr = pSample->GetBufferByIndex(0, &pBuffer);
    if (FAILED(hr)) return hr;

    // Direct3D �T�[�t�F�X���擾
    hr = MFGetService(pBuffer, MR_BUFFER_SERVICE, IID_PPV_ARGS(&pSurface));
    if (SUCCEEDED(hr))
    {
        // �擾�����T�[�t�F�X�� g_pMovieSurface �ɃZ�b�g
        g_pMovieSurface = pSurface;
    }

    SAFE_RELEASE(pBuffer);
    SAFE_RELEASE(pSurface);

    return hr;
}

//---------------------------------------------------------
// ���[�r�[�Đ��֐�
//---------------------------------------------------------
HRESULT PlayMovie(HWND hwnd, const wchar_t* videoFilePath)
{
    IMFMediaSession* pSession = NULL;
    IMFMediaSource* pSource = NULL;
    IMFTopology* pTopology = NULL;
    IMFSourceResolver* pResolver = NULL;
    IMFPresentationDescriptor* pPD = NULL;
    HRESULT hr = S_OK;

    hr = MFStartup(MF_VERSION);
    RETURN_HR(hr);
    hr = MFCreateSourceResolver(&pResolver);
    RETURN_HR(hr);

    MF_OBJECT_TYPE ObjectType = MF_OBJECT_INVALID;
    IUnknown* pSourceUnk = NULL;
    hr = pResolver->CreateObjectFromURL(videoFilePath, MF_RESOLUTION_MEDIASOURCE, NULL, &ObjectType, &pSourceUnk);
    RETURN_HR(hr);
    hr = pSourceUnk->QueryInterface(IID_PPV_ARGS(&pSource));
    pSourceUnk->Release();
    RETURN_HR(hr);

    hr = MFCreateMediaSession(nullptr, &pSession);
    RETURN_HR(hr);
    hr = CreatePlaybackTopology(pSource, hwnd, &pTopology);
    RETURN_HR(hr);
    hr = pSession->SetTopology(0, pTopology);
    RETURN_HR(hr);

    PROPVARIANT varStart;
    PropVariantInit(&varStart);
    varStart.vt = VT_EMPTY;
    hr = pSession->Start(nullptr, &varStart);
    PropVariantClear(&varStart);
    RETURN_HR(hr);

    std::thread eventThread([pSession]()
        {
            pSession->AddRef();
            IMFMediaEvent* pEvent = NULL;
            MediaEventType meType;
            HRESULT hr = S_OK;
            bool exitThread = false;

            // �X���b�h���ŃC�x���g������
            while (!exitThread)
            {
                IMFMediaEvent* pEvent = nullptr;
                MediaEventType meType;

                hr = pSession->GetEvent(MF_EVENT_FLAG_NO_WAIT, &pEvent);
                if (SUCCEEDED(hr))
                {
                    hr = pEvent->GetType(&meType);

                    if (meType == METransformHaveOutput)
                    {
                        // �t���[���̎擾
                        IMFMediaBuffer* pBuffer = nullptr;
                        IMFSample* pSample = nullptr;

                        hr = pEvent->QueryInterface(IID_PPV_ARGS(&pSample));
                        if (SUCCEEDED(hr))
                        {
                            ProcessFrame(pSample);
                        }

                        SAFE_RELEASE(pSample);
                    }

                    if (meType == MESessionEnded)
                    {
                        exitThread = true;
                    }

                    SAFE_RELEASE(pEvent);
                }

                UpdateMovie(); // DirectX9�̕`��f�[�^���X�V
                Draw();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            pSession->Close();
            hr = pSession->GetEvent(MF_EVENT_FLAG_NO_WAIT, &pEvent);
            if (SUCCEEDED(hr))
            {
                hr = pEvent->GetType(&meType);
                if (SUCCEEDED(hr) && meType == MESessionClosed)
                {
                    // �Z�b�V���������S�ɏI��
                }
                pEvent->Release();
            }

            pSession->Release();
        });

    eventThread.join();

    pSource->Shutdown();
    pSource->Release();
    SAFE_RELEASE(pResolver);
    SAFE_RELEASE(pTopology);
    SAFE_RELEASE(pSession);
    MFShutdown();

    return hr;
}