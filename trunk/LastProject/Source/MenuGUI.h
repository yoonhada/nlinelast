#pragma once
#ifndef _MENUGUI_H_
#define _MENUGUI_H_

class Mouse;
class GUIBase;
class GUIBackground;
class GUIBtnManager;

class MenuGUI
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		CreateBackground();
	VOID		CreateButton();
	
	VOID		OnDown( INT x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnUp( INT x, INT y );

public:
	MenuGUI( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd ) 
		: m_pd3dDevice( _pd3dDevice ), m_pSprite( _pSprite ), m_hWnd( _hWnd )
	{
		this->Initialize();
	}
	~MenuGUI()
	{
		this->Release();
	}

	VOID		Create();
	VOID		Update();
	VOID		Render();

	VOID		Command( DWORD& _dOut );

	BOOL		NextBackgroundImage();

private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPD3DXSPRITE			m_pSprite;
	HWND					m_hWnd;

	Mouse*					m_pMouse;
	GUIBase*				m_pGUIBase;
	GUIBackground*			m_pGUIBackground;
	GUIBtnManager*			m_pGUIBtnManager;

public:

};

#endif