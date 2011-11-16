#define _GRAP

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
	if ( m_WeaponType.nType > NONE )
		PrivateProfile( WRITE );

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

	m_fZAng[0] = DEG2RAD(0);
	m_fZAng[1] = DEG2RAD(0);
	m_fZAng[2] = DEG2RAD(0);
	m_fZAng[3] = DEG2RAD(90);
	m_fZAng[4] = DEG2RAD(-15);
	m_fZAng[5] = DEG2RAD(0);
	m_fZAng[6] = DEG2RAD(45);
	m_fZAng[7] = DEG2RAD(90);
	m_fZAng[8] = DEG2RAD(135);
	m_fZAng[9] = DEG2RAD(0);

	m_fBBSize[0] =-12.5f;
	m_fBBSize[1] =- 1.5f;
	m_fBBSize[2] =-16.5f;
	m_fBBSize[3] =  2.5f;
	m_fBBSize[4] =  1.5f;
	m_fBBSize[5] =-10.5f;

	m_bAtkTime = FALSE;
}

HRESULT CWeapon::Release()
{
	Clear();
	SAFE_DELETE( m_pMap );
	SAFE_DELETE( m_pCube );
	SAFE_RELEASE( m_pTotalIB );
	SAFE_RELEASE( m_pTotalVB );
	return S_OK;
}

