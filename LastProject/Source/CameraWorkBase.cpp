#include "stdafx.h"
#include "CameraWorkBase.h"

VOID CameraWorkBase::Initialize()
{
	m_dColBox				= D3DCOLOR_XRGB( 255, 127, 39 );	//	주
	m_dColBoxLink			= D3DCOLOR_XRGB( 164, 0, 187 );	//	분홍
	m_dColPosition			= D3DCOLOR_XRGB( 255, 255, 0 );		//	노
	m_dColLookAt			= D3DCOLOR_XRGB( 0, 255, 0 );		//	초
	m_dColPositionToLookAt	= D3DCOLOR_XRGB( 0, 0, 255 );		//	파
}

VOID CameraWorkBase::Release()
{
}

VOID CameraWorkBase::CreateImage_Box( LPIMAGE _pImage )
{
	INT iNumVertices	= 8;
	INT iNumIndices		= 12;

	_pImage->iNumVertices	= iNumVertices;
	_pImage->iNumIndices	= iNumIndices;

	//	Init Vertex
	VERTEX	aVertex[ 8 ];
	
	aVertex[ 0 ].vecPos = D3DXVECTOR3( -1.0f, 1.0f, -1.0f );
	aVertex[ 1 ].vecPos = D3DXVECTOR3( 1.0f, 1.0f, -1.0f );
	aVertex[ 2 ].vecPos = D3DXVECTOR3( -1.0f, 1.0f, 1.0f );
	aVertex[ 3 ].vecPos = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	aVertex[ 4 ].vecPos = D3DXVECTOR3( -1.0f, -1.0f, -1.0f );
	aVertex[ 5 ].vecPos = D3DXVECTOR3( 1.0f, -1.0f, -1.0f );
	aVertex[ 6 ].vecPos = D3DXVECTOR3( -1.0f, -1.0f, 1.0f );
	aVertex[ 7 ].vecPos = D3DXVECTOR3( 1.0f, -1.0f, 1.0f );

	aVertex[ 0 ].dColor	= m_dColBox;
	aVertex[ 1 ].dColor	= m_dColBox;
	aVertex[ 2 ].dColor	= m_dColBox;
	aVertex[ 3 ].dColor	= m_dColBox;
	aVertex[ 4 ].dColor	= m_dColBox;
	aVertex[ 5 ].dColor	= m_dColBox;
	aVertex[ 6 ].dColor	= m_dColBox;
	aVertex[ 7 ].dColor	= m_dColBox;

	//	Init Index
	INDEX	aIndex[ 12 ];
	
	aIndex[ 0 ]._0 = 0;		aIndex[ 0 ]._1 = 1;	
	aIndex[ 1 ]._0 = 0;		aIndex[ 1 ]._1 = 2;	
	aIndex[ 2 ]._0 = 2;		aIndex[ 2 ]._1 = 3;	
	aIndex[ 3 ]._0 = 3;		aIndex[ 3 ]._1 = 1;	

	aIndex[ 4 ]._0 = 0;		aIndex[ 4 ]._1 = 4;	
	aIndex[ 5 ]._0 = 1;		aIndex[ 5 ]._1 = 5;	
	aIndex[ 6 ]._0 = 2;		aIndex[ 6 ]._1 = 6;	
	aIndex[ 7 ]._0 = 3;		aIndex[ 7 ]._1 = 7;	

	aIndex[ 8 ]._0 = 4;		aIndex[ 8 ]._1 = 5;	
	aIndex[ 9 ]._0 = 5;		aIndex[ 9 ]._1 = 7;	
	aIndex[ 10 ]._0 = 6;	aIndex[ 10 ]._1 = 7;	
	aIndex[ 11 ]._0 = 6;	aIndex[ 11 ]._1 = 4;	

	//	Init VertexBuffer, IndexBuffer
	CreateVB( &_pImage->pVB, iNumVertices, sizeof( VERTEX ), VERTEX::FVF );
	SetVB( _pImage->pVB, aVertex, iNumVertices, sizeof( VERTEX ) );

	CreateIB( &_pImage->pIB, iNumIndices, sizeof( INDEX ) );
	SetIB( _pImage->pIB, aIndex, iNumIndices, sizeof( INDEX) );
}

