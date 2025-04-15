//------------------------------------------
//
//���C���̏���[main.cpp]
//Author fuma sato
//
//------------------------------------------

#include "main.h"
#include "resource.h"
#include "camera.h"
#include "fade.h"
#include "light.h"
#include "sound.h"
#include "input.h"
#include "pause.h"
#include "game.h"
#include "result.h"
#include "title.h"
#include "stddxbuff.h"

LPDIRECT3D9 g_pD3D = NULL;//�_�C���N�gX�I�u�W�F�N�g�̃O���[�o���|�C���^��錾
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;//�_�C���N�gX�f�o�C�X�̃O���[�o���|�C���^��錾
MODE g_mode = MODE_TITLE;
HINSTANCE g_hInstance;
HWND g_hWnd;
bool g_bStop = false;
bool g_bFullScreen = false;
LPD3DXFONT g_pFont = NULL;//�t�H���g�|�C���^
int g_nCountFPS = 0;//FPS�J�E���^
bool g_bDebug = true;

//------------------------
//���C���֐�
//------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//�E�C���h�E�ݒ�
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),//�������T�C�Y
		CS_CLASSDC,//�X�^�C��
		WindowProc,//�v���V�[�W��
		0,//0
		0,//0
		hInstance,//�C���X�^���X�n���h��
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),//�^�X�N�o�[�A�C�R��
		LoadCursor(hInstance,MAKEINTRESOURCE(IDC_CURSOR1)),//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 3),//�w�i�F
		MAKEINTRESOURCE(IDR_MENU1),//���j���[�o�[
		CLASS_NAME,//�N���X�̖��O
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1))//�A�C�R��
	};

	srand((unsigned int)time(NULL));//�����ݒ�
	HWND hWnd = nullptr;//�n���h��
	MSG msg = { 0 };//���b�Z�[�W
	RECT rect = { 0,0,(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };//�E�C���h�E�T�C�Y�̐ݒ�
	DWORD dwCurrntTime;//���݂̃V�X�e������
	DWORD dwExceLastTime;//�O��̃V�X�e������

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// DPI�X�P�[�����O�Ή�
	if (FAILED(SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE)))
	{
		SetProcessDPIAware();  // Windows 7 �݊��p
	}

	//�N���C�A���g�̈�̃T�C�Y����
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx
	(
		0,//�X�^�C��
		CLASS_NAME,//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,//�X�^�C��
		CW_USEDEFAULT,//������W
		CW_USEDEFAULT,//�E�����W
		(rect.right - rect.left),//��
		(rect.bottom - rect.top),//����
		NULL,//�e
		NULL,//���j���[ID
		hInstance,//�C���X�^���X�n���h��
		NULL//�쐬�f�[�^
	);

	g_hInstance = hInstance;
	g_hWnd = hWnd;

	//�\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (FAILED(Init(hInstance, hWnd, TRUE)))//����������
	{
		return E_FAIL;
	}

	RECT Rect;
	GetWindowRect(hWnd, &Rect);

	timeBeginPeriod(1);//����\�̐ݒ�
	dwCurrntTime = 0;//��������
	dwExceLastTime = timeGetTime();//�J�n�������L�^

	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)//�I�����b�Z�[�W
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (!g_bStop)
			{
				dwCurrntTime = timeGetTime();//�������擾
				if ((dwCurrntTime-dwFPSLastTime)>=FPS_TIME)
				{//0.5�b
					//FPS�Z�o
					g_nCountFPS = (dwFrameCount * 1000) / (dwCurrntTime - dwFPSLastTime);
					dwFPSLastTime = dwCurrntTime;
					dwFrameCount = 0;
				}

				if ((dwCurrntTime - dwExceLastTime) >= 1000 / FPS)//�ݒ肵���Ԋu���󂢂���
				{//60����1
					dwExceLastTime = dwCurrntTime;//����̍X�V�������L�^

					Update();//�X�V����
					Render();//�`�揈��

					dwFrameCount++;
				}
			}
		}
	}

	Uninit();//�I������

	//�E�C���h�E�N���X�̓o�^����
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	timeEndPeriod(1);//����\��߂�

	CursorSwitch(ON);

	//�I��
	return (int)msg.wParam;
}

