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

	VOID		InitInfo( D3DXVECTOR3& _vecStart, D3DXVECTOR3& _vecEnd, FLOAT _fTileSize );
	VOID		InitTileData( D3DXVECTOR3& _vecStart, D3DXVECTOR3& _vecEnd, FLOAT _fTileSize );
	VOID		InitLineData( D3DXVECTOR3& _vecStart, D3DXVECTOR3& _vecEnd, FLOAT _fTileSize );

	//	짜증나...
	VOID		InitBBX( LPWSTR _BBXFileName );
	VOID		CreateDataFromBBX( INT _Index );
	VOID		CreateWallFromBBX( D3DXVECTOR3& _vecPivot, FLOAT* _pfMinus, FLOAT* _pfPlus );

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

	VOID		Create( LPWSTR _BBXFileName, D3DXVECTOR3 _vecStart, D3DXVECTOR3 _vecEnd, FLOAT _fTileSize );
	VOID		Update();
	VOID		Render();

	BOOL		SetInfo( INT _iX, INT _iY, DWORD _dType );
	BOOL		SetInfo( FLOAT _fX, FLOAT _fY, DWORD _dType );

	VOID		GetNavGraphNode( INT* _pOut );

private:
	typedef struct _TILEDATA
	{
		LPTILEVERTEX	pVertex;
		INT				iNumVertex;	//	버릴까?
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
			if( pVertex != NULL )
				delete[] pVertex;
			if( pIndex != NULL )
				delete[] pIndex;

			pVertex = NULL;
			pIndex	= NULL;
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
			if( pVertex != NULL )
				delete[] pVertex;
			
			pVertex = NULL;
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
			if( pNavGraphNode != NULL )
				delete[] pNavGraphNode;
		}
	}INFO, *LPINFO;
		
	BBXParser*			m_pBBXParser;

	TILEDATA			m_TileData;
	LINEDATA			m_LineData;
	INFO				m_Info;

	INT					m_iNumBBXImage;
	
public:

};

#endif