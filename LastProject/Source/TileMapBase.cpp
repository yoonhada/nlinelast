#include "stdafx.h"
#include "TileMapBase.h"

VOID TileMapBase::Initialize()
{
}

VOID TileMapBase::Release()
{
}

VOID TileMapBase::CreateTileImage( IMAGE& _TileImage, INT _iNumVertex, LPTILEVERTEX _pVertex, INT _iNumIndex, LPINDEX _pIndex, INT _iWidth, INT _iHeight )
{
	//	Set NumVertex, NumIndex
	_TileImage.iNumVertex	= _iNumVertex;
	_TileImage.iNumIndex	= _iNumIndex;

	//	Set VertexBuffer
	CreateVB( &_TileImage.pVB, _iNumVertex, sizeof( TILEVERTEX ), TILEVERTEX::FVF );
	SetVB( _TileImage.pVB, _pVertex, _iNumVertex, sizeof( TILEVERTEX ) );

	//	Set IndexBuffer
	CreateIB( &_TileImage.pIB, _iNumIndex, sizeof( INDEX ) );
	SetIB( _TileImage.pIB, _pIndex, _iNumIndex, sizeof( INDEX ) );

	//	Set Texture
	m_pd3dDevice->CreateTexture( ( _iWidth - 1 ), ( _iHeight - 1 ), 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &_TileImage.pTexture, NULL );
	
	D3DLOCKED_RECT d3drc;
	_TileImage.pTexture->LockRect( 0, &d3drc, NULL, 0 );
	
	UINT* pByte = static_cast<UINT*>( d3drc.pBits );
	memset( pByte, 0xffffffff, sizeof( UINT ) * _iWidth * _iHeight );

	_TileImage.pTexture->UnlockRect( 0 );	
}

VOID TileMapBase::CreateLineImage( IMAGE& _LineImage, INT _iNumVertex, LPLINEVERTEX _pVertex )
{
	//	Set NumVertex, NumIndex
	_LineImage.iNumVertex	= _iNumVertex / 2;

	//	Set VertexBuffer
	CreateVB( &_LineImage.pVB, _iNumVertex, sizeof( LINEVERTEX ), LINEVERTEX::FVF );
	SetVB( _LineImage.pVB, _pVertex, _iNumVertex, sizeof( LINEVERTEX ) );
}

VOID TileMapBase::RenderTileImage( IMAGE& _TileImage )
{
	if( _TileImage.pTexture != NULL )
		m_pd3dDevice->SetTexture( 0, _TileImage.pTexture );
	//	Draw
	m_pd3dDevice->SetStreamSource( 0, _TileImage.pVB, 0, sizeof( TILEVERTEX ) );
	m_pd3dDevice->SetFVF( TILEVERTEX::FVF );
	m_pd3dDevice->SetIndices( _TileImage.pIB );
	m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, _TileImage.iNumVertex, 0, _TileImage.iNumIndex );

	m_pd3dDevice->SetTexture( 0, NULL );
}

VOID TileMapBase::RenderLineImage( IMAGE& _LineImage )
{
	m_pd3dDevice->SetStreamSource( 0, _LineImage.pVB, 0, sizeof( LINEVERTEX ) );
	m_pd3dDevice->SetFVF( LINEVERTEX::FVF );
	m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, _LineImage.iNumVertex );
}

VOID TileMapBase::ChangePixel( IMAGE& _TileImage, INT _iX, INT _iY, DWORD _dColor )
{
	if( _TileImage.pTexture == NULL )
		return;

	D3DSURFACE_DESC ddsd;
	_TileImage.pTexture->GetLevelDesc( 0, &ddsd );
	INT itexWidth	= ddsd.Width;
	INT itexHeight	= ddsd.Height;

	if( itexWidth < _iX || itexHeight < _iY )
		return;

	D3DLOCKED_RECT d3drc;
	_TileImage.pTexture->LockRect( 0, &d3drc, NULL, 0 );
	UINT* pByte = static_cast<UINT*>( d3drc.pBits );
	pByte += _iX + ( /*ddsd.Height - 1 -*/ _iY ) * ddsd.Width;
	(*pByte) = _dColor;
	_TileImage.pTexture->UnlockRect( 0 );
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