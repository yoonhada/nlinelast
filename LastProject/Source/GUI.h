#pragma once 
#ifndef _GUI_H_
#define _GUI_H_

class Mouse;
class GUIBtnManager;

typedef struct _BTNDATA
{
	DWORD	dID;
	DWORD	dStlye;
	FLOAT	fX, fY;
	FLOAT	fWidth, fHeight;
	TCHAR	sNormal[ 1024 ];
	TCHAR	sHot[ 1024 ];
	TCHAR	sDown[ 1024 ];
	TCHAR	sDisable[ 1024 ];
}BTNDATA, *LPBTNDATA;


class GUI
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		OnDown( INT x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnUp( INT x, INT y );

public:
	GUI( LPDIRECT3DDEVICE9 _pd3dDevice, HWND _hWnd ) : m_pd3dDevice( _pd3dDevice ), m_hWnd( _hWnd )
	{
		this->Initialize();
	}
	~GUI()
	{
		this->Release();
	}

	VOID		Create( LPBTNDATA _pBtnData );
	VOID		Update();
	VOID		Render();

	VOID		Command( DWORD& _dID );

private:
	HWND					m_hWnd;
	LPDIRECT3DDEVICE9		m_pd3dDevice;

	Mouse*					m_pMouse;
	GUIBtnManager*			m_pGUIBtnManager;

public:

};

#endif