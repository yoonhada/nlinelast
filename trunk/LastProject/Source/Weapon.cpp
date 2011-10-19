#include "stdafx.h"
#include "Map.h"
#include "Weapon.h"

CWeapon::CWeapon( LPDIRECT3DDEVICE9	_pd3dDevice )
: Map( _pd3dDevice )
{
	Clear();
}

CWeapon::~CWeapon()
{
	Release();
}

VOID CWeapon::Clear()
{
	m_nFrame = 0;
	m_nState = 0;
	m_nWeaponType = -1;
	m_nAKeyFrameTime = 0;
	m_nBKeyFrameTime = 0;
	m_nDelay = 12;
}

HRESULT CWeapon::Release()
{
	Clear();
	return S_OK;
}

HRESULT CWeapon::Create()
{
	assert(m_nWeaponType != -1);
	Clear();

	// 한번에 읽어서 로딩????
	// 개별파일로 만들어 로딩????
	switch (m_nWeaponType)
	{
	case 0:
		Map::Create( L"ASE File/Weapon.ASE", NULL );
		//후딜 포함 프레임
		m_nAKeyFrameTime = 20;		
		m_nBKeyFrameTime = 50;
#ifdef _DEBUG
		m_nDelay = 50;
#else
		m_nDelay = 12;
#endif // _DEBUG
		break;
	case 1:
		Map::Create( L"ASE File/Weapon.ASE", NULL );
		m_nAKeyFrameTime = 20;
		m_nBKeyFrameTime = 50;
#ifdef _DEBUG
		m_nDelay = 50;
#else
		m_nDelay = 12;
#endif // _DEBUG
		break;
	case 2:
		Map::Create( L"ASE File/Weapon.ASE", NULL );
		m_nAKeyFrameTime = 20;
		m_nBKeyFrameTime = 50;
#ifdef _DEBUG
		m_nDelay = 50;
#else
		m_nDelay = 12;
#endif // _DEBUG
		break;
	case 3:
		Map::Create( L"ASE File/Weapon.ASE", NULL );
		m_nAKeyFrameTime = 20;
		m_nBKeyFrameTime = 50;
#ifdef _DEBUG
		m_nDelay = 50;
#else
		m_nDelay = 12;
#endif // _DEBUG
		break;
	default:
		break;
	}

	m_nState = 0;
	m_nFrame = 0;

	return S_OK;
}

VOID CWeapon::SetKeyA()
{
	// 콤보
	if ( ( 0 < m_nFrame && m_nFrame <= m_nDelay && 
		( m_nState & 0x000F ) == 0x0001 ) || 
		( 0x0000 == ( m_nState & 0x000F ) ) )
	{
		m_nState = ( m_nState & 0x0F0F ) + 0x0001;
		m_nFrame = m_nAKeyFrameTime;
	}
}

VOID CWeapon::SetKeyB()
{
	// 콤보
	if ( ( 0 < m_nFrame && m_nFrame <= m_nDelay && 
		( m_nState & 0x0F00 ) == 0x0100 ) || 
		( 0x0000 == ( m_nState & 0x0F00 ) ) )
	{
		m_nState = ( m_nState & 0x0F0F ) + 0x0100;
		m_nFrame = m_nBKeyFrameTime;
	}
}

VOID CWeapon::Update()
{	
	FLOAT fCharSize = 20;
	FLOAT fWeapSize = 10;


	if ( ( m_nState & 0x000F ) == 0x0001 ) 
	{
		Set_ControlTranslate( 0, -( fCharSize + fWeapSize )  );
	}
	else if ( ( m_nState & 0x000F ) == 0x0002 ) 
	{
		Set_ControlTranslate( 0, ( fCharSize + fWeapSize ) );
	}
	else
	{
		Set_ControlTranslate( 0, 0 );
	}

	if ( ( m_nState & 0x0F00 ) == 0x0100 ) 
	{
		Set_ControlTranslate( 1, fWeapSize );
		Set_ControlTranslate( 2, 0 );
	}
	else if ( ( m_nState & 0x0F00 ) == 0x0200 )
	{
		Set_ControlTranslate( 1, 0 );
		Set_ControlTranslate( 2, -fWeapSize );
	}
	else 
	{
		Set_ControlTranslate( 1, 0 );
		Set_ControlTranslate( 2, 0 );
	}

	Calcul_MatWorld();
};

VOID CWeapon::Render()
{
	if ( m_nFrame > 0 )
	{
		Map::Render();
		m_nFrame--;
	}
	else
	{
		m_nState = 0x0000;
	}
}