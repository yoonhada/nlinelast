#pragma once
#ifndef _GUIBASE_H_
#define _GUIBASE_H_

//	Animation test
#include <vector>
//	End

class GUIBase
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		SetMatrix( D3DXVECTOR3& _vecScale, D3DXVECTOR3& _vecRotate, D3DXVECTOR3& _vecTranslate );

	HRESULT		CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF );
	HRESULT		SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size );

	HRESULT		CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size );
	HRESULT		SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size );

	HRESULT		LoadTextureFromFile(	LPDIRECT3DTEXTURE9* _ppOutTexture, 
										LPCWSTR FileName, 
										UINT iWidth	 = D3DX_DEFAULT_NONPOW2, 
										UINT iHeight = D3DX_DEFAULT_NONPOW2, 
										D3DCOLOR colClear = NULL );

public:
	enum { GBS_TOPLEFT = 0, GBS_CENTER };

	//	struct Vertex
	typedef struct _VERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };

		D3DXVECTOR3	vecPosition;
		D3DXVECTOR2	vecTexcoord;
	}VERTEX, *LPVERTEX;

	//	struct Index
	typedef struct _INDEX
	{
		DWORD _0, _1, _2;
	}INDEX, *LPINDEX;

	//	struct Texture
	typedef struct _TEXTURE
	{
		LPDIRECT3DTEXTURE9	pTex;
		INT					iFrameSpeed;
		
		_TEXTURE()
		{	//	iFrameSpeed = 1000;
			pTex = NULL;
		}
		~_TEXTURE()
		{
			if( pTex != NULL )
				pTex->Release();
			pTex = NULL;
		}
	}TEXTURE, *LPTEXTURE;

	//	struct Image 
	//	IMAGE -> IMAGE3D로 수정
	typedef std::vector< LPTEXTURE >	 TEXTUREVECTOR;
	typedef std::vector< TEXTUREVECTOR > TEXTUREVECTOR2;

	typedef struct _IMAGE
	{
		LPDIRECT3DVERTEXBUFFER9	pVB;
		INT						iNumVertices;	//	4로 고정인데 있을 필요가 있나?
		LPDIRECT3DINDEXBUFFER9	pIB;
		INT						iNumIndices;	//	2로 고정인데 있을 필요가 있나?
		
		TEXTUREVECTOR2			vec2Tex;	
		BOOL					bAniPlay;
		DWORD					dCurrentTime;
		DWORD					dBeginTime;
		INT						iCurrentFrame;
		INT						iCurrentAnimation;

		D3DXVECTOR3				vecScale;
		D3DXVECTOR3				vecRotate;
		D3DXVECTOR3				vecTrans;
		
		_IMAGE()
		{
			pVB		= NULL;
			pIB		= NULL;
			
			bAniPlay			= FALSE;
			dCurrentTime		= 0;
			dBeginTime			= 0;
			iCurrentFrame		= 0;
			iCurrentAnimation	= 0;

			vecScale	= D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
			vecRotate	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			vecTrans	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
		~_IMAGE()
		{
			if( pVB != NULL )
				pVB->Release();
			if( pIB != NULL )
				pIB->Release();
			
			TEXTUREVECTOR2::iterator itE2;
			for( itE2=vec2Tex.begin() ; itE2!=vec2Tex.end() ; itE2++ )
			{
				TEXTUREVECTOR::iterator itE;
				for( itE=itE2->begin() ; itE!=itE2->end() ; itE++ )
					delete (*itE);
			}

			pVB		= NULL;
			pIB		= NULL;
		}
	}IMAGE, *LPIMAGE;

	//	struct Image2D
	typedef struct _IMAGE2D
	{
		RECT				rtSource;
		D3DXVECTOR3			vecCenter;
		D3DXVECTOR3			vecPosition;

		TEXTUREVECTOR2		vec2Tex;	
		BOOL				bAniPlay;
		DWORD				dCurrentTime;
		DWORD				dBeginTime;
		INT					iCurrentFrame;
		INT					iCurrentAnimation;
	
		_IMAGE2D()
		{
			bAniPlay			= FALSE;
			dCurrentTime		= 0;
			dBeginTime			= 0;
			iCurrentFrame		= 0;
			iCurrentAnimation	= 0;
		}
		~_IMAGE2D()
		{
			TEXTUREVECTOR2::iterator itE2;
			for( itE2=vec2Tex.begin() ; itE2!=vec2Tex.end() ; itE2++ )
			{
				TEXTUREVECTOR::iterator itE;
				for( itE=itE2->begin() ; itE!=itE2->end() ; itE++ )
					delete (*itE);
			}
		}
	}IMAGE2D, *LPIMAGE2D;

	
	//	struct Param
	typedef std::vector< LPWSTR >		TCHARVECTOR;
	typedef std::vector< TCHARVECTOR >	TCHARVECTOR2;

	typedef struct _IMAGEPARAM
	{
		DWORD			dPivotType;
		FLOAT			fX, fY;
		FLOAT			fWidth, fHeight;

		TCHARVECTOR2	vec2FileName;

		_IMAGEPARAM()
		{
			dPivotType = GBS_TOPLEFT;
		}
		~_IMAGEPARAM()
		{
			TCHARVECTOR2::iterator itE2;
			for( itE2=vec2FileName.begin() ; itE2!=vec2FileName.end() ; itE2++ )
			{
				TCHARVECTOR::iterator itE;
				for( itE=itE2->begin() ; itE!=itE2->end() ; itE++ )
					delete (*itE);
			}
		}
	}IMAGEPARAM, *LPIMAGEPARAM;

	GUIBase( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : m_pd3dDevice( _pd3dDevice ), m_pSprite( _pSprite )
	{
		this->Initialize();
	}
	virtual ~GUIBase()
	{
		this->Release();
	}

	//	CreateImage -> CreateImage3D
	VOID		CreateImage(	IMAGE& _Image,					//	Load Texture
								DWORD _dPivotType,
								FLOAT _fX, FLOAT _fY, 
								FLOAT _fWidth, FLOAT _fHeight,	//	_pFileName == NULL -> CreateTexture
								LPCWSTR _pFileName = NULL );

	//	참조형 말고 포인트형으로 받자 
	//	통일통일~ 우리의 소원은 통일~
	VOID		CreateImage( IMAGE& _Image, IMAGEPARAM& _imgParam );	//	Load Texture
	VOID		CreateImage2D( IMAGE2D& _Image2D, IMAGEPARAM& _imgParam );

	VOID		RenderImage( IMAGE& _Image );
	VOID		RenderImage2D( IMAGE2D& _Image2D );

	VOID		ImageScale( IMAGE& _Image, FLOAT _fX, FLOAT _fY, FLOAT _fZ );
	VOID		ImageTranslate( IMAGE& _Image, FLOAT _fX, FLOAT _fY, FLOAT _fZ );

	VOID		AddFileName( INT _iNumAni, IMAGEPARAM& _imgParam, LPWSTR _pFileName );
	VOID		AddTexture( INT _iNumAni, IMAGE& _Image, LPWSTR _pFileName );
	VOID		AddTexture( INT _iNumAni, IMAGE2D& _Image2D, LPWSTR _pFileName );

	//	DrawFontOnTexture
	VOID		SetFont( LPWSTR _FontType, INT _iSize, INT _iWeight );
	VOID		DrawFontOnTexture( LPWSTR _Str, COLORREF _Color, LPDIRECT3DTEXTURE9 _pTexture, INT _iWidth, INT _iHeight );
	//	End
private:
	//	DrawFontOnTexture
	HFONT				m_hFont;
	COLORREF			m_colFont;
	//	End

	//	Animation Test
	DWORD				m_dBeginTime;
	DWORD				m_dCurrentTime;

	//	End
	INT					m_iFrameSpeed;

public:
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	LPD3DXSPRITE		m_pSprite;
	
	
};

#endif