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
#include "GameEventCombo.h"
#include "EfSurface.h"

#include "MainGUI.h"
#include "OptionScene.h"

#include "GameEventTutorial.h"
#include "GameEventTutorialManager.h"
#include "GameEventScoreBoard.h"

// AI 테스트용
#include "Seek.h"
#include "Chase.h"

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
	m_pMatrices					= NULL;
	m_pCamera					= NULL;
	m_pD3dDevice				= NULL;
	m_pCharactors				= NULL;
	m_pAxis						= NULL;
	m_pMonster					= NULL;
	m_pTileMap					= NULL;
	m_pASEViewer				= NULL;
	
	m_pMainGUI					= NULL;
	m_pOptionScene				= NULL;
	m_pEventGUICombo			= NULL;

	m_pGameEventTutorialManager	= NULL;

	m_iMaxCharaNum = CObjectManage::GetInstance()->Get_MaxCharaNum();


	//m_pCharView = new CEfSurface();
}

HRESULT CMainScene::Create( LPDIRECT3DDEVICE9 a_pD3dDevice, LPD3DXSPRITE a_Sprite, HWND a_hWnd )
{
	m_pD3dDevice = a_pD3dDevice;

	m_pMatrices = CMatrices::GetInstance();

#ifdef _DEBUG
	FLOAT fSize = 1000.0f;
	INT nDeep = 4;
	m_pAxis = new Axis( m_pD3dDevice );
	m_pAxis->Create( fSize, nDeep );
#endif

	//카메라 생성
	m_pCamera = new CCamera;
	m_pCamera->Create( m_pD3dDevice );
	//m_pCharView->Create( m_pD3dDevice );
	//m_pCharView->Restore();

	// GUI 생성
	m_pMainGUI = new MainGUI( m_pD3dDevice, a_Sprite, a_hWnd );
	m_pMainGUI->Create();
	m_pOptionScene = new OptionScene;
	m_pOptionScene->Create( m_pD3dDevice, a_Sprite, a_hWnd );

	// 튜토리얼	// 이벤트
	INT nMaxCharaNum = CObjectManage::GetInstance()->Get_MaxCharaNum();
	m_pGameEvent = CGameEvent::GetInstance();
	m_pGameEvent->Create( m_pD3dDevice, nMaxCharaNum );
	m_pEventGUICombo = new CGameEventCombo( m_pD3dDevice, CObjectManage::GetInstance()->GetSprite() );
	m_pEventGUICombo->Create();
	m_pEventGUICombo->Initialize();
	m_pEventGUICombo->SetTime( 15.0f );
	m_pGameEventTutorialManager = new GameEventTutorialManager();
	m_pGameEventTutorialManager->Create( a_pD3dDevice, a_Sprite );
	m_pGameEventScoreBoard = new GameEventScoreBoard(m_pD3dDevice, CObjectManage::GetInstance()->GetSprite(), GHWND );
	m_pGameEventScoreBoard->Create();
	m_pGameEventScoreBoard->SetState( GameEventScoreBoard::GES_HIDDEN );
	
	INT nChar, nCount = 0;
	for (int Loop = 0; Loop < 4; ++Loop)
	{
		nChar = CObjectManage::GetInstance()->Get_CharTable( Loop );

 		if ( nChar > 0 )
			m_pGameEventScoreBoard->AddData( nCount++, nChar );
	}
	


	//맵 생성
	m_pASEViewer = CObjectManage::GetInstance()->Get_ASEViewer();
	m_pASEViewer->Create(  L"ASE File/Map/Stage_Beta.ASE", L"ASE File/Map/Stage_Beta_Box.BBX" );

	//타일맵 생성
	m_pTileMap = new TileMap( m_pD3dDevice );
	m_pTileMap->Create( D3DXVECTOR3( -510.0f, 0.0f, -950.0f ), D3DXVECTOR3( 510.0f, 0.0f, 950.0f ), 10.0f );
	m_pTileMap->LoadBBXFile( L"ASE File/Map/Stage_Beta_Box.BBX" );

	// GUI 초기화
	for (int i = 0; i < MainGUI::MMP_END; ++i )
	{
		m_pMainGUI->SetMiniMapObjectVisible( i, FALSE );
	}

	//캐릭터 생성
	m_nClientID = CObjectManage::GetInstance()->Get_ClientNumber();
	CreateCharactor();

	//몬스터 생성
	m_pMonster = CObjectManage::GetInstance()->Get_Monster();
	CreateMonster();

	// 아이템 생성
	m_pFirstAidKit = CObjectManage::GetInstance()->Get_FirstAidKit();
	CreateFirstAidKit();

	// 잡동사니 생성
	m_pWall = CObjectManage::GetInstance()->Get_Wall();
	CreateWall();
	
	//조명 생성
	m_pLight = new CLight;
	m_pLight->Create( m_pD3dDevice );

	Seek::GetInstance()->Initialize( m_pTileMap );
	Astar::GetInstance()->Initialize( m_pTileMap->GetInfo() );

	// 프로젝션 설정
	m_pMatrices->SetupProjection();


	CInput::GetInstance()->EnableInput( FALSE );

	if ( CObjectManage::GetInstance()->IsHost() )
	{
		CGameEvent::GetInstance()->AddEvent( CGameEvent::SCENE_TUTORIAL, 0.01f );
	}

	CDebugConsole::GetInstance()->Messagef( L"**** MainScene Create End **** \n\n" );

	CSound::GetInstance()->StopBGM( 0 );
	CSound::GetInstance()->PlayBGM( CSound::BGM_GAME );

	return S_OK;
}

