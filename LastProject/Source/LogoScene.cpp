#include "stdafx.h"
#include "LogoScene.h"
#include "Charactor.h"
#include "CameraWork.h"
#include "GameEventCombo.h"

VOID LogoScene::Initialize()
{
	m_scnNext		= SCENE_STORY;
	m_scnState		= SCENE_RUNNING;

	m_pLogo			= NULL;
	m_pCameraWork	= NULL;

	m_pLight		= NULL;
	m_pMatrices		= NULL;

	m_datLogo.bActivate		= TRUE;
	m_datLogo.dFrameSpeed	= 1200;

	m_datScene.bActivate	= FALSE;
	m_datScene.dFrameSpeed	= 2200;
}

VOID LogoScene::Release()
{
	SAFE_DELETE_ARRAY( m_pLogo );

	SAFE_DELETE( m_pCameraWork );

	SAFE_DELETE( m_pLight );

	//CTree::DestoryInstance();
}

HRESULT LogoScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pD3dDevice	=	_pd3dDevice;

	// Create Logo
	m_pLogo = new CCharactor[6];

	m_pLogo[ 0 ].Create( m_pD3dDevice );
	m_pLogo[ 0 ].Load( L"Data/Logo/N_Beta.csav" );
	m_pLogo[ 0 ].Set_Position( D3DXVECTOR3( 55.0f, 100.0f, 0.0f ) );
	m_pLogo[ 0 ].Set_Angle( -MTP_FUN::Deg2Rad<210>::radians ); // D3DXToRadian( -30.0f ) );
	m_pLogo[ 0 ].EnableShadow( FALSE );
	
	m_pLogo[ 1 ].Create( m_pD3dDevice );
	m_pLogo[ 1 ].Load( L"Data/Logo/-_Beta.csav" );
	m_pLogo[ 1 ].Set_Position( D3DXVECTOR3( 35.0f, 100.0f,-5.0f ) );
	m_pLogo[ 1 ].Set_Angle( -MTP_FUN::Deg2Rad<15>::radians ); // D3DXToRadian( -15.0f ) );
	m_pLogo[ 1 ].EnableShadow( FALSE );

	m_pLogo[ 2 ].Create( m_pD3dDevice );
	m_pLogo[ 2 ].Load( L"Data/Logo/L_Beta.csav" );
	m_pLogo[ 2 ].Set_Position( D3DXVECTOR3( 15.0f, 100.0f,-10.0f ) );
	m_pLogo[ 2 ].Set_Angle( MTP_FUN::Deg2Rad<90>::radians ); // D3DXToRadian( 90.0f ) );
	m_pLogo[ 2 ].EnableShadow( FALSE );

	m_pLogo[ 3 ].Create( m_pD3dDevice );
	m_pLogo[ 3 ].Load( L"Data/Logo/I_Beta.csav" );
	m_pLogo[ 3 ].Set_Position( D3DXVECTOR3( -5.0f, 100.0f,-10.0f ) );
	//m_pLogo[ 3 ].Set_Angle( MTP_FUN::Deg2Rad<0>::radians ); //D3DXToRadian( 0.0f ) );
	m_pLogo[ 3 ].EnableShadow( FALSE );

	m_pLogo[ 4 ].Create( m_pD3dDevice );
	m_pLogo[ 4 ].Load( L"Data/Logo/N_Beta.csav" );
	m_pLogo[ 4 ].Set_Position( D3DXVECTOR3(-25.0f, 100.0f,-5.0f ) );
	m_pLogo[ 4 ].Set_Angle( MTP_FUN::Deg2Rad<195>::radians ); // D3DXToRadian( 7.5f ) );
	m_pLogo[ 4 ].EnableShadow( FALSE );

	m_pLogo[ 5 ].Create( m_pD3dDevice );
	m_pLogo[ 5 ].Load( L"Data/Logo/E_Beta.csav" );
	m_pLogo[ 5 ].Set_Position( D3DXVECTOR3(-45.0f, 100.0f, 0.0f ) );
	m_pLogo[ 5 ].Set_Angle( MTP_FUN::Deg2Rad<195>::radians ); //D3DXToRadian( 195.0f ) );
	m_pLogo[ 5 ].EnableShadow( FALSE );

	//	Create CameraWork
	m_pCameraWork = new CameraWork( m_pD3dDevice );
	m_pCameraWork->Create();
	D3DXVECTOR3 vecBox0( 89.0f, 10.0f, -306.0f ), vecBox1( 865.0f, 50.0f, 117.0f ), vecBox2( -115.0f, 100.0f, 570.0f ), vecBox3( -6.0f, 20.0f, 120.0f );
	D3DXVECTOR3 vecBox4( -189.0f, 10.0f, -133.0f ), vecBox5( 4.0f, 10.0f, 0.0f );
	m_pCameraWork->SetPosition_Box4( vecBox0, vecBox1, vecBox2, vecBox3 );
	m_pCameraWork->SetLookAtCourse_Box2( vecBox4, vecBox5 );
	
	m_pCameraWork->SetWorkingPeriod( 5000 );

	//	Create Light
	m_pLight = new CLight;
	m_pLight->Create( m_pD3dDevice );


	//	Create Matrices
	m_pMatrices = CMatrices::GetInstance();
	
	//CTree::GetInstance()->Create( 0, 0 );

	//	Set Logo Time
	m_datLogo.dBeginTime	= timeGetTime();

	return S_OK;
}

