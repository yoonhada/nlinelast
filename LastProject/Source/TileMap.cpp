#include "stdafx.h"
#include "TileMap.h"
#include "BBXParser.h"

VOID TileMap::Initialize()
{
}

VOID TileMap::Release()
{
}

VOID TileMap::InitInfo( D3DXVECTOR3& _vecStart, D3DXVECTOR3& _vecEnd, FLOAT _fTileSize )
{
	INT		iNumWidth	= static_cast<INT>( ( _vecEnd.x - _vecStart.x ) / _fTileSize + 1.0f );
	INT		iNumHeight	= static_cast<INT>( ( _vecEnd.z - _vecStart.z ) / _fTileSize + 1.0f );
	
	FLOAT	fNumWidth	= static_cast<FLOAT>( iNumWidth );
	FLOAT	fNumHeight	= static_cast<FLOAT>( iNumHeight );

	//	Set Info
	m_Info.iMaxWidth	= iNumWidth - 1;
	m_Info.iMaxHeight	= iNumHeight - 1;

	m_Info.fTileSize	= _fTileSize;

	m_Info.vecStart		= _vecStart;
	m_Info.vecEnd		= _vecEnd;
	//m_Info.vecEnd.x		= _vecStart.x + fNumWidth * _fTileSize;
	//m_Info.vecEnd.y		= _vecStart.y;
	//m_Info.vecEnd.z		= _vecStart.z + fNumHeight * _fTileSize;

	m_Info.pNavGraphNode = new INT[ m_Info.iMaxWidth * m_Info.iMaxHeight ];
	memset( m_Info.pNavGraphNode, 0, sizeof( INT ) *  m_Info.iMaxWidth * m_Info.iMaxHeight );
}
VOID TileMap::InitTileData( D3DXVECTOR3& _vecStart, D3DXVECTOR3& _vecEnd, FLOAT _fTileSize )
{
	INT		iNumWidth	= static_cast<INT>( ( _vecEnd.x - _vecStart.x ) / _fTileSize + 1.0f );
	INT		iNumHeight	= static_cast<INT>( ( _vecEnd.z - _vecStart.z ) / _fTileSize + 1.0f );
	
	FLOAT	fNumWidth	= static_cast<FLOAT>( iNumWidth );
	FLOAT	fNumHeight	= static_cast<FLOAT>( iNumHeight );

	//	Set TileData
	//	Set Vertex
	m_TileData.iNumVertex	= iNumWidth * iNumHeight;

	SAFE_DELETE_ARRAY( m_TileData.pVertex );
	m_TileData.pVertex		= new TILEVERTEX[ m_TileData.iNumVertex ];

	FLOAT incU = 1.0f / ( fNumWidth - 1.0f );
	FLOAT incV = 1.0f / ( fNumHeight - 1.0f );

	INT l = 0;
	for( INT z=0 ; z<iNumHeight ; z++ )
	{
		for( INT x=0 ; x<iNumWidth ; x++ )
		{
			m_TileData.pVertex[ l ].vecPosition.x = _vecStart.x + _fTileSize * static_cast<FLOAT>( x );
			m_TileData.pVertex[ l ].vecPosition.y = 0.0f;
			m_TileData.pVertex[ l ].vecPosition.z = _vecStart.z + _fTileSize * static_cast<FLOAT>( z );

			m_TileData.pVertex[ l ].vecTexcoord.x = incU * static_cast<FLOAT>( x );
			m_TileData.pVertex[ l ].vecTexcoord.y = incV * static_cast<FLOAT>( z );

			l++;
		}
	}

	//	Set Index
	m_TileData.iNumIndex	= ( iNumWidth - 1 ) * ( iNumHeight - 1 ) * 2;

	SAFE_DELETE_ARRAY( m_TileData.pIndex );
	m_TileData.pIndex = new INDEX[ m_TileData.iNumIndex ];

	l = 0;
	for( INT i=0 ; i<iNumHeight-1 ; i++ )
	{
		for( INT j=0 ; j<iNumWidth-1 ; j++ )
		{
			m_TileData.pIndex[ l		]._0	=	i			* ( iNumWidth  ) + j;
			m_TileData.pIndex[ l		]._1	=	( i + 1 )	* ( iNumWidth  ) + j;
			m_TileData.pIndex[ l		]._2	=	( i + 1 )	* ( iNumWidth  ) + ( j + 1 );
						 
			m_TileData.pIndex[ l + 1	]._0	=	i			* ( iNumWidth  ) + ( j + 1 );
			m_TileData.pIndex[ l + 1	]._1	=	i			* ( iNumWidth  ) + j;
			m_TileData.pIndex[ l + 1	]._2	=	( i + 1 )	* ( iNumWidth  ) + ( j + 1 );

			l+=2;
		}
	}

	//	Set VertexBuffer, IndexBuffer, Texture
	CreateTileImage( m_TileData.Image, m_TileData.iNumVertex, m_TileData.pVertex, m_TileData.iNumIndex, m_TileData.pIndex, iNumWidth, iNumHeight );


}
VOID TileMap::InitLineData( D3DXVECTOR3& _vecStart, D3DXVECTOR3& _vecEnd, FLOAT _fTileSize )
{
	INT		iNumWidth	= static_cast<INT>( ( _vecEnd.x - _vecStart.x ) / _fTileSize + 1.0f );
	INT		iNumHeight	= static_cast<INT>( ( _vecEnd.z - _vecStart.z ) / _fTileSize + 1.0f );
	
	FLOAT	fNumWidth	= static_cast<FLOAT>( iNumWidth );
	FLOAT	fNumHeight	= static_cast<FLOAT>( iNumHeight );

	//	Set LineData
	//	Set Vertex
	m_LineData.iNumVertex = ( iNumWidth + iNumHeight ) * 2;
	
	m_LineData.pVertex = new LINEVERTEX[ m_LineData.iNumVertex ];

	INT l=0;
	//	Set Width
	for( INT i=0 ; i<iNumHeight ; i++ )
	{
		m_LineData.pVertex[ l     ].vecPosition.x = _vecStart.x;
		m_LineData.pVertex[ l     ].vecPosition.y = 0.5f;
		m_LineData.pVertex[ l     ].vecPosition.z = _vecStart.z + ( i * _fTileSize );

		m_LineData.pVertex[ l     ].dColor = 0xff0000ff;

		m_LineData.pVertex[ l + 1 ].vecPosition.x = _vecStart.x + ( ( iNumWidth - 1 ) * _fTileSize );
		m_LineData.pVertex[ l + 1 ].vecPosition.y = 0.5f;
		m_LineData.pVertex[ l + 1 ].vecPosition.z = _vecStart.z + ( i * _fTileSize );

		m_LineData.pVertex[ l + 1 ].dColor = 0xff0000ff;

		l+=2;
	}

	//	Set Height
	for( INT i=0 ; i<iNumHeight ; i++ )
	{
		m_LineData.pVertex[ l     ].vecPosition.x = _vecStart.x + ( i * _fTileSize );
		m_LineData.pVertex[ l     ].vecPosition.y = 0.5f;
		m_LineData.pVertex[ l     ].vecPosition.z = _vecStart.z;

		m_LineData.pVertex[ l     ].dColor = 0xff0000ff;

		m_LineData.pVertex[ l + 1 ].vecPosition.x = _vecStart.x + ( i * _fTileSize );
		m_LineData.pVertex[ l + 1 ].vecPosition.y = 0.5f;
		m_LineData.pVertex[ l + 1 ].vecPosition.z = _vecStart.z + ( ( iNumHeight - 1 ) * _fTileSize );

		m_LineData.pVertex[ l + 1 ].dColor = 0xff0000ff;

		l+=2;
	}

	CreateLineImage( m_LineData.Image, m_LineData.iNumVertex, m_LineData.pVertex );
}