HRESULT CMainScene::Release()
{
	//SAFE_DELETE( m_pCharView );
	SAFE_DELETE( m_pLight );
	SAFE_DELETE( m_pAxis );
	SAFE_DELETE( m_pOptionScene );
	SAFE_DELETE( m_pMainGUI );
	SAFE_DELETE( m_pTileMap );
	SAFE_DELETE( m_pCamera );
	SAFE_DELETE( m_pGameEventTutorialManager );
	SAFE_DELETE( m_pEventGUICombo );

	return S_OK;
}

VOID CMainScene::CreateCharactor()
{
	//캐릭터 생성
	CCharactor * pChar;
	m_pCharactors = CObjectManage::GetInstance()->Get_Charactor();

	INT nIndex;
	for(INT Loop = 0; Loop < m_iMaxCharaNum; ++Loop )
	{
		nIndex = CObjectManage::GetInstance()->Get_CharTable(Loop);

		if ( nIndex != -1 )
		{
			pChar = &( m_pCharactors[ nIndex ] );
			CTree::GetInstance()->GetCharVector()->push_back( pChar->GetBoundBox() );

			m_pMainGUI->SetMiniMapObjectVisible( MainGUI::MMP_DADDY + nIndex, TRUE );
			
			CGameEvent::GetInstance()->SetShotedPoint( Loop, -pChar->GetvCubeSize() );
		}
	}
}

VOID CMainScene::CreateMonster()
{
	m_pMonster[1]->Create( m_pD3dDevice, L"Data/CharData/11_16_pa_sm_v6" );

	for( INT Loop = 0; Loop < 3; ++Loop )
	{
		if ( m_pGameEvent->GetMonsterState() & ( 0x0001 << Loop ) )
		{
			m_pMonster[ Loop ]->InitAniAndState();

			m_pMainGUI->SetMiniMapObjectVisible( MainGUI::MMP_PANDA + Loop, TRUE );
		}
	}
}

VOID CMainScene::CreateFirstAidKit()
{
	for ( int Loop = 0; Loop < 4; ++Loop )
	{
		m_pFirstAidKit[Loop].Create( m_pD3dDevice );
		m_pFirstAidKit[Loop].Load( L"Data/CharData/FirstAidKit_1.csav" );
		m_pFirstAidKit[Loop].Set_MonsterNumber( CGameEvent::ITEM_FAK1 << Loop );
		CTree::GetInstance()->GetMonsVector()->push_back( m_pFirstAidKit[Loop].GetBoundBox() );		

		//m_pMainGUI->SetMiniMapObjectVisible( MainGUI::MMP_FIRSTAIDKIT_0 + Loop, TRUE );
	}
}

VOID CMainScene::CreateWall()
{
	for ( int Loop = 0; Loop < 3; ++Loop )
	{
		m_pWall[Loop].Create( m_pD3dDevice );
		if( Loop == 1 )		m_pWall[Loop].Load( L"Data/CharData/Obstacle_0.csav" );
		else				m_pWall[Loop].Load( L"Data/CharData/Obstacle_1.csav" );
		m_pWall[Loop].Set_MonsterNumber( CGameEvent::DOOR_LEFT << Loop );
		CTree::GetInstance()->GetMonsVector()->push_back( m_pWall[Loop].GetBoundBox() );
	}
}

