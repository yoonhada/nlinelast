/**
	@class	DirectX9
	@date	2011/08/26
	@author	김종철
	@brief	DirectX 클래스
*/


#include "stdafx.h"

/**
	@input	VOID
	@return	VOID
	@brief	생성자
*/
CDirectX9::CDirectX9()
{
	Clear();
}


/**
	@input	VOID
	@return	VOID
	@brief	소멸자
*/
CDirectX9::~CDirectX9()
{
	Release();
}

/**
	@input	VOID
	@return	VOID
	@brief	멤버 변수 초기화
*/
VOID CDirectX9::Clear()
{
	m_pD3D			= NULL;
	m_pD3dDevice	= NULL;
	ZeroMemory( &m_D3dpp, sizeof( D3DPRESENT_PARAMETERS ) );
}

/**
	@input	윈도우 핸들
	@return	초기화 결과
	@brief	초기화
*/
HRESULT CDirectX9::Create( HWND a_hWnd )
{
	// D3D 객체 생성
	if ( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		return E_FAIL;
	}

	// 구조체 설정
	m_D3dpp.Windowed				= TRUE;
	m_D3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	m_D3dpp.BackBufferFormat		= D3DFMT_UNKNOWN;
	m_D3dpp.EnableAutoDepthStencil	= TRUE;
	m_D3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	m_D3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;

	D3DCAPS9 caps;
	DWORD dwPSProcess;
	// 디바이스의 능력값(caps)을 읽어온다
	m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );

	// 지원하는 픽셀쉐이더 버전이 1.0이하라면 REF드라이버를, 1.0이상이면 HW드라이버를 생성한다.
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

	//// 디바이스 생성
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
	@brief	오브젝트들 제거
*/
VOID CDirectX9::Release()
{
	SAFE_RELEASE( m_pD3dDevice );
	SAFE_RELEASE( m_pD3D );
}


/**
	@input	VOID
	@return	VOID
	@brief	렌더링 시작
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
	@brief	렌더링 끝
*/
VOID CDirectX9::endScene()
{
	// End Scene
	m_pD3dDevice->EndScene();

	// 후면 버퍼 -> 실제 화면
	m_pD3dDevice->Present( NULL, NULL, NULL, NULL );
}
