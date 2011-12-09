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


#include "MainGUI.h"
#include "OptionScene.h"

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
	m_pMatrices		= NULL;
	m_pCamera		= NULL;
	m_pD3dDevice	= NULL;
	//m_pMyCharactors = NULL;
	//m_pOtherCharactors = NULL;
	m_pCharactors	= NULL;
	m_pAxis			= NULL;
	m_pMonster		= NULL;
	m_pTileMap		= NULL;
	m_pASEViewer	= NULL;
	
	m_pMainGUI		= NULL;
	m_pOptionScene	= NULL;
	m_pEventGUICombo= NULL;

	m_iMaxCharaNum = CObjectManage::GetInstance()->Get_MaxCharaNum();
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

	//이벤트
	INT nMaxCharaNum = CObjectManage::GetInstance()->Get_MaxCharaNum();
	CGameEvent::GetInstance()->Create( m_pD3dDevice, nMaxCharaNum );
	//m_pGameEvent = new CGameEvent( nMaxCharaNum, this );
	//m_pGameEvent->Create( m_pD3dDevice );

	//맵 생성
	m_pASEViewer = CObjectManage::GetInstance()->Get_ASEViewer();
	m_pASEViewer->Create(  L"ASE File/Map/Stage_Beta.ASE", L"ASE File/Map/Stage_Beta_Box.BBX" );

	//타일맵 생성
	m_pTileMap = new TileMap( m_pD3dDevice );
	m_pTileMap->Create( D3DXVECTOR3( -510.0f, 0.0f, -950.0f ), D3DXVECTOR3( 510.0f, 0.0f, 950.0f ), 10.0f );
	m_pTileMap->LoadBBXFile( L"ASE File/Map/Stage_Beta_Box.BBX" );

	//캐릭터 생성
	m_nClientID = CObjectManage::GetInstance()->Get_ClientNumber();
	CreateCharactor();

	//몬스터 생성
	m_pMonster = CObjectManage::GetInstance()->Get_Monster();
	m_pMonster[1]->Create( m_pD3dDevice, L"Data/CharData/11_16_pa_sm_v6" );
	//m_pMonster->Create( m_pD3dDevice, L"Data/CharData/27_pierro_body_11_28" );
	//m_pMonster->ChangeAnimation( CMonster::ANIM_STAND );

	m_pGameEvent = CGameEvent::GetInstance();
	if ( m_pGameEvent->GetMonsterState() & 0x01 )	//1 2 3 4
	{
		m_pMonster[0]->InitAniAndState();
	}
	if ( m_pGameEvent->GetMonsterState() & 0x02 )
	{
		m_pMonster[1]->InitAniAndState();
	}
	if ( m_pGameEvent->GetMonsterState()  & 0x04 )
	{
		m_pMonster[2]->InitAniAndState();
	}

	// 아이템 생성
	m_pFirstAidKit = CObjectManage::GetInstance()->Get_FirstAidKit();
	m_pFirstAidKit->Create( m_pD3dDevice );
	m_pFirstAidKit->Load( L"Data/CharData/FirstAidKit_1.csav" );

	//조명 생성
	m_pLight = new CLight;
	m_pLight->Create( m_pD3dDevice );

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
	CCharactor * pChar;
	m_pCharactors = CObjectManage::GetInstance()->Get_Charactor();

	INT nIndex;
	for(INT Loop = 0; Loop < m_iMaxCharaNum; ++Loop )
	{
		nIndex = CObjectManage::GetInstance()->Get_CharTable(Loop);

		if ( nIndex != -1 )
		{
			pChar = &( m_pCharactors[ nIndex ] );

			//m_pCharactors[Loop]->Create( m_pD3dDevice );
			//m_pCharactors[Loop]->LoadKindChar( Loop );

			//if (Loop == m_nClientID)
			//{
			//	m_pCharactors[m_nClientID]->Set_Active( TRUE );
			//}

			//pChar->Set_Position( m_pGameEvent->GetDefaultCharPosition( Loop ) );
			CTree::GetInstance()->GetCharVector()->push_back( pChar->GetBoundBox() );
		}
	}
}