VOID CameraWorkBase::CreateInfo_BezierCurve( LPINFO _pInfo, INT _iNumVertices, D3DXVECTOR3& _vecPoint0, D3DXVECTOR3& _vecPoint1, D3DXVECTOR3& _vecPoint2, D3DXVECTOR3& _vecPoint3 )
{
	if( _pInfo->pVertex == NULL || _pInfo->iNumVertices != _iNumVertices )
	{	//	처음이거나 점의 갯수가 변했을때 새로 만든다
		if( _pInfo->pVertex != NULL )
			delete[] _pInfo->pVertex;
		if( _pInfo->pIndex != NULL )
			delete[] _pInfo->pIndex;
		if( _pInfo->pfIncInterporation != NULL )
			delete[] _pInfo->pfIncInterporation;

		_pInfo->iNumVertices		= _iNumVertices;
		_pInfo->iNumIndices			= _iNumVertices - 1;
		_pInfo->iNumInterporation	= _iNumVertices;

		_pInfo->pVertex				= new VERTEX[ _pInfo->iNumVertices ];
		_pInfo->pIndex				= new INDEX[ _pInfo->iNumIndices ];
		_pInfo->pfIncInterporation	= new FLOAT[ _pInfo->iNumInterporation ];

		//	Init Index
		for( INT i=0 ; i<_pInfo->iNumIndices ; i++ )
		{
			_pInfo->pIndex[ i ]._0 = i;
			_pInfo->pIndex[ i ]._1 = i + 1;
		}
		//	Init IncInterporation
		for( INT i=0 ; i<_pInfo->iNumInterporation ; i++ )
			_pInfo->pfIncInterporation[ i ] = 0.1f;
	}
			
	//	Set Vertex
	LPVERTEX pVertex = _pInfo->pVertex;

	//FLOAT fIncT = 1.0f / static_cast<FLOAT>( _iNumVertices );
	FLOAT fIncT = 1.0f / static_cast<FLOAT>( _pInfo->iNumIndices );

	INT i=0;
	for( FLOAT t=0.0f ; t<1.0f ; t+=fIncT )
	{
		pVertex[ i ].vecPos.x = _vecPoint0.x * ( 1.0f - t ) * ( 1.0f - t ) * ( 1.0f - t ) + 
								_vecPoint1.x * 3.0f * t * ( 1.0f - t ) * ( 1.0f - t ) + 
								_vecPoint2.x * 3.0f * t * t * ( 1.0f - t ) +
								_vecPoint3.x * t * t * t;
		
		pVertex[ i ].vecPos.y = _vecPoint0.y * ( 1.0f - t ) * ( 1.0f - t ) * ( 1.0f - t ) + 
								_vecPoint1.y * 3.0f * t * ( 1.0f - t ) * ( 1.0f - t ) + 
								_vecPoint2.y * 3.0f * t * t * ( 1.0f - t ) +
								_vecPoint3.y * t * t * t;

		pVertex[ i ].vecPos.z = _vecPoint0.z * ( 1.0f - t ) * ( 1.0f - t ) * ( 1.0f - t ) + 
								_vecPoint1.z * 3.0f * t * ( 1.0f - t ) * ( 1.0f - t ) + 
								_vecPoint2.z * 3.0f * t * t * ( 1.0f - t ) +
								_vecPoint3.z * t * t * t;
		
		pVertex[ i ].dColor	= m_dColPosition;

		i++;
		//if( i >= _iNumVertices )
		//	break;
	}

	pVertex[ _iNumVertices - 1 ].vecPos = _vecPoint3;
	pVertex[ _iNumVertices - 1 ].dColor = m_dColPosition;
}

