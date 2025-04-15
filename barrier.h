//---------------------------------------
//
//�o���A�����̒�`�E�錾[barrier.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BARRIER_H_
#define _BARRIER_H_

#include"main.h"

#define BARRIER_FILE "data\\txt\\sato\\barrier.txt"                                //�t�@�C��
#define BARRIER_TEX "data\\TEXTURE\\Base.jpg"                                      //�e�N�X�`��
#define BARRIER_MAX (64)                                                           //�ő吔
#define BARRIER_WIDTH (350.0f)                                                     //����
#define BARRIER_HEIGHT (400.0f)                                                    //��
#define BARRIER_Z (0.0f)                                                           //���s
#define BARRIER_Z_DEF (0.0f)                                                       //�e�N�X�`���p
#define BARRIER_XNUM (1)                                                           //������
#define BARRIER_YNUM (1)                                                           //��������
#define VT_MAX_BARRIER ((BARRIER_XNUM + 1) * (BARRIER_YNUM + 1))                   //���_��
#define INDEX_NUM_BARRIER (((BARRIER_XNUM+1)*2)*BARRIER_YNUM+(BARRIER_YNUM-1)*2)   //�C���f�b�N�X��
#define POLYGON_NUM_BARRIER (BARRIER_XNUM*BARRIER_YNUM*2+(BARRIER_YNUM-1)*4)       //�|���S����
#define BARRIER_SCALE_MIN (0.5f)                                                   //�傫���ŏ�
#define BARRIER_SCALE_MAX (2.0f)                                                   //�傫���ő�

#define BARRIER_TOP_MAX (3)                                                   //�W

// �o���A�̌����ڕς��
#define BARRIER_DEF_COLOR D3DXCOLOR(0.3f,0.3f,0.3f,0.2f)                           //�f�t�H���g�J���[
#define BARRIER_DRAW_NUM (10)
#define BARRIER_ALPHA_SPEED (0.005f)
#define BARRIER_ALPHA_MIN (0.2f)
#define BARRIER_ALPHA_MAX (0.5f)
#define BARRIER_ALPHA_REACTION (0.8f)

//�o���A�^�C�v
typedef enum
{
	BTYPE_1 = 0,
	BTYPE_MAX
}BTYPE;

typedef struct
{
	D3DXVECTOR3 pos;      //�ʒu
	D3DXVECTOR3 rot;      //����
	float Width;          //��
	float Height;         //����
	D3DXMATRIX mtxWorld;  //�}�g���b�N�X
	BTYPE Type;           //�^�C�v
	SWITCH Switch;        //Switch
	D3DXCOLOR col;        //�J���[
	bool bTop;            //�W
	bool bUse;            //�g�p
}Barrier;

void InitBarrier(void);                                                                                                 //����������
void UninitBarrier(void);                                                                                               //�I������
void UpdateBarrier(void);                                                                                               //�X�V����
void DrawBarrier(void);                                                                                                 //�`�揈��
int SetBarrier(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, BTYPE Type, bool bTop, float Corner);       //�ݒ�
void EndBarrier(BTYPE Type);                                                                                            //�j��
Barrier* GetBarrier(void);                                                                                              //�擾
void BarrierSwitch(BTYPE Type,SWITCH Switch);                                                                           //Switch
void CollisionBarrier(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fRadius, float fHeight);                           //�Ǎ���
void ReflectionBarrier(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius);                     //����
#endif _BARRIER_H_