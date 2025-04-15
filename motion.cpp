#include "motion.h"

// �O���[�o���ϐ��錾
SET_MOTION_MANAGER g_SetMotiMane;	// ���[�V�����J�E���g���

//************************************************
// ���[�V�����̏�����
//************************************************
void InitMotion(void)
{
	g_SetMotiMane.nCntMotionInfo = 0;
	g_SetMotiMane.nCntKeyInfo = 0;
	g_SetMotiMane.nCntKey = 0;
}

//************************************************
// ���[�V�����e�L�X�g�̓ǂݍ���	SCRIPT
//************************************************
void ReadMotionText(const char* pFileName, Model* pModel, LPDIRECT3DTEXTURE9* pTex, MOTION_SOURCE* motion, CHARPARAM_SOURCE* charParam)
{
	char astrA[256] = {};		// ������i�[�p�ϐ�
	char astrB[256] = {};		// �Â��i�[�p�ϐ�
	char* aResult = 0;
	int nResult = 0;
	int nCnt = 0;
	// �t�@�C�����J��
	FILE* pFile = fopen(pFileName, "r");

	if (pFile == NULL)
	{
		return;
	}

	while (1)
	{	
		// �ꕶ���ǂݎ��
		aResult = fgets(&astrA[0],2, pFile);
		
		// �ꕶ�������
		astrB[nCnt] = astrA[0];

		// �J�E���g��i�߂�,�����������[��
		nCnt = (nCnt + 1) % 256;
		
		if (strcmp(&astrA[0], "#") == 0)
		{// �R�����g������ǂ񂾂炻�̂��Ƃ̕����͓ǂݔ�΂�
			ReadComment(pFile, aResult);	// �R�����g��ǂݔ�΂�
			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrA[0], "\n") == 0)
		{// ���s��񂾂�
			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrA[0], "\t") == 0)
		{// ���s��񂾂�

			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "SCRIPT") == 0)
		{// SCRIPT�ǂݎ��
			ReadScript(pFile, pModel, motion, pTex, charParam);	// �X�N���v�g�̓ǂݍ��݂Ɉȍ~
		}

		if (aResult == NULL)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
		else if (nResult == EOF)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);
}

