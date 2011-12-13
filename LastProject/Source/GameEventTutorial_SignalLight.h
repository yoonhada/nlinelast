#pragma once
#ifndef _GAMEEVENTTUTORIAL_SIGNALLIGHT_H_
#define _GAMEEVENTTUTORIAL_SIGNALLIGHT_H_

#include "GameEventTutorial.h"

class GUIBase;
class GUIBackground;

class GameEventTutorial_SignalLight : public GameEventTutorial
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	GameEventTutorial_SignalLight( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : m_pd3dDevice( _pd3dDevice ), m_pSprite( _pSprite )
	{
		this->Initialize();
	}
	virtual ~GameEventTutorial_SignalLight()
	{
		this->Release();
	}

	virtual VOID		Create();
	virtual VOID		Update();
	virtual VOID		Render();

	virtual DWORD		GetState();
	virtual VOID		SetState( DWORD _dState );

private:
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	LPD3DXSPRITE		m_pSprite;

	GUIBase*			m_pGUIBase;
	GUIBackground*		m_pGUIBackground;

	DWORD				m_dCurrentState;

public:

};

#endif