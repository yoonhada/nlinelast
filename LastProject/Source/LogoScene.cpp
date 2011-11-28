#include "stdafx.h"
#include "LogoScene.h"
#include "Charactor.h"

VOID LogoScene::Initialize()
{
	m_scnNext		= SCENE_STORY;
	m_scnState		= SCENE_RUNNING;

	m_pLogo			= NULL;
}

VOID LogoScene::Release()
{
	SAFE_DELETE_ARRAY( m_pLogo );

	CTree::DestoryInstance();
}

HRESULT LogoScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pD3dDevice	=	_pd3dDevice;
	//로고
	m_pLogo = new CCharactor[6];

	m_pLogo[ 0 ].Create( _pd3dDevice );
	m_pLogo[ 0 ].Load( L"Data/Logo/N.csav" );
	m_pLogo[ 0 ].Set_Position( D3DXVECTOR3( -130.0f, 0.0f, 200.0f ) );

	m_pLogo[ 1 ].Create( _pd3dDevice );
	m_pLogo[ 1 ].Load( L"Data/Logo/-.csav" );
	m_pLogo[ 1 ].Set_Position( D3DXVECTOR3( -160.0f, 0.0f, 200.0f ) );

	m_pLogo[ 2 ].Create( _pd3dDevice );
	m_pLogo[ 2 ].Load( L"Data/Logo/L.csav" );
	m_pLogo[ 2 ].Set_Position( D3DXVECTOR3( -220.0f, 0.0f, 200.0f ) );

	m_pLogo[ 3 ].Create( _pd3dDevice );
	m_pLogo[ 3 ].Load( L"Data/Logo/I.csav" );
	m_pLogo[ 3 ].Set_Position( D3DXVECTOR3( -220.0f, 0.0f, 200.0f ) );

	m_pLogo[ 4 ].Create( _pd3dDevice );
	m_pLogo[ 4 ].Load( L"Data/Logo/N.csav" );
	m_pLogo[ 4 ].Set_Position( D3DXVECTOR3( -250.0f, 0.0f, 200.0f ) );

	m_pLogo[ 5 ].Create( _pd3dDevice );
	m_pLogo[ 5 ].Load( L"Data/Logo/E.csav" );
	m_pLogo[ 5 ].Set_Position( D3DXVECTOR3( -280.0f, 0.0f, 200.0f ) );

	
	CTree::GetInstance()->Create( 0, 0 );
	return S_OK;
}

VOID LogoScene::Update()
{
	//로고
	for( INT i=0; i<6; i++ )
		m_pLogo[ i ].UpdateOtherPlayer( TRUE );

	if( GetKeyState( VK_LEFT ) & 0x8000 )
		m_pLogo[ 0 ].BreakCubeAll();
	if( GetKeyState( VK_RIGHT ) & 0x8000 )
		m_pLogo[ 1 ].BreakCubeAll();
	if( GetKeyState( VK_UP ) & 0x8000 )
		m_pLogo[ 2 ].BreakCubeAll();
	if( GetKeyState( VK_DOWN ) & 0x8000 )
		m_pLogo[ 3 ].BreakCubeAll();
	if( GetKeyState( 'z' ) & 0x8000 )
		m_pLogo[ 4 ].BreakCubeAll();
	if( GetKeyState( 'x' ) & 0x8000 )
		m_pLogo[ 5 ].BreakCubeAll();

	//m_pLogo[0].Collision();
}

VOID LogoScene::Render()
{
	static FLOAT fX = 0.0f, fY = 0.0f, fZ = 0.0f;
	

	D3DXVECTOR3		vecEyePt( /*fX*/0.0f, /*fY*/-100.0f, /*fZ*/-400.0f );
	D3DXVECTOR3		vecLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3		vecUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16	matView;
	D3DXMatrixLookAtLH( &matView, &vecEyePt, &vecLookatPt, &vecUpVec );
	m_pD3dDevice->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.0f, 1.0f, 10000.0f );
	m_pD3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	for( INT i=0; i<6; i++ )
		m_pLogo[ i ].Render();
}

INT LogoScene::GetSceneNext()
{
	return m_scnNext;
}

INT LogoScene::GetSceneState()
{
	return m_scnState;
}