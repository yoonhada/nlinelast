#define _GRAP

#include "stdafx.h"
#include "Map.h"
#include "ASEParser.h"
#include "BBXParser.h"
#include "Weapon.h"
#include "CharCube.h"

CWeapon::CWeapon( LPDIRECT3DDEVICE9	_pd3dDevice )
: m_pD3dDevice(_pd3dDevice)
, m_pMap(NULL)
{
	Clear();
}

CWeapon::~CWeapon()
{
	Release();
}

VOID CWeapon::Clear()
{
	for (int i = 0; i < 10; ++i)
	{
		m_WeaponType.nFrameBegin[i] = 0;
		m_WeaponType.nFrameTime[i] = 0;
		m_WeaponType.nFrameAtk[i] = 0;
		m_WeaponType.nDelay[i] = 0;
		m_WeaponType.vDir[i].x = m_WeaponType.vDir[i].y = m_WeaponType.vDir[i].z = 0.0f;
	}

	m_fZAng[0] = MTP_FUN::Deg2Rad<30>::radians;		// D3DXToRadian( 0.0f);
	m_fZAng[1] = MTP_FUN::Deg2Rad<30>::radians;		// D3DXToRadian( 0.0f);
	m_fZAng[2] = MTP_FUN::Deg2Rad<30>::radians;		// D3DXToRadian( 0.0f);
	m_fZAng[3] = -MTP_FUN::Deg2Rad<90>::radians;	// D3DXToRadian( -90.0f);
	m_fZAng[4] = -MTP_FUN::Deg2Rad<15>::radians;	// D3DXToRadian( -15.0f);
	m_fZAng[5] = MTP_FUN::Deg2Rad<30>::radians;		// D3DXToRadian( 0.0f);
	m_fZAng[6] = MTP_FUN::Deg2Rad<45>::radians;		// D3DXToRadian( 45.0f);
	m_fZAng[7] = MTP_FUN::Deg2Rad<90>::radians;		// D3DXToRadian( 90.0f);
	m_fZAng[8] = MTP_FUN::Deg2Rad<135>::radians;	// D3DXToRadian( 135.0f);
	m_fZAng[9] = MTP_FUN::Deg2Rad<30>::radians;		// D3DXToRadian( 0.0f);

	// XYZ Min
	m_fBBSize[0] =-10.5f;
	m_fBBSize[1] =- 1.5f;
	m_fBBSize[2] =-16.5f;
	// XYZ Max
	m_fBBSize[3] =  7.5f;
	m_fBBSize[4] =  1.5f;
	m_fBBSize[5] =  7.5f;

	m_fScale = 0;
	m_bAtkTime = FALSE;
}

HRESULT CWeapon::Release()
{
	SAFE_DELETE( m_pMap );

	return S_OK;
}

VOID CWeapon::_GetProfileInt( INT *nOut, LPWSTR lpAppName, LPWSTR lpszBuf, INT nIndex, INT nDefault )
{
	WCHAR lpKeyName[256];
	wsprintf( lpKeyName, L"%s%d", lpszBuf, nIndex );
	nOut[nIndex] = GetPrivateProfileInt( lpAppName, lpKeyName, nDefault, WEAPONFILE );
}

VOID CWeapon::_GetProfileVector( D3DXVECTOR3 *vVec, LPWSTR lpAppName, LPWSTR lpszBuf, INT nIndex, INT nDefaultX, INT nDefaultY, INT nDefaultZ, FLOAT fPower )
{
	FLOAT x, y, z;
	WCHAR lpKeyName[256];
	wsprintf( lpKeyName, L"%s%dX", lpszBuf, nIndex );
	x = fPower * GetPrivateProfileInt( lpAppName, lpKeyName, nDefaultX, WEAPONFILE );
	wsprintf( lpKeyName, L"%s%dY", lpszBuf, nIndex );
	y = fPower * GetPrivateProfileInt( lpAppName, lpKeyName, nDefaultY, WEAPONFILE );
	wsprintf( lpKeyName, L"%s%dZ", lpszBuf, nIndex );
	z = fPower * GetPrivateProfileInt( lpAppName, lpKeyName, nDefaultZ, WEAPONFILE );
	vVec[nIndex] = D3DXVECTOR3(x, y, z); 
}

