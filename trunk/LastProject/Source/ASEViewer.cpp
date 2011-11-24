#include "stdafx.h"
#include "ASEViewer.h"
#include "ASEParser.h"
#include "BBXParser.h"
#include "TileMap.h"

#define ANI_1 0
#define ANI_2 1
#define ANI_3 2

VOID ASEViewer::Initialize()
{
	m_pASEData		= NULL;
	m_pBBXData		= NULL;

	m_pASEParser	= NULL;
	m_pBBXParser	= NULL;
	m_pTileMap		= NULL;

	m_iNumASEData	= 0;
	m_iNumBBXData	= 0;

	m_pASEParser	= new ASEParser();
	m_pBBXParser	= new BBXParser();
	m_pTileMap		= new TileMap( m_pd3dDevice );
}


VOID ASEViewer::Release()
{
	if( m_pASEData != NULL )
		delete[] m_pASEData;
	if( m_pBBXData != NULL )
		delete[] m_pBBXData;

	if( m_pASEParser != NULL )
		delete m_pASEParser;
	if( m_pBBXParser != NULL )
		delete m_pBBXParser;
	if( m_pTileMap != NULL )
		delete m_pTileMap;
}

VOID ASEViewer::InitASE( LPWSTR _ASEFileName )
{
	if( FALSE == m_pASEParser->LoadFile( _ASEFileName ) )
	{
		MessageBox( NULL, L"LoadASEFile failed...", NULL, MB_OK );
		return;
	}
	
	m_iNumASEData = m_pASEParser->GetNumObject();
	
	if( m_pASEData != NULL )
		delete[] m_pASEData;
	m_pASEData = new DATA[ m_iNumASEData ];

	for( INT i=0 ; i<m_iNumASEData ; i++ )
	{
		CDebugConsole::GetInstance()->Messagef( L"%d\n", i );
		CreateDataFromASE( i );
	}
}

VOID ASEViewer::InitBBX( LPWSTR _BBXFileName )
{
	if( FALSE == m_pBBXParser->LoadFile( _BBXFileName ) )
	{
		MessageBox( NULL, L"LoadBBXFile failed...", NULL, MB_OK );
		return;
	}

	m_iNumBBXData	= m_pBBXParser->GetNumBoundBox();

	if( m_pBBXData != NULL )
		delete[] m_pBBXData;
	m_pBBXData = new DATA[ m_iNumBBXData ];

	for( INT i=0 ; i<m_iNumBBXData ; i++ )
		CreateDataFromBBX( i );
}
VOID ASEViewer::CreateDataFromASE( const INT _Index )
{
	ASEParser::MATERIALDATA		MaterialData;
	ASEParser::GEOMOBJECTDATA	GeomObjectData;

	m_pASEParser->GetObjectData( _Index, MaterialData, GeomObjectData );

	lstrcpy( m_pASEData[ _Index ].aObjectName, GeomObjectData.pNodeName );
	lstrcpy( m_pASEData[ _Index ].aTextureFileName, MaterialData.pTextureFileName );

	m_pASEData[ _Index ].iNumVertex		=	GeomObjectData.iNumVertex;
	m_pASEData[ _Index ].iNumIndex		=	GeomObjectData.iNumIndex;

	CreateVB( &m_pASEData[ _Index ].pVB, m_pASEData[ _Index ].iNumVertex, sizeof( ASEParser::VERTEX ), ASEParser::VERTEX::FVF );
	SetVB( m_pASEData[ _Index ].pVB, GeomObjectData.pVertex, m_pASEData[ _Index ].iNumVertex, sizeof( ASEParser::VERTEX ) );
	
	CreateIB( &m_pASEData[ _Index ].pIB, m_pASEData[ _Index ].iNumIndex, sizeof( ASEParser::INDEX ) );
	SetIB( m_pASEData[ _Index ].pIB, GeomObjectData.pIndex, m_pASEData[ _Index ].iNumIndex, sizeof( ASEParser::INDEX ) );

	if( MaterialData.pTextureFileName != NULL )
		LoadTextureFromFile( &m_pASEData[ _Index ].pTex, MaterialData.pTextureFileName );
}

VOID ASEViewer::CreateDataFromBBX( const INT _Index )
{
	BBXParser::DATA BBXData;

	m_pBBXParser->GetData( _Index, BBXData );

	m_pBBXData[ _Index ].iNumVertex = 8;
	m_pBBXData[ _Index ].iNumIndex	= 12;

	CreateVB( &m_pBBXData[ _Index ].pVB, m_pBBXData[ _Index ].iNumVertex, sizeof( BBXParser::VERTEX ), BBXParser::VERTEX::FVF );
	SetVB( m_pBBXData[ _Index ].pVB, BBXData.aVertex, m_pBBXData[ _Index ].iNumVertex, sizeof( BBXParser::VERTEX ) );
	CreateIB( &m_pBBXData[ _Index ].pIB, m_pBBXData[ _Index ].iNumIndex, sizeof( BBXParser::INDEX ) );
	SetIB( m_pBBXData[ _Index ].pIB, BBXData.aIndex, m_pBBXData[ _Index ].iNumIndex, sizeof( BBXParser::INDEX ) );

	////	BoundBox
	//CBoundBox* pBoundBox = new CBoundBox();
	//
	//pBoundBox->SetSize( 0, BBXData.Info.fMinusSize[ 0 ] );
	//pBoundBox->SetSize( 1, BBXData.Info.fMinusSize[ 1 ] );
	//pBoundBox->SetSize( 2, BBXData.Info.fMinusSize[ 2 ] );

	//pBoundBox->SetSize( 3, BBXData.Info.fPlusSize[ 0 ] );
	//pBoundBox->SetSize( 4, BBXData.Info.fPlusSize[ 1 ] );
	//pBoundBox->SetSize( 5, BBXData.Info.fPlusSize[ 2 ] );

	//pBoundBox->SetPosition( BBXData.Info.vPivot );

	//CTree::InsertObject( CTree::GetRoot(), pBoundBox );
	////	End

}

