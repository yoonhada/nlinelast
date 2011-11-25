#pragma once
#ifndef _TILEMAP_H_
#define _TILEMAP_H_

#include "TileMapBase.h"

class TileMap : public TileMapBase
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		InitInfo( D3DXVECTOR3& _vecStart, D3DXVECTOR3& _vecEnd, FLOAT _fTileSize );
	VOID		InitTileData( D3DXVECTOR3& _vecStart, D3DXVECTOR3& _vecEnd, FLOAT _fTileSize );
	VOID		InitLineData( D3DXVECTOR3& _vecStart, D3DXVECTOR3& _vecEnd, FLOAT _fTileSize );

public:
	enum { TLM_WAY = 0, TLM_WALL, TLM_COURSE };

	TileMap( LPDIRECT3DDEVICE9 _pd3dDevice ) : TileMapBase( _pd3dDevice )
	{
		this->Initialize();
	}
	~TileMap()
	{
		this->Release();
	}

	VOID		Create( D3DXVECTOR3 _vecStart, D3DXVECTOR3 _vecEnd, FLOAT _fTileSize );
	VOID		Update();
	VOID		Render();

	VOID		SetBBXData( LPD3DXVECTOR3 _pvecPivot, FLOAT* _pfMinus, FLOAT* _pfPlus );
	BOOL		SetInfo( INT _iX, INT _iY, DWORD _dType );
	BOOL		SetInfo( FLOAT _fX, FLOAT _fZ, DWORD _dType );

private:
	typedef struct _TILEDATA
	{
		LPTILEVERTEX	pVertex;
		INT				iNumVertex;	//	¹ö¸±±î?
		LPINDEX			pIndex;
		INT				iNumIndex;

		FLOAT			fTileSize;

		IMAGE			Image;
		_TILEDATA()
		{
			pVertex = NULL;
			pIndex	= NULL;
		}
		~_TILEDATA()
		{
			SAFE_DELETE_ARRAY( pVertex );
			SAFE_DELETE_ARRAY( pIndex );
		}
	}TILEDATA, *LPTILEDATA;

	typedef struct _LINEDATA
	{
		LPLINEVERTEX	pVertex;
		INT				iNumVertex;

		IMAGE			Image;
		
		_LINEDATA()
		{
			pVertex = NULL;
		}
		~_LINEDATA()
		{
			SAFE_DELETE_ARRAY( pVertex );
		}
	}LINEDATA, *LPLINEDATA;

	typedef struct _INFO
	{
		INT*		pNavGraphNode;
		INT			iMaxWidth;
		INT			iMaxHeight;
		FLOAT		fTileSize;
		
		D3DXVECTOR3 vecStart;
		D3DXVECTOR3 vecEnd;

		_INFO()
		{
			pNavGraphNode = NULL;
		}
		~_INFO()
		{
			SAFE_DELETE_ARRAY( pNavGraphNode );
		}
	}INFO, *LPINFO;
		
	TILEDATA			m_TileData;
	LINEDATA			m_LineData;
	INFO				m_Info;
	
public:
	INFO*	GetMapInfo()	{ return &m_Info; }

};

#endif