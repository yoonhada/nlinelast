#pragma once
#ifndef _OPTIONSCENE_H_
#define _OPTIONSCENE_H_

#include "Scene.h"

class OptionGUI;
class HelpScene;
class SetupScene;

class OptionScene : public IScene
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	OptionScene() 
	{
		this->Initialize();
	}
	virtual ~OptionScene()
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

	OptionGUI*				m_pOptionGUI;
	HelpScene*				m_pHelpScene;
	SetupScene*				m_pSetupScene;

	BOOL					m_bHelpScene;
	BOOL					m_bSetupScene;

public:

};

#endif