VOID CWeapon::PrivateProfile( LPWSTR lpwStr, BOOL bRW )
{
	if( bRW == READ )
	{
		m_pMap->CleanupAnimationData();
		
		_GetProfileInt( m_WeaponType.nFrameBegin, lpwStr, L"Begin", EnumCharFrame::BASE,	 0 );
		_GetProfileInt( m_WeaponType.nFrameBegin, lpwStr, L"Begin", EnumCharFrame::ATTACK1,	 1 );
		_GetProfileInt( m_WeaponType.nFrameBegin, lpwStr, L"Begin", EnumCharFrame::ATTACK2,	25 );
		_GetProfileInt( m_WeaponType.nFrameBegin, lpwStr, L"Begin", EnumCharFrame::A,		49 );
		_GetProfileInt( m_WeaponType.nFrameBegin, lpwStr, L"Begin", EnumCharFrame::TEMP1,	73 );
		_GetProfileInt( m_WeaponType.nFrameBegin, lpwStr, L"Begin", EnumCharFrame::TEMP2,	96 );
		_GetProfileInt( m_WeaponType.nFrameBegin, lpwStr, L"Begin", EnumCharFrame::TEMP3,	119 );
		_GetProfileInt( m_WeaponType.nFrameBegin, lpwStr, L"Begin", EnumCharFrame::TEMP4,	141 );
		_GetProfileInt( m_WeaponType.nFrameTime,  lpwStr, L"Time" , EnumCharFrame::BASE,	 0 );
		_GetProfileInt( m_WeaponType.nFrameTime,  lpwStr, L"Time" , EnumCharFrame::ATTACK1,	24 );
		_GetProfileInt( m_WeaponType.nFrameTime,  lpwStr, L"Time" , EnumCharFrame::ATTACK2,	24 );
		_GetProfileInt( m_WeaponType.nFrameTime,  lpwStr, L"Time" , EnumCharFrame::A,		24 );
		_GetProfileInt( m_WeaponType.nFrameTime,  lpwStr, L"Time" , EnumCharFrame::TEMP1,	23 );
		_GetProfileInt( m_WeaponType.nFrameTime,  lpwStr, L"Time" , EnumCharFrame::TEMP2,	23 );
		_GetProfileInt( m_WeaponType.nFrameTime,  lpwStr, L"Time" , EnumCharFrame::TEMP3,	23 );
		_GetProfileInt( m_WeaponType.nFrameTime,  lpwStr, L"Time" , EnumCharFrame::TEMP4,	23 );
		_GetProfileInt( m_WeaponType.nFrameAtk,   lpwStr, L"Atk"  , EnumCharFrame::BASE,	10 );
		_GetProfileInt( m_WeaponType.nFrameAtk,   lpwStr, L"Atk"  , EnumCharFrame::ATTACK1,	10 );
		_GetProfileInt( m_WeaponType.nFrameAtk,   lpwStr, L"Atk"  , EnumCharFrame::ATTACK2,	10 );
		_GetProfileInt( m_WeaponType.nFrameAtk,   lpwStr, L"Atk"  , EnumCharFrame::A,		10 );
		_GetProfileInt( m_WeaponType.nFrameAtk,   lpwStr, L"Atk"  , EnumCharFrame::TEMP1,	10 );
		_GetProfileInt( m_WeaponType.nFrameAtk,   lpwStr, L"Atk"  , EnumCharFrame::TEMP2,	10 );
		_GetProfileInt( m_WeaponType.nFrameAtk,   lpwStr, L"Atk"  , EnumCharFrame::TEMP3,	10 );
		_GetProfileInt( m_WeaponType.nFrameAtk,   lpwStr, L"Atk"  , EnumCharFrame::TEMP4,	10 );
		_GetProfileInt( m_WeaponType.nDelay,      lpwStr, L"Delay", EnumCharFrame::BASE,	16 );
		_GetProfileInt( m_WeaponType.nDelay,      lpwStr, L"Delay", EnumCharFrame::ATTACK1,	16 );
		_GetProfileInt( m_WeaponType.nDelay,      lpwStr, L"Delay", EnumCharFrame::ATTACK2,	16 );
		_GetProfileInt( m_WeaponType.nDelay,      lpwStr, L"Delay", EnumCharFrame::A,		16 );
		_GetProfileInt( m_WeaponType.nDelay,      lpwStr, L"Delay", EnumCharFrame::TEMP1,	16 );
		_GetProfileInt( m_WeaponType.nDelay,      lpwStr, L"Delay", EnumCharFrame::TEMP2,	16 );
		_GetProfileInt( m_WeaponType.nDelay,      lpwStr, L"Delay", EnumCharFrame::TEMP3,	16 );
		_GetProfileInt( m_WeaponType.nDelay,      lpwStr, L"Delay", EnumCharFrame::TEMP4,	16 );

		_GetProfileVector( m_WeaponType.vDir, lpwStr, L"Dir", EnumCharFrame::BASE,	   0,   0, - 3, 0.1f );
		_GetProfileVector( m_WeaponType.vDir, lpwStr, L"Dir", EnumCharFrame::ATTACK1,  0, -13, - 3, 0.1f );
		_GetProfileVector( m_WeaponType.vDir, lpwStr, L"Dir", EnumCharFrame::ATTACK2,-10, - 3, - 3, 0.1f );
		_GetProfileVector( m_WeaponType.vDir, lpwStr, L"Dir", EnumCharFrame::A,		  13,   0, - 3, 0.1f );
		_GetProfileVector( m_WeaponType.vDir, lpwStr, L"Dir", EnumCharFrame::TEMP1,	   3, -10, - 3, 0.1f );
		_GetProfileVector( m_WeaponType.vDir, lpwStr, L"Dir", EnumCharFrame::TEMP2,	   0, -13, - 3, 0.1f );
		_GetProfileVector( m_WeaponType.vDir, lpwStr, L"Dir", EnumCharFrame::TEMP3,	 - 3, -10, - 3, 0.1f );
		_GetProfileVector( m_WeaponType.vDir, lpwStr, L"Dir", EnumCharFrame::TEMP4,	 -13,   0, - 3, 0.1f );
		
		m_pMap->AddAnimationData ( IDLE		, EnumCharFrame::BASE  , m_WeaponType.nFrameBegin[EnumCharFrame::BASE	 ], m_WeaponType.nFrameTime[EnumCharFrame::BASE	  ], TRUE );
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::ATTACK1, m_WeaponType.nFrameBegin[EnumCharFrame::ATTACK1], m_WeaponType.nFrameTime[EnumCharFrame::ATTACK1], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::ATTACK2, m_WeaponType.nFrameBegin[EnumCharFrame::ATTACK2], m_WeaponType.nFrameTime[EnumCharFrame::ATTACK2], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::A		, m_WeaponType.nFrameBegin[EnumCharFrame::A		 ], m_WeaponType.nFrameTime[EnumCharFrame::A	  ], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::TEMP1  , m_WeaponType.nFrameBegin[EnumCharFrame::TEMP1  ], m_WeaponType.nFrameTime[EnumCharFrame::TEMP1  ], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::TEMP2  , m_WeaponType.nFrameBegin[EnumCharFrame::TEMP2  ], m_WeaponType.nFrameTime[EnumCharFrame::TEMP2  ], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::TEMP3  , m_WeaponType.nFrameBegin[EnumCharFrame::TEMP3  ], m_WeaponType.nFrameTime[EnumCharFrame::TEMP3  ], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::TEMP4  , m_WeaponType.nFrameBegin[EnumCharFrame::TEMP4  ], m_WeaponType.nFrameTime[EnumCharFrame::TEMP4  ], FALSE);

		m_nState = EnumCharFrame::BASE;
	}
}

