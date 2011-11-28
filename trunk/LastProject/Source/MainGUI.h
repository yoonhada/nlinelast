#pragma once
#ifndef _MAINGUI_H_
#define _MAINGUI_H_

class Mouse;
class GUIBase;
class GUIBackground;
class GUIListbox;
class GUIEdit;

class MainGUI
{
private:
	VOID		Initialize();
	VOID		Release();

	
	VOID		OnDown( INT x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnUp( INT x, INT y );

public:
	MainGUI( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd ) 
		: m_pd3dDevice( _pd3dDevice ), m_pSprite( _pSprite ), m_hWnd( _hWnd )
	{
		this->Initialize();
	}
	~MainGUI()
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
	GUIBackground*			m_pGUICharacterState;
	GUIBackground*			m_pGUIMiniMap;
	GUIListbox*				m_pChatWindow;
	GUIEdit*				m_pChatEdit;
	
	
public:

};

#endif