#pragma once
#ifndef _GAMEEVENTTUTORIALMANAGER_H_
#define _GAMEEVENTTUTORIALMANAGER_H_

class GameEventTutorial;

class GameEventTutorialManager
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		ChangeEvent( DWORD _dNextEvent );

public:
	GameEventTutorialManager( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite )
	{
		this->Initialize();
	}
	~GameEventTutorialManager()
	{
		this->Release();
	}

	VOID		Create();
	VOID		Update();
	VOID		Render();

private:
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	LPD3DXSPRITE		m_pSprite;

	GameEventTutorial*	m_pGameEventTutorial;

public:

};

#endif