HRESULT CWeapon::Create()
{
	m_pMap = new Map( m_pD3dDevice );

	switch ( m_WeaponType.nType )
	{
	case SPANNER:
		m_pMap->Create( L"ASE File/Spanner.ASE", NULL );		
		PrivateProfile( L"FRAME1" );
		break;
	case FRYPEN:
		m_pMap->Create( L"ASE File/FlyingFan.ASE", NULL );
		PrivateProfile( L"FRAME2" );
		break;
	case GUITAR:
		m_pMap->Create( L"ASE File/Guitar.ASE", NULL );
		PrivateProfile( L"FRAME3" );
		break;
	case MAGICSTICK:
		m_pMap->Create( L"ASE File/MagicStick.ASE", NULL );
		PrivateProfile( L"FRAME4" );
		break;
	default:
		break;
	}

	return S_OK;
}

VOID CWeapon::SetKeyA()
{
	INT nCurrFrame = m_pMap->GetCurrentFrame();

	if ( m_nState == EnumCharFrame::ATTACK2 &&
		( nCurrFrame > ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nDelay[m_nState] ) ) && 
		( nCurrFrame < ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] ) ) )
	{
		m_bAtkTime = FALSE;
		m_nState = EnumCharFrame::A;
		m_pMap->SetAnimation( m_nState );
		CNetwork::GetInstance()->CS_Player_Attack_Animation( m_nState );

		CSound::GetInstance()->PlayEffect( 0 );
	}
	else if ( m_nState == EnumCharFrame::ATTACK1 && 
		( nCurrFrame > ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nDelay[m_nState] ) ) && 
		( nCurrFrame < ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] ) ) )
	{
		m_bAtkTime = FALSE;
		m_nState = EnumCharFrame::ATTACK2;
		m_pMap->SetAnimation( m_nState );
		CNetwork::GetInstance()->CS_Player_Attack_Animation( m_nState );

		CSound::GetInstance()->PlayEffect( 0 );
	}
	else if ( m_nState == EnumCharFrame::BASE && nCurrFrame == 0 )
	{
		m_fScale = 1.0f;
		m_bAtkTime = FALSE;
		m_nState = EnumCharFrame::ATTACK1;
		m_pMap->SetAnimation( m_nState );
		CNetwork::GetInstance()->CS_Player_Attack_Animation( m_nState );

		CSound::GetInstance()->PlayEffect( 0 );
	} 
}