VOID TileMap::Create( D3DXVECTOR3 _vecStart, D3DXVECTOR3 _vecEnd, FLOAT _fTileSize )
{
	InitInfo( _vecStart, _vecEnd, _fTileSize );
	InitTileData( _vecStart, _vecEnd, _fTileSize );
	InitLineData( _vecStart, _vecEnd, _fTileSize );

}
VOID TileMap::Update()
{
}

VOID TileMap::Render()
{
	RenderTileImage( m_TileData.Image );
	RenderLineImage( m_LineData.Image );
}

BOOL TileMap::SetInfo( INT _iX, INT _iY, DWORD _dType )
{
	if( 0 > _iX && _iX > m_Info.iMaxWidth )
		return FALSE;
	if( 0 > _iY && _iY > m_Info.iMaxHeight )
		return FALSE;

	switch( _dType )
	{
	case TLM_WAY:
		ChangePixel( m_TileData.Image, _iX, _iY, 0xffffffff );
		m_Info.pNavGraphNode[ _iX + _iY * m_Info.iMaxWidth ] = 0;
		break;
	case TLM_WALL:
		ChangePixel( m_TileData.Image, _iX, _iY, 0xff000000 );
		m_Info.pNavGraphNode[ _iX + _iY * m_Info.iMaxWidth ] = 1;
		break;
	case TLM_COURSE:
		ChangePixel( m_TileData.Image, _iX, _iY, 0xff00ff00 );
		break;
	}

	for( INT y=0 ; y<m_Info.iMaxHeight ; y++ )
	{
		for( INT x=0 ; x<m_Info.iMaxWidth ; x++ )
		{
			CDebugConsole::GetInstance()->Messagef( L"%d ", m_Info.pNavGraphNode[ x + y * m_Info.iMaxHeight ] );
		}
		CDebugConsole::GetInstance()->Messagef( L"\n" );
	}
	CDebugConsole::GetInstance()->Messagef( L"\n\n" );

	return TRUE;
}

