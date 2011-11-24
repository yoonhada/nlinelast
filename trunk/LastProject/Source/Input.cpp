/**
	@file	Input.cpp
	@date	2011/09/27
	@author	�����
	@brief	Ű����, ���콺 ó�� Ŭ����
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
	m_hWnd				= NULL;
	m_vRotate = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	//m_fXPos = 0.0f;
	m_fYRotate = 0.0f;
	m_fXRotate = 0.0f;

	D3DXMatrixIdentity( &m_matMatrix );

	m_bF8button = m_bF9button = FALSE;

	m_bEnable = FALSE;

	ZeroMemory( &m_MousePosOld, sizeof(m_MousePosOld) );
	ZeroMemory( &m_MousePos, sizeof(m_MousePos) );

	m_iLbuttonCheck = -1;
	m_iRbuttonCheck = -1;

	// ���콺 Ŀ���� �����.
	//ShowCursor( FALSE );
}


HRESULT CInput::Create( HWND a_hWnd )
{

	m_hWnd		= a_hWnd;

	// ���� ���콺 ��ġ
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
	if( ( GetFocus() == m_hWnd && m_bEnable ) && 
		( !( GetAsyncKeyState( VK_LSHIFT ) || GetAsyncKeyState( VK_CONTROL ) ) ) )
	{
		GetCursorPos( &m_MousePosOld );
		// ���콺�� ��ȭ��
		INT dx = m_MousePosOld.x - m_MousePos.x;
		INT dy = m_MousePosOld.y - m_MousePos.y;

		// ��ȭ���� �ʹ� ũ�� 0���� �ʱ�ȭ (ó�� ���۽� ī�޶� ��ġ �̻��Ѱ� ����)
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
		// Y�� ȸ�� ��
		m_fYRotate = fYaw;
		m_fXRotate = fPitch;
		////CDebugConsole::GetInstance()->Messagef( "Input : %f\n", m_fYRotate );

		// ���콺�� �������� �߾����� �ʱ�ȭ
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

		if ( GetAsyncKeyState( VK_LBUTTON ) )
		{
			++m_iLbuttonCheck;
		}
		else
		{
			m_bLbutton = FALSE;
			m_iLbuttonCheck = -1;
		}

		if ( GetAsyncKeyState( VK_RBUTTON ) )
		{
			++m_iRbuttonCheck;
		}
		else
		{
			m_bRbutton = FALSE;
			m_iRbuttonCheck = -1;
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

		if( ( GetAsyncKeyState( VK_F1 ) & 0x0001 ) == TRUE )
		{
			m_bF1button = TRUE;
		}
		else
		{
			m_bF1button = FALSE;
		}

		for (int i = 0; i < 10; ++i)
		{
			if( ( GetAsyncKeyState( '0' + i ) ) )
			{
				m_bNumKey[i] = TRUE;
			}
			else
			{
				m_bNumKey[i] = FALSE;
			}
		}
	}
}