VOID CMainScene::MonsterBreakNockdown()
{
//	CGameEvent::GetInstance()->GetMonsterIndex()
	if ( CGameEvent::GetInstance()->GetMonsterState()  & 0x01 )	//1 2 3 4
	{
		m_pMonster[0]->BreakNockdown();
	}
	if ( CGameEvent::GetInstance()->GetMonsterState()  & 0x02 )
	{
		m_pMonster[1]->BreakNockdown();
	}
	if ( CGameEvent::GetInstance()->GetMonsterState()  & 0x04 )
	{
		m_pMonster[2]->BreakNockdown();
	}
}

VOID CMainScene::Update()
{
	// 치트키 처리
#ifdef _DEBUG
	if( GetKeyState( '5' ) & 0x8000 )
		m_pCamera->SetEffect(1);
	if( GetKeyState( '6' ) & 0x8000 )
		m_pGameEvent->AddEvent( CGameEvent::EVENT_COMBO, 0.1f);
#endif // _DEBUG

	CGameEvent::GetInstance()->Set_PlayerIndex( -1 );
	CGameEvent::GetInstance()->Set_MonsterIndex( -1 );

	CCharactor * pChar;
	pChar = &( m_pCharactors[ CObjectManage::GetInstance()->Get_CharTable( m_nClientID ) ]);

	// 캐릭터: 인풋 값 받아오기
	pChar->UpdateByInput();

	if ( pChar->CollisionAtk( ) )
	{
		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity( &mat );
		if ( pChar->BreakQube( mat ) )
			m_pCamera->SetEffect( 1 );
		CObjectManage::GetInstance()->Send_NetworkSendDestroyData( FALSE );
	}
	CTree::GetInstance()->SetMonsAtkClear();

	// 카메라: 캐릭터 위치,각도 받아오기
	m_pCamera->SetView( 
		pChar->Get_CharaPos2Camera(), 
		pChar->Get_PreCharaPos2Camera(), 
		10.0f, 75.0f, 
		pChar->Get_CharaAngle(),
		CInput::GetInstance()->Get_MouseXRotate() );

	m_pCamera->CheckObjectCollision( 
		m_pCamera->GetEye(), 
		pChar->Get_CharaPos(), 
		pChar->Get_CharaAngle() );

	// 다른 플레이어는 값으로 이동
	INT nIndex;
	for( INT Loop = 0; Loop < m_iMaxCharaNum; ++Loop )
	{
		if( Loop == m_nClientID )
			continue;
		nIndex = CObjectManage::GetInstance()->Get_CharTable( Loop );
		if ( nIndex != -1 )
		{
			pChar = &( m_pCharactors[ nIndex ] );
			pChar->UpdateOtherPlayer();
		}
	}

	pChar = &( m_pCharactors[ CObjectManage::GetInstance()->Get_CharTable( m_nClientID ) ]);


	if ( m_pGameEvent->GetMonsterState()  & 0x01 )	//1 2 3 4
	{
		m_pMonster[0]->Update();
	}
	if ( m_pGameEvent->GetMonsterState()  & 0x02 )
	{
		m_pMonster[1]->Update();
	}
	if ( m_pGameEvent->GetMonsterState()  & 0x04 )
	{
		m_pMonster[2]->Update();
	}

	//m_pMonster->Update();
	//m_pMonster->UpdateByValue( D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f );

	// 아이템 쓸까나??
	m_pFirstAidKit->Update();

	//m_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );	
	m_pASEViewer->Update();

	m_pMainGUI->Update();
	m_pOptionScene->Update();

	CNetwork::GetInstance()->UpdateGame();
	if ( m_pEventGUICombo )
		m_pEventGUICombo->Update();
	EventSwitch( m_pGameEvent->Update() );

	CTree::GetInstance()->SetCharAtkClear();
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

	if ( m_pGameEvent->GetMonsterState()  & 0x01 )	//1 2 3 4
		m_pMonster[0]->Render();
	if ( m_pGameEvent->GetMonsterState()  & 0x02 )
		m_pMonster[1]->Render();
	if ( m_pGameEvent->GetMonsterState()  & 0x04 )
		m_pMonster[2]->Render();

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

	if ( m_pEventGUICombo )
		m_pEventGUICombo->Render();
}