VOID LogoScene::Update()
{
	//로고
	for( INT i=0; i<6; i++ )
	{
		m_pLogo[ i ].Set_ControlTranslate( 1, /*1000.0f*/0.0f );
		m_pLogo[ i ].UpdateOtherPlayer( TRUE );
	}

	D3DXVECTOR3 vPos;
	if( GetKeyState( VK_LBUTTON ) & 0x8000 )
	{
		m_scnState = IScene::SCENE_END;
		return ;
	}
	//if( GetKeyState( VK_DOWN ) & 0x8001 )
	//{
	//	m_pLogo[ 0 ].BreakCubeAll();
	//	m_pLogo[ 1 ].BreakCubeAll();
	//	m_pLogo[ 2 ].BreakCubeAll();
	//	m_pLogo[ 3 ].BreakCubeAll();
	//	m_pLogo[ 4 ].BreakCubeAll();
	//	m_pLogo[ 5 ].BreakCubeAll();
	//}

	static INT iNumLogo = 0;
	
	DWORD dCurrentTime = timeGetTime();

	if( m_datLogo.bActivate )
	{
		if( dCurrentTime > m_datLogo.dBeginTime + m_datLogo.dFrameSpeed )
		{
			m_datLogo.dBeginTime = dCurrentTime;

			m_pLogo[ iNumLogo ].BreakCubeAll();

			if( iNumLogo < 6 )
				iNumLogo++;
			else
			{
				m_datLogo.bActivate		= FALSE;

				m_datScene.bActivate	= TRUE;
				m_datScene.dBeginTime	= timeGetTime();
			}
		}
	}
	
	//	종료
	if( m_datScene.bActivate )
	{
		if( dCurrentTime > m_datScene.dBeginTime + m_datScene.dFrameSpeed )
		{
			m_scnState = IScene::SCENE_END;
			return ;
		}
	}

	m_pCameraWork->Update();

	/*static D3DXVECTOR3 vecBox0( 89.0f, 10.0f, -306.0f ), vecBox1( 865.0f, 50.0f, 117.0f ), vecBox2( -115.0f, 100.0f, 570.0f ), vecBox3( -6.0f, 90.0f, 79.0f );
	static D3DXVECTOR3 vecBox4( -189.0f, 10.0f, -133.0f ), vecBox5( 4.0f, 10.0f, -33.0f );
	
	if( GetKeyState( 'A' ) &0x8000 )
		vecBox0.x -= 1.0f;
	if( GetKeyState( 'D' ) &0x8000 )
		vecBox0.x += 1.0f;
	if( GetKeyState( 'S' ) &0x8000 )
		vecBox0.z -= 1.0f;
	if( GetKeyState( 'W' ) &0x8000 )
		vecBox0.z += 1.0f;

	if( GetKeyState( 'F' ) &0x8000 )
		vecBox1.x -= 1.0f;
	if( GetKeyState( 'H' ) &0x8000 )
		vecBox1.x += 1.0f;
	if( GetKeyState( 'G' ) &0x8000 )
		vecBox1.z -= 1.0f;
	if( GetKeyState( 'T' ) &0x8000 )
		vecBox1.z += 1.0f;

	if( GetKeyState( 'J' ) &0x8000 )
		vecBox2.x -= 1.0f;
	if( GetKeyState( 'L' ) &0x8000 )
		vecBox2.x += 1.0f;
	if( GetKeyState( 'K' ) &0x8000 )
		vecBox2.z -= 1.0f;
	if( GetKeyState( 'I' ) &0x8000 )
		vecBox2.z += 1.0f;

	if( GetKeyState( VK_LEFT ) &0x8000 )
		vecBox3.x -= 1.0f;
	if( GetKeyState( VK_RIGHT ) &0x8000 )
		vecBox3.x += 1.0f;
	if( GetKeyState( VK_DOWN ) &0x8000 )
		vecBox3.z -= 1.0f;
	if( GetKeyState( VK_UP ) &0x8000 )
		vecBox3.z += 1.0f;

	m_pCameraWork->SetPosition_Box4( vecBox0, vecBox1, vecBox2, vecBox3 );
	m_pCameraWork->SetLookAtCourse_Box2( vecBox4, vecBox5 );

	CDebugConsole::GetInstance()->Messagef( L"Box0 : %f %f %f\n", vecBox4.x, vecBox4.y, vecBox4.z );
	CDebugConsole::GetInstance()->Messagef( L"Box1 : %f %f %f\n", vecBox5.x, vecBox5.y, vecBox5.z );
	CDebugConsole::GetInstance()->Messagef( L"Box2 : %f %f %f\n", vecBox2.x, vecBox2.y, vecBox2.z );
	CDebugConsole::GetInstance()->Messagef( L"Box3 : %f %f %f\n", vecBox3.x, vecBox3.y, vecBox3.z );*/
}

