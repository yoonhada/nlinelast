#include "stdafx.h"
#include "GUIBackground.h"

VOID GUIBackground::Initialize()
{
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
	RenderImage3D( &m_Data.Image3D );
}

BOOL GUIBackground::NextAnimation()
{
	INT iAnimationSize = m_Data.Image3D.vec2Tex.size();
	if( m_Data.Image3D.iCurrentAnimation + 1 >= iAnimationSize )
	{
		m_Data.Image3D.iCurrentAnimation = 0;
		return FALSE;
	}

	m_Data.Image3D.iCurrentAnimation++;

	return TRUE;
}