#pragma once
#ifndef _TILEMAPBASE_H_
#define _TILEMAPBASE_H_

class TileMapBase
{
private:
	VOID		Initialize();
	VOID		Release();

	HRESULT		CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF );
	HRESULT		SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size );

	HRESULT		CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size );
	HRESULT		SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size );

public:
	TileMapBase( LPDIRECT3DDEVICE9 _pd3dDevice ) : m_pd3dDevice( _pd3dDevice )
	{
		this->Initialize();
	}
	~TileMapBase()
	{
		this->Release();
	}

	typedef struct _TILEVERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };

		D3DXVECTOR3 vecPosition;
		D3DXVECTOR3 vecTexcoord;
	}TILEVERTEX, *LPTILEVERTEX;

	typedef struct _LINEVERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

		D3DXVECTOR3 vecPosition;
		DWORD		dColor;
	}LINEVERTEX, *LPLINEVERTEX;

	typedef struct _INDEX
	{
		DWORD _0, _1, _2;
	}INDEX, *LPINDEX;

	typedef struct _IMAGE
	{
		LPDIRECT3DVERTEXBUFFER9	pVB;
		INT						iVertices;
		LPDIRECT3DINDEXBUFFER9	pIB;
		INT						iIndices;

		LPDIRECT3DTEXTURE9		pTexture;

		DWORD					FVF;

		_IMAGE()
		{
			pVB			= NULL;
			pIB			= NULL;
			pTexture	= NULL;
		}
		~_IMAGE()
		{
			SAFE_RELEASE( pVB );
			SAFE_RELEASE( pIB );
			SAFE_RELEASE( pTexture );
		}
	}IMAGE, *LPIMAGE;

	VOID		CreateTileImage( IMAGE& _TileImage, INT _iVertices, LPTILEVERTEX _pVertex, INT _iIndices, LPINDEX _pIndex, INT _itexWidth, INT _itexHeight );
	VOID		CreateLineImage( IMAGE& _LineImage, INT _iVertices, LPLINEVERTEX _pVertex );
	
	VOID		RenderTileImage( LPIMAGE _pTileImage );
	VOID		RenderLineImage( LPIMAGE _pLineImage );

	VOID		ChangePixel( LPDIRECT3DTEXTURE9 _pTexture, INT _iX, INT _iY, DWORD _dColor );

private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;

public:

};

#endif