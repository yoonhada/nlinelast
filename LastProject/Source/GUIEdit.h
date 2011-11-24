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
		BOOL	bActivate;
		RECT	rtText;
		TCHAR	Str[ 1024 ];

		IMAGE	Image;

		_DATA()
		{
			bActivate = FALSE;
		}
	}DATA, *LPDATA;

	VOID		Create( GUIBase::IMAGEPARAM& _imgParam );
	VOID		Update();
	VOID		Render();

private:
	//	GUIBase::m_hFont 
	//	GUIBase에 넣어야 하나?
	LPD3DXFONT		m_pFont;
	
	HDC				m_hDC;
	HFONT			m_hFont, m_hOldFont;

	DATA			m_Data;

	D3DXVECTOR3		m_vecPosition;
	IMAGE2D			m_img2DCaret;

public:

};

#endif