#include "stdafx.h"
#include "GUIBackground.h"

VOID GUIBackground::Initialize()
{
	m_dState = GUIBackground::BGD_NORMAL;
}

VOID GUIBackground::Release()
{
}

VOID GUIBackground::Create( IMAGEPARAM& _imgParam )
{
	CreateImage3D( m_Data.Image3D, _imgParam );
}

VOID GUIBackground::Create( FLOAT _fX, FLOAT _fY, FLOAT _fWidht, FLOAT _fHeight, IMAGEPARAM& _imgParam )
{
	CreateImage3D( m_Data.Image3D, _fX, _fY, _fWidht, _fHeight, _imgParam );
}

VOID GUIBackground::Update()
{
}

VOID GUIBackground::Render()
{
	if( m_dState == BGD_HIDDEN )
		return;

	RenderImage3D( &m_Data.Image3D );
}

VOID GUIBackground::SetPosition( LPD3DXVECTOR3 _pvecPosition )
{
	Image3DTranslate( &m_Data.Image3D, _pvecPosition->x, _pvecPosition->y, _pvecPosition->z );
}

VOID GUIBackground::SetState( DWORD _dState )
{
	m_dState = _dState;
}

BOOL GUIBackground::SelectAnimation( INT _iNumAni )
{
	INT iAnimationSize = m_Data.Image3D.vec2Tex.size();

	if( _iNumAni >= iAnimationSize )
	{
		MessageBox( NULL, L"GUIBackground::SelectAnimation(){ _iNumAni >= iAnimationSize }", NULL, MB_OK );
		return FALSE;
	}

	m_Data.Image3D.iCurrentAnimation	= _iNumAni;
	m_Data.Image3D.iCurrentFrame		= 0;

	return TRUE;
}

BOOL GUIBackground::NextAnimation()
{
	INT iAnimationSize = m_Data.Image3D.vec2Tex.size();
	if( m_Data.Image3D.iCurrentAnimation + 1 >= iAnimationSize )
	{
		m_Data.Image3D.iCurrentAnimation	= 0;
		m_Data.Image3D.iCurrentFrame		= 0;
		return TRUE;
	}

	m_Data.Image3D.iCurrentAnimation++;
	m_Data.Image3D.iCurrentFrame = 0;

	return FALSE;
}