INT CMainScene::GetSceneNext()
{
	return m_scnNext;
}

INT CMainScene::GetSceneState()
{
	return m_scnState;
}

VOID CMainScene::InitCharState()
{
	for ( INT Loop = 0; Loop < 4; ++Loop )
	{
		m_pCharactors[Loop].Set_Position( m_pGameEvent->GetCharPosition( Loop ) );
	}
}

VOID CMainScene::InitMonsterState()
{
	if ( m_pGameEvent->GetMonsterState()  & 0x01 )	//1 2 3 4
	{
		m_pMonster[0]->Set_Pos( m_pGameEvent->GetMonsPosition( 0 ) );
		m_pMonster[0]->Set_Angle( 0.0f );
		m_pMonster[0]->ChangeAnimation( CMonster::ANIM_STAND );
		m_pMonster[0]->EnableShadow( TRUE );
		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			m_pMonster[0]->GetFSM()->SetCurrentState( Seek::GetInstance() );
		}
	}
	if ( m_pGameEvent->GetMonsterState()  & 0x02 )
	{
		m_pMonster[1]->Set_Pos( m_pGameEvent->GetMonsPosition( 1 ) );
		m_pMonster[1]->Set_Angle( 0.0f );
		m_pMonster[1]->ChangeAnimation( CMonster::ANIM_STAND );
		m_pMonster[1]->EnableShadow( TRUE );
		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			m_pMonster[1]->GetFSM()->SetCurrentState( Seek::GetInstance() );
		}
	}
	if ( m_pGameEvent->GetMonsterState()  & 0x04 )
	{
		m_pMonster[2]->Set_Pos( m_pGameEvent->GetMonsPosition( 2 ) );
		m_pMonster[2]->Set_Angle( 0.0f );
		m_pMonster[2]->ChangeAnimation( CMonster::ANIM_STAND );
		m_pMonster[2]->EnableShadow( TRUE );
		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			m_pMonster[2]->GetFSM()->SetCurrentState( Seek::GetInstance() );
		}
	}
}

