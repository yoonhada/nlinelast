/**
	@class	DirectX9
	@date	2011/08/26
	@author	����ö
	@brief	DirectX Ŭ����
*/


#include "stdafx.h"

/**
	@input	VOID
	@return	VOID
	@brief	������
*/
CDirectX9::CDirectX9()
{
	Clear();
}


/**
	@input	VOID
	@return	VOID
	@brief	�Ҹ���
*/
CDirectX9::~CDirectX9()
{
	Release();
}

/**
	@input	VOID
	@return	VOID
	@brief	��� ���� �ʱ�ȭ
*/
VOID CDirectX9::Clear()
{
	m_pD3D			= NULL;
	m_pD3dDevice	= NULL;
	ZeroMemory( &m_D3dpp, sizeof( D3DPRESENT_PARAMETERS ) );
}

/**
	@input	������ �ڵ�
	@return	�ʱ�ȭ ���
	@brief	�ʱ�ȭ
*/
HRESULT CDirectX9::Create( HWND a_hWnd )
{
	// D3D ��ü ����
	if ( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		return E_FAIL;
	}

	// ����ü ����
	m_D3dpp.Windowed				= TRUE;
	m_D3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	m_D3dpp.BackBufferFormat		= D3DFMT_UNKNOWN;
	m_D3dpp.EnableAutoDepthStencil	= TRUE;
	m_D3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	m_D3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;

	D3DCAPS9 caps;
	DWORD dwPSProcess;
	// ����̽��� �ɷ°�(caps)�� �о�´�
	m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );

	// �����ϴ� �ȼ����̴� ������ 1.0���϶�� REF����̹���, 1.0�̻��̸� HW����̹��� �����Ѵ�.
	dwPSProcess = ( caps.PixelShaderVersion < D3DPS_VERSION(1,0) ) ? 0 : 1;

	if( dwPSProcess )
	{
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, a_hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, 
			&m_D3dpp, &m_pD3dDevice ) ) )
			return E_FAIL;
	}
	else
	{
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, a_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, 
			&m_D3dpp, &m_pD3dDevice ) ) )
			return E_FAIL;
	}

	//// ����̽� ����
	//if ( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, a_hWnd,
	//								 D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_D3dpp, &m_pD3dDevice ) ) )
	//{
	//	return E_FAIL;
	//}

	// RenderState
	m_pD3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	m_pD3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	return S_OK;
}


/**
	@input	VOID
	@return	VOID
	@brief	������Ʈ�� ����
*/
VOID CDirectX9::Release()
{
	SAFE_RELEASE( m_pD3dDevice );
	SAFE_RELEASE( m_pD3D );
}


/**
	@input	VOID
	@return	VOID
	@brief	������ ����
*/
VOID CDirectX9::beginScene()
{
	// Clear
	m_pD3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

	// Begin Scene
	if ( !SUCCEEDED( m_pD3dDevice->BeginScene() ) )
	{
		return;
	}
}


/**
	@input	VOID
	@return	VOID
	@brief	������ ��
*/
VOID CDirectX9::endScene()
{
	// End Scene
	m_pD3dDevice->EndScene();

	// �ĸ� ���� -> ���� ȭ��
	m_pD3dDevice->Present( NULL, NULL, NULL, NULL );
}
