#pragma once
#ifndef _TILEMAP_H_
#define _TILEMAP_H_

#include "TileMapBase.h"

class BBXParser;

class TileMap : public TileMapBase
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		InitInfo( LPD3DXVECTOR3 _vecStart, LPD3DXVECTOR3 _vecEnd, FLOAT _fTileSize );
	VOID		InitTileImage();
	VOID		InitLineImage();

public:
	enum { TLM_WAY = 0, TLM_WALL, TLM_COURSE };

	typedef struct _INFO
	{
		INT*		pNavGraphNode;
		
		INT			iGraphWidth;
		INT			iGraphHeight;

		INT			iTileWidth;
		INT			iTileHeight;

		FLOAT		fTileSize;

		D3DXVECTOR3	vecStart;
		D3DXVECTOR3	vecEnd;

		_INFO()
		{
			pNavGraphNode = NULL;
		}
		~_INFO()
		{
			SAFE_DELETE_ARRAY( pNavGraphNode );
		}
	}INFO, *LPINFO;

	TileMap( LPDIRECT3DDEVICE9 _pd3dDevice ) : TileMapBase( _pd3dDevice )
	{
		this->Initialize();
	}
	virtual ~TileMap()
	{
		this->Release();
	}

	VOID		Create( D3DXVECTOR3 _vecStart, D3DXVECTOR3 _vecEnd, FLOAT _fTileSize );
	VOID		Update();
	VOID		Render();

	BOOL		SetInfo( DWORD _dType, INT _iX, INT _iY );
	BOOL		SetInfo( DWORD _dType, FLOAT _fX, FLOAT _fZ );
	BOOL		SetBBXData( LPD3DXVECTOR3 _pvecPivot, FLOAT* _pfMinus, FLOAT* _pfPlus );
	
	VOID		LoadBBXFile( LPWSTR _pFileName );

private:
	typedef struct _DATA
	{
		IMAGE	imgTile;
		IMAGE	imgLine;
		INFO	Info;
	}DATA, *LPDATA;

	DATA			m_Data;

	BBXParser*		m_pBBXParser;

public:
	LPINFO		GetInfo(){ return &m_Data.Info; };

};

#endif