VOID CMainScene::CheatKeys()
{
	if( CInput::GetInstance()->Get_NumKey( 0 ) )
	{
		m_pCamera->SetEffect( NONE );
		m_pGameEvent->ClearEvent();
		m_pGameEvent->AddEvent( CGameEvent::SCENE_TUTORIAL_END, 0.1f);
	}
	if( CInput::GetInstance()->Get_NumKey( 1 ) )
	{
		m_pGameEvent->ClearEvent();
		m_pGameEvent->AddEvent( CGameEvent::EVENT_COMBO, 0.01f);
	}
}

VOID CMainScene::Update()
{
	D3DXVECTOR3 vPos;
	CGameEvent::GetInstance()->IndexInit();
	CNetwork::GetInstance()->UpdateGame();

	CheatKeys();
	CObjectManage * pOM = CObjectManage::GetInstance();
	CCharactor * pChar;
	pChar = &( m_pCharactors[ pOM->Get_CharTable( m_nClientID ) ] );

	// 캐릭터: 인풋 값 받아오기
	pChar->UpdateByInput( );

	if ( pChar->CollisionAtk( ) )
	{
		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity( &mat );
		if ( pChar->BreakQube( mat ) )
		{
			m_pCamera->SetEffect( 1 );
		}
		pOM->Send_NetworkSendDestroyData( FALSE, 0 );
	}

	CTree::GetInstance()->SetMonsAtkClear();

	// 카메라: 캐릭터 위치,각도 받아오기
	m_pCamera->SetView( pChar->Get_CharaPos2Camera(), pChar->Get_PreCharaPos2Camera(), 
						10.0f, 75.0f, 
						pChar->Get_CharaAngle(), CInput::GetInstance()->Get_MouseXRotate() );

	// 다른 플레이어는 값으로 이동
	INT nIndex;
	for( INT Loop = 0; Loop < m_iMaxCharaNum; ++Loop )
	{
		nIndex = pOM->Get_CharTable( Loop );
		if ( nIndex == -1 )
			continue;
	
		pChar = &( m_pCharactors[ nIndex ] );

		vPos = pChar->Get_CharaPos();
		m_pMainGUI->SetMiniMapObjectPosition( MainGUI::MMP_DADDY + nIndex, &vPos );

		if( Loop != m_nClientID )
		{
			pChar->UpdateOtherPlayer();			
		}
	}

	for ( int Loop = 0; Loop < 3; ++Loop )
	{
		if ( m_pGameEvent->GetMonsterState() & ( 0x0001 << Loop ) )
		{
			m_pMonster[Loop]->Update();
			vPos = m_pMonster[Loop]->Get_Pos();
			m_pMainGUI->SetMiniMapObjectPosition( MainGUI::MMP_PANDA + Loop, &vPos );
		}
	}

	// 아이템 쓸까나??
	for ( int Loop = 0; Loop < 4; ++Loop )
	{
		m_pFirstAidKit[Loop].Update();
		vPos = m_pFirstAidKit[Loop].Get_CharaPos();
		m_pMainGUI->SetMiniMapObjectPosition( MainGUI::MMP_FIRSTAIDKIT_0 + Loop, &vPos );
	}

	for ( int Loop = 0; Loop < 3; ++Loop )
	{
		m_pWall[Loop].Update();
		CObjectManage::GetInstance()->Send_NetworkSendDestroyData( TRUE + 1, Loop );
	}
	
	m_pASEViewer->Update();

	m_pMainGUI->Update();
	m_pOptionScene->Update();

	m_pGameEventTutorialManager->Update();
	m_pGameEventScoreBoard->Update();

	m_pEventGUICombo->Update();
	
	EventSwitch( m_pGameEvent->Update() );

	CTree::GetInstance()->SetCharAtkClear();

	//m_pCharView->Update( &( m_pCharactors[ pOM->Get_CharTable( m_nClientID ) ] ) );
	//m_pCharView->Update( m_pASEViewer );
}