//************************************************
// �X�N���v�g�̓ǂݎ��
//************************************************
void ReadScript(FILE* pFile, Model* pModel, MOTION_SOURCE* motion, LPDIRECT3DTEXTURE9* pTex, CHARPARAM_SOURCE* charParam)
{
	char astrA[256] = {};		// ������i�[�p�ϐ�
	char astrB[256] = {};	// �Â��i�[�p�ϐ�
	char *aResult = 0;
	int nResult = 0;
	int nCnt = 0;
	int nCntModel = 0;

	while (1)
	{
		// �ꕶ���ǂݎ��
		aResult = fgets(&astrA[0], 2, pFile);

		// �ꕶ�������
		astrB[nCnt] = astrA[0];

		// �J�E���g��i�߂�,�����������[��
		nCnt = (nCnt + 1) % 256;

		if (strcmp(&astrA[0], "#") == 0)
		{// �R�����g������ǂ񂾂炻�̂��Ƃ̕����͓ǂݔ�΂�
			ReadComment(pFile, aResult);	// �R�����g��ǂݔ�΂�
			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrA[0], "\n") == 0)
		{// ���s��񂾂�
			
			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrA[0], "\t") == 0)
		{// ���s��񂾂�

			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "NUM_MODEL") == 0)
		{// ���f�����̓ǂݍ���
			fgets(&astrA[0],3,pFile);								// =�ǂݍ���
			nResult = fscanf(pFile, "%d", &charParam->nNumModel);	// ���f�����ɑ��
			StringClear(&astrB[0], 256);							// �ۑ��p������̃N���A
			nCnt = 0;												// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "MODEL_FILENAME") == 0)
		{// MODEL_FILENAME�ǂݎ��
			fgets(&astrA[0], 3, pFile);					// =�ǂݍ���
			nResult = fscanf(pFile, "%s", &astrA[0]);	// �t�@�C���p�X�ǂݍ���
			SetModel(&astrA[0], pModel, pTex, nCntModel);		// X�t�B���̃p�X�̓ǂݍ���
			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;									// �ۑ��p������̃J�E���g�[��
			nCntModel++;								// ���f���J�E���g��i�߂�
		}
		else if (strcmp(&astrB[0], "CHARACTERSET") == 0)
		{// CHARACTERSET�ǂݎ��
			ReadCharacterSet(pFile, pModel, motion, charParam);
			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;									// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "MOTIONSET") == 0)
		{// MOTIONSET�ǂݎ��
			MostionSet(pFile, motion);		// ���[�V�����Z�b�g�ֈȍ~
			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;									// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "END_SCRIPT") == 0)
		{// END_SCRIPT�ǂݎ��
			break;
		}

		if (aResult == NULL)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
		else if (nResult == EOF)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
	}

	// �J�E���g���Ă���ϐ���������
	g_SetMotiMane.nCntMotionInfo = 0;
	g_SetMotiMane.nCntKeyInfo = 0;
	g_SetMotiMane.nCntKey = 0;
}

//************************************************
// �L�����N�^�[�Z�b�g�̓ǂݍ���
//************************************************
void ReadCharacterSet(FILE* pFile, Model* pModel, MOTION_SOURCE* motion, CHARPARAM_SOURCE* charParam)
{
	char astrA[256] = {};	// ������i�[�p�ϐ�
	char astrB[256] = {};	// �Â��i�[�p�ϐ�
	char* aResult = 0;
	int nResult = 0;
	int nCnt = 0;

	while (1)
	{
		// �ꕶ���ǂݎ��
		aResult = fgets(&astrA[0], 2, pFile);

		// �ꕶ�������
		astrB[nCnt] = astrA[0];

		// �J�E���g��i�߂�,�����������[��
		nCnt = (nCnt + 1) % 256;

		if (strcmp(&astrA[0], "#") == 0)
		{// �R�����g������ǂ񂾂炻�̂��Ƃ̕����͓ǂݔ�΂�
			ReadComment(pFile, aResult);	// �R�����g��ǂݔ�΂�
			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrA[0], "\n") == 0)
		{// ���s��񂾂�

			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrA[0], "\t") == 0)
		{// ���s��񂾂�

			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "NUM_PARTS") == 0)
		{// �p�[�c���̓ǂݍ���
			fgets(&astrA[0], 3, pFile);					// =�ǂݍ���
			nResult = fscanf(pFile, "%d", &charParam->nNumModel);	// ���f�����ɑ��
			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;									// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "RADIUS") == 0)
		{// ���f�����̓ǂݍ���
			fgets(&astrA[0], 3, pFile);							// =�ǂݍ���
			nResult = fscanf(pFile, "%f", &charParam->fWidth);	// ���f�����ɑ��
			StringClear(&astrB[0], 256);						// �ۑ��p������̃N���A
			nCnt = 0;											// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "HEIGHT") == 0)
		{// ���f�����̓ǂݍ���
			fgets(&astrA[0], 3, pFile);							// =�ǂݍ���
			nResult = fscanf(pFile, "%f", &charParam->fHeigt);	// ���f�����ɑ��
			StringClear(&astrB[0], 256);						// �ۑ��p������̃N���A
			nCnt = 0;											// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "PARTSSET") == 0)
		{// �p�[�c��ݒ�
			PartsSet(pFile,pModel);
			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "END_CHARACTERSET") == 0)
		{// �p�[�c��ݒ�
			break;
		}

		if (aResult == NULL)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
		else if (nResult == EOF)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
	}
}

