#include "stdafx.h"
#include "LogoScene.h"
#include "Charactor.h"

VOID LogoScene::Initialize()
{
	m_scnNext		= SCENE_STORY;
	m_scnState		= SCENE_RUNNING;

	m_pLogo			= NULL;

	m_pLight		= NULL;
	m_pMatrices		= NULL;

	m_datLogo.bActivate		= TRUE;
	m_datLogo.dFrameSpeed	= 1000;

	m_datScene.bActivate	= FALSE;
	m_datScene.dFrameSpeed	= 2000;
}

VOID LogoScene::Release()
{
	SAFE_DELETE_ARRAY( m_pLogo );

	SAFE_DELETE( m_pLight );

	CTree::DestoryInstance();
}

HRESULT LogoScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pD3dDevice	=	_pd3dDevice;

	// Create Logo
	m_pLogo = new CCharactor[6];

	m_pLogo[ 0 ].Create( m_pD3dDevice );
	m_pLogo[ 0 ].Load( L"Data/Logo/N_Beta.csav" );
	m_pLogo[ 0 ].Set_Position( D3DXVECTOR3( 50.0f, 0.0f, 0.0f ) );

	m_pLogo[ 1 ].Create( m_pD3dDevice );
	m_pLogo[ 1 ].Load( L"Data/Logo/-_Beta.csav" );
	m_pLogo[ 1 ].Set_Position( D3DXVECTOR3( 30.0f, 0.0f, 0.0f ) );

	m_pLogo[ 2 ].Create( m_pD3dDevice );
	m_pLogo[ 2 ].Load( L"Data/Logo/L_Beta.csav" );
	m_pLogo[ 2 ].Set_Position( D3DXVECTOR3( 10.0f, 0.0f, 0.0f ) );

	m_pLogo[ 3 ].Create( m_pD3dDevice );
	m_pLogo[ 3 ].Load( L"Data/Logo/I_Beta.csav" );
	m_pLogo[ 3 ].Set_Position( D3DXVECTOR3( -10.0f, 0.0f, 0.0f ) );

	m_pLogo[ 4 ].Create( m_pD3dDevice );
	m_pLogo[ 4 ].Load( L"Data/Logo/N_Beta.csav" );
	m_pLogo[ 4 ].Set_Position( D3DXVECTOR3( -30.0f, 0.0f, 0.0f ) );

	m_pLogo[ 5 ].Create( m_pD3dDevice );
	m_pLogo[ 5 ].Load( L"Data/Logo/E_Beta.csav" );
	m_pLogo[ 5 ].Set_Position( D3DXVECTOR3( -50.0f, 0.0f, 0.0f ) );

	//	Create Tree
	CTree::GetInstance()->Create( 0, 0 );

	//	Create Light
	m_pLight = new CLight;
	m_pLight->Create( m_pD3dDevice );

	//	Create Matrices
	m_pMatrices = CMatrices::GetInstance();
	
	CTree::GetInstance()->Create( 0, 0 );

	//	Set Logo Time
	m_datLogo.dBeginTime	= timeGetTime();

	return S_OK;
}

VOID LogoScene::Update()
{
	//�ΰ�
	for( INT i=0; i<6; i++ )
		m_pLogo[ i ].UpdateOtherPlayer( TRUE );

	if( GetKeyState( VK_ESCAPE ) & 0x8000 )
		m_scnState = IScene::SCENE_END;
	
	if( GetKeyState( VK_RIGHT ) & 0x8000 )
		m_pLogo[ 0 ].BreakCubeAll();

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

	if( m_datScene.bActivate )
	{
		if( dCurrentTime > m_datScene.dBeginTime + m_datScene.dFrameSpeed )
			m_scnState = IScene::SCENE_END;
	}

}

VOID LogoScene::Render()
{
	static FLOAT fX = 0.0f, fY = 50.0f, fZ = 100.0f;

	D3DXVECTOR3		vecEyePt( fX, fY, fZ );
	D3DXVECTOR3		vecLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3		vecUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16	matView;
	D3DXMatrixLookAtLH( &matView, &vecEyePt, &vecLookatPt, &vecUpVec );
	m_pD3dDevice->SetTransform( D3DTS_VIEW, &matView );

	m_pLight->EnableLight();
	m_pMatrices->SetupProjection();

	for( INT i=0; i<6; i++ )
		m_pLogo[ i ].Render();

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