VOID	CMainScene::Render()
{
	CCharactor * pChar;
	m_pMatrices->SetupProjection();

	m_pLight->EnableLight();
/*
#ifdef _DEBUG
	m_pAxis->Render();
#endif
*/
	m_pD3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	m_pASEViewer->Render();
	m_pD3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	m_pLight->EnableCharacterLight();

	INT nIndex;
	for( INT Loop = 0; Loop < m_iMaxCharaNum; ++Loop )
	{
		nIndex = CObjectManage::GetInstance()->Get_CharTable( Loop );
		if ( nIndex != -1 )
		{
			pChar = &( m_pCharactors[ nIndex ] );
			pChar->Render();
		}
	}

	for ( int Loop = 0; Loop < 3; ++Loop )
	{
		if ( m_pGameEvent->GetMonsterState() & ( 0x0001 << Loop ) )
		{
			m_pMonster[Loop]->Render();
		}
	}
	
	for ( int Loop = 0; Loop < 3; ++Loop )
	{
		if ( m_pGameEvent->GetMonsterState() & ( CGameEvent::ITEM_FAK1 << Loop ) )
		{
			m_pFirstAidKit[Loop].Render();
		}
	}
	
	for ( int Loop = 0; Loop < 3; ++Loop )
	{
		if ( m_pGameEvent->GetMonsterState() & ( CGameEvent::DOOR_LEFT << Loop ) )
		{
			m_pWall[Loop].Render();
		}
	}

	m_pLight->EnableLight();

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
	m_pGameEventTutorialManager->Render();
	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	m_pEventGUICombo->Render();
	m_pGameEventScoreBoard->Render();

	//m_pCharView->Render();
}

INT CMainScene::GetSceneNext()
{
	return m_scnNext;
}

INT CMainScene::GetSceneState()
{
	return m_scnState;
}

VOID CMainScene::EventInitGameState( INT nEvent )
{
	EventInitCharState( nEvent );
	EventInitMonsterState( nEvent );
}

VOID CMainScene::EventInitCharState( INT nEvent )
{
	switch ( nEvent )
	{
	case CGameEvent::SCENE_TUTORIAL:
		m_pCharactors[0].Set_Position( m_pGameEvent->GetCharPosition( 0, 0 ) );
		m_pCharactors[1].Set_Position( m_pGameEvent->GetCharPosition( 0, 1 ) );
		m_pCharactors[2].Set_Position( m_pGameEvent->GetCharPosition( 0, 2 ) );
		m_pCharactors[3].Set_Position( m_pGameEvent->GetCharPosition( 0, 3 ) );
		break;
	case CGameEvent::SCENE_BEAR:
		m_pCharactors[0].Set_Position( m_pGameEvent->GetCharPosition( 1, 0 ) );
		m_pCharactors[1].Set_Position( m_pGameEvent->GetCharPosition( 1, 1 ) );
		m_pCharactors[2].Set_Position( m_pGameEvent->GetCharPosition( 1, 2 ) );
		m_pCharactors[3].Set_Position( m_pGameEvent->GetCharPosition( 1, 3 ) );
		break;
	default:
		break;
	}
}

