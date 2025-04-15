//---------------------------------------
//
//スコア処理[score.cpp]
//Author fuma sato
//
//---------------------------------------

#include"score.h"

//--------------------
//桁分割処理
//--------------------
int Digit(int nData, int nDigit)
{
	int aNumber[SCORE_MAX - 1] = { 0 };//分割用配列
	int nData2 = 1;//計算用

	for (int i = SCORE_MAX - 2; i >= 0; i--)
	{//桁数分
		if (i == 0)
		{
			aNumber[i] = nData / nData2;//最上位の桁
		}
		aNumber[i] = nData % (nData2 * DIGIT) / nData2;//計算
		nData2 *= DIGIT;//桁引き上げ
	}

	return aNumber[nDigit];//指定の桁を返す
}