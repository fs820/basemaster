//---------------------------------------
//
//�Ǐ����̒�`�E�錾[wall.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _WALL_H_
#define _WALL_H_

#include"main.h"

#define WALL_FILE "data\\TXT\\wall.txt"       //�ʒu�ۑ��t�@�C��
#define WALL_TEX "data\\TEXTURE\\wall000.jpg" //�e�N�X�`��
#define WALL_MAX (512)                        //�ő吔
#define VT_MAX_WALL ((WALL_XNUM + 1) * (WALL_YNUM + 1))                 //���_��
#define INDEX_NUM_WALL (((WALL_XNUM+1)*2)*WALL_YNUM+(WALL_YNUM-1)*2)    //�C���f�b�N�X��
#define POLYGON_NUM_WALL (WALL_XNUM*WALL_YNUM*2+(WALL_YNUM-1)*4)        //�|���S����

#define BACK_WALL_ALPHA (0.5f)

//�ǂ̃^�C�v
typedef enum
{
	WTYPE_NORMAL = 0,
	WTYPE_INVISIBLE,
	WTYPE_MAX
}WALL_TYPE;

//�����Ȃ��Ǎ\����
typedef struct
{
	D3DXVECTOR3 pos;     //�ʒu
	D3DXVECTOR3 rot;     //��]
	D3DXMATRIX mtxWorld; //�}�g���b�N�X
	float Width;         //��
	float Height;        //����
	bool bAlpha;         //����
	bool bBack;          //�o�b�N�̕�
	bool bUse;           //�g�p
	WALL_TYPE Type;      //�^�C�v
}Wall;

void InitWall(void);//����������
void UninitWall(void);//�I������
void UpdateWall(void);//�X�V����
void DrawWall(void);//�`�揈��
int SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, WALL_TYPE Type); //�ݒu
Wall* GetWall(void);                                               //�擾
void CollisionWall(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float Width, float fHeight); // �����蔻��
bool TriggerWall(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fHeight);   // �����蔻��
void LoadWall(void); //�ǂݍ���
void SaveWall(void); //��������
void UpdateWallEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot);    //�G�f�B�^�[

int GetWallNum(WALL_TYPE);                                               //��
int GetWallType(void);                                                   //�^�C�v
int GetDelWall(void);                                                    //�^�C�v

#endif _WALL_H_