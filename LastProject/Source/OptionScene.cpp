#include "Stdafx.h"
#include "OptionScene.h"
#include "OptionGUI.h"
#include "HelpScene.h"
#include "SetupScene.h"

VOID OptionScene::Initialize()
{
	m_scnNext		= SCENE_MENU;
	m_scnState		= SCENE_RUNNING;

	m_pOptionGUI	= NULL; 
	m_pHelpScene	= NULL;
	m_pSetupScene	= NULL;

	m_bHelpScene	= FALSE;
	m_bSetupScene	= FALSE;
}

VOID OptionScene::Release()
{
	SAFE_DELETE( m_pOptionGUI );
	SAFE_DELETE( m_pHelpScene );
	SAFE_DELETE( m_pSetupScene );
}

HRESULT OptionScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pOptionGUI	= new OptionGUI( _pd3dDevice, _pSprite, _hWnd );
	m_pHelpScene	= new HelpScene;
	m_pSetupScene	= new SetupScene;

	m_pOptionGUI->Create();
	m_pHelpScene->Create( _pd3dDevice, _pSprite, _hWnd );
	m_pSetupScene->Create( _pd3dDevice, _pSprite, _hWnd );

	return S_OK;
}

VOID OptionScene::Update()
{
	m_pOptionGUI->Update();

	if( m_bHelpScene )
		m_pHelpScene->Update();
	if( m_bSetupScene )
		m_pSetupScene->Update();
	
	DWORD dID;
	m_pOptionGUI->Command( dID );
	switch( dID )
	{
	case OPTION_SETUP:
		m_bSetupScene	= !m_bSetupScene;
		m_bHelpScene	= FALSE;
		break;
	case OPTION_HELP:
		m_bHelpScene	= !m_bHelpScene;
		m_bSetupScene	= FALSE;
		break;
	}
}

VOID OptionScene::Render()
{
	m_pOptionGUI->Render();
	
	if( m_bHelpScene )
		m_pHelpScene->Render();
	if( m_bSetupScene )
		m_pSetupScene->Render();
}

INT OptionScene::GetSceneNext()
{
	return m_scnNext;
}

INT OptionScene::GetSceneState()
{
	return m_scnState;
}