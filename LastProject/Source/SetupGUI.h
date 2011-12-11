#pragma once
#ifndef _SETUPGUI_H_
#define _SETUPGUI_H_

class Mouse;
class GUIBase;
class GUIBackground;
class GUIScrollbar;

class SetupGUI
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		CreateBackground();
	VOID		CreateScrollbar();
	
	VOID		OnDown( INT x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnUp( INT x, INT y );

public:
	SetupGUI( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd ) 
		: m_pD3dDevice( _pd3dDevice ), m_pSprite( _pSprite ), m_hWnd( _hWnd )
	{
		this->Initialize();
	}
	virtual ~SetupGUI()
	{
		this->Release();
	}

	VOID		Create();
	VOID		Update();
	VOID		Render();

	VOID		Command( DWORD& _dOut );

	BOOL		NextBackgroundImage();

private:
	LPDIRECT3DDEVICE9		m_pD3dDevice;
	LPD3DXSPRITE			m_pSprite;
	HWND					m_hWnd;

	Mouse*					m_pMouse;
	GUIBase*				m_pGUIBase;
	GUIBackground*			m_pGUIBackground;
	GUIScrollbar*			m_pGUIEffectSound;
	GUIScrollbar*			m_pGUIBackSound;

	FLOAT					m_fX, m_fY;
	FLOAT					m_fWidth, m_fHeight;

public:

};

#endif