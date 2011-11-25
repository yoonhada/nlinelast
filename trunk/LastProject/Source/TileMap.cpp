#include "stdafx.h"
#include "TileMap.h"
#include "BBXParser.h"

VOID TileMap::Initialize()
{
	m_pBBXParser = NULL;
	m_pBBXParser = new BBXParser;
}

VOID TileMap::Release()
{
	SAFE_DELETE( m_pBBXParser );
}

VOID TileMap::InitInfo( LPD3DXVECTOR3 _pvecStart, LPD3DXVECTOR3 _pvecEnd, FLOAT _fTileSize )
{
	//	Init Info
	m_Data.Info.iTileWidth		= static_cast<INT>( ( _pvecEnd->x - _pvecStart->x ) / _fTileSize + 1.0f );
	m_Data.Info.iTileHeight		= static_cast<INT>( ( _pvecEnd->z - _pvecStart->z ) / _fTileSize + 1.0f );
	//	타일이랑 그래프 넓이 높이를 햇갈리지 말자
	m_Data.Info.iGraphWidth		= m_Data.Info.iTileWidth - 1;
	m_Data.Info.iGraphHeight	= m_Data.Info.iTileHeight - 1;

	m_Data.Info.fTileSize		= _fTileSize;

	m_Data.Info.vecStart		= (*_pvecStart);
	m_Data.Info.vecEnd			= (*_pvecEnd);

	m_Data.Info.pNavGraphNode = new INT[ m_Data.Info.iGraphWidth * m_Data.Info.iGraphHeight ];
	ZeroMemory( m_Data.Info.pNavGraphNode, sizeof( INT ) * m_Data.Info.iGraphWidth * m_Data.Info.iGraphHeight );
}
VOID TileMap::InitTileImage()
{
	//	Init TileImage
	//	Set Vertex
	m_Data.imgTile.iVertices = m_Data.Info.iTileWidth * m_Data.Info.iTileHeight;

	LPTILEVERTEX pTileVertex = new TILEVERTEX[ m_Data.imgTile.iVertices ];

	FLOAT incU = static_cast<FLOAT>( 1.0f / m_Data.Info.iTileWidth );
	FLOAT incV = static_cast<FLOAT>( 1.0f / m_Data.Info.iTileHeight );

	INT l = 0;
	for( INT z=0 ; z<m_Data.Info.iTileHeight ; z++ )
	{
		for( INT x=0 ; x<m_Data.Info.iTileWidth ; x++ )
		{
			pTileVertex[ l ].vecPosition.x = m_Data.Info.vecStart.x + m_Data.Info.fTileSize * static_cast<FLOAT>( x );
			pTileVertex[ l ].vecPosition.y = 0.0f;
			pTileVertex[ l ].vecPosition.z = m_Data.Info.vecStart.z + m_Data.Info.fTileSize * static_cast<FLOAT>( z );

			pTileVertex[ l ].vecTexcoord.x = incU * static_cast<FLOAT>( x );
			pTileVertex[ l ].vecTexcoord.y = incV * static_cast<FLOAT>( z );

			l++;
		}
	}
	//	Set Index
	m_Data.imgTile.iIndices = ( m_Data.Info.iTileWidth - 1 ) * ( m_Data.Info.iTileHeight - 1 ) * 2;

	LPINDEX pTileIndex = new INDEX[ m_Data.imgTile.iIndices ];

	l = 0;
	for( INT z=0 ; z<m_Data.Info.iTileHeight-1 ; z++ )
	{
		for( INT x=0 ; x<m_Data.Info.iTileWidth-1 ; x++ )
		{
			pTileIndex[ l     ]._0 =	x         + z         * m_Data.Info.iTileWidth;
			pTileIndex[ l     ]._1 =	x         + ( z + 1 ) * m_Data.Info.iTileWidth;
			pTileIndex[ l     ]._2 =	( x + 1 ) + ( z + 1 ) * m_Data.Info.iTileWidth;

			pTileIndex[ l + 1 ]._0 =	( x + 1 ) + z         * m_Data.Info.iTileWidth;
			pTileIndex[ l + 1 ]._1 =	x         + z         * m_Data.Info.iTileWidth;
			pTileIndex[ l + 1 ]._2 =	( x + 1 ) + ( z + 1 ) * m_Data.Info.iTileWidth;

			l+=2;
		}
	}

	CreateTileImage( m_Data.imgTile,	m_Data.imgTile.iVertices,
										pTileVertex,
										m_Data.imgTile.iIndices,
										pTileIndex,
										m_Data.Info.iTileWidth,
										m_Data.Info.iTileHeight );

	SAFE_DELETE_ARRAY( pTileVertex );
	SAFE_DELETE_ARRAY( pTileIndex );

}
VOID TileMap::InitLineImage()
{
	//	Init LineImage
	m_Data.imgLine.iVertices = ( m_Data.Info.iTileWidth + m_Data.Info.iTileHeight ) * 2;

	LPLINEVERTEX pLineVertex = new LINEVERTEX[ m_Data.imgLine.iVertices ];

	INT l = 0;
	//	Set Width
	for( INT i=0 ; i<m_Data.Info.iTileHeight ; i++ )
	{
		pLineVertex[ l     ].vecPosition.x	= m_Data.Info.vecStart.x;
		pLineVertex[ l     ].vecPosition.y	= 0.5f;
		pLineVertex[ l     ].vecPosition.z	= m_Data.Info.vecStart.z + ( i * m_Data.Info.fTileSize );

		pLineVertex[ l     ].dColor			= 0xff0000ff;

		pLineVertex[ l + 1 ].vecPosition.x	= m_Data.Info.vecStart.x + static_cast<FLOAT>( m_Data.Info.iTileWidth - 1 ) * m_Data.Info.fTileSize;
		pLineVertex[ l + 1 ].vecPosition.y	= 0.5f;
		pLineVertex[ l + 1 ].vecPosition.z	= m_Data.Info.vecStart.z + static_cast<FLOAT>( i ) * m_Data.Info.fTileSize;

		pLineVertex[ l + 1 ].dColor			= 0xff0000ff;

		l+=2;
	}

	//	Set Height
	for( INT i=0 ; i<m_Data.Info.iTileWidth ; i++ )
	{
		pLineVertex[ l     ].vecPosition.x	= m_Data.Info.vecStart.x + static_cast<FLOAT>( i ) * m_Data.Info.fTileSize;
		pLineVertex[ l     ].vecPosition.y	= 0.5f;
		pLineVertex[ l     ].vecPosition.z	= m_Data.Info.vecStart.z;

		pLineVertex[ l     ].dColor			= 0xff0000ff;

		pLineVertex[ l + 1 ].vecPosition.x	= m_Data.Info.vecStart.x + static_cast<FLOAT>( i ) * m_Data.Info.fTileSize;
		pLineVertex[ l + 1 ].vecPosition.y	= 0.5f;
		pLineVertex[ l + 1 ].vecPosition.z	= m_Data.Info.vecStart.z + static_cast<FLOAT>( m_Data.Info.iTileHeight - 1 ) * m_Data.Info.fTileSize;

		pLineVertex[ l + 1 ].dColor			= 0xff0000ff;

		l+=2;
	}

	CreateLineImage( m_Data.imgLine, m_Data.imgLine.iVertices, pLineVertex );

	SAFE_DELETE_ARRAY( pLineVertex );
}

