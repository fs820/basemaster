//---------------------------------------
//
//DirectX�����̒�`�E�錾[stddx.h]
//Author fuma sato
//
//---------------------------------------
#ifndef _STDDX_H_
#define _STDDX_H_
#include"main.h"

// �r���{�[�h�^�C�v
typedef enum
{
    BILLBOARD_NONE = 0,          // �r���{�[�h�Ȃ�
    BILLBOARD_FULL,              // �t���r���{�[�h�i�J�����Ɋ��S�Ǐ]�j
    BILLBOARD_Y_AXIS,            // Y���Œ�i�����Ŕ^�C�v�j
    BILLBOARD_XZ_AXIS,           // X/Z���Œ�i�n�ʂɉ����^�C�v�j
    BILLBOARD_TYPE_MAX           // ��ނ̏��
}BILLBOARD_TYPE;

// �`��^�C�v�e���v���[�g
typedef enum
{
    DRAW_NONE = 0,      // �����_�[�X�e�[�g�Ȃ�
    DRAW_NORMAL,        // �ʏ�
    DRAW_BILLBOARD,     // �r���{�[�h
    DRAW_EFFECT,        // �G�t�F�N�g
    DRAW_SHADOW,        // �V���h�E
    DRAW_BARRIER,       // �o���A
    DRAW_UI,            // UI�v�f�iHUD�A���j���[�Ȃǁj
    DRAW_TRANSPARENT,   // ����/�������I�u�W�F�N�g
    DRAW_ADDITIVE,      // ���Z�����i���A�p�[�e�B�N���Ȃǁj
    DRAW_SUBTRACTIVE,   // ���Z����
    DRAW_MULTIPLY,      // ��Z����
    DRAW_WIREFRAME,     // ���C���[�t���[���\��
    DRAW_SKYBOX,        // �X�J�C�{�b�N�X
    DRAW_WATER,         // ����
    DRAW_OUTLINE,       // �A�E�g���C���`��
    DRAW_TEMP_MAX       // �e���v���[�g�̏��
} DRAW_TEMP;

// �����_�[�X�e�[�g�Ǘ��\����
typedef struct
{
    bool Lighting = false;               // ���C�e�B���O
    D3DCULL cullMode = D3DCULL_NONE;     // �J�����O���[�h
    bool alphaBlendEnable = false;       // �A���t�@�u�����h
    D3DBLENDOP blendOp = D3DBLENDOP_ADD; // �u�����h���Z
    D3DBLEND srcBlend = D3DBLEND_ZERO;   // �\�[�X�u�����h
    D3DBLEND destBlend = D3DBLEND_ZERO;  // �f�X�e�B�l�[�V�����u�����h
    bool zEnable = false;                // Z�o�b�t�@
    bool zWriteEnable = false;           // Z�o�b�t�@��������
    D3DCMPFUNC zFunc = D3DCMP_NEVER;     // Z�o�b�t�@��r�֐�
	float zBias = 0.0f;                  // Z�o�C�A�X
    bool alphaTestEnable = false;        // �A���t�@�e�X�g
    DWORD alphaRef = 0;                  // �A���t�@�e�X�g�Q�ƒl
    D3DCMPFUNC alphaFunc = D3DCMP_NEVER; // �A���t�@�e�X�g��r�֐�
    bool antialiaseLineEnable = false;   // �A���`�G�C���A�X
    bool stencilEnable = false;          // �X�e���V���e�X�g
} RSOP;

