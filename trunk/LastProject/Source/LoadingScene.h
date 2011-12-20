#pragma once
#ifndef _LOADINGSCENE_H_
#define _LOADINGSCENE_H_

#include "Scene.h"

class LoadingGUI;
class CMonster;

class LoadingScene : public IScene
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	LoadingScene() 
	{
		this->Initialize();
	}
	virtual ~LoadingScene()
	{
		this->Release();
	}

	virtual HRESULT	Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd );
	virtual VOID	Update();
	virtual VOID	Render();
	
	virtual INT		GetSceneNext();
	virtual INT		GetSceneState();

	VOID			SelectScene( DWORD _dScene );

private:
	LPDIRECT3DDEVICE9		m_pD3dDevice;
	LPD3DXSPRITE			m_pSprite;
	HWND					m_hWnd;
	

	INT						m_scnNext;
	INT						m_scnState;

	LoadingGUI*				m_pLoadingGUI;

	CLight*					m_pLight;
	CMatrices*				m_pMatrices;

	CMonster*				m_pClown;
	CMonster*				m_pPanda;
	CMonster*				m_pBrownBear;

public:

};

#endif