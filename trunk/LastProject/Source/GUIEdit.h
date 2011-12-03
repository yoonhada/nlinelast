#pragma once
#ifndef _GUIEDIT_H_
#define _GUIEDIT_H_

#include "GUIBase.h"

class GUIEdit : public GUIBase
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		Cleanup();

	VOID		CreateGUICaret();

public:
	enum { EDT_STATIC = 0, EDT_DYNAMIC };

	GUIEdit( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : GUIBase( _pd3dDevice, _pSprite )
	{
		this->Initialize();
	}
	~GUIEdit()
	{
		this->Release();
	}
	
	typedef struct _DATA
	{
		RECT	rtText;
		TCHAR	Str[ 1024 ];

		IMAGE2D	img2DBackground;
		IMAGE2D	img2DCaret;

	}DATA, *LPDATA;

	VOID			Create( GUIBase::IMAGEPARAM& _imgParam );
	VOID			Create( FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, GUIBase::IMAGEPARAM& _imgParam );
	VOID			Update();
	VOID			Render();

	VOID			SetStyle( DWORD _dStyle );
	VOID			SetFont( LPWSTR _pFaceName, INT _iWidth, INT _iHeight );
	BOOL			TakeMessage( LPWSTR _pStr );
private:
	DATA			m_Data;
	
	DWORD			m_dStyle;

	BOOL			m_bActivate;
	BOOL			m_bMessage;

	//	Font Info
	TCHAR			m_aFaceName[ 1024 ];
	INT				m_iFontX, m_iFontY;
	INT				m_iFontWidth, m_iFontHeight;

public:

};

#endif