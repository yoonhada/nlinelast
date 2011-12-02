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

// AI �׽�Ʈ��
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
	//m_pMyCharactors = NULL;
	//m_pOtherCharactors = NULL;
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

#ifdef _DEBUG
	FLOAT fSize = 1000.0f;
	INT nDeep = 4;
	m_pAxis = new Axis( m_pD3dDevice );
	m_pAxis->Create( fSize, nDeep );
#endif

	//ī�޶� ����
	m_pCamera = new CCamera;
	m_pCamera->Create( m_pD3dDevice );

	//�̺�Ʈ
	INT nMaxCharaNum = CObjectManage::GetInstance()->Get_MaxCharaNum();
	m_pGameEvent = new CGameEvent( nMaxCharaNum );
	m_pGameEvent->Create();

	//ĳ���� ����
	m_nClientID = CObjectManage::GetInstance()->Get_ClientNumber();
	CreateCharactor();

	//���� ����
	m_pMonster = CObjectManage::GetInstance()->Get_Monster();
	m_pMonster->Create( m_pD3dDevice, L"Data/CharData/27_pierro_body_11_28" );

	// ������ ����
	m_pFirstAidKit = CObjectManage::GetInstance()->Get_FirstAidKit();
	m_pFirstAidKit->Create( m_pD3dDevice );
	m_pFirstAidKit->Load( L"Data/CharData/FirstAidKit_1.csav" );

	//���� ����
	m_pLight = new CLight;
	m_pLight->Create( m_pD3dDevice );

	//�� ����
	// Error ���ܻ��� 
	m_pASEViewer = CObjectManage::GetInstance()->Get_ASEViewer();
	m_pASEViewer->Create(  L"ASE File/Map/Stage_Beta.ASE", L"ASE File/Map/Stage_Beta_Box.BBX" );

	//Ÿ�ϸ� ����
	m_pTileMap = new TileMap( m_pD3dDevice );
	m_pTileMap->Create( D3DXVECTOR3( -510.0f, 0.0f, -950.0f ), D3DXVECTOR3( 510.0f, 0.0f, 950.0f ), 10.0f );
	m_pTileMap->LoadBBXFile( L"ASE File/Map/Stage_Beta_Box.BBX" );


	Seek::GetInstance()->Initialize( m_pTileMap );
	Chase::GetInstance()->Initialize( m_pTileMap->GetInfo() );

	Astar::GetInstance()->Initialize( m_pTileMap->GetInfo() );

	// �������� ����
	m_pMatrices->SetupProjection();

	// GUI ����
	m_pMainGUI = new MainGUI( m_pD3dDevice, a_Sprite, a_hWnd );
	m_pMainGUI->Create();
	m_pOptionScene = new OptionScene;
	m_pOptionScene->Create( m_pD3dDevice, a_Sprite, a_hWnd );

	CInput::GetInstance()->EnableInput(FALSE);

	////m_pStateMachine->SetCurrentState( Seek::GetInstance() );
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
	//ĳ���� ����
	m_pCharactors = CObjectManage::GetInstance()->Get_Charactors();

	for(INT Loop = 0; Loop < m_iMaxCharaNum; ++Loop )
	{
		if( !m_pCharactors[Loop] )
			continue;

		//m_pCharactors[Loop]->Create( m_pD3dDevice );
		//m_pCharactors[Loop]->LoadKindChar( Loop );

		if (Loop == m_nClientID)
		{
			m_pCharactors[m_nClientID]->Set_Active( TRUE );
		}

		m_pCharactors[m_nClientID]->Set_Position( m_pGameEvent->GetDefaultCharPosition( Loop ) );
		CTree::GetInstance()->GetCharVector()->push_back( m_pCharactors[Loop]->GetBoundBox() );
	}
}

VOID CMainScene::Update()
{
	// ĳ����: ��ǲ �� �޾ƿ���
	m_pCharactors[ m_nClientID ]->UpdateByInput();
	if ( m_pCharactors[ m_nClientID ]->CollisionAtk( ) )
	{
		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity( &mat );
		m_pCharactors[ m_nClientID ]->BreakQube( mat );

		CObjectManage::GetInstance()->Send_NetworkSendDestroyData( TRUE );
	}
	CTree::GetInstance()->SetMonsAtkClear();

	// ī�޶�: ĳ���� ��ġ,���� �޾ƿ���
	m_pCamera->SetView( 
		m_pCharactors[m_nClientID]->Get_CharaPos2Camera(), 
		m_pCharactors[m_nClientID]->Get_PreCharaPos2Camera(), 
		10.0f, 75.0f, 
		m_pCharactors[m_nClientID]->Get_CharaAngle(),
		CInput::GetInstance()->Get_MouseXRotate() );

	m_pCamera->CheckObjectCollision( 
		m_pCamera->GetEye(), 
		m_pCharactors[m_nClientID]->Get_CharaPos(), 
		m_pCharactors[m_nClientID]->Get_CharaAngle() );

	// �ٸ� �÷��̾�� ������ �̵�
	for( INT Loop = 0; Loop < m_iMaxCharaNum; ++Loop )
	{
		if ( Loop != m_nClientID)
		{
			m_pCharactors[Loop]->UpdateOtherPlayer();
		}
	}

	m_pMonster->Update();
	m_pMonster->UpdateByValue( D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f );

	// ������ ���??
	m_pFirstAidKit->Update();

	//m_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );	
	m_pASEViewer->Update();

	m_pMainGUI->Update();
	m_pOptionScene->Update();

	CNetwork::GetInstance()->UpdateGame();
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