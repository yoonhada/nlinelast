#pragma once

#ifndef _MAP_H_
#define _MAP_H_

class ASEParser;
class BBXParser;

class Map : public CObjectSRT
{
private:
	VOID		Initialize();
	
	VOID		InitASE( LPWSTR _ASEFileName );
	VOID		InitBBX( LPWSTR _BBXFileName );
	VOID		CreateDataFromASE( const INT _Index );
	VOID		CreateDataFromBBX( const INT _Index );
	VOID		RenderASEData( INT _Index );
	VOID		RenderASEData( INT _Index, D3DXMATRIX _matCharacter );
	VOID		RenderBBXData( INT _Index );

	HRESULT		CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF );
	HRESULT		CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size );
	HRESULT		SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size );
	HRESULT		SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size );
	HRESULT		LoadTextureFromFile( LPDIRECT3DTEXTURE9* _ppTexture, LPCWSTR FileName );

public:
	Map( LPDIRECT3DDEVICE9	_pd3dDevice ) : m_pd3dDevice( _pd3dDevice )
	{
		this->Initialize();
	}
	~Map()
	{
		this->Release();
	}

	VOID		Create( LPWSTR _ASEFileName, LPWSTR _BBXFileName );
	VOID		Update();
	VOID		Render();
	VOID		Render( D3DXMATRIX& _matCharacter );
	VOID		Release();

public:
	BOOL		AddAnimationData( const DWORD _dType, DWORD _dID, INT _iStartFrame, INT _iEndFrame, BOOL _bLoop );
	BOOL		AddAnimationData2( const DWORD _dType, DWORD _dID, INT _iStartFrame, INT _iEndFrame, BOOL _bLoop );
	BOOL		SetAnimation( DWORD _dID );

private:
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

	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPDATA					m_pASEData;
	LPDATA					m_pBBXData;
	INT						m_iNumASEData;
	INT						m_iNumBBXData;

	ASEParser*				m_pASEParser;
	BBXParser*				m_pBBXParser;
	std::vector<CBoundBox*> m_pBoundBoxVector;
	
public:

};

#endif