#include "Stdafx.h"
#include "MainScene.h"

#include "Monster.h"
#include "BillBoard.h"
#include "Charactor.h"
#include "Camera.h"
#include "Axis.h"
#include "TileMap.h"
#include "ASEViewer.h"
#include "Weapon.h"
#include "TimeLifeItem.h"
#include "GameEvent.h"

#include "MainGUI.h"
#include "OptionScene.h"

// AI 테스트용
#ifdef _DEBUG
#include "Seek.h"
#include "Chase.h"
#else
#include "Seek.h"
#include "Chase.h"
#endif


CMainScene::CMainScene()
{
	Clear();

	m_scnNext	= IScene::SCENE_MENU;
	m_scnState	= IScene::SCENE_RUNNING;
}

CMainScene::~CMainScene()
{
	Release();
}

VOID	CMainScene::Clear()
{
	m_pMatrices		= NULL;
	m_pCamera		= NULL;
	m_pD3dDevice	= NULL;
	m_pCharactors	= NULL;
	m_pAxis			= NULL;
	m_pMonster		= NULL;
	m_pTileMap		= NULL;
	m_pASEViewer	= NULL;
	m_pGameEvent	= NULL;
	
	m_pMainGUI		= NULL;
	m_pOptionScene	= NULL;

	m_iMaxCharaNum = CObjectManage::GetInstance()->Get_MaxCharaNum();
}

HRESULT CMainScene::Create( LPDIRECT3DDEVICE9 a_pD3dDevice, LPD3DXSPRITE a_Sprite, HWND a_hWnd )
{
	m_pD3dDevice = a_pD3dDevice;

	m_pMatrices = CMatrices::GetInstance();

	//트리 생성
	FLOAT fSize = 1000.0f;
	INT nDeep = 4;
	CTree::GetInstance()->Create( fSize, nDeep );
#ifdef _DEBUG
	m_pAxis = new Axis( m_pD3dDevice );
	m_pAxis->Create( fSize, nDeep );
#endif

	//카메라 생성
	m_pCamera = new CCamera;
	m_pCamera->Create( m_pD3dDevice );

	//캐릭터 생성
	CreateCharactor();

	//몬스터 생성
	m_pMonster = CObjectManage::GetInstance()->Get_Monster();
	m_pMonster->Create( m_pD3dDevice, L"Data/CharData/27_pierro_body_11_28" );

	// 아이템 생성
	m_pFirstAidKit = CObjectManage::GetInstance()->Get_FirstAidKit();
	m_pFirstAidKit->Create( m_pD3dDevice );
	m_pFirstAidKit->Load( L"Data/CharData/FirstAidKit_1.csav" );

	//조명 생성
	m_pLight = new CLight;
	m_pLight->Create( m_pD3dDevice );

	//맵 생성
	// Error 예외생성 
	m_pASEViewer = CObjectManage::GetInstance()->Get_ASEViewer();
	m_pASEViewer->Create(  L"ASE File/Map/Stage_Beta.ASE", L"ASE File/Map/Stage_Beta_Box.BBX" );

	//타일맵 생성
	m_pTileMap = new TileMap( m_pD3dDevice );
	m_pTileMap->Create( D3DXVECTOR3( -510.0f, 0.0f, -950.0f ), D3DXVECTOR3( 510.0f, 0.0f, 950.0f ), 10.0f );
	m_pTileMap->LoadBBXFile( L"ASE File/Map/Stage_Beta_Box.BBX" );

	//이벤트
	m_pGameEvent = new CGameEvent( CObjectManage::GetInstance()->Get_MaxCharaNum() );

	Seek::GetInstance()->Initialize( m_pTileMap );
	Chase::GetInstance()->Initialize( m_pTileMap->GetInfo() );

	Astar::GetInstance()->Initialize( m_pTileMap->GetInfo() );

	// 프로젝션 설정
	m_pMatrices->SetupProjection();

	// GUI 생성
	m_pMainGUI = new MainGUI( m_pD3dDevice, a_Sprite, a_hWnd );
	m_pMainGUI->Create();
	m_pOptionScene = new OptionScene;
	m_pOptionScene->Create( m_pD3dDevice, a_Sprite, a_hWnd );

	CInput::GetInstance()->EnableInput(FALSE);
	CDebugConsole::GetInstance()->Messagef( L"**** MainScene Create End **** \n\n" );

	return S_OK;
}

