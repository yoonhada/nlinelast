#include "stdafx.h"
#include "Cube8Vertex.h"

#ifdef _8CUBE

CCube::CCube()
{
	Clear();
}

CCube::~CCube()
{
}

VOID CCube::Clear()
{
	m_pD3dDevice = NULL;
	m_Color = 0xFFFFFFFF;
}

HRESULT CCube::Create( LPDIRECT3DDEVICE9 a_pD3dDevice,
					  LPDIRECT3DVERTEXBUFFER9 &a_pVB, LPDIRECT3DINDEXBUFFER9 &a_pIB, 
					  INT a_iStartVB, INT a_iStartIB, FLOAT a_fCubeSize )
{
	m_pD3dDevice = a_pD3dDevice;
	m_iStartVB = a_iStartVB;
	m_iStartIB = a_iStartIB;
	_InitVB( a_pVB );
	_InitIB( a_pIB );

	return S_OK;
}

HRESULT CCube::Release()
{
	return S_OK;
}

HRESULT CCube::InitTexture( DWORD a_Color, DWORD a_OutLineColor )
{
	m_Color = a_Color;

	return S_OK;
}

HRESULT CCube::_InitVB( LPDIRECT3DVERTEXBUFFER9 &a_pVB )
{

	//    v0----- v1
	//   /|      /|
	//  v3------v2|
	//  | |     | |
	//  | |v4---|-|v5
	//  |/      |/
	//  v7------v6

	FLOAT fSize = 0.5f;
	D3DXVECTOR3 v0 = D3DXVECTOR3( -fSize,  fSize,  fSize );
	D3DXVECTOR3 v1 = D3DXVECTOR3(  fSize,  fSize,  fSize );
	D3DXVECTOR3 v2 = D3DXVECTOR3(  fSize,  fSize, -fSize );
	D3DXVECTOR3 v3 = D3DXVECTOR3( -fSize,  fSize, -fSize );
	D3DXVECTOR3 v4 = D3DXVECTOR3( -fSize, -fSize,  fSize );
	D3DXVECTOR3 v5 = D3DXVECTOR3(  fSize, -fSize,  fSize );
	D3DXVECTOR3 v6 = D3DXVECTOR3(  fSize, -fSize, -fSize );
	D3DXVECTOR3 v7 = D3DXVECTOR3( -fSize, -fSize, -fSize );

	CUBEVERTEX* pVertices = NULL;

	if( FAILED( a_pVB->Lock( m_iStartVB * sizeof( CUBEVERTEX ), CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ), (VOID**)&pVertices, 0 ) ) )
	{
		Release();
		return E_FAIL;
	}

	CUBEVERTEX vertices[8] = {

		//Top------------------------------------------------------

		// 0
		{ v0, 
		  m_Color
		},	

		// 1
		{ v1, 
		 m_Color
		},	

		// 2
		{ v2, 
		  m_Color
		},	

		// 3
		{ v3, 
		  m_Color
		},	

		// 4
		{ v4, 
		  m_Color
		},	

		// 5
		{ v5, 
		  m_Color
		},	

		// 6
		{ v6, 
		  m_Color
		},	

		// 7
		{ v7, 
		  m_Color
		}

	};	

	memcpy( pVertices, vertices, CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ) );
	a_pVB->Unlock();

	return S_OK;
}

HRESULT CCube::_InitIB( LPDIRECT3DINDEXBUFFER9 &a_pIB )
{

	VOID* pIndices = NULL;

	if( FAILED( a_pIB->Lock( m_iStartIB, 12 * sizeof( CUBEINDEX ), (VOID**)&pIndices, 0 ) ) )
	{
		Release();
		return E_FAIL;
	}

	//    v0----- v1
	//   /|      /|
	//  v3------v2|
	//  | |     | |
	//  | |v4---|-|v5
	//  |/      |/
	//  v7------v6

	CUBEINDEX i[12] = 
	{	
		//TOP
		{ 0, 1, 3 },	{ 1, 2, 3 },
		//BOTTOM
		{ 7, 5, 4 },	{ 7, 6, 5 },

		//FRONT
		{ 3, 2, 7 },	{ 2, 6, 7 },
		//BACK
		{ 1, 4, 5 },	{ 1, 0, 4 },

		//LEFT
		{ 0, 3, 4 },	{ 3, 7, 4 },
		//RIGHT
		{ 2, 1, 6 },	{ 1, 5, 6 }
	};

	memcpy( pIndices, i, 12 * sizeof( CUBEINDEX ) );
	a_pIB->Unlock();

	return S_OK;
}

VOID CCube::Update()
{

}

VOID CCube::Render()
{
	m_pD3dDevice->SetTexture( 0, NULL );
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pD3dDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	m_pD3dDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pD3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	
	
	m_pD3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, m_iStartVB, 0, CUBEVERTEX::VertexNum, 0, 12 );
	
}

#endif