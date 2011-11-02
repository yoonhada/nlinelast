#include "stdafx.h"
#include "Map.h"
#include "ASEParser.h"
#include "BBXParser.h"
#include "Weapon.h"
#include "CharCube.h"

CWeapon::CWeapon( LPDIRECT3DDEVICE9	_pd3dDevice )
: m_pd3dDevice(_pd3dDevice)
{
	m_pMap = new Map( _pd3dDevice );
	Clear();
}

CWeapon::~CWeapon()
{
	if ( m_WeaponType.nType == SPANNER )
		PrivateProfile( WRITE );
	Release();
}

VOID CWeapon::Clear()
{
}

HRESULT CWeapon::Release()
{
	Clear();
	SAFE_DELETE( m_pMap );
	return S_OK;
}

VOID CWeapon::PrivateProfile(BOOL bRW)
{
	if( bRW == READ )
	{
		m_WeaponType.nFrameBegin[0] = GetPrivateProfileInt( L"Frame", L"Begin0",	 0, WEAPONFILE );
		m_WeaponType.nFrameBegin[1] = GetPrivateProfileInt( L"Frame", L"Begin1",	 0, WEAPONFILE );
		m_WeaponType.nFrameBegin[2] = GetPrivateProfileInt( L"Frame", L"Begin2",	10, WEAPONFILE );
		m_WeaponType.nFrameBegin[3] = GetPrivateProfileInt( L"Frame", L"Begin3",	20, WEAPONFILE );
		m_WeaponType.nFrameTime[0]  = GetPrivateProfileInt( L"Frame", L"Time0",		10, WEAPONFILE );
		m_WeaponType.nFrameTime[1]  = GetPrivateProfileInt( L"Frame", L"Time1",		10, WEAPONFILE );
		m_WeaponType.nFrameTime[2]  = GetPrivateProfileInt( L"Frame", L"Time2",		10, WEAPONFILE );
		m_WeaponType.nFrameTime[3]  = GetPrivateProfileInt( L"Frame", L"Time3",		10, WEAPONFILE );
		m_WeaponType.nFrameAtk[0]   = GetPrivateProfileInt( L"Frame", L"Atk0",		 4, WEAPONFILE );
		m_WeaponType.nFrameAtk[1]   = GetPrivateProfileInt( L"Frame", L"Atk1",		 4, WEAPONFILE );
		m_WeaponType.nFrameAtk[2]   = GetPrivateProfileInt( L"Frame", L"Atk2",		 4, WEAPONFILE );
		m_WeaponType.nFrameAtk[3]   = GetPrivateProfileInt( L"Frame", L"Atk3",		 4, WEAPONFILE );
		m_WeaponType.nDelay[0]		= GetPrivateProfileInt( L"Frame", L"Delay0",	10, WEAPONFILE );
		m_WeaponType.nDelay[1]		= GetPrivateProfileInt( L"Frame", L"Delay1",	10, WEAPONFILE );
		m_WeaponType.nDelay[2]		= GetPrivateProfileInt( L"Frame", L"Delay2",	10, WEAPONFILE );
		m_WeaponType.nDelay[3]		= GetPrivateProfileInt( L"Frame", L"Delay3",	10, WEAPONFILE );

		m_pMap->AddAnimationData ( IDLE,		 EnumCharFrame::BASE, m_WeaponType.nFrameBegin[0], m_WeaponType.nFrameBegin[0], TRUE );
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::ATTACK1, m_WeaponType.nFrameBegin[1], m_WeaponType.nFrameTime[1], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::ATTACK2, m_WeaponType.nFrameBegin[2], m_WeaponType.nFrameTime[2], FALSE);

		m_nState = 0;
		m_nFrame = 0;
		m_WeaponType.vDir[0] = D3DXVECTOR3(-1.0f,  0.0f, 0.0f );
		m_WeaponType.vDir[1] = D3DXVECTOR3(-0.2f, -1.0f, 0.0f );

	}
	else
	{
		WCHAR buf[256];
		wsprintf( buf, L"%d", m_WeaponType.nFrameBegin[0] ); WritePrivateProfileString( L"Frame", L"Begin0",	buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nFrameBegin[1] ); WritePrivateProfileString( L"Frame", L"Begin1",	buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nFrameBegin[2] ); WritePrivateProfileString( L"Frame", L"Begin2",	buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nFrameBegin[3] ); WritePrivateProfileString( L"Frame", L"Begin3",	buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nFrameTime[0] );  WritePrivateProfileString( L"Frame", L"Time0",		buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nFrameTime[1] );  WritePrivateProfileString( L"Frame", L"Time1",		buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nFrameTime[2] );  WritePrivateProfileString( L"Frame", L"Time2",		buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nFrameTime[3] );  WritePrivateProfileString( L"Frame", L"Time3",		buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nFrameAtk[0] );   WritePrivateProfileString( L"Frame", L"Atk0",		buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nFrameAtk[1] );   WritePrivateProfileString( L"Frame", L"Atk1",		buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nFrameAtk[2] );   WritePrivateProfileString( L"Frame", L"Atk2",		buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nFrameAtk[3] );   WritePrivateProfileString( L"Frame", L"Atk3",		buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nDelay[0] );	     WritePrivateProfileString( L"Frame", L"Delay0",	buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nDelay[1] );	     WritePrivateProfileString( L"Frame", L"Delay1",	buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nDelay[2] );	     WritePrivateProfileString( L"Frame", L"Delay2",	buf, WEAPONFILE );
		wsprintf( buf, L"%d", m_WeaponType.nDelay[3] );	     WritePrivateProfileString( L"Frame", L"Delay3",	buf, WEAPONFILE );
	}
}

