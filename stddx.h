//---------------------------------------
//
//DirectX処理の定義・宣言[stddx.h]
//Author fuma sato
//
//---------------------------------------
#ifndef _STDDX_H_
#define _STDDX_H_
#include"main.h"

// ビルボードタイプ
typedef enum
{
    BILLBOARD_NONE = 0,          // ビルボードなし
    BILLBOARD_FULL,              // フルビルボード（カメラに完全追従）
    BILLBOARD_Y_AXIS,            // Y軸固定（立ち看板タイプ）
    BILLBOARD_XZ_AXIS,           // X/Z軸固定（地面に沿うタイプ）
    BILLBOARD_TYPE_MAX           // 種類の上限
}BILLBOARD_TYPE;

// 描画タイプテンプレート
typedef enum
{
    DRAW_NONE = 0,      // レンダーステートなし
    DRAW_NORMAL,        // 通常
    DRAW_BILLBOARD,     // ビルボード
    DRAW_EFFECT,        // エフェクト
    DRAW_SHADOW,        // シャドウ
    DRAW_BARRIER,       // バリア
    DRAW_UI,            // UI要素（HUD、メニューなど）
    DRAW_TRANSPARENT,   // 透明/半透明オブジェクト
    DRAW_ADDITIVE,      // 加算合成（光、パーティクルなど）
    DRAW_SUBTRACTIVE,   // 減算合成
    DRAW_MULTIPLY,      // 乗算合成
    DRAW_WIREFRAME,     // ワイヤーフレーム表示
    DRAW_SKYBOX,        // スカイボックス
    DRAW_WATER,         // 水面
    DRAW_OUTLINE,       // アウトライン描画
    DRAW_TEMP_MAX       // テンプレートの上限
} DRAW_TEMP;

// レンダーステート管理構造体
typedef struct
{
    bool Lighting = false;               // ライティング
    D3DCULL cullMode = D3DCULL_NONE;     // カリングモード
    bool alphaBlendEnable = false;       // アルファブレンド
    D3DBLENDOP blendOp = D3DBLENDOP_ADD; // ブレンド演算
    D3DBLEND srcBlend = D3DBLEND_ZERO;   // ソースブレンド
    D3DBLEND destBlend = D3DBLEND_ZERO;  // デスティネーションブレンド
    bool zEnable = false;                // Zバッファ
    bool zWriteEnable = false;           // Zバッファ書き込み
    D3DCMPFUNC zFunc = D3DCMP_NEVER;     // Zバッファ比較関数
	float zBias = 0.0f;                  // Zバイアス
    bool alphaTestEnable = false;        // アルファテスト
    DWORD alphaRef = 0;                  // アルファテスト参照値
    D3DCMPFUNC alphaFunc = D3DCMP_NEVER; // アルファテスト比較関数
    bool antialiaseLineEnable = false;   // アンチエイリアス
    bool stencilEnable = false;          // ステンシルテスト
} RSOP;

// サンプラーステート管理構造体
typedef struct
{
    DWORD maxAnisotropy = 1;                             // 異方性フィルタの最大値
    D3DTEXTUREFILTERTYPE minFilter = D3DTEXF_NONE;       // テクスチャミニフィケーションフィルタ
    D3DTEXTUREFILTERTYPE magFilter = D3DTEXF_NONE;       // テクスチャマグニフィケーションフィルタ
    DWORD maxMipLevel = 0;                               // ミップマップレベル
    float mipMapLODBias = 0.0f;                          // ミップマップLODバイアス
    D3DTEXTUREFILTERTYPE mipFilter = D3DTEXF_NONE;       // ミップマップフィルタ
    D3DTEXTUREADDRESS addressU = D3DTADDRESS_WRAP;       // U方向のアドレスモード
    D3DTEXTUREADDRESS addressV = D3DTADDRESS_WRAP;       // V方向のアドレスモード
    D3DTEXTUREADDRESS addressW = D3DTADDRESS_WRAP;       // W方向のアドレスモード (3Dテクスチャ用)
    D3DCOLOR borderColor = D3DCOLOR_ARGB(255, 255, 255, 255); // ボーダーカラー (Clamp時の境界色)
} SSOP;

// テクスチャステージステート管理構造体
typedef struct
{
    D3DTEXTUREOP colorOp = D3DTOP_MODULATE;                           // カラーオペレーション
    DWORD colorArg0 = D3DTA_CURRENT;                                  // オペレーションの引数0 (一部のオペレーション用)
    DWORD colorArg1 = D3DTA_TEXTURE;                                  // オペレーションの引数1
    DWORD colorArg2 = D3DTA_DIFFUSE;                                  // オペレーションの引数2
    D3DTEXTUREOP alphaOp = D3DTOP_MODULATE;                           // アルファオペレーション
    DWORD alphaArg0 = D3DTA_CURRENT;                                  // オペレーションの引数0 (一部のオペレーション用)
    DWORD alphaArg1 = D3DTA_TEXTURE;                                  // オペレーションの引数1
    DWORD alphaArg2 = D3DTA_DIFFUSE;                                  // オペレーションの引数2
    DWORD texCoordIndex = 0;                                          // テクスチャのインデックス
    D3DTEXTURETRANSFORMFLAGS texTureTransFormFlags = D3DTTFF_DISABLE; // テクスチャ変換
    DWORD resultArg = D3DTA_CURRENT;                                  // 結果の格納先
    D3DCOLOR constant = D3DCOLOR_ARGB(255, 255, 255, 255);            // 定数値 (D3DTA_CONSTANTを使用時)
} TSSOP;

