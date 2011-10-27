#include "stdafx.h"
#include "Map.h"
#include "ASEParser.h"
#include "BBXParser.h"

#define ANI_1 0
#define ANI_2 1
#define ANI_3 2

VOID Map::Initialize()
{
	m_pASEData		= NULL;
	m_pBBXData		= NULL;

	m_pASEParser	= NULL;
	m_pBBXParser	= NULL;

	m_iNumASEData	= 0;
	m_iNumBBXData	= 0;

	m_pASEParser	= new ASEParser();
	m_pBBXParser	= new BBXParser();
}

VOID Map::Release()
{
	if( m_pASEData != NULL )
		delete[] m_pASEData;
	if( m_pBBXData != NULL )
		delete[] m_pBBXData;

	if( m_pASEParser != NULL )
		delete m_pASEParser;
	if( m_pBBXParser != NULL )
		delete m_pBBXParser;

	for( std::vector<CBoundBox*>::iterator iter=m_pBoundBoxVector.begin(); iter!=m_pBoundBoxVector.end(); ++iter)
	{
		delete *iter;
	}

	if ( m_pBoundBoxVector.empty() == FALSE )
	{
		m_pBoundBoxVector.clear();
		m_pBoundBoxVector.erase( m_pBoundBoxVector.begin(), m_pBoundBoxVector.end() );
	}
}

VOID Map::InitASE( LPWSTR _ASEFileName )
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
		CreateDataFromASE( i );
}

VOID Map::InitBBX( LPWSTR _BBXFileName )
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
VOID Map::CreateDataFromASE( const INT _Index )
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

VOID Map::CreateDataFromBBX( const INT _Index )
{
	BBXParser::DATA BBXData;

	m_pBBXParser->GetData( _Index, BBXData );

	m_pBBXData[ _Index ].iNumVertex = 8;
	m_pBBXData[ _Index ].iNumIndex	= 12;

	CreateVB( &m_pBBXData[ _Index ].pVB, m_pBBXData[ _Index ].iNumVertex, sizeof( BBXParser::VERTEX ), BBXParser::VERTEX::FVF );
	SetVB( m_pBBXData[ _Index ].pVB, BBXData.aVertex, m_pBBXData[ _Index ].iNumVertex, sizeof( BBXParser::VERTEX ) );
	CreateIB( &m_pBBXData[ _Index ].pIB, m_pBBXData[ _Index ].iNumIndex, sizeof( BBXParser::INDEX ) );
	SetIB( m_pBBXData[ _Index ].pIB, BBXData.aIndex, m_pBBXData[ _Index ].iNumIndex, sizeof( BBXParser::INDEX ) );

	//	BoundBox
	CBoundBox* pBoundBox = new CBoundBox();
	
	pBoundBox->SetSize( 0, BBXData.Info.fMinusSize[ 0 ] );
	pBoundBox->SetSize( 1, BBXData.Info.fMinusSize[ 1 ] );
	pBoundBox->SetSize( 2, BBXData.Info.fMinusSize[ 2 ] );

	pBoundBox->SetSize( 3, BBXData.Info.fPlusSize[ 0 ] );
	pBoundBox->SetSize( 4, BBXData.Info.fPlusSize[ 1 ] );
	pBoundBox->SetSize( 5, BBXData.Info.fPlusSize[ 2 ] );

	pBoundBox->SetPosition( BBXData.Info.vPivot );

	CTree::GetInstance()->InsertObject( CTree::GetInstance()->GetRoot(), pBoundBox );
	m_pBoundBoxVector.push_back(pBoundBox);
	//	End
}

VOID Map::Create( LPWSTR _ASEFileName, LPWSTR _BBXFileName )
{
	InitASE( _ASEFileName );

	if( _BBXFileName != NULL )
		InitBBX( _BBXFileName );
}

VOID Map::Update()
{
	m_pASEParser->FrameMove();
}


VOID Map::RenderASEData( INT _Index )
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

VOID Map::RenderASEData( INT _Index, D3DXMATRIX _matCharacter )
{
	if( m_pASEData[ _Index ].pTex != NULL )
		m_pd3dDevice->SetTexture( 0, m_pASEData[ _Index ].pTex );

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	m_pASEParser->GetAniTrack( matWorld, _Index );
	
	//	New
	matWorld = matWorld * _matCharacter;
	//	End
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	m_pd3dDevice->SetStreamSource( 0, m_pASEData[ _Index ].pVB, 0, sizeof( ASEParser::VERTEX ) );
	m_pd3dDevice->SetFVF( ASEParser::VERTEX::FVF );
	m_pd3dDevice->SetIndices( m_pASEData[ _Index ].pIB );
	m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_pASEData[ _Index ].iNumVertex, 0, m_pASEData[ _Index ].iNumIndex );

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity( &matIdentity );
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matIdentity );
}