VOID CWeapon::PrivateProfile(BOOL bRW)
{
	if( bRW == READ )
	{
		WCHAR lpKeyName[256];
		m_pMap->CleanupAnimationData();
		wsprintf( lpKeyName, L"Begin%d", EnumCharFrame::BASE	);	m_WeaponType.nFrameBegin[EnumCharFrame::BASE	] = GetPrivateProfileInt( L"Frame", lpKeyName,  0, WEAPONFILE );
		wsprintf( lpKeyName, L"Begin%d", EnumCharFrame::ATTACK1 );	m_WeaponType.nFrameBegin[EnumCharFrame::ATTACK1 ] = GetPrivateProfileInt( L"Frame", lpKeyName,  1, WEAPONFILE );
		wsprintf( lpKeyName, L"Begin%d", EnumCharFrame::ATTACK2 );	m_WeaponType.nFrameBegin[EnumCharFrame::ATTACK2 ] = GetPrivateProfileInt( L"Frame", lpKeyName, 25, WEAPONFILE );
		wsprintf( lpKeyName, L"Begin%d", EnumCharFrame::A		);	m_WeaponType.nFrameBegin[EnumCharFrame::A		] = GetPrivateProfileInt( L"Frame", lpKeyName, 49, WEAPONFILE );
		wsprintf( lpKeyName, L"Begin%d", EnumCharFrame::TEMP1	);	m_WeaponType.nFrameBegin[EnumCharFrame::TEMP1	] = GetPrivateProfileInt( L"Frame", lpKeyName, 73, WEAPONFILE );
		wsprintf( lpKeyName, L"Begin%d", EnumCharFrame::TEMP2	);	m_WeaponType.nFrameBegin[EnumCharFrame::TEMP2	] = GetPrivateProfileInt( L"Frame", lpKeyName, 96, WEAPONFILE );
		wsprintf( lpKeyName, L"Begin%d", EnumCharFrame::TEMP3	); 	m_WeaponType.nFrameBegin[EnumCharFrame::TEMP3	] = GetPrivateProfileInt( L"Frame", lpKeyName, 119, WEAPONFILE );
		wsprintf( lpKeyName, L"Begin%d", EnumCharFrame::TEMP4	);	m_WeaponType.nFrameBegin[EnumCharFrame::TEMP4	] = GetPrivateProfileInt( L"Frame", lpKeyName, 141, WEAPONFILE );
		wsprintf( lpKeyName, L"Time%d" , EnumCharFrame::BASE	);	m_WeaponType.nFrameTime[EnumCharFrame::BASE		] = GetPrivateProfileInt( L"Frame", lpKeyName,  0, WEAPONFILE );
		wsprintf( lpKeyName, L"Time%d" , EnumCharFrame::ATTACK1 );	m_WeaponType.nFrameTime[EnumCharFrame::ATTACK1	] = GetPrivateProfileInt( L"Frame", lpKeyName, 24, WEAPONFILE );
		wsprintf( lpKeyName, L"Time%d" , EnumCharFrame::ATTACK2 );	m_WeaponType.nFrameTime[EnumCharFrame::ATTACK2	] = GetPrivateProfileInt( L"Frame", lpKeyName, 24, WEAPONFILE );
		wsprintf( lpKeyName, L"Time%d" , EnumCharFrame::A		);	m_WeaponType.nFrameTime[EnumCharFrame::A		] = GetPrivateProfileInt( L"Frame", lpKeyName, 24, WEAPONFILE );
		wsprintf( lpKeyName, L"Time%d" , EnumCharFrame::TEMP1	);	m_WeaponType.nFrameTime[EnumCharFrame::TEMP1	] = GetPrivateProfileInt( L"Frame", lpKeyName, 23, WEAPONFILE );
		wsprintf( lpKeyName, L"Time%d" , EnumCharFrame::TEMP2	);	m_WeaponType.nFrameTime[EnumCharFrame::TEMP2	] = GetPrivateProfileInt( L"Frame", lpKeyName, 23, WEAPONFILE );
		wsprintf( lpKeyName, L"Time%d" , EnumCharFrame::TEMP3	);	m_WeaponType.nFrameTime[EnumCharFrame::TEMP3	] = GetPrivateProfileInt( L"Frame", lpKeyName, 23, WEAPONFILE );
		wsprintf( lpKeyName, L"Time%d" , EnumCharFrame::TEMP4	);	m_WeaponType.nFrameTime[EnumCharFrame::TEMP4	] = GetPrivateProfileInt( L"Frame", lpKeyName, 23, WEAPONFILE );
		wsprintf( lpKeyName, L"Atk%d"  , EnumCharFrame::BASE	);	m_WeaponType.nFrameAtk[EnumCharFrame::BASE		] = GetPrivateProfileInt( L"Frame", lpKeyName, 10, WEAPONFILE );
		wsprintf( lpKeyName, L"Atk%d"  , EnumCharFrame::ATTACK1 );	m_WeaponType.nFrameAtk[EnumCharFrame::ATTACK1	] = GetPrivateProfileInt( L"Frame", lpKeyName, 10, WEAPONFILE );
		wsprintf( lpKeyName, L"Atk%d"  , EnumCharFrame::ATTACK2 );	m_WeaponType.nFrameAtk[EnumCharFrame::ATTACK2	] = GetPrivateProfileInt( L"Frame", lpKeyName, 10, WEAPONFILE );
		wsprintf( lpKeyName, L"Atk%d"  , EnumCharFrame::A		);	m_WeaponType.nFrameAtk[EnumCharFrame::A			] = GetPrivateProfileInt( L"Frame", lpKeyName, 10, WEAPONFILE );
		wsprintf( lpKeyName, L"Atk%d"  , EnumCharFrame::TEMP1	);	m_WeaponType.nFrameAtk[EnumCharFrame::TEMP1		] = GetPrivateProfileInt( L"Frame", lpKeyName, 10, WEAPONFILE );
		wsprintf( lpKeyName, L"Atk%d"  , EnumCharFrame::TEMP2	);	m_WeaponType.nFrameAtk[EnumCharFrame::TEMP2		] = GetPrivateProfileInt( L"Frame", lpKeyName, 10, WEAPONFILE );
		wsprintf( lpKeyName, L"Atk%d"  , EnumCharFrame::TEMP3	);	m_WeaponType.nFrameAtk[EnumCharFrame::TEMP3		] = GetPrivateProfileInt( L"Frame", lpKeyName, 10, WEAPONFILE );
		wsprintf( lpKeyName, L"Atk%d"  , EnumCharFrame::TEMP4	);	m_WeaponType.nFrameAtk[EnumCharFrame::TEMP4		] = GetPrivateProfileInt( L"Frame", lpKeyName, 10, WEAPONFILE );
		wsprintf( lpKeyName, L"Delay%d", EnumCharFrame::BASE	);	m_WeaponType.nDelay[EnumCharFrame::BASE			] = GetPrivateProfileInt( L"Frame", lpKeyName, 16, WEAPONFILE );
		wsprintf( lpKeyName, L"Delay%d", EnumCharFrame::ATTACK1	);	m_WeaponType.nDelay[EnumCharFrame::ATTACK1		] = GetPrivateProfileInt( L"Frame", lpKeyName, 16, WEAPONFILE );
		wsprintf( lpKeyName, L"Delay%d", EnumCharFrame::ATTACK2	);	m_WeaponType.nDelay[EnumCharFrame::ATTACK2		] = GetPrivateProfileInt( L"Frame", lpKeyName, 16, WEAPONFILE );
		wsprintf( lpKeyName, L"Delay%d", EnumCharFrame::A		);	m_WeaponType.nDelay[EnumCharFrame::A			] = GetPrivateProfileInt( L"Frame", lpKeyName, 16, WEAPONFILE );
		wsprintf( lpKeyName, L"Delay%d", EnumCharFrame::TEMP1	);	m_WeaponType.nDelay[EnumCharFrame::TEMP1		] = GetPrivateProfileInt( L"Frame", lpKeyName, 16, WEAPONFILE );
		wsprintf( lpKeyName, L"Delay%d", EnumCharFrame::TEMP2	);	m_WeaponType.nDelay[EnumCharFrame::TEMP2		] = GetPrivateProfileInt( L"Frame", lpKeyName, 16, WEAPONFILE );
		wsprintf( lpKeyName, L"Delay%d", EnumCharFrame::TEMP3	);	m_WeaponType.nDelay[EnumCharFrame::TEMP3		] = GetPrivateProfileInt( L"Frame", lpKeyName, 16, WEAPONFILE );
		wsprintf( lpKeyName, L"Delay%d", EnumCharFrame::TEMP4	);	m_WeaponType.nDelay[EnumCharFrame::TEMP4		] = GetPrivateProfileInt( L"Frame", lpKeyName, 16, WEAPONFILE );
		m_WeaponType.vDir[EnumCharFrame::BASE	] = D3DXVECTOR3( 0.0f,  0.0f, -0.3f );
		m_WeaponType.vDir[EnumCharFrame::ATTACK1] = D3DXVECTOR3( 0.0f, -1.3f, -0.3f );
		m_WeaponType.vDir[EnumCharFrame::ATTACK2] = D3DXVECTOR3(-1.0f, -0.3f, -0.3f );
		m_WeaponType.vDir[EnumCharFrame::A		] = D3DXVECTOR3( 1.3f,  0.0f, -0.3f );
		m_WeaponType.vDir[EnumCharFrame::TEMP1  ] = D3DXVECTOR3( 0.3f, -1.0f, -0.3f );
		m_WeaponType.vDir[EnumCharFrame::TEMP2  ] = D3DXVECTOR3( 0.0f, -1.3f, -0.3f );
		m_WeaponType.vDir[EnumCharFrame::TEMP3  ] = D3DXVECTOR3(-0.3f, -1.0f, -0.3f );
		m_WeaponType.vDir[EnumCharFrame::TEMP4  ] = D3DXVECTOR3(-1.3f,  0.0f, -0.3f );

		m_pMap->AddAnimationData ( IDLE		,  EnumCharFrame::BASE  , m_WeaponType.nFrameBegin[EnumCharFrame::BASE	 ], m_WeaponType.nFrameTime[EnumCharFrame::BASE	  ], TRUE );
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::ATTACK1, m_WeaponType.nFrameBegin[EnumCharFrame::ATTACK1], m_WeaponType.nFrameTime[EnumCharFrame::ATTACK1], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::ATTACK2, m_WeaponType.nFrameBegin[EnumCharFrame::ATTACK2], m_WeaponType.nFrameTime[EnumCharFrame::ATTACK2], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::A		, m_WeaponType.nFrameBegin[EnumCharFrame::A		 ], m_WeaponType.nFrameTime[EnumCharFrame::A	  ], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::TEMP1  , m_WeaponType.nFrameBegin[EnumCharFrame::TEMP1  ], m_WeaponType.nFrameTime[EnumCharFrame::TEMP1  ], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::TEMP2  , m_WeaponType.nFrameBegin[EnumCharFrame::TEMP2  ], m_WeaponType.nFrameTime[EnumCharFrame::TEMP2  ], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::TEMP3  , m_WeaponType.nFrameBegin[EnumCharFrame::TEMP3  ], m_WeaponType.nFrameTime[EnumCharFrame::TEMP3  ], FALSE);
		m_pMap->AddAnimationData2( POST_IDLE, EnumCharFrame::TEMP4  , m_WeaponType.nFrameBegin[EnumCharFrame::TEMP4  ], m_WeaponType.nFrameTime[EnumCharFrame::TEMP4  ], FALSE);

		m_nState = EnumCharFrame::BASE;

	}
