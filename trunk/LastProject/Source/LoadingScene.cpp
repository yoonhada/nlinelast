#include "stdafx.h"
#include "LoadingScene.h"
#include "LoadingGUI.h"
#include "Monster.h"

VOID LoadingScene::Initialize()
{
	m_scnNext		= SCENE_MENU;
	m_scnState		= SCENE_RUNNING;

	m_pLoadingGUI	= NULL;

	m_pLight		= NULL;
	m_pMatrices		= NULL;

	m_pClown		= NULL;
	m_pPanda		= NULL;
	m_pBrownBear	= NULL;
}

VOID LoadingScene::Release()
{
	m_pClown->Set_Pos( D3DXVECTOR3( 0, 0, 0 ) );
	m_pClown->Set_Angle( 0.0f );
//	m_pClown->Set_iSelectedFrameNum( 0 );
	m_pClown->ChangeAnimation( CMonster::ANIM_STAND );
	m_pClown->EnableShadow( TRUE );

	m_pPanda->Set_Pos( D3DXVECTOR3( 0, 0, 0 ) );
	m_pPanda->Set_Angle( 0.0f );
	m_pPanda->ChangeAnimation( CMonster::ANIM_STAND );
	m_pPanda->EnableShadow( TRUE );

	m_pBrownBear->Set_Pos( D3DXVECTOR3( 0, 0, 0 ) );
	m_pBrownBear->Set_Angle( 0.0f );
	m_pBrownBear->ChangeAnimation( CMonster::ANIM_STAND );
	m_pBrownBear->EnableShadow( TRUE );

	SAFE_DELETE( m_pLoadingGUI );

	SAFE_DELETE( m_pLight );

	//SAFE_DELETE( m_pClown );
	//SAFE_DELETE( m_pPanda );
}

HRESULT LoadingScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pD3dDevice = _pd3dDevice;

	m_pLoadingGUI		= new LoadingGUI( _pd3dDevice, _pSprite, _hWnd );
	m_pLoadingGUI->Create();

	//	Create Light
	m_pLight = new CLight;
	m_pLight->Create( _pd3dDevice );

	//	Create Matrices
	m_pMatrices = CMatrices::GetInstance();	

	// Create Monster
	m_pClown = CObjectManage::GetInstance()->Get_Clown();	// new CMonster;
	m_pClown->Create( _pd3dDevice, L"Data/CharData/27_pierro_body_11_28" );
	m_pClown->Set_Pos( D3DXVECTOR3( 19.0f, 0.0f, 81.0f ) );
	m_pClown->Set_Angle( 2.49f );
	m_pClown->ChangeAnimation( CMonster::ANIM_LODING );
	m_pClown->EnableShadow( FALSE );

	m_pPanda = CObjectManage::GetInstance()->Get_Panda();		// new CMonster;
	m_pPanda->Create( _pd3dDevice, L"Data/CharData/11_16_pa_sm_v6" );
	m_pPanda->Set_Pos( D3DXVECTOR3( -17.0f, 0.0f, 117.0f ) );
	m_pPanda->Set_Angle( 5.44f );
	m_pPanda->ChangeAnimation( CMonster::ANIM_LODING );
	m_pPanda->EnableShadow( FALSE );

	m_pBrownBear = CObjectManage::GetInstance()->Get_Bear();		// new CMonster;
	m_pBrownBear->Create( _pd3dDevice, L"Data/CharData/11_16_Brown_Bear_v6" );
	m_pBrownBear->Set_Pos( D3DXVECTOR3( -17.0f, 0.0f, 150.0f ) );
	m_pBrownBear->Set_Angle( 5.90f );
	m_pBrownBear->ChangeAnimation( CMonster::ANIM_LODING );
	m_pBrownBear->EnableShadow( FALSE );

	return S_OK;
}

