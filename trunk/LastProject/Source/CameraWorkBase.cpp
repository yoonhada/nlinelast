#include "stdafx.h"
#include "CameraWorkBase.h"

VOID CameraWorkBase::Initialize()
{
}

VOID CameraWorkBase::Release()
{
}

VOID CameraWorkBase::CreateImage_Camera( LPIMAGE _pimgCamera, DWORD _dColor )
{	//	Scale, Rotate, Translate로 변형 가능
	//	Init Vertices, Indices
	_pimgCamera->iNumVertices	= 8;
	_pimgCamera->iNumIndices	= 12;

	//	Init Vertex
	VERTEX	aVertex[ 8 ];
	
	aVertex[ 0 ].vecPos = D3DXVECTOR3( -3.0f, 3.0f, -3.0f );
	aVertex[ 1 ].vecPos = D3DXVECTOR3( 3.0f, 3.0f, -3.0f );
	aVertex[ 2 ].vecPos = D3DXVECTOR3( -1.0f, 1.0f, 1.0f );
	aVertex[ 3 ].vecPos = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	aVertex[ 4 ].vecPos = D3DXVECTOR3( -3.0f, -3.0f, -3.0f );
	aVertex[ 5 ].vecPos = D3DXVECTOR3( 3.0f, -3.0f, -3.0f );
	aVertex[ 6 ].vecPos = D3DXVECTOR3( -1.0f, -1.0f, 1.0f );
	aVertex[ 7 ].vecPos = D3DXVECTOR3( 1.0f, -1.0f, 1.0f );

	aVertex[ 0 ].dColor	= _dColor;
	aVertex[ 1 ].dColor	= _dColor;
	aVertex[ 2 ].dColor	= _dColor;
	aVertex[ 3 ].dColor	= _dColor;
	aVertex[ 4 ].dColor	= _dColor;
	aVertex[ 5 ].dColor	= _dColor;
	aVertex[ 6 ].dColor	= _dColor;
	aVertex[ 7 ].dColor	= _dColor;

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
	CreateVB( &_pimgCamera->pVB, 8, sizeof( VERTEX ), VERTEX::FVF );
	SetVB( _pimgCamera->pVB, aVertex, 8, sizeof( VERTEX ) );

	CreateIB( &_pimgCamera->pIB, 12, sizeof( INDEX ) );
	SetIB( _pimgCamera->pIB, aIndex, 12, sizeof( INDEX) );
}

VOID CameraWorkBase::CreateImage_Box( LPIMAGE _pimgBox, DWORD _dColor )
{	//	Scale, Rotate, Translate로 변형 가능
	//	Init Vertices, Indices
	_pimgBox->iNumVertices	= 8;
	_pimgBox->iNumIndices	= 12;

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

	aVertex[ 0 ].dColor	= _dColor;
	aVertex[ 1 ].dColor	= _dColor;
	aVertex[ 2 ].dColor	= _dColor;
	aVertex[ 3 ].dColor	= _dColor;
	aVertex[ 4 ].dColor	= _dColor;
	aVertex[ 5 ].dColor	= _dColor;
	aVertex[ 6 ].dColor	= _dColor;
	aVertex[ 7 ].dColor	= _dColor;

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
	CreateVB( &_pimgBox->pVB, 8, sizeof( VERTEX ), VERTEX::FVF );
	SetVB( _pimgBox->pVB, aVertex, 8, sizeof( VERTEX ) );

	CreateIB( &_pimgBox->pIB, 12, sizeof( INDEX ) );
	SetIB( _pimgBox->pIB, aIndex, 12, sizeof( INDEX) );
}