//--------------------------
//�E�C���h�E�v���V�[�W��
//--------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID = 0;//�Ԃ�l�̊i�[
	static bool bFreeMause=false;

	switch (uMsg)
	{
	case WM_DESTROY://�j��
		//WM_QUIT���b�Z�[�W
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN://�L�[����
		switch (wParam)
		{
		case VK_ESCAPE://ESC�L�[
			CursorSwitch(ON);
			nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);
			if (nID == IDYES)
			{
				//�j��(WM_DESTROY)
				DestroyWindow(hWnd);
			}
			break;
		case VK_F2:
			g_bDebug = !g_bDebug;
			break;
		case VK_F4:
			if (bFreeMause)
			{
				bFreeMause = false;

				CursorSwitch(OFF);
			}
			else
			{
				bFreeMause = true;

				CursorSwitch(ON);
			}
			break;
		}
		break;
	case WM_SETFOCUS:
		g_bStop = false;
		break;
	case WM_KILLFOCUS:
		g_bStop = true;
		break;
	case WM_COMMAND:
		// ���j���[���ڂ�ID���擾
		switch (LOWORD(wParam))
		{
		case IDM_FILE_NEW:
			// �V�K�쐬�̏���
			break;
		case IDM_FILE_OPEN:
			// �t�@�C�����J������
			break;
		case IDM_FILE_SAVE:
			// �ۑ�����
			break;
		case IDM_FILE_SAVEAS:
			// ���O��t���ĕۑ�����
			break;
		case IDM_FILE_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_EDIT_UNDO:
			// ���ɖ߂�����
			break;
		case IDM_EDIT_CUT:
			// �؂��菈��
			break;
		case IDM_EDIT_COPY:
			// �R�s�[����
			break;
		case IDM_EDIT_PASTE:
			// �\��t������
			break;
		case IDM_HELP_ABOUT:
			MessageBox(hWnd, "�A�v���P�[�V�����̃o�[�W�������", "�o�[�W�������", MB_OK | MB_ICONINFORMATION);
			break;
		}
		break;
	}
	//�K�v�f�[�^��Ԃ�
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//------------------------------
//����������
//------------------------------
HRESULT Init(HINSTANCE hInstanse, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;//�_�C���N�gX�f�B�X�v���C���[�h�̕ϐ��錾
	D3DPRESENT_PARAMETERS d3dpp;//�_�C���N�gX�p�����[�^�̕ϐ��錾

	//�I�u�W�F�N�g����
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//�f�B�X�v���C���[�h
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �N���C�A���g�̈�̃T�C�Y���擾
	RECT clientRect;
	GetClientRect(g_hWnd, &clientRect);

	ZeroMemory(&d3dpp, sizeof(d3dpp));//0��~���l�߂�

	//�o�b�N�o�b�t�@�̐ݒ�
	d3dpp.BackBufferWidth = clientRect.right - clientRect.left;
	d3dpp.BackBufferHeight = clientRect.bottom - clientRect.top;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;// �`��̗�������炷 ���������ׂ�������̂Ŏg��Ȃ��Ƃ��͖߂�
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL; // �`��̗�������炷 �[�x�o�b�t�@�𖈃t���[���j������ ���������ׂ����炷���V���h�E�}�b�s���O���ł��Ȃ��Ȃ�
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//�f�o�C�X����
	if (FAILED(g_pD3D->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice
	)))
	{
		if (FAILED(g_pD3D->CreateDevice
		(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice
		)))
		{
			if (FAILED(g_pD3D->CreateDevice
			(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice
			)))
			{
				return E_FAIL;
			}
		}
	}

	//�`��ݒ�
	RSOP rsOp;                //�����_�[�X�e�[�g�̍\����
	SSOP ssOp[TEXTURE_MAX];   //�T���v���[�X�e�[�g�̍\����
	TSSOP tssOp[TEXTURE_MAX]; //�e�N�X�`���X�e�[�W�̍\����
	ZeroMemory(&rsOp, sizeof(rsOp)); //0��~���l�߂�
	RETURN_HR(GetDrawOperation(&rsOp, &ssOp[0], TEXTURE_MAX, &tssOp[0], TEXTURE_MAX)) //�`������擾

	//�����_�[�X�e�[�g�̐ݒ�
	rsOp.Lighting = TRUE;                  // ���C�e�B���O
	rsOp.cullMode = D3DCULL_CCW;           // �J�����O�̐ݒ�
	rsOp.alphaBlendEnable = TRUE;          // �v���C���[�̒��ɓ����x��������
	rsOp.srcBlend = D3DBLEND_SRCALPHA;     // �����@
	rsOp.destBlend = D3DBLEND_INVSRCALPHA; // �����@
	rsOp.blendOp = D3DBLENDOP_ADD;         // �u�����h���Z
	rsOp.zEnable = TRUE;                   // Z�o�b�t�@
	rsOp.zWriteEnable = TRUE;              // Z�o�b�t�@��������
	rsOp.zFunc = D3DCMP_LESSEQUAL;         // Z�o�b�t�@��r�֐�
	rsOp.zBias = 0.0f;                     // Z�o�C�A�X
	rsOp.alphaTestEnable = FALSE;          // �A���t�@�e�X�g
	rsOp.alphaRef = 0;                     // �A���t�@�e�X�g�Q�ƒl
	rsOp.alphaFunc = D3DCMP_GREATER;       // �A���t�@�e�X�g��r�֐�
	rsOp.antialiaseLineEnable = TRUE;      // �A���`�G�C���A�X
	rsOp.stencilEnable = FALSE;            // �X�e���V���e�X�g

	// GPU �ɍ��킹���ٕ����t�B���^�ݒ�
	D3DCAPS9 caps;
	g_pD3DDevice->GetDeviceCaps(&caps);
	DWORD maxAniso = min(caps.MaxAnisotropy, static_cast<DWORD>(16));

	// �T���u���[�X�e�[�g�̐ݒ�
	for (unsigned int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		ssOp[nCntTex].maxAnisotropy = maxAniso; // �ٕ����t�B���^�̍ő�l
		ssOp[nCntTex].minFilter = D3DTEXF_ANISOTROPIC; // �e�N�X�`���~�j�t�B�P�[�V�����t�B���^
		ssOp[nCntTex].magFilter = D3DTEXF_ANISOTROPIC; // �e�N�X�`���}�O�j�t�B�P�[�V�����t�B���^
		ssOp[nCntTex].maxMipLevel = 0; // �~�b�v�}�b�v���x��
		ssOp[nCntTex].mipMapLODBias = -0.75f; // �~�b�v�}�b�vLOD�o�C�A�X
		ssOp[nCntTex].mipFilter = D3DTEXF_ANISOTROPIC; // �~�b�v�}�b�v�t�B���^
		ssOp[nCntTex].addressU = D3DTADDRESS_WRAP; // U�����̃A�h���X���[�h
		ssOp[nCntTex].addressV = D3DTADDRESS_WRAP; // V�����̃A�h���X���[�h
		ssOp[nCntTex].addressW = D3DTADDRESS_WRAP; // W�����̃A�h���X���[�h (3D�e�N�X�`���p)
		ssOp[nCntTex].borderColor = D3DCOLOR_ARGB(255, 55, 100, 100); // �{�[�_�[�J���[ (Clamp���̋��E�F)
	}

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
    // �X�e�[�W 0 �̂ݎg�p�i�ʏ�̃I�u�W�F�N�g�j
	//�J���[
	tssOp[0].colorOp = D3DTOP_MODULATE; // �J���[�I�y���[�V����
	tssOp[0].colorArg0 = D3DTA_CURRENT; // �I�y���[�V�����̈���0
	tssOp[0].colorArg1 = D3DTA_TEXTURE; // �I�y���[�V�����̈���1
	tssOp[0].colorArg2 = D3DTA_DIFFUSE; // �I�y���[�V�����̈���2
	//�����x
	tssOp[0].alphaOp = D3DTOP_MODULATE; // �A���t�@�I�y���[�V����
	tssOp[0].alphaArg0 = D3DTA_CURRENT; // �I�y���[�V�����̈���0
	tssOp[0].alphaArg1 = D3DTA_TEXTURE; // �I�y���[�V�����̈���1
	tssOp[0].alphaArg2 = D3DTA_DIFFUSE; // �I�y���[�V�����̈���2
	//���̑�
	tssOp[0].texCoordIndex = 0; // �e�N�X�`���̃C���f�b�N�X
	tssOp[0].texTureTransFormFlags = D3DTTFF_DISABLE; // �e�N�X�`���ϊ�
	tssOp[0].resultArg = D3DTA_CURRENT; // ���ʂ̊i�[��
	tssOp[0].constant = D3DCOLOR_ARGB(255, 55, 100, 100); // �萔�l (D3DTA_CONSTANT���g�p��)

	// �X�e�[�W 1 �ȍ~�𖳌���
	for (unsigned int nCnt = 1; nCnt < TEXTURE_MAX; nCnt++)
	{
		tssOp[nCnt].colorOp = D3DTOP_DISABLE; // �J���[�I�y���[�V����
		tssOp[nCnt].alphaOp = D3DTOP_DISABLE; // �A���t�@�I�y���[�V����
	}

	RETURN_HR(SetDrawOperation(&rsOp, &ssOp[0], TEXTURE_MAX, &tssOp[0], TEXTURE_MAX)) //�`�����ݒ�

	// �f�o�b�O�p�\���t�H���g�̐���
	D3DXCreateFont
	(
		g_pD3DDevice, 18, 0, FW_NORMAL, 1,             // ����, ��, ����
		FALSE, DEFAULT_CHARSET,                        // ���{����Ȃ� SHIFTJIS_CHARSET �ł���
		OUT_TT_PRECIS,                                 // TrueType�t�H���g�D��
		CLEARTYPE_QUALITY,                             // �A���`�G�C���A�X�L��
		DEFAULT_PITCH | FF_DONTCARE,                   // �ʏ�
		"MS Gothic", &g_pFont                          // �t�H���g�������m���Ȃ��̂�
	);

	//�I�u�W�F�N�g�̏���������
	RETURN_HR(InitKeyboard(hInstanse, hWnd));
	RETURN_HR(InitMouse(hInstanse, hWnd));
	RETURN_HR(InitdJoypad(hInstanse, hWnd));
	RETURN_HR(InitJoypad());
	RETURN_HR(InitSound(hWnd));
	InitCamera();
	InitLight();
	InitFade(g_mode);

	return S_OK;
}

