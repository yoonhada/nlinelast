#pragma once

#ifndef _NORMALLINE_H_
#define _NORMALLINE_H_

#include <list>

class NormalLine
{
private:
	VOID		Initialize();
	VOID		Release();

	HRESULT		CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF );
	HRESULT		SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size );

public:
	NormalLine( LPDIRECT3DDEVICE9 _pd3dDevice ) : m_pd3dDevice( _pd3dDevice )
	{
		this->Initialize();
	}
	~NormalLine()
	{
		this->Release();
	}

	VOID		Create( D3DCOLOR _Color, INT _iNumVertex, LPD3DXVECTOR3 _pPosition, LPD3DXVECTOR3 _pNormal );
	VOID		Render();

private:
	typedef struct _VERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

		D3DXVECTOR3	pos;
		D3DCOLOR	color;
	}VERTEX, *LPVERTEX;

	typedef struct _DATA
	{
		LPDIRECT3DVERTEXBUFFER9	pVB;
		INT						iNumLine;

		_DATA()
		{
			pVB = NULL;
		}
		~_DATA()
		{
			SAFE_RELEASE( pVB );
		}
	}DATA, *LPDATA;

	typedef std::list< LPDATA > LPDATALIST;

	LPDIRECT3DDEVICE9	m_pd3dDevice;
	
	LPDATALIST			m_pDataList;

public:

};

#endif