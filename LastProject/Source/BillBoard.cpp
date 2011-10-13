/**
@file	BillBoard.cpp
@date	2011/09/27
@author	yoonhada@gmail.com
@brief	ºôº¸µå 
*/

#include "stdafx.h"
#include "BillBoard.h"

CBillBoard::_EFFECT CBillBoard::m_pVtx[4] = 
{
	{ D3DXVECTOR3(-1, 0, 0 ), D3DXVECTOR2( 0, 1 ) }, 
	{ D3DXVECTOR3(-1, 4, 0 ), D3DXVECTOR2( 0, 0 ) }, 
	{ D3DXVECTOR3( 1, 0, 0 ), D3DXVECTOR2( 1, 1 ) }, 
	{ D3DXVECTOR3( 1, 4, 0 ), D3DXVECTOR2( 1, 0 ) }, 
};

D3DXVECTOR2 CBillBoard::m_pTexUV[16][4] =
{
	{ D3DXVECTOR2( 0.00f, 0.25f ), D3DXVECTOR2( 0.00f, 0.00f ), D3DXVECTOR2( 0.25f, 0.25f ), D3DXVECTOR2( 0.25f, 0.00f ) },
	{ D3DXVECTOR2( 0.25f, 0.25f ), D3DXVECTOR2( 0.25f, 0.00f ), D3DXVECTOR2( 0.50f, 0.25f ), D3DXVECTOR2( 0.50f, 0.00f ) },
	{ D3DXVECTOR2( 0.50f, 0.25f ), D3DXVECTOR2( 0.50f, 0.00f ), D3DXVECTOR2( 0.75f, 0.25f ), D3DXVECTOR2( 0.75f, 0.00f ) },
	{ D3DXVECTOR2( 0.75f, 0.25f ), D3DXVECTOR2( 0.75f, 0.00f ), D3DXVECTOR2( 1.00f, 0.25f ), D3DXVECTOR2( 1.00f, 0.00f ) },
																															   
	{ D3DXVECTOR2( 0.00f, 0.50f ), D3DXVECTOR2( 0.00f, 0.25f ), D3DXVECTOR2( 0.25f, 0.50f ), D3DXVECTOR2( 0.25f, 0.25f ) },
	{ D3DXVECTOR2( 0.25f, 0.50f ), D3DXVECTOR2( 0.25f, 0.25f ), D3DXVECTOR2( 0.50f, 0.50f ), D3DXVECTOR2( 0.50f, 0.25f ) },
	{ D3DXVECTOR2( 0.50f, 0.50f ), D3DXVECTOR2( 0.50f, 0.25f ), D3DXVECTOR2( 0.75f, 0.50f ), D3DXVECTOR2( 0.75f, 0.25f ) },
	{ D3DXVECTOR2( 0.75f, 0.50f ), D3DXVECTOR2( 0.75f, 0.25f ), D3DXVECTOR2( 1.00f, 0.50f ), D3DXVECTOR2( 1.00f, 0.25f ) },
																															   
	{ D3DXVECTOR2( 0.00f, 0.75f ), D3DXVECTOR2( 0.00f, 0.50f ), D3DXVECTOR2( 0.25f, 0.75f ), D3DXVECTOR2( 0.25f, 0.50f ) },
	{ D3DXVECTOR2( 0.25f, 0.75f ), D3DXVECTOR2( 0.25f, 0.50f ), D3DXVECTOR2( 0.50f, 0.75f ), D3DXVECTOR2( 0.50f, 0.50f ) },
	{ D3DXVECTOR2( 0.50f, 0.75f ), D3DXVECTOR2( 0.50f, 0.50f ), D3DXVECTOR2( 0.75f, 0.75f ), D3DXVECTOR2( 0.75f, 0.50f ) },
	{ D3DXVECTOR2( 0.75f, 0.75f ), D3DXVECTOR2( 0.75f, 0.50f ), D3DXVECTOR2( 1.00f, 0.75f ), D3DXVECTOR2( 1.00f, 0.50f ) },
																															   
	{ D3DXVECTOR2( 0.00f, 1.00f ), D3DXVECTOR2( 0.00f, 0.75f ), D3DXVECTOR2( 0.25f, 1.00f ), D3DXVECTOR2( 0.25f, 0.75f ) },
	{ D3DXVECTOR2( 0.25f, 1.00f ), D3DXVECTOR2( 0.25f, 0.75f ), D3DXVECTOR2( 0.50f, 1.00f ), D3DXVECTOR2( 0.50f, 0.75f ) },
	{ D3DXVECTOR2( 0.50f, 1.00f ), D3DXVECTOR2( 0.50f, 0.75f ), D3DXVECTOR2( 0.75f, 1.00f ), D3DXVECTOR2( 0.75f, 0.75f ) },
	{ D3DXVECTOR2( 0.75f, 1.00f ), D3DXVECTOR2( 0.75f, 0.75f ), D3DXVECTOR2( 1.00f, 1.00f ), D3DXVECTOR2( 1.00f, 0.75f ) },
};