VOID LogoScene::Render()
{
	//static FLOAT fX = 0.0f, fY = 1020.0f, fZ = 100.0f;

	//	Test
	D3DXVECTOR3 vecCameraPosition, vecCameraLookAt;
	m_pCameraWork->GetCameraPosition( vecCameraPosition );
	m_pCameraWork->GetCameraLookAt( vecCameraLookAt );

	D3DXVECTOR3		vecEyePt/*( 0.0f, 1500.0f, 0.0f);*/ = vecCameraPosition; /*( 0.0f, 1020.0f, 100.0f );*/
	D3DXVECTOR3		vecLookatPt/*( 1.0f, 1.0f, 1.0f );*/  = vecCameraLookAt; /*( 0.0f, 1000.0f, 0.0f );*/
	D3DXVECTOR3		vecUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16	matView;
	D3DXMatrixLookAtLH( &matView, &vecEyePt, &vecLookatPt, &vecUpVec );
	m_pD3dDevice->SetTransform( D3DTS_VIEW, &matView );

	m_pLight->EnableLight();
	m_pMatrices->SetupProjection();

	for( INT i=0; i<6; i++ )
	{
		m_pLogo[ i ].Render();
	}

	m_pLight->DisableLight();
}

INT LogoScene::GetSceneNext()
{
	return m_scnNext;
}

INT LogoScene::GetSceneState()
{
	return m_scnState;
}