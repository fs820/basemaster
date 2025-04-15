//---------------------------------------
//
//ベクトルの当たり判定処理の定義・宣言[vectorcollision.h]
//Author fuma sato
//
//---------------------------------------
#include "main.h"

//------------------
//押し戻し
//------------------
void VectorCollision(D3DXVECTOR3& Movingpos, D3DXVECTOR3& MovingposOld, float MovingWidth, float MovingHeight, D3DXVECTOR3& StaticStartpos, D3DXVECTOR3& StaticEndpos, float StaticHeight)
{
	D3DXVECTOR3 Staticvec = {}, Collvec = {}, CollOldvec = {}, Movingvec = {}, Norvec = {}, Dovec = {}, Hit = {};
	float StaticCross, CollCross;

	Staticvec = StaticEndpos - StaticStartpos;	//ベクトル

	Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
	D3DXVec3Normalize(&Norvec, &Norvec);

	// 体分の計算
	D3DXVECTOR3 Pos = Movingpos + -Norvec * MovingWidth, PosOld = MovingposOld + -Norvec * MovingWidth;

	Collvec = Pos - StaticStartpos;		        //壁に対するプレイヤーのベクトル
	CollOldvec = PosOld - StaticStartpos;		//壁に対するプレイヤーの旧ベクトル
	Movingvec = Pos - PosOld;			        //プレイヤーの移動ベクトル
	if (StaticHeight == NULL)
	{
		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) <= 0.0f && (Staticvec.z * CollOldvec.x) - (Staticvec.x * CollOldvec.z) >= 0.0f)
		{//壁の外側
			StaticCross = (Staticvec.z * Movingvec.x) - (Staticvec.x * Movingvec.z);
			CollCross = (Collvec.z * Movingvec.x) - (Collvec.x * Movingvec.z);
			CollCross /= StaticCross;
			if (CollCross >= -0.05f && CollCross < 1.05f)
			{//壁の範囲
				Hit = StaticStartpos + Staticvec * CollCross;
				Movingvec = Pos - Hit;//プレイヤーの移動ベクトル
				Dovec = Norvec * ((-Movingvec.x * Norvec.x) + (-Movingvec.z * Norvec.z));
				Movingpos += Dovec * 1.1f;
			}
		}
	}
	else
	{
		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) <= 0.0f && (Staticvec.z * CollOldvec.x) - (Staticvec.x * CollOldvec.z) >= 0.0f && Pos.y < StaticStartpos.y + StaticHeight && Pos.y + MovingHeight > StaticStartpos.y)
		{//壁の外側
			StaticCross = (Staticvec.z * Movingvec.x) - (Staticvec.x * Movingvec.z);
			CollCross = (Collvec.z * Movingvec.x) - (Collvec.x * Movingvec.z);
			CollCross /= StaticCross;
			if (CollCross >= -0.05f && CollCross < 1.05f)
			{//壁の範囲
				Hit = StaticStartpos + Staticvec * CollCross;
				Movingvec = Pos - Hit;//プレイヤーの移動ベクトル
				Dovec = Norvec * ((-Movingvec.x * Norvec.x) + (-Movingvec.z * Norvec.z));
				Movingpos += Dovec * 1.1f;
			}
		}
	}
}

//------------------
//真偽
//------------------
_NODISCARD bool VectorTrigger(D3DXVECTOR3& Movingpos, D3DXVECTOR3& MovingposOld, float MovingWidth, float MovingHeight, D3DXVECTOR3& StaticStartpos, D3DXVECTOR3& StaticEndpos, float StaticHeight)
{
	D3DXVECTOR3 Staticvec = {}, Collvec = {}, CollOldvec = {}, Movingvec = {}, Norvec = {};
	float StaticCross, CollCross;

	Staticvec = StaticEndpos - StaticStartpos;	//ベクトル

	Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
	D3DXVec3Normalize(&Norvec, &Norvec);

	// 体分の計算
	D3DXVECTOR3 Pos = Movingpos + -Norvec * MovingWidth, PosOld = MovingposOld + -Norvec * MovingWidth;

	Collvec = Pos - StaticStartpos;		        //壁に対するプレイヤーのベクトル
	CollOldvec = MovingposOld - StaticStartpos;		//壁に対するプレイヤーの旧ベクトル
	Movingvec = Pos - PosOld;			        //プレイヤーの移動ベクトル

	if (StaticHeight == NULL)
	{
		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) <= 0.0f && (Staticvec.z * CollOldvec.x) - (Staticvec.x * CollOldvec.z) >= 0.0f)
		{//壁の外側
			StaticCross = (Staticvec.z * Movingvec.x) - (Staticvec.x * Movingvec.z);
			CollCross = (Collvec.z * Movingvec.x) - (Collvec.x * Movingvec.z);
			CollCross /= StaticCross;
			if (CollCross >= 0.0f && CollCross < 1.0f)
			{//壁の範囲
  				return true;
			}
		}
	}
	else
	{
		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) <= 0.0f && (Staticvec.z * CollOldvec.x) - (Staticvec.x * CollOldvec.z) >= 0.0f && Pos.y < StaticStartpos.y + StaticHeight && Pos.y + MovingHeight > StaticStartpos.y)
		{//壁の外側
			StaticCross = (Staticvec.z * Movingvec.x) - (Staticvec.x * Movingvec.z);
			CollCross = (Collvec.z * Movingvec.x) - (Collvec.x * Movingvec.z);
			CollCross /= StaticCross;
			if (CollCross >= 0.0f && CollCross < 1.0f)
			{//壁の範囲
				return true;
			}
		}
	}

	return false;
}

