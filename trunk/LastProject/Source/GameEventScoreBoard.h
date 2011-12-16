#pragma once
#ifndef _GAMEEVENTSCOREBOARD_H_
#define _GAMEEVENTSCOREBOARD_H_

#include <vector>

class GUIBase;
class GUIBackground;

#define MAXPOSITIONALNUMBER		10
#define MAXIDENTIFIER			4

class GameEventScoreBoard
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		InitIdentifierImage();
	VOID		InitNumberImage();

	INT			GetNumberOfDecimal( INT _iNumber );
	VOID		LinkScore( INT _iPositionalNumber, GUIBackground* _apScore, INT* _aiScore );
	
public:
	enum { GES_DADDY = 0, GES_MOM, GES_SON, GES_DAUGHTER };

	GameEventScoreBoard()
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

		GUIBackground*	pIdentifier;

		GUIBackground*	apScore[ MAXPOSITIONALNUMBER ];
		INT				aiScore[ MAXPOSITIONALNUMBER ];

		INT				iPositionalNumber;
		INT				iCurrentPositionalNumber;

		DWORD			dBeginTime;
		DWORD			dCurrentTime;
		DWORD			dFrameSpeed;

		D3DXVECTOR3		vecTranslate;

		_DATA()
		{
			dBeginTime			= 0;
			dFrameSpeed			= 800;

			for( INT i=0 ; i<MAXPOSITIONALNUMBER ; i++ )
				aiScore[ i ] = 0;
		}
	}DATA, *LPDATA;

	typedef std::vector< LPDATA > DATAVECTOR;

	VOID		Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite );
	VOID		Update();
	VOID		Render();

	VOID		AddData( DWORD _dID, DWORD _dIdentifier );
	VOID		SetScore( DWORD _dID, INT _iScore );
	
private:
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	LPD3DXSPRITE		m_pSprite;

	DATAVECTOR			m_vecData;

	GUIBase*			m_pGUIBase;

	GUIBackground*		m_pIdentifier[ MAXIDENTIFIER ];

	GUIBackground*		m_pDynamicNumber;
	GUIBackground*		m_pStaticNumber[ MAXPOSITIONALNUMBER ];

	DWORD				m_dFrameSpeed;

	FLOAT				m_fX,			m_fY;
	FLOAT				m_fWidth,		m_fHeight;

	FLOAT				m_fIdtWidth,	m_fIdtHeight;
	FLOAT				m_fNumWidth,	m_fNumHeight;

public:

};

#endif