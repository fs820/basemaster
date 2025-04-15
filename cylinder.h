//---------------------------------------
//
//�|���S���\�������̒�`�E�錾[cylinder.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include"main.h"

#define CYLINDER_TEX "data\\TEXTURE\\wall000.jpg"
#define CYLINDER_MAX (64)
#define CYLINDER_RADIUS (450.0f)
#define CYLINDER_HEIGHT (100.0f)
#define CYLINDER_HNUM (16)
#define CYLINDER_VNUM (1)
#define VT_MAX_CYLI ((CYLINDER_HNUM + 1) * (CYLINDER_VNUM + 1))
#define INDEX_CYLI_NUM (((CYLINDER_HNUM+1)*2)*CYLINDER_VNUM+(CYLINDER_VNUM-1)*2)
#define POLYGON_CYLI_NUM (CYLINDER_HNUM*CYLINDER_VNUM*2+(CYLINDER_VNUM-1)*4)

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	bool bAlpha;
	bool bUse;
}Cylinder;

void InitCylinder(void);//�|���S���̏���������
void UninitCylinder(void);//�|���S���̏I������
void UpdateCylinder(void);//�|���S���̍X�V����
void DrawCylinder(void);//�|���S���̕`�揈��
void SetCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif _MESHFIELD_H_