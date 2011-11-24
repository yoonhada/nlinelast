#pragma once
#ifndef _GUILISTBOX_H_
#define _GUILISTBOX_H_

#include "GUIBase.h"
#include <list>

class GUIScrollbar;

class GUIListbox : public GUIBase
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		CopyItemToBack( LPIMAGE _pimgItem, LPIMAGE _pimgBack );
	VOID		CopyBackToFront( LPIMAGE _pimgBack, LPIMAGE _pimgFront );
	
public:
	GUIListbox( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : GUIBase( _pd3dDevice, _pSprite )
	{
		this->Initialize();
	}
	virtual ~GUIListbox()
	{
		this->Release();
	}
	
	enum { LBX_HROZ = 0, LBX_VERT };

	typedef struct _DATA
	{
		IMAGE	imgFront;
		IMAGE	imgBack;
		
		RECT	rtFront;
		RECT	rtBack;
		
		FLOAT	fRatioBackToFront;
		FLOAT	fScrollSpeed;
		INT		iItemGap;

		FLOAT	fX, fY;
		FLOAT	fWidth, fHeight;
		_DATA()
		{
			iItemGap			= 50;
		}
	}DATA, *LPDATA;

	typedef struct _ITEM
	{
		LPIMAGE	pimgTexture;
		LPIMAGE	pimgText;

		_ITEM()
		{
			pimgTexture = NULL;
			pimgText	= NULL;
		}
		~_ITEM()
		{
			if( pimgTexture != NULL )
				delete pimgTexture;
			if( pimgText != NULL )
				delete pimgText;
		}
	}ITEM, *LPITEM;

	typedef std::list< LPITEM > ITEMLIST;

	//VOID		Create( FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight );
	VOID		Create( GUIBase::IMAGEPARAM& _imgFront, GUIBase::IMAGEPARAM& _imgBack );
	VOID		AddItem( LPWSTR _Text, LPWSTR _TextureFileName = NULL );
	VOID		SetItemGap( INT _iItemGap );
	VOID		Update();
	VOID		Render();

	VOID		OnDown( INT x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnUp( INT x, INT y );

private:
	GUIScrollbar*	m_pGUIScrollbar;

	DATA			m_Data;
	ITEMLIST		m_lstItem;

	DWORD			m_dStyle;		//	가로, 세로 따로 할려고 했는데 아직;;
	BOOL			m_bActScrollbar;
public:

};

#endif