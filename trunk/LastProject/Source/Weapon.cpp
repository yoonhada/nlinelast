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
	Release();
}

VOID CWeapon::Clear()
{
	//ZeroMemory( &m_WeaponType, sizeof( m_WeaponType ) );
	//m_WeaponType.pBBA.Clear();
}

HRESULT CWeapon::Release()
{
	Clear();
	SAFE_DELETE( m_pMap );
	return S_OK;
}

HRESULT CWeapon::Create()
{
	switch ( m_WeaponType.nType )
	{
		
	case SPANNER:
		m_pMap->Create( L"ASE File/Spanner.ASE", NULL );
		m_WeaponType.nFrameBegin[0] = 0;		
		m_WeaponType.nFrameBegin[1] = 0;		
		m_WeaponType.nFrameBegin[2] = 20;		
		m_WeaponType.nFrameBegin[3] = 30;		
		m_WeaponType.nFrameTime[0]  = 0;		
		m_WeaponType.nFrameTime[1]  = 12;		
		m_WeaponType.nFrameTime[2]  = 12;		
		m_WeaponType.nFrameTime[3]  = 12;
		m_WeaponType.nFrameAtk[0] = static_cast<INT>(m_WeaponType.nFrameTime[0] * 0.6f);
		m_WeaponType.nFrameAtk[1] = static_cast<INT>(m_WeaponType.nFrameTime[1] * 0.6f);
		m_WeaponType.nFrameAtk[2] = static_cast<INT>(m_WeaponType.nFrameTime[2] * 0.6f);
		m_WeaponType.nFrameAtk[3] = static_cast<INT>(m_WeaponType.nFrameTime[3] * 0.6f);
		m_WeaponType.nDelay[0] = 12;
		m_WeaponType.nDelay[1] = 12;
		m_WeaponType.nDelay[2] = 12;
		m_WeaponType.nDelay[3] = 12;
		break;
	case FRYPEN:
		m_pMap->Create( L"ASE File/FlyingFan.ASE", NULL );
		m_WeaponType.nFrameBegin[0] = 0;		
		m_WeaponType.nFrameBegin[1] = 0;		
		m_WeaponType.nFrameBegin[2] = 20;		
		m_WeaponType.nFrameBegin[3] = 30;		
		m_WeaponType.nFrameTime[0]  = 12;		
		m_WeaponType.nFrameTime[1]  = 12;		
		m_WeaponType.nFrameTime[2]  = 12;		
		m_WeaponType.nFrameTime[3]  = 12;		
		m_WeaponType.nFrameAtk[0] = static_cast<INT>(m_WeaponType.nFrameTime[0] * 0.6f);
		m_WeaponType.nFrameAtk[1] = static_cast<INT>(m_WeaponType.nFrameTime[1] * 0.6f);
		m_WeaponType.nFrameAtk[2] = static_cast<INT>(m_WeaponType.nFrameTime[2] * 0.6f);
		m_WeaponType.nFrameAtk[3] = static_cast<INT>(m_WeaponType.nFrameTime[3] * 0.6f);
		m_WeaponType.nDelay[0] = 12;
		m_WeaponType.nDelay[1] = 12;
		m_WeaponType.nDelay[2] = 12;
		m_WeaponType.nDelay[3] = 12;
		break;
	case GUITAR:
		m_pMap->Create( L"ASE File/Guitar.ASE", NULL );
		m_WeaponType.nFrameBegin[0] = 0;		
		m_WeaponType.nFrameBegin[1] = 0;		
		m_WeaponType.nFrameBegin[2] = 20;		
		m_WeaponType.nFrameBegin[3] = 30;		
		m_WeaponType.nFrameTime[0]  = 12;		
		m_WeaponType.nFrameTime[1]  = 12;		
		m_WeaponType.nFrameTime[2]  = 12;		
		m_WeaponType.nFrameTime[3]  = 12;	
		m_WeaponType.nFrameAtk[0] = static_cast<INT>(m_WeaponType.nFrameTime[0] * 0.6f);
		m_WeaponType.nFrameAtk[1] = static_cast<INT>(m_WeaponType.nFrameTime[1] * 0.6f);
		m_WeaponType.nFrameAtk[2] = static_cast<INT>(m_WeaponType.nFrameTime[2] * 0.6f);
		m_WeaponType.nFrameAtk[3] = static_cast<INT>(m_WeaponType.nFrameTime[3] * 0.6f);
		m_WeaponType.nDelay[0] = 12;
		m_WeaponType.nDelay[1] = 12;
		m_WeaponType.nDelay[2] = 12;
		m_WeaponType.nDelay[3] = 12;
		break;
	case MAGICSTICK:
		m_pMap->Create( L"ASE File/MagicStick.ASE", NULL );
		m_WeaponType.nFrameBegin[0] = 0;		
		m_WeaponType.nFrameBegin[1] = 0;		
		m_WeaponType.nFrameBegin[2] = 20;		
		m_WeaponType.nFrameBegin[3] = 30;		
		m_WeaponType.nFrameTime[0]  = 12;		
		m_WeaponType.nFrameTime[1]  = 12;		
		m_WeaponType.nFrameTime[2]  = 12;		
		m_WeaponType.nFrameTime[3]  = 12;	
		m_WeaponType.nFrameAtk[0] = static_cast<INT>(m_WeaponType.nFrameTime[0] * 0.6f);
		m_WeaponType.nFrameAtk[1] = static_cast<INT>(m_WeaponType.nFrameTime[1] * 0.6f);
		m_WeaponType.nFrameAtk[2] = static_cast<INT>(m_WeaponType.nFrameTime[2] * 0.6f);
		m_WeaponType.nFrameAtk[3] = static_cast<INT>(m_WeaponType.nFrameTime[3] * 0.6f);
		m_WeaponType.nDelay[0] = 12;
		m_WeaponType.nDelay[1] = 12;
		m_WeaponType.nDelay[2] = 12;
		m_WeaponType.nDelay[3] = 12;
		break;
	default:
		break;
	}

	
	m_pMap->AddAnimationData ( IDLE,		 EnumCharFrame::BASE, m_WeaponType.nFrameBegin[0], m_WeaponType.nFrameBegin[0], TRUE );
	m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::ATTACK1, m_WeaponType.nFrameBegin[1], m_WeaponType.nFrameTime[1], FALSE);
	m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::ATTACK2, m_WeaponType.nFrameBegin[2], m_WeaponType.nFrameTime[2], FALSE);

	m_nState = 0;
	m_nFrame = 0;
	m_WeaponType.vDir[0] = D3DXVECTOR3(-1.0f,  0.0f, 0.0f );
	m_WeaponType.vDir[1] = D3DXVECTOR3(-0.2f, -1.0f, 0.0f );

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
//#ifdef _GRAP
//	if( CInput::GetInstance()->Get_F9button() )
//	{
//		CInput::GetInstance()->Set_F9button(FALSE);
//		OPENFILENAME OFN;
//		WCHAR lpstrFile[MAX_PATH]=L"";
//
//		memset(&OFN, 0, sizeof(OPENFILENAME));
//		OFN.lStructSize = sizeof(OPENFILENAME);
//		OFN.hwndOwner=CWinBase::GetInstance()->Get_hWnd();
//		OFN.lpstrFilter=TEXT("ASE ÆÄÀÏ(*.ASE)\0*.ASE\0");
//		OFN.lpstrFile=lpstrFile;
//		OFN.nMaxFile=MAX_PATH;
//		if (GetOpenFileName(&OFN)!=0)
//		{
//			WCHAR* ptr = wcstok( lpstrFile, L"." );
//			WCHAR Temp[255];
//			wsprintf( Temp, L"%s.ASE", ptr );
//
//			m_pMap->Create( Temp, NULL );
//			m_WeaponType.nAKeyFrameBegin = 1;		
//			m_WeaponType.nAKeyFrameTime  = 299;		
//			m_WeaponType.nBKeyFrameBegin = 30;
//			m_WeaponType.nBKeyFrameTime  = 10;
//			m_WeaponType.nDelay = 12;
//
//			m_pMap->AddAnimationData( IDLE,		 EnumCharFrame::BASE, 0, 0, TRUE );
//			m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::ATTACK1, 
//				m_WeaponType.nAKeyFrameBegin, m_WeaponType.nAKeyFrameTime, FALSE);
//			m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::ATTACK2, 
//				m_WeaponType.nBKeyFrameBegin, m_WeaponType.nAKeyFrameTime, FALSE);
//
//		}
//	}
//#endif // _GRAP

	if ( ( m_nFrame == m_WeaponType.nFrameAtk[m_WeaponType.nType] ) && m_nFrame != 0 )
	{
		CTree::GetInstance()->GetAtkVector()->push_back(&m_WeaponType.pBBA);
		m_nFrame--;
	}
	else if ( m_nFrame != 0 )
	{
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