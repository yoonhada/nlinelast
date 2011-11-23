#include "Stdafx.h"
#include "MainScene.h"

#include "Monster.h"
#include "BillBoard.h"
#include "Charactor.h"
#include "Camera.h"
#include "Axis.h"
#include "Map.h"
#include "Weapon.h"
#include "TileMap.h"
#include "Seek.h"
#include "TimeLifeItem.h"

// AI 테스트용
#ifdef _DEBUG
#include "Seek.h"
#endif


CMainScene::CMainScene()
{
	Clear();
}

CMainScene::~CMainScene()
{
	Release();
}

VOID	CMainScene::Clear()
{
	m_pMatrices = NULL;
	m_pCamera = NULL;
	m_pD3dDevice = NULL;
	m_pBill = NULL;
	m_pMyCharactor = NULL;
	m_pCharactors = NULL;
	m_pAxis = NULL;
	m_pGrid = NULL;
	m_pMonster = NULL;
	m_pMap = NULL;

	for( INT i=0; i<4; ++i )
	{
		m_pCharactorList[i] = NULL;
	}

	m_iMaxCharaNum = CObjectManage::GetInstance()->Get_MaxCharaNum();
	//m_bHost = FALSE;
	//m_iClientNumber = 0;

	m_pLogo = NULL;
	m_pTileMap = NULL;
}

HRESULT CMainScene::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
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
	m_pMonster->Create( m_pD3dDevice, L"Data/CharData/11_16_pa_sm_v6" );

	// 아이템 생성
	m_pFirstAidKit = CObjectManage::GetInstance()->Get_FirstAidKit();
	m_pFirstAidKit->Create( m_pD3dDevice );
	m_pFirstAidKit->Load( L"Data/CharData/FirstAidKit_1.csav" );

	//조명 생성
	m_pLight = new CLight;
	m_pLight->Create( m_pD3dDevice );

	//맵 생성
	m_pMap = new Map( m_pD3dDevice );
	m_pMap->Create( L"ASE File/Stage4_Alpha.ASE", L"ASE File/Stage4_Alpha.BBX" );
	m_pMap->AddAnimationData( 1000, 0, 0, 300, TRUE );

	//타일맵 생성
	m_pTileMap = new TileMap( m_pD3dDevice );
	m_pTileMap->Create( L"ASE File/Stage4_Alpha.BBX", D3DXVECTOR3(-510.0f, 0.0f, -510.0f), D3DXVECTOR3(510.0f, 0.0f, 510.0f), 10.0f );

	Seek::GetInstance()->Initialize( m_pTileMap );

	Astar::GetInstance()->Initialize( m_pTileMap->GetMapInfo()->iMaxWidth,
									  m_pTileMap->GetMapInfo()->iMaxHeight,
									  m_pTileMap->GetMapInfo()->pNavGraphNode );

	// 프로젝션 설정
	m_pMatrices->SetupProjection();


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

	return S_OK;
}

HRESULT CMainScene::Release()
{
	SAFE_DELETE ( m_pCamera );

	SAFE_DELETE ( m_pGrid );

	//SAFE_DELETE( m_pMyCharactor );

	//SAFE_DELETE_ARRAY( m_pCharactors );

	//SAFE_DELETE( m_pMonster );

	SAFE_DELETE ( m_pMap );

	SAFE_DELETE ( m_pLight );

	SAFE_DELETE( m_pBill );

	SAFE_DELETE( m_pAxis );


	SAFE_DELETE_ARRAY( m_pLogo );

	SAFE_DELETE( m_pTileMap );

	return S_OK;
}

