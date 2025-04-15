//---------------------------------------
//
//DirectX����[stddx.cpp]
//Author fuma sato
//
//---------------------------------------
#include "stddx.h"
#include <bit>
#include "camera.h"
using std::bit_cast;

// �����_�[�X�e�[�g�̃e���v���[�g�̒�`
constexpr RSOP RSOP_TEMP[DRAW_TEMP_MAX] =
{
	// DRAW_NONE
	{
		false,              // Lighting
		D3DCULL_NONE,       // cullMode
		false,              // alphaBlendEnable
		D3DBLENDOP_ADD,     // blendOp
		D3DBLEND_ZERO,      // srcBlend
		D3DBLEND_ZERO,      // destBlend
		false,              // zEnable
		false,              // zWriteEnable
		D3DCMP_NEVER,       // zFunc
		0.0f,               // zBias
		false,              // alphaTestEnable
		0,                  // alphaRef
		D3DCMP_NEVER,       // alphaFunc
		false,              // antialiaseLineEnable
		false               // stencilEnable
	},

	// DRAW_NORMAL
	{
		true,               // Lighting
		D3DCULL_CCW,        // cullMode
		false,              // alphaBlendEnable
		D3DBLENDOP_ADD,     // blendOp
		D3DBLEND_ONE,       // srcBlend
		D3DBLEND_ZERO,      // destBlend
		true,               // zEnable
		true,               // zWriteEnable
		D3DCMP_LESSEQUAL,   // zFunc
		0.0f,               // zBias
		false,              // alphaTestEnable
		0,                  // alphaRef
		D3DCMP_ALWAYS,      // alphaFunc
		false,              // antialiaseLineEnable
		false               // stencilEnable
	},

	// DRAW_BILLBOARD (���̃e���v���[�g�����l�ɒ�`)
	{
		false,              // Lighting
		D3DCULL_NONE,       // cullMode
		true,               // alphaBlendEnable
		D3DBLENDOP_ADD,     // blendOp
		D3DBLEND_SRCALPHA,  // srcBlend
		D3DBLEND_INVSRCALPHA, // destBlend
		true,               // zEnable
		true,               // zWriteEnable
		D3DCMP_LESSEQUAL,   // zFunc
		0.0f,               // zBias
		false,              // alphaTestEnable
		0,                  // alphaRef
		D3DCMP_ALWAYS,      // alphaFunc
		false,              // antialiaseLineEnable
		false               // stencilEnable
	},

	// ���̃e���v���[�g�̒�`...
};

// �T���v���[�X�e�[�g�̃e���v���[�g�̒�`
constexpr SSOP SSOP_TEMP[DRAW_TEMP_MAX] =
{
	// DRAW_NONE
	{
		D3DTEXF_POINT,      // minFilter
		D3DTEXF_POINT,      // magFilter
		D3DTEXF_NONE,       // mipFilter
		D3DTADDRESS_WRAP,   // addressU
		D3DTADDRESS_WRAP    // addressV
	},

	// DRAW_NORMAL
	{
		D3DTEXF_LINEAR,     // minFilter
		D3DTEXF_LINEAR,     // magFilter
		D3DTEXF_LINEAR,     // mipFilter
		D3DTADDRESS_WRAP,   // addressU
		D3DTADDRESS_WRAP    // addressV
	},

	// ���̃e���v���[�g�̒�`...
};

// �e�N�X�`���X�e�[�W�X�e�[�g�̃e���v���[�g�̒�`
constexpr TSSOP TSSOP_TEMP[DRAW_TEMP_MAX] =
{
	// DRAW_NONE
	{
		D3DTOP_DISABLE,     // colorOp
		D3DTOP_DISABLE,     // alphaOp
		D3DTA_TEXTURE,      // colorArg1
		D3DTA_CURRENT       // colorArg2
	},

	// DRAW_NORMAL
	{
		D3DTOP_MODULATE,    // colorOp
		D3DTOP_MODULATE,    // alphaOp
		D3DTA_TEXTURE,      // colorArg1
		D3DTA_CURRENT       // colorArg2
	},

	// ���̃e���v���[�g�̒�`...
};

//----------------------------------------------------
//2D�̒��_�o�b�t�@�𐶐�����
//----------------------------------------------------
HRESULT CreateVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, UINT VertexSize, UINT VertexCount, DWORD Usage, D3DPOOL Pool)
{
	//���_�o�b�t�@�̐���
	RETURN_HR(GetDevice()->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VertexSize * VertexCount,
		Usage,
		FVF_VERTEX_2D,
		Pool,
		ppVtxBuff,
		nullptr
	))

	return S_OK;
}

//----------------------------------------------------
//3D�̒��_�o�b�t�@�𐶐�����
//----------------------------------------------------
HRESULT CreateVertex3D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, UINT VertexSize, UINT VertexCount, DWORD Usage, D3DPOOL Pool)
{
	//���_�o�b�t�@�̐���
	RETURN_HR(GetDevice()->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * VertexSize * VertexCount,
		Usage,
		FVF_VERTEX_3D,
		Pool,
		ppVtxBuff,
		nullptr
	))

	return S_OK;
}

