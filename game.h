//------------------------------------------
//
//�Q�[�����C���̏����̒�`�E�錾[game.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _GAME_H_
#define _GAME_H_

#include"main.h"

#define INTER (60)//�I���P�\
#define GAME_WALL (8000.0f)//��l�̕�
#define GRAVITY (-0.12f)//�d��
#define TEXTURE_NUMBER "data\\TEXTURE\\Base.jpg"
#define TIME_LIMIT (120)
#define FRAME (60)
#define HOUR (24)
#define MINUTE (60)

//�Q�[����Ԃ̗񋓌^��`
typedef enum
{
	GAMESTATE_NONE=0,//�Q�[���łȂ�
	GAMESTATE_NORMAL,//�Q�[����
	GAMESTATE_PAUSE,//�|�[�Y��
	GAMESTATE_END,//�I��
	GAMESTATE_MAX//��
}GAMESTATE;

//�I�u�W�F�N�g�^�C�v�̗񋓌^��`
typedef enum
{
	TYPE_NORMAL = 0,
	TYPE_WHITE,
	TYPE_BLACK,
	TYPE_MAX
}WHITE_TYPE;

void InitGame(void);//�Q�[���̏���������
void UninitGame(void);//�Q�[���̏I������
void UpdateGame(void);//�Q�[���̍X�V����
void DrawGame(void);//�Q�[���̕`�揈��
void SetGameState(GAMESTATE state);//�Q�[���̏�Ԑݒ�
GAMESTATE GetGameState(void);//�Q�[����Ԏ擾
bool GetbSnow(void);
bool GetClear(void);
int GetClearTime(void);
int GetTime(void);
void TimeReset(void);

#endif _GAME_H_