//------------------------------
//�I������
//------------------------------
void Uninit(void)
{
	UninitKeyboard();
	UninitMouse();
	UninitdJoypad();
	UninitJoypad();
	UninitSound();
	UninitCamera();
	UninitLight();
	UninitFade();

	//�f�o�b�O�t�H���g�j��
	SAFE_RELEASE(g_pFont);

	//�f�o�C�X�j��
	SAFE_RELEASE(g_pD3DDevice);

	//�I�u�W�F�N�g�j��
	SAFE_RELEASE(g_pD3D);
}

//------------------------------
//�X�V����
//------------------------------
void Update(void)
{
	UpdateKeyboard();
	UpdateMouse();
	UpdatedJoypad();
	UpdateJoypad();
	UpdateCamera();
	UpdateLight();
	UpdateFade();

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;
	case MODE_GAME:
		UpdateGame();
		break;
	case MODE_RESULT:
		UpdateResult();
		break;
	case MODE_RANK:
		break;
	}
}

//------------------------------
//�`�揈��
//------------------------------
void Render(void)
{
	Camera* pCamera = GetCamera();

	for (int nCount = 0; nCount < CAMERA_XNUM * CAMERA_YNUM; nCount++, pCamera++)
	{
		g_pD3DDevice->SetViewport(&pCamera->viewport);

		g_pD3DDevice->Clear//�t�����g�o�b�t�@�̃N���A
		(
			0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0
		);

		if (SUCCEEDED(g_pD3DDevice->BeginScene()))//�`��J�n
		{
			SetCamera(nCount);

			//�I�u�W�F�N�g�`��
			switch (g_mode)
			{
			case MODE_TITLE:
				DrawTitle();
				break;
			case MODE_GAME:
				DrawGame();
				break;
			case MODE_RESULT:
				DrawResult();
				break;
			case MODE_RANK:
				break;
			}
			DrawFade();

#ifdef _DEBUG

			if (g_bDebug)
			{
				DrawDebug();
			}

#endif

			g_pD3DDevice->EndScene();//�`��I��
		}
		//�o�b�N�o�b�t�@�ɕ\����؂�ւ���
		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}
}

