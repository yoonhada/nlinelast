#pragma once
#ifndef _GUIEDIT_H_
#define _GUIEDIT_H_

#include "GUIBase.h"

class GUICaret;

class GUIEdit : public GUIBase
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		Cleanup();

	BOOL		IsPtOnMe( INT _iX, INT _iY );

public:
	enum { EDT_STYLE_STATIC = 0, EDT_STYLE_DYNAMIC };
	enum { EDT_STATE_NORMAL = 1000, EDT_STATE_DISABLE, EDT_STATE_HIDDEN };

	GUIEdit( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : GUIBase( _pd3dDevice, _pSprite )
	{
		this->Initialize();
	}
	virtual ~GUIEdit()
	{
		this->Release();
	}

	//VOID			Create( GUIBase::IMAGEPARAM& _imgParam );
	VOID			Create( DWORD _dID, FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, IMAGEPARAM& _imgParamEdit, IMAGEPARAM& _imgParamCaret );
	BOOL			Update( BOOL _bReturn );
	VOID			Render();

	VOID			SetStyle( DWORD _dStyle );
	VOID			SetState( DWORD _dState );
	VOID			EnableFocus( BOOL _bEnable );
	VOID			SetFont( LPWSTR _pFaceName, INT _iWidth, INT _iHeight );
	VOID			SetText( LPWSTR _pcText );
	BOOL			TakeMessage( LPWSTR _pcText );
	
	LPWSTR			GetText(){ return m_pcText; };
	INT*			GetTextLength(){ return &m_iTextLength; };
	BOOL			GetFocus(){ return m_bFocus; };

	//	영역 체크
	BOOL			OnDown( INT _iX, INT _iY );
	BOOL			OnMove( INT _iX, INT _iY );
	BOOL			OnUp( INT _iX, INT _iY );

private:
	DWORD			m_dID;
	DWORD			m_dStyle;
	DWORD			m_dState;
	
	BOOL			m_bFocus;
	BOOL			m_bMessage;

	RECT			m_rtText;
	RECT			m_rtEdit;
	IMAGE3D			m_img3DEdit;
	
	//	Font Info
	TCHAR			m_aFaceName[ 1024 ];
	INT				m_iFontX, m_iFontY;
	INT				m_iFontWidth, m_iFontHeight;

	TCHAR			m_pcText[ 1024 ];
	INT				m_iTextLength;

	GUICaret*		m_pGUICaret;

public:
	static DWORD	EdtMessage;

};

#endif