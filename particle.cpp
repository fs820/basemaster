//---------------------------------------
//
//�e�\������[particle.cpp]
//Author fuma sato
//
//---------------------------------------

#include"particle.h"
#include"player.h"
#include"effect.h"

//�e�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 scale;//�X�P�[��
	int nLife;//����
	bool bUse;//�g�p���Ă��邩�ǂ���
}PARTICLE;

//�O���[�o��
PARTICLE g_aParticle[MAX_PARTICLE];//�e�̏��

//-----------------
//�e�̏���������
//-----------------
void InitParticle(void)
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		g_aParticle[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle[i].nLife = PARTICLE_LIFE;
		g_aParticle[i].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
	}
}

//----------------
//�e�̏I������
//----------------
void UninitParticle(void)
{

}

//----------------
//�e�̍X�V����
//----------------
void UpdateParticle(void)
{

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (g_aParticle[i].bUse == true)
		{//�e���g�p����Ă���

			for (int i2 = 0; i2 < PARTICLE_NUM; i2++)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), move = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f), dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DCOLOR col = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
				float fangle = 0.0f, fspeed = 0.0f, fscale = 0.0f, length = 0.0f;
				int nLife=0;
				pos=g_aParticle[i].pos;
				fangle = (float)(rand() % 629 - 314) / 100.0f;
				fspeed = (float)(rand() % 100) / 10.0f + 1.0f;
				dir.x = (float)(rand() % 201 - 100) / 100.0f;
				dir.y = (float)(rand() % 201 - 100) / 100.0f;
				dir.z = (float)(rand() % 201 - 100) / 100.0f;
				length = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
				dir.x /= length;
				dir.y /= length;
				dir.z /= length;
				move.x = dir.x * fspeed;
				move.y = dir.y * fspeed;
				move.z = dir.z * fspeed;
				col = D3DXCOLOR((float)(rand() % 101) / 100.0f, (float)(rand() % 101) / 100.0f, (float)(rand() % 101) / 100.0f, (float)(rand() % 51) / 100.0f);
				fscale = (float)(rand() % (int)(g_aParticle[i].scale.x + 1.0f) * 10.0f);
				scale.x = fscale;
				scale.y = fscale;
				scale.z = fscale;
				nLife = g_aParticle[i].nLife;

				SetEffect(pos, move, col, scale, nLife, EFFECT_TYPE_NORMAL);
			}

			g_aParticle[i].nLife--;
			g_aParticle[i].scale *= ((float)g_aParticle[i].nLife) / ((float)PARTICLE_LIFE);

			if (g_aParticle[i].nLife <= 0)
			{
				g_aParticle[i].bUse = false;
			}
		}
	}
}

//-------------------
//���ʏ�e
//-------------------
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (g_aParticle[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aParticle[i].pos = pos;
			g_aParticle[i].scale = scale;
			g_aParticle[i].nLife = PARTICLE_LIFE;
			g_aParticle[i].bUse = true;
			break;
		}
	}
}