#pragma once
#ifndef _LOOBYGUI_H_
#define _LOBBYGUI_H_

class Mouse;
class GUIBase;
class GUIBackground;
class GUIBtnManager;
class GUIEditManager;
class GUIListbox;

class LobbyGUI
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		CreateBackground();
	VOID		CreateButton();
	VOID		CreateEdit();
	VOID		CreateListbox();
	
	VOID		OnDown( INT x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnUp( INT x, INT y );

public:
	LobbyGUI( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd ) 
		: m_pD3dDevice( _pd3dDevice ), m_pSprite( _pSprite ), m_hWnd( _hWnd )
	{
		this->Initialize();
	}
	virtual ~LobbyGUI()
	{
		this->Release();
	}

	VOID		Create();
	VOID		Update();
	VOID		Render();

	VOID		Command( DWORD& _dOut );
	VOID		EnableButton( DWORD _dID, BOOL _bEnable );
	DWORD		GetButtonState( DWORD _dID );
	VOID		ChangeScene( DWORD _dID, DWORD _dState );
	VOID		SetText( LPWSTR _pText );
	VOID		GetText( LPWSTR _pText );
	BOOL		TakeChattingMassage( LPWSTR _pText );
	
private:
	LPDIRECT3DDEVICE9		m_pD3dDevice;
	LPD3DXSPRITE			m_pSprite;
	HWND					m_hWnd;

	FLOAT					m_fX, m_fY;
	FLOAT					m_fWidth, m_fHeight;

	LPWSTR					m_pStr;

	Mouse*					m_pMouse;
	GUIBase*				m_pGUIBase;
	GUIBackground*			m_pGUIBackground;
	GUIBtnManager*			m_pGUIBtnManager;
	GUIEditManager*			m_pGUIEditManager;
	GUIListbox*				m_pGUIListbox;

public:

};

#endif