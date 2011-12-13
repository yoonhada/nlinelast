/**
	@file	Cube.cpp
	@date	2011/09/17
	@author	백경훈
	@brief	큐브 클래스
*/

#include "stdafx.h"
#include "Cube24Vertex.h"

#ifdef _24CUBE

CCube::CCube()
{
	Clear();
}

CCube::~CCube()
{
	Release();
}

VOID CCube::Clear()
{
	m_pD3dDevice = NULL ;
	m_pTexture = NULL;

	m_iStartVB = 0;
	m_iStartIB = 0;
	m_fCubeSize = 0.0f;
}

HRESULT CCube::Create( LPDIRECT3DDEVICE9 a_pD3dDevice, 
					  LPDIRECT3DVERTEXBUFFER9 &a_pVB, LPDIRECT3DINDEXBUFFER9 &a_pIB, 
					  INT a_iStartVB, INT a_iStartIB, FLOAT a_fCubeSize )
{
	m_pD3dDevice = a_pD3dDevice;
	m_iStartVB = a_iStartVB;
	m_iStartIB = a_iStartIB;
	m_fCubeSize = a_fCubeSize;
	_InitVB( a_pVB );
	_InitIB( a_pIB );

	return S_OK;
}

HRESULT CCube::Release()
{
	/*if(m_pVB != NULL)
	{
		m_pVB->Release();
		m_pVB=NULL;
	}

	if(m_pIB != NULL)
	{
		m_pIB->Release();
		m_pIB=NULL;
	}*/

	if(m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture=NULL;
	}

	return S_OK;
}

HRESULT CCube::InitTexture( DWORD a_Color, DWORD a_OutLineColor )
{

	/*if( FAILED( D3DXCreateTextureFromFile( GD3D9DEVICE, L"Media/TerrainTexture/test.bmp", &m_pTexture ) ) )
	{
		return E_FAIL;
	}*/

	m_Mtrl.Diffuse = D3DXCOLOR( a_Color );
	m_Mtrl.Ambient = D3DXCOLOR( a_Color );
	m_Mtrl.Power = 1.0f;
	m_Mtrl.Emissive = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.0f );
	m_Mtrl.Specular = D3DXCOLOR( a_Color );

	//LPBYTE pData;	
	//LPDWORD pDWord;
	//D3DLOCKED_RECT Texture_Locked;
	//INT TextureSize = 8;


	//if( m_pTexture != NULL )
	//{
	//	m_pTexture->Release();
	//	m_pTexture=NULL;
	//}
	//if( FAILED( m_pD3dDevice->CreateTexture( TextureSize, TextureSize, 1, 0, D3DFMT_A8R8G8B8, 
	//	D3DPOOL_MANAGED, &m_pTexture, NULL ) ) )
	//{	
	//	return E_FAIL;	
	//}

	//memset( &Texture_Locked, 0, sizeof(D3DLOCKED_RECT) );
	//if( FAILED(m_pTexture->LockRect(0, &Texture_Locked, NULL, 0)) )
	//{	
	//	return E_FAIL;	
	//}

	//pData = (LPBYTE)Texture_Locked.pBits;
	//for(INT iLoopY=0; iLoopY<TextureSize; ++iLoopY)
	//{
	//	pDWord = LPDWORD(pData + iLoopY * Texture_Locked.Pitch);
	//	for(INT iLoopX=0; iLoopX<TextureSize; ++iLoopX)
	//	{	
	//		//외곽선 처리
	//		if( iLoopX == 0 || iLoopX == TextureSize-1 || iLoopY == 0 || iLoopY == TextureSize-1 )
	//		{
	//			*(pDWord + iLoopX) = a_OutLineColor;
	//		}
	//		else
	//		{
	//			*(pDWord + iLoopX) = a_Color;
	//		}
	//	}
	//}

	//if( FAILED(m_pTexture->UnlockRect(0)) )	
	//{	
	//	return E_FAIL;	
	//}
	

	return S_OK;
}