VOID CMainScene::EventInitMonsterState( INT nEvent )
{
	switch ( nEvent )
	{
	case CGameEvent::SCENE_TUTORIAL:
		m_pGameEvent->SetMonstersState( CGameEvent::DOOR_LEFT | CGameEvent::DOOR_CENTER | CGameEvent::DOOR_RIGHT );
		
		for (int Loop = 0; Loop < 3; ++Loop )
		{
			m_pWall[Loop].Set_Position( m_pGameEvent->GetWallPosition( Loop ) );
			m_pWall[Loop].SetActive( TRUE );
		}
		
		break;
	case CGameEvent::SCENE_TUTORIAL_END:
		for (int Loop = 0; Loop < 3; ++Loop )
		{
			m_pWall[Loop].Set_Position( m_pGameEvent->GetNonePosition() );
			m_pWall[Loop].SetActive( FALSE );
		}
		break;
	case CGameEvent::SCENE_BEAR:
		m_pGameEvent->SetMonstersState( CGameEvent::BEAR | CGameEvent::PANDA );
		for ( int Loop = 0; Loop < 3; ++Loop )
		{
			if ( m_pGameEvent->GetMonsterState() & ( 0x0001 << Loop ) )
			{
				m_pMonster[Loop]->Set_Pos( m_pGameEvent->GetMonsPosition( Loop ) );
				m_pMonster[Loop]->Set_Angle( 0.0f );
				m_pMonster[Loop]->ChangeAnimation( CMonster::ANIM_STAND );
				m_pMonster[Loop]->EnableShadow( TRUE );
				if( CObjectManage::GetInstance()->IsHost() == TRUE )
				{
					m_pMonster[Loop]->GetFSM()->SetCurrentState( Seek::GetInstance() );
				}

				m_pMainGUI->SetMiniMapObjectVisible( MainGUI::MMP_PANDA + Loop, TRUE );
			}
			else
			{
				m_pMainGUI->SetMiniMapObjectVisible( MainGUI::MMP_PANDA + Loop, FALSE );
			}
		}
		break;
	case CGameEvent::SCENE_CLOWN:
		m_pGameEvent->SetMonstersState( CGameEvent::CLOWN );
		for ( int Loop = 0; Loop < 3; ++Loop )
		{
			if ( m_pGameEvent->GetMonsterState() & ( 0x0001 << Loop ) )
			{
				m_pMonster[Loop]->Set_Pos( m_pGameEvent->GetMonsPosition( Loop ) );
				m_pMonster[Loop]->Set_Angle( 0.0f );
				m_pMonster[Loop]->ChangeAnimation( CMonster::ANIM_STAND );
				m_pMonster[Loop]->EnableShadow( TRUE );
				if( CObjectManage::GetInstance()->IsHost() == TRUE )
				{
					m_pMonster[Loop]->GetFSM()->SetCurrentState( Seek::GetInstance() );
				}

				m_pMainGUI->SetMiniMapObjectVisible( MainGUI::MMP_PANDA + Loop, TRUE );
			}
			else
			{
				m_pMainGUI->SetMiniMapObjectVisible( MainGUI::MMP_PANDA + Loop, FALSE );
			}
		}
		break;
	default:
		break;
	}


}