VOID TileMap::Create( D3DXVECTOR3 _vecStart, D3DXVECTOR3 _vecEnd, FLOAT _fTileSize )
{
	InitInfo( &_vecStart, &_vecEnd, _fTileSize );
	InitTileImage();
	InitLineImage();
}

VOID TileMap::LoadBBXFile( LPWSTR _pFileName )
{
	m_pBBXParser->LoadFile( _pFileName );

	INT iNumBBXData = m_pBBXParser->GetNumBoundBox();

	for( INT i=0 ; i<iNumBBXData ; i++ )
	{ 
		BBXParser::DATA BBXData;
		m_pBBXParser->GetData( i, BBXData );
		
		SetBBXData( &BBXData.Info.vPivot, BBXData.Info.fMinusSize, BBXData.Info.fPlusSize );
	}

}

VOID TileMap::Update()
{
}

VOID TileMap::Render()
{
	RenderTileImage( &m_Data.imgTile );
	RenderLineImage( &m_Data.imgLine );
}

BOOL TileMap::SetInfo( DWORD _dType, INT _iX, INT _iY )
{
	if( 0 > _iX || _iX >= m_Data.Info.iGraphWidth )return FALSE;
	if( 0 > _iY || _iY >= m_Data.Info.iGraphHeight )return FALSE;

	switch( _dType )
	{
	case TLM_WAY:
		ChangePixel( m_Data.imgTile.pTexture, _iX, _iY, 0xffffffff );
		m_Data.Info.pNavGraphNode[ _iX + _iY * m_Data.Info.iGraphWidth ] = 0;
		break;
	case TLM_WALL:
		ChangePixel( m_Data.imgTile.pTexture, _iX, _iY, 0xff000000 );
		m_Data.Info.pNavGraphNode[ _iX + _iY * m_Data.Info.iGraphWidth ] = 1;
		break;
	case TLM_COURSE:
		ChangePixel( m_Data.imgTile.pTexture, _iX, _iY, 0xff00ff00 );
		break;
	}

	return TRUE;

}

