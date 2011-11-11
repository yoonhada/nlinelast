#include "Stdafx.h"
#include "LoadScene.h"

#include "Monster.h"
#include "BillBoard.h"
#include "Charactor.h"
#include "Camera.h"
#include "Axis.h"
#include "Map.h"
#include "Weapon.h"
#include "TileMap.h"

#include "MainScene.h"

CLoadScene::CLoadScene()
{
	Clear();
}
CLoadScene::~CLoadScene()
{
	Release();
}

VOID CLoadScene::Clear()
{
	m_pMatrices = NULL;
	m_pCamera = NULL;
	m_pD3dDevice = NULL;
	m_pBill = NULL;
	m_pMyCharactor = NULL;
	m_pCharactors = NULL;
	m_pAlphaMon = NULL;
	m_pAxis = NULL;
	m_pGrid = NULL;
	m_pMonster = NULL;
	m_pMap = NULL;

	for( INT i=0; i<4; ++i )
	{
		m_pCharactorList[i] = NULL;
	}

	m_iMaxCharaNum = 3;
	m_bHost = FALSE;
	m_iClientNumber = 0;

	m_pLogo = NULL;
	m_pTileMap = NULL;
}
HRESULT CLoadScene::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	m_pD3dDevice = a_pD3dDevice;

	m_pMatrices = CMatrices::GetInstance();

	//카메라 생성
	m_pCamera = new CCamera;
	m_pCamera->Create( m_pD3dDevice );

	//캐릭터 생성
	m_pMyCharactor = new CCharactor;
	m_pMyCharactor->Create( m_pD3dDevice, m_pMatrices );
	m_pMyCharactor->Set_Active( TRUE );
	m_pMyCharactor->Load( L"Data/CharData/DDAL_0.csav" );
	m_pMyCharactor->CreateWeapon( CWeapon::SPANNER );

	//조명 생성
	m_pLight = new CLight;
	m_pLight->Create( m_pD3dDevice );

	m_pMatrices->SetupProjection();


	return S_OK;
}
HRESULT CLoadScene::Release()
{
	SAFE_DELETE ( m_pCamera );

	SAFE_DELETE ( m_pGrid );

	SAFE_DELETE( m_pMyCharactor );

	SAFE_DELETE_ARRAY( m_pCharactors );

	SAFE_DELETE( m_pMonster );

	SAFE_DELETE ( m_pMap );

	SAFE_DELETE ( m_pLight );

	SAFE_DELETE( m_pBill );

	SAFE_DELETE( m_pAxis );

	SAFE_DELETE( m_pAlphaMon );

	SAFE_DELETE_ARRAY( m_pLogo );

	SAFE_DELETE( m_pTileMap );

	return S_OK;
}
VOID CLoadScene::Update()
{
	//인풋 업데이트
	CInput::GetInstance()->Update( 50.0f, 150.0f, CFrequency::GetInstance()->getFrametime() );

	// 캐릭터: 인풋 값 받아오기
	m_pMyCharactor->UpdateByValue( D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f );
	m_pMyCharactor->UpdateOtherPlayer();
	//m_pMyCharactor->Update();

	m_pCamera->SetView( 
		m_pMyCharactor->Get_CharaPos2Camera(), 
		m_pMyCharactor->Get_CharaPos2Camera(), 
		10.0f, 75.0f, 
		m_pMyCharactor->Get_CharaAngle(),
		CInput::GetInstance()->Get_MouseXRotate() );

	static BOOL bTemp = FALSE;
	if( bTemp == FALSE )
	{
		CMainScene* pMain = new CMainScene;
		CSceneManage::GetInstance()->OrderChangeScene( pMain );
		bTemp = TRUE;
	}
}

VOID CLoadScene::Render()
{
	m_pLight->EnableLight();
	m_pMyCharactor->Render();
}