//----------------------------------------------------
//�e�N�X�`���o�b�t�@�𐶐�����
//----------------------------------------------------
HRESULT CreateTexture(LPDIRECT3DTEXTURE9* ppTextureBuff, const String* TexPass, int TextureCount, D3DXIMAGE_INFO* pImageInfo, DWORD Filter, D3DPOOL Pool)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (pImageInfo == nullptr)
	{
		for (int nCntTexture = 0; nCntTexture < TextureCount; nCntTexture++)
		{
			D3DXIMAGE_INFO ImageInfo;
			//�e�N�X�`�����̎擾
			RETURN_HR(D3DXGetImageInfoFromFile
			(
				TexPass[nCntTexture].aName,
				&ImageInfo
			))

			//�e�N�X�`���̓ǂݍ���
			RETURN_HR(D3DXCreateTextureFromFileEx
			(
				pDevice,                    // �f�o�C�X�̃|�C���^
				TexPass[nCntTexture].aName, // �e�N�X�`���̃p�X
				ImageInfo.Width,            // �e�N�X�`���̕�
				ImageInfo.Height,           // �e�N�X�`���̍���
				ImageInfo.MipLevels,        // �~�b�v�}�b�v���x��
				0,                          // 0�i���I�e�N�X�`���Ȃ�D3DUSAGE_DYNAMIC�j
				ImageInfo.Format,           // �t�H�[�}�b�g
				Pool,                       // �������v�[��
				Filter,                     // �摜�̊g�k�t�B���^
				Filter,                     // �~�b�v�}�b�v�̃t�B���^
				0,                          // �J���[�L�[�i�����ɂ������F�j
				nullptr,                    // `D3DXIMAGE_INFO`�i�摜�����擾����ꍇ�j
				nullptr,                    // �p���b�g�f�[�^�i�p���b�g�Ȃ��Ȃ�NULL�j
				&ppTextureBuff[nCntTexture] // �e�N�X�`���̃|�C���^
			))
		}
	}
	else
	{
		for (int nCntTexture = 0; nCntTexture < TextureCount; nCntTexture++)
		{
			//�e�N�X�`�����̎擾
			RETURN_HR(D3DXGetImageInfoFromFile
			(
				TexPass[nCntTexture].aName,
				&pImageInfo[nCntTexture]
			))

			//�e�N�X�`���̓ǂݍ���
			RETURN_HR(D3DXCreateTextureFromFileEx
			(
				pDevice,                             // �f�o�C�X�̃|�C���^
				TexPass[nCntTexture].aName,          // �e�N�X�`���̃p�X
				pImageInfo[nCntTexture].Width,       // �e�N�X�`���̕�
				pImageInfo[nCntTexture].Height,      // �e�N�X�`���̍���
				pImageInfo[nCntTexture].MipLevels,   // �~�b�v�}�b�v���x��
				0,                                   // 0�i���I�e�N�X�`���Ȃ�D3DUSAGE_DYNAMIC�j
				pImageInfo[nCntTexture].Format,      // �t�H�[�}�b�g
				Pool,                                // �������v�[��
				Filter,                              // �摜�̊g�k�t�B���^
				Filter,                              // �~�b�v�}�b�v�̃t�B���^
				0,                                   // �J���[�L�[�i�����ɂ������F�j
				&pImageInfo[nCntTexture],            // `D3DXIMAGE_INFO`�i�摜�����擾����ꍇ�j
				nullptr,                             // �p���b�g�f�[�^�i�p���b�g�Ȃ��Ȃ�NULL�j
				&ppTextureBuff[nCntTexture]          // �e�N�X�`���̃|�C���^
			))
		}
	}
	return S_OK;
}

//----------------------------------------------------
//2D�̒��_�o�b�t�@��ݒ肷��
//----------------------------------------------------
HRESULT SetVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, DWORD LockFlags, int OffsetIdx, int BuffMax, int yDivisions, int xDivisions, float Width, float Height, float rhw, D3DXCOLOR col, int TexCount, D3DXIMAGE_INFO* pImageInfo, bool PosSenter)
{
	HRESULT hr; //�����m�F
	VERTEX_2D* pVtx;//���_���|�C���^
	int nCnt, nCnt2, nCnt3; //�J�E���g�ϐ�

	//���_�o�b�t�@�����b�N
	hr = (*ppVtxBuff)->Lock(sizeof(VERTEX_2D) * (xDivisions + 1) * (yDivisions + 1) * OffsetIdx, sizeof(VERTEX_2D) * (xDivisions + 1) * (yDivisions + 1) * BuffMax, (void**)&pVtx, LockFlags);
	RETURN_HR(hr)

	for (nCnt = 0; nCnt < BuffMax; nCnt++)
	{//�w�萔���[�v
		for (nCnt2 = 0; nCnt2 < yDivisions + 1; nCnt2++)
		{//1������Z���̃��[�v
			for (nCnt3 = 0; nCnt3 < xDivisions + 1; nCnt3++)
			{//1������X���̃��[�v
				//���W�ݒ�
				if (PosSenter)
				{
					pVtx[nCnt2 * (xDivisions + 1) + nCnt3].pos = D3DXVECTOR3(-Width * 0.5f + (Width / (float)xDivisions) * nCnt3, -Height * 0.5f + (Height / (float)yDivisions) * nCnt2, 0.0f);
				}
				else
				{
					pVtx[nCnt2 * (xDivisions + 1) + nCnt3].pos = D3DXVECTOR3(-Width * 0.5f + (Width / (float)xDivisions) * nCnt3, -Height + (Height / (float)yDivisions) * nCnt2, 0.0f);
				}

				//rhw
				pVtx[nCnt2 * (xDivisions + 1) + nCnt3].rhw = rhw;

				//�J���[
				pVtx[nCnt2 * (xDivisions + 1) + nCnt3].col = col;

				//�e�N�X�`��
				if (pImageInfo == nullptr)
				{
					for (int nCntTex = 0; nCntTex < TexCount; nCntTex++)
					{
						pVtx[nCnt2 * (xDivisions + 1) + nCnt3].tex[nCntTex] = D3DXVECTOR2((Width / (float)xDivisions) * nCnt3, (Height / (float)yDivisions) * nCnt2);
					}
				}
				else
				{
					for (int nCntTex = 0; nCntTex < TexCount; nCntTex++)
					{
						pVtx[nCnt2 * (xDivisions + 1) + nCnt3].tex[nCntTex] = D3DXVECTOR2(((Width / (float)pImageInfo[nCntTex].Width) / (float)xDivisions) * nCnt3, ((Height / (float)pImageInfo[nCntTex].Height) / (float)yDivisions) * nCnt2);
					}
				}
			}
		}
		pVtx += (xDivisions + 1) * (yDivisions + 1);
	}

	//���_�o�b�t�@���A�����b�N
	hr = (*ppVtxBuff)->Unlock();
	RETURN_HR(hr)

	return hr;
}