//************************************************
// ���[�V�����Z�b�g
//************************************************
void MostionSet(FILE* pFile, MOTION_SOURCE* motion)
{
	char astrA[256] = {};	// ������i�[�p�ϐ�
	char astrB[256] = {};	// �Â��i�[�p�ϐ�
	char* aResult = 0;
	int nResult = 0;
	int nCnt = 0;

	MOTION_INFO setMotInfo;
	setMotInfo.bLoop = false;
	setMotInfo.nNumKey = 0;

	while (1)
	{
		// �ꕶ���ǂݎ��
		aResult = fgets(&astrA[0], 2, pFile);

		// �ꕶ�������
		astrB[nCnt] = astrA[0];

		// �J�E���g��i�߂�,�����������[��
		nCnt = (nCnt + 1) % 256;

		if (strcmp(&astrA[0], "#") == 0)
		{// �R�����g������ǂ񂾂炻�̂��Ƃ̕����͓ǂݔ�΂�
			ReadComment(pFile, aResult);	// �R�����g��ǂݔ�΂�
			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrA[0], "\n") == 0)
		{// ���s��񂾂�

			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrA[0], "\t") == 0)
		{// ���s��񂾂�

			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "LOOP") == 0)
		{// ���[�v���̓ǂݍ���
			int nFlag = 0;
			fgets(&astrA[0], 3, pFile);					// =�ǂݍ���
			nResult = fscanf(pFile, "%d", &nFlag);		// ���f�����ɑ��

			if (nFlag > 0)
			{// �[�����ゾ������g�D���[
				setMotInfo.bLoop = true;
			}

			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;									// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "NUM_KEY") == 0)
		{// �L�[�����̓ǂݍ���
			fgets(&astrA[0], 3, pFile);					// =�ǂݍ���
			nResult = fscanf(pFile, "%d", &setMotInfo.nNumKey);		// ���f�����ɑ��
			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;
		}
		else if (strcmp(&astrB[0], "KEYSET") == 0)
		{// �L�[�Z�b�g�̓ǂݍ���
			KeySet(pFile, motion);	// �L�[�Z�b�g�J�n
			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;
		}
		else if (strcmp(&astrB[0], "END_MOTIONSET") == 0)
		{// ���[�V�����Z�b�g�I���
			MostionSet(motion,setMotInfo);	// �擾��������S���Ԃ�����
			g_SetMotiMane.nCntMotionInfo++;	// �L�[�Z�b�g�ԍ��ǉ�

			g_SetMotiMane.nCntKeyInfo = 0;	// �L�[���J�E���g���Z�b�g
			break;
		}

		if (aResult == NULL)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
		else if (nResult == EOF)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
	}
}

//************************************************
// ���[�V�����I���[
//************************************************
void MostionSet(MOTION_SOURCE* motion, MOTION_INFO setMotInfo)
{
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].bLoop = setMotInfo.bLoop;
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].nNumKey = setMotInfo.nNumKey;
}

//************************************************
// �L�[�Z�b�g�̐ݒ�
//************************************************
void KeySet(FILE* pFile, MOTION_SOURCE* motion)
{
	char astrA[256] = {};	// ������i�[�p�ϐ�
	char astrB[256] = {};	// �Â��i�[�p�ϐ�
	char* aResult = 0;
	int nResult = 0;
	int nCnt = 0;

	KEY_INFO keyInfo;
	keyInfo.nFrame = 1;

	while (1)
	{
		// �ꕶ���ǂݎ��
		aResult = fgets(&astrA[0], 2, pFile);

		// �ꕶ�������
		astrB[nCnt] = astrA[0];

		// �J�E���g��i�߂�,�����������[��
		nCnt = (nCnt + 1) % 256;

		if (strcmp(&astrA[0], "#") == 0)
		{// �R�����g������ǂ񂾂炻�̂��Ƃ̕����͓ǂݔ�΂�
			ReadComment(pFile, aResult);	// �R�����g��ǂݔ�΂�
			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrA[0], "\n") == 0)
		{// ���s��񂾂�

			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrA[0], "\t") == 0)
		{// ���s��񂾂�

			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "FRAME") == 0)
		{// �t���[�����̓ǂݍ���
			fgets(&astrA[0], 3, pFile);					// =�ǂݍ���
			nResult = fscanf(pFile, "%d", &keyInfo.nFrame);	// ���f�����ɑ��
			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;									// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "KEY") == 0)
		{// �L�[�̓ǂݍ���
			Key(pFile, motion);
			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;									// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "END_KEYSET") == 0)
		{// �L�[�̓ǂݍ���
			EndKeySet(motion,keyInfo);		// �擾�����������ׂĂԂ�����
			g_SetMotiMane.nCntKeyInfo++;	// �L�[����i�߂�
			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;

			g_SetMotiMane.nCntKey = 0;		// �L�[�J�E���g�����Z�b�g
			break;
		}

		if (aResult == NULL)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
		else if (nResult == EOF)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
	}
}

//************************************************
// �L�[�Z�b�g�I���[
//************************************************
void EndKeySet(MOTION_SOURCE* motion, KEY_INFO keyInfo)
{
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].nFrame = keyInfo.nFrame;
}

