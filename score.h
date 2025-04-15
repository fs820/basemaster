//---------------------------------------
//
//スコア処理の定義・宣言[score.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _SCORE_H_
#define _SCORE_H_

#include"main.h"

#define SCORE_MAX (5)//桁数
#define U_MAX_S (11)//テクスチャ数(横)
#define V_MAX_S (1)//テクスチャ数(縦)
#define SCORE_WIDTH (160.0f)//サイズ
#define SCORE_HEIGHT (64.0f)//サイズ
#define DIGIT (10)//進数

int Digit(int nData, int nDigit);//分割処理

#endif _SCORE_H_