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
	m_hWnd			= NULL;
	m_vRotate		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vPos			= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	//m_fXPos = 0.0f;
	m_fYRotate = 0.0f;
	m_fXRotate = -MTP_FUN::Deg2Rad<45>::radians;	// D3DXToRadian( -45.0f );

	D3DXMatrixIdentity( &m_matMatrix );

	m_bF8button = m_bF9button = FALSE;
	m_bESCKey = FALSE;
	m_bLbutton = m_bRbutton = FALSE;

	m_bEnable = TRUE;

	ZeroMemory( &m_MousePosOld, sizeof(m_MousePosOld) );
	ZeroMemory( &m_MousePos, sizeof(m_MousePos) );

	m_bNumKey[0] = m_bNumKey[1] = m_bNumKey[2] = m_bNumKey[3] = m_bNumKey[4] = \
	m_bNumKey[5] = m_bNumKey[6] = m_bNumKey[7] = m_bNumKey[8] = m_bNumKey[9] = FALSE;
}

HRESULT CInput::Create( HWND a_hWnd )
{

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
	if( ( GetFocus() == m_hWnd && !m_bEnable ) )
	{
		if ( !( GetAsyncKeyState( VK_LSHIFT ) || GetAsyncKeyState( VK_CONTROL ) ) )
		{
			GetCursorPos( &m_MousePosOld );
			// 마우스의 변화값
			INT dx = m_MousePosOld.x - m_MousePos.x;
			INT dy = m_MousePosOld.y - m_MousePos.y;

			// 변화값이 너무 크면 0으로 초기화 (처음 시작시 카메라 위치 이상한거 방지)
			if( dx + dy > 1000 )		dx =  dy = 0;

			m_vRotate = D3DXVECTOR3( 
				static_cast<FLOAT>(dx) * a_fCameraRotateSpeed * a_fFrameTime,
				static_cast<FLOAT>(dy) * a_fCameraRotateSpeed * a_fFrameTime,
				0.0f );

			// Y축 회전 값
			m_fYRotate = D3DXToRadian( m_vRotate.x * 0.075f );
			m_fXRotate = D3DXToRadian( m_vRotate.y * 0.075f );
			////CDebugConsole::GetInstance()->Messagef( "Input : %f\n", m_fXRotate );

			// 마우스를 윈도우의 중앙으로 초기화
			MouseMoveCenter();

			if ( GetAsyncKeyState( 'A' ) )			{ m_vPos.x =-a_fCameraMoveSpeed * a_fFrameTime;		}
			if ( GetAsyncKeyState( 'D' ) )			{ m_vPos.x = a_fCameraMoveSpeed * a_fFrameTime;		}
			if ( GetAsyncKeyState( 'W' ) )			{ m_vPos.z = a_fCameraMoveSpeed * a_fFrameTime;		}
			if ( GetAsyncKeyState( 'S' ) )			{ m_vPos.z =-a_fCameraMoveSpeed * a_fFrameTime;		}

			m_bEndbutton = m_bHomebutton = FALSE;

			if ( GetAsyncKeyState( VK_END ) )		{ m_bEndbutton = TRUE;								}
			if ( GetAsyncKeyState( VK_HOME ) )		{ m_bHomebutton = TRUE;								}

			if ( GetAsyncKeyState( VK_ESCAPE ) )		{ ++m_bESCKey;									}
			else if ( GetAsyncKeyState( VK_LBUTTON ) )	{ ++m_bLbutton;									}
			else if ( GetAsyncKeyState( VK_RBUTTON ) )	{ ++m_bRbutton;									}
			//else if ( GetAsyncKeyState( VK_F1 ) )		{ ++m_bF1button;								}
			//else if ( GetAsyncKeyState( VK_F8 ) )		{ ++m_bF8button;								}
			//else if ( GetAsyncKeyState( VK_F9 ) )		{ ++m_bF9button;								}
			else
			{
				m_bESCKey = FALSE;
				m_bLbutton = m_bRbutton = FALSE;
				m_bF1button = m_bF8button = m_bF9button = FALSE;
			}

			for (int i = 0; i < 12; ++i)
			{
				if( GetAsyncKeyState( VK_F1 + i ) )		++m_bFunKey[i];
				else									m_bFunKey[i] = FALSE;
			}

			for (int i = 0; i < 10; ++i)
			{
				if( GetAsyncKeyState( '0' + i ) )		++m_bNumKey[i];
				else									m_bNumKey[i] = FALSE;
			}
		}
	}
}

VOID CInput::MouseMoveCenter()
{
	RECT	rc;
	GetClientRect( m_hWnd, &rc );
	m_MousePosOld.x = (rc.right - rc.left) / 2;
	m_MousePosOld.y = (rc.bottom - rc.top) / 2;
	ClientToScreen( m_hWnd, &m_MousePosOld );
	SetCursorPos( m_MousePosOld.x, m_MousePosOld.y );
	m_MousePos.x = m_MousePosOld.x;
	m_MousePos.y = m_MousePosOld.y;

	m_vPos.x = m_vPos.y = m_vPos.z = 0.0f;
}

VOID CInput::EnableInput( BOOL bEnable )
{
	m_bEnable = bEnable; 
}

BOOL CInput::Get_NumKey( INT nInput )
{
	if ( 0 <= nInput && nInput <= 9 )
		return (m_bNumKey[nInput] == TRUE);

	return FALSE;
}

BOOL CInput::Get_FunKey( INT nInput )
{
	if ( 1 <= nInput && nInput <= 12 )
		return (m_bFunKey[nInput - 1] == TRUE);

	return FALSE;
}
