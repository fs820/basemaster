#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"
#include "model.h"

#define MOTION_KEY_MAX (16)			// �L�[�̍ő吔
#define MOTION_KEYINFO_MAX (32)		// �L�[�C���t�H�̍ő吔
#define MOTION_MOTION_INFO_MAX (15)	// ���[�V�����C���t�H�ő吔

// �L�[�̍\����
typedef struct
{
	float fPosX;	// �ʒuX
	float fPosY;	// �ʒuY
	float fPosZ;	// �ʒuZ
	float fRotX;	// ����X
	float fRotY;	// ����Y
	float fRotZ;	// ����Z
}KEY;

// �L�[���̍\����
typedef struct
{
	int nFrame;					// �Đ��t���[��
	KEY aKey[MOTION_KEY_MAX];	// �e�p�[�c�̃L�[�v�f
}KEY_INFO;

// �L�[���[�V�������\����
typedef struct
{
	bool bLoop;				                // ���[�v���邩�ǂ���
	int nNumKey;			                // �L�[�̑���
	KEY_INFO aKeyinfo[MOTION_KEYINFO_MAX];	// �L�[���
}MOTION_INFO;

typedef struct
{
	MOTION_INFO aMotionInfo[MOTION_MOTION_INFO_MAX];	// ���[�V�������
	int nNumMotion;					                    // ���[�V�����̑���
}MOTION_SOURCE;

typedef struct
{
	MOTION_INFO MotionInfo;         // ���̃��[�V�������

	int nNowMotiType;				// ���݂̃��[�V�����^�C�v
	int nNumKey;					// �L�[�̑���
	bool bLoop;                     // ���[�v���邩�ǂ���
	int nKey;						// ���݂̃L�[No.
	int nNextKey;	                // ���̃L�[No.
	int nCounterKey;				// �L�[�J�E���^�[
	int nCounterMotion;				// ���[�V�����J�E���^�[
	bool MotionEnd;                 // ���[�V�������I����Ă��邩


	MOTION_INFO MotionInfoBlend;    // �u�����h��̃��[�V�������

	int nNowMotiTypeBlend;		    // �u�����h��̃��[�V�����^�C�v
	int nNumKeyBlend;				// �u�����h��̃L�[�̑���
	bool bLoopBlend;                // �u�����h�悪���[�v���邩�ǂ���
	int nKeyBlend;					// �u�����h��̌��݂̃L�[No.
	int nNextKeyBlend;	            // �u�����h��̎��̃L�[No.
	int nCounterKeyBlend;		    // �u�����h�L�[�J�E���^�[
	int nCounterMotionBlend;		// �u�����h���[�V�����J�E���^�[
	bool MotionEndBlend;            // �u�����h��̃��[�V�������I����Ă��邩

	bool bBlend;                    // �u�����h���邩
	int nBlendFrame;                // �u�����h�t���[��
	int nBlendCounter;              // �u�����h�J�E���^�[
}MOTION;

// �L�����N�^�[�p�����[�^�[
typedef struct
{
	float fWidth;	  // ��
	float fHeigt;	  // ����
	float fJamp;	  // �W�����v��
	float fMovement;  // �ړ���
	float fgraviment; // �d��
	int nNumModel;	  // ���f����
}CHARPARAM_SOURCE;

// �L�����N�^�[�p�����[�^�[
typedef struct
{
	int nLife;		   // �̗�
	int nCondition;	   // ���
	int nCntCondition; // ��ԊǗ��J�E���^�[
	bool bJanp;		   // �W�����v���Ă��邩�ǂ���
	bool bDisp;		   // �\�����܂���
}CHARPARAM;

typedef struct
{
	int nCntMotionInfo;		//	���[�V���������J�E���g�������
	int nCntKeyInfo;		//	�L�[���[�V�����̏����J�E���g�������
	int nCntKey;			//	�L�[���J�E���g�������
}SET_MOTION_MANAGER;

// �v���g�^�C�v�錾
void InitMotion(void);	// ���[�V����������
void ReadMotionText(const char* pFileName,Model *pModel, LPDIRECT3DTEXTURE9* pTex, MOTION_SOURCE* motion, CHARPARAM_SOURCE*charParam);	// ���[�V�����e�L�X�g�ǂݍ���
void ReadScript(FILE* pFile, Model* pModel, MOTION_SOURCE* motion, LPDIRECT3DTEXTURE9* pTex, CHARPARAM_SOURCE* charParam);				// �X�N���v�g�̓ǂݍ���
void ReadCharacterSet(FILE* pFile, Model* pModel, MOTION_SOURCE* motion, CHARPARAM_SOURCE* charParam);
bool UpdateMotion(MOTION* motion, ModelMatrix* modelmtx, Model* model, int nNumModel, float Angle);
void SetMotion(MOTION* motion, MOTION_SOURCE* motionsource, int nType, bool bBlend, int BlendFrame);	// ���[�V�����̐ؑ�

void MostionSet(FILE* pFile, MOTION_SOURCE* motion);
void MostionSet(MOTION_SOURCE* motion, MOTION_INFO setMotInfo);
void KeySet(FILE* pFile, MOTION_SOURCE* motion);
void EndKeySet(MOTION_SOURCE* motion, KEY_INFO keyInfo);
void Key(FILE* pFile, MOTION_SOURCE* motion);
void EndKey(MOTION_SOURCE* motion, KEY key);

void StringClear(char* astr, int nMaxCunt);
void ReadComment(FILE *pFile, char* aResult);
#endif // !_MOTION_H_