#ifdef _GRAP
	else
	{
		WCHAR buf[256];
		WCHAR lpKeyName[256];
		for (int i = 0; i < 10; ++i)
		{
			wsprintf( buf, L"%d", m_WeaponType.nFrameBegin[i] ); 
			wsprintf( lpKeyName, L"Begin%d", i );
			WritePrivateProfileString( L"Frame", lpKeyName,	buf, WEAPONFILE );
		}
		for (int i = 0; i < 10; ++i)
		{
			wsprintf( buf, L"%d", m_WeaponType.nFrameTime[i] );  
			wsprintf( lpKeyName, L"Time%d", i );
			WritePrivateProfileString( L"Frame", lpKeyName, buf, WEAPONFILE );
		}
		for (int i = 0; i < 10; ++i)
		{
			wsprintf( buf, L"%d", m_WeaponType.nFrameAtk[i] );   
			wsprintf( lpKeyName, L"Atk%d", i );
			WritePrivateProfileString( L"Frame", lpKeyName,	buf, WEAPONFILE );
		}
		for (int i = 0; i < 10; ++i)
		{
			wsprintf( buf, L"%d", m_WeaponType.nDelay[i] );	     
			wsprintf( lpKeyName, L"Delay%d", i );
			WritePrivateProfileString( L"Frame", lpKeyName,	buf, WEAPONFILE );
		}
	}