//------------------
//反射
//------------------
void VectorReflection(D3DXVECTOR3& Movingpos, D3DXVECTOR3& MovingposOld, D3DXVECTOR3& Move, float MovingWidth, float MovingHeight, D3DXVECTOR3& StaticStartpos, D3DXVECTOR3& StaticEndpos, float StaticHeight)
{
	D3DXVECTOR3 Staticvec = {}, Collvec = {}, CollOldvec = {}, Movingvec = {}, Norvec = {}, Dovec = {}, Hit = {};
	float StaticCross, CollCross;

	Staticvec = StaticEndpos - StaticStartpos;	//ベクトル

	Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
	D3DXVec3Normalize(&Norvec, &Norvec);

	Collvec = Movingpos - StaticStartpos;		    //壁に対するプレイヤーのベクトル
	CollOldvec = MovingposOld - StaticStartpos;		//壁に対するプレイヤーの旧ベクトル
	Movingvec = Movingpos - MovingposOld;			//プレイヤーの移動ベクトル
	if (StaticHeight == NULL)
	{
		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) <= 0.0f && (Staticvec.z * CollOldvec.x) - (Staticvec.x * CollOldvec.z) >= 0.0f)
		{//壁の外側
			StaticCross = (Staticvec.z * Movingvec.x) - (Staticvec.x * Movingvec.z);
			CollCross = (Collvec.z * Movingvec.x) - (Collvec.x * Movingvec.z);
			CollCross /= StaticCross;
			if (CollCross >= -0.05f && CollCross < 1.05f)
			{//壁の範囲
				Hit = StaticStartpos + Staticvec * CollCross;
				Movingvec = Movingpos - Hit;//プレイヤーの移動ベクトル
				Dovec = Norvec * ((-Movingvec.x * Norvec.x) + (-Movingvec.z * Norvec.z));
				Movingpos += Dovec * 2.0f;
				Move += Dovec * 2.0f;
			}
		}
	}
	else
	{
		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) <= 0.0f && (Staticvec.z * CollOldvec.x) - (Staticvec.x * CollOldvec.z) >= 0.0f && Movingpos.y < StaticStartpos.y + StaticHeight && Movingpos.y + MovingHeight > StaticStartpos.y)
		{//壁の外側
			StaticCross = (Staticvec.z * Movingvec.x) - (Staticvec.x * Movingvec.z);
			CollCross = (Collvec.z * Movingvec.x) - (Collvec.x * Movingvec.z);
			CollCross /= StaticCross;
			if (CollCross >= -0.05f && CollCross < 1.05f)
			{//壁の範囲
				Hit = StaticStartpos + Staticvec * CollCross;
				Movingvec = Movingpos - Hit;//プレイヤーの移動ベクトル
				Dovec = Norvec * ((-Movingvec.x * Norvec.x) + (-Movingvec.z * Norvec.z));
				Movingpos += Dovec * 2.0f;
				Move += Dovec * 2.0f;
			}
		}
	}
}

//----------
//当たり判定
//----------
bool VectorInside(D3DXVECTOR3& Movingpos, float MovingWidth, D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 Staticvec = {}, Collvec = {}, Norvec = {};

	Staticvec = pPos[1] - pPos[0];	//ベクトル

	Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
	D3DXVec3Normalize(&Norvec, &Norvec);

	// 体分の計算
	D3DXVECTOR3 Pos = Movingpos + Norvec * MovingWidth;

	Collvec = Pos - pPos[0];		        //壁に対するプレイヤーのベクトル

	if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) >= 0.0f)
	{//壁の外側
		Staticvec = pPos[2] - pPos[1];	//ベクトル

		Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
		D3DXVec3Normalize(&Norvec, &Norvec);

		// 体分の計算
		D3DXVECTOR3 Pos = Movingpos + Norvec * MovingWidth;

		Collvec = Pos - pPos[1];		        //壁に対するプレイヤーのベクトル

		if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) >= 0.0f)
		{//壁の外側
			Staticvec = pPos[3] - pPos[2];	//ベクトル

			Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
			D3DXVec3Normalize(&Norvec, &Norvec);

			// 体分の計算
			D3DXVECTOR3 Pos = Movingpos + Norvec * MovingWidth;

			Collvec = Pos - pPos[2];		        //壁に対するプレイヤーのベクトル

			if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) >= 0.0f)
			{//壁の外側
				Staticvec = pPos[0] - pPos[3];	//ベクトル

				Norvec = D3DXVECTOR3(Staticvec.z, Staticvec.y, -Staticvec.x);
				D3DXVec3Normalize(&Norvec, &Norvec);

				// 体分の計算
				D3DXVECTOR3 Pos = Movingpos + Norvec * MovingWidth;

				Collvec = Pos - pPos[3];		        //壁に対するプレイヤーのベクトル

				if ((Staticvec.z * Collvec.x) - (Staticvec.x * Collvec.z) >= 0.0f)
				{//壁の外側
					return true;
				}
			}
		}
	}
	return false;
}