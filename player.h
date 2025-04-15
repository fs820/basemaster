//---------------------------------------
//
//���f���\�������̒�`�E�錾[player.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"main.h"
#include"model.h"
#include "object.h"

#define PLAYER_MODEL "data\\MODEL\\Base.x"
#define SPEED (0.2f)
#define SPEED_DEF (0.0f)
#define JUMP (2.0f)
#define JUMP_INA (0.9f)
#define PLAYER_INA (0.1f)
#define ROT_SPEED (0.1f)
#define SCALE_SPEED (0.2f)
#define SCALE_MIN (0.05f)
#define SCALE_MAX (1.0f)
#define START_Z (0.0f)
#define START_X (0.0f)
#define PLAYER_LIFE (1000)
#define BLEND_FRAME (10.0f)
#define PLAYER_SIZE (0.5f)

//�v���C���[��Ԃ̗񋓌^��`
typedef enum
{
	PLAYERSTATE_APPEAR = 0,//�������
	PLAYERSTATE_NORMAL,//�ʏ�
	PLAYERSTATE_DAMAGE,//�_���[�W
	PLAYERSTATE_DIE,//��
	PLAYERSTATE_MAX//��
}PLAYERSTATE;

//���[�V�����̎��
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,
	MOTIONTYPE_MOVE,
	MOTIONTYPE_ACTION,
	MOTIONTYPE_JUMP,
	MOTIONTYPE_LANDING,
	MOTIONTYPE_MAX
}MOTIONTYPE;

//�v���C���[�\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 Destrot;
	D3DXVECTOR3 scale;
	bool bJump;
	int nLife;
	PLAYERSTATE state;
	D3DXMATRIX mtxWorld;
	int nIdxShadow;
	int nIdxLife;
	Object* pObject;
	bool bUse;
}Player;

void InitPlayer(void);//�|���S���̏���������
void UninitPlayer(void);//�|���S���̏I������
void UpdatePlayer(void);//�|���S���̍X�V����
void DrawPlayer(void);//�|���S���̕`�揈��
Player* GetPlayer(void);
#endif _PLAYER_H_