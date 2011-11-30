#include "stdafx.h"
#include "GUIWriting.h"

GUIWriting& GUIWriting::GetInstance()
{
	static GUIWriting m_GUIWriting;

	return m_GUIWriting;
}

VOID GUIWriting::Initialize()
{
	m_iNum		= 0;
	m_bReturn	= FALSE;
}

VOID GUIWriting::Release()
{
}

VOID GUIWriting::IMEUpdate( WPARAM& _wParam, LPARAM& _lParam )
{
	if( _lParam & GCS_COMPSTR )
		m_Str[ m_iNum ]		= static_cast<TCHAR>( _wParam );
	if( _lParam & GCS_RESULTSTR )
		m_Str[ m_iNum++ ]	= static_cast<TCHAR>( _wParam );
}

VOID GUIWriting::CHARUpdate( WPARAM& _wParam, LPARAM& _lParam )
{
	switch( _wParam )
	{
	case VK_RETURN:
		m_bReturn = TRUE;
		break;
	case VK_BACK:
		if( m_iNum - 1 >= 0 )
			m_Str[ --m_iNum ] = '\0';
		break;
	default:
		m_Str[ m_iNum++ ] = static_cast<TCHAR>( _wParam );
		break;
	}

	//Console::GetInstance().Messagef( L"%s\n", m_Str );
}

BOOL GUIWriting::GetText( LPWSTR _pStr, INT& _iNum )
{
	memset( _pStr, 0, sizeof( _pStr ) );
	wcsncpy( _pStr, m_Str, sizeof( TCHAR ) * m_iNum );

	//	Test
	_iNum = m_iNum;

	if( m_bReturn )
	{
		m_bReturn = FALSE;
		return TRUE;
	}

	return FALSE;
}

VOID GUIWriting::Cleanup()
{
	memset( m_Str, 0, sizeof( m_Str ) );

	m_iNum = 0;
}

//VOID GUIWriting::SetHWND( HWND& _hWnd )
//{
//	m_hWnd = _hWnd;
//}
//
//const HWND& GUIWriting::GetHWND()
//{
//	return m_hWnd;
//}
//