VOID CameraWorkBase::CreateInfo_Line2( LPINFO _pInfo, INT _iNumVertices, D3DXVECTOR3& _vecPoint0, D3DXVECTOR3& _vecPoint1 )
{
	if( _pInfo->pVertex == NULL || _pInfo->iNumVertices != _iNumVertices )
	{	//	처음이거나 점의 갯수가 변했을때 새로 만든다
		if( _pInfo->pVertex != NULL )
			delete[] _pInfo->pVertex;
		if( _pInfo->pIndex != NULL )
			delete[] _pInfo->pIndex;
		if( _pInfo->pfIncInterporation != NULL )
			delete[] _pInfo->pfIncInterporation;

		_pInfo->iNumVertices		= _iNumVertices;
		_pInfo->iNumIndices			= _iNumVertices - 1;
		_pInfo->iNumInterporation	= _iNumVertices;

		_pInfo->pVertex				= new VERTEX[ _pInfo->iNumVertices ];
		_pInfo->pIndex				= new INDEX[ _pInfo->iNumIndices ];
		_pInfo->pfIncInterporation	= new FLOAT[ _pInfo->iNumInterporation ];

		//	Init Index
		for( INT i=0 ; i<_pInfo->iNumIndices ; i++ )
		{
			_pInfo->pIndex[ i ]._0 = i;
			_pInfo->pIndex[ i ]._1 = i + 1;
		}
		//	Init IncInterporation
		for( INT i=0 ; i<_pInfo->iNumInterporation ; i++ )
			_pInfo->pfIncInterporation[ i ] = 0.1f;
	}

	//	Set Vertex
	LPVERTEX pVertex = _pInfo->pVertex;
	
	FLOAT fNumIndices = static_cast<FLOAT>( _pInfo->iNumIndices );

	FLOAT fIncX = ( _vecPoint1.x - _vecPoint0.x ) / fNumIndices;
	FLOAT fIncY = ( _vecPoint1.y - _vecPoint0.y ) / fNumIndices;
	FLOAT fIncZ = ( _vecPoint1.z - _vecPoint0.z ) / fNumIndices;

	for( INT i=0 ; i<_iNumVertices ; i++ )
	{
		pVertex[ i ].vecPos.x = _vecPoint0.x + fIncX * static_cast<FLOAT>( i );
		pVertex[ i ].vecPos.y = _vecPoint0.y + fIncY * static_cast<FLOAT>( i );
		pVertex[ i ].vecPos.z = _vecPoint0.z + fIncZ * static_cast<FLOAT>( i );
	}

}

VOID CameraWorkBase::CheckNumVerticesOfLine4( INT& _iNumVertices )
{
	INT iNumVertices = 4;

	if( iNumVertices >= _iNumVertices )
	{
		_iNumVertices = iNumVertices;
		return;
	}

	while( 1 )
	{
		iNumVertices = iNumVertices + iNumVertices - 1;

		if( iNumVertices >= _iNumVertices )
		{
			_iNumVertices = iNumVertices;
			return;
		}
	}
}

