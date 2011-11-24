#pragma once
#ifndef _LOGINSCENE_H_
#define _LOGINSCENE_H_

#include "Scene.h"

class LoginGUI;

class LoginScene : public IScene
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	LoginScene() 
	{
		this->Initialize();
	}
	virtual ~LoginScene()
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

	LoginGUI*				m_pLoginGUI;

public:

};

#endif