// �T���v���[�X�e�[�g�Ǘ��\����
typedef struct
{
    DWORD maxAnisotropy = 1;                             // �ٕ����t�B���^�̍ő�l
    D3DTEXTUREFILTERTYPE minFilter = D3DTEXF_NONE;       // �e�N�X�`���~�j�t�B�P�[�V�����t�B���^
    D3DTEXTUREFILTERTYPE magFilter = D3DTEXF_NONE;       // �e�N�X�`���}�O�j�t�B�P�[�V�����t�B���^
    DWORD maxMipLevel = 0;                               // �~�b�v�}�b�v���x��
    float mipMapLODBias = 0.0f;                          // �~�b�v�}�b�vLOD�o�C�A�X
    D3DTEXTUREFILTERTYPE mipFilter = D3DTEXF_NONE;       // �~�b�v�}�b�v�t�B���^
    D3DTEXTUREADDRESS addressU = D3DTADDRESS_WRAP;       // U�����̃A�h���X���[�h
    D3DTEXTUREADDRESS addressV = D3DTADDRESS_WRAP;       // V�����̃A�h���X���[�h
    D3DTEXTUREADDRESS addressW = D3DTADDRESS_WRAP;       // W�����̃A�h���X���[�h (3D�e�N�X�`���p)
    D3DCOLOR borderColor = D3DCOLOR_ARGB(255, 255, 255, 255); // �{�[�_�[�J���[ (Clamp���̋��E�F)
} SSOP;

// �e�N�X�`���X�e�[�W�X�e�[�g�Ǘ��\����
typedef struct
{
    D3DTEXTUREOP colorOp = D3DTOP_MODULATE;                           // �J���[�I�y���[�V����
    DWORD colorArg0 = D3DTA_CURRENT;                                  // �I�y���[�V�����̈���0 (�ꕔ�̃I�y���[�V�����p)
    DWORD colorArg1 = D3DTA_TEXTURE;                                  // �I�y���[�V�����̈���1
    DWORD colorArg2 = D3DTA_DIFFUSE;                                  // �I�y���[�V�����̈���2
    D3DTEXTUREOP alphaOp = D3DTOP_MODULATE;                           // �A���t�@�I�y���[�V����
    DWORD alphaArg0 = D3DTA_CURRENT;                                  // �I�y���[�V�����̈���0 (�ꕔ�̃I�y���[�V�����p)
    DWORD alphaArg1 = D3DTA_TEXTURE;                                  // �I�y���[�V�����̈���1
    DWORD alphaArg2 = D3DTA_DIFFUSE;                                  // �I�y���[�V�����̈���2
    DWORD texCoordIndex = 0;                                          // �e�N�X�`���̃C���f�b�N�X
    D3DTEXTURETRANSFORMFLAGS texTureTransFormFlags = D3DTTFF_DISABLE; // �e�N�X�`���ϊ�
    DWORD resultArg = D3DTA_CURRENT;                                  // ���ʂ̊i�[��
    D3DCOLOR constant = D3DCOLOR_ARGB(255, 255, 255, 255);            // �萔�l (D3DTA_CONSTANT���g�p��)
} TSSOP;

// �^�ϊ��p�̋��p��
typedef union
{
	int Int;                // ����
	float Float;            // ���������_
	double Double;          // �{���x���������_
	long Long;              // ������
	short Short;            // �Z����
	unsigned int UInt;      // �����Ȃ�����
	unsigned long ULong;    // �����Ȃ�������
	unsigned short UShort;  // �����Ȃ��Z����
	unsigned char UChar[8]; // �����Ȃ�����
	signed char SChar[8];   // �����t������
	wchar_t WChar[8];       // ���C�h����
	TCHAR TChar[8];         // �e�L�X�g����
	char Char[8];           // ����
	bool Bool;              // �^�U�l
	DWORD Dword;            // DWORD
	WORD Word;              // WORD
	BYTE Byte[8];           // BYTE
}TYPE_CONVERSION;

