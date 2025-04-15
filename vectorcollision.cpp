//---------------------------------------
//
//�x�N�g���̓����蔻�菈���̒�`�E�錾[vectorcollision.h]
//Author fuma sato
//
//---------------------------------------
#include "main.h"

//------------------
//�����߂�
//------------------
void VectorCollision(D3DXVECTOR3& Movingpos, D3DXVECTOR3& MovingposOld, float MovingWidth, float MovingHeight, D3DXVECTOR3& StaticStartpos, D3DXVECTOR3& StaticEndpos, float StaticHeight)
{
	D3DXVECTOR3 Staticvec = {}, Collvec = {}, CollOldvec = {}, Movingvec = {}, Norvec = {}, Dovec = {}, Hit = {};
	float StaticCross, CollCross;

	Staticvec = StaticEndpos - StaticStartpos;	//�x�N�g��

	Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
	D3DXVec3Normalize(&Norvec, &Norvec);

	// �̕��̌v�Z
	D3DXVECTOR3 Pos = Movingpos + -Norvec * MovingWidth, PosOld = MovingposOld + -Norvec * MovingWidth;

	Collvec = Pos - StaticStartpos;		        //�ǂɑ΂���v���C���[�̃x�N�g��
	CollOldvec = PosOld - StaticStartpos;		//�ǂɑ΂���v���C���[�̋��x�N�g��
	Movingvec = Pos - PosOld;			        //�v���C���[�̈ړ��x�N�g��
	if (StaticHeight == NULL)
	{
		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) <= 0.0f && (Staticvec.z * CollOldvec.x) - (Staticvec.x * CollOldvec.z) >= 0.0f)
		{//�ǂ̊O��
			StaticCross = (Staticvec.z * Movingvec.x) - (Staticvec.x * Movingvec.z);
			CollCross = (Collvec.z * Movingvec.x) - (Collvec.x * Movingvec.z);
			CollCross /= StaticCross;
			if (CollCross >= -0.05f && CollCross < 1.05f)
			{//�ǂ͈̔�
				Hit = StaticStartpos + Staticvec * CollCross;
				Movingvec = Pos - Hit;//�v���C���[�̈ړ��x�N�g��
				Dovec = Norvec * ((-Movingvec.x * Norvec.x) + (-Movingvec.z * Norvec.z));
				Movingpos += Dovec * 1.1f;
			}
		}
	}
	else
	{
		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) <= 0.0f && (Staticvec.z * CollOldvec.x) - (Staticvec.x * CollOldvec.z) >= 0.0f && Pos.y < StaticStartpos.y + StaticHeight && Pos.y + MovingHeight > StaticStartpos.y)
		{//�ǂ̊O��
			StaticCross = (Staticvec.z * Movingvec.x) - (Staticvec.x * Movingvec.z);
			CollCross = (Collvec.z * Movingvec.x) - (Collvec.x * Movingvec.z);
			CollCross /= StaticCross;
			if (CollCross >= -0.05f && CollCross < 1.05f)
			{//�ǂ͈̔�
				Hit = StaticStartpos + Staticvec * CollCross;
				Movingvec = Pos - Hit;//�v���C���[�̈ړ��x�N�g��
				Dovec = Norvec * ((-Movingvec.x * Norvec.x) + (-Movingvec.z * Norvec.z));
				Movingpos += Dovec * 1.1f;
			}
		}
	}
}

//------------------
//�^�U
//------------------
_NODISCARD bool VectorTrigger(D3DXVECTOR3& Movingpos, D3DXVECTOR3& MovingposOld, float MovingWidth, float MovingHeight, D3DXVECTOR3& StaticStartpos, D3DXVECTOR3& StaticEndpos, float StaticHeight)
{
	D3DXVECTOR3 Staticvec = {}, Collvec = {}, CollOldvec = {}, Movingvec = {}, Norvec = {};
	float StaticCross, CollCross;

	Staticvec = StaticEndpos - StaticStartpos;	//�x�N�g��

	Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
	D3DXVec3Normalize(&Norvec, &Norvec);

	// �̕��̌v�Z
	D3DXVECTOR3 Pos = Movingpos + -Norvec * MovingWidth, PosOld = MovingposOld + -Norvec * MovingWidth;

	Collvec = Pos - StaticStartpos;		        //�ǂɑ΂���v���C���[�̃x�N�g��
	CollOldvec = MovingposOld - StaticStartpos;		//�ǂɑ΂���v���C���[�̋��x�N�g��
	Movingvec = Pos - PosOld;			        //�v���C���[�̈ړ��x�N�g��

	if (StaticHeight == NULL)
	{
		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) <= 0.0f && (Staticvec.z * CollOldvec.x) - (Staticvec.x * CollOldvec.z) >= 0.0f)
		{//�ǂ̊O��
			StaticCross = (Staticvec.z * Movingvec.x) - (Staticvec.x * Movingvec.z);
			CollCross = (Collvec.z * Movingvec.x) - (Collvec.x * Movingvec.z);
			CollCross /= StaticCross;
			if (CollCross >= 0.0f && CollCross < 1.0f)
			{//�ǂ͈̔�
  				return true;
			}
		}
	}
	else
	{
		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) <= 0.0f && (Staticvec.z * CollOldvec.x) - (Staticvec.x * CollOldvec.z) >= 0.0f && Pos.y < StaticStartpos.y + StaticHeight && Pos.y + MovingHeight > StaticStartpos.y)
		{//�ǂ̊O��
			StaticCross = (Staticvec.z * Movingvec.x) - (Staticvec.x * Movingvec.z);
			CollCross = (Collvec.z * Movingvec.x) - (Collvec.x * Movingvec.z);
			CollCross /= StaticCross;
			if (CollCross >= 0.0f && CollCross < 1.0f)
			{//�ǂ͈̔�
				return true;
			}
		}
	}

	return false;
}

