#include "stdafx.h"
#include "LogoScene.h"
#include "Charactor.h"
#include "CameraWork.h"
//#include "EfSurface.h"

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
	m_datScene.dFrameSpeed	= 3000;

	m_pD3dDevice = NULL;
	m_pSprite = NULL;
	m_hWnd = NULL;
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
	m_pLogo[ 0 ].Set_Angle( -MTP_FUN::Deg2Rad<210>::radians );
	m_pLogo[ 0 ].EnableShadow( FALSE );
	
	m_pLogo[ 1 ].Create( m_pD3dDevice );
	m_pLogo[ 1 ].Load( L"Data/Logo/-_Beta.csav" );
	m_pLogo[ 1 ].Set_Position( D3DXVECTOR3( 35.0f, 100.0f,-5.0f ) );
	m_pLogo[ 1 ].Set_Angle( -MTP_FUN::Deg2Rad<15>::radians );
	m_pLogo[ 1 ].EnableShadow( FALSE );

	m_pLogo[ 2 ].Create( m_pD3dDevice );
	m_pLogo[ 2 ].Load( L"Data/Logo/L_Beta.csav" );
	m_pLogo[ 2 ].Set_Position( D3DXVECTOR3( 15.0f, 100.0f,-10.0f ) );
	m_pLogo[ 2 ].Set_Angle( MTP_FUN::Deg2Rad<90>::radians );
	m_pLogo[ 2 ].EnableShadow( FALSE );

	m_pLogo[ 3 ].Create( m_pD3dDevice );
	m_pLogo[ 3 ].Load( L"Data/Logo/I_Beta.csav" );
	m_pLogo[ 3 ].Set_Position( D3DXVECTOR3( -5.0f, 100.0f,-10.0f ) );
	//m_pLogo[ 3 ].Set_Angle( MTP_FUN::Deg2Rad<0>::radians );
	m_pLogo[ 3 ].EnableShadow( FALSE );

	m_pLogo[ 4 ].Create( m_pD3dDevice );
	m_pLogo[ 4 ].Load( L"Data/Logo/N_Beta.csav" );
	m_pLogo[ 4 ].Set_Position( D3DXVECTOR3(-25.0f, 100.0f,-5.0f ) );
	m_pLogo[ 4 ].Set_Angle( MTP_FUN::Deg2Rad<195>::radians );
	m_pLogo[ 4 ].EnableShadow( FALSE );

	m_pLogo[ 5 ].Create( m_pD3dDevice );
	m_pLogo[ 5 ].Load( L"Data/Logo/E_Beta.csav" );
	m_pLogo[ 5 ].Set_Position( D3DXVECTOR3(-45.0f, 100.0f, 0.0f ) );
	m_pLogo[ 5 ].Set_Angle( MTP_FUN::Deg2Rad<195>::radians );
	m_pLogo[ 5 ].EnableShadow( FALSE );

	//	Create CameraWork
	m_pCameraWork = new CameraWork( m_pD3dDevice );
	
	CameraWork::PARAMETER prmCameraWork;

	prmCameraWork.avecPosition[ 0 ] = D3DXVECTOR3( 201.0f, 10.0f, -220.0f );
	prmCameraWork.avecPosition[ 1 ] = D3DXVECTOR3( 466.0f, 50.0f, 68.0f );
	prmCameraWork.avecPosition[ 2 ] = D3DXVECTOR3( -90.0f, 100.0f, 747.0f );
	prmCameraWork.avecPosition[ 3 ] = D3DXVECTOR3( -6.0f, 20.0f, 120.0f );

	prmCameraWork.avecLookAt[ 0 ] = D3DXVECTOR3( 7.0f, 10.0f, -6.0f );
	prmCameraWork.avecLookAt[ 1 ] = D3DXVECTOR3( 4.0f, 10.0f, 0.0f );

	m_pCameraWork->AddData( CWK_LOGO_EVENT, &prmCameraWork );
	m_pCameraWork->SelectData( CWK_LOGO_EVENT );
	m_pCameraWork->SetWholeWorkingPeriod( CWK_LOGO_EVENT, CameraWork::CWK_POSITION, 5000 );
	m_pCameraWork->SetWholeWorkingPeriod( CWK_LOGO_EVENT, CameraWork::CWK_LOOKAT, 5000 );

	//	Create Light
	m_pLight = new CLight;
	m_pLight->Create( m_pD3dDevice );


	//	Create Matrices
	m_pMatrices = CMatrices::GetInstance();
	
	//	Set Logo Time
	m_datLogo.dBeginTime	= timeGetTime();

	//m_pEff = new CEfSurface();
	//m_pEff->Create( m_pD3dDevice );
	//m_pEff->Restore();

	return S_OK;
}

VOID LogoScene::Update()
{
	//로고
	for( INT i=0; i<6; i++ )
	{
		m_pLogo[ i ].Set_ControlTranslate( 1, /*1000.0f*/10.0f );
		m_pLogo[ i ].UpdateOtherPlayer( TRUE );
	}

	D3DXVECTOR3 vPos;
	if( GetKeyState( VK_ESCAPE ) & 0x8000 )
	{
		m_scnState = IScene::SCENE_END;
		return ;
	}

	static INT iNumLogo = 0;
	
	DWORD dCurrentTime = timeGetTime();

	if( m_datLogo.bActivate )
	{
		if( dCurrentTime > m_datLogo.dBeginTime + m_datLogo.dFrameSpeed )
		{
			m_datLogo.dBeginTime = dCurrentTime;

			m_pLogo[ iNumLogo ].BreakNockdown( 2.0f );

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
	//m_pEff->Update( m_pLogo );
}

VOID LogoScene::Render()
{
	//static FLOAT fX = 0.0f, fY = 1020.0f, fZ = 100.0f;

	//	Test
	D3DXVECTOR3 vecCameraPosition, vecCameraLookAt;
	m_pCameraWork->GetCameraPosition( vecCameraPosition );
	m_pCameraWork->GetCameraLookAt( vecCameraLookAt );

	D3DXVECTOR3		vecEyePt = vecCameraPosition;
	D3DXVECTOR3		vecLookatPt = vecCameraLookAt;
	D3DXVECTOR3		vecUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16	matView;
	D3DXMatrixLookAtLH( &matView, &vecEyePt, &vecLookatPt, &vecUpVec );
	m_pD3dDevice->SetTransform( D3DTS_VIEW, &matView );

	m_pLight->EnableCharacterLight();
	m_pMatrices->SetupProjection();

	for( INT i = 0; i < 6; i++ )
	{
		m_pLogo[ i ].Render();
	}

	m_pLight->DisableLight();
	//m_pEff->Render();
}

INT LogoScene::GetSceneNext()
{
	return m_scnNext;
}

INT LogoScene::GetSceneState()
{
	return m_scnState;
}