/**
@class	Axis
@date	2011/08/26
@author	김종철
@brief	좌표축
*/

#include "stdafx.h"
#include "Axis.h"

/**
@input	D3D 디바이스
@return	VOID
@brief	생성자
*/
Axis::Axis( LPDIRECT3DDEVICE9 _pd3dDevice )
: m_pD3dDevice(_pd3dDevice)
, m_pVB(NULL)
, m_fSize(1000)
{
}

/**
@input	VOID
@return	VOID
@brief	소멸자
*/
Axis::~Axis()
{
	Release();
}

/**
@input	VOID
@return	생성결과
@brief	초기화 및 정점 버퍼 생성
*/
HRESULT Axis::Create()
{
	return InitVB();
}

HRESULT Axis::Create( FLOAT fRange, INT nDeep )
{
	m_fSize = fRange;
	m_nDeep = nDeep;

	return Create();
}

/**
@input	VOID
@return	VOID
@brief	오브젝트들 제거
*/
HRESULT Axis::Release()
{
	SAFE_RELEASE( m_pVB );
	return S_OK;
}


/**
@input	VOID
@return	생성 결과
@brief	정점 버퍼를 생성한다.
*/

HRESULT Axis::InitVB()
{
	INT nSize = (int)(pow( 2.0f, m_nDeep ) + 0.5f) + 1;
	UINT uLength = nSize * 4 * sizeof( VERTEX );
	// 정점 버퍼 생성
	if ( FAILED( m_pD3dDevice->CreateVertexBuffer( uLength, 0, VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL ) ) )
	{
		Release();
		return E_FAIL;
	}

	VERTEX* pVertices = NULL;
	if ( FAILED( m_pVB->Lock( 0, uLength, ( VOID** )&pVertices, 0 ) ) )
	{
		Release();
		return E_FAIL;
	}

	FLOAT fX, fY, fZ;
	FLOAT fStep = m_fSize / ( ( nSize - 1 ) / 2 );
	fX = -m_fSize;
	fZ = -m_fSize;
	fY = 10.0f;

	int i= 0;
	while ( fX <= m_fSize )
	{
		pVertices[            i + 0]._pos = D3DXVECTOR3( fX, fY, m_fSize );			pVertices[            i + 0]._col = 0xFF00FF00;
		pVertices[            i + 1]._pos = D3DXVECTOR3( fX, fY,-m_fSize ); 		pVertices[            i + 1]._col = 0xFF00FF00;
		pVertices[nSize * 2 + i + 0]._pos = D3DXVECTOR3( m_fSize, fY, fZ ); 		pVertices[nSize * 2 + i + 0]._col = 0xFF00FF00;
		pVertices[nSize * 2 + i + 1]._pos = D3DXVECTOR3(-m_fSize, fY, fZ );			pVertices[nSize * 2 + i + 1]._col = 0xFF00FF00;

		fX += fStep;
		fZ += fStep;

		i+=2;

	}
	m_pVB->Unlock();

	return S_OK;
}

HRESULT Axis::InitVB2()
{
	FLOAT fSize = pow( 2.0f, m_nDeep );
	
	UINT uLength = 40 * sizeof( VERTEX );
	// 정점 버퍼 생성
	if ( FAILED( m_pD3dDevice->CreateVertexBuffer( uLength, 0, VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL ) ) )
	{
		Release();
		return E_FAIL;
	}

	VERTEX* pVertices = NULL;
	if ( FAILED( m_pVB->Lock( 0, 40 * sizeof( VERTEX ), ( VOID** )&pVertices, 0 ) ) )
	{
		Release();
		return E_FAIL;
	}

	// X 축
	float fStep = 100.0f;
	for (int i = 0; i < 20; i+=2)
	{
		pVertices[i+0]._pos = D3DXVECTOR3(-1000.0f + fStep * i, 100.0f,-1000.0f ); pVertices[i]._col = 0xFF00FF00;
		pVertices[i+1]._pos = D3DXVECTOR3(-1000.0f + fStep * i, 100.0f, 1000.0f ); pVertices[i+1]._col = 0xFF00FF00;
	}
	for (int i = 0; i < 20; i+=2)
	{
		pVertices[20+i+0]._pos = D3DXVECTOR3(-1000.0f, 100.0f,-1000.0f + fStep * i ); pVertices[20+i]._col = 0xFF00FF00;
		pVertices[20+i+1]._pos = D3DXVECTOR3( 1000.0f, 100.0f,-1000.0f + fStep * i ); pVertices[20+i+1]._col = 0xFF00FF00;
	}

	m_pVB->Unlock();

	return S_OK;
}

/**
@input	VOID
@return	VOID
@brief	Update
*/
VOID Axis::Update()
{

}


/**
@input	VOID
@return	VOID
@brief	Draw
*/
VOID Axis::Render()
{
	INT nSize = (int)(pow( 2.0f, m_nDeep ) + 0.5f) + 1;
	UINT uLength = nSize * 4 * sizeof( VERTEX );
	
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	m_pD3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	m_pD3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof( VERTEX ) );
	m_pD3dDevice->SetFVF( VERTEX::FVF );

	m_pD3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, nSize * 2 );
}