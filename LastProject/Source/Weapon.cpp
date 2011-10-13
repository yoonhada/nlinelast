#include "stdafx.h"
#include "Weapon.h"

CWeapon::CWeapon( )
: m_nFrame(16)
, m_nState(0)
, m_nKeyFrame(0)
{
	Create();
}

CWeapon::~CWeapon()
{
	Release();
}

HRESULT CWeapon::Release()
{
	return S_OK;
}

HRESULT CWeapon::Create()
{
	Release();

	m_nState = 0;
	m_nKeyFrame = 0;

	return S_OK;
}

VOID CWeapon::Update()
{
}

VOID CWeapon::Render()
{
	if (m_nKeyFrame > 0)
		m_nKeyFrame--;
}

VOID CWeapon::SetKeyA()
{
	if( m_nKeyFrame == 0 ){
		if ( ( m_nState & 0x000F ) )	m_nState += ( m_nState & 0x000F );
		else							m_nState = 0x0001;

		m_nKeyFrame = m_nFrame;
	}
}

VOID CWeapon::SetKeyB()
{
	if( m_nKeyFrame == 0 ){
		if ( ( m_nState & 0x00F0 ) )	m_nState += ( m_nState & 0x00F0 );
		else							m_nState = 0x0010;

		m_nKeyFrame = m_nFrame;
	}
}