//------------------------
//���[�h�̐؂�ւ�
//------------------------
void SetMode(MODE mode)
{
	//���݂̉�ʂ̏I������
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;
	case MODE_GAME:
		UninitGame();
		break;
	case MODE_RESULT:
		UninitResult();
		break;
	case MODE_RANK:
		break;
	}

	g_mode = mode;

	//�V������ʂ̏���������
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		break;
	case MODE_GAME:
		InitGame();
		break;
	case MODE_RESULT:
		InitResult();
		break;
	case MODE_RANK:
		break;
	}
}

//--------------------------------
// �p�x�̐��K��
//--------------------------------
float NormalizeAngle(float angle)
{
	while (angle > D3DX_PI)
	{
		angle -= D3DX_PI * 2.0f;
	}
	while (angle < -D3DX_PI)
	{
		angle += D3DX_PI * 2.0f;
	}
	return angle;
}

//---------------------------------
// �����W
//---------------------------------
void SpherePos(D3DXVECTOR3& OutPos, D3DXVECTOR3 BasePos, D3DXVECTOR3 rot, float Length)
{
	OutPos.x = BasePos.x + sinf(rot.x) * sinf(rot.y) * Length;
	OutPos.y = BasePos.y + cosf(rot.x) * Length;
	OutPos.z = BasePos.z + sinf(rot.x) * cosf(rot.y) * Length;
}