BOOL TileMap::SetInfo( FLOAT _fX, FLOAT _fZ, DWORD _dType )
{
	if( m_Info.vecStart.x > _fX && _fX > m_Info.vecEnd.x )
		return FALSE;
	if( m_Info.vecStart.z > _fZ && _fZ > m_Info.vecEnd.z )
		return FALSE;

	INT iX = static_cast<INT>( ( _fX - m_Info.vecStart.x ) / m_Info.fTileSize );
	INT iY = static_cast<INT>( ( _fZ - m_Info.vecStart.z ) / m_Info.fTileSize );

	if( iX < 0 )iX *= -1;
	if( iY < 0 )iY *= -1;

	switch( _dType )
	{
	case TLM_WAY:
		ChangePixel( m_TileData.Image, iX, iY, 0xffffffff );
		m_Info.pNavGraphNode[ iX + iY * m_Info.iMaxWidth ] = 0;
		break;
	case TLM_WALL:
		ChangePixel( m_TileData.Image, iX, iY, 0xff000000 );
		m_Info.pNavGraphNode[ iX + iY * m_Info.iMaxWidth ] = 1;
		break;
	case TLM_COURSE:
		ChangePixel( m_TileData.Image, iX, iY, 0xff00ff00 );
		break;
	}

	return TRUE;
}

VOID TileMap::SetBBXData( LPD3DXVECTOR3 _pvecPivot, FLOAT* _pfMinus, FLOAT* _pfPlus )
{
	FLOAT	fStartX = _pvecPivot->x + _pfMinus[ 0 ];
	FLOAT	fStartZ = _pvecPivot->z + _pfMinus[ 2 ];

	INT iNumWidth	= static_cast<INT>( ( _pfPlus[ 0 ] - _pfMinus[ 0 ] ) / m_Info.fTileSize );
	INT iNumHeight	= static_cast<INT>( ( _pfPlus[ 2 ] - _pfMinus[ 2 ] ) / m_Info.fTileSize );

	FLOAT fX = 0.0f;
	FLOAT fZ = 0.0f;
	for( INT j=0 ; j<=iNumHeight ; j++ )
	{
		for( INT i=0 ; i<=iNumWidth ; i++ )
		{
			SetInfo( fStartX + m_Info.fTileSize * fX, fStartZ + m_Info.fTileSize * fZ, TLM_WALL );

			fX += 1.0f;
		}
		fZ += 1.0f;
		fX = 0.0f;
	}
}