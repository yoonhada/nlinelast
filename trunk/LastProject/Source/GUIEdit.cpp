#include "stdafx.h"
#include "GUIEdit.h"
#include "GUIWriting.h"
#include "GUIFont.h"

VOID GUIEdit::Initialize()
{
	memset( m_Data.Str, 0, sizeof( m_Data.Str ) );

	m_bActivate		= FALSE;
	m_bMessage		= FALSE;
}

VOID GUIEdit::Release()
{
}

VOID GUIEdit::Create( GUIBase::IMAGEPARAM& _imgParam )
{
	m_Data.iX		= static_cast<INT>( _imgParam.fX + ( 10 ) * 0.5f );
	m_Data.iY		= static_cast<INT>( _imgParam.fY + ( _imgParam.fHeight - 20 ) * 0.5f );
	
	// Create Edit
	SetRect(	&m_Data.rtText,
				m_Data.iX,
				m_Data.iY,
				static_cast<INT>( _imgParam.fX + _imgParam.fWidth ),
				static_cast<INT>( _imgParam.fY + _imgParam.fHeight ) );

	CreateImage2D( m_Data.img2DBackground, _imgParam );

	//	Test Caret Image2D
	GUIBase::IMAGEPARAM imgCaret;

	imgCaret.dPivotType = GUIBase::GBS_TOPLEFT;
	imgCaret.fX			= static_cast<FLOAT>( m_Data.iX );
	imgCaret.fY			= static_cast<FLOAT>( m_Data.iY );
	imgCaret.fWidth		= 5.0f;
	imgCaret.fHeight	= static_cast<FLOAT>( 10 );

	AddFileName( 0, imgCaret, L"Img\\Caret0.png", 500 );
	AddFileName( 0, imgCaret, L"Img\\Caret1.png", 500 );

	CreateImage2D( m_Data.img2DCaret, imgCaret );
}
VOID GUIEdit::Create( FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, GUIBase::IMAGEPARAM& _imgParam )
{
	m_Data.iX		= static_cast<INT>( _fX + ( 10 ) * 0.5f );
	m_Data.iY		= static_cast<INT>( _fY + ( _fHeight - 20 ) * 0.5f );
	
	// Create Edit
	SetRect(	&m_Data.rtText,
				m_Data.iX,
				m_Data.iY,
				static_cast<INT>( _fX + _fWidth ),
				static_cast<INT>( _fY + _fHeight ) );

	CreateImage2D( m_Data.img2DBackground, _fX, _fY, _fWidth, _fHeight, _imgParam );

	//	Test Caret Image2D
	GUIBase::IMAGEPARAM imgCaret;

	imgCaret.dPivotType = GUIBase::GBS_TOPLEFT;
	imgCaret.fX			= static_cast<FLOAT>( m_Data.iX );
	imgCaret.fY			= static_cast<FLOAT>( m_Data.iY );
	imgCaret.fWidth		= 5.0f;
	imgCaret.fHeight	= static_cast<FLOAT>( 20 );

	AddFileName( 0, imgCaret, L"Img\\Caret0.png", 500 );
	AddFileName( 0, imgCaret, L"Img\\Caret1.png", 500 );

	CreateImage2D( m_Data.img2DCaret, imgCaret );
}

VOID GUIEdit::Update()
{
	INT iNum = 0;
	GUIWriting::GetInstance().GetText( m_Data.Str, iNum );

	if( m_Data.Str[ 0 ] == '\n' )
	{
		m_bActivate = !m_bActivate;

		GUIWriting::GetInstance().Cleanup();
	}
	
	if( !m_bActivate )
	{
		GUIWriting::GetInstance().Cleanup();
		return;
	}

	if( m_Data.Str[ iNum - 1 ] == '\n' && iNum != 1 )
	{
		GUIWriting::GetInstance().Cleanup();
		m_bMessage = TRUE;
	}

	GUIFont::GetInstance().Create( L"ÈÞ¸Õ¸ÅÁ÷Ã¼", 20, 20, m_pD3dDevice );
	SIZE Size;
	GUIFont::GetInstance().GetTextSize( m_Data.Str, Size );
	m_Data.img2DCaret.vecPosition.x = static_cast<FLOAT>( m_Data.iX + Size.cx );
}

VOID GUIEdit::Render()
{
	if( !m_bActivate )
		return;
	RenderImage2D( &m_Data.img2DBackground );
	RenderImage2D( &m_Data.img2DCaret );

	GUIFont::GetInstance().Render( m_Data.Str, &m_Data.rtText, 0xff000000 );
}

BOOL GUIEdit::TakeMessage( LPWSTR _pStr )
{
	if( !m_bMessage )
		return FALSE;

	memcpy( _pStr, m_Data.Str, sizeof( m_Data.Str ) );

	m_bMessage = FALSE;

	return TRUE;
}