VOID CMainScene::EventSwitch( INT nEvent )
{
	switch ( nEvent )
	{
	case CGameEvent::INIT:
		CDebugConsole::GetInstance()->Message( "CGameEvent::INIT \n" );
		EventInit();
		break;
	case CGameEvent::EVENT_CAMERA:
		CDebugConsole::GetInstance()->Message( "CGameEvent::EVENT_CAMERA \n" );
		EventCamera();
		break;
	case CGameEvent::TUTORIAL_ATACK:
		CDebugConsole::GetInstance()->Message( "CGameEvent::TUTORIAL_ATACK \n" );
		// Do Something
		TutorialAtack();
		if ( CObjectManage::GetInstance()->IsHost() ) 
		{
			CGameEvent::GetInstance()->AddEvent( CGameEvent::TUTORIAL_COMBO, 10.0f );
			CNetwork::GetInstance()->CS_EVENT_STATE( nEvent ); 
		}
		break;
	case CGameEvent::TUTORIAL_COMBO:
		CDebugConsole::GetInstance()->Message( "CGameEvent::TUTORIAL_COMBO \n" );
		if ( CObjectManage::GetInstance()->IsHost() ) 
		{
			CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO, 1.0f );
			SetTimer( GHWND, CGameEvent::TUTORIAL_COMBO, 1 * 1000, NULL );
			CNetwork::GetInstance()->CS_EVENT_STATE( nEvent ); 
		}
		break;
	case CGameEvent::EVENT_COMBO:
		CDebugConsole::GetInstance()->Message( "CGameEvent::EVENT_COMBO \n" );
		EventCombo();
		if ( CObjectManage::GetInstance()->IsHost() ) 
		{		
			CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO_FAIL, 30.0f );
			CNetwork::GetInstance()->CS_EVENT_COMBO( m_pEventGUICombo->GetKindEvet() ); 
		}
		break;
	case CGameEvent::EVENT_COMBO_END:
		CDebugConsole::GetInstance()->Message( "CGameEvent::EVENT_COMBO_END \n" );
		EventDestoryCombo();
		if ( CObjectManage::GetInstance()->IsHost() ) 
		{
			// CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO, 30.0f );
			CNetwork::GetInstance()->CS_EVENT_STATE( nEvent ); 
		}
		break;
	case CGameEvent::EVENT_COMBO_SUCCESS:
		CDebugConsole::GetInstance()->Message( "CGameEvent::EVENT_COMBO_SUCCESS \n" );
		if ( m_pEventGUICombo )
		{
			m_pEventGUICombo->Success();
			if ( CObjectManage::GetInstance()->IsHost() ) 
			{
				CGameEvent::GetInstance()->AddEvent( CGameEvent::MONSTER_BREAK_NOCKDOWN, 2.0f );				
				// CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO_END, 5.0f );
				CNetwork::GetInstance()->CS_EVENT_STATE( nEvent ); 
			}
		}		
		break;
	case CGameEvent::EVENT_COMBO_FAIL:
		CDebugConsole::GetInstance()->Message( "CGameEvent::EVENT_COMBO_FAIL \n" );
		if ( m_pEventGUICombo )
			m_pEventGUICombo->Fail();
		if ( CObjectManage::GetInstance()->IsHost() ) 
		{
			// CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO_END, 5.0f );
			CNetwork::GetInstance()->CS_EVENT_STATE( nEvent ); 
		}
		break;
	case CGameEvent::EVENT_FAK:
		CDebugConsole::GetInstance()->Message( "CGameEvent::EVENT_FAK \n" );
		EventFirstAidKit();
		/// 미구현
		break;
	case CGameEvent::MONSTER_BREAK_NOCKDOWN:
		MonsterBreakNockdown();
		if ( CObjectManage::GetInstance()->IsHost() ) 
		{
			CNetwork::GetInstance()->CS_EVENT_STATE( nEvent ); 
			//CNetwork::GetInstance()->CS_EVENT_MON_ND()
		}
		break;
	default:
		break;
	}
}

VOID CMainScene::EventInit()
{
	// 케릭터 위치 조정
	InitCharState();

	// 몬스터 위치 조정
	InitMonsterState();

	// Game Point
}

VOID CMainScene::EventCamera()
{
	m_pCamera->SetEffect( CCamera::EVENTWORK );
}

VOID CMainScene::TutorialAtack()
{

}

VOID CMainScene::EventCombo()
{
	if ( !m_pEventGUICombo )
	{
		CObjectManage * pOM = CObjectManage::GetInstance();

		if ( pOM->IsHost() )
		{
			m_pEventGUICombo = new CGameEventCombo( m_pD3dDevice, pOM->GetSprite() );

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
				m_pEventGUICombo->AddCombo( i, pOM->Get_CharTable()[nSelect] + 1 );
			}		
		}
		else
		{
			m_pEventGUICombo = new CGameEventCombo( m_pD3dDevice, pOM->GetSprite() );

			for ( int i = 0; i < 4; ++i )
			{
				m_pEventGUICombo->AddCombo( i, pOM->Get_EventTable()[i] );
			}		
		}

		m_pEventGUICombo->Create();
	}
}

VOID CMainScene::EventDestoryCombo()
{
	SAFE_DELETE( m_pEventGUICombo );
}

VOID CMainScene::EventFirstAidKit()
{
	m_pFirstAidKit->SetActive( TRUE );
}