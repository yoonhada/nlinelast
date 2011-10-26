#include "stdafx.h"
#include "ShadowCell.h"

CShadowCell::CShadowCell(VOID) : m_pD3dDevice(NULL), m_pTexture(NULL), m_pVB(NULL), m_pIB(NULL)
{
	Clear();
}

CShadowCell::~CShadowCell(VOID)
{
	Release();
}

VOID CShadowCell::Clear()
{
	ZeroMemory( m_Vertices, sizeof(m_Vertices) );
}

HRESULT CShadowCell::Create( LPDIRECT3DDEVICE9 a_pD3dDevice, 
						FLOAT a_Vertex11, FLOAT a_Vertex12, FLOAT a_Vertex13,
					    FLOAT a_Vertex21, FLOAT a_Vertex22, FLOAT a_Vertex23,
					    FLOAT a_Vertex31, FLOAT a_Vertex32, FLOAT a_Vertex33,
					    FLOAT a_Vertex41, FLOAT a_Vertex42, FLOAT a_Vertex43,
						FLOAT a_Uv11, FLOAT a_Uv12,
						FLOAT a_Uv21, FLOAT a_Uv22,
						FLOAT a_Uv31, FLOAT a_Uv32,
						FLOAT a_Uv41, FLOAT a_Uv42, 
						LPCWSTR a_FileName )
{
	m_pD3dDevice = a_pD3dDevice;

	if( S_OK != CreateVB( a_Vertex11, a_Vertex12, a_Vertex13,
		a_Vertex21, a_Vertex22, a_Vertex23,
		a_Vertex31, a_Vertex32, a_Vertex33,
		a_Vertex41, a_Vertex42, a_Vertex43,
		a_Uv11, a_Uv12,
		a_Uv21, a_Uv22,
		a_Uv31, a_Uv32,
		a_Uv41, a_Uv42 ) )
	{
		return E_FAIL;
	}
	
	if( S_OK != CreateIB() )
	{
		return E_FAIL;
	}
	
	if( S_OK != LoadTexture( a_FileName ) )
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CShadowCell::Release()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pTexture);

	return S_OK;
}

HRESULT CShadowCell::CreateVB( FLOAT a_Vertex11, FLOAT a_Vertex12, FLOAT a_Vertex13,
						    FLOAT a_Vertex21, FLOAT a_Vertex22, FLOAT a_Vertex23,
						    FLOAT a_Vertex31, FLOAT a_Vertex32, FLOAT a_Vertex33,
						    FLOAT a_Vertex41, FLOAT a_Vertex42, FLOAT a_Vertex43,
						    FLOAT a_Uv11, FLOAT a_Uv12,
							FLOAT a_Uv21, FLOAT a_Uv22,
							FLOAT a_Uv31, FLOAT a_Uv32,
							FLOAT a_Uv41, FLOAT a_Uv42 )
{
	// 정점 버퍼 생성
	if( FAILED( m_pD3dDevice->CreateVertexBuffer( 4 * sizeof( CELLVERTEX ),
		0, CELLVERTEX::FVF, D3DPOOL_MANAGED, &m_pVB, NULL ) ) )
	{
		Release();
		return E_FAIL;
	}

	VOID* pVertices = NULL;

	if( FAILED( m_pVB->Lock( 0, 4 * sizeof( CELLVERTEX ), (VOID**)&pVertices, 0 ) ) )
	{
		Release();
		return E_FAIL;
	}

	CELLVERTEX vertices[4] = {

		{ D3DXVECTOR3( a_Vertex11,  a_Vertex12, a_Vertex13 ), 
		//D3DXVECTOR3( a_Normal11,  a_Normal12, a_Normal13 ), 
		D3DXVECTOR2( a_Uv11, a_Uv12 ),
		//0x80808080 
		},	// 0
		
		{ D3DXVECTOR3( a_Vertex21,  a_Vertex22, a_Vertex23 ), 
		//D3DXVECTOR3( a_Normal21,  a_Normal22, a_Normal23 ), 
		D3DXVECTOR2( a_Uv21, a_Uv22 ),
		//0x80808080 
		},	// 1
		
		{ D3DXVECTOR3( a_Vertex31,  a_Vertex32, a_Vertex33 ), 
		//D3DXVECTOR3( a_Normal31,  a_Normal32, a_Normal33 ), 
		D3DXVECTOR2( a_Uv31, a_Uv32 ),
		//0x80808080 
		},	// 2
		
		{ D3DXVECTOR3( a_Vertex41,  a_Vertex42, a_Vertex43 ), 
		//D3DXVECTOR3( a_Normal41,  a_Normal42, a_Normal43 ), 
		D3DXVECTOR2( a_Uv41, a_Uv42 ),
		//0x80808080 
		},	// 3
	};	

	memcpy( m_Vertices, vertices, 4 * sizeof( CELLVERTEX ) );
	memcpy( pVertices, vertices, 4 * sizeof( CELLVERTEX ) );
	m_pVB->Unlock();

	return S_OK;
}