//----------------------------------------------------
//3D�̒��_�o�b�t�@��ݒ肷��
//----------------------------------------------------
HRESULT SetVertex3D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, DWORD LockFlags, int OffsetIdx, int BuffMax, int yDivisions, int xDivisions, float Width, float Height, D3DXCOLOR col, int TexCount, D3DXIMAGE_INFO* pImageInfo, bool PosSenter)
{
	HRESULT hr; //�����m�F
	int nCnt, nCnt2, nCnt3; //�J�E���g�ϐ�

	VERTEX_3D* pVtx;//���_���|�C���^

	//���_�o�b�t�@�����b�N
	hr = (*ppVtxBuff)->Lock(sizeof(VERTEX_3D) * (xDivisions + 1) * (yDivisions + 1) * OffsetIdx, sizeof(VERTEX_3D) * (xDivisions + 1) * (yDivisions + 1) * BuffMax, (void**)&pVtx, LockFlags);
	RETURN_HR(hr)


	for (nCnt = 0; nCnt < BuffMax; nCnt++)
	{//�w�萔���[�v
		for (nCnt2 = 0; nCnt2 < yDivisions + 1; nCnt2++)
		{//1������Z���̃��[�v
			for (nCnt3 = 0; nCnt3 < xDivisions + 1; nCnt3++)
			{//1������X���̃��[�v
				//���W�ݒ�
				if (PosSenter)
				{
					pVtx[nCnt2 * (xDivisions + 1) + nCnt3].pos = D3DXVECTOR3(-Width * 0.5f + (Width / (float)xDivisions) * nCnt3, Height * 0.5f - (Height / (float)yDivisions) * nCnt2, 0.0f);
				}
				else
				{
					pVtx[nCnt2 * (xDivisions + 1) + nCnt3].pos = D3DXVECTOR3(-Width * 0.5f + (Width / (float)xDivisions) * nCnt3, Height - (Height / (float)yDivisions) * nCnt2, 0.0f);
				}

				//nor
				pVtx[nCnt2 * (xDivisions + 1) + nCnt3].nor = NOR;

				//�J���[
				pVtx[nCnt2 * (xDivisions + 1) + nCnt3].col = col;

				//�e�N�X�`��
				if (pImageInfo == nullptr)
				{
					for (int nCntTex = 0; nCntTex < TexCount; nCntTex++)
					{
						pVtx[nCnt2 * (xDivisions + 1) + nCnt3].tex[nCntTex] = D3DXVECTOR2((Width / (float)xDivisions) * nCnt3, (Height / (float)yDivisions) * nCnt2);
					}
				}
				else
				{
					for (int nCntTex = 0; nCntTex < TexCount; nCntTex++)
					{
						pVtx[nCnt2 * (xDivisions + 1) + nCnt3].tex[nCntTex] = D3DXVECTOR2(((Width / (float)pImageInfo[nCntTex].Width) / (float)xDivisions) * nCnt3, ((Height / (float)pImageInfo[nCntTex].Height) / (float)yDivisions) * nCnt2);
					}
				}
			}
		}
		pVtx += (xDivisions + 1) * (yDivisions + 1);
	}

	//���_�o�b�t�@���A�����b�N
	hr = (*ppVtxBuff)->Unlock();
	RETURN_HR(hr)

	return hr;
}

//----------------------------------------------------
//�C���f�b�N�X�o�b�t�@��ݒ肷��
//----------------------------------------------------
HRESULT SetIndex(LPDIRECT3DINDEXBUFFER9* ppIdxBuff, int yzDivisions, int xDivisions)
{
	HRESULT hr; //�����m�F
	WORD* pIdx;
	int nCnt;

	hr = (*ppIdxBuff)->Lock(0, 0, (void**)&pIdx, 0);
	RETURN_HR(hr)


	for (nCnt = 0; nCnt < (((xDivisions + 1) * 2) * yzDivisions + (yzDivisions - 1) * 2) - (yzDivisions - 1) * 2; nCnt++)
	{//�C���f�b�N�X����
		if (nCnt % ((xDivisions + 1) * 2) == 0 && nCnt != 0)
		{
			//�C���f�b�N�X�w��
			pIdx[nCnt - 2 + ((nCnt / ((xDivisions + 1) * 2)) * 2)] = (xDivisions + 1) - (xDivisions + 1) * ((nCnt - 1) % 2) + ((nCnt - 1) / 2);
			//�C���f�b�N�X�w��
			pIdx[nCnt - 1 + ((nCnt / ((xDivisions + 1) * 2)) * 2)] = (xDivisions + 1) - (xDivisions + 1) * (nCnt % 2) + (nCnt / 2);
		}

		//�C���f�b�N�X�w��
		pIdx[nCnt + ((nCnt / ((xDivisions + 1) * 2)) * 2)] = (xDivisions + 1) - (xDivisions + 1) * (nCnt % 2) + (nCnt / 2);
	}

	hr = (*ppIdxBuff)->Unlock();
	RETURN_HR(hr)

	return hr;
}

//----------------------------------------------------
//�`������擾����
//----------------------------------------------------
HRESULT GetDrawOperation(RSOP* rsOp, SSOP* ssOp, unsigned int SamplerCount, TSSOP* tssOp, unsigned int TextureStateCount)
{
	RETURN_HR(GetRenderOperation(rsOp))                           // �����_�[�X�e�[�g���
	RETURN_HR(GetSamplerOperation(ssOp, SamplerCount))            // �T���v���[�X�e�[�g���
	RETURN_HR(GetTextureStageOperation(tssOp, TextureStateCount)) // �e�N�X�`���X�e�[�W�X�e�[�g���

	return S_OK;
}

//----------------------------------------------------
//�`�����ύX����
//----------------------------------------------------
HRESULT SetDrawOperation(RSOP* rsOp, SSOP* ssOp, unsigned int SamplerCount, TSSOP* tssOp, unsigned int TextureStateCount)
{
	RETURN_HR(SetRenderOperation(rsOp))                           // �����_�[�X�e�[�g���
	RETURN_HR(SetSamplerOperation(ssOp, SamplerCount))            // �T���v���[�X�e�[�g���
	RETURN_HR(SetTextureStageOperation(tssOp, TextureStateCount)) // �e�N�X�`���X�e�[�W�X�e�[�g���

	return S_OK;
}

