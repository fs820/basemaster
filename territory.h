//---------------------------------------
//
//�e���g���[�����̒�`�E�錾[territory.h]
//Author fuma sato
//
//---------------------------------------
#ifndef _TERRITORY_H_
#define _TERRITORY_H_
#include "main.h"

#define TERRITORY_FILE  "data\\txt\\sato\\territory.txt"
#define TERRITORY_TEX   "data\\TEXTURE\\shadow000.jpg"
#define TERRITORY_MAX (256)

//�񋓌^
typedef enum
{
	TERRITORY_TYPE_A = 0,
	TERRITORY_TYPE_B,
	TERRITORY_TYPE_BOSS,
	TERRITORY_TYPE_MAX
}TERRITORY_TYPE;

//�e���g���[�\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	float fRadius;			// ���a
	TERRITORY_TYPE Type;	// �^�C�v
	bool bUse;				// �g�p
#ifdef _DEBUG
	D3DXCOLOR col;          // ����
	D3DXMATRIX mtxWorld;    // �}�g���b�N�X
#endif // _DEBUG
}Territory;

void InitTerritory(void);                                              //����������
int SetTerritory(D3DXVECTOR3 pos, float fRadius, TERRITORY_TYPE Type); //�ݒ菈��
int LinkTerritory(int* nIdx, TERRITORY_TYPE Type);                     //�����N�ݒ菈��
int CheckTerritory(int* nIdx, int Quantity, D3DXVECTOR3 pos);          //�`�F�b�N����
D3DXVECTOR3 GetTerritoryPos(int nIdx);                                 //�ʒu�̎擾

void UpdateTerritoryEdit(D3DXVECTOR3 Epos);                    //�e���g���[�[�G�f�B�^�[
void LoadTerritory(void);                                      //�ݒu���ǂݍ���
void SaveTerritory(void);                                      //�ݒu��񏑂��o��
TERRITORY_TYPE GetTerritoryType(void);                         //�^�C�v�̎擾
int GetDelTerritory(void);                                     //�f���[�g�i���o�[�̎擾

void UninitTerritory(void); //�I������
#ifdef _DEBUG
void DrawTerritory(void); //�`�揈��
#endif // _DEBUG

#endif //_TERRITORY_H_