#pragma once
#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_

#include "Scene.h"

class MenuGUI;
class OptionScene;
class LoginScene;

class MenuScene : public IScene
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	MenuScene() 
	{
		this->Initialize();
	}
	virtual ~MenuScene()
	{
		this->Release();
	}
	
	virtual HRESULT		Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd );

	virtual VOID		Update();
	virtual VOID		Render();

	virtual INT			GetSceneNext();
	virtual INT			GetSceneState();

private:
	LPDIRECT3DDEVICE9		m_pD3dDevice;
	LPD3DXSPRITE			m_pSprite;
	HWND					m_hWnd;
	
	
	INT						m_scnNext;
	INT						m_scnState;

	MenuGUI*				m_pMenuGUI;
	OptionScene*			m_pOptionScene;
	LoginScene*				m_pLoginScene;

	BOOL					m_bLoginScene;

public:

};

#endif