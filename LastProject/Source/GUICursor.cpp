#include "stdafx.h"
#include "GUICursor.h"

GUICursor& GUICursor::GetInstance()
{
	static GUICursor m_GUICursor;

	return m_GUICursor;
}

VOID GUICursor::Initialize()
{
	m_hCurArrow			= LoadCursor( NULL, IDC_ARROW );

	m_hCurSizeWE		= LoadCursor( NULL, IDC_SIZEWE );	
	m_hCurSizeNS		= LoadCursor( NULL, IDC_SIZENS );	
	m_hCurSizeNWSE		= LoadCursor( NULL, IDC_SIZENWSE );
	m_hCurSizeNESW		= LoadCursor( NULL, IDC_SIZENESW );

	m_hCurSizeAll		= LoadCursor( NULL, IDC_SIZEALL );

	m_hCurrentCursor	= m_hCurArrow;
}

VOID GUICursor::Release()
{
}

VOID GUICursor::Update( LPARAM _lParam )
{
	if( LOWORD( _lParam ) == HTCLIENT )
		SetCursor( m_hCurrentCursor );
}
VOID GUICursor::Change( DWORD _dType )
{
	switch( _dType )
	{
	case CSR_ARROW:
		m_hCurrentCursor = m_hCurArrow;
		break;
	case CSR_WE:
		m_hCurrentCursor = m_hCurSizeWE;
		break;
	case CSR_NS:
		m_hCurrentCursor = m_hCurSizeNS;
		break;
	case CSR_NWSE:
		m_hCurrentCursor = m_hCurSizeNWSE;
		break;
	case CSR_NESW:
		m_hCurrentCursor = m_hCurSizeNESW;
		break;
	case CSR_ALL:
		m_hCurrentCursor = m_hCurSizeAll;
		break;
	}
	
	SetCursor( m_hCurrentCursor );
}