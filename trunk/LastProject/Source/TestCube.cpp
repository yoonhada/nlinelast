/**
@class	Axis
@date	2011/08/26
@author	����ö
@brief	��ǥ��
*/

#include "stdafx.h"
#include "TestCube.h"

/**
@input	D3D ����̽�
@return	VOID
@brief	������
*/
TestCube::TestCube( LPDIRECT3DDEVICE9 _pd3dDevice )
: m_pD3dDevice(_pd3dDevice)
, m_pVB(NULL)
, m_pIB(NULL)
{
}

/**
@input	VOID
@return	VOID
@brief	�Ҹ���
*/
TestCube::~TestCube()
{
	Release();
}

/**
@input	VOID
@return	�������
@brief	�ʱ�ȭ �� ���� ���� ����
*/
HRESULT TestCube::Create()
{
	InitVB();
	InitIB();

	return S_OK;
}


/**
@input	VOID
@return	VOID
@brief	������Ʈ�� ����
*/
HRESULT TestCube::Release()
{
	SAFE_RELEASE( m_pVB );
	SAFE_RELEASE( m_pIB );

	return S_OK;
}


/**
@input	VOID
@return	���� ���
@brief	���� ���۸� �����Ѵ�.
*/

HRESULT TestCube::InitVB()
{
	VERTEX vertices[] =
	{
		{ D3DXVECTOR3( -10.0f,  10.0f,  10.0f ), 0xffff0000 },
		{ D3DXVECTOR3(  10.0f,  10.0f,  10.0f ), 0xffff0000 },
		{ D3DXVECTOR3(  10.0f,  10.0f, -10.0f ), 0xffff0000 },
		{ D3DXVECTOR3( -10.0f,  10.0f, -10.0f ), 0xffff0000 },

		{ D3DXVECTOR3( -10.0f, -10.0f,  10.0f ), 0xffff0000 },
		{ D3DXVECTOR3(  10.0f, -10.0f,  10.0f ), 0xffff0000 },
		{ D3DXVECTOR3(  10.0f, -10.0f, -10.0f ), 0xffff0000 },
		{ D3DXVECTOR3( -10.0f, -10.0f, -10.0f ), 0xffff0000 },
	};

	// ���� ���� ����
	if ( FAILED( m_pD3dDevice->CreateVertexBuffer( 8 * sizeof( VERTEX ), 0, VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL ) ) )
	{
		Release();
		return E_FAIL;
	}

	VERTEX* pVertices = NULL;
	if ( FAILED( m_pVB->Lock( 0, sizeof( vertices ), ( VOID** )&pVertices, 0 ) ) )
	{
		Release();
		return E_FAIL;
	}
	
	memcpy( pVertices, vertices, sizeof( vertices ) );
	
	m_pVB->Unlock();

	return S_OK;
}


HRESULT TestCube::InitIB()
{
	INDEX indices[] =
	{
		{ 0, 1, 2 }, { 0, 2, 3 },
		{ 4, 6, 5 }, { 4, 7, 6 },
		{ 0, 3, 7 }, { 0, 7, 4 },
		{ 1, 5, 6 }, { 1, 6, 2 },
		{ 3, 2, 6 }, { 3, 6, 7 },
		{ 0, 4, 5 }, { 0, 5, 1 },
	};

	if( FAILED( m_pD3dDevice->CreateIndexBuffer( 12 * sizeof( INDEX ), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL ) ) )
	{
		return E_FAIL;
	}

	VOID* pIndices;
	if( FAILED( m_pIB->Lock( 0, sizeof( indices ), (VOID**)&pIndices, 0 ) ) )
	{
		return E_FAIL;
	}

	memcpy( pIndices, indices, sizeof( indices ) );

	m_pIB->Unlock();

	return S_OK;
}


/**
@input	VOID
@return	VOID
@brief	Update
*/
VOID TestCube::Update()
{

}


/**
@input	VOID
@return	VOID
@brief	Draw
*/
VOID TestCube::Render()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	m_pD3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	m_pD3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof( VERTEX ) );
	m_pD3dDevice->SetFVF( VERTEX::FVF );
	m_pD3dDevice->SetIndices( m_pIB );

	m_pD3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12 );
}