VOID LoadingScene::Update()
{
	m_pLoadingGUI->Update();

	m_pClown->Update();
	//m_pClown->UpdateByValue( D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f );
	m_pPanda->Update();
	//m_pPanda->UpdateByValue( D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f );
	m_pBrownBear->Update();

	//static D3DXVECTOR3 vecClown( 19.0f, 0.0f, 81.0f ), vecPanda( -17.0f, 0.0f, 117.0f ), vecBrownBear( -17.0f, 0.0f, 150.0f );
	//static FLOAT		fClown = 2.49f, fPanda = 5.44f, fBrownBear = 5.9f;
	//
	//CDebugConsole::GetInstance()->Messagef( L"Clown : %f, %f, %f, %f\n", vecClown.x, vecClown.y, vecClown.z, fClown );
	//CDebugConsole::GetInstance()->Messagef( L"Panda : %f, %f, %f, %f\n", vecPanda.x, vecPanda.y, vecPanda.z, fPanda );
	//CDebugConsole::GetInstance()->Messagef( L"BrownBear : %f, %f, %f, %f\n", vecBrownBear.x, vecBrownBear.y, vecBrownBear.z, fBrownBear );

	//if( GetKeyState( 'A' ) & 0x8000 )
	//	vecClown.x += 1.0f;
	//if( GetKeyState( 'D' ) & 0x8000 )
	//	vecClown.x -= 1.0f;
	//if( GetKeyState( 'W' ) & 0x8000 )
	//	vecClown.z += 1.0f;
	//if( GetKeyState( 'S' ) & 0x8000 )
	//	vecClown.z -= 1.0f;
	//if( GetKeyState( 'Q' ) & 0x8000 )
	//	fClown += 0.05f;
	//if( GetKeyState( 'E' ) & 0x8000 )
	//	fClown -= 0.05f;

	//if( GetKeyState( 'H' ) & 0x8000 )
	//	vecPanda.x += 1.0f;
	//if( GetKeyState( 'F' ) & 0x8000 )
	//	vecPanda.x -= 1.0f;
	//if( GetKeyState( 'T' ) & 0x8000 )
	//	vecPanda.z += 1.0f;
	//if( GetKeyState( 'G' ) & 0x8000 )
	//	vecPanda.z -= 1.0f;
	//if( GetKeyState( 'R' ) & 0x8000 )
	//	fPanda += 0.05f;
	//if( GetKeyState( 'Y' ) & 0x8000 )
	//	fPanda -= 0.05f;

	//if( GetKeyState( 'L' ) & 0x8000 )
	//	vecBrownBear.x += 1.0f;
	//if( GetKeyState( 'J' ) & 0x8000 )
	//	vecBrownBear.x -= 1.0f;
	//if( GetKeyState( 'I' ) & 0x8000 )
	//	vecBrownBear.z += 1.0f;
	//if( GetKeyState( 'K' ) & 0x8000 )
	//	vecBrownBear.z -= 1.0f;
	//if( GetKeyState( 'O' ) & 0x8000 )
	//	fBrownBear += 0.05f;
	//if( GetKeyState( 'U' ) & 0x8000 )
	//	fBrownBear -= 0.05f;

	//m_pClown->Set_Pos( vecClown );
	//m_pPanda->Set_Pos( vecPanda );
	//m_pBrownBear->Set_Pos( vecBrownBear );

	//m_pClown->Set_Angle( fClown );
	//m_pPanda->Set_Angle( fPanda );
	//m_pBrownBear->Set_Angle( fBrownBear );
}

VOID LoadingScene::Render()
{
	static FLOAT fX = 10.0f, fY = 55.0f, fZ = 224.0f;

	/*if( GetKeyState( 'Z' ) & 0x8000 )
		fX += 1.0f;
	if( GetKeyState( 'X' ) & 0x8000 )
		fX -= 1.0f;
	if( GetKeyState( 'C' ) & 0x8000 )
		fY += 1.0f;
	if( GetKeyState( 'V' ) & 0x8000 )
		fY -= 1.0f;
	if( GetKeyState( 'B' ) & 0x8000 )
		fZ += 1.0f;
	if( GetKeyState( 'N' ) & 0x8000 )
		fZ -= 1.0f;*/
	
	D3DXVECTOR3		vecEyePt( fX, fY, fZ );
	D3DXVECTOR3		vecLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3		vecUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16	matView;
	D3DXMatrixLookAtLH( &matView, &vecEyePt, &vecLookatPt, &vecUpVec );
	m_pD3dDevice->SetTransform( D3DTS_VIEW, &matView );
	
	m_pMatrices->SetupProjection();
	m_pLight->EnableCharacterLight( D3DXVECTOR3( 0.0f, -1.0f, -150.0f ) );

	//	Render Monster
	m_pClown->Render();
	m_pPanda->Render();
	m_pBrownBear->Render();

	m_pLight->DisableLight();
	
	//	Render GUI
	m_pLoadingGUI->Render();
}

INT LoadingScene::GetSceneNext()
{
	return m_scnNext;
}

INT LoadingScene::GetSceneState()
{
	return m_scnState;
}