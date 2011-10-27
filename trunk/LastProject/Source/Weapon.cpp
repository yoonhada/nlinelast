#include "stdafx.h"
#include "Map.h"
#include "ASEParser.h"
#include "BBXParser.h"
#include "Weapon.h"

CWeapon::CWeapon( LPDIRECT3DDEVICE9	_pd3dDevice )
{
	m_pMap = new Map( _pd3dDevice );
	Clear();
}

CWeapon::~CWeapon()
{
	Release();
}

VOID CWeapon::Clear()
{
	ZeroMemory( &m_WeaponType, sizeof( m_WeaponType ) );
}

HRESULT CWeapon::Release()
{
	Clear();
	SAFE_DELETE( m_pMap );
	return S_OK;
}

HRESULT CWeapon::Create()
{
	switch ( m_WeaponType.nWeaponType )
	{
	case 0:
		m_pMap->Create( L"ASE File/Spanner.ASE", NULL );
		m_WeaponType.nAKeyFrameTime = 20;		
		m_WeaponType.nBKeyFrameTime = 50;
		m_WeaponType.nDelay = 12;
		break;
	case 1:
		m_pMap->Create( L"ASE File/FlyingFan.ASE", NULL );
		m_WeaponType.nAKeyFrameTime = 20;
		m_WeaponType.nBKeyFrameTime = 50;
		m_WeaponType.nDelay = 12;
		break;
	case 2:
		m_pMap->Create( L"ASE File/Guitar.ASE", NULL );
		m_WeaponType.nAKeyFrameTime = 20;
		m_WeaponType.nBKeyFrameTime = 50;
		m_WeaponType.nDelay = 50;
		break;
	case 3:
		m_pMap->Create( L"ASE File/MagicStick.ASE", NULL );
		m_WeaponType.nAKeyFrameTime = 20;
		m_WeaponType.nBKeyFrameTime = 50;
		m_WeaponType.nDelay = 12;
		break;
	default:
		break;
	}

	m_pMap->AddAnimationData( ASEANI_IDLE, 0, 0, 0, TRUE );
	m_pMap->AddAnimationData(ASEANI_POST_IDLE, 1, 1, 30, FALSE);


	m_nState = 0;
	m_nFrame = 0;

	return S_OK;
}

INT CWeapon::SetKeyA()
{
	m_nState = 1;
	//if ( ( 0 < m_nFrame && m_nFrame <= m_WeaponType.nDelay && ( m_nState & 0x000F ) == 0x0001 ) || 
	//	 ( ( m_nState & 0x000F ) == 0x0000 ) )
	//{
	//	m_nState = ( m_nState & 0x0F0F ) + 0x0001;
	//	m_nFrame = m_WeaponType.nAKeyFrameTime;
	//}
	m_pMap->SetAnimation( m_nState );
	return m_nState;
}

INT CWeapon::SetKeyB()
{
	// ÄÞº¸
	if ( ( 0 < m_nFrame && m_nFrame <= m_WeaponType.nDelay && ( m_nState & 0x0F00 ) == 0x0100 ) || 
		 ( ( m_nState & 0x0F00 ) == 0x0000 ) )
	{
		m_nState = ( m_nState & 0x0F0F ) + 0x0100;
		m_nFrame = m_WeaponType.nBKeyFrameTime;
	}

	return m_nState;
}

VOID CWeapon::Update()
{	
	FLOAT fCharSize = 20;
	FLOAT fWeapSize = 10;

	m_pMap->Update();
	
	//if ( ( m_nState & 0x000F ) == 0x0001 ) 
	//{
	//	m_pMap->SetAnimation( 1 );
	//}
	//else if ( ( m_nState & 0x000F ) == 0x0002 ) 
	//{
	//	m_pMap->SetAnimation( 2 );
	//}
	//else
	//{
	//	m_pMap->SetAnimation( 0 );
	//}

	//if ( ( m_nState & 0x0F00 ) == 0x0100 ) 
	//{
	//	m_pMap->SetAnimation( 3 );
	//}
	//else if ( ( m_nState & 0x0F00 ) == 0x0200 )
	//{
	//	m_pMap->SetAnimation( 4 );
	//}
	//else 
	//{
	//	m_pMap->SetAnimation( 0 );
	//}

	//m_pMap->Set_ControlScale( 0, 0 );
	//m_pMap->Set_ControlScale( 1, 0 );
	//m_pMap->Set_ControlScale( 2, 0 );
	m_pMap->Set_ControlRotate( 0, 0 );
	m_pMap->Set_ControlRotate( 1, 0 );
	m_pMap->Set_ControlRotate( 2, 0 );
	m_pMap->Set_ControlTranslate( 0, 0 );
	m_pMap->Set_ControlTranslate( 1, 0 );
	m_pMap->Set_ControlTranslate( 2, 0 );

	m_pMap->Calcul_MatWorld();
};

const D3DXMATRIXA16& CWeapon::Get_MatWorld()
{
	return m_pMap->Get_MatWorld(); 
}

VOID CWeapon::Render( D3DXMATRIX _matCharacter )
{
	m_pMap->Render( _matCharacter );
}