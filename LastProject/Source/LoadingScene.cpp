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
	m_pPanda->Create( _pd3dDevice, L"Data/CharData/11_16_pa_sm_v8" );
	m_pPanda->Set_Pos( D3DXVECTOR3( -17.0f, 0.0f, 117.0f ) );
	m_pPanda->Set_Angle( 5.44f );
	m_pPanda->ChangeAnimation( CMonster::ANIM_LODING );
	m_pPanda->EnableShadow( FALSE );
	return S_OK;
}

VOID LoadingScene::Update()
{
	m_pLoadingGUI->Update();

	m_pClown->Update();
	//m_pClown->UpdateByValue( D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f );
	m_pPanda->Update();
	//m_pPanda->UpdateByValue( D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f );

	/*static D3DXVECTOR3 vecClown( 19.0f, 0.0f, 81.0f ), vecPanda( -17.0f, 0.0f, 117.0f );
	static FLOAT		fClown = 2.49f, fPanda = 5.44f;
	
	CDebugConsole::GetInstance()->Messagef( L"Clown : %f, %f, %f, %f\n", vecClown.x, vecClown.y, vecClown.z, fClown );
	CDebugConsole::GetInstance()->Messagef( L"Panda : %f, %f, %f, %f\n", vecPanda.x, vecPanda.y, vecPanda.z, fPanda );

	if( GetKeyState( '1' ) & 0x8000 )
		vecClown.x += 1.0f;
	if( GetKeyState( '2' ) & 0x8000 )
		vecClown.x -= 1.0f;
	if( GetKeyState( '3' ) & 0x8000 )
		vecClown.y += 1.0f;
	if( GetKeyState( '4' ) & 0x8000 )
		vecClown.y -= 1.0f;
	if( GetKeyState( '5' ) & 0x8000 )
		vecClown.z += 1.0f;
	if( GetKeyState( '6' ) & 0x8000 )
		vecClown.z -= 1.0f;
	if( GetKeyState( '7' ) & 0x8000 )
		fClown += 0.05f;
	if( GetKeyState( '8' ) & 0x8000 )
		fClown -= 0.05f;
	

	if( GetKeyState( 'Q' ) & 0x8000 )
		vecPanda.x += 1.0f;
	if( GetKeyState( 'W' ) & 0x8000 )
		vecPanda.x -= 1.0f;
	if( GetKeyState( 'E' ) & 0x8000 )
		vecPanda.y += 1.0f;
	if( GetKeyState( 'R' ) & 0x8000 )
		vecPanda.y -= 1.0f;
	if( GetKeyState( 'T' ) & 0x8000 )
		vecPanda.z += 1.0f;
	if( GetKeyState( 'Y' ) & 0x8000 )
		vecPanda.z -= 1.0f;
	if( GetKeyState( 'U' ) & 0x8000 )
		fPanda += 0.05f;
	if( GetKeyState( 'I' ) & 0x8000 )
		fPanda -= 0.05f;

	m_pClown->Set_Pos( vecClown );
	m_pPanda->Set_Pos( vecPanda );

	m_pClown->Set_Angle( fClown );
	m_pPanda->Set_Angle( fPanda );*/
}

VOID LoadingScene::Render()
{
	static FLOAT fX = 10.0f, fY = 48.0f, fZ = 224.0f;

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
	m_pLight->EnableLight( D3DXVECTOR3( 0.0f, -1.0f, -150.0f ) );

	//	Render Monster
	m_pClown->Render();
	m_pPanda->Render();

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