#pragma once
#ifndef _GUISCROLLBAR_H_
#define _GUISCROLLBAR_H_

#include "GUIBase.h"

class GUIScrollbar : public GUIBase
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		GetThumbRect( RECT *trt );
	VOID		PosFromPixel( INT Pixel );

public:
	GUIScrollbar( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : GUIBase( _pd3dDevice, _pSprite )
	{
		this->Initialize();
	}

	virtual ~GUIScrollbar()
	{
		this->Release();
	}
	
	enum { SCL_HROZ = 0, SCL_VERT };

	typedef struct _DATA
	{
		IMAGE3D	img3DBack;
		IMAGE3D	img3DThumb;
		
		RECT	rtBack;
		RECT	rtThumb;

		POINT	ptBackPosition;

		INT		iMax, iMin;
		INT		iOffset;
		INT		iTSize;
		INT		iTGap;

		INT		iValue;
	}DATA, *LPDATA;

	VOID		Create( IMAGEPARAM& _imgThumb, IMAGEPARAM& _imgBack );
	VOID		SetStyle( DWORD _dStyle );
	VOID		SetRange( INT _iMin, INT _iMax );

	VOID		Update();
	VOID		Render();

	VOID		OnUp( INT x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnDown( INT x, INT y );

	INT			GetValue(){ return m_Data.iValue; };
	VOID		SetValue( INT _iValue ){ m_Data.iValue = _iValue; };

private:
	DATA				m_Data;
	DWORD				m_dStyle;

	//	Mouse
	BOOL				m_bLKeyDown;		//	마우스상태
	BOOL				m_bLKeyUp;
	BOOL				m_bLKeyDrag;
	//	End

public:

};

#endif