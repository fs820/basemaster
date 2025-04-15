//---------------------------------------
//
// �A�C�e�������̒�`�E�錾[item.h]
// Author fuma sato
//
//---------------------------------------

#ifndef _ITEM_H_
#define _ITEM_H_

#include"main.h"
#include"game.h"

#define IMANAGER_FILE "data\\txt\\sato\\itemManager.txt"
#define ITEM_FILE   "data\\txt\\sato\\Item.txt"

#define ITEM_MAX (32)					// �ő吔
#define ITEM_SUBSET_MAX (2)             // �T�u�Z�b�g��
#define ITEM_TEXTURE_MAX (2)			// �e�N�X�`����
#define ITEM_SCALE_MAX (20.0f)			// �傫���̍ő�
#define ITEM_SCALE_MIN (0.5f)			// �傫���̍ŏ�
#define ITEM_TRIGGER_SPACE (200.0f)		// �擾�\�͈�
#define ITEMUI_TEXTUREW_MAX (90.0f)	// �A�C�e��UI�̉���			misaki
#define ITEMUI_TEXTUREH_MAX (100.0f)	// �A�C�e��UI�̏c��			misaki
#define ITEMUI_DISTANCE_MAX (8.0f)		// �A�C�e��UI���m�̋���		misaki

// �A�C�e���^�C�v�̗񋓌^��`
typedef enum
{
	ITYPE_TEST = 0,
	ITYPE_MAX
}ITYPE;

// �A�C�e�����f���\����
typedef struct
{
	String Pass;                                    // �t�@�C���p�X�̊i�[
	LPD3DXMESH pMesh;                               // ���b�V��
	LPD3DXBUFFER pBuffMat;                          // �}�e���A��
	DWORD dwNumMat;                                 // �}�e���A����
	int TexIdx[ITEM_SUBSET_MAX];                  //�}�e���A���C���f�b�N�X
	D3DXVECTOR3 vtxMin;                             // �ŏ�
	D3DXVECTOR3 vtxMax;                             // �ő�
	int nNumVtx;                                    // ���_��
	DWORD sizeFVF;                                  // �T�C�Y
	BYTE* pVtxBuff;                                 // �o�b�t�@�̃|�C���^
}ItemModel;

// �A�C�e���}�l�[�W���[�\����
typedef struct
{
	int ItemNum;						            // �ǂݍ��ރt�@�C����
	ItemModel ItemModel[ITYPE_MAX];		            // �t�@�C���̊i�[
	LPDIRECT3DTEXTURE9 apTexture[ITEM_TEXTURE_MAX]; // �e�N�X�`��
}ItemManager;

// �A�C�e���\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	float fLength;			// ����
	D3DXVECTOR3 scale;		// �傫��
	D3DXMATRIX mtxWorld;	// �}�g���b�N�X
	int nIdxShadow;			// �e
	int nRadarIdx;			// ���[�_�[�̃C���f�b�N�X
	ITYPE Type;				// �^�C�v���ʎq
	bool bAlpha;			// ����
	bool bUse;				// �g�p
}Item;

void InitItem(void);															// ����������
void UninitItem(void);															// �I������
void UpdateItem(void);															// �X�V����
void DrawItem(void);															// �`�揈��
void DrawItemUI(void);															// �A�C�e��UI�̕`�揈�� misaki
void SetItemManager(ItemManager ItemManager);									// �ǂݍ��񂾃t�@�C�������i�[����
int SetItem(ITYPE Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);	// �A�C�e����ݒu����
Item* GetItem(void);															// �A�C�e���̎擾
int GetItemNum(void);															// ��ސ��̎擾
bool TriggerItem(D3DXVECTOR3 pos, Item** pItem);								// �����蔻��
char* GetItemName(int nNumber);													// �t�@�C���p�X�̎擾(Debug�\���p)

void LoadItemManager(void);														// �t�@�C�����ǂݍ���
void LoadItem(void);															// �ݒu���ǂݍ���
void SaveItem(void);															// �ݒu��񏑂��o��
																				   
void UpdateItemEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot);						// �A�C�e���G�f�B�^�[
int GetInumber(void);															// ���݂�X�t�@�C���i���o�[
int GetIDelnumber(void);														// ���݂̃f���[�g�i���o�[

#endif _ITEM_H_