//************************************************
// �L�[��ݒ�
//************************************************
void Key(FILE* pFile, MOTION_SOURCE* motion)
{
	char astrA[256] = {};	// ������i�[�p�ϐ�
	char astrB[256] = {};	// �Â��i�[�p�ϐ�
	char* aResult = 0;
	int nResult = 0;
	int nCnt = 0;

	KEY key;
	key.fPosX = 0.0f;
	key.fPosY = 0.0f;
	key.fPosZ = 0.0f;
	key.fRotX = 0.0f;
	key.fRotY = 0.0f;
	key.fRotZ = 0.0f;

	while (1)
	{
		// �ꕶ���ǂݎ��
		aResult = fgets(&astrA[0], 2, pFile);

		// �ꕶ�������
		astrB[nCnt] = astrA[0];

		// �J�E���g��i�߂�,�����������[��
		nCnt = (nCnt + 1) % 256;

		if (strcmp(&astrA[0], "#") == 0)
		{// �R�����g������ǂ񂾂炻�̂��Ƃ̕����͓ǂݔ�΂�
			ReadComment(pFile, aResult);	// �R�����g��ǂݔ�΂�
			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrA[0], "\n") == 0)
		{// ���s��񂾂�

			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrA[0], "\t") == 0)
		{// ���s��񂾂�

			StringClear(&astrB[0], 256);	// �ۑ��p������̃N���A
			nCnt = 0;						// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "POS") == 0)
		{// POS�̓ǂݍ���
			fgets(&astrA[0], 3, pFile);					// =�ǂݍ���
			nResult = fscanf(pFile, "%f", &key.fPosX);	// �ʒuX
			nResult = fscanf(pFile, "%f", &key.fPosY);	// �ʒuY
			nResult = fscanf(pFile, "%f", &key.fPosZ);	// �ʒuZ
			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;									// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "ROT") == 0)
		{// ROT�̓ǂݍ���
			fgets(&astrA[0], 3, pFile);					// =�ǂݍ���
			nResult = fscanf(pFile, "%f", &key.fRotX);	// ����X
			nResult = fscanf(pFile, "%f", &key.fRotY);	// ����Y
			nResult = fscanf(pFile, "%f", &key.fRotZ);	// ����Z
			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;									// �ۑ��p������̃J�E���g�[��
		}
		else if (strcmp(&astrB[0], "END_KEY") == 0)
		{// �L�[�ǂݍ��݂��[���
			EndKey(motion,key);			// �擾�����������ׂĂԂ�����
			g_SetMotiMane.nCntKey++;	// �L�[�Z�b�g�Ԗڒǉ�
			StringClear(&astrB[0], 256);				// �ۑ��p������̃N���A
			nCnt = 0;
			break;
		}

		if (aResult == NULL)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
		else if (nResult == EOF)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
	}
}

//************************************************
// �L�[�̏I���[
//************************************************
void EndKey(MOTION_SOURCE* motion, KEY key)
{
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].aKey[g_SetMotiMane.nCntKey].fPosX = key.fPosX;
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].aKey[g_SetMotiMane.nCntKey].fPosY = key.fPosY;
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].aKey[g_SetMotiMane.nCntKey].fPosZ = key.fPosZ;

	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].aKey[g_SetMotiMane.nCntKey].fRotX = key.fRotX;
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].aKey[g_SetMotiMane.nCntKey].fRotY = key.fRotY;
	motion->aMotionInfo[g_SetMotiMane.nCntMotionInfo].aKeyinfo[g_SetMotiMane.nCntKeyInfo].aKey[g_SetMotiMane.nCntKey].fRotZ = key.fRotZ;
}

//************************************************
// ������̃N���A
//************************************************
void StringClear(char* astr,int nMaxCunt)
{
	for (int nCntB = 0; nCntB < nMaxCunt; nCntB++)
	{// �ۑ����Ă��镶��������ׂăN���A
		if (astr[nCntB] == '\0')
		{
			break;
		}

		astr[nCntB] = '\0';
	}
}

//************************************************
// �R�����g
//************************************************
void ReadComment(FILE *pFile,char*aResult)
{
	char astr[2];

	while (1)
	{
		// �ꕶ�����ǂ�
		fgets(&astr[0], 2, pFile);

		if (*aResult = strcmp(&astr[0], "\n") == 0)
		{// ���s�����ǂ񂾂�
			break;
		}

		if (*aResult == EOF)
		{// �e�L�X�g�t�@�C���S���ǂݐ؂��������
			break;
		}
	}
}

