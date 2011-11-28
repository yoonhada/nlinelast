#pragma once
#ifndef _SETUPSCENE_H_
#define _SETUPSCENE_H_

#include "Scene.h"

class SetupGUI;

class SetupScene : public IScene
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	SetupScene() 
	{
		this->Initialize();
	}
	virtual ~SetupScene()
	{
		this->Release();
	}

	virtual HRESULT	Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd );
	virtual VOID	Update();
	virtual VOID	Render();
	
	virtual INT		GetSceneNext();
	virtual INT		GetSceneState();

private:
	LPDIRECT3DDEVICE9		m_pD3dDevice;
	LPD3DXSPRITE			m_pSprite;
	HWND					m_hWnd;

	INT						m_scnNext;
	INT						m_scnState;

	SetupGUI*				m_pSetupGUI;

public:

};

#endif