HRESULT CMainScene::Release()
{
	SAFE_DELETE ( m_pLight );
	SAFE_DELETE( m_pAxis );
	SAFE_DELETE( m_pOptionScene );
	SAFE_DELETE( m_pMainGUI );
	SAFE_DELETE( m_pGameEvent );
	SAFE_DELETE( m_pTileMap );
	SAFE_DELETE ( m_pCamera );

#ifdef _DEBUG
	SAFE_DELETE( m_pAxis );
#endif // _DEBUG

	return S_OK;
}

VOID CMainScene::CreateCharactor()
{
	//캐릭터 생성
	m_pCharactors = CObjectManage::GetInstance()->Get_Charactors();

	for(INT Loop = 0; Loop < m_iMaxCharaNum; ++Loop )
	{
		if( !m_pCharactors[Loop] )
			continue;

		m_pCharactors[Loop]->Create( m_pD3dDevice );
		m_pCharactors[Loop]->LoadKindChar( Loop );

		if (Loop == 0)
		{
			m_pCharactors[0]->Set_Active( TRUE );
		}
		else
		{
			m_pCharactors[Loop]->Set_Position( D3DXVECTOR3(-100, 0, 600) );
		}
		CTree::GetInstance()->GetCharVector()->push_back( m_pCharactors[Loop]->GetBoundBox() );
	}
}

VOID	CMainScene::Update()
{
	// 캐릭터: 인풋 값 받아오기
	m_pCharactors[0]->UpdateByInput();
	if ( m_pCharactors[0]->CollisionAtk( ) )
	{
		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity( &mat );
		m_pCharactors[0]->BreakQube( mat );

		CObjectManage::GetInstance()->Send_NetworkSendDestroyData( TRUE );
	}
	CTree::GetInstance()->SetMonsAtkClear();

	// 카메라: 캐릭터 위치,각도 받아오기
	m_pCamera->SetView( 
		m_pCharactors[0]->Get_CharaPos2Camera(), 
		m_pCharactors[0]->Get_PreCharaPos2Camera(), 
		10.0f, 75.0f, 
		m_pCharactors[0]->Get_CharaAngle(),
		CInput::GetInstance()->Get_MouseXRotate() );

	m_pCamera->CheckObjectCollision( 
		m_pCamera->GetEye(), 
		m_pCharactors[0]->Get_CharaPos(), 
		m_pCharactors[0]->Get_CharaAngle() );

	// 다른 플레이어는 값으로 이동
	for( INT Loop = 1; Loop < m_iMaxCharaNum; ++Loop )
	{			
		m_pCharactors[Loop]->UpdateOtherPlayer();
	}

	m_pMonster->Update();
	m_pMonster->UpdateByValue( D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f );

	// 아이템 쓸까나??
	m_pFirstAidKit->Update();

	//m_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );	
	m_pASEViewer->Update();

	m_pMainGUI->Update();
	m_pOptionScene->Update();

	CNetwork::GetInstance()->Update();
	m_pGameEvent->Update();	

	//if ( CInput::GetInstance()->Get_NumKey(7) )
	//	m_pMonster->BreakCubeAll();
}

VOID	CMainScene::Render()
{
	m_pMatrices->SetupProjection();

	m_pLight->EnableLight();

#ifdef _DEBUG
	m_pAxis->Render();
#endif

	//m_pD3dDevice->SetTransform( D3DTS_WORLD, &m_pASEViewe->Get_MatWorld() );
	m_pASEViewer->Render();

	for( INT Loop = 0; Loop < m_iMaxCharaNum; ++Loop )
	{
		if( !m_pCharactors[Loop] )
			continue;

		if( m_pCharactors[Loop]->Get_Active() )
		{
			m_pCharactors[Loop]->Render();
		}
	}

	m_pMonster->Render();

	m_pFirstAidKit->Render();

#ifdef _DEBUG
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	m_pD3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pTileMap->Render();
	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	TwDraw();
#endif

	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pMainGUI->Render();
	m_pOptionScene->Render();
	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

INT CMainScene::GetSceneNext()
{
	return m_scnNext;
}

INT CMainScene::GetSceneState()
{
	return m_scnState;
}