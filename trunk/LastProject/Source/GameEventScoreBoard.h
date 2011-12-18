#pragma once
#ifndef _GAMEEVENTSCOREBOARD_H_
#define _GAMEEVENTSCOREBOARD_H_

#include "GUIBase.h"

#define MAX_POSITIONAL		10
#define MAX_IDENTIFIER		4

class Mouse;
class GUIBtnManager;

class GameEventScoreBoard : public GUIBase
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		CreateSceneImage();
	VOID		CreateBackgroundImage();
	VOID		CreateIdentifierImage();
	VOID		CreateNumberImage();
	VOID		CreateButton();

	INT			GetNumberOfDecimal( INT  _iNumber );

	VOID		OnDown( INT _iX, INT _iY );
	VOID		OnMove( INT _iX, INT _iY );
	VOID		OnUp( INT _iX, INT _iY );

public:
	enum { GES_NORMAL = 100, GES_GRAY, GES_HIDDEN };
	enum { GES_DADDY = 0, GES_MOM, GES_SON, GES_DAUGHTER };

	GameEventScoreBoard( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd ) 
		: GUIBase( _pd3dDevice, _pSprite ), m_hWnd( _hWnd )
	{
		this->Initialize();
	}
	~GameEventScoreBoard()
	{
		this->Release();
	}

	typedef struct _DATA
	{
		DWORD			dID;

		LPIMAGE3D		pimg3DIdentifier;

		LPIMAGE3D		apimg3DScore[ MAX_POSITIONAL ];
		INT				aiScore[ MAX_POSITIONAL ];

		INT				iPositionalNumber;
		INT				iCurrentPositionalNumber;

		DWORD			dBeginTime;
		DWORD			dCurrentTime;
		
		D3DXVECTOR3		vecTranslate;

		_DATA()
		{
			dBeginTime					= 0;

			iCurrentPositionalNumber	= 0;

			for( INT i=0 ; i<MAX_POSITIONAL ; i++ )
				aiScore[ i ] = 0;
		}
	}DATA, *LPDATA;

	typedef std::vector< LPDATA > DATAVECTOR;

	VOID		Create();
	VOID		Update();
	VOID		Render();

	VOID		AddData( DWORD _dID, DWORD _dIdentifier );
	VOID		SetScore( DWORD _dID, INT _iScore );
	
	VOID		Skip();

	VOID		SetState( DWORD _dState );

	VOID		Command( DWORD& _dOut );

private:
	HWND				m_hWnd;

	DATAVECTOR			m_vecData;

	IMAGE2D				m_img2DScene;	//	È¸»ö

	IMAGE3D				m_img3DBackground;

	IMAGE3D				m_aimg3DIdentifier[ MAX_IDENTIFIER ];

	IMAGE3D				m_img3DDynamicNumber;
	IMAGE3D				m_aimg3DStaticNumber[ MAX_POSITIONAL ];

	Mouse*				m_pMouse;
	GUIBtnManager*		m_pGUIBtnManager;

	DWORD				m_dScoreFrameSpeed;
	DWORD				m_dPositionalFrameSpeed;

	FLOAT				m_fBgdX,		m_fBgdY;
	FLOAT				m_fBgdWidth,	m_fBgdHeight;

	FLOAT				m_fIdtWidth,	m_fIdtHeight;
	FLOAT				m_fNumWidth,	m_fNumHeight;

	DWORD				m_dState;

public:

};

#endif