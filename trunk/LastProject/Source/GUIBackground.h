#pragma once
#ifndef _GUIBACKGROUND_H_
#define _GUIBACKGROUND_H_

#include "GUIBase.h"

class GUIBackground : public GUIBase
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	GUIBackground( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : GUIBase( _pd3dDevice, _pSprite )
	{
		this->Initialize();
	}
	virtual ~GUIBackground()
	{
		this->Release();
	}

	typedef struct _DATA
	{
		IMAGE		Image;
	}DATA, *LPDATA;

	VOID		Create( IMAGEPARAM& _imgParam );
	VOID		Update();
	VOID		Render();

	BOOL		NextAnimation();

private:
	DATA		m_Data;
	
public:

};

#endif