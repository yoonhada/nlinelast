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
	m_pBill			= NULL;
	//m_pMyCharactor	= NULL;
	m_pCharactors	= NULL;
	m_pAxis			= NULL;
	m_pGrid			= NULL;
	m_pMonster		= NULL;
	m_pTileMap		= NULL;
	m_pASEViewer	= NULL;
	m_pGameEvent	= NULL;
	
	m_pMainGUI		= NULL;
	m_pOptionScene	= NULL;

	for( INT i=0; i<4; ++i )
	{
		m_pCharactorList[i] = NULL;
	}

	m_iMaxCharaNum = CObjectManage::GetInstance()->Get_MaxCharaNum();

	//m_bHost = FALSE;
	//m_iClientNumber = 0;

	m_pLogo		= NULL;
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

	//그리드 생성
	m_pGrid = new CGrid;
	m_pGrid->Create( m_pD3dDevice );

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

	//로고
	/*m_pLogo = new CCharactor[6];
	D3DXVECTOR3 vec[6] = { 
		D3DXVECTOR3( -130.0f, 0.0f, 200.0f ), 
		D3DXVECTOR3( -160.0f, 0.0f, 200.0f ), 
		D3DXVECTOR3( -190.0f, 0.0f, 200.0f ), 
		D3DXVECTOR3( -220.0f, 0.0f, 200.0f ), 
		D3DXVECTOR3( -250.0f, 0.0f, 200.0f ), 
		D3DXVECTOR3( -280.0f, 0.0f, 200.0f ) 
	};
	for(INT Loop=0; Loop<6; ++Loop )
	{
		m_pLogo[Loop].Create( m_pD3dDevice, m_pMatrices );
		if(Loop == 0)
		{
			m_pLogo[Loop].Load( L"Data/Logo/N.csav" );
		}
		else if(Loop == 1)
		{
			m_pLogo[Loop].Load( L"Data/Logo/-.csav" );
		}
		else if(Loop == 2)
		{
			m_pLogo[Loop].Load( L"Data/Logo/L.csav" );
		}
		else if(Loop == 3)
		{
			m_pLogo[Loop].Load( L"Data/Logo/I.csav" );
		}
		else if(Loop == 4)
		{
			m_pLogo[Loop].Load( L"Data/Logo/N.csav" );
		}
		else if(Loop == 5)
		{
			m_pLogo[Loop].Load( L"Data/Logo/E.csav" );
		}

		m_pLogo[Loop].Set_Position( vec[Loop] );
	}*/

	CDebugConsole::GetInstance()->Messagef( L"**** Create End **** \n\n" );

	//CObjectManage::GetInstance()->Set_MyCharactor( m_pMyCharactor );
	//CObjectManage::GetInstance()->Set_Charactors( m_pCharactors );
	//CObjectManage::GetInstance()->Set_CharactorList( m_pCharactorList );
	//CObjectManage::GetInstance()->Set_pAlphaMon( m_pAlphaMon );

	CInput::GetInstance()->EnableInput(FALSE);

	return S_OK;
}

HRESULT CMainScene::Release()
{
	SAFE_DELETE ( m_pCamera );

	SAFE_DELETE ( m_pGrid );

	//SAFE_DELETE( m_pMyCharactor );

	//SAFE_DELETE_ARRAY( m_pCharactors );

	//SAFE_DELETE( m_pMonster );

	//SAFE_DELETE ( m_pASEViewer );

	SAFE_DELETE ( m_pLight );

	SAFE_DELETE( m_pBill );

	SAFE_DELETE( m_pAxis );

	SAFE_DELETE_ARRAY( m_pLogo );

	SAFE_DELETE( m_pMainGUI );
	SAFE_DELETE( m_pOptionScene );


	return S_OK;
}

VOID CMainScene::CreateCharactor()
{
	//캐릭터 생성
	FLOAT fYawZero = 1.0f;
	m_pCharactors = CObjectManage::GetInstance()->Get_Charactors();
	//m_pMyCharactor = CObjectManage::GetInstance()->Get_Charactors();
	//m_pCharactors[0].LoadKindChar( 0 );
	m_pCharactors[0].Set_Active( TRUE );
	//m_pMyCharactor->Create( m_pD3dDevice );
	//m_pMyCharactor->LoadKindChar( 3 );
	//m_pMyCharactor->Set_Active( TRUE );
	CTree::GetInstance()->GetCharVector()->push_back( m_pCharactors[0].GetBoundBox() );

	for(INT Loop = 1; Loop < m_iMaxCharaNum; ++Loop )
	{
		//m_pCharactors[Loop].Create( m_pD3dDevice );
		m_pCharactors[Loop].LoadKindChar( Loop );
		m_pCharactors[Loop].Set_Position( D3DXVECTOR3(0, 0, 0) );
		CTree::GetInstance()->GetCharVector()->push_back( m_pCharactors[Loop].GetBoundBox() );

		//D3DXVECTOR3 vec1 = m_pCharactors[Loop].GetBoundBox()->GetPosition();
	}

	// 인풋 업데이트
	//FLOAT fMoveSpeed = 50.0f;
	//FLOAT fRotateSpeed = 150.0f; 
	//CInput::GetInstance()->Update( fMoveSpeed, fRotateSpeed, CFrequency::GetInstance()->getFrametime() );
}