//�v���g�^�C�v�錾
HRESULT CreateVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, UINT VerrexSize, UINT VertexCount, DWORD Usage, D3DPOOL Pool);                                  // 2D���_�o�b�t�@�̍쐬
HRESULT CreateVertex3D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, UINT VerrexSize, UINT VertexCount, DWORD Usage, D3DPOOL Pool);                                  // 3D���_�o�b�t�@�̍쐬
HRESULT CreateTexture(LPDIRECT3DTEXTURE9* ppTextureBuff, const String* TexPass, int TextureCount, D3DXIMAGE_INFO* pImageInfo, DWORD Filter, D3DPOOL Pool); // �e�N�X�`���̍쐬
HRESULT SetVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, DWORD LockFlags, int OffsetIdx, int BuffMax, int yDivisions, int xDivisions, float Width, float Height, float rhw, D3DXCOLOR col, int TexCount, D3DXIMAGE_INFO* pImageInfo, bool PosSenter); // 2D���_�o�b�t�@�̐ݒ�
HRESULT SetVertex3D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, DWORD LockFlags, int OffsetIdx, int BuffMax, int yDivisions, int xDivisions, float Width, float Height, D3DXCOLOR col, int TexCount, D3DXIMAGE_INFO* pImageInfo, bool PosSenter);            // 3D���_�o�b�t�@�̐ݒ�
HRESULT SetIndex(LPDIRECT3DINDEXBUFFER9* ppIdxBuff, int yDivisions, int xDivisions);                                                                                                                                                                 // �C���f�b�N�X�o�b�t�@�̐ݒ�
HRESULT GetDrawOperation(RSOP* rsOp, SSOP* ssOp, unsigned int SamplerCount, TSSOP* tssOp, unsigned int TextureStateCount);                         // �`����̎擾
HRESULT SetDrawOperation(RSOP* rsOp, SSOP* ssOp, unsigned int SamplerCount, TSSOP* tssOp, unsigned int TextureStateCount);                         // �`����̐ݒ�
HRESULT GetDrawOperationTemp(RSOP* rsOp, SSOP* ssOp, unsigned int SamplerCount, TSSOP* tssOp, unsigned int TextureStateCount, DRAW_TEMP DrawTemp); // �`����e���v���[�g�̎擾
#ifdef _STD_UNSECURE_USE // �Z�L�����e�B�[�ʂȂǂł������߂��Ȃ��֐����g�p����ꍇ
HRESULT SetDrawOperationTemp(unsigned int SamplerCount, unsigned int TextureStateCount, DRAW_TEMP DrawTemp);                                       // �`����e���v���[�g�̐ݒ�
#endif // _STD_UNSECURE_USE
HRESULT GetRenderOperation(RSOP* rsOp);   // �����_�[���̎擾
_NODISCARD RSOP GetRenderOperation(void); // �����_�[���̎擾(����)
HRESULT SetRenderOperation(RSOP* rsOp);   // �����_�[���̐ݒ�
HRESULT GetSamplerOperation(SSOP* ssOp, unsigned int SamplerCount); // �T���v���[���̎擾
_NODISCARD SSOP* GetSamplerOperation(unsigned int SamplerCount);    // �T���v���[���̎擾(����)
HRESULT SetSamplerOperation(SSOP* ssOp, unsigned int SamplerCount); // �T���v���[���̐ݒ�
HRESULT GetTextureStageOperation(TSSOP* tssOp, unsigned int TextureStateCount); // �e�N�X�`���X�e�[�W���̎擾
_NODISCARD TSSOP* GetTextureStageOperation(unsigned int TextureStateCount);     // �e�N�X�`���X�e�[�W���̎擾(����)
HRESULT SetTextureStageOperation(TSSOP* tssOp, unsigned int TextureStateCount); // �e�N�X�`���X�e�[�W���̐ݒ�
HRESULT DrawVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, LPDIRECT3DINDEXBUFFER9* ppIdxBuff, LPDIRECT3DTEXTURE9* ppTextureBuff, D3DPRIMITIVETYPE PrimitiveType, UINT VertexSize, UINT OffsetIdx, UINT PrimitiveCount);                                                                                                                              // 2D���_�o�b�t�@�̕`��
HRESULT DrawVertex3D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, LPDIRECT3DINDEXBUFFER9* ppIdxBuff, LPDIRECT3DTEXTURE9* ppTextureBuff, D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, D3DXVECTOR3* pScale, D3DXMATRIX* pMtxWorld, BILLBOARD_TYPE BillboardType, float zBias, D3DPRIMITIVETYPE PrimitiveType, UINT VertexSize, UINT OffsetIdx, UINT PrimitiveCount); // 3D���_�o�b�t�@�̕`��
#endif //_STDDX_H_