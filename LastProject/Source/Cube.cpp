/**
	@file	Cube.cpp
	@date	2011/09/17
	@author	백경훈
	@brief	큐브 클래스
*/

#include "stdafx.h"
#include "Cube.h"

#ifdef _CELLCUBE

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

	m_pCell = NULL;
}

HRESULT CCube::Create( LPDIRECT3DDEVICE9 a_pD3dDevice, 
					  LPDIRECT3DVERTEXBUFFER9 &a_pVB, LPDIRECT3DINDEXBUFFER9 &a_pIB, 
					  INT a_iStartVB, INT a_iStartIB, FLOAT a_fCubeSize )
{
	m_pD3dDevice = a_pD3dDevice;
	m_iStartVB = a_iStartVB;
	m_iStartIB = a_iStartIB;
	m_fCubeSize = a_fCubeSize;
	//_InitVB( a_pVB );
	//_InitIB( a_pIB );

	m_pCell = new CCell[6];
	for(INT Loop=0; Loop<6; ++Loop)
	{
		m_pCell[Loop].Create( m_pD3dDevice, a_pVB, a_pIB, 
			m_iStartVB + ( Loop * 4 ), 0, m_fCubeSize, Loop );
	}

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

	SAFE_DELETE_ARRAY( m_pCell );

	return S_OK;
}

HRESULT CCube::InitTexture( DWORD a_Color, DWORD a_OutLineColor )
{

	/*if( FAILED( D3DXCreateTextureFromFile( GD3D9DEVICE, L"Media/TerrainTexture/test.bmp", &m_pTexture ) ) )
	{
		return E_FAIL;
	}*/

	LPBYTE pData;	
	LPDWORD pDWord;
	D3DLOCKED_RECT Texture_Locked;
	INT TextureSize = 8;


	if( m_pTexture != NULL )
	{
		m_pTexture->Release();
		m_pTexture=NULL;
	}
	if( FAILED( m_pD3dDevice->CreateTexture( TextureSize, TextureSize, 1, 0, D3DFMT_A8R8G8B8, 
		D3DPOOL_MANAGED, &m_pTexture, NULL ) ) )
	{	
		return E_FAIL;	
	}

	memset( &Texture_Locked, 0, sizeof(D3DLOCKED_RECT) );
	if( FAILED(m_pTexture->LockRect(0, &Texture_Locked, NULL, 0)) )
	{	
		return E_FAIL;	
	}

	pData = (LPBYTE)Texture_Locked.pBits;
	for(INT iLoopY=0; iLoopY<TextureSize; ++iLoopY)
	{
		pDWord = LPDWORD(pData + iLoopY * Texture_Locked.Pitch);
		for(INT iLoopX=0; iLoopX<TextureSize; ++iLoopX)
		{	
			//외곽선 처리
			if( iLoopX == 0 || iLoopX == TextureSize-1 || iLoopY == 0 || iLoopY == TextureSize-1 )
			{
				*(pDWord + iLoopX) = a_OutLineColor;
			}
			else
			{
				*(pDWord + iLoopX) = a_Color;
			}
		}
	}

	if( FAILED(m_pTexture->UnlockRect(0)) )	
	{	
		return E_FAIL;	
	}

	return S_OK;
}



VOID CCube::Update()
{

}

VOID CCube::Render()
{
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
	//m_pD3dDevice->SetTexture( 0, m_pTexture );
	for(INT Loop=0; Loop<6; ++Loop)
	{
		//CMatrices::GetInstance()->SetupModeltoWorld( m_pCell[Loop].Get_MatWorld() );
		if( m_pCell[Loop].Get_FaceVisble() == TRUE )
		{
			m_pCell[Loop].Render();
		}
	}
	//m_pD3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, m_iStartVB, 0, CUBEVERTEX::VertexNum, 0, 12 );

	//m_pD3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pD3dDevice->SetTexture( 0, NULL );
	
}

#endif