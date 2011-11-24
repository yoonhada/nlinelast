#pragma once
#ifndef _ASEVIEWERBASE_H_
#define _ASEVIEWERBASE_H_

class ASEViewerBase
{
private:
	VOID		Initialize();
	VOID		Release();
	
public:
	ASEViewerBase( LPDIRECT3DDEVICE9 _pd3dDevice ) : m_pd3dDevice( _pd3dDevice )
	{
		this->Initialize();
	}
	~ASEViewerBase()
	{
		this->Release();
	}

	typedef struct _DATA
	{
		TCHAR					aObjectName[ 512 ];
		TCHAR					aTextureFileName[ 512 ];
		INT						iNumVertex;
		INT						iNumIndex;
		LPDIRECT3DVERTEXBUFFER9	pVB;
		LPDIRECT3DINDEXBUFFER9	pIB;
		LPDIRECT3DTEXTURE9		pTex;

		_DATA()
		{
			pVB					= NULL;
			pIB					= NULL;
			pTex				= NULL;
		}
		~_DATA()
		{
			if( pVB != NULL )
				pVB->Release();
			if( pIB != NULL )
				pIB->Release();
			if( pTex != NULL )
				pTex->Release();
		}
	}DATA, *LPDATA;

	typedef struct _TILEINFO
	{
		INT*		pNavGraphNode;

		FLOAT		fTileSize;

		D3DXVECTOR3 vecStart;
		D3DXVECTOR3 vecEnd;

		_TILEINFO()
		{
			pNavGraphNode = NULL;
		}
		~_TILEINFO()
		{
			if( pNavGraphNode != NULL )
				delete[] pNavGraphNode;
		}
	}TILEINFO, *LPTILEINFO;


	HRESULT		CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF );
	HRESULT		CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size );
	HRESULT		SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size );
	HRESULT		SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size );
	HRESULT		LoadTextureFromFile( LPDIRECT3DTEXTURE9* _ppTexture, LPCWSTR FileName );

private:

public:
	LPDIRECT3DDEVICE9		m_pd3dDevice;

};

#endif