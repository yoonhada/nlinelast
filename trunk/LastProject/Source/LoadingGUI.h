#pragma once
#ifndef _LOADINGGUI_H_
#define _LOADINGGUI_H_

class GUIBase;
class GUIBackground;

class LoadingGUI
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		CreateBackground();

public:
	LoadingGUI( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd ) 
		: m_pD3dDevice( _pd3dDevice ), m_pSprite( _pSprite ), m_hWnd( _hWnd )
	{
		this->Initialize();
	}
	virtual ~LoadingGUI()
	{
		this->Release();
	}

	VOID		Create();
	VOID		Update();
	VOID		Render();

private:
	LPDIRECT3DDEVICE9		m_pD3dDevice;
	LPD3DXSPRITE			m_pSprite;
	HWND					m_hWnd;

	GUIBase*				m_pGUIBase;
	GUIBackground*			m_pGUIBackground;

public:

};

#endif