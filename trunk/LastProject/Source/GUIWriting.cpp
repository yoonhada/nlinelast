#include "stdafx.h"
#include "GUIWriting.h"

GUIWriting& GUIWriting::GetInstance()
{
	static GUIWriting m_GUIWriting;

	return m_GUIWriting;
}

VOID GUIWriting::Initialize()
{
	m_pText			= NULL;
	m_piTextLength	= NULL;

	m_bReturn		= FALSE;
}

VOID GUIWriting::Release()
{
}

VOID GUIWriting::IMEUpdate( WPARAM& _wParam, LPARAM& _lParam )
{
	if( m_pText == NULL )
		return;

	if( _lParam & GCS_COMPSTR )
		m_pText[ (*m_piTextLength) ]		= static_cast<TCHAR>( _wParam );
	if( _lParam & GCS_RESULTSTR )
		m_pText[ (*m_piTextLength)++ ]	= static_cast<TCHAR>( _wParam );
}

VOID GUIWriting::CHARUpdate( WPARAM& _wParam, LPARAM& _lParam )
{
	if( _wParam == VK_RETURN )
		m_bReturn = TRUE;
	if( m_pText == NULL )
		return;

	switch( _wParam )
	{
	case VK_ESCAPE:
		break;
	case VK_RETURN:
		break;
	case VK_BACK:
		if( (*m_piTextLength) - 1 >= 0 )
			m_pText[ --(*m_piTextLength) ] = '\0';
		break;
	default:
		m_pText[ (*m_piTextLength)++ ] = static_cast<TCHAR>( _wParam );
		break;
	}

}

BOOL GUIWriting::GetReturn()
{
	if( m_bReturn )
	{
		m_bReturn = FALSE;
		return TRUE;
	}

	return FALSE;
}

VOID GUIWriting::ConnectText( LPWSTR _pText, INT* _piTextLength )
{
	m_pText			= _pText;
	m_piTextLength	= _piTextLength;
}

VOID GUIWriting::DisconnectText()
{
	m_pText = NULL;
}