VOID CMainScene::EventSwitch( INT nEvent )
{
	FLOAT fComboTime = 15.0f;
	FLOAT fComboTerm = 15.0f;

	switch ( nEvent )
	{
	case CGameEvent::SCENE_TUTORIAL:
		CDebugConsole::GetInstance()->Message( "CGameEvent::SCENE_TUTORIAL\n" );
		EventStateNetwork( nEvent );
		EventInitGameState( nEvent );
		CGameEvent::GetInstance()->AddEvent( CGameEvent::MAP_WALK, 1.0f );
		break;
	case CGameEvent::MAP_WALK:
		CDebugConsole::GetInstance()->Message( "CGameEvent::MAP_WALK \n" );
		EventStateNetwork( nEvent );
		EventMapCameraWalk( CCamera::MAP_WALK );
		break;
	case CGameEvent::MAP_WALK_END:
		CDebugConsole::GetInstance()->Message( "CGameEvent::MAP_WALK_END \n" );
		EventStateNetwork( nEvent );
		CGameEvent::GetInstance()->AddEvent( CGameEvent::TUTORIAL_ATACK, 5.0f );
		break;
	case CGameEvent::TUTORIAL_ATACK:
		CDebugConsole::GetInstance()->Message( "CGameEvent::TUTORIAL_ATACK \n" );
		CGameEvent::GetInstance()->SetTutorial( nEvent );
		TutorialAtack( );
		CGameEvent::GetInstance()->AddEvent( CGameEvent::TUTORIAL_ATACK_END, fComboTime );
		if ( CObjectManage::GetInstance()->IsHost() )
		{
			CGameEvent::GetInstance()->AddEvent( CGameEvent::TUTORIAL_COMBO, fComboTime + fComboTerm );
		}
		break;
	case CGameEvent::TUTORIAL_ATACK_END:
		CDebugConsole::GetInstance()->Message( "CGameEvent::TUTORIAL_ATACK_END \n" );
		TutorialAtackEnd();
		break;
	case CGameEvent::TUTORIAL_COMBO:
		CDebugConsole::GetInstance()->Message( "CGameEvent::TUTORIAL_COMBO \n" );
		EventStateNetwork( nEvent );
		
		if ( CGameEvent::GetInstance()->GetTutorial() == CGameEvent::TUTORIAL_ATACK )
		{
			CGameEvent::GetInstance()->SetTutorial( nEvent );
			TutorialCombo();
		}		
		if ( CGameEvent::GetInstance()->GetTutorial() == CGameEvent::TUTORIAL_COMBO )
		{			
			EventCombo();			
			if ( CObjectManage::GetInstance()->IsHost() )
			{
				CGameEvent::GetInstance()->AddEvent( CGameEvent::TUTORIAL_COMBO_FAIL, fComboTime );
				CGameEvent::GetInstance()->AddEvent( CGameEvent::TUTORIAL_COMBO, fComboTime + fComboTerm );
			}
		}		
		break;
	case CGameEvent::TUTORIAL_COMBO_FAIL:		
		if ( CGameEvent::GetInstance()->GetTutorial() == CGameEvent::TUTORIAL_COMBO )
		{
			CDebugConsole::GetInstance()->Message( "CGameEvent::TUTORIAL_COMBO_FAIL \n" );
			EventStateNetwork( nEvent );
			m_pEventGUICombo->Fail();
			
			if ( CObjectManage::GetInstance()->IsHost() )
			{
				CGameEvent::GetInstance()->AddEvent( CGameEvent::TUTORIAL_COMBO_END, 3.0f );
			}
		}
		break;
	case CGameEvent::TUTORIAL_COMBO_SUCCESS:
		CDebugConsole::GetInstance()->Message( "CGameEvent::TUTORIAL_COMBO_SUCCESS \n" );
		EventStateNetwork( nEvent );		
		DoorBreakNockdown();
		if ( CGameEvent::GetInstance()->GetTutorial( ) == CGameEvent::TUTORIAL_COMBO )
		{
			CGameEvent::GetInstance()->SetTutorial( nEvent );

			if ( CObjectManage::GetInstance()->IsHost() )
			{
				CGameEvent::GetInstance()->AddEvent( CGameEvent::TUTORIAL_COMBO_END, 3.0f );
				CGameEvent::GetInstance()->AddEvent( CGameEvent::SCENE_TUTORIAL_END, 4.0f );
			}
		}

		break;
	case CGameEvent::TUTORIAL_COMBO_END:
		CDebugConsole::GetInstance()->Message( "CGameEvent::TUTORIAL_COMBO_END \n" );
		EventStateNetwork( nEvent );
		m_pGameEventTutorialManager->EndEvent();
		EventComboEnd();
		if ( CObjectManage::GetInstance()->IsHost() )
		{
			CGameEvent::GetInstance()->AddEvent( CGameEvent::TUTORIAL_COMBO, fComboTime + fComboTerm );
		}

		EventInitGameState( nEvent );		
		break;
	case CGameEvent::SCENE_TUTORIAL_END:
		CDebugConsole::GetInstance()->Message( "CGameEvent::SCENE_TUTORIAL_END \n" );
		CGameEvent::GetInstance()->SetTutorial( nEvent );
		EventStateNetwork( nEvent );
		EventInitGameState( nEvent );
		CGameEvent::GetInstance()->ClearEvent( );
		if ( CObjectManage::GetInstance()->IsHost() )
		{
			CGameEvent::GetInstance()->AddEvent( CGameEvent::SCENE_BEAR, 0.01f );
		}
		break;
	case CGameEvent::SCENE_BEAR:
		CDebugConsole::GetInstance()->Message( "CGameEvent::SCENE_BEAR \n" );
		CGameEvent::GetInstance()->SetTutorial( nEvent );
		EventInitGameState( nEvent );
		EventStateNetwork( nEvent );
		if ( CObjectManage::GetInstance()->IsHost() )
		{			
			CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO, fComboTime + fComboTerm );
		}
		break;
	case CGameEvent::SCENE_BEAR_END:
		CGameEvent::GetInstance()->ClearEvent( );
		m_pEventGUICombo->Initialize();
		if ( CObjectManage::GetInstance()->IsHost() )
		{
			CGameEvent::GetInstance()->AddEvent( CGameEvent::SCENE_CLOWN, 0.01f );
		}
		break;
//////////////////////////////////////////////////////////////////////////
	case CGameEvent::SCENE_CLOWN:
		CDebugConsole::GetInstance()->Message( "CGameEvent::SCENE_CLOWN \n" );
		CGameEvent::GetInstance()->SetTutorial( nEvent );
		EventStateNetwork( nEvent );
		EventMapCameraWalk( CCamera::TARGET_SHOW );
		EventInitGameState( nEvent );
		break;

	case CGameEvent::EVENT_COMBO:
		CDebugConsole::GetInstance()->Message( "CGameEvent::EVENT_COMBO \n" );
		EventStateNetwork( nEvent );
		EventCombo();
		if ( CObjectManage::GetInstance()->IsHost() ) 
		{
			CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO_FAIL, fComboTime );
			//CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO, fComboTime + fComboTerm );

			CNetwork::GetInstance()->CS_EVENT_COMBO( m_pEventGUICombo->GetKindEvet() ); 
		}
		break;
	case CGameEvent::EVENT_COMBO_FAIL:
		CDebugConsole::GetInstance()->Message( "CGameEvent::EVENT_COMBO_FAIL \n" );
		m_pEventGUICombo->Fail();
		if ( CObjectManage::GetInstance()->IsHost() ) 		
		{
			//if ( CGameEvent::GetInstance()->GetTutorial() < CGameEvent::SCENE_BEAR )
			//{
			//	CGameEvent::GetInstance()->AddEvent( CGameEvent::TUTORIAL_COMBO_FAIL, 0.01f );
			//}
			//else
			//{
				CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO_END, 3.0f );
				CNetwork::GetInstance()->CS_EVENT_STATE( nEvent ); 
			//}
		}
		break;
	case CGameEvent::EVENT_COMBO_SUCCESS:
		CDebugConsole::GetInstance()->Message( "CGameEvent::EVENT_COMBO_SUCCESS \n" );
		m_pEventGUICombo->Success();
		if ( CObjectManage::GetInstance()->IsHost() ) 
		{
			if ( CGameEvent::GetInstance()->GetTutorial() < CGameEvent::SCENE_BEAR )
			{
				CGameEvent::GetInstance()->AddEvent( CGameEvent::TUTORIAL_COMBO_SUCCESS, 0.01f );
			}
			else
			{
				CGameEvent::GetInstance()->AddEvent( CGameEvent::MONSTER_BREAK_NOCKDOWN, 2.0f );				
				CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO_END, 3.0f );
				CNetwork::GetInstance()->CS_EVENT_STATE( nEvent ); 
			}
		}		
		break;
	case CGameEvent::EVENT_COMBO_END:
		CDebugConsole::GetInstance()->Message( "CGameEvent::EVENT_COMBO_END \n" );		
		if ( CObjectManage::GetInstance()->IsHost() ) 
		{
			EventComboEnd();
			if ( CObjectManage::GetInstance()->IsHost() ) 		
			{
				CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO, fComboTime + fComboTerm );
			}
			CNetwork::GetInstance()->CS_EVENT_STATE( nEvent ); 
		}
		break;
	//case CGameEvent::EVENT_FAK:
	//	CDebugConsole::GetInstance()->Message( "CGameEvent::EVENT_FAK \n" );
	//	EventFirstAidKit();
	//	/// 미구현
	//	break;
	case CGameEvent::MONSTER_BREAK_NOCKDOWN:
		CDebugConsole::GetInstance()->Message( "CGameEvent::MONSTER_BREAK_NOCKDOWN \n" );
		MonsterBreakNockdown();
		EventStateNetwork( nEvent );
		break;
	case CGameEvent::DOOR_BREAK_NOCKDOWN:
		CDebugConsole::GetInstance()->Message( "CGameEvent::DOOR_BREAK_NOCKDOWN \n" );
		DoorBreakNockdown();
		EventStateNetwork( nEvent );
		break;
	case CGameEvent::GAME_WIN_END:
		EventStateNetwork( nEvent );
		CDebugConsole::GetInstance()->Message( "CGameEvent::GAME_WIN_END \n" );
		GamePoint();
		//MessageBox( GHWND, L"게임종료", L"축하", MB_OK );
		//GameEnd();
		break;
	case CGameEvent::GAME_LOSE_END:
		EventStateNetwork( nEvent );
		CDebugConsole::GetInstance()->Message( "CGameEvent::GAME_LOSE_END \n" );
		//GameEnd();
		break;
	default:
		break;
	}
}

