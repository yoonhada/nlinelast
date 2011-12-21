#include "stdafx.h"
#include "GUIEdit.h"
#include "GUIFont.h"
#include "GUICaret.h"

DWORD GUIEdit::EdtMessage = 0;

VOID GUIEdit::Initialize()
{
	//	Init Data
	m_dStyle		= EDT_STYLE_STATIC;
	m_dState		= EDT_STATE_NORMAL;
	m_bFocus		= FALSE;
	m_bMessage		= FALSE;
	
	//	Init Font Info
	_tcscpy_s( m_aFaceName, L"휴먼매직체" );
	m_iFontWidth	= 10;
	m_iFontHeight	= 10;

	//	new GUICaret
	m_pGUICaret = new GUICaret( m_pd3dDevice, m_pSprite );
}

VOID GUIEdit::Release()
{
	delete m_pGUICaret;
}

//VOID GUIEdit::Create( GUIBase::IMAGEPARAM& _imgParam )
//{
//	//	Set Font X, Y
//	m_iFontX	= static_cast<INT>( _imgParam.fX + ( m_iFontWidth ) * 0.5f );
//	m_iFontY	= static_cast<INT>( _imgParam.fY + ( _imgParam.fHeight - m_iFontHeight ) * 0.5f );
//	
//	// Create Texture Area
//	SetRect(	&m_Data.rtText,
//				m_iFontX,
//				m_iFontY,
//				static_cast<INT>( _imgParam.fX + _imgParam.fWidth ),
//				static_cast<INT>( _imgParam.fY + _imgParam.fHeight ) );
//
//	//	Create Imgae
//	CreateImage2D( m_Data.img2DBackground, _imgParam );
//
//	//	Test Caret Image2D
//	CreateGUICaret();
//
//}

VOID GUIEdit::Create( DWORD _dID, FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, IMAGEPARAM& _imgParamEdit, IMAGEPARAM& _imgParamCaret )
{
	// Init ID
	m_dID = _dID;

	m_fX		= _fX;
	m_fY		= _fY;
	m_fWidth	= _fWidth;
	m_fHeight	= _fHeight;

	//	Set Font X, Y
	m_iFontX		= static_cast<INT>( _fX + ( m_iFontWidth ) * 0.5f );
	m_iFontY		= static_cast<INT>( _fY + ( _fHeight - m_iFontHeight ) * 0.5f );
	
	// Create Edit
	SetRect(	&m_rtText,
				m_iFontX,
				m_iFontY,
				static_cast<INT>( _fX + _fWidth ),
				static_cast<INT>( _fY + _fHeight ) );
	
	CreateImage3D( m_img3DEdit, _fX, _fY, _fWidth, _fHeight, _imgParamEdit );

	//	Init Font Info
	m_iTextLength = 0;
	ZeroMemory( m_pcText, sizeof( m_pcText ) );

	//	Create Caret
	m_pGUICaret->Create( _imgParamCaret );
}

BOOL GUIEdit::Update( BOOL _bReturn )
{	//	함수 포인터를 써서 할 수 있도록 하자
	if( m_dStyle == EDT_STYLE_STATIC )
	{
		if( m_bFocus & _bReturn )
			EnableFocus( FALSE );
	}

	if( m_dStyle == EDT_STYLE_DYNAMIC && _bReturn )
	{
		if( m_dState == EDT_STATE_NORMAL )
		{
			EnableFocus( FALSE );
			SetState( EDT_STATE_HIDDEN );

			return FALSE;
		}
		
		if( m_dState == EDT_STATE_HIDDEN )
		{	
			SetState( EDT_STATE_NORMAL );
			EnableFocus( TRUE );
			
			return TRUE;
		}
		
	}
	
	if(	m_dState == EDT_STATE_DISABLE || m_dState == EDT_STATE_HIDDEN )return FALSE;

	//	이것도 어서 싱글톤으로 만들어 EditManager에서 돌 수 있도록 하자
	m_pGUICaret->Update();

	return FALSE;
}

VOID GUIEdit::Render()
{
	if(	m_dState == EDT_STATE_HIDDEN )return;
		
	RenderImage3D( &m_img3DEdit );
	
	//	Set Font
	GUIFont::GetInstance().Create( m_aFaceName, m_iFontWidth, m_iFontHeight, m_pd3dDevice );
	GUIFont::GetInstance().Render( m_pcText, &m_rtText, 0xff000000 );

	if( m_bFocus )
		m_pGUICaret->Render();

}


