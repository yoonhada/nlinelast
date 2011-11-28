#pragma once
#ifndef _LOBBYSCENE_H_
#define _LOBBYSCENE_H_

#include "Scene.h"

class LobbyGUI;
class OptionScene;

class LobbyScene : public IScene
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	LobbyScene() 
	{
		this->Initialize();
	}
	virtual ~LobbyScene()
	{
		this->Release();
	}

	virtual HRESULT	Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd );
	virtual VOID	Update();
	virtual VOID	Render();
	
	virtual INT		GetSceneNext();
	virtual INT		GetSceneState();

private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPD3DXSPRITE			m_pSprite;
	HWND					m_hWnd;
	
	INT						m_scnNext;
	INT						m_scnState;

	LobbyGUI*				m_pLobbyGUI;
	OptionScene*			m_pOptionScene;

public:

};

#endif