//------------------------
//�f�o�C�X�̎擾
//------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//-----------------
//���[�h�̎擾
//-----------------
MODE GetMode(void)
{
	return g_mode;
}

//-----------------
//�n���h���̎擾
//-----------------
HWND GethWnd(void)
{
	return g_hWnd;
}

//-----------------
//�C���X�^���X�n���h���擾
//-----------------
HINSTANCE GethInstanse(void)
{
	return g_hInstance;
}

//-----------------
// �G���[
//-----------------
void Error(HRESULT hr)
{
	char msg[256];
	snprintf(msg, sizeof(msg), "���s���܂����B\nHRESULT: 0x%08X", hr);
	MessageBox(g_hWnd, msg, "�G���[", MB_OK | MB_ICONERROR);
	PostMessage(g_hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
}

//------------------
//�}�E�X����
//------------------
void ConfineCursorToWindow(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	rect.left += 10;
	rect.top += 10;
	rect.right -= 10;
	rect.bottom -= 10;

	MapWindowPoints(hWnd, NULL, (POINT*)&rect, 2);
	ClipCursor(&rect);
}

//-----------------
//�}�E�X���
//-----------------
void ReleaseCursor(void)
{
	ClipCursor(NULL); // ����������
}

//-----------------
// �}�E�X�ؑ�
//-----------------
void CursorSwitch(SWITCH Switch)
{
	if (Switch==ON)
	{
		ReleaseCursor();
		while (ShowCursor(TRUE) < 0);
	}
	else
	{
		while (ShowCursor(FALSE) >= 0);
		ConfineCursorToWindow(GethWnd());
	}
}

//-------------------------
// �}�E�X�J�[�\���̈ʒu���擾����s
//-------------------------
POINT GetCursorPoint(void)
{
	POINT point;
	GetCursorPos(&point);
	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	if (ScreenToClient(GethWnd(), &point))
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// DirectX�̃T�C�Y���擾����
		float width = 0.0f;
		float height = 0.0f;
		IDirect3DSurface9* pBackBuffer = nullptr;
		if (SUCCEEDED(pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer)))
		{// �o�b�N�o�b�t�@�̎擾
			D3DSURFACE_DESC desc;
			pBackBuffer->GetDesc(&desc);
			width = (float)desc.Width;
			height = (float)desc.Height;
			pBackBuffer->Release();  // �擾��������
		}
		else
		{
			width = (float)SCREEN_WIDTH;
			height = (float)SCREEN_HEIGHT;
		}

		// �N���C�A���g�̈�̃T�C�Y���擾
		RECT clientRect;
		GetClientRect(GethWnd(), &clientRect);

		// �N���C�A���g���W��DirectX�̍��W�ɃX�P�[�����O
		float scaleX = width / (clientRect.right - clientRect.left);
		float scaleY = height / (clientRect.bottom - clientRect.top);

		int fixedX = static_cast<int>(point.x * scaleX);
		int fixedY = static_cast<int>(point.y * scaleY);

		// ���W��͈͓��ɌŒ�
		fixedX = max(0, min((int)width, fixedX));
		fixedY = max(0, min((int)height, fixedY));

		point.x = fixedX;
		point.y = fixedY;
	}

	return point;
}

//--------------------
//
//�f�o�b�O�p
//
//--------------------