//------------------
//����
//------------------
void VectorReflection(D3DXVECTOR3& Movingpos, D3DXVECTOR3& MovingposOld, D3DXVECTOR3& Move, float MovingWidth, float MovingHeight, D3DXVECTOR3& StaticStartpos, D3DXVECTOR3& StaticEndpos, float StaticHeight)
{
	D3DXVECTOR3 Staticvec = {}, Collvec = {}, CollOldvec = {}, Movingvec = {}, Norvec = {}, Dovec = {}, Hit = {};
	float StaticCross, CollCross;

	Staticvec = StaticEndpos - StaticStartpos;	//�x�N�g��

	Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
	D3DXVec3Normalize(&Norvec, &Norvec);

	Collvec = Movingpos - StaticStartpos;		    //�ǂɑ΂���v���C���[�̃x�N�g��
	CollOldvec = MovingposOld - StaticStartpos;		//�ǂɑ΂���v���C���[�̋��x�N�g��
	Movingvec = Movingpos - MovingposOld;			//�v���C���[�̈ړ��x�N�g��
	if (StaticHeight == NULL)
	{
		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) <= 0.0f && (Staticvec.z * CollOldvec.x) - (Staticvec.x * CollOldvec.z) >= 0.0f)
		{//�ǂ̊O��
			StaticCross = (Staticvec.z * Movingvec.x) - (Staticvec.x * Movingvec.z);
			CollCross = (Collvec.z * Movingvec.x) - (Collvec.x * Movingvec.z);
			CollCross /= StaticCross;
			if (CollCross >= -0.05f && CollCross < 1.05f)
			{//�ǂ͈̔�
				Hit = StaticStartpos + Staticvec * CollCross;
				Movingvec = Movingpos - Hit;//�v���C���[�̈ړ��x�N�g��
				Dovec = Norvec * ((-Movingvec.x * Norvec.x) + (-Movingvec.z * Norvec.z));
				Movingpos += Dovec * 2.0f;
				Move += Dovec * 2.0f;
			}
		}
	}
	else
	{
		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) <= 0.0f && (Staticvec.z * CollOldvec.x) - (Staticvec.x * CollOldvec.z) >= 0.0f && Movingpos.y < StaticStartpos.y + StaticHeight && Movingpos.y + MovingHeight > StaticStartpos.y)
		{//�ǂ̊O��
			StaticCross = (Staticvec.z * Movingvec.x) - (Staticvec.x * Movingvec.z);
			CollCross = (Collvec.z * Movingvec.x) - (Collvec.x * Movingvec.z);
			CollCross /= StaticCross;
			if (CollCross >= -0.05f && CollCross < 1.05f)
			{//�ǂ͈̔�
				Hit = StaticStartpos + Staticvec * CollCross;
				Movingvec = Movingpos - Hit;//�v���C���[�̈ړ��x�N�g��
				Dovec = Norvec * ((-Movingvec.x * Norvec.x) + (-Movingvec.z * Norvec.z));
				Movingpos += Dovec * 2.0f;
				Move += Dovec * 2.0f;
			}
		}
	}
}

//----------
//�����蔻��
//----------
bool VectorInside(D3DXVECTOR3& Movingpos, float MovingWidth, D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 Staticvec = {}, Collvec = {}, Norvec = {};

	Staticvec = pPos[1] - pPos[0];	//�x�N�g��

	Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
	D3DXVec3Normalize(&Norvec, &Norvec);

	// �̕��̌v�Z
	D3DXVECTOR3 Pos = Movingpos + Norvec * MovingWidth;

	Collvec = Pos - pPos[0];		        //�ǂɑ΂���v���C���[�̃x�N�g��

	if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) >= 0.0f)
	{//�ǂ̊O��
		Staticvec = pPos[2] - pPos[1];	//�x�N�g��

		Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
		D3DXVec3Normalize(&Norvec, &Norvec);

		// �̕��̌v�Z
		D3DXVECTOR3 Pos = Movingpos + Norvec * MovingWidth;

		Collvec = Pos - pPos[1];		        //�ǂɑ΂���v���C���[�̃x�N�g��

		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) >= 0.0f)
		{//�ǂ̊O��
			Staticvec = pPos[3] - pPos[2];	//�x�N�g��

			Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
			D3DXVec3Normalize(&Norvec, &Norvec);

			// �̕��̌v�Z
			D3DXVECTOR3 Pos = Movingpos + Norvec * MovingWidth;

			Collvec = Pos - pPos[2];		        //�ǂɑ΂���v���C���[�̃x�N�g��

			if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) >= 0.0f)
			{//�ǂ̊O��
				Staticvec = pPos[0] - pPos[3];	//�x�N�g��

				Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
				D3DXVec3Normalize(&Norvec, &Norvec);

				// �̕��̌v�Z
				D3DXVECTOR3 Pos = Movingpos + Norvec * MovingWidth;

				Collvec = Pos - pPos[3];		        //�ǂɑ΂���v���C���[�̃x�N�g��

				if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) >= 0.0f)
				{//�ǂ̊O��
					return true;
				}
			}
		}
	}
	return false;
}