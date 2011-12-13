#pragma once
#ifndef _GAMEEVENTTUTORIALMANAGER_H_
#define _GAMEEVENTTUTORIALMANAGER_H_

class GameEventTutorial;

class GameEventTutorialManager
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	GameEventTutorialManager()
	{
		this->Initialize();
	}
	~GameEventTutorialManager()
	{
		this->Release();
	}

	VOID		Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite );
	VOID		Update();
	VOID		Render();

	VOID		ChangeEvent( DWORD _dNextEvent );
	VOID		EndEvent();

private:
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	LPD3DXSPRITE		m_pSprite;

	GameEventTutorial*	m_pGameEventTutorial;

public:

};

#endif

