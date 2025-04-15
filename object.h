//---------------------------------------
//
//�I�u�W�F�N�g�����̒�`�E�錾[object.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include"main.h"
#include"game.h"

#define OMANAGER_FILE "data\\txt\\sato\\objectManager.txt"
#define OBJECT_FILE   "data\\txt\\sato\\object.txt"

#define OBJECT_MAX (64)          //�ő吔
#define OBJECT_SUBSET_MAX (4)    //�T�u�Z�b�g��
#define OBJECT_TEXTURE_MAX (9)   //�e�N�X�`����
#define OBJECT_SCALE_MAX (20.0f) //�傫���̍ő�
#define OBJECT_SCALE_MIN (0.5f)  //�傫���̍ŏ�

#define ROKET_TRIGGER_SPACE (300.0f) //�g���K�[�X�y�[�X

//�I�u�W�F�N�g�^�C�v�̗񋓌^��`
typedef enum
{
	OTYPE_TEST = 0,
	OTYPE_MAX
}OTYPE;

//�I�u�W�F�N�g���f���\����
typedef struct
{
	String Pass;                                      //�t�@�C���p�X�̊i�[
	LPD3DXMESH pMesh;                                 //���b�V��
	LPD3DXBUFFER pBuffMat;                            //�}�e���A��
	DWORD dwNumMat;                                   //�}�e���A����
	int TexIdx[OBJECT_SUBSET_MAX];                  //�}�e���A���C���f�b�N�X
	D3DXVECTOR3 vtxMin;                               //�ŏ�
	D3DXVECTOR3 vtxMax;                               //�ő�
	int nNumVtx;                                      //���_��
	DWORD sizeFVF;                                    //�T�C�Y
	BYTE* pVtxBuff;                                   //�o�b�t�@�̃|�C���^
}ObjectModel;

//�I�u�W�F�N�g�}�l�[�W���[�\����
typedef struct
{
	int ObjectNum;                      //�ǂݍ��ރt�@�C����
	ObjectModel ObjectModel[OTYPE_MAX]; //�t�@�C���̊i�[
	LPDIRECT3DTEXTURE9 apTexture[OBJECT_TEXTURE_MAX]; //�e�N�X�`��
}ObjectManager;

//�I�u�W�F�N�g�\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//��]
	float fLength;			//����
	float fHeight;			//����
	D3DXVECTOR3 scale;		//�傫��
	D3DXMATRIX mtxWorld;	//�}�g���b�N�X
	int nIdxShadow[2];		//�e
	OTYPE Type;				//�^�C�v���ʎq
	bool bAlpha;			//����
	bool bUse;				//�g�p
	int nIdxRadar;			// ���[�_�[
}Object;

void InitObject(void);    //����������
void UninitObject(void);  //�I������
void UpdateObject(void);  //�X�V����
void DrawObject(void);    //�`�揈��
void SetObjectManager(ObjectManager ObjectManager);                             //�ǂݍ��񂾃t�@�C�������i�[����
int SetObject(OTYPE Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale); //�I�u�W�F�N�g��ݒu����
Object* GetObject_EX(void);       //�I�u�W�F�N�g�̎擾
int GetObjectNum(void);           //��ސ��̎擾
int GetObjectIdx(OTYPE Type, int Number);  //�C���f�b�N�X�̎擾

bool CollisionObject(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius, float fHeight, Object** pObject); //�����蔻��
bool TriggerObject(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld,  float fRadius);                                                       //�����蔻��
bool InsideObject(OTYPE Type, D3DXVECTOR3 pos, Object** pObject);                                                                                    //�����蔻��

void LoadObjectManager(void); //�t�@�C�����ǂݍ���
void LoadObject(void);        //�ݒu���ǂݍ���
void SaveObject(void);        //�ݒu��񏑂��o��

char* GetObjectName(int nNumber);                          //�t�@�C���p�X�̎擾(Debug�\���p)
void UpdateObjectEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot); //�I�u�W�F�N�g�G�f�B�^�[
int GetOnumber(void);                                      //���݂�X�t�@�C���i���o�[
int GetODelnumber(void);                                   //���݂̃f���[�g�i���o�[

#endif //_OBJECT_H_