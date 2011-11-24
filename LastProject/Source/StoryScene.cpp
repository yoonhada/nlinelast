#include "stdafx.h"
#include "StoryScene.h"
#include "StoryGUI.h"

VOID StoryScene::Initialize()
{
	m_scnNext		= SCENE_MENU;
	m_scnState		= SCENE_RUNNING;

	m_pStoryGUI		= NULL;
	
}

VOID StoryScene::Release()
{
	SAFE_DELETE( m_pStoryGUI );
}

HRESULT StoryScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pStoryGUI		= new StoryGUI( _pd3dDevice, _pSprite, _hWnd );
	m_pStoryGUI->Create();

	return S_OK;
}

VOID StoryScene::Update()
{
	m_pStoryGUI->Update();

	BOOL bEndStory = TRUE;

	DWORD dID;
	m_pStoryGUI->Command( dID );
	switch( dID )
	{
	case GUIBTN_STORY_NEXT:
		bEndStory = m_pStoryGUI->NextBackgroundImage();
		break;
	}
	if( !bEndStory )
		m_scnState = IScene::SCENE_END;
}

VOID StoryScene::Render()
{
	m_pStoryGUI->Render();
}

INT StoryScene::GetSceneNext()
{
	return m_scnNext;
}

INT StoryScene::GetSceneState()
{
	return m_scnState;
}