VOID GUIEdit::SetStyle( DWORD _dStyle )
{
	switch( _dStyle )
	{
	case EDT_STYLE_STATIC:
		m_dState = EDT_STATE_NORMAL;
		break;
	case EDT_STYLE_DYNAMIC:
		m_dState = EDT_STATE_HIDDEN;
		break;
	}

	m_dStyle = _dStyle;
}

VOID GUIEdit::SetState( DWORD _dState )
{
	m_dState = _dState;
}

VOID GUIEdit::EnableFocus( BOOL _bEnable )
{
	if( m_dState == EDT_STATE_DISABLE || m_dState == EDT_STATE_HIDDEN )
		return;

	m_bFocus = _bEnable;

	if( m_bFocus )
	{
		//	Connect Caret
		FLOAT fFontX		= static_cast<FLOAT>( m_iFontX );
		FLOAT fFontY		= static_cast<FLOAT>( m_iFontY );
		FLOAT fFontWidth	= static_cast<FLOAT>( m_iFontWidth );
		FLOAT fFontHeight	= static_cast<FLOAT>( m_iFontHeight );

		fFontX	= m_fX + ( fFontWidth ) * 0.5f;
		fFontY	= m_fY + ( m_fHeight - fFontHeight ) * 0.5f;
	
		m_pGUICaret->ConnectText( m_pcText, fFontX, fFontY, fFontWidth, fFontHeight+ 5.0f );

		if( m_dStyle == EDT_STYLE_DYNAMIC )
			CInput::GetInstance()->EnableInput( TRUE );
	}
	else
	{
		m_pGUICaret->DisconnectText();

		m_bMessage = TRUE;

		if( m_dStyle == EDT_STYLE_DYNAMIC )
			CInput::GetInstance()->EnableInput( FALSE );
	}
}

VOID GUIEdit::SetFont( LPWSTR _pFaceName, INT _iWidth, INT _iHeight )
{
	_tcscpy_s( m_aFaceName, _pFaceName );
	m_iFontWidth	= _iWidth;
	m_iFontHeight	= _iHeight;

	//	Set Font X, Y
	m_iFontX		= static_cast<INT>( m_fX + ( m_iFontWidth ) * 0.5f );
	m_iFontY		= static_cast<INT>( m_fY + ( m_fHeight - m_iFontHeight ) * 0.5f );
	
	// Create Edit
	SetRect(	&m_rtText,
				m_iFontX,
				m_iFontY,
				static_cast<INT>( m_fX + m_fWidth ),
				static_cast<INT>( m_fY + m_fHeight ) );
}

VOID GUIEdit::SetText( LPWSTR _pcText )
{
	_tcscpy( m_pcText, _pcText );
	m_iTextLength = _tcslen( m_pcText );
}

BOOL GUIEdit::TakeMessage( LPWSTR _pcText )
{
	memcpy( _pcText, m_pcText, sizeof( m_pcText ) );

	if( m_bMessage )
	{
		m_bMessage = FALSE;

		if( m_dStyle == EDT_STYLE_DYNAMIC )
		{
			ZeroMemory( m_pcText, sizeof( m_pcText ) );
			m_iTextLength = 0;
		}
		return TRUE;
	}
	
	return FALSE;
}

BOOL GUIEdit::OnDown( INT _iX, INT _iY )
{
	return TRUE;
}

BOOL GUIEdit::OnMove( INT _iX, INT _iY )
{	//	GUICursor를 넣어 보자
	return TRUE;
}

BOOL GUIEdit::OnUp( INT _iX, INT _iY )
{
	if( m_dState == EDT_STATE_DISABLE || m_dState == EDT_STATE_HIDDEN )return FALSE;
	
	if( IsPtOnMe( _iX, _iY ) ) 
	{
		EdtMessage = m_dID;
		EnableFocus( TRUE );
		return TRUE;
	}
	else
	{
		if( m_bFocus != FALSE )
			EnableFocus( FALSE );
	}

	return FALSE;
}

BOOL GUIEdit::IsPtOnMe( INT _iX, INT _iY )
{
	POINT pt = { _iX, _iY };

	return ( PtInRect( &m_rtText, pt ) );
}


VOID GUIEdit::TextClear()
{
	ZeroMemory( m_pcText, sizeof( m_pcText ) );
	m_iTextLength = 0;
}
