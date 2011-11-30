#pragma once
#ifndef _GUIBTNMANAGER_H_
#define _GUIBTNMANAGER_H_

#include "GUIBase.h"

class GUIBtn;

class GUIBtnManager
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	GUIBtnManager( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : m_pd3dDevice( _pd3dDevice ), m_pSprite( _pSprite )
	{
		this->Initialize();
	}
	~GUIBtnManager()
	{
		this->Release();
	}

	typedef struct _DATA
	{
		DWORD		ID;
		GUIBtn*		pGUIBtn;

		_DATA()
		{
			pGUIBtn = NULL;
		}
	}DATA, *LPDATA;

	typedef std::vector< LPDATA > DATAVECTOR;

	VOID		Create( DWORD _dID, 
						DWORD _dStyle, 
						GUIBase::IMAGEPARAM& _imgNormal, 
						GUIBase::IMAGEPARAM& _imgHot, 
						GUIBase::IMAGEPARAM& _imgDown, 
						GUIBase::IMAGEPARAM& _imgDisable );

	VOID		Create( DWORD _dID, 
						DWORD _dStyle, 
						FLOAT _fX, FLOAT _fY,
						FLOAT _fWidth, FLOAT _fHeight,
						GUIBase::IMAGEPARAM& _imgNormal, 
						GUIBase::IMAGEPARAM& _imgHot, 
						GUIBase::IMAGEPARAM& _imgDown, 
						GUIBase::IMAGEPARAM& _imgDisable );

	VOID		Update( INT x, INT y );
	VOID		Render();

	VOID		OnDown( INT x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnUp( INT x, INT y );

	VOID		GetCommandID( DWORD& _ID );
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPD3DXSPRITE			m_pSprite;

	DATAVECTOR				m_vecData;
	INT						m_iNumData;

public:

};

#endif