HRESULT CWeapon::Create()
{
	switch ( m_WeaponType.nType )
	{
	case SPANNER:
		m_pMap->Create( L"ASE File/Spanner.ASE", NULL );		
		PrivateProfile();
		break;
	case FRYPEN:
		m_pMap->Create( L"ASE File/FlyingFan.ASE", NULL );
		PrivateProfile();
		break;
	case GUITAR:
		m_pMap->Create( L"ASE File/Guitar.ASE", NULL );
		PrivateProfile();
		break;
	case MAGICSTICK:
		m_pMap->Create( L"ASE File/MagicStick.ASE", NULL );
		PrivateProfile();
		break;
	default:
		break;
	}


	return S_OK;
}

VOID CWeapon::SetKeyA( const D3DXVECTOR3& vPos, const FLOAT fAngle )
{
	if ( ( m_nState & 0x000F ) == 0x0000 )	//( 0 < m_nFrame && m_nFrame <= m_WeaponType.nDelay && ( m_nState & 0x000F ) == 0x0001 ) || 
	{
		m_nState = ( m_nState & 0x0F0F ) + 0x0001;
		m_nFrame = m_WeaponType.nFrameTime[m_WeaponType.nType];

		m_pMap->SetAnimation( EnumCharFrame::ATTACK1 );
		m_WeaponType.AKeyBB( vPos, fAngle );
	}
}

VOID CWeapon::SetKeyB( const D3DXVECTOR3& vPos, const FLOAT fAngle )
{
	if (  ( m_nState & 0x0F00 ) == 0x0000 ) // ( 0 < m_nFrame && m_nFrame <= m_WeaponType.nDelay && ( m_nState & 0x0F00 ) == 0x0100 ) || 
	{
		m_nState = ( m_nState & 0x0F0F ) + 0x0100;
		m_nFrame = m_WeaponType.nFrameTime[m_WeaponType.nType];

		m_pMap->SetAnimation( EnumCharFrame::ATTACK2 );
		m_WeaponType.BKeyBB( vPos, fAngle );
	}
}

VOID CWeapon::Update()
{
#ifdef _GRAP
	if( CInput::GetInstance()->Get_F9button() )
	{
		CInput::GetInstance()->Set_F9button(FALSE);
		OPENFILENAME OFN;
		WCHAR lpstrFile[MAX_PATH]=L"";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner=CWinBase::GetInstance()->Get_hWnd();
		OFN.lpstrFilter=TEXT("ASE ÆÄÀÏ(*.ASE)\0*.ASE\0");
		OFN.lpstrFile=lpstrFile;
		OFN.nMaxFile=MAX_PATH;
		if (GetOpenFileName(&OFN)!=0)
		{
			WCHAR* ptr = wcstok( lpstrFile, L"." );
			WCHAR Temp[255];
			wsprintf( Temp, L"%s.ASE", ptr );

			m_pMap->Create( Temp, NULL );
			m_WeaponType.nFrameBegin[0] = 0;
			m_WeaponType.nFrameTime[0]  = 200;
			m_WeaponType.nFrameAtk[0]   = 40;
			m_WeaponType.nDelay[0]		= 20;

			m_pMap->AddAnimationData( IDLE,		 EnumCharFrame::BASE, 0, 0, TRUE );
			m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::ATTACK1, m_WeaponType.nFrameBegin[0], m_WeaponType.nFrameTime[0], FALSE);
			m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::ATTACK2, m_WeaponType.nFrameBegin[0], m_WeaponType.nFrameTime[0], FALSE);

		}
	}
	if( CInput::GetInstance()->Get_F8button() )
	{
		PrivateProfile( READ );
	}
#endif // _GRAP

	if ( m_nFrame != 0 )
	{
		if ( m_nFrame == m_WeaponType.nFrameAtk[m_WeaponType.nType] )
		{
			CTree::GetInstance()->GetAtkVector()->push_back(&m_WeaponType.pBBA);
		}

		m_nFrame--;
	}
	else
	{
		m_nState = 0;
	}
	
	//m_pMap->Set_ControlScale( 0, 0f );
	//m_pMap->Set_ControlScale( 1, 0 );
	//m_pMap->Set_ControlScale( 2, 0f );
	//m_pMap->Set_ControlRotate( 0, 0 );
	//m_pMap->Set_ControlRotate( 1, 0 );
	//m_pMap->Set_ControlRotate( 2, 0 );
	//m_pMap->Set_ControlTranslate( 0, 0 );
	//m_pMap->Set_ControlTranslate( 1, 0 );
	//m_pMap->Set_ControlTranslate( 2, 0 );

	m_pMap->Update();
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