//************************************************
// ���[�V�����̍X�V
//************************************************
bool UpdateMotion(MOTION* motion, ModelMatrix* modelmtx, Model* model, int nNumModel, float Angle)
{
	//���̃��[�V����
	KEY nextKey[16] = {};                // ���̃L�[
	KEY nowKey[16] = {};                 // ���̃L�[
	KEY diffKey[16] = {};	             // �L�[�̍���
	KEY Hope[16] = {};		             // ��]�̒l

	//�u�����h��̃��[�V����
	KEY nextKeyBlend[16] = {};            // ���̃L�[
	KEY nowKeyBlend[16] = {};             // ���̃L�[
	KEY diffKeyBlend[16] = {};            // �L�[�̍���
	KEY HopeBlend[16] = {};		          // ��]�̒l

	//����
	KEY diffKeyDest[16] = {};	          // �L�[�̍���
	KEY HopeDest[16] = {};		          // ��]�̒l

	if (motion->nNextKey < motion->nNumKey)
	{// ���[�V�����̑S�̃L�[���l���Ⴂ�ꍇ

		motion->nCounterMotion++;	// ���[�V�����J�E���g
		motion->nCounterKey++;		// �L�[�J�E���g

		// �S���f���p�[�c�̍X�V
		for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
		{
			nextKey[nCntModel] = motion->MotionInfo.aKeyinfo[motion->nNextKey].aKey[nCntModel]; // ���̃L�[
			nowKey[nCntModel] = motion->MotionInfo.aKeyinfo[motion->nKey].aKey[nCntModel];      // ���̃L�[

			// �L�[�̍���(�ʒu)�����߂�
			diffKey[nCntModel].fPosX = (nextKey[nCntModel].fPosX - nowKey[nCntModel].fPosX);
			diffKey[nCntModel].fPosY = (nextKey[nCntModel].fPosY - nowKey[nCntModel].fPosY);
			diffKey[nCntModel].fPosZ = (nextKey[nCntModel].fPosZ - nowKey[nCntModel].fPosZ);

			// �L�[�̍���(����)�����߂�
			diffKey[nCntModel].fRotX = (nextKey[nCntModel].fRotX - nowKey[nCntModel].fRotX);
			diffKey[nCntModel].fRotY = (nextKey[nCntModel].fRotY - nowKey[nCntModel].fRotY);
			diffKey[nCntModel].fRotZ = (nextKey[nCntModel].fRotZ - nowKey[nCntModel].fRotZ);

			// �I�t�Z�b�g�ɑ����l�����߂�
			Hope[nCntModel].fPosX = nowKey[nCntModel].fPosX + diffKey[nCntModel].fPosX * ((float)motion->nCounterKey / (float)motion->MotionInfo.aKeyinfo[motion->nKey].nFrame);
			Hope[nCntModel].fPosY = nowKey[nCntModel].fPosY + diffKey[nCntModel].fPosY * ((float)motion->nCounterKey / (float)motion->MotionInfo.aKeyinfo[motion->nKey].nFrame);
			Hope[nCntModel].fPosZ = nowKey[nCntModel].fPosZ + diffKey[nCntModel].fPosZ * ((float)motion->nCounterKey / (float)motion->MotionInfo.aKeyinfo[motion->nKey].nFrame);

			// ���������߂�
			Hope[nCntModel].fRotX = nowKey[nCntModel].fRotX + diffKey[nCntModel].fRotX * ((float)motion->nCounterKey / (float)motion->MotionInfo.aKeyinfo[motion->nKey].nFrame);
			Hope[nCntModel].fRotY = nowKey[nCntModel].fRotY + diffKey[nCntModel].fRotY * ((float)motion->nCounterKey / (float)motion->MotionInfo.aKeyinfo[motion->nKey].nFrame);
			Hope[nCntModel].fRotZ = nowKey[nCntModel].fRotZ + diffKey[nCntModel].fRotZ * ((float)motion->nCounterKey / (float)motion->MotionInfo.aKeyinfo[motion->nKey].nFrame);
		}

		if (motion->nCounterKey >= motion->MotionInfo.aKeyinfo[motion->nKey].nFrame)
		{// �t���[���S�������I�������
			motion->nCounterKey = 0;          	    // �L�[�J�E���g�[��
			motion->nNextKey++;	                    // ���̃L�[�փJ�E���g
			motion->nKey++;		                    // ���̃L�[�փJ�E���g
			motion->nNextKey %= motion->nNumKey;	// �L�[�̑������ɐ�������
			motion->nKey %= motion->nNumKey;		// �L�[�̑������ɐ�������

			if (motion->nNextKey == 0 && motion->bLoop == false)
			{// ���[�v���Ȃ����[�V�����͏I��点��
				motion->nNextKey = motion->nNumKey;	// �v�Z�̏������ɓ���Ȃ��悤�ɂ���
				motion->nCounterMotion = 0;			// ���[�V�����J�E���g�[��
				motion->MotionEnd = true;		    // ���[�V�����I���
			}
		}
	}

	if (motion->bBlend == true && motion->nNextKeyBlend < motion->nNumKeyBlend)
	{// �u�����h���邩���[�V�����̑S�̃L�[���l���Ⴂ�ꍇ
		motion->nCounterMotionBlend++;	// ���[�V�����J�E���g
		motion->nCounterKeyBlend++;		// �L�[�J�E���g

		// �S���f���p�[�c�̍X�V
		for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
		{
			nextKeyBlend[nCntModel] = motion->MotionInfoBlend.aKeyinfo[motion->nNextKeyBlend].aKey[nCntModel]; // ���̃L�[
			nowKeyBlend[nCntModel] = motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].aKey[nCntModel];      // ���̃L�[

			// �L�[�̍���(�ʒu)�����߂�
			diffKeyBlend[nCntModel].fPosX = (nextKeyBlend[nCntModel].fPosX - nowKeyBlend[nCntModel].fPosX);
			diffKeyBlend[nCntModel].fPosY = (nextKeyBlend[nCntModel].fPosY - nowKeyBlend[nCntModel].fPosY);
			diffKeyBlend[nCntModel].fPosZ = (nextKeyBlend[nCntModel].fPosZ - nowKeyBlend[nCntModel].fPosZ);

			// �L�[�̍���(����)�����߂�
			diffKeyBlend[nCntModel].fRotX = (nextKeyBlend[nCntModel].fRotX - nowKeyBlend[nCntModel].fRotX);
			diffKeyBlend[nCntModel].fRotY = (nextKeyBlend[nCntModel].fRotY - nowKeyBlend[nCntModel].fRotY);
			diffKeyBlend[nCntModel].fRotZ = (nextKeyBlend[nCntModel].fRotZ - nowKeyBlend[nCntModel].fRotZ);

			// �I�t�Z�b�g�ɑ����l�����߂�
			HopeBlend[nCntModel].fPosX = nowKeyBlend[nCntModel].fPosX + diffKeyBlend[nCntModel].fPosX * ((float)motion->nCounterKeyBlend / (float)motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame);
			HopeBlend[nCntModel].fPosY = nowKeyBlend[nCntModel].fPosY + diffKeyBlend[nCntModel].fPosY * ((float)motion->nCounterKeyBlend / (float)motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame);
			HopeBlend[nCntModel].fPosZ = nowKeyBlend[nCntModel].fPosZ + diffKeyBlend[nCntModel].fPosZ * ((float)motion->nCounterKeyBlend / (float)motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame);

			// ���������߂�
			HopeBlend[nCntModel].fRotX = nowKeyBlend[nCntModel].fRotX + diffKeyBlend[nCntModel].fRotX * ((float)motion->nCounterKeyBlend / (float)motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame);
			HopeBlend[nCntModel].fRotY = nowKeyBlend[nCntModel].fRotY + diffKeyBlend[nCntModel].fRotY * ((float)motion->nCounterKeyBlend / (float)motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame);
			HopeBlend[nCntModel].fRotZ = nowKeyBlend[nCntModel].fRotZ + diffKeyBlend[nCntModel].fRotZ * ((float)motion->nCounterKeyBlend / (float)motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame);
		}

		if (motion->nCounterKeyBlend >= motion->MotionInfoBlend.aKeyinfo[motion->nKeyBlend].nFrame)
		{// �t���[���S�������I�������
			motion->nCounterKeyBlend = 0;          	        // �L�[�J�E���g�[��
			motion->nNextKeyBlend++;	                    // ���̃L�[�փJ�E���g
			motion->nKeyBlend++;		                    // ���̃L�[�փJ�E���g
			motion->nNextKeyBlend %= motion->nNumKeyBlend;	// �L�[�̑������ɐ�������
			motion->nKeyBlend %= motion->nNumKeyBlend;		// �L�[�̑������ɐ�������

			if (motion->nNextKeyBlend == 0 && motion->bLoopBlend == false)
			{// ���[�v���Ȃ����[�V�����͏I��点��
				motion->nNextKeyBlend = motion->nNumKeyBlend;	// �v�Z�̏������ɓ���Ȃ��悤�ɂ���
				motion->nCounterMotionBlend = 0;			    // ���[�V�����J�E���g�[��
				motion->MotionEndBlend = true;		            // ���[�V�����I���
			}
		}
	}

	// �S���f���p�[�c�̍X�V
	for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
	{
		if (motion->bBlend == true)
		{
			motion->nBlendCounter++; //�u�����h�J�E���^�[

			// �L�[�̍���(�ʒu)�����߂�
			diffKeyDest[nCntModel].fPosX = (HopeBlend[nCntModel].fPosX - Hope[nCntModel].fPosX);
			diffKeyDest[nCntModel].fPosY = (HopeBlend[nCntModel].fPosY - Hope[nCntModel].fPosY);
			diffKeyDest[nCntModel].fPosZ = (HopeBlend[nCntModel].fPosZ - Hope[nCntModel].fPosZ);

			// �L�[�̍���(����)�����߂�
			diffKeyDest[nCntModel].fRotX = (HopeBlend[nCntModel].fRotX - Hope[nCntModel].fRotX);
			diffKeyDest[nCntModel].fRotY = (HopeBlend[nCntModel].fRotY - Hope[nCntModel].fRotY);
			diffKeyDest[nCntModel].fRotZ = (HopeBlend[nCntModel].fRotZ - Hope[nCntModel].fRotZ);

			// �I�t�Z�b�g�ɑ����l�����߂�
			HopeDest[nCntModel].fPosX = Hope[nCntModel].fPosX + diffKeyDest[nCntModel].fPosX * ((float)motion->nBlendCounter / (float)motion->nBlendFrame);
			HopeDest[nCntModel].fPosY = Hope[nCntModel].fPosY + diffKeyDest[nCntModel].fPosY * ((float)motion->nBlendCounter / (float)motion->nBlendFrame);
			HopeDest[nCntModel].fPosZ = Hope[nCntModel].fPosZ + diffKeyDest[nCntModel].fPosZ * ((float)motion->nBlendCounter / (float)motion->nBlendFrame);

			// ���������߂�
			HopeDest[nCntModel].fRotX = Hope[nCntModel].fRotX + diffKeyDest[nCntModel].fRotX * ((float)motion->nBlendCounter / (float)motion->nBlendFrame);
			HopeDest[nCntModel].fRotY = Hope[nCntModel].fRotY + diffKeyDest[nCntModel].fRotY * ((float)motion->nBlendCounter / (float)motion->nBlendFrame);
			HopeDest[nCntModel].fRotZ = Hope[nCntModel].fRotZ + diffKeyDest[nCntModel].fRotZ * ((float)motion->nBlendCounter / (float)motion->nBlendFrame);

			// ���f���̈ʒu�A������ݒ肷��
			modelmtx[nCntModel].pos = model[nCntModel].offSet + D3DXVECTOR3(HopeDest[nCntModel].fPosX, HopeDest[nCntModel].fPosY, HopeDest[nCntModel].fPosZ);
			modelmtx[nCntModel].rot = D3DXVECTOR3(HopeDest[nCntModel].fRotX, HopeDest[nCntModel].fRotY, HopeDest[nCntModel].fRotZ);
		}
		else if (motion->MotionEnd == false)
		{
			// ���f���̈ʒu�A������ݒ肷��
			modelmtx[nCntModel].pos = model[nCntModel].offSet + D3DXVECTOR3(Hope[nCntModel].fPosX, Hope[nCntModel].fPosY, Hope[nCntModel].fPosZ);
			modelmtx[nCntModel].rot = D3DXVECTOR3(Hope[nCntModel].fRotX, Hope[nCntModel].fRotY, Hope[nCntModel].fRotZ);
		}

		if ((motion->bBlend == true || motion->MotionEnd == false) && model[nCntModel].nIdxModelParent == -1)
		{// �{�̂̃|�X�����т��Ă���������
			modelmtx[nCntModel].rot.y += Angle;
		}
	}

	if (motion->bBlend == true && motion->nBlendCounter >= motion->nBlendFrame)
	{// �u�����h�t���[���S�������I�������
		motion->nBlendCounter = 0;                      // �u�����h�J�E���^�[�[��

		// �u�����h������̃��[�V�����ɂ���
		motion->nNowMotiType = motion->nNowMotiTypeBlend;					  // �^�C�v
		motion->MotionInfo = motion->MotionInfoBlend;                         // ���[�V�������
		motion->nNumKey = motion->MotionInfo.nNumKey;	                      // �L�[�̍ő吔
		motion->bLoop = motion->MotionInfo.bLoop;	                          // ���[�v
		motion->nKey = motion->nKeyBlend;									  // �������Ă���L�[
		motion->nNextKey = motion->nNextKeyBlend;                             // ���̃L�[
		motion->nCounterMotion = motion->nCounterMotionBlend;				  // �t���[���J�E���^�[
		motion->nCounterKey = motion->nCounterKeyBlend;						  // �L�[�J�E���^�[
		motion->MotionEnd = motion->MotionEndBlend;                           // ���[�V����

		motion->bBlend = false;                         // �u�����h�I��
	}

	return motion->MotionEnd;
}