VOID ASEViewer::Create( LPWSTR _ASEFileName, LPWSTR _BBXFileName )
{
	InitASE( _ASEFileName );

	//m_pASEParser->AddAnimationData( ASEANI_POST_IDLE, ANI_1, 0, 10, FALSE );
	m_pASEParser->AddAnimationData( ASEANI_IDLE, ANI_1, 0, 100, TRUE );
	m_pASEParser->AddAnimationData( ASEANI_POST_IDLE, ANI_2, 10, 20, FALSE );
	m_pASEParser->AddAnimationData( ASEANI_POST_IDLE, ANI_3, 20, 30, FALSE );

	if( _BBXFileName != NULL )
		InitBBX( _BBXFileName );

	m_pTileMap->Create( D3DXVECTOR3( -510.0f, 0.0f, -510.0f ), D3DXVECTOR3( 510.0f, 0.0f, 510.0f ), 10.0f );

	for( INT i=0 ; i<m_pBBXParser->GetNumBoundBox() ; i++ )
	{
		BBXParser::DATA BBXData;
		m_pBBXParser->GetData( i, BBXData );

		m_pTileMap->SetBBXData( &BBXData.Info.vPivot, BBXData.Info.fMinusSize, BBXData.Info.fPlusSize );
	}

	//	Set TileMapInfo
	INT iTileWidth	= m_pTileMap->GetMapInfo()->iMaxWidth;
	INT iTileHeight	= m_pTileMap->GetMapInfo()->iMaxHeight;

	m_TileMapInfo.pNavGraphNode = new INT[ iTileWidth * iTileHeight ];
	memcpy( m_TileMapInfo.pNavGraphNode, m_pTileMap->GetMapInfo()->pNavGraphNode, sizeof( INT ) * iTileWidth * iTileHeight );
	//m_TileMapInfo.pNavGraphNode = m_pTileMap->GetMapInfo()->pNavGraphNode;

	m_TileMapInfo.fTileSize	= m_pTileMap->GetMapInfo()->fTileSize;

	m_TileMapInfo.vecStart	= m_pTileMap->GetMapInfo()->vecStart;
	m_TileMapInfo.vecEnd	= m_pTileMap->GetMapInfo()->vecEnd;

}

VOID ASEViewer::Update()
{
	m_pASEParser->FrameMove();

	if( GetKeyState( '1' ) & 0x8000 )
		m_pASEParser->SetAnimation( ANI_1 );
	if( GetKeyState( '2' ) & 0x8000 )
		m_pASEParser->SetAnimation( ANI_2 );
	if( GetKeyState( '3' ) & 0x8000 )
		m_pASEParser->SetAnimation( ANI_3 );
}


VOID ASEViewer::RenderASEData( INT _Index )
{
	if( m_pASEData[ _Index ].pTex != NULL )
		m_pd3dDevice->SetTexture( 0, m_pASEData[ _Index ].pTex );

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	m_pASEParser->GetAniTrack( matWorld, _Index );
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	m_pd3dDevice->SetStreamSource( 0, m_pASEData[ _Index ].pVB, 0, sizeof( ASEParser::VERTEX ) );
	m_pd3dDevice->SetFVF( ASEParser::VERTEX::FVF );
	m_pd3dDevice->SetIndices( m_pASEData[ _Index ].pIB );
	m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_pASEData[ _Index ].iNumVertex, 0, m_pASEData[ _Index ].iNumIndex );

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity( &matIdentity );
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matIdentity );

}

VOID ASEViewer::RenderBBXData( INT _Index )
{
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	
	m_pd3dDevice->SetStreamSource( 0, m_pBBXData[ _Index ].pVB, 0, sizeof( BBXParser::VERTEX ) );
	m_pd3dDevice->SetFVF( BBXParser::VERTEX::FVF );
	m_pd3dDevice->SetIndices( m_pBBXData[ _Index ].pIB );
	m_pd3dDevice->DrawIndexedPrimitive( /*BBXParser::DATA::PT*/D3DPT_LINELIST, 0, 0, m_pBBXData[ _Index ].iNumVertex, 0, m_pBBXData[ _Index ].iNumIndex );

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

VOID ASEViewer::Render()
{
	for( INT i=0 ; i<m_iNumASEData ; i++ )
		RenderASEData( i );
	for( INT i=0 ; i<m_iNumBBXData ; i++ )
		RenderBBXData( i );

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pTileMap->Render();
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

INT ASEViewer::GetCurrentFrame()
{
	return m_pASEParser->m_iCurrentFrame;
}