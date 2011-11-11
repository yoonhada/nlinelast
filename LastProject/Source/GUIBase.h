#pragma once
#ifndef _GUIBASE_H_
#define _GUIBASE_H_

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

	HRESULT		LoadTextureFromFile(	LPDIRECT3DTEXTURE9* _ppTexture,		//	원본크기를 받아온다
										LPCWSTR FileName, 
										D3DCOLOR colClear = NULL );

	HRESULT		LoadTextureFromFile(	LPDIRECT3DTEXTURE9* _ppOutTexture, 
										LPCWSTR FileName, 
										UINT iWidth, 
										UINT iHeight, 
										D3DCOLOR colClear = NULL );

public:
	typedef struct _VERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };

		D3DXVECTOR3	vecPosition;
		D3DXVECTOR2	vecTexcoord;
	}VERTEX, *LPVERTEX;

	typedef struct _INDEX
	{
		DWORD _0, _1, _2;
	}INDEX, *LPINDEX;

	typedef struct _IMAGE
	{
		LPDIRECT3DVERTEXBUFFER9	pVB;
		INT						iNumVertices;
		LPDIRECT3DINDEXBUFFER9	pIB;
		INT						iNumIndices;
		
		LPDIRECT3DTEXTURE9		pTex;		

		D3DXVECTOR3				vecScale;
		D3DXVECTOR3				vecRotate;
		D3DXVECTOR3				vecTrans;
		
		_IMAGE()
		{
			pVB		= NULL;
			pIB		= NULL;
			pTex	= NULL;

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
			if( pTex != NULL )
				pTex->Release();

			pVB		= NULL;
			pIB		= NULL;
			pTex	= NULL;
		}
	}IMAGE, *LPIMAGE;

	GUIBase( LPDIRECT3DDEVICE9 _pd3dDevice ) : m_pd3dDevice( _pd3dDevice )
	{
		this->Initialize();
	}
	virtual ~GUIBase()
	{
		this->Release();
	}

	VOID		CreateImage(	IMAGE& _Image,					//	Load Texture
								FLOAT _fX, FLOAT _fY, 
								FLOAT _fWidth, FLOAT _fHeight, 
								LPCWSTR _pFileName );
	VOID		CreateImage(	IMAGE& _Image,					//	Create Texture
								FLOAT _fX, FLOAT _fY, 
								FLOAT _fWidth, FLOAT _fHeight );
	
	VOID		RenderImage( IMAGE& _Image );

	VOID		ImageScale( IMAGE& _Image, FLOAT _fX, FLOAT _fY, FLOAT _fZ );
	VOID		ImageTranslate( IMAGE& _Image, FLOAT _fX, FLOAT _fY, FLOAT _fZ );

	//	DrawFontOnTexture
	VOID		SetFont( LPWSTR _FontType, INT _iSize, INT _iWeight );
	VOID		DrawFontOnTexture( LPWSTR _Str, COLORREF _Color, LPDIRECT3DTEXTURE9 _pTexture, INT _iWidth, INT _iHeight );
	//	End
private:
	//	DrawFontOnTexture
	HFONT				m_hFont;
	COLORREF			m_colFont;
	//	End

public:
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	
	
};

#endif