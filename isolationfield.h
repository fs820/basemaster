//---------------------------------------
//
//�u���o���A�����̒�`�E�錾[isolationfield.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ISOLATION_FIELD_H_
#define _ISOLATION_FIELD_H_

#include"main.h"
#include"barrier.h"

#define ISOLATION_FIELD_FILE "data\\txt\\sato\\Isolation.txt" //�ő吔
#define ISOLATION_FIELD_MAX (16) //�ő吔
#define ISOLATION_FACE_MAX (8)   //�ʂ̍ő吔

#define ISOLATION_WIDTH_MIN (10.0f)
#define ISOLATION_WIDTH_MAX (1000.0f)
#define ISOLATION_HEIGHT_MIN (10.0f)
#define ISOLATION_HEIGHT_MAX (1000.0f)
#define ISOLATION_DEPTH_MIN (10.0f)
#define ISOLATION_DEPTH_MAX (1000.0f)

//�u���o���A�̗񋓌^��`
typedef enum
{
	ISO_TYPE_1 = 0,
	ISO_TYPE_MAX
}ISO_TYPE;

//�u���o���A�\����
typedef struct
{
	D3DXVECTOR3 pos;                    //�ʒu
	D3DXVECTOR3 rot;                    //����
	float Width;                        //��
	float Height;                       //����
	float Depth;                        //���s
	float Corner;                       //�p
	ISO_TYPE Type;                      //�^�C�v
	SWITCH Switch;                      //Switch
	bool bUse;                          //�g�p
}IsolationField;

void InitIsolationField(void);                                                                                                 //����������
void UninitIsolationField(void);                                                                                               //�I������
void UpdateIsolationField(void);                                                                                               //�X�V����
void DrawIsolationField(void);                                                                                                 //�`�揈��
int SetIsolationField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, float Depth, float Corner, ISO_TYPE Type);  //�ݒ�
IsolationField* GetIsolationField(void);                                                                                       //�擾
void IsolationFieldSwitch(ISO_TYPE Type, SWITCH Switch);                                                                       //�I���I�t

void UpdateIsolationFieldEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot); //�u���o���A�G�f�B�^�[
void SaveIsolationField(void); //��������
void LoadIsolationField(void); //�ǂݍ���

int GetIsonumber(void);    //�^�C�v�i���o�[
int GetIsoDelnumber(void); //�f���[�g�i���o�[
#endif _ISOLATION_FIELD_H_