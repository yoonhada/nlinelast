#pragma once
#ifndef _GUIEDITMANAGER_H_
#define _GUIEDITMANAGER_H_

#include "GUIBase.h"

class GUIEdit;

class GUIEditManager
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		CheckFocus();

public:
	enum { EDT_STATIC = 0, EDT_DYNAMIC };
	enum { EDT_STATE_NORMAL = 1000, EDT_STATE_DISABLE, EDT_STATE_HIDDEN };

	GUIEditManager( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : m_pd3dDevice( _pd3dDevice ), m_pSprite( _pSprite )
	{
		this->Initialize();
	}
	virtual ~GUIEditManager()
	{
		this->Release();
	}

	typedef struct _DATA
	{
		DWORD		ID;
		GUIEdit*	pGUIEdit;

		_DATA()
		{
			pGUIEdit = NULL;
		}
	}DATA, *LPDATA;

	typedef std::vector< LPDATA > DATAVECTOR;

	/*VOID		Create( DWORD _dID, 
						DWORD _dStyle, 
						GUIBase::IMAGEPARAM& _imgNormal, 
						GUIBase::IMAGEPARAM& _imgHot, 
						GUIBase::IMAGEPARAM& _imgDown, 
						GUIBase::IMAGEPARAM& _imgDisable );*/

	VOID		Create( DWORD _dID, 
						DWORD _dStyle, 
						FLOAT _fX, FLOAT _fY,
						FLOAT _fWidth, FLOAT _fHeight,
						GUIBase::IMAGEPARAM& _imgParamEdit, 
						GUIBase::IMAGEPARAM& _imgParamCaret );

	VOID		Update();
	VOID		Render();

	VOID		OnDown( INT x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnUp( INT x, INT y );

	VOID		GetCommandID( DWORD& _ID );
	VOID		SetStyle( DWORD _dID, DWORD _dStyle );
	VOID		SetState( DWORD _dID, DWORD _dState );
	VOID		SetText( DWORD _dID, LPWSTR _pcText );
	VOID		SetFont( DWORD _dID, LPWSTR _pFaceName, INT _iWidth, INT _iHeight ); 
	BOOL		TakeMessage( DWORD _dID, LPWSTR _pcText );

private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPD3DXSPRITE			m_pSprite;

	DATAVECTOR				m_vecData;
	INT						m_iNumData;

public:

};

#endif