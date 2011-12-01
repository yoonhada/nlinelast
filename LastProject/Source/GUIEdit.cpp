#include "stdafx.h"
#include "GUIEdit.h"
#include "GUIWriting.h"
#include "GUIFont.h"

VOID GUIEdit::Initialize()
{
	memset( m_Data.Str, 0, sizeof( m_Data.Str ) );

	m_dStyle		= EDT_STATIC;

	m_bActivate		= TRUE;
	m_bMessage		= FALSE;

	//	Init Font Info
	_tcscpy_s( m_aFaceName, L"휴먼매직체" );
	m_iFontWidth	= 10;
	m_iFontHeight	= 10;
}

VOID GUIEdit::Release()
{
}

VOID GUIEdit::CreateGUICaret()
{
	//	Test Caret Image2D
	GUIBase::IMAGEPARAM imgCaret;

	imgCaret.dPivotType = GUIBase::GBS_TOPLEFT;
	imgCaret.fX			= static_cast<FLOAT>( m_iFontX );
	imgCaret.fY			= static_cast<FLOAT>( m_iFontY );
	imgCaret.fWidth		= static_cast<FLOAT>( m_iFontWidth );
	imgCaret.fHeight	= static_cast<FLOAT>( m_iFontHeight );

	AddFileName( 0, imgCaret, L"Img\\Caret0.png", 500 );
	AddFileName( 0, imgCaret, L"Img\\Caret1.png", 500 );

	CreateImage2D( m_Data.img2DCaret, imgCaret );
}

VOID GUIEdit::SetStyle( DWORD _dStyle )
{
	switch( _dStyle )
	{
	case EDT_STATIC:
		m_bActivate = TRUE;
		break;
	case EDT_DYNAMIC:
		m_bActivate = FALSE;
		break;
	}

	m_dStyle = _dStyle;
}

VOID GUIEdit::SetFont( LPWSTR _pFaceName, INT _iWidth, INT _iHeight )
{
	_tcscpy( m_aFaceName, _pFaceName );
	m_iFontWidth	= _iWidth;
	m_iFontHeight	= _iHeight;

	CreateGUICaret();
}

VOID GUIEdit::Create( GUIBase::IMAGEPARAM& _imgParam )
{
	//	Set Font X, Y
	m_iFontX	= static_cast<INT>( _imgParam.fX + ( m_iFontWidth ) * 0.5f );
	m_iFontY	= static_cast<INT>( _imgParam.fY + ( _imgParam.fHeight - m_iFontHeight ) * 0.5f );
	
	// Create Texture Area
	SetRect(	&m_Data.rtText,
				m_iFontX,
				m_iFontY,
				static_cast<INT>( _imgParam.fX + _imgParam.fWidth ),
				static_cast<INT>( _imgParam.fY + _imgParam.fHeight ) );

	//	Create Imgae
	CreateImage2D( m_Data.img2DBackground, _imgParam );

	//	Test Caret Image2D
	CreateGUICaret();
}

VOID GUIEdit::Create( FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, GUIBase::IMAGEPARAM& _imgParam )
{
	//	Set Font X, Y
	m_iFontX		= static_cast<INT>( _fX + ( m_iFontWidth ) * 0.5f );
	m_iFontY		= static_cast<INT>( _fY + ( _fHeight - m_iFontHeight ) * 0.5f );
	
	// Create Edit
	SetRect(	&m_Data.rtText,
				m_iFontX,
				m_iFontY,
				static_cast<INT>( _fX + _fWidth ),
				static_cast<INT>( _fY + _fHeight ) );

	CreateImage2D( m_Data.img2DBackground, _fX, _fY, _fWidth, _fHeight, _imgParam );

	//	Test Caret Image2D
	GUIBase::IMAGEPARAM imgCaret;

	imgCaret.dPivotType = GUIBase::GBS_TOPLEFT;
	imgCaret.fX			= static_cast<FLOAT>( m_iFontX );
	imgCaret.fY			= static_cast<FLOAT>( m_iFontY );
	imgCaret.fWidth		= static_cast<FLOAT>( m_iFontWidth );
	imgCaret.fHeight	= static_cast<FLOAT>( m_iFontHeight );

	AddFileName( 0, imgCaret, L"Img\\Caret0.png", 500 );
	AddFileName( 0, imgCaret, L"Img\\Caret1.png", 500 );

	CreateImage2D( m_Data.img2DCaret, imgCaret );

	// 버퍼 초기화
	GUIWriting::GetInstance().Cleanup();

}

VOID GUIEdit::Update()
{
	INT iNum = 0;
	
	if( GUIWriting::GetInstance().GetText( m_Data.Str, iNum ) && m_dStyle == EDT_DYNAMIC )
	{
		if( iNum !=  0 )
			m_bMessage	= TRUE;
		
		m_bActivate = !m_bActivate;
		GUIWriting::GetInstance().Cleanup();
	}
	
	if( !m_bActivate )
	{
		GUIWriting::GetInstance().Cleanup();
		return;
	}

	GUIFont::GetInstance().Create( m_aFaceName, m_iFontWidth, m_iFontHeight, m_pd3dDevice );
	SIZE Size;
	GUIFont::GetInstance().GetTextSize( m_Data.Str, Size );
	m_Data.img2DCaret.vecPosition.x = static_cast<FLOAT>( m_iFontX + Size.cx );
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

	memcpy( _pStr, m_Data.Str, sizeof( m_Data.Str ) );

	if( !m_bMessage || m_bActivate )
		return FALSE;

	m_bMessage = FALSE;

	return TRUE;
}