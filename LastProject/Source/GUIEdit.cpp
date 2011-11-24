#include "stdafx.h"
#include "GUIEdit.h"
#include "GUIWriting.h"

VOID GUIEdit::Initialize()
{
	m_pFont = NULL;

	memset( m_Data.Str, 0, sizeof( m_Data.Str ) );
}

VOID GUIEdit::Release()
{
	m_pFont->Release();

	SelectObject( m_hDC, m_hOldFont );
	DeleteObject( m_hFont );
	ReleaseDC( GUIWriting::GetInstance().GetHWND(), m_hDC );
}

VOID GUIEdit::CreateDFont( LPWSTR _pFaceName, INT _iWidth, INT _iHeight )
{
	//	Create Directx Font
	if( m_pFont != NULL )
		m_pFont->Release();

	D3DXFONT_DESC desc;
	ZeroMemory( &desc, sizeof( D3DXFONT_DESC ) );

	desc.Width				=	_iWidth;
	desc.Height				=	_iHeight;
	desc.Weight				=	FW_NORMAL;
	desc.MipLevels			=	D3DX_DEFAULT;
	desc.Italic				=	FALSE;
	desc.CharSet			=	DEFAULT_CHARSET;
	desc.OutputPrecision	=	OUT_DEFAULT_PRECIS;
	desc.Quality			=	DEFAULT_QUALITY;
	desc.PitchAndFamily		=	DEFAULT_PITCH;

	lstrcpy( desc.FaceName, _pFaceName );

	if( FAILED( D3DXCreateFontIndirect( GUIBase::m_pd3dDevice, &desc, &m_pFont ) ) )
		MessageBox( NULL, L"D3DXCreateFontIndirect() failed", NULL, MB_OK );

	//	Get HDC
	//	GetTextExtentPoint32W에 들어갈 글자 정보가 있는 HDC를 구한다
	LOGFONTW lf;
	ZeroMemory( &lf, sizeof( LOGFONTW ) );

	lf.lfWidth				= desc.Width;		
	lf.lfHeight				= desc.Height;
	lf.lfWeight				= desc.Weight;
	lf.lfItalic				= desc.Italic;
	lf.lfCharSet			= desc.CharSet;
	lf.lfOutPrecision		= desc.OutputPrecision;
	lf.lfQuality			= desc.Quality;
	lf.lfPitchAndFamily		= desc.PitchAndFamily;

	lstrcpy( lf.lfFaceName, desc.FaceName );

	m_hDC		= GetDC( GUIWriting::GetInstance().GetHWND() );
	m_hFont		= CreateFontIndirectW( &lf );
	m_hOldFont	= (HFONT)SelectObject( m_hDC, m_hFont );
}
BOOL GUIEdit::TextOutput( RECT& _rt, LPCWSTR lpszStr, DWORD Color )
{
	m_pFont->DrawTextW( NULL, lpszStr, -1, &_rt, DT_LEFT, Color );

	return TRUE;
}

VOID GUIEdit::Create( GUIBase::IMAGEPARAM& _imgParam )
{
	// Create Edit
	SetRect(	&m_Data.rtText,
				static_cast<INT>( _imgParam.fX + 10.0f ),
				static_cast<INT>( _imgParam.fY + 5.0f ),
				static_cast<INT>( _imgParam.fX + _imgParam.fWidth ),
				static_cast<INT>( _imgParam.fY + _imgParam.fHeight ) );

	CreateImage( m_Data.Image, _imgParam );

	//	Test Caret Image2D
	GUIBase::IMAGEPARAM imgCaret;

	imgCaret.dPivotType = GUIBase::GBS_TOPLEFT;
	imgCaret.fX			= _imgParam.fX;
	imgCaret.fY			= _imgParam.fY;
	imgCaret.fWidth		= 5.0f;
	imgCaret.fHeight	= 20.0f;

	AddFileName( 0, imgCaret, L"Img\\Caret0.png" );
	AddFileName( 0, imgCaret, L"Img\\Caret1.png" );

	CreateImage2D( m_img2DCaret, imgCaret );

	m_img2DCaret.vec2Tex[ 0 ][ 0 ]->iFrameSpeed = 500;
	m_img2DCaret.vec2Tex[ 0 ][ 1 ]->iFrameSpeed = 500;

	m_vecPosition.x = _imgParam.fX;
	m_vecPosition.y = _imgParam.fY;

	//	Create Font
	CreateDFont( L"휴먼매직체", 10, 20 );
}

VOID GUIEdit::Update()
{
	GUIWriting::GetInstance().GetText( m_Data.Str );

	SIZE Size;
	GetTextExtentPoint32W( m_hDC, m_Data.Str, lstrlen( m_Data.Str ), &Size );
	m_img2DCaret.vecPosition.x = m_vecPosition.x + static_cast<FLOAT>( Size.cx );
}

VOID GUIEdit::Render()
{
	RenderImage( m_Data.Image );
	//RenderImage( m_imgCaret );	
	RenderImage2D( m_img2DCaret );
	TextOutput( m_Data.rtText, m_Data.Str, 0xff000000 );
}