VOID CWeapon::SetKeyB()
{
	INT nCurrFrame = m_pMap->GetCurrentFrame();

	if ( m_nState == EnumCharFrame::ATTACK2 &&
		( m_pMap->GetCurrentFrame() > ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nDelay[m_nState] ) ) && 
		( m_pMap->GetCurrentFrame() < ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] ) ) )
	{
		m_fScale = 2.0f;
		m_bAtkTime = FALSE;
		m_nState = EnumCharFrame::TEMP4;
		m_pMap->SetAnimation( m_nState );
		CNetwork::GetInstance()->CS_Player_Attack_Animation( m_nState );

		CSound::GetInstance()->PlayEffect( 0 );
	}
	//else if ( m_nState == EnumCharFrame::TEMP2 &&
	//	( m_pMap->GetCurrentFrame() > ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nDelay[m_nState] ) ) && 
	//	( m_pMap->GetCurrentFrame() < ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] ) ) )
	//{
	//	m_bAtkTime = FALSE;
	//	m_nState = EnumCharFrame::TEMP3;
	//	m_pMap->SetAnimation( m_nState );
	//	CNetwork::GetInstance()->CS_UTOM_Attack_Animation( m_nState );
	//}
	else if ( m_nState == EnumCharFrame::TEMP1 &&
		( m_pMap->GetCurrentFrame() > ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nDelay[m_nState] ) ) && 
		( m_pMap->GetCurrentFrame() < ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] ) ) )
	{
		m_bAtkTime = FALSE;
		m_nState = EnumCharFrame::TEMP3;
		m_pMap->SetAnimation( m_nState );
		CNetwork::GetInstance()->CS_Player_Attack_Animation( m_nState );

		CSound::GetInstance()->PlayEffect( 0 );
	}
	else if ( m_nState == EnumCharFrame::BASE  && nCurrFrame == 0 )
	{
		m_fScale = 1.0f;
		m_bAtkTime = FALSE;
		m_nState = EnumCharFrame::TEMP1;
		m_pMap->SetAnimation( m_nState );
		CNetwork::GetInstance()->CS_Player_Attack_Animation( m_nState );

		CSound::GetInstance()->PlayEffect( 0 );
	}
}

VOID CWeapon::AddAtkBBx( D3DXVECTOR3 &vPos, FLOAT fAngle )
{
	SetBBx( vPos, fAngle );
	CTree::GetInstance()->GetCharAtkVector()->push_back( &m_WeaponType.pBBA );
}

