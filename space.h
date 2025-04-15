//---------------------------------------
//
//sphere�����̒�`�E�錾[space.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _SPACE_H_
#define _SPACE_H_

#include"main.h"

#define SPACE_TEX_MAX (2)//�e�N�X�`��
#define SPACE_RADIUS (10000.0f)//���a
#define SPACE_HNUM (256)//H������
#define SPACE_VNUM (64)//V������
#define SPACE_ANG (90.0f)//����

//����
#define VT_MAX_SP ((SPACE_HNUM + 1) * SPACE_VNUM+1)//���̒��_��*�c�̃��C����(�ɂ𔲂���)+�� (����)
#define INDEX_SP_NUM (((SPACE_HNUM+1)*2)*SPACE_VNUM+(SPACE_VNUM-2)*2-SPACE_HNUM)//���̒��_��*2��**�c�̃��C����(�ɂ𔲂���)+�k�ރ|���S��-�ɂ̕s�v��
#define POLYGON_SP_NUM (SPACE_HNUM*SPACE_VNUM*2+(SPACE_VNUM-2)*4-SPACE_HNUM)//�S�̂̕���*2*�k�ރ|���S��-�ɂ̕s�v��

const String TexPass[SPACE_TEX_MAX] =
{
	"data\\TEXTURE\\Base.jpg",
	"data\\TEXTURE\\Base.jpg"
};

const float TexScale[SPACE_TEX_MAX] =
{
	10.0f,
	10.0f
};

//Texture�\����
typedef struct
{
	String TexPass;
	D3DXVECTOR2 TexSize;
}TEXTURE;

//Space�\����
typedef struct
{
	D3DXMATRIX mtxWorld;//�}�g���b�N�X
	TEXTURE Texture[SPACE_TEX_MAX];
	bool bUse;//�g�p
}Space;

void InitSpace(void);   //����������
void UninitSpace(void); //�I������
void UpdateSpace(void); //�X�V����
void DrawSpace(void);   //�`�揈��
void SetSpace(void);    //�ݒ菈��

#endif _SPACE_H_