VOID CameraWorkBase::CreateImage_Course( LPIMAGE _pimgCourse, LPCOURSEINFO _pinfCourse )
{	//	Scale, Rotate, Translate로 변형 불가
	//	Vertex는 외부에서 받아온다
	if( _pimgCourse->pVB == NULL && _pimgCourse->pIB == NULL )
	{	//	처음 만들거나 iNumVertices정보가 변경되었을때
		//	Create VertexBuffer
		_pimgCourse->iNumVertices	= _pinfCourse->iNumVertices;
		
		CreateVB( &_pimgCourse->pVB, _pimgCourse->iNumVertices, sizeof( VERTEX ), VERTEX::FVF );
		SetVB( _pimgCourse->pVB, _pinfCourse->pVertex, _pimgCourse->iNumVertices, sizeof( VERTEX ) );

		//	Create IndexBuffer
		_pimgCourse->iNumIndices	= _pinfCourse->iNumVertices - 1;

		//	Line으로 그리는 것이니 항상 같음
		LPINDEX pIndex = new INDEX[ _pimgCourse->iNumIndices ];

		for( INT i=0 ; i<_pimgCourse->iNumIndices ; i++ )
		{
			pIndex[ i ]._0 = i;
			pIndex[ i ]._1 = i + 1;
		}

		CreateIB( &_pimgCourse->pIB, _pimgCourse->iNumIndices, sizeof( INDEX ) );
		SetIB( _pimgCourse->pIB, pIndex, _pimgCourse->iNumIndices, sizeof( INDEX) );

		delete[] pIndex;

		return;
	}

	LPVOID pVertices;
	if( FAILED( _pimgCourse->pVB->Lock( 0, sizeof( VERTEX ) * _pimgCourse->iNumVertices, (LPVOID*)&pVertices, 0 ) ) )
	{
		MessageBox( NULL, L"CreateImage_Course(){ ... pVB->Lock failed ... }", NULL, MB_OK );
		return;
	}

	memcpy( pVertices, _pinfCourse->pVertex, sizeof( VERTEX ) * _pinfCourse->iNumVertices );

	_pimgCourse->pVB->Unlock();

}


VOID CameraWorkBase::CreateBoxLink( LPIMAGE _pimgBoxLink, LPIMAGE _pimgBox, DWORD _dColor )
{
	//	Init Vertices, Indices
	_pimgBoxLink->iNumVertices	= 4;
	_pimgBoxLink->iNumIndices	= 3;

	//	Init Vertex
	VERTEX aVertex[ 4 ];

	aVertex[ 0 ].vecPos = _pimgBox[ 0 ].vecTranslate;
	aVertex[ 0 ].dColor	= _dColor;

	aVertex[ 1 ].vecPos = _pimgBox[ 1 ].vecTranslate;
	aVertex[ 1 ].dColor	= _dColor;

	aVertex[ 2 ].vecPos = _pimgBox[ 2 ].vecTranslate;
	aVertex[ 2 ].dColor	= _dColor;

	aVertex[ 3 ].vecPos = _pimgBox[ 3 ].vecTranslate;
	aVertex[ 3 ].dColor	= _dColor;

	//	Init Index
	INDEX aIndex[ 3 ];

	aIndex[ 0 ]._0 = 0;		aIndex[ 0 ]._1 = 1;
	aIndex[ 1 ]._0 = 1;		aIndex[ 1 ]._1 = 2;
	aIndex[ 2 ]._0 = 2;		aIndex[ 2 ]._1 = 3;

	//	Init VertexBuffer, IndexBuffer
	CreateVB( &_pimgBoxLink->pVB, 4, sizeof( VERTEX ), VERTEX::FVF );
	SetVB( _pimgBoxLink->pVB, aVertex, 4, sizeof( VERTEX ) );

	CreateIB( &_pimgBoxLink->pIB, 3, sizeof( INDEX ) );
	SetIB( _pimgBoxLink->pIB, aIndex, 3, sizeof( INDEX) );

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

HRESULT	CameraWorkBase::CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF )
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

HRESULT	CameraWorkBase::SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size )
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

HRESULT	CameraWorkBase::CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size )
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

HRESULT	CameraWorkBase::SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size )
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
