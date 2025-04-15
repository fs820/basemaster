//---------------------------------------
//
//�u���o���A����[isolationfield.cpp]
//Author fuma sato
//
//---------------------------------------
#include"isolationfield.h"
#include"camera.h"
#include"input.h"

//�O���[�o���ϐ�
IsolationField g_aIsolationField[ISOLATION_FIELD_MAX];
#ifdef _DEBUG
ISO_TYPE g_Type = ISO_TYPE_1;
int g_DelIso = -1;
#endif // _DEBUG

//---------------
//����������
//---------------
void InitIsolationField(void)
{
	//�\���̂̏�����
	for (int nCnt = 0; nCnt < ISOLATION_FIELD_MAX; nCnt++)
	{//�ő吔��
		g_aIsolationField[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ʒu
		g_aIsolationField[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //����
		g_aIsolationField[nCnt].Width = 0.0f;                          //��
		g_aIsolationField[nCnt].Height = 0.0f;                         //����
		g_aIsolationField[nCnt].Depth = 0.0f;                          //���s
		g_aIsolationField[nCnt].Corner = 0.0f;                         //�p
		g_aIsolationField[nCnt].Type = ISO_TYPE_1;                     //�^�C�v
		g_aIsolationField[nCnt].Switch = OFF;                          //Switch
		g_aIsolationField[nCnt].bUse = false;                          //�g�p
	}

#ifdef _DEBUG
	g_Type = ISO_TYPE_1;
	g_DelIso = -1;
#endif // _DEBUG
}

//---------------
//�I������
//---------------
void UninitIsolationField(void)
{
	//�\���̂̏�����
	for (int nCnt = 0; nCnt < ISOLATION_FIELD_MAX; nCnt++)
	{//�ő吔��
		g_aIsolationField[nCnt].bUse = false;                          //�g�p
	}
}

//---------------
//�X�V����
//---------------
void UpdateIsolationField(void)
{

}

//---------------
//�`�揈��
//---------------
void DrawIsolationField(void)
{

}

//---------------
//�ݒ�
//---------------
int SetIsolationField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Width, float Height, float Depth, float Corner, ISO_TYPE Type)
{
	for (int nCnt = 0; nCnt < ISOLATION_FIELD_MAX; nCnt++)
	{//�ő吔��
		if (g_aIsolationField[nCnt].bUse == false)
		{//�g�p���Ă��Ȃ�
			g_aIsolationField[nCnt].pos = pos;   //�ʒu
			g_aIsolationField[nCnt].rot = rot;   //����
			g_aIsolationField[nCnt].Width = Width;                           //��
			g_aIsolationField[nCnt].Height = Height;                         //����
			g_aIsolationField[nCnt].Depth = Depth;                           //���s
			g_aIsolationField[nCnt].Corner = Corner;                         //�p
			g_aIsolationField[nCnt].Type = Type;                             //�^�C�v
			//�o���A�̐���
			SetBarrier(g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * 0.5f), g_aIsolationField[nCnt].rot, g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner), g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);
			SetBarrier(g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * 0.5f), g_aIsolationField[nCnt].rot + D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner), g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);
			SetBarrier(g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * 0.5f), g_aIsolationField[nCnt].rot + D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner), g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);
			SetBarrier(g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * 0.5f), g_aIsolationField[nCnt].rot + D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner), g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);

			//�p�̃o���A�̐���
			D3DXVECTOR3 aPos[2] = { D3DXVECTOR3(0.0f,0.0f,0.0f) }, SenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f),Space = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			float fAngle = 0.0f, fLong = 0.0f;

			aPos[0] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			aPos[1] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			Space = aPos[1] - aPos[0];
			SenterPos = aPos[0] + Space * 0.5f;
			fAngle = atan2f(Space.x, Space.z) + D3DX_PI * -0.5f;
			fLong = sqrtf(Space.x * Space.x + Space.z * Space.z);
			SetBarrier(SenterPos, D3DXVECTOR3(0.0f, fAngle, 0.0f), fLong, g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);

			aPos[0] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			aPos[1] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * 0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			Space = aPos[1] - aPos[0];
			SenterPos = aPos[0] + Space * 0.5f;
			fAngle = atan2f(Space.x, Space.z) + D3DX_PI * -0.5f;
			fLong = sqrtf(Space.x * Space.x + Space.z * Space.z);
			SetBarrier(SenterPos, D3DXVECTOR3(0.0f, fAngle, 0.0f), fLong, g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);

			aPos[0] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			aPos[1] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			Space = aPos[1] - aPos[0];
			SenterPos = aPos[0] + Space * 0.5f;
			fAngle = atan2f(Space.x, Space.z) + D3DX_PI * -0.5f;
			fLong = sqrtf(Space.x * Space.x + Space.z * Space.z);
			SetBarrier(SenterPos, D3DXVECTOR3(0.0f, fAngle, 0.0f), fLong, g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);

			aPos[0] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			aPos[1] = g_aIsolationField[nCnt].pos + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y) * g_aIsolationField[nCnt].Depth * 0.5f) + D3DXVECTOR3(sinf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f, 0.0f, cosf(g_aIsolationField[nCnt].rot.y + D3DX_PI * -0.5f) * g_aIsolationField[nCnt].Width * (1.0f - g_aIsolationField[nCnt].Corner) * 0.5f);
			Space = aPos[1] - aPos[0];
			SenterPos = aPos[0] + Space * 0.5f;
			fAngle = atan2f(Space.x, Space.z) + D3DX_PI * -0.5f;
			fLong = sqrtf(Space.x * Space.x + Space.z * Space.z);
			SetBarrier(SenterPos, D3DXVECTOR3(0.0f, fAngle, 0.0f), fLong, g_aIsolationField[nCnt].Height, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), false, NULL);

			SetBarrier(g_aIsolationField[nCnt].pos + D3DXVECTOR3(0.0f, g_aIsolationField[nCnt].Height, 0.0f), g_aIsolationField[nCnt].rot, g_aIsolationField[nCnt].Width, g_aIsolationField[nCnt].Depth, (BTYPE)(BTYPE_ISOLATION_1 + g_aIsolationField[nCnt].Type), true, g_aIsolationField[nCnt].Corner);

			g_aIsolationField[nCnt].Switch = ON;                            //Switch
			g_aIsolationField[nCnt].bUse = true;                             //�g�p
			return nCnt;
		}
	}
	return -1;
}