//----------------------------------------------------
//�`����̃e���v���[�g���擾����
//----------------------------------------------------
HRESULT GetDrawOperationTemp(RSOP* rsOp, SSOP* ssOp, unsigned int SamplerCount, TSSOP* tssOp, unsigned int TextureStateCount, DRAW_TEMP DrawTemp)
{
	// �w�肳�ꂽ�e���v���[�g�Ɋ�Â��ĕ`������擾
	switch (DrawTemp)
	{
	case DRAW_NONE:
		break;
	}
}

//----------------------------------------------------
// �`������e���v���[�g�ɕύX���� _STD_UNSECURE_USE
//----------------------------------------------------
HRESULT SetDrawOperationTemp(unsigned int SamplerCount, unsigned int TextureStateCount, DRAW_TEMP DrawTemp)
{

}

//----------------------------------------------------
//�����_�[�X�e�[�g�����擾����
//----------------------------------------------------
HRESULT GetRenderOperation(RSOP* rsOp)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DWORD Ram;

	// ���C�e�B���O
	RETURN_HR(pDevice->GetRenderState(D3DRS_LIGHTING, &Ram))
	rsOp->Lighting = (Ram != 0);

	// �J�����O���[�h
	RETURN_HR(pDevice->GetRenderState(D3DRS_CULLMODE, &Ram))
	rsOp->cullMode = static_cast<D3DCULL>(Ram);

	// �A���t�@�u�����h
	RETURN_HR(pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &Ram))
	rsOp->alphaBlendEnable = (Ram != 0);

	// �u�����h���Z
	RETURN_HR(pDevice->GetRenderState(D3DRS_BLENDOP, &Ram))
	rsOp->blendOp = static_cast<D3DBLENDOP>(Ram);

	// �\�[�X�u�����h
	RETURN_HR(pDevice->GetRenderState(D3DRS_SRCBLEND, &Ram))
	rsOp->srcBlend = static_cast<D3DBLEND>(Ram);

	// �f�X�e�B�l�[�V�����u�����h
	RETURN_HR(pDevice->GetRenderState(D3DRS_DESTBLEND, &Ram))
	rsOp->destBlend = static_cast<D3DBLEND>(Ram);

	// Z�o�b�t�@
	RETURN_HR(pDevice->GetRenderState(D3DRS_ZENABLE, &Ram))
	rsOp->zEnable = (Ram != 0);

	// Z�o�b�t�@��������
	RETURN_HR(pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &Ram))
	rsOp->zWriteEnable = (Ram != 0);

	// Z�o�b�t�@��r�֐�
	RETURN_HR(pDevice->GetRenderState(D3DRS_ZFUNC, &Ram))
	rsOp->zFunc = static_cast<D3DCMPFUNC>(Ram);

	// Z�o�C�A�X
	RETURN_HR(pDevice->GetRenderState(D3DRS_DEPTHBIAS, &Ram))
	rsOp->zBias = bit_cast<float>(Ram); //float bit�ϊ�


	// �A���t�@�e�X�g
	RETURN_HR(pDevice->GetRenderState(D3DRS_ALPHATESTENABLE, &Ram))
	rsOp->alphaTestEnable = (Ram != 0);

	// �A���t�@�e�X�g�Q�ƒl
	RETURN_HR(pDevice->GetRenderState(D3DRS_ALPHAREF, &Ram))
	rsOp->alphaRef = Ram;

	// �A���t�@�e�X�g��r�֐�
	RETURN_HR(pDevice->GetRenderState(D3DRS_ALPHAFUNC, &Ram))
	rsOp->alphaFunc = static_cast<D3DCMPFUNC>(Ram);

	// �A���`�G�C���A�X
	RETURN_HR(pDevice->GetRenderState(D3DRS_ANTIALIASEDLINEENABLE, &Ram))
	rsOp->antialiaseLineEnable = (Ram != 0);

	// �X�e���V���e�X�g
	RETURN_HR(pDevice->GetRenderState(D3DRS_STENCILENABLE, &Ram))
	rsOp->stencilEnable = (Ram != 0);

	return S_OK;
}

//----------------------------------------------------
//�����_�[�X�e�[�g�����擾����
//----------------------------------------------------
RSOP GetRenderOperation(void)
{
	RSOP rsOp;
	CHECK_HR(GetRenderOperation(&rsOp)) // �����_�[�X�e�[�g�����擾
	return rsOp;
}

//----------------------------------------------------
//�����_�[�X�e�[�g����ύX����
//----------------------------------------------------
HRESULT SetRenderOperation(RSOP* rsOp)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	static RSOP rsOpOld = GetRenderOperation();

	if (rsOp->Lighting != rsOpOld.Lighting)
	{
		// ���C�e�B���O
		RETURN_HR(pDevice->SetRenderState(D3DRS_LIGHTING, rsOp->Lighting))
	}

	if (rsOp->cullMode != rsOpOld.cullMode)
	{
		// �J�����O���[�h
		RETURN_HR(pDevice->SetRenderState(D3DRS_CULLMODE, rsOp->cullMode))
	}

	if (rsOp->alphaBlendEnable != rsOpOld.alphaBlendEnable)
	{
		// �A���t�@�u�����h
		RETURN_HR(pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, rsOp->alphaBlendEnable))
	}

	if (rsOp->blendOp != rsOpOld.blendOp)
	{
		// �u�����h���Z
		RETURN_HR(pDevice->SetRenderState(D3DRS_BLENDOP, rsOp->blendOp))
	}

	if (rsOp->srcBlend != rsOpOld.srcBlend)
	{
		// �\�[�X�u�����h
		RETURN_HR(pDevice->SetRenderState(D3DRS_SRCBLEND, rsOp->srcBlend))
	}

	if (rsOp->destBlend != rsOpOld.destBlend)
	{
		// �f�X�e�B�l�[�V�����u�����h
		RETURN_HR(pDevice->SetRenderState(D3DRS_DESTBLEND, rsOp->destBlend))
	}

	if (rsOp->zEnable != rsOpOld.zEnable)
	{
		// Z�o�b�t�@
		RETURN_HR(pDevice->SetRenderState(D3DRS_ZENABLE, rsOp->zEnable))
	}

	if (rsOp->zWriteEnable != rsOpOld.zWriteEnable)
	{
		// Z�o�b�t�@��������
		RETURN_HR(pDevice->SetRenderState(D3DRS_ZWRITEENABLE, rsOp->zWriteEnable))
	}

	if (rsOp->zFunc != rsOpOld.zFunc)
	{
		// Z�o�b�t�@��r�֐�
		RETURN_HR(pDevice->SetRenderState(D3DRS_ZFUNC, rsOp->zFunc))
	}

	if (fabsf(rsOp->zBias - rsOpOld.zBias) > 0.0001f)
	{
		// Z�o�C�A�X
		RETURN_HR(pDevice->SetRenderState(D3DRS_DEPTHBIAS, bit_cast<DWORD>(rsOp->zBias))) //DWORD bit�ϊ�
	}

	if (rsOp->alphaTestEnable != rsOpOld.alphaTestEnable)
	{
		// �A���t�@�e�X�g
		RETURN_HR(pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, rsOp->alphaTestEnable))
	}

	if (rsOp->alphaRef != rsOpOld.alphaRef)
	{
		// �A���t�@�e�X�g�Q�ƒl
		RETURN_HR(pDevice->SetRenderState(D3DRS_ALPHAREF, rsOp->alphaRef))
	}

	if (rsOp->alphaFunc != rsOpOld.alphaFunc)
	{
		// �A���t�@�e�X�g��r�֐�
		RETURN_HR(pDevice->SetRenderState(D3DRS_ALPHAFUNC, rsOp->alphaFunc))
	}

	if (rsOp->antialiaseLineEnable != rsOpOld.antialiaseLineEnable)
	{
		// �A���`�G�C���A�X
		RETURN_HR(pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, rsOp->antialiaseLineEnable))
	}

	if (rsOp->stencilEnable != rsOpOld.stencilEnable)
	{
		// �X�e���V���e�X�g
		RETURN_HR(pDevice->SetRenderState(D3DRS_STENCILENABLE, rsOp->stencilEnable))
	}

	rsOpOld = *rsOp; // ���݂̐ݒ��ۑ�����
	return S_OK;
}