VOID CMainScene::GamePoint()
{
	INT nChar, nCount = 0;
	for (int Loop = 0; Loop < 4; ++Loop)
	{
		nChar = CObjectManage::GetInstance()->Get_CharTable( Loop );

		if ( nChar > 0 )
			m_pGameEventScoreBoard->SetScore( Loop, CGameEvent::GetInstance()->GetAttackPoint( Loop ) );
	}

	m_pGameEventScoreBoard->SetState( GameEventScoreBoard::GES_GRAY );
}

VOID CMainScene::GameEnd()
{
	CInput::GetInstance()->EnableInput( TRUE );
	m_scnNext	= IScene::SCENE_MENU;
	m_scnState	= IScene::SCENE_END;

	CNetwork::GetInstance()->Close();
	CNetwork::DestoryInstance();
}

VOID CMainScene::EventStateNetwork( INT nEvent )
{
	if ( CObjectManage::GetInstance()->IsHost() )
	{
		CNetwork::GetInstance()->CS_EVENT_STATE( nEvent ); 
	}
}

VOID CMainScene::EventMapCameraWalk( INT nEvent )
{
	m_pCamera->SetEffect( nEvent );
}

VOID CMainScene::TutorialAtack()
{
	// 공격 콤보, 아이템설명
	m_pGameEventTutorialManager->ChangeEvent( GameEventTutorial::TUTORIAL_COMBO );
}

