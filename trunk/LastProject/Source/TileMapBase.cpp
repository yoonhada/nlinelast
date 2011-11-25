#include "stdafx.h"
#include "TileMapBase.h"

VOID TileMapBase::Initialize()
{
}

VOID TileMapBase::Release()
{
}

VOID TileMapBase::CreateTileImage( IMAGE& _TileImage, INT _iVertices, LPTILEVERTEX _pVertex, INT _iIndices, LPINDEX _pIndex, INT _itexWidth, INT _itexHeight )
{
	//	Set NumVertex, NumIndex
	_TileImage.iVertices	= _iVertices;
	_TileImage.iIndices		= _iIndices;

	//	Set VertexBuffer
	CreateVB( &_TileImage.pVB, _iVertices, sizeof( TILEVERTEX ), TILEVERTEX::FVF );
	SetVB( _TileImage.pVB, _pVertex, _iVertices, sizeof( TILEVERTEX ) );

	//	Set IndexBuffer
	CreateIB( &_TileImage.pIB, _iIndices, sizeof( INDEX ) );
	SetIB( _TileImage.pIB, _pIndex, _iIndices, sizeof( INDEX ) );

	//	Set Texture
	m_pd3dDevice->CreateTexture( _itexWidth, _itexHeight, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &_TileImage.pTexture, NULL );

	D3DLOCKED_RECT d3drc;
	_TileImage.pTexture->LockRect( 0, &d3drc, NULL, 0 );

	UINT* pByte = static_cast<UINT*>( d3drc.pBits );
	memset( pByte, 0xffffffff, sizeof( UINT ) * _itexWidth * _itexHeight );

	_TileImage.pTexture->UnlockRect( 0 );	
}

VOID TileMapBase::CreateLineImage( IMAGE& _LineImage, INT _iVertices, LPLINEVERTEX _pVertex )
{
	//	Set NumVertex, NumIndex
	_LineImage.iVertices	= _iVertices / 2;

	//	Set VertexBuffer
	CreateVB( &_LineImage.pVB, _iVertices, sizeof( LINEVERTEX ), LINEVERTEX::FVF );
	SetVB( _LineImage.pVB, _pVertex, _iVertices, sizeof( LINEVERTEX ) );
}

VOID TileMapBase::RenderTileImage( LPIMAGE _pTileImage )
{
	if( _pTileImage->pTexture != NULL )
		m_pd3dDevice->SetTexture( 0, _pTileImage->pTexture );
	//	Draw
	m_pd3dDevice->SetStreamSource( 0, _pTileImage->pVB, 0, sizeof( TILEVERTEX ) );
	m_pd3dDevice->SetFVF( TILEVERTEX::FVF );
	m_pd3dDevice->SetIndices( _pTileImage->pIB );
	m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, _pTileImage->iVertices, 0, _pTileImage->iIndices );

	m_pd3dDevice->SetTexture( 0, NULL );
}

VOID TileMapBase::RenderLineImage( LPIMAGE _pLineImage )
{
	m_pd3dDevice->SetStreamSource( 0, _pLineImage->pVB, 0, sizeof( LINEVERTEX ) );
	m_pd3dDevice->SetFVF( LINEVERTEX::FVF );
	m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, _pLineImage->iVertices );
}

VOID TileMapBase::ChangePixel( LPDIRECT3DTEXTURE9 _pTexture, INT _iX, INT _iY, DWORD _dColor )
{
	if( _pTexture == NULL )
	{
		MessageBox( NULL, L"TileMapBase::ChangePixel( ... _pTexture == NULL, ...  )", NULL, MB_OK );
		return;
	}

	D3DSURFACE_DESC ddsd;
	_pTexture->GetLevelDesc( 0, &ddsd );

	INT itexWidth	= ddsd.Width;
	INT itexHeight	= ddsd.Height;

	D3DLOCKED_RECT d3drc;
	_pTexture->LockRect( 0, &d3drc, NULL, 0 );

	UINT* pByte =	static_cast<UINT*>( d3drc.pBits );
	pByte		+=	_iX + _iY * itexWidth;
	(*pByte)	=	_dColor;

	_pTexture->UnlockRect( 0 );
}

HRESULT	TileMapBase::CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF )
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
HRESULT	TileMapBase::SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size )
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

HRESULT	TileMapBase::CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size )
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

HRESULT	TileMapBase::SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size )
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