BOOL TileMap::SetInfo( DWORD _dType, FLOAT _fX, FLOAT _fZ )
{
	INT iX = static_cast<INT>( ( _fX - m_Data.Info.vecStart.x ) / m_Data.Info.fTileSize );
	INT iY = static_cast<INT>( ( _fZ - m_Data.Info.vecStart.z ) / m_Data.Info.fTileSize );

	if( 0 > iX || iX >= m_Data.Info.iGraphWidth )return FALSE;
	if( 0 > iY || iY >= m_Data.Info.iGraphHeight )return FALSE;

	switch( _dType )
	{
	case TLM_WAY:
		ChangePixel( m_Data.imgTile.pTexture, iX, iY, 0xffffffff );
		m_Data.Info.pNavGraphNode[ iX + iY * m_Data.Info.iGraphWidth ] = 0;
		break;
	case TLM_WALL:
		ChangePixel( m_Data.imgTile.pTexture, iX, iY, 0xff000000 );
		m_Data.Info.pNavGraphNode[ iX + iY * m_Data.Info.iGraphWidth ] = 1;
		break;
	case TLM_COURSE:
		ChangePixel( m_Data.imgTile.pTexture, iX, iY, 0xff00ff00 );
		break;
	}

	return TRUE;
}

BOOL TileMap::SetBBXData( LPD3DXVECTOR3 _pvecPivot, FLOAT* _pfMinus, FLOAT* _pfPlus )
{
	FLOAT fStartX = _pvecPivot->x + _pfMinus[ 0 ];
	FLOAT fStartZ = _pvecPivot->z + _pfMinus[ 2 ];

	INT iBBXWidth	= static_cast<INT>( ( _pfPlus[ 0 ] - _pfMinus[ 0 ] ) / m_Data.Info.fTileSize );
	INT iBBXHeight	= static_cast<INT>( ( _pfPlus[ 2 ] - _pfMinus[ 2 ] ) / m_Data.Info.fTileSize );

	FLOAT fX = 0.0f;
	FLOAT fZ = 0.0f;

	for( INT y=0 ; y<=iBBXHeight ; y++ )
	{
		for( INT x=0 ; x<=iBBXWidth ; x++ )
		{
			SetInfo( TLM_WALL,	fStartX + fX * m_Data.Info.fTileSize,
								fStartZ + fZ * m_Data.Info.fTileSize );

			fX += 1.0f;
		}
		fZ += 1.0f;
		fX = 0.0f;
	}

	return TRUE;
}