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

	
	VOID		OnDown( INT  x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnUp( INT x, INT y );

	VOID		CreateMiniMap();

public:
	enum {	MMP_DADDY = 0, MMP_MOM, MMP_SON, MMP_DAUGHTER, 
			MMP_FIRSTAIDKIT_0, MMP_FIRSTAIDKIT_1, MMP_FIRSTAIDKIT_2, MMP_FIRSTAIDKIT_3, MMP_FIRSTAIDKIT_4, 
			MMP_PANDA, MMP_BEAR, MMP_CROWN,
			MMP_END };

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

	//	MiniMap
	VOID		SetMiniMapObjectVisible( INT _iIndex, BOOL _bVisible );
	VOID		SetMiniMapObjectPosition( INT _iIndex, LPD3DXVECTOR3 _pvecPosition );

private:
	LPDIRECT3DDEVICE9		m_pD3dDevice;
	LPD3DXSPRITE			m_pSprite;
	HWND					m_hWnd;

	Mouse*					m_pMouse;
	GUIBase*				m_pGUIBase;
	GUIListbox*				m_pChatWindow;
	GUIEditManager*			m_pChatEdit;

	//	MiniMap Data
	GUIBackground*			m_pMiniMap;
	GUIBackground*			m_pMmpObject[ 12 ];

	FLOAT					m_fMapWidth,	m_fMapHeight;
	FLOAT					m_fMmpRatioX,	m_fMmpRatioY;

	FLOAT					m_fMmpX,		m_fMmpY;
	FLOAT					m_fMmpWidth,	m_fMmpHeight;
	
public:

};

#endif