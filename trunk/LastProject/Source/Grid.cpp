/**
	@class	Grid.cpp
	@date	2011/09/17
	@author	백경훈
	@brief	그리드 클래스
*/

#include "stdafx.h"
#include "Grid.h"

CGrid::CGrid()
{
	Clear();
}

CGrid::~CGrid()
{
	Release();
}

VOID CGrid::Clear()
{
	m_pVB = NULL;
}

HRESULT CGrid::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	m_pD3dDevice = a_pD3dDevice;

	if ( S_OK != _InitVB() )
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGrid::Release()
{
	if(m_pVB != NULL)
	{
		m_pVB->Release();
		m_pVB=NULL;
	}

	return S_OK;
}

HRESULT CGrid::_InitVB()
{
	
	FLOAT fLength = 255.0f;

	if( FAILED( m_pD3dDevice->CreateVertexBuffer( 6 * sizeof(GRIDVERTEX), 0,
		GRIDVERTEX::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL) ) )
	{
		Release();
		return E_FAIL;
	}

	GRIDVERTEX* pVertex;

	if( FAILED(m_pVB->Lock( 0, 6 * sizeof(GRIDVERTEX), (VOID**)&pVertex, 0)) )
	{	
		return E_FAIL;	
	}
	
	GRIDVERTEX v[6];

	v[0].Color = 0xffff0000;
	v[0].p = D3DXVECTOR3( -fLength, 0.0f, 0.0f );
	v[1].Color = 0xffff0000;
	v[1].p = D3DXVECTOR3( fLength, 0.0f, 0.0f );

	v[2].Color = 0xff00ff00;
	v[2].p = D3DXVECTOR3( 0.0f, -fLength, 0.0f );
	v[3].Color = 0xff00ff00;
	v[3].p = D3DXVECTOR3( 0.0f, fLength, 0.0f );

	v[4].Color = 0xff0000ff;
	v[4].p = D3DXVECTOR3( 0.0f, 0.0f, -fLength );
	v[5].Color = 0xff0000ff;
	v[5].p = D3DXVECTOR3( 0.0f, 0.0f, fLength );
	
	memcpy( pVertex, v, 6 * sizeof(GRIDVERTEX) );

	m_pVB->Unlock();

	return S_OK;
}

VOID CGrid::Update()
{

}

VOID CGrid::Render()
{
	//픽킹 정점버퍼등록
	m_pD3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(GRIDVERTEX));
	m_pD3dDevice->SetFVF(GRIDVERTEX::FVF);
	//그리기

	m_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pD3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	m_pD3dDevice->SetTexture(0, NULL);

	m_pD3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 3);
	//GD3D9DEVICE->SetRenderState( D3DRS_POINTSIZE, FtoDw(2.0f) );
	//GD3D9DEVICE->DrawPrimitive(D3DPT_POINTLIST, 0, m_iVertexNum);

	m_pD3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	m_pD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}