HRESULT CShadowCell::CreateIB()
{
	// 인덱스 버퍼 생성
	if( FAILED( m_pD3dDevice->CreateIndexBuffer( 2 * sizeof( CELLINDEX ), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL ) ) )
	{
		Release();
		return E_FAIL;
	}

	VOID* pIndices = NULL;

	if( FAILED( m_pIB->Lock( 0, 2 * sizeof( CELLINDEX ), (VOID**)&pIndices, 0 ) ) )
	{
		Release();
		return E_FAIL;
	}

	CELLINDEX i[2] = 
	{	
		{ 0, 1, 3 },	{ 1, 2, 3 }
	};

	memcpy( pIndices, i, 2 * sizeof( CELLINDEX ) );
	m_pIB->Unlock();

	return S_OK;
}


HRESULT CShadowCell::LoadTexture( LPCWSTR a_FileName )
{
	SAFE_RELEASE(m_pTexture);

	if( FAILED( D3DXCreateTextureFromFileEx( m_pD3dDevice, a_FileName, 
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT, 0,
		D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0,
		NULL, NULL, &m_pTexture ) ) )
	{
		MessageBox( NULL, a_FileName, L"Load Failed ShadowCell.cpp Line 171", MB_OK );
		return E_FAIL;
	}

	/*D3DSURFACE_DESC ddsd;

	m_pTexture->GetLevelDesc(0, &ddsd);

	D3DLOCKED_RECT rect;
	ZeroMemory( &rect, sizeof(rect) );

	m_pTexture->LockRect(0, &rect, 0, 0);
	BYTE* pBuffer = static_cast<BYTE*>(rect.pBits);

	for(INT y=0; y<ddsd.Height; ++y)
	{
		for(INT x=0; x<ddsd.Width; ++x)
		{
			BYTE* pColor = &pBuffer[ (x * 4) + y * ( ddsd.Width * 4) ];

			pColor[0] = 0x80;
			pColor[1] = 0x80;
			pColor[2] = 0x80;
			pColor[3] = 0x80;
		}
	}

	m_pTexture->UnlockRect(0);*/

	return S_OK;
}

VOID CShadowCell::Render()
{
	m_pD3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ); //컬링은 끄기
	//m_pD3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );   //Z버퍼 끄기
	m_pD3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	//m_pD3dDevice->SetRenderState( D3DRS_ALPHAREF, (DWORD)0x0000009 );
	//m_pD3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

	//m_pD3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	m_pD3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCCOLOR );
	m_pD3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	m_pD3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );


	m_pD3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof( CELLVERTEX ) );
	m_pD3dDevice->SetFVF( CELLVERTEX::FVF );
	m_pD3dDevice->SetIndices( m_pIB );

	//m_pD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//m_pD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	//m_pD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//m_pD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3dDevice->SetTexture( 0, m_pTexture );
	//m_pD3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	//m_pD3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//D3D9DEVICE->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP); 
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	m_pD3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 );

	m_pD3dDevice->SetTexture( 0, NULL );

	m_pD3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW ); // 컬링 켜기
	//D3D9DEVICE->SetRenderState( D3DRS_ZENABLE, TRUE );
	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_pD3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	//m_pD3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );

	m_pD3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}