//----------------------------------------------------
//�T���v���[�X�e�[�g�����擾����
//----------------------------------------------------
HRESULT GetSamplerOperation(SSOP* ssOp, unsigned int SamplerCount)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DWORD Ram;
	SamplerCount = static_cast<unsigned int>(min(static_cast<int>(SamplerCount), TEXTURE_MAX)); // �ő吔�𒴂��Ȃ��悤�ɂ���

	for (unsigned int nCnt = 0; nCnt < SamplerCount; nCnt++)
	{
		// �ٕ����t�B���^�̍ő�l
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_MAXANISOTROPY, &Ram))
		ssOp[nCnt].maxAnisotropy = static_cast<DWORD>(Ram);

		// �e�N�X�`���~�j�t�B�P�[�V�����t�B���^
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_MINFILTER, &Ram))
		ssOp[nCnt].minFilter = static_cast<D3DTEXTUREFILTERTYPE>(Ram);

		// �e�N�X�`���}�O�j�t�B�P�[�V�����t�B���^
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_MAGFILTER, &Ram))
		ssOp[nCnt].magFilter = static_cast<D3DTEXTUREFILTERTYPE>(Ram);

		// �~�b�v�}�b�v���x��
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_MAXMIPLEVEL, &Ram))
		ssOp[nCnt].maxMipLevel = Ram;

		// �~�b�v�}�b�vLOD�o�C�A�X
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_MIPMAPLODBIAS, &Ram))

		//ssOp[nCnt].mipMapLODBias = *reinterpret_cast<float*>(&Ram); // DWORD��float�ɕϊ�
		//memcpy_s(&ssOp[nCnt].mipMapLODBias, sizeof(float), &Ram, sizeof(DWORD)); // memcpy_s���g�p���Ĉ��S�ɃR�s�[����

		TYPE_CONVERSION TypeConversion;                  // �^�ϊ��p�̋��p��
		TypeConversion.Dword = Ram;                      // DWORD�����p�̂Ɋi�[
		ssOp[nCnt].mipMapLODBias = TypeConversion.Float; // ���p�̂���float���擾����

		ssOp[nCnt].mipMapLODBias = bit_cast<float>(Ram); // float bit�ϊ�

		// �~�b�v�}�b�v�t�B���^
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_MIPFILTER, &Ram))
		ssOp[nCnt].mipFilter = static_cast<D3DTEXTUREFILTERTYPE>(Ram);

		// U�����̃A�h���X���[�h
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_ADDRESSU, &Ram))
		ssOp[nCnt].addressU = static_cast<D3DTEXTUREADDRESS>(Ram);

		// V�����̃A�h���X���[�h
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_ADDRESSV, &Ram))
		ssOp[nCnt].addressV = static_cast<D3DTEXTUREADDRESS>(Ram);

		// W�����̃A�h���X���[�h (3D�e�N�X�`���p)
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_ADDRESSW, &Ram))
		ssOp[nCnt].addressW = static_cast<D3DTEXTUREADDRESS>(Ram);

		// �{�[�_�[�J���[ (Clamp���̋��E�F)
		RETURN_HR(pDevice->GetSamplerState(nCnt, D3DSAMP_BORDERCOLOR, &Ram))
		ssOp[nCnt].borderColor = static_cast<D3DCOLOR>(Ram);
	}

	return S_OK;
}

//----------------------------------------------------
//�����_�[�X�e�[�g�����擾����
//----------------------------------------------------
SSOP* GetSamplerOperation(unsigned int SamplerCount)
{
	SSOP ssOp[TEXTURE_MAX];
	CHECK_HR(GetSamplerOperation(&ssOp[0], SamplerCount)) // �T���v���[�X�e�[�g�����擾
	return &ssOp[0];
}

