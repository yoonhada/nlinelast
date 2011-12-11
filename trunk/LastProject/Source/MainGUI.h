#pragma once
#ifndef _MAINGUI_H_
#define _MAINGUI_H_

class Mouse;
class GUIBase;
class GUIBackground;
class GUIListbox;
class GUIEditManager;

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
		: m_pD3dDevice( _pd3dDevice ), m_pSprite( _pSprite ), m_hWnd( _hWnd )
	{
		this->Initialize();
	}
	virtual ~MainGUI()
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
	GUIBackground*			m_pGUICharacterState;
	GUIBackground*			m_pGUIMiniMap;
	GUIListbox*				m_pChatWindow;
	GUIEditManager*			m_pChatEdit;
	
	
public:

};

#endif