//---------------
//�擾
//---------------
IsolationField* GetIsolationField(void)
{
	return &g_aIsolationField[0];
}

//---------------
//�j��
//---------------
void IsolationFieldSwitch(ISO_TYPE Type, SWITCH Switch)
{
	//�o���A�̃I���I�t
	BarrierSwitch((BTYPE)(BTYPE_ISOLATION_1 + Type), Switch);
	for (int nCnt = 0; nCnt < ISOLATION_FIELD_MAX; nCnt++)
	{
		if (g_aIsolationField[nCnt].bUse == true && g_aIsolationField[nCnt].Type == Type)
		{
			g_aIsolationField[nCnt].Switch = Switch;
		}
	}
}

//--------------------------
//�I�u�W�F�N�g�G�f�B�^�[
//--------------------------
void UpdateIsolationFieldEdit(D3DXVECTOR3 Epos, D3DXVECTOR3 Erot)
{
#ifdef _DEBUG
	Camera* pCamera = GetCamera();
	static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static int HoldNumber = -1, EndNumber = -1;
	static float Width = 100.0f, Height = 100.0f, Depth = 100.0f, Corner = 0.0f;
	MODE Mode = GetMode();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_9))
	{//�\���̐ؑ�
		static bool bON = false;
		if (bON)
		{
			IsolationFieldSwitch(ISO_TYPE_1, OFF);
			IsolationFieldSwitch(ISO_TYPE_2, OFF);
			IsolationFieldSwitch(ISO_TYPE_3, OFF);
			bON = false;
		}
		else
		{
			IsolationFieldSwitch(ISO_TYPE_1, ON);
			IsolationFieldSwitch(ISO_TYPE_2, ON);
			IsolationFieldSwitch(ISO_TYPE_3, ON);
			bON = true;
		}
	}
