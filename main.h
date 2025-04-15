//------------------------------------------
//
//���C���̏����̒�`�E�錾[main.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_

#define DIRECTINPUT_VERSION 0x0800
#define NOMINMAX  // ��max,min �𖳌��� sato
#include <windows.h>
#include <algorithm>      // �Vmin,max��L���� sato
using std::clamp;         // clamp
using std::max;           // max
using std::min;           // min
#include<stdio.h>
#include<time.h>
#include <strsafe.h>
#include <shellscalingapi.h>
#include <shlwapi.h>
#include"d3dx9.h"
#include"dinput.h" // ���͂ɕK�v�ȃt�@�C��
#include"Xinput.h" // Xinput(�p�b�h����)
#include"xaudio2.h"

// Debug sato
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

// ���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"uuid.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Shcore.lib")


// �}�N����`
#define FPS (60) // �X�V�X�s�[�h

// ���O
#define CLASS_NAME "WindowClass" // �N���X��
#define WINDOW_NAME "C_Bese_Master" // �E�C���h�E��
#define DATA_FILE "data\\DATA\\data.bin"//�X�R�A�ۑ��p�t�@�C��

// �傫��
#define SCREEN_WIDTH (2560.0f)  // ��
#define SCREEN_HEIGHT (1440.0f) // ����

// ���_
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX8) // ���_�t�H�[�}�b�g
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX8) // ���_�t�H�[�}�b�g
#define VT_MAX (4)//���_��
#define UV_DEF (1.0f) // �e�N�X�`���̃f�t�H���g�̃T�C�Y
#define TEXTURE_MAX (8) // �e�N�X�`���̍ő吔

#define ROT_RAND ((float)(rand() % 629) - 314.0f) * 0.01f

// �R���g���[���[
#define	ELE_CON "GP30X-Dinput"//�w�Z�x���R���g���[��
#define PS_CON "Wireless Controller"//�v���X�e4
#define NIN_CON "Pro Controller"//�v���R��

//�f�o�b�O�p
#define FPS_TIME (500)
#define FONT_SIZE (20)

// ��{�l
#define NOR D3DXVECTOR3(0.0f, 0.0f, -1.0f) // ��{�@���x�N�g��

// �����}�N��
#define FOR(Max) for(int i = 0; i < Max; i++)
#define FOR_NAME(Name, Max) for(int Name = 0; Name < Max; Name++)
#define FOR_SIZE(Size, Max) for(Size i = 0; i < Max; i++)
#define FOR_SIZE_NAME(Size, Name, Max) for(Size Name = 0; Name < Max; Name++)
#define FOR_MIN(Min, Max) for(int i = Min; i < Max; i++)
#define FOR_NAME_MIN(Name, Min, Max) for(int Name = Min; Name < Max; Name++)
#define FOR_SIZE_MIN(Size, Min, Max) for(Size i = Min; i < Max; i++)
#define FOR_SIZE_NAME_MIN(Size, Name, Min, Max) for(Size Name = Min; Name < Max; Name++)

// �G���[�`�F�b�N�p�}�N��
#define RETURN_HR(a) HRESULT hr = a; if (FAILED(hr)) { Error(hr); return hr; }
#define CHECK_HR(a) HRESULT hr = a; if (FAILED(hr)) { Error(hr); }
#define SAFE_RELEASE(p) { if ((p) != nullptr) { (p)->Release(); (p) = nullptr; } }

// ��ʂ̎��
typedef enum
{
	MODE_TITLE = 0,//�^�C�g��
	MODE_TUTO,//�`���[�g���A��
	MODE_GAME,//�Q�[��
	MODE_RESULT,//���U���g
	MODE_RANK,//�����L���O
	MODE_OPTION,//�I�v�V����
	MODE_MAX//���[�h��
} MODE;

//Switch
typedef enum
{
	OFF = 0,
	ON,
	SWITCH_MAX
}SWITCH;

//������\���� sato
typedef struct
{
	char aName[64]; //�t�@�C���p�X
}String;

// ���_���̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;    // ���W
	float rhw;          // ���W�ϊ��W��(1.0f)
	D3DCOLOR col;       // �J���[
	D3DXVECTOR2 tex[8]; // �e�N�X�`�����W
} VERTEX_2D;

// ���_���̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;    // ���W
	D3DXVECTOR3 nor;    //�@���x�N�g��
	D3DCOLOR col;       // �J���[
	D3DXVECTOR2 tex[8]; // �e�N�X�`�����W
} VERTEX_3D;

// �v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // �E�C���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hInstanse, HWND hWnd, BOOL bWindow = TRUE); // ����������
void Uninit(void); // �I������
void Update(void); // �X�V����
void Render(void); // �`�揈��
LPDIRECT3DDEVICE9 GetDevice(void); // �f�o�C�X�̎擾
void SetMode(MODE mode);//���[�h�ݒ�
MODE GetMode(void);//���[�h�擾
HWND GethWnd(void);//�n���h���擾
HINSTANCE GethInstanse(void);//�C���X�^���X�擾

void Error(HRESULT hr);// �G���[

void ConfineCursorToWindow(HWND hwnd);//�J�[�\������
void ReleaseCursor(void);//�J�[�\���������
void CursorSwitch(SWITCH Switch); // �J�[�\���̐ؑ�

POINT GetCursorPoint(void); // �J�[�\���̈ʒu���擾����

// �ėp�֐�
float NormalizeAngle(float angle);
void SpherePos(D3DXVECTOR3& OutPos, D3DXVECTOR3 BasePos, D3DXVECTOR3 rot, float Length);

//�f�o�b�O�p
void DrawDebug(void);

// ���O�Ƀf�o�b�N�\������֐��S
void DebugPrintVector(const char* label, D3DXVECTOR3 vec);         // �x�N�g�����f�o�b�O�\������֐�
void DebugPrintNumber(const char* label, int nNumber);             // ���l���f�o�b�O�\������֐�
void DebugPrintBool(const char* label, bool bTrue);                // �^�U���f�o�b�N�\������֐�
void DebugPrintFloat(const char* label, float fNumber);            // float���f�o�b�O�\������֐�
void DebugPrintMatrix(const char* label, D3DXMATRIX mat);          // D3DXMATRIX���f�o�b�O�\������֐�
void DebugPrintString(const char* label, const char* str);         // ��������f�o�b�O�\������֐�
void DebugPrintVector2(const char* label, D3DXVECTOR2 vec);        // 2D�x�N�g�����f�o�b�O�\������֐�
void DebugPrintVector4(const char* label, D3DXVECTOR4 vec);        // 4D�x�N�g�����f�o�b�O�\������֐�
void DebugPrintColor(const char* label, D3DXCOLOR color);          // �J���[���f�o�b�O�\������֐�
void DebugPrintQuaternion(const char* label, D3DXQUATERNION quat); // D3DXQUATERNION���f�o�b�O�\������֐�
void DebugPrintHRESULT(const char* label, HRESULT hr);             // HRESULT���f�o�b�O�\������֐�

#endif // _MAIN_H_