#endif
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

	m_pCube = new CCube();
	if( FAILED( m_pd3dDevice->CreateVertexBuffer( CCube::CUBEVERTEX::VertexNum * sizeof( CCube::CUBEVERTEX ),
		0, CCube::CUBEVERTEX::FVF, D3DPOOL_MANAGED, &m_pTotalVB, NULL ) ) )
	{
		MessageBox(GHWND, L"Vertex Buffer Failed", NULL, MB_OK);
	}

	if( FAILED( m_pd3dDevice->CreateIndexBuffer( CCube::CUBEINDEX::IndexNum * sizeof( CCube::CUBEINDEX ), 
		0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pTotalIB, NULL ) ) )
	{
		MessageBox(GHWND, L"Index Buffer Failed", NULL, MB_OK);
	}

	m_pCube->Create( m_pd3dDevice, m_pTotalVB, m_pTotalIB, 0, 0 );
	m_pCube->InitTexture( 0xFFFF0000 );
	return S_OK;
}

VOID CWeapon::SetKeyA()
{
	INT nCurrFrame = m_pMap->GetCurrentFrame();

	if ( m_nState == EnumCharFrame::ATTACK2 &&
		( nCurrFrame > ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nDelay[m_nState] ) ) && 
		( nCurrFrame < ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] ) ) )
	{
		m_nState = EnumCharFrame::A;
		m_pMap->SetAnimation( m_nState );
	}
	else if ( m_nState == EnumCharFrame::ATTACK1 && 
		( nCurrFrame > ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nDelay[m_nState] ) ) && 
		( nCurrFrame < ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] ) ) )
	{
		m_nState = EnumCharFrame::ATTACK2;
		m_pMap->SetAnimation( m_nState );
	}
	else if ( m_nState == EnumCharFrame::BASE && nCurrFrame == 0 )
	{
		m_nState = EnumCharFrame::ATTACK1;
		m_pMap->SetAnimation( m_nState );
	} 
}