//--------------------
//�f�o�b�O�\��
//--------------------
void DrawDebug(void)
{
	RECT rect = { 0,0,(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };
	char aStr[1024] = { '\0' };
	extern Camera g_camera[CAMERA_XNUM * CAMERA_YNUM];//�J�����Q��

	//������̑��
	snprintf(&aStr[0], sizeof(aStr), "FPS:%d\n", g_nCountFPS);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//������̑��
	snprintf(&aStr[0], sizeof(aStr), "�J�����A���O��Y:%f\n", g_camera[0].rot.y);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//������̑��
	snprintf(&aStr[0], sizeof(aStr), "�J�����A���O��X:%f\n", g_camera[0].rot.x);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//������̑��
	snprintf(&aStr[0], sizeof(aStr), "���_:%f %f %f\n", g_camera[0].posV.x, g_camera[0].posV.y, g_camera[0].posV.z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//������̑��
	snprintf(&aStr[0], sizeof(aStr), "�ڕW���_:%f %f %f\n", g_camera[0].posVDest.x, g_camera[0].posVDest.y, g_camera[0].posVDest.z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//������̑��
	snprintf(&aStr[0], sizeof(aStr), "�����_:%f %f %f\n", g_camera[0].posR.x, g_camera[0].posR.y, g_camera[0].posR.z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//������̑��
	snprintf(&aStr[0], sizeof(aStr), "�ڕW�����_:%f %f %f\n", g_camera[0].posRDest.x, g_camera[0].posRDest.y, g_camera[0].posRDest.z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;

	//������̑��
	snprintf(&aStr[0], sizeof(aStr), "����:%f\n", g_camera[0].fDistance);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += FONT_SIZE;
}

//*********************************************************************************************
// �x�N�g�����f�o�b�O�\������֐�
//*********************************************************************************************
void DebugPrintVector(const char* label, D3DXVECTOR3 vec)
{
	char buffer[128];
	snprintf(buffer, sizeof(buffer), "%s: x=%.2f, y=%.2f, z=%.2f\n", label, vec.x, vec.y, vec.z);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// ���l���f�o�b�O�\������֐�
//*********************************************************************************************
void DebugPrintNumber(const char* label, int nNumber)
{
	char buffer[128];
	snprintf(buffer, sizeof(buffer), "%s: %d\n", label, nNumber);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// ���l���f�o�b�O�\������֐�
//*********************************************************************************************
void DebugPrintBool(const char* label, bool bTrue)
{
	char buffer[128];
	snprintf(buffer, sizeof(buffer), "%s: %s\n", label, bTrue ? "true" : "false");
	OutputDebugString(buffer);
}

//*********************************************************************************************
// float���f�o�b�O�\������֐�
//*********************************************************************************************
void DebugPrintFloat(const char* label, float fNumber)
{
	char buffer[128];
	snprintf(buffer, sizeof(buffer), "%s: %.2f\n", label, fNumber);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// D3DXMATRIX���f�o�b�O�\������֐�
//*********************************************************************************************
void DebugPrintMatrix(const char* label, D3DXMATRIX mat)
{
	char buffer[512];
	snprintf(buffer, sizeof(buffer),
		"%s:\n"
		"[%.2f, %.2f, %.2f, %.2f]\n"
		"[%.2f, %.2f, %.2f, %.2f]\n"
		"[%.2f, %.2f, %.2f, %.2f]\n"
		"[%.2f, %.2f, %.2f, %.2f]\n",
		label,
		mat._11, mat._12, mat._13, mat._14,
		mat._21, mat._22, mat._23, mat._24,
		mat._31, mat._32, mat._33, mat._34,
		mat._41, mat._42, mat._43, mat._44);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// ��������f�o�b�O�\������֐�
//*********************************************************************************************
void DebugPrintString(const char* label, const char* str)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s: %s\n", label, str);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// 2D�x�N�g�����f�o�b�O�\������֐�
//*********************************************************************************************
void DebugPrintVector2(const char* label, D3DXVECTOR2 vec)
{
	char buffer[128];
	snprintf(buffer, sizeof(buffer), "%s: x=%.2f, y=%.2f\n", label, vec.x, vec.y);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// 4D�x�N�g�����f�o�b�O�\������֐�
//*********************************************************************************************
void DebugPrintVector4(const char* label, D3DXVECTOR4 vec)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s: x=%.2f, y=%.2f, z=%.2f, w=%.2f\n", label, vec.x, vec.y, vec.z, vec.w);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// �J���[���f�o�b�O�\������֐�
//*********************************************************************************************
void DebugPrintColor(const char* label, D3DXCOLOR color)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s: r=%.2f, g=%.2f, b=%.2f, a=%.2f\n", label, color.r, color.g, color.b, color.a);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// D3DXQUATERNION���f�o�b�O�\������֐�
//*********************************************************************************************
void DebugPrintQuaternion(const char* label, D3DXQUATERNION quat)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s: x=%.2f, y=%.2f, z=%.2f, w=%.2f\n", label, quat.x, quat.y, quat.z, quat.w);
	OutputDebugString(buffer);
}

//*********************************************************************************************
// HRESULT���f�o�b�O�\������֐�
//*********************************************************************************************
void DebugPrintHRESULT(const char* label, HRESULT hr)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s: 0x%08X\n", label, hr);
	OutputDebugString(buffer);
}