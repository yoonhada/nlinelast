/**
	@file	CCell.cpp
	@date	2011/09/23
	@author	백경훈
	@brief	큐브 면 클래스
*/

#include "stdafx.h"
#include "Cell.h"

#ifdef _CELLCUBE

CCell::CCell()
{
	Clear();
}

CCell::~CCell()
{
	Release();
}

VOID CCell::Clear()
{
	m_pD3dDevice = NULL ;

	m_iStartVB = 0;
	m_iStartIB = 0;
	m_fCubeSize = 0.0f;
	m_iFaceNum = 0;
	m_iVisible = TRUE;
}

HRESULT CCell::Create( LPDIRECT3DDEVICE9 a_pD3dDevice, 
					  LPDIRECT3DVERTEXBUFFER9 &a_pVB, LPDIRECT3DINDEXBUFFER9 &a_pIB, 
					  INT a_iStartVB, INT a_iStartIB, FLOAT a_fCubeSize, INT a_iFaceNum )
{
	m_pD3dDevice = a_pD3dDevice;
	m_iStartVB = a_iStartVB;
	m_iStartIB = a_iStartIB;
	m_fCubeSize = a_fCubeSize;
	m_iFaceNum = a_iFaceNum;
	_InitVB( a_pVB );
	_InitIB( a_pIB );

	return S_OK;
}

HRESULT CCell::Release()
{
	return S_OK;
}

HRESULT CCell::_InitVB( LPDIRECT3DVERTEXBUFFER9 &a_pVB )
{

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

	INT test = CUBEVERTEX::VertexNum;

	if( FAILED( a_pVB->Lock( m_iStartVB * sizeof( CUBEVERTEX ), CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ), (VOID**)&pVertices, 0 ) ) )
	{
		Release();
		return E_FAIL;
	}

	CUBEVERTEX verticesLast[CUBEVERTEX::VertexNum];

	if( m_iFaceNum == EnumCubeCellFace::TOP )
	{
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
				  }
		  };
			  memcpy( verticesLast, vertices, CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ) );
	}
	else if( m_iFaceNum == EnumCubeCellFace::BOTTOM )
	{
			  CUBEVERTEX vertices[CUBEVERTEX::VertexNum] = {
				  //Bottom------------------------------------------------------

				  // 4
				  { v7, 
				  //a_Color,
				  nBottom, 
				  tTL
				  },	

				  // 5
				  { v6, 
				  //a_Color,
				  nBottom, 
				  tTR
				  },	

				  // 6
				  { v5, 
				  //a_Color,
				  nBottom, 
				  tBL
				  },	

				  // 7
				  { v4, 
				  //a_Color,
				  nBottom, 
				  tBR
				  }
		  };
			  memcpy( verticesLast, vertices, CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ) );
	}
	else if( m_iFaceNum == EnumCubeCellFace::FRONT )
	{
			  CUBEVERTEX vertices[CUBEVERTEX::VertexNum] = {
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
				  }
		  };
			  memcpy( verticesLast, vertices, CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ) );
	}
	else if( m_iFaceNum == EnumCubeCellFace::BACK )
	{
			  CUBEVERTEX vertices[CUBEVERTEX::VertexNum] = {
				  //Back------------------------------------------------------

				  // 12
				  { v1, 
				  //a_Color,
				  nBack, 
				  tTR
				  },	

				  // 13
				  { v0, 
				  //a_Color,
				  nBack, 
				  tTL
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
				  }
		  };
			  memcpy( verticesLast, vertices, CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ) );
	}
	else if( m_iFaceNum == EnumCubeCellFace::LEFT )
	{
			  CUBEVERTEX vertices[CUBEVERTEX::VertexNum] = {
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
				  }
		  };
			  memcpy( verticesLast, vertices, CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ) );
	}
	else if( m_iFaceNum == EnumCubeCellFace::RIGHT )
	{
			  CUBEVERTEX vertices[CUBEVERTEX::VertexNum] = {
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
			  memcpy( verticesLast, vertices, CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ) );
	}

	memcpy( pVertices, verticesLast, CUBEVERTEX::VertexNum * sizeof( CUBEVERTEX ) );
	a_pVB->Unlock();

	return S_OK;
}

HRESULT CCell::_InitIB( LPDIRECT3DINDEXBUFFER9 &a_pIB )
{

	//    v0----- v1
	//   /|      /|
	//  v3------v2|
	//  | |     | |
	//  | |v4---|-|v5
	//  |/      |/
	//  v7------v6

	CUBEINDEX* pIndices = NULL;

	if( FAILED( a_pIB->Lock( m_iStartIB, 2 * sizeof( CUBEINDEX ), (VOID**)&pIndices, 0 ) ) )
	{
		Release();
		return E_FAIL;
	}

	CUBEINDEX i[2] = 
	{
		{ 0, 1, 3 },	{ 1, 2, 3 }
	};

	memcpy( pIndices, i, 2 * sizeof( CUBEINDEX ) );
	a_pIB->Unlock();

	return S_OK;
}

VOID CCell::Render()
{
	m_pD3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, m_iStartVB, 0, CUBEVERTEX::VertexNum, 0, 2 );
}

#endif