VOID Map::RenderBBXData( INT _Index )
{
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	
	m_pd3dDevice->SetStreamSource( 0, m_pBBXData[ _Index ].pVB, 0, sizeof( BBXParser::VERTEX ) );
	m_pd3dDevice->SetFVF( BBXParser::VERTEX::FVF );
	m_pd3dDevice->SetIndices( m_pBBXData[ _Index ].pIB );
	m_pd3dDevice->DrawIndexedPrimitive( /*BBXParser::DATA::PT*/D3DPT_LINELIST, 0, 0, m_pBBXData[ _Index ].iNumVertex, 0, m_pBBXData[ _Index ].iNumIndex );

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

VOID Map::Render()
{
	for( INT i=0 ; i<m_iNumASEData ; i++ )
		RenderASEData( i );
	for( INT i=0 ; i<m_iNumBBXData ; i++ )
		RenderBBXData( i );
}

VOID Map::Render( D3DXMATRIX& _matCharacter )
{
	for( INT i=0 ; i<m_iNumASEData ; i++ )
		RenderASEData( i, _matCharacter );
	for( INT i=0 ; i<m_iNumBBXData ; i++ )
		RenderBBXData( i );
}

BOOL Map::AddAnimationData( const DWORD _dType, DWORD _dID, INT _iStartFrame, INT _iEndFrame, BOOL _bLoop )
{
	//	Sample
	//AddAnimationData( ASEANI_IDLE, ANI_1, 0, 100, TRUE );
	//AddAnimationData( ASEANI_POST_IDLE, ANI_2, 10, 20, FALSE );
	//AddAnimationData( ASEANI_POST_IDLE, ANI_3, 20, 30, FALSE );

	if( m_pASEParser->AddAnimationData( _dType, _dID, _iStartFrame, _iEndFrame, _bLoop ) )
		return TRUE;

	return FALSE;
}
BOOL Map::SetAnimation( DWORD _dID )
{
	if( m_pASEParser->SetAnimation( _dID ) )
		return TRUE;

	return FALSE;
}

HRESULT	Map::CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF )
{
	if( FAILED( m_pd3dDevice->CreateVertexBuffer(	_nVertex * _Size,
													0,
													_FVF,
													D3DPOOL_DEFAULT,
													&(*_ppVB),
													NULL ) ) )
	{
		MessageBox( NULL, L"CreateVertexBuffer() failed.", NULL, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}
HRESULT	Map::SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size )
{
	LPVOID pvertices;
	if( FAILED( _pVB->Lock( 0, _nVertex *  _Size, (VOID**)&pvertices, 0 ) ) )
	{
		MessageBox( NULL, L"m_pVB->Lock() failed.", NULL, MB_OK );
		return E_FAIL;
	}
	
		memcpy( pvertices, _pvertices, _nVertex * _Size );

	_pVB->Unlock();

	return S_OK;
}
HRESULT	Map::CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size )
{
	if( FAILED( m_pd3dDevice->CreateIndexBuffer(	_nIndex * _Size,
													0,
													D3DFMT_INDEX32,
													D3DPOOL_DEFAULT,
													&(*_ppIB),
													NULL ) ) )
	{
		MessageBox( NULL, TEXT("CreateIndexBuffer() failed."), NULL, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}
HRESULT	Map::SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size )
{
	LPVOID pIndices;
	if( FAILED( _pIB->Lock( 0, _nIndex * _Size, (VOID**)&pIndices, 0 ) ) )
	{
		MessageBox( NULL, L"m_pIB->Lock() failed.", NULL, MB_OK );
		return E_FAIL;
	}
	
		memcpy( pIndices, _indices, _nIndex * _Size );

	_pIB->Unlock();

	return S_OK;
}

HRESULT Map::LoadTextureFromFile( LPDIRECT3DTEXTURE9* _ppTexture, LPCWSTR FileName )
{
	if( FAILED( D3DXCreateTextureFromFileEx(	
					m_pd3dDevice,
					FileName,				//	컴파일러 설정이 Unicode를 요구하고 있는 경우 데이터 타입 LPCSTR은 LPCWSTR이 된다
					D3DX_DEFAULT_NONPOW2,	//	원본 크기를 받아온다 2의 승수로도 받아올수 있다
					D3DX_DEFAULT_NONPOW2,	//	원본 크기를 받아온다
					D3DX_DEFAULT,			//	요구되는 밉레벨의 수, 이 값이 0또는 D3DX_DEFAULT의 경우 완전한 밉맵 체인 생성
					NULL,					//	동적 텍스쳐
					D3DFMT_X8R8G8B8,		//	텍스처 포멧 D3DFMT_UNKKNOWN의 경우 포멧은 파일로부터 취득
					D3DPOOL_MANAGED,		//	텍스처의 배치처가 되는 메모리 클래스를 기술한다
					D3DX_DEFAULT,			//	필터링 방법, D3DX_DEFAULT는 D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER 와 같다
					D3DX_DEFAULT,			//	필터링 방법, D3DX_DEFAULT는 D3DX_FILTER_BOX 와 같다
					NULL,					//	투명이 되는 D3DCOLOR값, 컬러키를 무효로 하는 경우는 0을 지정
					NULL,					//	소스 이미지 파일내의 데이터의 기술을 저장하는 D3DXIMAGE INFO 구조체의 포인터
					NULL,					//	저장하는 256 색 팔레트를 나타내는 PALETTEENTRY 구조체의 포인터
					&(*_ppTexture) ) ) )		//	생성된 큐브 텍스처 개체를 나타내는 IDirect3DTexture9 인터페이스의 포인터 주소
	{
		//MessageBox( NULL, L"D3DCreateTextureFromFile() Failed.", NULL, MB_OK );
		return E_FAIL;
	}
	
	TCHAR	str[ 1024 ];
	wsprintf( str, L"LoadTexture : %s\n", FileName );
	//CDebugConsole::GetInstance()->Message( str );

	return S_OK;

}
