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
	CreateImage( m_Data.Image, _imgParam );
}

VOID GUIBackground::Update()
{
}

VOID GUIBackground::Render()
{
	RenderImage( m_Data.Image );
}

BOOL GUIBackground::NextAnimation()
{
	if( m_Data.Image.iCurrentAnimation + 1 >= m_Data.Image.vec2Tex.size() )
	{
		m_Data.Image.iCurrentAnimation = 0;
		return FALSE;
	}

	m_Data.Image.iCurrentAnimation++;

	return TRUE;
}