VOID CWeapon::SetKeyB()
{
	INT nCurrFrame = m_pMap->GetCurrentFrame();

	if ( m_nState == EnumCharFrame::ATTACK2 &&
		( m_pMap->GetCurrentFrame() > ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nDelay[m_nState] ) ) && 
		( m_pMap->GetCurrentFrame() < ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] ) ) )
	{
		m_nState = EnumCharFrame::TEMP4;
		m_pMap->SetAnimation( m_nState );
	}
	else if ( m_nState == EnumCharFrame::TEMP2 &&
		( m_pMap->GetCurrentFrame() > ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nDelay[m_nState] ) ) && 
		( m_pMap->GetCurrentFrame() < ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] ) ) )
	{
		m_nState = EnumCharFrame::TEMP3;
		m_pMap->SetAnimation( m_nState );
	}
	else if ( m_nState == EnumCharFrame::TEMP1 &&
		( m_pMap->GetCurrentFrame() > ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nDelay[m_nState] ) ) && 
		( m_pMap->GetCurrentFrame() < ( m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] ) ) )
	{
		m_nState = EnumCharFrame::TEMP2;
		m_pMap->SetAnimation( m_nState );
	}
	else if ( m_nState == EnumCharFrame::BASE  && nCurrFrame == 0 )
	{
		m_nState = EnumCharFrame::TEMP1;
		m_pMap->SetAnimation( m_nState );
	}
}

VOID CWeapon::AddAtkBBx( D3DXVECTOR3 &vPos, FLOAT fAngle )
{
	SetBBx( vPos, fAngle );
	CTree::GetInstance()->GetCharAtkVector()->push_back( &m_WeaponType.pBBA );

	//m_bAtkTime = FALSE;
}

VOID CWeapon::SetBBx( const D3DXVECTOR3& vPos, const FLOAT fAngle )
{
	m_WeaponType.pBBA.SetPosition( vPos );
	m_WeaponType.pBBA.SetAngleY( fAngle );
	m_WeaponType.pBBA.SetAngleZ( m_fZAng[m_nState] );

	m_WeaponType.pBBA.SetSize( 0, m_fBBSize[0] );
	m_WeaponType.pBBA.SetSize( 1, m_fBBSize[1] );
	m_WeaponType.pBBA.SetSize( 2, m_fBBSize[2] );

	m_WeaponType.pBBA.SetSize( 3, m_fBBSize[3] );
	m_WeaponType.pBBA.SetSize( 4, m_fBBSize[4] );
	m_WeaponType.pBBA.SetSize( 5, m_fBBSize[5] );

	m_pCube->Set_ControlScale( 0, m_fBBSize[3] - m_fBBSize[0] );
	m_pCube->Set_ControlScale( 1, m_fBBSize[4] - m_fBBSize[1] );
	m_pCube->Set_ControlScale( 2, m_fBBSize[5] - m_fBBSize[2] );
	m_pCube->Set_ControlRotate( 2, m_fZAng[m_nState] );
	m_pCube->Set_ControlTranslate( 0, ( m_fBBSize[0] + m_fBBSize[3] ) * 0.5f );
	m_pCube->Set_ControlTranslate( 1, ( m_fBBSize[1] + m_fBBSize[4] ) * 0.5f + 7.5f );
	m_pCube->Set_ControlTranslate( 2, ( m_fBBSize[2] + m_fBBSize[5] ) * 0.5f );
	
	m_WeaponType.pBBA.SetDirection( m_WeaponType.vDir[m_nState] );
}

