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

	VOID		CreateDFont( LPWSTR _pFaceName, INT _iWidth, INT _iHeight );	// CreateDirectxFont
	BOOL		TextOutput( RECT& _rt, LPCWSTR lpszStr, DWORD Color );

public:
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

		INT		iX, iY;
		//INT		iWidth, iHeight;

	}DATA, *LPDATA;

	VOID			Create( GUIBase::IMAGEPARAM& _imgParam );
	VOID			Create( FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, GUIBase::IMAGEPARAM& _imgParam );
	VOID			Update();
	VOID			Render();

	BOOL			TakeMessage( LPWSTR _pStr );

private:
	DATA			m_Data;
	
	BOOL			m_bActivate;
	BOOL			m_bMessage;

public:

};

#endif