//----------------------------------------------------
//�T���v���[�X�e�[�g����ύX����
//----------------------------------------------------
HRESULT SetSamplerOperation(SSOP* ssOp, unsigned int SamplerCount)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	static SSOP ssOpOld[TEXTURE_MAX]; // �T���v���[�X�e�[�g�����擾
	static bool ssOpOldFlag = false; // �������t���O
	if (!ssOpOldFlag)
	{
		// ����̂݁A�T���v���[�X�e�[�g�����擾
		CHECK_HR(GetSamplerOperation(ssOpOld, TEXTURE_MAX))
		ssOpOldFlag = true; // �t���O�𗧂Ă�
	}
	SamplerCount = static_cast<unsigned int>(min(static_cast<int>(SamplerCount), TEXTURE_MAX)); // �ő吔�𒴂��Ȃ��悤�ɂ���

	for (unsigned int nCnt = 0; nCnt < SamplerCount; nCnt++)
	{
		if (ssOp[nCnt].maxAnisotropy != ssOpOld[nCnt].maxAnisotropy)
		{
			// �ٕ����t�B���^�̍ő�l
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_MAXANISOTROPY, ssOp[nCnt].maxAnisotropy))
		}

		if (ssOp[nCnt].minFilter != ssOpOld[nCnt].minFilter)
		{
			// �e�N�X�`���~�j�t�B�P�[�V�����t�B���^
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_MINFILTER, ssOp[nCnt].minFilter))
		}

		if (ssOp[nCnt].magFilter != ssOpOld[nCnt].magFilter)
		{
			// �e�N�X�`���}�O�j�t�B�P�[�V�����t�B���^
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_MAGFILTER, ssOp[nCnt].magFilter))
		}

		if (ssOp[nCnt].maxMipLevel != ssOpOld[nCnt].maxMipLevel)
		{
			// �~�b�v�}�b�v���x��
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_MAXMIPLEVEL, ssOp[nCnt].maxMipLevel))
		}

		if (fabsf(ssOp[nCnt].mipMapLODBias - ssOpOld[nCnt].mipMapLODBias) > 0.0001f)
		{
			// �~�b�v�}�b�vLOD�o�C�A�X
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_MIPMAPLODBIAS, bit_cast<DWORD>(ssOp[nCnt].mipMapLODBias))) // DWORD bit�ϊ�
		}

		if (ssOp[nCnt].mipFilter != ssOpOld[nCnt].mipFilter)
		{
			// �~�b�v�}�b�v�t�B���^
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_MIPFILTER, ssOp[nCnt].mipFilter))
		}

		if (ssOp[nCnt].addressU != ssOpOld[nCnt].addressU)
		{
			// U�����̃A�h���X���[�h
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_ADDRESSU, ssOp[nCnt].addressU))
		}

		if (ssOp[nCnt].addressV != ssOpOld[nCnt].addressV)
		{
			// V�����̃A�h���X���[�h
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_ADDRESSV, ssOp[nCnt].addressV))
		}

		if (ssOp[nCnt].addressW != ssOpOld[nCnt].addressW)
		{
			// W�����̃A�h���X���[�h (3D�e�N�X�`���p)
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_ADDRESSW, ssOp[nCnt].addressW))
		}

		if (ssOp[nCnt].borderColor != ssOpOld[nCnt].borderColor)
		{
			// �{�[�_�[�J���[ (Clamp���̋��E�F)
			RETURN_HR(pDevice->SetSamplerState(nCnt, D3DSAMP_BORDERCOLOR, ssOp[nCnt].borderColor))
		}

		ssOpOld[nCnt] = ssOp[nCnt]; // ���݂̐ݒ��ۑ�����
	}

	return S_OK;
}

//----------------------------------------------------
//�e�N�X�`���X�e�[�W�X�e�[�g�����擾����
//----------------------------------------------------
HRESULT GetTextureStageOperation(TSSOP* tssOp, unsigned int TextureStateCount)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DWORD Ram;
	TextureStateCount = static_cast<unsigned int>(min(static_cast<int>(TextureStateCount), TEXTURE_MAX)); // �ő吔�𒴂��Ȃ��悤�ɂ���

	for (unsigned int nCnt = 0; nCnt < TextureStateCount; nCnt++)
	{
		// �J���[�I�y���[�V����
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_COLOROP, &Ram))
		tssOp[nCnt].colorOp = static_cast<D3DTEXTUREOP>(Ram);

		// �I�y���[�V�����̈���0 (�ꕔ�̃I�y���[�V�����p)
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_COLORARG0, &Ram))
		tssOp[nCnt].colorArg0 = static_cast<DWORD>(Ram);

		// �I�y���[�V�����̈���1
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_COLORARG1, &Ram))
		tssOp[nCnt].colorArg1 = static_cast<DWORD>(Ram);

		// �I�y���[�V�����̈���2
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_COLORARG2, &Ram))
		tssOp[nCnt].colorArg2 = static_cast<DWORD>(Ram);

		// �A���t�@�I�y���[�V����
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_ALPHAOP, &Ram))
		tssOp[nCnt].alphaOp = static_cast<D3DTEXTUREOP>(Ram);

		// �I�y���[�V�����̈���0 (�ꕔ�̃I�y���[�V�����p)
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_ALPHAARG0, &Ram))
		tssOp[nCnt].alphaArg0 = static_cast<DWORD>(Ram);

		// �I�y���[�V�����̈���1
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_ALPHAARG1, &Ram))
		tssOp[nCnt].alphaArg1 = static_cast<DWORD>(Ram);

		// �I�y���[�V�����̈���2
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_ALPHAARG2, &Ram))
		tssOp[nCnt].alphaArg2 = static_cast<DWORD>(Ram);

		// �e�N�X�`���̃C���f�b�N�X
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_TEXCOORDINDEX, &Ram))
		tssOp[nCnt].texCoordIndex = static_cast<DWORD>(Ram);

		// �e�N�X�`���ϊ�
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_TEXTURETRANSFORMFLAGS, &Ram))
		tssOp[nCnt].texTureTransFormFlags = static_cast<D3DTEXTURETRANSFORMFLAGS>(Ram);

		// ���ʂ̊i�[��
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_RESULTARG, &Ram))
		tssOp[nCnt].resultArg = static_cast<DWORD>(Ram);

		// �萔�l (D3DTA_CONSTANT���g�p��)
		RETURN_HR(pDevice->GetTextureStageState(nCnt, D3DTSS_CONSTANT, &Ram))
		tssOp[nCnt].constant = static_cast<D3DCOLOR>(Ram);
	}

	return S_OK;
}