VOID CameraWorkBase::CreateInfo_Line4( LPINFO _pInfo, INT _iNumVertices, D3DXVECTOR3& _vecPoint0, D3DXVECTOR3& _vecPoint1, D3DXVECTOR3& _vecPoint2, D3DXVECTOR3& _vecPoint3 )
{	
	//	|				|				|				|	4		+ 4 - 1
	//	|		|		|		|		|		|		|	7		+ 7 - 1
	//	|	|	|	|	|	|	|	|	|	|	|	|	|	13		+ 13 - 1
	//	| | | | | | | | | | | | | | | | | | | | | | | | |	25		+ 25 - 1
	//	4개의 선이 동일한 갯수의 점을 가지고 있게 한다
	CheckNumVerticesOfLine4( _iNumVertices );

	if( _pInfo->pVertex == NULL || _pInfo->iNumVertices != _iNumVertices )
	{	//	처음이거나 점의 갯수가 변했을때 새로 만든다
		if( _pInfo->pVertex != NULL )
			delete[] _pInfo->pVertex;
		if( _pInfo->pIndex != NULL )
			delete[] _pInfo->pIndex;
		if( _pInfo->pfIncInterporation != NULL )
			delete[] _pInfo->pfIncInterporation;

		_pInfo->iNumVertices		= _iNumVertices;
		_pInfo->iNumIndices			= _iNumVertices - 1;
		_pInfo->iNumInterporation	= _iNumVertices;

		_pInfo->pVertex				= new VERTEX[ _pInfo->iNumVertices ];
		_pInfo->pIndex				= new INDEX[ _pInfo->iNumIndices ];
		_pInfo->pfIncInterporation	= new FLOAT[ _pInfo->iNumInterporation ];

		//	Init Index
		for( INT i=0 ; i<_pInfo->iNumIndices ; i++ )
		{
			_pInfo->pIndex[ i ]._0 = i;
			_pInfo->pIndex[ i ]._1 = i + 1;
		}
		//	Init IncInterporation
		for( INT i=0 ; i<_pInfo->iNumInterporation ; i++ )
			_pInfo->pfIncInterporation[ i ] = 0.1f;
	}

	
	//	Init Vertex
	LPVERTEX pVertex	= _pInfo->pVertex;

	INT iIncVertices	= _iNumVertices / 3;
	
	INT iStartVertex	= 0;
	INT iEndVertex		= 0;

	FLOAT fIncVertices	= static_cast<FLOAT>( iIncVertices );

	FLOAT fIncX = 0.0f, fIncY = 0.0f, fIncZ = 0.0f;

	INT i = 0;
	
	//	Init Line 0
	iStartVertex	= iIncVertices * 0;
	iEndVertex		= iIncVertices * 1;

	fIncX = ( _vecPoint1.x - _vecPoint0.x ) / fIncVertices;
	fIncY = ( _vecPoint1.y - _vecPoint0.y ) / fIncVertices;
	fIncZ = ( _vecPoint1.z - _vecPoint0.z ) / fIncVertices;

	i = 0;
	for( INT iIndex=iStartVertex ; iIndex<iEndVertex ; iIndex++ )
	{
		pVertex[ iIndex ].vecPos.x	= _vecPoint0.x + fIncX * static_cast<FLOAT>( i );
		pVertex[ iIndex ].vecPos.y	= _vecPoint0.y + fIncY * static_cast<FLOAT>( i );
		pVertex[ iIndex ].vecPos.z	= _vecPoint0.z + fIncZ * static_cast<FLOAT>( i );

		pVertex[ iIndex ].dColor	= m_dColBoxLink;

		i++;
	}

	//	Init Line 1
	iStartVertex	= iIncVertices * 1;
	iEndVertex		= iIncVertices * 2;

	fIncX = ( _vecPoint2.x - _vecPoint1.x ) / fIncVertices;
	fIncY = ( _vecPoint2.y - _vecPoint1.y ) / fIncVertices;
	fIncZ = ( _vecPoint2.z - _vecPoint1.z ) / fIncVertices;

	i = 0;
	for( INT iIndex=iStartVertex ; iIndex<iEndVertex ; iIndex++ )
	{
		pVertex[ iIndex ].vecPos.x	= _vecPoint1.x + fIncX * static_cast<FLOAT>( i );
		pVertex[ iIndex ].vecPos.y	= _vecPoint1.y + fIncY * static_cast<FLOAT>( i );
		pVertex[ iIndex ].vecPos.z	= _vecPoint1.z + fIncZ * static_cast<FLOAT>( i );

		pVertex[ iIndex ].dColor	= m_dColBoxLink;

		i++;
	}

	//	Init Line 2
	iStartVertex	= iIncVertices * 2;
	iEndVertex		= iIncVertices * 3;

	fIncX = ( _vecPoint3.x - _vecPoint2.x ) / fIncVertices;
	fIncY = ( _vecPoint3.y - _vecPoint2.y ) / fIncVertices;
	fIncZ = ( _vecPoint3.z - _vecPoint2.z ) / fIncVertices;

	i = 0;
	for( INT iIndex=iStartVertex ; iIndex<iEndVertex ; iIndex++ )
	{
		pVertex[ iIndex ].vecPos.x	= _vecPoint2.x + fIncX * static_cast<FLOAT>( i );
		pVertex[ iIndex ].vecPos.y	= _vecPoint2.y + fIncY * static_cast<FLOAT>( i );
		pVertex[ iIndex ].vecPos.z	= _vecPoint2.z + fIncZ * static_cast<FLOAT>( i );

		pVertex[ iIndex ].dColor	= m_dColBoxLink;

		i++;
	}

	pVertex[ iEndVertex ].vecPos = _vecPoint3;
	pVertex[ iEndVertex ].dColor = m_dColBoxLink;

}

VOID CameraWorkBase::CreateImage( LPIMAGE _pImage, LPINFO _pInfo )
{
	//	Vertex는 외부에서 받아온다
	if( _pImage->pVB == NULL && _pImage->pIB == NULL )
	{	//	처음 만들거나 iNumVertices정보가 변경되었을때
		
		//	Create VertexBuffer
		_pImage->iNumVertices	= _pInfo->iNumVertices;
		
		CreateVB( &_pImage->pVB, _pImage->iNumVertices, sizeof( VERTEX ), VERTEX::FVF );
		SetVB( _pImage->pVB, _pInfo->pVertex, _pImage->iNumVertices, sizeof( VERTEX ) );

		//	Create IndexBuffer
		_pImage->iNumIndices	= _pInfo->iNumIndices;

		CreateIB( &_pImage->pIB, _pImage->iNumIndices, sizeof( INDEX ) );
		SetIB( _pImage->pIB, _pInfo->pIndex, _pImage->iNumIndices, sizeof( INDEX) );

		return;
	}

	LPVOID pVertices;
	if( FAILED( _pImage->pVB->Lock( 0, sizeof( VERTEX ) * _pInfo->iNumVertices, (LPVOID*)&pVertices, 0 ) ) )
	{
		MessageBox( NULL, L"CreateImage_Course(){ ... pVB->Lock failed ... }", NULL, MB_OK );
		return;
	}

	memcpy( pVertices, _pInfo->pVertex, sizeof( VERTEX ) * _pInfo->iNumVertices );

	_pImage->pVB->Unlock();
}