// 型変換用の共用体
typedef union
{
	int Int;                // 整数
	float Float;            // 浮動小数点
	double Double;          // 倍精度浮動小数点
	long Long;              // 長整数
	short Short;            // 短整数
	unsigned int UInt;      // 符号なし整数
	unsigned long ULong;    // 符号なし長整数
	unsigned short UShort;  // 符号なし短整数
	unsigned char UChar[8]; // 符号なし文字
	signed char SChar[8];   // 符号付き文字
	wchar_t WChar[8];       // ワイド文字
	TCHAR TChar[8];         // テキスト文字
	char Char[8];           // 文字
	bool Bool;              // 真偽値
	DWORD Dword;            // DWORD
	WORD Word;              // WORD
	BYTE Byte[8];           // BYTE
}TYPE_CONVERSION;

//プロトタイプ宣言
HRESULT CreateVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, UINT VerrexSize, UINT VertexCount, DWORD Usage, D3DPOOL Pool);                                  // 2D頂点バッファの作成
HRESULT CreateVertex3D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, UINT VerrexSize, UINT VertexCount, DWORD Usage, D3DPOOL Pool);                                  // 3D頂点バッファの作成
HRESULT CreateTexture(LPDIRECT3DTEXTURE9* ppTextureBuff, const String* TexPass, int TextureCount, D3DXIMAGE_INFO* pImageInfo, DWORD Filter, D3DPOOL Pool); // テクスチャの作成
HRESULT SetVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, DWORD LockFlags, int OffsetIdx, int BuffMax, int yDivisions, int xDivisions, float Width, float Height, float rhw, D3DXCOLOR col, int TexCount, D3DXIMAGE_INFO* pImageInfo, bool PosSenter); // 2D頂点バッファの設定
HRESULT SetVertex3D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, DWORD LockFlags, int OffsetIdx, int BuffMax, int yDivisions, int xDivisions, float Width, float Height, D3DXCOLOR col, int TexCount, D3DXIMAGE_INFO* pImageInfo, bool PosSenter);            // 3D頂点バッファの設定
HRESULT SetIndex(LPDIRECT3DINDEXBUFFER9* ppIdxBuff, int yDivisions, int xDivisions);                                                                                                                                                                 // インデックスバッファの設定
HRESULT GetDrawOperation(RSOP* rsOp, SSOP* ssOp, unsigned int SamplerCount, TSSOP* tssOp, unsigned int TextureStateCount);                         // 描画情報の取得
HRESULT SetDrawOperation(RSOP* rsOp, SSOP* ssOp, unsigned int SamplerCount, TSSOP* tssOp, unsigned int TextureStateCount);                         // 描画情報の設定
HRESULT GetDrawOperationTemp(RSOP* rsOp, SSOP* ssOp, unsigned int SamplerCount, TSSOP* tssOp, unsigned int TextureStateCount, DRAW_TEMP DrawTemp); // 描画情報テンプレートの取得
#ifdef _STD_UNSECURE_USE // セキュリティー面などでおすすめしない関数を使用する場合
HRESULT SetDrawOperationTemp(unsigned int SamplerCount, unsigned int TextureStateCount, DRAW_TEMP DrawTemp);                                       // 描画情報テンプレートの設定
#endif // _STD_UNSECURE_USE
HRESULT GetRenderOperation(RSOP* rsOp);   // レンダー情報の取得
_NODISCARD RSOP GetRenderOperation(void); // レンダー情報の取得(引数)
HRESULT SetRenderOperation(RSOP* rsOp);   // レンダー情報の設定
HRESULT GetSamplerOperation(SSOP* ssOp, unsigned int SamplerCount); // サンプラー情報の取得
_NODISCARD SSOP* GetSamplerOperation(unsigned int SamplerCount);    // サンプラー情報の取得(引数)
HRESULT SetSamplerOperation(SSOP* ssOp, unsigned int SamplerCount); // サンプラー情報の設定
HRESULT GetTextureStageOperation(TSSOP* tssOp, unsigned int TextureStateCount); // テクスチャステージ情報の取得
_NODISCARD TSSOP* GetTextureStageOperation(unsigned int TextureStateCount);     // テクスチャステージ情報の取得(引数)
HRESULT SetTextureStageOperation(TSSOP* tssOp, unsigned int TextureStateCount); // テクスチャステージ情報の設定
HRESULT DrawVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, LPDIRECT3DINDEXBUFFER9* ppIdxBuff, LPDIRECT3DTEXTURE9* ppTextureBuff, D3DPRIMITIVETYPE PrimitiveType, UINT VertexSize, UINT OffsetIdx, UINT PrimitiveCount);                                                                                                                              // 2D頂点バッファの描画
HRESULT DrawVertex3D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff, LPDIRECT3DINDEXBUFFER9* ppIdxBuff, LPDIRECT3DTEXTURE9* ppTextureBuff, D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, D3DXVECTOR3* pScale, D3DXMATRIX* pMtxWorld, BILLBOARD_TYPE BillboardType, float zBias, D3DPRIMITIVETYPE PrimitiveType, UINT VertexSize, UINT OffsetIdx, UINT PrimitiveCount); // 3D頂点バッファの描画
#endif //_STDDX_H_