//----------------------------------------------------
//�����_�[�X�e�[�g�����擾����
//----------------------------------------------------
TSSOP* GetTextureStageOperation(unsigned int TextureStateCount)
{
	TSSOP tssOp[TEXTURE_MAX];
	CHECK_HR(GetTextureStageOperation(&tssOp[0], TextureStateCount)) // �e�N�X�`���X�e�[�W�X�e�[�g�����擾
	return &tssOp[0];
}

//----------------------------------------------------
//�e�N�X�`���X�e�[�W�X�e�[�g����ύX����
//----------------------------------------------------
HRESULT SetTextureStageOperation(TSSOP* tssOp, unsigned int TextureStateCount)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	static TSSOP tssOpOld[TEXTURE_MAX];
	static bool tssOpOldFlag = false; // �������t���O
	if (!tssOpOldFlag)
	{
		// ����̂݁A�e�N�X�`���X�e�[�W�X�e�[�g�����擾
		CHECK_HR(GetTextureStageOperation(tssOpOld, TEXTURE_MAX))
		tssOpOldFlag = true; // �t���O�𗧂Ă�
	}
	TextureStateCount = static_cast<unsigned int>(min(static_cast<int>(TextureStateCount), TEXTURE_MAX)); // �ő吔�𒴂��Ȃ��悤�ɂ���

	for (unsigned int nCnt = 0; nCnt < TextureStateCount; nCnt++)
	{
		if (tssOp[nCnt].colorOp != tssOpOld[nCnt].colorOp)
		{
			// �J���[�I�y���[�V����
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_COLOROP, tssOp[nCnt].colorOp))
		}

		if (tssOp[nCnt].colorArg0 != tssOpOld[nCnt].colorArg0)
		{
			// �I�y���[�V�����̈���0 (�ꕔ�̃I�y���[�V�����p)
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_COLORARG0, tssOp[nCnt].colorArg0))
		}

		if (tssOp[nCnt].colorArg1 != tssOpOld[nCnt].colorArg1)
		{
			// �I�y���[�V�����̈���1
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_COLORARG1, tssOp[nCnt].colorArg1))
		}

		if (tssOp[nCnt].colorArg2 != tssOpOld[nCnt].colorArg2)
		{
			// �I�y���[�V�����̈���2
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_COLORARG2, tssOp[nCnt].colorArg2))
		}

		if (tssOp[nCnt].alphaOp != tssOpOld[nCnt].alphaOp)
		{
			// �A���t�@�I�y���[�V����
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_ALPHAOP, tssOp[nCnt].alphaOp))
		}

		if (tssOp[nCnt].alphaArg0 != tssOpOld[nCnt].alphaArg0)
		{
			// �I�y���[�V�����̈���0 (�ꕔ�̃I�y���[�V�����p)
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_ALPHAARG0, tssOp[nCnt].alphaArg0))
		}

		if (tssOp[nCnt].alphaArg1 != tssOpOld[nCnt].alphaArg1)
		{
			// �I�y���[�V�����̈���1
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_ALPHAARG1, tssOp[nCnt].alphaArg1))
		}

		if (tssOp[nCnt].alphaArg2 != tssOpOld[nCnt].alphaArg2)
		{
			// �I�y���[�V�����̈���2
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_ALPHAARG2, tssOp[nCnt].alphaArg2))
		}

		if (tssOp[nCnt].texCoordIndex != tssOpOld[nCnt].texCoordIndex)
		{
			// �e�N�X�`���̃C���f�b�N�X
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_TEXCOORDINDEX, tssOp[nCnt].texCoordIndex))
		}

		if (tssOp[nCnt].texTureTransFormFlags != tssOpOld[nCnt].texTureTransFormFlags)
		{
			// �e�N�X�`���ϊ�
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_TEXTURETRANSFORMFLAGS, tssOp[nCnt].texTureTransFormFlags))
		}

		if (tssOp[nCnt].resultArg != tssOpOld[nCnt].resultArg)
		{
			// ���ʂ̊i�[��
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_RESULTARG, tssOp[nCnt].resultArg))
		}

		if (tssOp[nCnt].constant != tssOpOld[nCnt].constant)
		{
			// �萔�l (D3DTA_CONSTANT���g�p��)
			RETURN_HR(pDevice->SetTextureStageState(nCnt, D3DTSS_CONSTANT, tssOp[nCnt].constant))
		}

		tssOpOld[nCnt] = tssOp[nCnt]; // ���݂̐ݒ��ۑ�����
	}

	return S_OK;
}

