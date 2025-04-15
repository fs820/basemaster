//---------------------------------------
//
//�C���^���N�g�����̒�`�E�錾[interact.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _INTERACT_H_
#define _INTERACT_H_

#include"main.h"

#define INTERACT_TEX_KEY "data\\TEXTURE\\Base.jpg"          //�L�[�{�[�h�e�N�X�`��
#define INTERACT_TEX_JOY "data\\TEXTURE\\Base.jpg"          //�R���g���[���[�e�N�X�`��

//�C���^���N�g�^�C�v�̗񋓌^��`
typedef enum
{
	INTERACT_TYPE_KEY = 0,
	INTERACT_TYPE_JOY,
	INTERACT_TYPE_MAX
}INTERACT_TYPE;

constexpr String INTERACT_TEX[INTERACT_TYPE_MAX] =
{
	"data\\TEXTURE\\Base.jpg",
	"data\\TEXTURE\\Base.jpg"
};

//�C���^���N�g�\����
typedef struct
{
	D3DXVECTOR3 pos;      //�ʒu
	D3DXVECTOR3 scale;    //�傫��
	INTERACT_TYPE Type;   //�^�C�v
	D3DXMATRIX mtxWorld;  //�}�g���b�N�X
	bool bUse;            //�g�p
}Interact;

void InitInteract(void);                              //����������
void UninitInteract(void);                            //�I������
void UpdateInteract(void);                            //�X�V����
void DrawInteract(void);                              //�`�揈��
void SetInteract(D3DXVECTOR3 pos, D3DXVECTOR3 scale); //�ݒ�
void EndInteract(void);                               //�j��
void SetInteractType(INTERACT_TYPE Type);             //�^�C�v�ύX

#endif _INTERACT_H_