HRESULT CCube::_InitVB( LPDIRECT3DVERTEXBUFFER9 &a_pVB )
{

	//InitTexture( a_Color );
	

	/*if( FAILED( GD3D9DEVICE->CreateVertexBuffer( CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ),
		0, CUBEVERTEX::FVF, D3DPOOL_MANAGED, &m_pVB, NULL ) ) )
	{
		CleanUp();
		return E_FAIL;
	}*/

	//    v0----- v1
	//   /|      /|
	//  v3------v2|
	//  | |     | |
	//  | |v4---|-|v5
	//  |/      |/
	//  v7------v6

	FLOAT fSize = m_fCubeSize;
	D3DXVECTOR3 v0 = D3DXVECTOR3( -fSize,  fSize,  fSize );
	D3DXVECTOR3 v1 = D3DXVECTOR3(  fSize,  fSize,  fSize );
	D3DXVECTOR3 v2 = D3DXVECTOR3(  fSize,  fSize, -fSize );
	D3DXVECTOR3 v3 = D3DXVECTOR3( -fSize,  fSize, -fSize );
	D3DXVECTOR3 v4 = D3DXVECTOR3( -fSize, -fSize,  fSize );
	D3DXVECTOR3 v5 = D3DXVECTOR3(  fSize, -fSize,  fSize );
	D3DXVECTOR3 v6 = D3DXVECTOR3(  fSize, -fSize, -fSize );
	D3DXVECTOR3 v7 = D3DXVECTOR3( -fSize, -fSize, -fSize );

	FLOAT fNom = 1.0f;
	D3DXVECTOR3 nTop	= D3DXVECTOR3(  0.0f,   fNom,  0.0f );
	D3DXVECTOR3 nBottom = D3DXVECTOR3(  0.0f,  -fNom,  0.0f );
	D3DXVECTOR3 nFront	= D3DXVECTOR3(  0.0f,   0.0f, -fNom );
	D3DXVECTOR3 nBack	= D3DXVECTOR3(  0.0f,   0.0f,  fNom );
	D3DXVECTOR3 nLeft	= D3DXVECTOR3( -fNom,   0.0f,  0.0f );
	D3DXVECTOR3 nRight	= D3DXVECTOR3(  fNom,   0.0f,  0.0f );

	D3DXVECTOR2 tTL = D3DXVECTOR2( 0.0f, 0.0f );
	D3DXVECTOR2 tTR = D3DXVECTOR2( 1.0f, 0.0f );
	D3DXVECTOR2 tBL = D3DXVECTOR2( 1.0f, 1.0f );
	D3DXVECTOR2 tBR = D3DXVECTOR2( 0.0f, 1.0f );

	CUBEVERTEX* pVertices = NULL;

	if( FAILED( a_pVB->Lock( m_iStartVB * sizeof( CUBEVERTEX ), CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ), (VOID**)&pVertices, 0 ) ) )
	{
		Release();
		return E_FAIL;
	}

	CUBEVERTEX vertices[CUBEVERTEX::VertexNum] = {

		//Top------------------------------------------------------

		// 0
		{ v0, 
		  //a_Color,
		  nTop, 
		  tTL
		},	

		// 1
		{ v1, 
		 // a_Color,
		  nTop, 
		  tTR
		},	

		// 2
		{ v2, 
		  //a_Color,
		  nTop, 
		  tBL
		},	

		// 3
		{ v3, 
		  //a_Color,
		  nTop, 
		  tBR
		},	

		//Bottom------------------------------------------------------

		// 4
		{ v4, 
		  //a_Color,
		  nBottom, 
		  tTL
		},	

		// 5
		{ v5, 
		  //a_Color,
		  nBottom, 
		  tTR
		},	

		// 6
		{ v6, 
		  //a_Color,
		  nBottom, 
		  tBL
		},	

		// 7
		{ v7, 
		  //a_Color,
		  nBottom, 
		  tBR
		},	

		//Front------------------------------------------------------

		// 8
		{ v3, 
		  //a_Color,
		  nFront, 
		  tTL
		},	

		// 9
		{ v2, 
		  //a_Color,
		  nFront, 
		  tTR
		},	

		// 10
		{ v6, 
		  //a_Color,
		  nFront, 
		  tBL
		},	

		// 11
		{ v7, 
		  //a_Color,
		  nFront, 
		  tBR
		},	

		//Back------------------------------------------------------

		// 12
		{ v0, 
		  //a_Color,
		  nBack, 
		  tTL
		},	

		// 13
		{ v1, 
		  //a_Color,
		  nBack, 
		  tTR
		},	

		// 14
		{ v4, 
		  //a_Color,
		  nBack, 
		  tBR
		},	

		// 15
		{ v5, 
		  //a_Color,
		  nBack, 
		  tBL
		},	

		//Left------------------------------------------------------

		// 16
		{ v0, 
		  //a_Color,
		  nLeft, 
		  tTL
		},	

		// 17
		{ v3, 
		  //a_Color,
		  nLeft, 
		  tTR
		},	

		// 18
		{ v7, 
		  //a_Color,
		  nLeft, 
		  tBL
		},	

		// 19
		{ v4, 
		  //a_Color,
		  nLeft, 
		  tBR
		},	

		//Right------------------------------------------------------

		// 20
		{ v2, 
		  //a_Color,
		  nRight, 
		  tTL
		},	

		// 21
		{ v1, 
		  //a_Color,
		  nRight, 
		  tTR
		},	

		// 22
		{ v5, 
		  //a_Color,
		  nRight, 
		  tBL
		},	

		// 23
		{ v6, 
		  //a_Color,
		  nRight, 
		  tBR
		}	

	};	

	memcpy( pVertices, vertices, CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ) );
	a_pVB->Unlock();

	return S_OK;
}

