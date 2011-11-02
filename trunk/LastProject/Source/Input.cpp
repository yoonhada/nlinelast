/**
	@file	Input.cpp
	@date	2011/09/27
	@author	백경훈
	@brief	키보드, 마우스 처리 클래스
*/


#include "stdafx.h"
#include "Input.h"


CInput::CInput()
{
	Clear();
}


CInput::~CInput()
{
	Release();
}

VOID CInput::Clear()
{
	m_pD3dDevice		= NULL;
	m_hWnd				= NULL;
	m_vRotate = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	//m_fXPos = 0.0f;
	m_fYRotate = 0.0f;
	m_fXRotate = 0.0f;

	D3DXMatrixIdentity( &m_matMatrix );

	m_bF8button = m_bF9button = FALSE;
}


HRESULT CInput::Create( LPDIRECT3DDEVICE9 a_pD3dDevice, HWND a_hWnd )
{
	m_pD3dDevice= a_pD3dDevice;

	m_hWnd		= a_hWnd;

	// 이전 마우스 위치
	RECT	rc;
	GetClientRect( m_hWnd, &rc );
	m_MousePosOld.x = (rc.right - rc.left) / 2;
	m_MousePosOld.y = (rc.bottom - rc.top) / 2;

	return S_OK;
}


HRESULT CInput::Release()
{
	return S_OK;
}


VOID CInput::Update( FLOAT a_fCameraMoveSpeed, FLOAT a_fCameraRotateSpeed, 
						  FLOAT a_fFrameTime )
{
	if( GetFocus() == m_hWnd )
	{
		if ( !( GetAsyncKeyState( VK_LSHIFT ) || GetAsyncKeyState( VK_CONTROL ) ) )
		{
			GetCursorPos( &m_MousePosOld );
			// 마우스의 변화값
			INT dx = m_MousePosOld.x - m_MousePos.x;
			INT dy = m_MousePosOld.y - m_MousePos.y;

			// 변화값이 너무 크면 0으로 초기화 (처음 시작시 카메라 위치 이상한거 방지)
			if( dx+dy > 1000 )
			{
				dx = 0;
				dy = 0;
			}


			m_vRotate = D3DXVECTOR3( static_cast<FLOAT>(dx) * a_fCameraRotateSpeed *  a_fFrameTime,
				static_cast<FLOAT>(dy) * a_fCameraRotateSpeed *  a_fFrameTime,
				0.0f );

			FLOAT fYaw	= D3DXToRadian(m_vRotate.x * 0.1f);
			FLOAT fPitch = D3DXToRadian(m_vRotate.y * 0.1f);
			// Y축 회전 값
			m_fYRotate = fYaw;
			m_fXRotate = fPitch;
			////CDebugConsole::GetInstance()->Messagef( "Input : %f\n", m_fYRotate );

			// 마우스를 윈도우의 중앙으로 초기화
			// 마우스를 나타나지 않게 않다.
			SetCursor( NULL );
			RECT	rc;
			GetClientRect( m_hWnd, &rc );
			m_MousePosOld.x = (rc.right - rc.left) / 2;
			m_MousePosOld.y = (rc.bottom - rc.top) / 2;
			ClientToScreen( m_hWnd, &m_MousePosOld );
			SetCursorPos( m_MousePosOld.x, m_MousePosOld.y );
			m_MousePos.x = m_MousePosOld.x;
			m_MousePos.y = m_MousePosOld.y;

			//D3DXMatrixRotationY( &m_matMatrix, m_fYRotate );
			//D3DXVec3TransformNormal( &m_vPos, &m_vPos, &m_matMatrix );

			m_vPos.x = m_vPos.y = m_vPos.z = 0.0f;

			if ( GetAsyncKeyState( 'A' ) )
			{
				m_vPos.x = -a_fCameraMoveSpeed * a_fFrameTime;
			}

			if ( GetAsyncKeyState( 'D' ) )
			{
				m_vPos.x = a_fCameraMoveSpeed * a_fFrameTime;
			}

			if ( GetAsyncKeyState( 'W' ) )
			{
				m_vPos.z = a_fCameraMoveSpeed * a_fFrameTime;
			}

			if ( GetAsyncKeyState( 'S' ) )
			{
				m_vPos.z = -a_fCameraMoveSpeed * a_fFrameTime;
			}

			if( GetAsyncKeyState( VK_ESCAPE ) )
			{
				PostMessage( m_hWnd, WM_DESTROY, NULL, NULL );
			}

			if ( GetAsyncKeyState( VK_LBUTTON ) /*& 0x0001 )*/ )
			{
				m_bLbutton = TRUE;
			}
			else
			{
				m_bLbutton = FALSE;
			}
			
			if ( GetAsyncKeyState( VK_RBUTTON ) /*& 0x0001 )*/)
			{
				m_bRbutton = TRUE;
			}
			else
			{
				m_bRbutton = FALSE;
			}

			if( GetAsyncKeyState( VK_END ) )
			{
				m_bENdbutton = TRUE;
			}
			else
			{
				m_bENdbutton = FALSE;
			}

			if( GetAsyncKeyState( VK_HOME ) )
			{
				m_bHomebutton = TRUE;
			}
			else
			{
				m_bHomebutton = FALSE;
			}

			if( ( GetAsyncKeyState( VK_F9 ) & 0x0001 ) == TRUE )
			{
				m_bF9button = TRUE;
			}
			else
			{
				m_bF9button = FALSE;
			}

			if( ( GetAsyncKeyState( VK_F8 ) & 0x0001 ) == TRUE )
			{
				m_bF8button = TRUE;
			}
			else
			{
				m_bF8button = FALSE;
			}
			
		}
	}
}