//----------------------------------------------------
//2D�̃o�b�t�@��`�悷��
//----------------------------------------------------
HRESULT DrawVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, LPDIRECT3DINDEXBUFFER9* ppIdxBuff, LPDIRECT3DTEXTURE9* ppTextureBuff, D3DPRIMITIVETYPE PrimitiveType, UINT VertexSize, UINT OffsetIdx, UINT PrimitiveCount)
{
	if (ppVtxBuff == nullptr || *ppVtxBuff == nullptr)
	{
		return E_POINTER;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@
	RETURN_HR(pDevice->SetStreamSource(0, ppVtxBuff[0], 0, sizeof(VERTEX_2D)))

	if (ppIdxBuff != nullptr)
	{
		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		RETURN_HR(pDevice->SetIndices(ppIdxBuff[0]))
	}

	//���_�t�H�[�}�b�g�̐ݒ�
	RETURN_HR(pDevice->SetFVF(FVF_VERTEX_2D))

	if (ppTextureBuff != nullptr)
	{
		//�e�N�X�`���̐ݒ�
		RETURN_HR(pDevice->SetTexture(0, ppTextureBuff[0]))
	}

	if (ppIdxBuff != nullptr)
	{
		//�|���S���̕`��
		RETURN_HR(pDevice->DrawIndexedPrimitive
		(
			PrimitiveType,//�^�C�v
			VertexSize * OffsetIdx,
			0,
			VertexSize,//���_��
			0,
			PrimitiveCount//�|���S���̌�
		))
	}
	else
	{
		//�|���S���̕`��
		RETURN_HR(pDevice->DrawPrimitive
		(
			PrimitiveType,//�^�C�v
			VertexSize * OffsetIdx,//�n�܂�̔ԍ�
			PrimitiveCount//�|���S���̌�
		))
	}

	return S_OK;
}

//----------------------------------------------------
//3D�̃o�b�t�@��`�悷��
//----------------------------------------------------
HRESULT DrawVertex3D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, LPDIRECT3DINDEXBUFFER9* ppIdxBuff, LPDIRECT3DTEXTURE9* ppTextureBuff, D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, D3DXVECTOR3* pScale, D3DXMATRIX* pMtxWorld, BILLBOARD_TYPE BillboardType, float zBias, D3DPRIMITIVETYPE PrimitiveType, UINT VertexSize, UINT OffsetIdx, UINT PrimitiveCount)
{
	if (ppVtxBuff == nullptr || *ppVtxBuff == nullptr)
	{
		return E_POINTER;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;//�v�Z�}�g���b�N�X

	if (zBias >= 0.00001f)
	{// Z�o�C�A�X�ݒ�
		RSOP rsOp;
		GetRenderOperation(&rsOp);
		rsOp.zBias += zBias; // �����_�[�X�e�[�g�������ɖ߂��܂ő���������
		SetRenderOperation(&rsOp);
	}

	if (pMtxWorld != nullptr)
	{
		//�}�g���b�N�X������
		D3DXMatrixIdentity(pMtxWorld);

		//�r���[�}�g���b�N�X�擾
		D3DXMATRIX mtxView;
		switch (BillboardType)
		{
		case BILLBOARD_NONE:
			break;
		case BILLBOARD_FULL:
			RETURN_HR(pDevice->GetTransform(D3DTS_VIEW, &mtxView))
			//�J�����̋t�s���ݒ�
			pMtxWorld->_11 = mtxView._11;
			pMtxWorld->_12 = mtxView._21;
			pMtxWorld->_13 = mtxView._31;
			pMtxWorld->_21 = mtxView._12;
			pMtxWorld->_22 = mtxView._22;
			pMtxWorld->_23 = mtxView._32;
			pMtxWorld->_31 = mtxView._13;
			pMtxWorld->_32 = mtxView._23;
			pMtxWorld->_33 = mtxView._33;
			break;
		case BILLBOARD_Y_AXIS:
			RETURN_HR(pDevice->GetTransform(D3DTS_VIEW, &mtxView))
			//�J�����̋t�s���ݒ�
			D3DXVECTOR3 zAxis = D3DXVECTOR3(mtxView._31, 0.0f, mtxView._33); // Y�������[����
			D3DXVec3Normalize(&zAxis, &zAxis);

			D3DXVECTOR3 yAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // �Œ�

			D3DXVECTOR3 xAxis;
			D3DXVec3Cross(&xAxis, &yAxis, &zAxis);
			D3DXVec3Normalize(&xAxis, &xAxis);

			pMtxWorld->_11 = xAxis.x;
			pMtxWorld->_12 = xAxis.y;
			pMtxWorld->_13 = xAxis.z;

			pMtxWorld->_21 = yAxis.x;
			pMtxWorld->_22 = yAxis.y;
			pMtxWorld->_23 = yAxis.z;

			pMtxWorld->_31 = zAxis.x;
			pMtxWorld->_32 = zAxis.y;
			pMtxWorld->_33 = zAxis.z;
			break;
		case BILLBOARD_XZ_AXIS:
			RETURN_HR(pDevice->GetTransform(D3DTS_VIEW, &mtxView))
			//�J�����̋t�s���ݒ�
			D3DXVECTOR3 xAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f); // X�����Œ�
			D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, 0.0f, 1.0f); // Z�����Œ�

			D3DXVECTOR3 yAxis;
			D3DXVec3Cross(&yAxis, &zAxis, &xAxis);
			D3DXVec3Normalize(&yAxis, &yAxis);

			pMtxWorld->_11 = xAxis.x;
			pMtxWorld->_12 = xAxis.y;
			pMtxWorld->_13 = xAxis.z;

			pMtxWorld->_21 = yAxis.x;
			pMtxWorld->_22 = yAxis.y;
			pMtxWorld->_23 = yAxis.z;

			pMtxWorld->_31 = zAxis.x;
			pMtxWorld->_32 = zAxis.y;
			pMtxWorld->_33 = zAxis.z;
			break;
		}

		if (pScale != nullptr)
		{
			//�傫���̔��f
			D3DXMatrixScaling(&mtxScale, pScale->x, pScale->y, pScale->z);
			D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxScale);
		}

		if (pRot != nullptr)
		{
			//�����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pRot->y, pRot->x, pRot->z);
			D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxRot);
		}

		if (pPos != nullptr)
		{
			//�ʒu�̌v�Z
			D3DXMatrixTranslation(&mtxTrans, pPos->x, pPos->y, pPos->z);
			D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxTrans);
		}

		//���[���h�}�g���b�N�X�̐ݒ�
		RETURN_HR(pDevice->SetTransform(D3DTS_WORLD, pMtxWorld))
	}

	//���_�o�b�t�@
	RETURN_HR(pDevice->SetStreamSource(0, ppVtxBuff[0], 0, sizeof(VERTEX_3D)))

	if (ppIdxBuff != nullptr)
	{
		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		RETURN_HR(pDevice->SetIndices(ppIdxBuff[0]))
	}

	//���_�t�H�[�}�b�g�̐ݒ�
	RETURN_HR(pDevice->SetFVF(FVF_VERTEX_3D))

	if (ppTextureBuff != nullptr)
	{
		//�e�N�X�`���̐ݒ�
		RETURN_HR(pDevice->SetTexture(0, ppTextureBuff[0]))
	}

	if (ppIdxBuff != nullptr)
	{
		//�|���S���̕`��
		RETURN_HR(pDevice->DrawIndexedPrimitive
		(
			PrimitiveType,//�^�C�v
			VertexSize * OffsetIdx,
			0,
			VertexSize,//���_��
			0,
			PrimitiveCount//�|���S���̌�
		))
	}
	else
	{
		//�|���S���̕`��
		RETURN_HR(pDevice->DrawPrimitive
		(
			PrimitiveType,//�^�C�v
			VertexSize * OffsetIdx,//�n�܂�̔ԍ�
			PrimitiveCount//�|���S���̌�
		))
	}

	return S_OK;
}