#endif // _DEBUG

	if (GetKeyboardTrigger(DIK_RIGHT))
	{// �I�u�W�F�N�g�ؑ�
		g_Type = (ISO_TYPE)(g_Type + 1);
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{// �I�u�W�F�N�g�ؑ�
		g_Type = (ISO_TYPE)(g_Type - 1);
	}

	//�͈͐���
	g_Type = (ISO_TYPE)(g_Type % (ISO_TYPE_MAX));

	if (g_Type < 0)
	{//���[�v
		g_Type = (ISO_TYPE)(ISO_TYPE_MAX - 1);
	}

	if (GetKeyboardTrigger(DIK_UP))
	{// DEL�I�u�W�F�N�g�ؑ�
		g_DelIso++;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{// DEL�I�u�W�F�N�g�ؑ�
		g_DelIso--;
	}

	for (int nCount = 0; nCount < ISOLATION_FIELD_MAX; nCount++)
	{//�ő吔��

		//�͈͐���
		if (g_DelIso < 0)
		{
			g_DelIso = ISOLATION_FIELD_MAX - 1;
		}
		g_DelIso %= ISOLATION_FIELD_MAX;

		if (g_aIsolationField[g_DelIso].bUse)
		{//�g���Ă���I�u�W�F�N�g�̑I��
			break;
		}
		else
		{//�g���Ă��Ȃ��ԍ��̓X�L�b�v

			if (GetKeyboardTrigger(DIK_DOWN))
			{// DEL�I�u�W�F�N�g�ؑ�
				g_DelIso--;
			}
			else
			{// DEL�I�u�W�F�N�g�ؑ�
				g_DelIso++;
			}
		}

		if (nCount == ISOLATION_FIELD_MAX - 1)
		{//�I�u�W�F�N�g���Ȃ�
			g_DelIso = -1;
		}
	}

	if (GetKeyboardPress(DIK_F) == true || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) == true)//A�L�[
	{
		if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
		{//����
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.25f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.25f) * 10.0f;
		}
		else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
		{//����
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.75f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.75f) * 10.0f;
		}
		else
		{//��
			pos.x += sinf(pCamera->rot.y + D3DX_PI * -0.5f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * -0.5f) * 10.0f;
		}
	}
	else if (GetKeyboardPress(DIK_H) == true || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) == true)//D�L�[
	{
		if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
		{//�E��
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * 10.0f;
		}
		else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
		{//�E��
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * 10.0f;
		}
		else
		{//�E
			pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * 10.0f;
			pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * 10.0f;
		}
	}
	else if (GetKeyboardPress(DIK_T) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
	{//��
		pos.x += sinf(pCamera->rot.y) * 10.0f;
		pos.z += cosf(pCamera->rot.y) * 10.0f;
	}
	else if (GetKeyboardPress(DIK_G) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
	{//��
		pos.x += sinf(pCamera->rot.y + D3DX_PI) * 10.0f;
		pos.z += cosf(pCamera->rot.y + D3DX_PI) * 10.0f;
	}

	if (GetKeyboardPress(DIK_V) == true)//W�L�[
	{//��
		pos.y -= 10.0f;
	}
	if (GetKeyboardPress(DIK_B) == true)//S�L�[
	{//��
		pos.y += 10.0f;
	}

	if (GetKeyboardPress(DIK_U) || GetJoykeyPress(JOYKEY_A, CONTROLLER_1))
	{// �����Ă����
		rot.x -= 0.1f;
	}
	if (GetKeyboardPress(DIK_I) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// �����Ă����
		rot.x += 0.1f;
	}
	if (GetKeyboardPress(DIK_J) || GetJoykeyPress(JOYKEY_LT, CONTROLLER_1))
	{// �����Ă����
		rot.y -= 0.1f;
	}
	if (GetKeyboardPress(DIK_K) || GetJoykeyPress(JOYKEY_RT, CONTROLLER_1))
	{// �����Ă����
		rot.y += 0.1f;
	}
	if (GetKeyboardPress(DIK_M) || GetJoykeyPress(JOYKEY_X, CONTROLLER_1))
	{// �����Ă����
		rot.z -= 0.1f;
	}
	if (GetKeyboardPress(DIK_COMMA) || GetJoykeyPress(JOYKEY_Y, CONTROLLER_1))
	{// �����Ă����
		rot.z += 0.1f;
	}
	if (GetKeyboardPress(DIK_1) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// �����Ă����
		Corner -= 0.01f;
	}
	if (GetKeyboardPress(DIK_2) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// �����Ă����
		Corner += 0.01f;
	}
	if (GetKeyboardPress(DIK_3) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// �����Ă����
		Width -= 10.0f;
	}
	if (GetKeyboardPress(DIK_4) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// �����Ă����
		Width += 10.0f;
	}
	if (GetKeyboardPress(DIK_5) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// �����Ă����
		Height -= 10.0f;
	}
	if (GetKeyboardPress(DIK_6) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// �����Ă����
		Height += 10.0f;
	}
	if (GetKeyboardPress(DIK_7) || GetJoykeyPress(JOYKEY_B, CONTROLLER_1))
	{// �����Ă����
		Depth -= 10.0f;
	}
	if (GetKeyboardPress(DIK_8) || GetJoykeyPress(JOYKEY_START, CONTROLLER_1))
	{// �����Ă����
		Depth += 10.0f;
	}

	if (GetKeyboardTrigger(DIK_0) || GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1))
	{// ������
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Width = 0.0f;
		Height = 0.0f;
		Depth = 0.0f;
		Corner = 0.0f;
	}

	//���K��
	if (rot.x > D3DX_PI)
	{
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x < -D3DX_PI)
	{
		rot.x += D3DX_PI * 2.0f;
	}
	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}
	if (rot.z > D3DX_PI)
	{
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z < -D3DX_PI)
	{
		rot.z += D3DX_PI * 2.0f;
	}

	//�X�P�[������
	Width = max(ISOLATION_WIDTH_MIN, min(ISOLATION_WIDTH_MAX, Width));
	Height = max(ISOLATION_HEIGHT_MIN, min(ISOLATION_HEIGHT_MAX, Height));
	Depth = max(ISOLATION_DEPTH_MIN, min(ISOLATION_DEPTH_MAX, Depth));
	Corner = max(0.0f, min(1.0f, Corner));

	if (GetKeyboardTrigger(DIK_RSHIFT) || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1))
	{// �������Ƃ�
		HoldNumber = SetIsolationField(Epos + pos, Erot + rot, Width, Height, Depth, Corner, g_Type);
		EndNumber = HoldNumber;
	}
	if (GetKeyboardPress(DIK_RSHIFT) || GetJoykeyPress(JOYKEY_RB, CONTROLLER_1))
	{// �����Ă����
		g_aIsolationField[HoldNumber].bUse = false;
		EndBarrier((BTYPE)(BTYPE_ISOLATION_1 + (BTYPE)g_aIsolationField[HoldNumber].Type));
		HoldNumber = SetIsolationField(Epos + pos, Erot + rot, Width, Height, Depth, Corner, g_Type);
		EndNumber = HoldNumber;
	}
	else
	{
		HoldNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_BACK) || GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1))
	{// �������Ƃ�
		g_aIsolationField[EndNumber].bUse = false;
		EndBarrier((BTYPE)(BTYPE_ISOLATION_1 + (BTYPE)g_aIsolationField[EndNumber].Type));
		EndNumber = -1;
	}

	if (GetKeyboardTrigger(DIK_F8))
	{// �Z�[�u
		SaveIsolationField();
	}

	if (GetKeyboardTrigger(DIK_DELETE) || (GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1) && GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1)))
	{// �������Ƃ�
		if (g_DelIso != -1)
		{
			g_aIsolationField[g_DelIso].bUse = false;
			EndBarrier((BTYPE)(BTYPE_ISOLATION_1 + (BTYPE)g_aIsolationField[g_DelIso].Type));
		}
	}