VOID CMainScene::TutorialAtackEnd()
{
	m_pGameEventTutorialManager->EndEvent();
}

VOID CMainScene::TutorialCombo()
{
	// 신호등 설명창	
	m_pGameEventTutorialManager->ChangeEvent( GameEventTutorial::TUTORIAL_SIGNALLIGHT );
}

VOID CMainScene::EventCombo()
{
	m_pEventGUICombo->Initialize();
	m_pEventGUICombo->Start();

	CObjectManage * pOM = CObjectManage::GetInstance();

	if ( pOM->IsHost() )
	{
		INT nClient = 0;
		// 접속 클라이언트 찾기.
		for ( int i = 0; i < 4; ++i )
		{
			if (pOM->Get_CharTable()[i] != -1 )
			{
				nClient++;
			}
		}

		INT nSelect;
		for ( int i = 0; i < 4; ++i )
		{
			nSelect = static_cast<int>( FastRand2() * nClient );
			m_pEventGUICombo->AddCombo( i, pOM->Get_CharTable()[nSelect] );
		}		
	}
	else
	{
		for ( int i = 0; i < 4; ++i )
		{
			m_pEventGUICombo->AddCombo( i, pOM->Get_EventTable()[i] );
		}		
	}

	if ( CObjectManage::GetInstance()->IsHost() ) 
	{
		CNetwork::GetInstance()->CS_EVENT_COMBO( m_pEventGUICombo->GetKindEvet() ); 
	}
}

VOID CMainScene::EventComboEnd()
{
	CGameEvent::GetInstance()->ClearCombo();
	m_pEventGUICombo->Initialize();	
}

VOID CMainScene::EventFirstAidKit()
{
	m_pFirstAidKit->SetActive( TRUE );
	m_pGameEventTutorialManager->ChangeEvent( GameEventTutorial::TUTORIAL_FIRSTAID );
}

VOID CMainScene::MonsterBreakNockdown()
{
	if (CGameEvent::GetInstance()->GetMonsterState() == CGameEvent::CLOWN )
	{
		if ( m_pMonster[ 2 ]->BreakNockdown( TRUE ) && CObjectManage::GetInstance()->IsHost() )
		{
			m_pMonster[ 2 ]->GetFSM()->SetCurrentState( NULL );

			CGameEvent::GetInstance()->AddEvent( CGameEvent::GAME_WIN_END, 2.0f );
		}
	}
	else
	{
		m_pMonster[ 0 ]->BreakNockdown();
		m_pMonster[ 0 ]->ChangeAnimation( CMonster::ANIM_STAND );
		m_pMonster[ 1 ]->BreakNockdown();
		m_pMonster[ 1 ]->ChangeAnimation( CMonster::ANIM_STAND );

		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			m_pMonster[ 0 ]->GetFSM()->SetCurrentState( NULL );
			m_pMonster[ 1 ]->GetFSM()->SetCurrentState( NULL );
		}

		CGameEvent::GetInstance()->AddEvent( CGameEvent::SCENE_BEAR_END, 2.0f );
	}
}

VOID CMainScene::DoorBreakNockdown()
{
	m_pWall[0].BreakNockdown();
	m_pWall[1].BreakNockdown();
	m_pWall[2].BreakNockdown();

	CSound::GetInstance()->PlayEffect( 0 );
}