VOID CMainScene::CreateCharactor()
{
	const INT nChar = 5;
	D3DXVECTOR3 vec[nChar] = { 
		D3DXVECTOR3( -130.0f, 0.0f,100.0f ), 
		D3DXVECTOR3( -160.0f, 0.0f,100.0f ), 
		D3DXVECTOR3( -190.0f, 0.0f,100.0f ), 
		D3DXVECTOR3( -210.0f, 0.0f,100.0f ), 
		D3DXVECTOR3(  0.0f, 0.0f, 100.0f ) 
	};

	//캐릭터 생성
	FLOAT fYawZero = 1.0f;
	m_pMyCharactor = CObjectManage::GetInstance()->Get_MyCharactor();
	m_pMyCharactor->Create( m_pD3dDevice );
	m_pMyCharactor->Set_Active( TRUE );
	m_pMyCharactor->Load( L"Data/CharData/APPA_0.csav" );
	m_pMyCharactor->CreateWeapon( CWeapon::SPANNER );

	CTree::GetInstance()->GetCharVector()->push_back( m_pMyCharactor->GetBoundBox() );

	m_pCharactors = CObjectManage::GetInstance()->Get_Charactors();
	
	for(INT Loop=0; Loop<m_iMaxCharaNum; ++Loop )
	{
		m_pCharactors[Loop].Create( m_pD3dDevice );
		if(Loop == 0)
		{
			m_pCharactors[Loop].Load( L"Data/CharData/MOM.csav" );
		}
		else if(Loop == 1)
		{
			m_pCharactors[Loop].Load( L"Data/CharData/ADDLE_0.csav" );
		}
		else
		{
			m_pCharactors[Loop].Load( L"Data/CharData/DDAL_0.csav" );
		}

		m_pCharactors[Loop].CreateWeapon( CWeapon::SPANNER /* + Loop + 1*/ );
		m_pCharactors[Loop].Set_Position( vec[Loop] );
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
	m_pMyCharactor->UpdateByInput();
	m_pMyCharactor->Update();
	if ( m_pMyCharactor->CollisionAtk( ) )
	{
		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity( &mat );
		m_pMyCharactor->BreakQube( mat );
	}

	//카메라: 캐릭터 위치,각도 받아오기
	m_pCamera->SetView( 
		m_pMyCharactor->Get_CharaPos2Camera(), 
		m_pMyCharactor->Get_PreCharaPos2Camera(), 
		10.0f, 75.0f, 
		m_pMyCharactor->Get_CharaAngle(),
		CInput::GetInstance()->Get_MouseXRotate() );

	m_pCamera->CheckObjectCollision( m_pCamera->GetEye(), m_pMyCharactor->Get_CharaPos(), m_pMyCharactor->Get_CharaAngle() );

	FLOAT fYawZero = 1.0f;

	for( INT Loop=0; Loop<m_iMaxCharaNum; ++Loop )
	{	
		//다른 플레이어는 값으로 이동
		//if( m_pCharactors[Loop].Get_Active() )
		//{
		//CDebugConsole::GetInstance()->Messagef( L"CHECK\n" );
		m_pCharactors[Loop].UpdateOtherPlayer();
		//m_pCharactors[Loop].Update();
		//}
	}

	std::vector<CBoundBox*> * vecBoundBox;
	std::vector<CBoundBox*>::iterator Iter;
	vecBoundBox = CTree::GetInstance()->GetMonsAtkVector();
	if ( !( vecBoundBox == NULL || vecBoundBox->empty() ) )
	{			
		Iter = vecBoundBox->begin();
		while ( Iter != vecBoundBox->end() )
		{
			delete ( *Iter );
			Iter++;
		}
	}
	
	vecBoundBox->erase( vecBoundBox->begin(), vecBoundBox->end() );

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
	static FLOAT fMonsterRun = 0.0f;
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

	m_pMap->Update();
/*
	m_pTileMap->SetInfo( 0, 0, TileMap::TLM_COURSE );
	m_pTileMap->SetInfo( 1, 0, TileMap::TLM_COURSE );
	m_pTileMap->SetInfo( 2, 0, TileMap::TLM_COURSE );
	m_pTileMap->SetInfo( 3, 0, TileMap::TLM_COURSE );
	m_pTileMap->SetInfo( 4, 0, TileMap::TLM_COURSE );
	m_pTileMap->SetInfo( 5, 0, TileMap::TLM_COURSE );
*/

	CNetwork::GetInstance()->Update();
}

VOID	CMainScene::Render()
{
	m_pLight->EnableLight();

#ifdef _DEBUG
	m_pAxis->Render();
#endif

	m_pD3dDevice->SetTransform( D3DTS_WORLD, &m_pMap->Get_MatWorld() );
	m_pMap->Render();

#ifdef _DEBUG
	m_pTileMap->Render();
#endif

	m_pMyCharactor->Render();

	for( INT Loop=0; Loop<m_iMaxCharaNum; ++Loop )
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
	TwDraw();
#endif

}