HRESULT CCube::_InitIB( LPDIRECT3DINDEXBUFFER9 &a_pIB )
{
	/*if( FAILED( GD3D9DEVICE->CreateIndexBuffer( 12 * sizeof( CUBEINDEX ),
		0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL ) ) )
	{
		CleanUp();
		return E_FAIL;
	}*/

	VOID* pIndices = NULL;

	if( FAILED( a_pIB->Lock( m_iStartIB, 12 * sizeof( CUBEINDEX ), (VOID**)&pIndices, 0 ) ) )
	{
		Release();
		return E_FAIL;
	}

	CUBEINDEX i[12] = 
	{	
		//TOP
		{ 0, 1, 3 },	{ 1, 2, 3 },
		//BOTTOM
		{ 7, 5, 4 },	{ 7, 6, 5 },

		//FRONT
		{ 8, 9, 11 },	{ 9, 10, 11 },
		//BACK
		{ 13, 14, 15 },	{ 13, 12, 14 },

		//LEFT
		{ 16, 17, 19 },	{ 17, 18, 19 },
		//RIGHT
		{ 20, 21, 23 },	{ 21, 22, 23 }
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
	m_pD3dDevice->SetMaterial( &m_Mtrl );

	//m_pD3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pD3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	//m_pD3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pD3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);

	//m_pD3dDevice->SetTexture( 0, m_pTexture );
	//m_pD3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//m_pD3dDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	//m_pD3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	//m_pD3dDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	
	//m_pD3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof( CUBEVERTEX ) );
	//m_pD3dDevice->SetFVF( CUBEVERTEX::FVF );
	//m_pD3dDevice->SetIndices( m_pIB );
	m_pD3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, m_iStartVB, 0, CUBEVERTEX::VertexNum, 0, 12 );

	//m_pD3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pD3dDevice->SetTexture( 0, NULL );
}

#endif