VOID CWeapon::Update()
{
	INT nCurrFrame = m_pMap->GetCurrentFrame();

#ifdef _GRAP
	if( CInput::GetInstance()->Get_F9button() )
	{
		CInput::GetInstance()->Set_F9button(FALSE);
		OPENFILENAME OFN;
		WCHAR lpstrFile[MAX_PATH]=L"";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner=CWinBase::GetInstance()->Get_hWnd();
		OFN.lpstrFilter=TEXT("ASE 파일(*.ASE)\0*.ASE\0");
		OFN.lpstrFile=lpstrFile;
		OFN.nMaxFile=MAX_PATH;
		if (GetOpenFileName(&OFN)!=0)
		{
			WCHAR* ptr = wcstok( lpstrFile, L"." );
			WCHAR Temp[255];
			wsprintf( Temp, L"%s.ASE", ptr );

			m_pMap->Create( Temp, NULL );
		}
	}
	if( CInput::GetInstance()->Get_F8button() )
	{
		PrivateProfile( READ );
	}
#endif // _GRAP

	if ( m_nState != EnumCharFrame::BASE )
	{
		//네트워크로 현재 애니메이션 상태 보내기
		CNetwork::GetInstance()->CS_UTOM_Attack_Animation( m_nState );

		//타격설정
		if ( m_bAtkTime == TRUE)
		{
			m_bAtkTime = 2;
		}
		else if ( ( nCurrFrame >= m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameAtk[m_nState] ) )
		{
			m_bAtkTime = TRUE;
		}
		else if ( nCurrFrame >= m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] )
		{
			m_bAtkTime = FALSE;
		}
		else if ( nCurrFrame == 0 )			/*nCurrFrame >= m_WeaponType.nFrameBegin[m_nState] + m_WeaponType.nFrameTime[m_nState] || */
		{
			m_nState = EnumCharFrame::BASE;
			m_bAtkTime = FALSE;
		}
	}

	UpdateSRT();

	CDebugConsole::GetInstance()->Messagef(L"%d-%d\n", m_nState, nCurrFrame);
}

VOID CWeapon::UpdateSRT()
{
	m_pMap->Set_ControlScale( 0, 0.75f );
	m_pMap->Set_ControlScale( 1, 0.75f );
	m_pMap->Set_ControlScale( 2, 0.75f );
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

	m_pd3dDevice->SetMaterial( &mtrl );

	m_pMap->Render( Get_MatWorld() * _matCharacter );	

	m_pd3dDevice->SetStreamSource( 0, m_pTotalVB, 0, sizeof( CCube::CUBEVERTEX ) );
	m_pd3dDevice->SetFVF( CCube::CUBEVERTEX::FVF );
	m_pd3dDevice->SetIndices( m_pTotalIB );

	m_pCube->Update();
	m_pCube->Calcul_MatWorld();
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &( m_pCube->Get_MatWorld() * _matCharacter ) );
	m_pCube->Render();

	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

}

INT CWeapon::Get_nFrame()
{
	return m_pMap->GetCurrentFrame(); 
}

INT CWeapon::Get_nState() 
{
	return m_nState; 
}

INT CWeapon::Get_Direction()
{
	FLOAT fRet = m_WeaponType.pBBA.GetDirection().x;
	return ( fRet < 0.0f ? -1 : ( fRet > 0.0f ? 1 : 0 ) );
}

VOID CWeapon::Set_Animation( INT _nState )	
{
	m_pMap->SetAnimation( _nState ); 
}

VOID CWeapon::Set_nState( INT _nState ) 
{ 
	m_pMap->SetAnimation( _nState ); 
}