CBillBoard::CBillBoard( LPDIRECT3DDEVICE9 _pd3dDevice )
: m_pd3dDevice( _pd3dDevice )
, m_nType( 0 )
, m_nStep( 0 )
, m_dwTick( 0 )
, m_nLife( 0 )
{
	m_pTexBillboard[0] = m_pTexBillboard[1] = m_pTexBillboard[2] = m_pTexBillboard[3] = NULL;
	Create();
}

CBillBoard::~CBillBoard()
{
	Release();
}

HRESULT CBillBoard::Release()
{
	for( int i = 0 ; i < 4 ; i++ ) 
		SAFE_RELEASE( m_pTexBillboard[i] );
	return S_OK;
}

HRESULT CBillBoard::Create()
{
	Release();
	Clear();
	D3DXCreateTextureFromFile( m_pd3dDevice, L"Bill1.jpg", &m_pTexBillboard[0] );		// Äô
	D3DXCreateTextureFromFile( m_pd3dDevice, L"Bill2.jpg", &m_pTexBillboard[1] );		// ±â´ö
	D3DXCreateTextureFromFile( m_pd3dDevice, L"Bill3.jpg", &m_pTexBillboard[2] );		// ´ö
	D3DXCreateTextureFromFile( m_pd3dDevice, L"Bill4.jpg", &m_pTexBillboard[3] );		// ´õ·¯
	return S_OK;
}

VOID CBillBoard::Clear()
{
	m_vPosition = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_pTexBillboard[0] = m_pTexBillboard[1] = m_pTexBillboard[2] = m_pTexBillboard[3] = NULL;
	D3DXMatrixIdentity( &m_matInver );
	D3DXMatrixIdentity( &m_matWorld );
}

VOID CBillBoard::Update()
{
	if (m_nLife > 0) --m_nLife;
	m_nStep = (m_nStep + 1) % 80;
	m_matInver._41 = m_vPosition.x;
	m_matInver._42 = m_vPosition.y;
	m_matInver._43 = m_vPosition.z;
	
	INT n = static_cast<int>( m_nStep * 0.2f );
	m_pVtx[0].tex = m_pTexUV[n][0];
	m_pVtx[1].tex = m_pTexUV[n][1];
	m_pVtx[2].tex = m_pTexUV[n][2];
	m_pVtx[3].tex = m_pTexUV[n][3];
}

VOID CBillBoard::Render()
{
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF, (DWORD)0x0000009 );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCCOLOR );
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

	m_pd3dDevice->SetFVF( _EFFECT::FVF );
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matInver );

	m_pd3dDevice->SetTexture( 0, m_pTexBillboard[m_nType] );
	m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_pVtx, sizeof( _EFFECT ) );

	m_pd3dDevice->SetTexture( 0, NULL );
	m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );

	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld ); 
}