VOID CameraWorkBase::RenderImage( LPIMAGE _pImage )
{
	//	Set World Matrix
	SetMatrix( &_pImage->vecScale, &_pImage->vecRotate, &_pImage->vecTranslate );

	//	Light Off
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	m_pd3dDevice->SetStreamSource( 0, _pImage->pVB, 0, sizeof( VERTEX ) );
	m_pd3dDevice->SetFVF( VERTEX::FVF );
	m_pd3dDevice->SetIndices( _pImage->pIB );
	m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINELIST, 0, 0, _pImage->iNumVertices, 0, _pImage->iNumIndices );

	//	Light On
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	//	Indentity World Matirx
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
}

VOID CameraWorkBase::SetMatrix( LPD3DXVECTOR3 _pvecScale, LPD3DXVECTOR3 _pvecRotate, LPD3DXVECTOR3 _pvecTranslate )
{
	//	Set World Matrix
	D3DXMATRIX matScale;
	D3DXMatrixIdentity( &matScale );
	D3DXMatrixScaling( &matScale, _pvecScale->x, _pvecScale->y, _pvecScale->z );

	D3DXMATRIX matRotateX;
	D3DXMatrixIdentity( &matRotateX );
	D3DXMatrixRotationX( &matRotateX, _pvecRotate->x );

	D3DXMATRIX matRotateY;
	D3DXMatrixIdentity( &matRotateY );
	D3DXMatrixRotationY( &matRotateY, _pvecRotate->y );

	D3DXMATRIX matRotateZ;
	D3DXMatrixIdentity( &matRotateZ );
	D3DXMatrixRotationZ( &matRotateZ, _pvecRotate->z );

	D3DXMATRIX matTranslate;
	D3DXMatrixIdentity( &matTranslate );
	D3DXMatrixTranslation( &matTranslate, _pvecTranslate->x, _pvecTranslate->y, _pvecTranslate->z );

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	matWorld = matScale * matRotateX * matRotateY * matRotateZ * matTranslate;

	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
}

HRESULT	CameraWorkBase::CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF )
{
	if( FAILED( m_pd3dDevice->CreateVertexBuffer(	_nVertex * _Size,
													0,
													_FVF,
													D3DPOOL_DEFAULT,
													&(*_ppVB),
													NULL ) ) )
	{
		MessageBox( NULL, L"CameraWorkBase::CreateVertexBuffer() failed.", NULL, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

HRESULT	CameraWorkBase::SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size )
{
	LPVOID pvertices;
	if( FAILED( _pVB->Lock( 0, _nVertex *  _Size, (VOID**)&pvertices, 0 ) ) )
	{
		MessageBox( NULL, L"CameraWorkBase::m_pVB->Lock() failed.", NULL, MB_OK );
		return E_FAIL;
	}
	
		memcpy( pvertices, _pvertices, _nVertex * _Size );

	_pVB->Unlock();

	return S_OK;
}

HRESULT	CameraWorkBase::CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size )
{
	if( FAILED( m_pd3dDevice->CreateIndexBuffer(	_nIndex * _Size,
													0,
													D3DFMT_INDEX32,
													D3DPOOL_DEFAULT,
													&(*_ppIB),
													NULL ) ) )
	{
		MessageBox( NULL, TEXT("CameraWorkBase::CreateIndexBuffer() failed."), NULL, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

HRESULT	CameraWorkBase::SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size )
{
	LPVOID pIndices;
	if( FAILED( _pIB->Lock( 0, _nIndex * _Size, (VOID**)&pIndices, 0 ) ) )
	{
		MessageBox( NULL, L"CameraWorkBase::m_pIB->Lock() failed.", NULL, MB_OK );
		return E_FAIL;
	}
	
		memcpy( pIndices, _indices, _nIndex * _Size );

	_pIB->Unlock();

	return S_OK;
}