//------------------------------------
//
//����Đ������̒�`�E�錾[movie.h]
//Author
//
//------------------------------------

#include"main.h"

#define MOVIE_TEST L"data/MOVIE/Test.mp4"

//�v���g�^�C�v�錾
void InitMovie(void);   // ����������
void UninitMovie(void); // �I������
void UpdateMovie(void); // �X�V����
void DrawMovie(void);   // �`�揈��
HRESULT PlayMovie(HWND hwnd, const wchar_t* videoFilePath);