VOID	CMainScene::Update()
{
	
	// 맵 로드
	//if( CInput::GetInstance()->Get_F9button() == TRUE )
	//{
	//	OPENFILENAME OFN;
	//	WCHAR lpstrFile[MAX_PATH]=L"";

	//	memset(&OFN, 0, sizeof(OPENFILENAME));
	//	OFN.lStructSize = sizeof(OPENFILENAME);
	//	OFN.hwndOwner=CWinBase::GetInstance()->Get_hWnd();
	//	OFN.lpstrFilter=TEXT("ASE 파일(*.ASE)\0*.ASE\0");
	//	OFN.lpstrFile=lpstrFile;
	//	OFN.nMaxFile=MAX_PATH;
	//	if (GetSaveFileName(&OFN)!=0)
	//	{
	//		WCHAR* ptr = wcstok( lpstrFile, L"." );
	//		WCHAR Temp[255];
	//		WCHAR Temp2[255];
	//		wsprintf( Temp, L"%s.ASE", ptr );
	//		wsprintf( Temp2, L"%s.BBX", ptr );
	//		//m_pMap->Release();
	//		m_pMap->Create( Temp, Temp2 );
	//		//m_CharEdit.Load( OFN.lpstrFile );
	//	}
	//}

	// 캐릭터: 인풋 값 받아오기
	m_pCharactors[0].UpdateByInput();
	m_pCharactors[0].Update();
	if ( m_pCharactors[0].CollisionAtk( ) )
	{
		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity( &mat );
		m_pCharactors[0].BreakQube( mat );
	}

	//카메라: 캐릭터 위치,각도 받아오기
	m_pCamera->SetView( 
		m_pCharactors[0].Get_CharaPos2Camera(), 
		m_pCharactors[0].Get_PreCharaPos2Camera(), 
		10.0f, 75.0f, 
		m_pCharactors[0].Get_CharaAngle(),
		CInput::GetInstance()->Get_MouseXRotate() );

	m_pCamera->CheckObjectCollision( m_pCamera->GetEye(), m_pCharactors[0].Get_CharaPos(), m_pCharactors[0].Get_CharaAngle() );

	FLOAT fYawZero = 1.0f;

	for( INT Loop = 1; Loop < m_iMaxCharaNum; ++Loop )
	{	
		//다른 플레이어는 값으로 이동
		//if( m_pCharactors[Loop].Get_Active() )
		//{
		//CDebugConsole::GetInstance()->Messagef( L"CHECK\n" );
		m_pCharactors[Loop].UpdateOtherPlayer();
		//m_pCharactors[Loop].Update();
		//}
	}

	////로고
	//for( INT Loop=0; Loop<6; ++Loop )
	//{
	//	m_pLogo[Loop].UpdateOtherPlayer();
	//	if( CInput::GetInstance()->Get_Lbutton() )
	//	{
	//		//m_pLogo[Loop].BreakCubeAll();
	//	}
	//}

	//static FLOAT TimeElapsed = 0.0f;
	static FLOAT fMonsterRun = 0.0f;\
	static FLOAT fMonsterAngle = 0.0f;
	//TimeElapsed += CFrequency::GetInstance()->getFrametime();
	//if( TimeElapsed >= 0.1f )
	//{
		fMonsterRun += 1.0f * CFrequency::GetInstance()->getFrametime();
		fMonsterAngle += 0.3f * CFrequency::GetInstance()->getFrametime();
		//TimeElapsed = 0.0f;
	//}

	m_pMonster->Update();
	m_pMonster->UpdateByValue( D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f );

	// 아이템 생성
	if (CInput::GetInstance()->Get_NumKey(3))
	{
		m_pFirstAidKit->SetActive(TRUE);
	}
	if (CInput::GetInstance()->Get_NumKey(4))
	{
		m_pFirstAidKit->SetActive(FALSE);
	}

	m_pFirstAidKit->Update();

	//if( CInput::GetInstance()->Get_Rbutton() )
	//{
	//	//m_pMonster->Set_iSelectedFrameNum( 1 );
	//}

	//m_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );	

	m_pASEViewer->Update();

	m_pMainGUI->Update();
	m_pOptionScene->Update();

	CNetwork::GetInstance()->Update();
	m_pGameEvent->Update();	CTree::GetInstance()->SetMonsAtkClear();
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

	m_pCharactors[0].Render();

	for( INT Loop = 1; Loop < m_iMaxCharaNum; ++Loop )
	{
		if( m_pCharactors[Loop].Get_Active() )
		{
			m_pCharactors[Loop].Render();
		}
	}

	//로고
	/*for( INT Loop=0; Loop<6; ++Loop )
	{
		m_pLogo[Loop].Render();
	}*/

	m_pMonster->Render();

	m_pFirstAidKit->Render();

#ifdef _DEBUG
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	m_pD3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pTileMap->Render();
	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
#endif


#ifdef _DEBUG
	TwDraw();
#endif

	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	D3DXMATRIX matView;
	D3DXMatrixIdentity( &matView );
	m_pD3dDevice->SetTransform( D3DTS_VIEW, &matView );
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