//************************************************
// ���[�V�����̐ؑ�
//************************************************
void SetMotion(MOTION* motion, MOTION_SOURCE* motionsource, int nType, bool bBlend, int BlendFrame)
{
	if (bBlend == true && motion->MotionInfo.nNumKey > 1)
	{// �u�����h����
		motion->nNowMotiTypeBlend = nType;							                    // �^�C�v��ς���
		motion->MotionInfoBlend = motionsource->aMotionInfo[motion->nNowMotiTypeBlend]; // ���[�V�����������炤
		motion->nNumKeyBlend = motion->MotionInfoBlend.nNumKey;	                        // �L�[�̍ő吔����
		motion->bLoopBlend = motion->MotionInfoBlend.bLoop;	                            // ���[�v�̑��
		motion->nKeyBlend = 0;										                    // �������Ă���L�[��������
		if (motion->nNumKeyBlend > 1)
		{// ��L�[�ȏ゠��Ȃ�
			motion->nNextKeyBlend = motion->nKeyBlend + 1;                              // ���̃L�[��ݒ�
		}
		else
		{// �Ȃ��Ȃ��
			motion->nNextKeyBlend = motion->nKeyBlend;								    // ���̃L�[��ݒ�
		}
		motion->nCounterMotionBlend = 0;								                // �t���[���J�E���^�[���[��
		motion->nCounterKeyBlend = 0;									                // �L�[�J�E���^�[���[��
		motion->MotionEndBlend = false;                                                 // ���[�V�����͏I����Ă��Ȃ�

		motion->nBlendFrame = BlendFrame;                                               // �u�����h�t���[��
		motion->nBlendCounter = 0;                                                      // �u�����h�J�E���^�[
		motion->bBlend = true;                                                          // �u�����h����
	}
	else
	{// �u�����h���Ȃ�
		motion->nNowMotiType = nType;							              // �^�C�v��ς���
		motion->MotionInfo = motionsource->aMotionInfo[motion->nNowMotiType]; // ���[�V�����������炤
		motion->nNumKey = motion->MotionInfo.nNumKey;	                      // �L�[�̍ő吔����
		motion->bLoop = motion->MotionInfo.bLoop;	                          // ���[�v�̑��
		motion->nKey = 0;										              // �������Ă���L�[��������
		if (motion->nNumKey > 1)
		{// ��L�[�ȏ゠��Ȃ�
			motion->nNextKey = motion->nKey + 1;                            // ���̃L�[��ݒ�
		}
		else
		{// �Ȃ��Ȃ��
			motion->nNextKey = motion->nKey;								// ���̃L�[��ݒ�
		}
		motion->nCounterMotion = 0;								            // �t���[���J�E���^�[���[��
		motion->nCounterKey = 0;									        // �L�[�J�E���^�[���[��
		motion->MotionEnd = false;                                          // ���[�V�����͏I����Ă��Ȃ�
		motion->bBlend = false;                                             // �u�����h���Ȃ�

		//�u�����h�p�̕ϐ������������Ă���
		motion->nNowMotiTypeBlend = -1;							                        // �^�C�v��ς���
		motion->MotionInfoBlend = motionsource->aMotionInfo[0];                        // ���[�V�����������炤
		motion->nNumKeyBlend = 0;	                                                    // �L�[�̍ő吔����
		motion->bLoopBlend = false;	                                                    // ���[�v�̑��
		motion->nKeyBlend = -1;										                    // �������Ă���L�[��������
		motion->nNextKeyBlend = -1;                                                     // ���̃L�[��ݒ�
		motion->nCounterMotionBlend = -1;								                // �t���[���J�E���^�[���[��
		motion->nCounterKeyBlend = -1;									                // �L�[�J�E���^�[���[��
		motion->MotionEndBlend = false;                                                 // ���[�V�����͏I����Ă��Ȃ�

		motion->nBlendFrame = 0;                                                        // �u�����h�t���[��
		motion->nBlendCounter = 0;                                                      // �u�����h�J�E���^�[
	}
}