VOID CWeapon::SetBBx( const D3DXVECTOR3& vPos, const FLOAT fAngle )
{
	m_WeaponType.pBBA.SetPosition( vPos );
	m_WeaponType.pBBA.SetScale( m_fScale );
	m_WeaponType.pBBA.SetAngleY( fAngle );
	m_WeaponType.pBBA.SetAngleZ( m_fZAng[m_nState] );

	m_WeaponType.pBBA.SetSize( 0, m_fBBSize[0] );
	m_WeaponType.pBBA.SetSize( 1, m_fBBSize[1] );
	m_WeaponType.pBBA.SetSize( 2, m_fBBSize[2] );

	m_WeaponType.pBBA.SetSize( 3, m_fBBSize[3] );
	m_WeaponType.pBBA.SetSize( 4, m_fBBSize[4] );
	m_WeaponType.pBBA.SetSize( 5, m_fBBSize[5] );
	
	m_WeaponType.pBBA.SetDirection( m_WeaponType.vDir[m_nState] );
}

VOID CWeapon::Update()
{
	INT nCurrFrame = m_pMap->GetCurrentFrame();

	if ( m_nState != EnumCharFrame::BASE )
	{
		//Ÿ�ݼ���
		if ( m_bAtkTime == TRUE)
		{
			m_bAtkTime = 2;
		}
		else if ( m_bAtkTime != 2 && ( nCurrFrame >= m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameAtk[m_nState] ) )
		{
			m_bAtkTime = TRUE;			
		}
		else if ( nCurrFrame == 0 )			/*nCurrFrame >= m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] || */
		{
			m_nState = EnumCharFrame::BASE;
			m_bAtkTime = FALSE;
		}
	}

	UpdateSRT();
}

VOID CWeapon::UpdateSRT()
{
	//m_pMap->Set_ControlScale( 0, 0.75f );
	//m_pMap->Set_ControlScale( 1, 0.75f );
	//m_pMap->Set_ControlScale( 2, 0.75f );
	//m_pMap->Set_ControlRotate( 0, 0 );
	//m_pMap->Set_ControlRotate( 1, 0 );
	//m_pMap->Set_ControlRotate( 2, 0 );
	m_pMap->Set_ControlTranslate( 0,-1.5 );
	//m_pMap->Set_ControlTranslate( 1, 0 );
	m_pMap->Set_ControlTranslate( 2,-2.5 );

	m_pMap->Update();
	m_pMap->Calcul_MatWorld();

};

const D3DXMATRIXA16& CWeapon::Get_MatWorld()
{
	return m_pMap->Get_MatWorld(); 
}

VOID CWeapon::Render( D3DXMATRIX _matCharacter )
{
	D3DMATERIAL9 mtrl;

	mtrl.Diffuse = D3DXCOLOR( 0xFFFFFFFF );
	mtrl.Ambient = D3DXCOLOR( 0xFFFFFFFF );
	mtrl.Power = 1.0f;
	mtrl.Emissive = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.0f );
	mtrl.Specular = D3DXCOLOR( 0xFFFFFFFF );

	m_pD3dDevice->SetMaterial( &mtrl );

	m_pMap->Render( Get_MatWorld() * _matCharacter );	
}

INT CWeapon::Get_nFrame(INT nType)
{
	INT nRet = 0;
	
	switch (nType)
	{
	case WEAPONTYPE::CURRENTFRAME:
		nRet = m_pMap->GetCurrentFrame(); 
		break;
	case WEAPONTYPE::FRAMEBEGIN:
		nRet = m_WeaponType.nFrameBegin[m_nState];
		break;
	case WEAPONTYPE::FRAMETIME:
		nRet = m_WeaponType.nFrameTime[m_nState];
		break;
	case WEAPONTYPE::FRAMEATK:
		nRet = m_WeaponType.nFrameAtk[m_nState];
		break;
	case WEAPONTYPE::DELAY:
		nRet = m_WeaponType.nDelay[m_nState];
		break;
	}
	
	return nRet;
}

INT CWeapon::Get_nState() 
{
	return m_nState; 
}

D3DXVECTOR3 CWeapon::Get_Direction()
{
	return m_WeaponType.pBBA.GetDirection();
}

VOID CWeapon::Set_Animation( INT _nState )	
{
	m_pMap->SetAnimation( _nState ); 
}

VOID CWeapon::Set_nState( INT _nState ) 
{ 
	m_pMap->SetAnimation( _nState ); 
}

VOID CWeapon::SetWeaponAni()
{
	m_pMap->AddAnimationData ( IDLE, EnumCharFrame::BASE, 0, 200, TRUE );
}