#endif // _DEBUG
}

//---------------------------------------
//���[�h����
//---------------------------------------
void LoadIsolationField(void)
{
	FILE* pFile;//�t�@�C���|�C���^��錾
	int nCount = 0;
	int nNumber = 0;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Width = 0.0f, Height = 0.0f, Depth = 0.0f, Corner = 0.0f;

	pFile = fopen(ISOLATION_FIELD_FILE, "r");//�t�@�C�����J��
	if (pFile != NULL)
	{//�J������
		for (nCount = 0; nCount < ISOLATION_FIELD_MAX; nCount++)
		{//�ő吔��
			if (fscanf(pFile, "%d", &nNumber) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &pos.x) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &pos.y) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &pos.z) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &rot.x) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &rot.y) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &rot.z) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &Width) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &Height) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &Depth) == EOF)break;//���l����������
			if (fscanf(pFile, "%f", &Corner) == EOF)break;//���l����������

			//�p�x�ϊ�
			rot = D3DXToRadian(rot);

			//�ُ�ԍ�
			if (nNumber >= ISO_TYPE_MAX || nNumber < 0)
			{
				continue;
			}

			//�I�u�W�F�N�g�ݒu
			SetIsolationField(pos, rot, Width, Height, Depth, Corner, (ISO_TYPE)nNumber);
		}
		fclose(pFile);//�t�@�C�������
	}
	else
	{//�J���Ȃ�����
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "���[�h�G���[", "Iso", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//---------------------------------------
//���[�h����
//---------------------------------------
void SaveIsolationField(void)
{
	FILE* pFile;//�t�@�C���|�C���^��錾
	D3DXVECTOR3 rot;
	int nCount = 0;

	pFile = fopen(ISOLATION_FIELD_FILE, "w");//�t�@�C�����J��

	if (pFile != NULL)
	{//�J������
		for (nCount = 0; nCount < ISOLATION_FIELD_MAX; nCount++)
		{//�ő吔��
			if (g_aIsolationField[nCount].bUse && g_aIsolationField[nCount].Switch == ON)
			{//�g�p���Ă���
				//�p�x�ϊ�
				rot = D3DXToDegree(g_aIsolationField[nCount].rot);

				fprintf(pFile, "%d\n", (int)g_aIsolationField[nCount].Type);//���l����������
				fprintf(pFile, "%.1f ", g_aIsolationField[nCount].pos.x);//���l����������
				fprintf(pFile, "%.1f ", g_aIsolationField[nCount].pos.y);//���l����������
				fprintf(pFile, "%.1f\n", g_aIsolationField[nCount].pos.z);//���l����������
				fprintf(pFile, "%.1f ", rot.x);//���l����������
				fprintf(pFile, "%.1f ", rot.y);//���l����������
				fprintf(pFile, "%.1f\n", rot.z);//���l����������
				fprintf(pFile, "%.1f ", g_aIsolationField[nCount].Width);//���l����������
				fprintf(pFile, "%.1f ", g_aIsolationField[nCount].Height);//���l����������
				fprintf(pFile, "%.1f ", g_aIsolationField[nCount].Depth);//���l����������
				fprintf(pFile, "%.1f\n", g_aIsolationField[nCount].Corner);//���l����������
			}
		}
		fclose(pFile);//�t�@�C�������
	}
	else
	{//�J���Ȃ�����
		HWND hWnd;
		hWnd = GethWnd();
		MessageBox(hWnd, "���[�h�G���[", "���[�h�ł��Ȃ�������", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//--------------------
//�^�C�v�i���o�[
//--------------------
int GetIsonumber(void)
{
#ifdef _DEBUG
	return (int)g_Type;
#endif // _DEBUG
}

//--------------------
//�f���[�g�i���o�[
//--------------------
int GetIsoDelnumber(void)
{
#ifdef _DEBUG
	return g_DelIso;
#endif // _DEBUG
}