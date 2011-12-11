#include "stdafx.h"
#include "GUICaret.h"
#include "GUIFont.h"

VOID GUICaret::Initialize()
{
	m_pText = NULL;
}

VOID GUICaret::Release()
{
}

VOID GUICaret::Create( GUIBase::IMAGEPARAM& _imgParam )
{
	CreateImage3D( m_img3DCaret, _imgParam );
}

VOID GUICaret::Render()
{
	if( m_pText == NULL )
		return;

	RenderImage3D( &m_img3DCaret );
}

VOID GUICaret::Update()
{
	if( m_pText == NULL )
		return;

	SIZE Size;
	GUIFont::GetInstance().GetTextSize( m_pText, Size );
	m_img3DCaret.vecTrans.x = static_cast<FLOAT>( m_fX + Size.cx );
}

VOID GUICaret::SetSize( INT _iWidth, INT _iHeight )
{
	FLOAT fWidth	= static_cast<FLOAT>( _iWidth );
	FLOAT fHeight	= static_cast<FLOAT>( _iHeight );

	Image3DScale( &m_img3DCaret, fWidth, fHeight, 1.0f );
}

VOID GUICaret::ConnectText( LPWSTR _pText, FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight )
{
	m_pText = _pText;

	m_fX = _fX;
	m_fY = _fY;

	Image3DScale( &m_img3DCaret, _fWidth, _fHeight, 1.0f );
	Image3DTranslate( &m_img3DCaret, _fX, _fY, 0.0f );
}

VOID GUICaret::DisconnectText()
{
	m_pText = NULL;
}