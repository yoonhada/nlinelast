#pragma once
#ifndef _GUILISTBOX_H_
#define _GUILISTBOX_H_

#include "GUIBase.h"
#include <list>

class GUIBtnManager;
class GUIScrollbar;

class GUIListbox : public GUIBase
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		CopyTextureToItem( INT _iNumItem, LPIMAGE2D _pimg2DTexture, LPIMAGE2D _pimg2DItemNormal, LPIMAGE2D _pimg2DItemDown );
	VOID		CopyTextToItem( INT _iNumItem, LPIMAGE2D _pimg2DText, LPIMAGE2D _pimg2DItemNormal, LPIMAGE2D _pimg2DItemDown );
	VOID		CopyItemToFront( INT _iNumItem, LPIMAGE2D _pimg2DItem, LPIMAGE2D _pimg2DFront );
		
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
	
	typedef struct _ITEM
	{
		LPIMAGE2D	pimg2DNormal;
		LPIMAGE2D	pimg2DDown;

		_ITEM()
		{
			pimg2DNormal = NULL;
			pimg2DDown	= NULL;
		}
		~_ITEM()
		{
			if( pimg2DNormal != NULL )
				delete pimg2DNormal;
			if( pimg2DDown != NULL )
				delete pimg2DDown;
		}
	}ITEM, *LPITEM;

	typedef std::list< LPITEM > ITEMLIST;
	
	typedef struct _DATA
	{
		IMAGE3D		img3DBackground;
		IMAGE2D		img2DFront;			//	나눈건 뭔가 이유가 있을꺼야...기억은 안나지만
		RECT		rtRear;
		
		ITEMLIST	lstItem;

		~_DATA()
		{
			ITEMLIST::iterator itE;
			for( itE = lstItem.begin() ; itE != lstItem.end() ; itE++ )
				delete (*itE);
			lstItem.clear();
		}
	}DATA, *LPDATA;

	//VOID		Create( GUIBase::IMAGEPARAM& _imgFront, GUIBase::IMAGEPARAM& _imgBack );
	VOID		Create( LPIMAGEPARAM _pimgBack );
	VOID		Create( FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, IMAGEPARAM& _imgBackground );
	VOID		AddItem( LPWSTR _Text, LPWSTR _TextureFileName = NULL );
	VOID		SetItemGap( INT _iItemGap );
	VOID		Update( INT _iX, INT _iY );
	VOID		Render();

	VOID		OnDown( INT x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnUp( INT x, INT y );

	VOID		SetFont( LPWSTR _pFaceName, INT _iWidth, INT _iHeight, DWORD _dColor );

private:
	enum { GLB_BTN_UP = 6000, GLB_BTN_DOWN };

	DATA			m_Data;
	//ITEMLIST		m_lstItem;

	DWORD			m_dStyle;		//	가로, 세로 따로 할려고 했는데 아직;;

	FLOAT			m_iMaxItem;
	INT				m_iScrollSpeed;
	INT				m_iItemGap;

	FLOAT			m_fX, m_fY;
	FLOAT			m_fWidth, m_fHeight;

	GUIBtnManager*	m_pGUIBtnManager;
	GUIScrollbar*	m_pGUIScrollbar;
	BOOL			m_bActScrollbar;

	TCHAR			m_aFaceName[ 1024 ];
	INT				m_iFontX, m_iFontY;
	INT				m_iFontWidth, m_iFontHeight;
	DWORD			m_dFontColor;

public:

};

#endif