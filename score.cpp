//---------------------------------------
//
//�X�R�A����[score.cpp]
//Author fuma sato
//
//---------------------------------------

#include"score.h"

//--------------------
//����������
//--------------------
int Digit(int nData, int nDigit)
{
	int aNumber[SCORE_MAX - 1] = { 0 };//�����p�z��
	int nData2 = 1;//�v�Z�p

	for (int i = SCORE_MAX - 2; i >= 0; i--)
	{//������
		if (i == 0)
		{
			aNumber[i] = nData / nData2;//�ŏ�ʂ̌�
		}
		aNumber[i] = nData % (nData2 * DIGIT) / nData2;//�v�Z
		nData2 *= DIGIT;//�������グ
	}

	return aNumber[nDigit];//�w��̌���Ԃ�
}