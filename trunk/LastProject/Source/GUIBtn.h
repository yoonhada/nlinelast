#pragma once
#ifndef _GUIBTN_H_
#define _GUIBTN_H_

#include "GUIBase.h"

#define GBS_PUSH		0
#define GBS_CHECK		1
#define GBN_CLICKED		0

enum eState { GB_NORMAL, GB_HOT, GB_DOWN, GB_DISABLE, GB_HIDDEN };

class GUIBtn : public GUIBase
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		Cleanup();

	VOID		ChangeState( eState _State );
	BOOL		IsPtOnMe( POINT pt );
	BOOL		IsPtOnMe( INT x, INT y );

public:
	GUIBtn( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : GUIBase( _pd3dDevice, _pSprite )
	{
		this->Initialize();
	}
	virtual ~GUIBtn()
	{ 
		this->Release();
	}

	typedef struct _DATA
	{
		IMAGE3D				aImage3D[ 4 ];
		D3DXVECTOR3			vecPosition;
		FLOAT				fWidth;
		FLOAT				fHeight;
	}DATA, *LPDATA;

	VOID		Create(		DWORD _dID, 
							DWORD _dStyle, 
							IMAGEPARAM& _imgNormal, 
							IMAGEPARAM& _imgHot, 
							IMAGEPARAM& _imgDown, 
							IMAGEPARAM& _imgDisable );

	VOID		Create(		DWORD _dID,							//	ID Resource
							DWORD _dStyle,						//	Style GBS_PUSH || GBS_CHECK
							FLOAT _fX, FLOAT _fY,				//	Button Position
							FLOAT _fWidth, FLOAT _fHeight,		//	Button Width, Height
							IMAGEPARAM& _imgNormal, 			//	Normal TextureFileName
							IMAGEPARAM& _imgHot, 				//	Hot TextureFileName
							IMAGEPARAM& _imgDown, 				//	Down TextureFileName
							IMAGEPARAM& _imgDisable );			//	Disable TextureFileName

	eState		GetState() { return m_State; }
	VOID		SetState( eState _State );
	VOID		OnDown( INT x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnUp( INT x, INT y );
	VOID		Enable(BOOL bEnable);
	BOOL		IsEnabled() { return ( m_State != GB_DISABLE ); }
	VOID		Show( BOOL bShow );
	BOOL		IsShow() { return ( m_State != GB_HIDDEN ); }
	VOID		Move( INT _x,INT _y );
	
	VOID		Render();
	VOID		Update( INT x, INT y );

private:
	DATA					m_Data;

	DWORD					m_ID;
	DWORD					m_Style;

	eState					m_State;
	eState					m_CheckState;
	
	BOOL					m_bCapture;
	
public:
	static BOOL				g_bCapture;

	static DWORD			btnMessage;

};

#endif