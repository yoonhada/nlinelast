#pragma once
#ifndef _LOBBYSCENE_H_
#define _LOBBYSCENE_H_

#include "Scene.h"

class LobbyGUI;
class OptionScene;
class CCharactor;
class CMonster;

class LobbyScene : public IScene
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	LobbyScene() 
	{
		this->Initialize();
	}
	virtual ~LobbyScene()
	{
		this->Release();
	}
	
	typedef struct _DATA
	{
		CCharactor*		pCharacter;
		D3DXVECTOR3		vecPosition;
		
		BOOL			bRotate;

		DWORD			dID;

		_DATA()
		{
			pCharacter	= NULL;
			bRotate		= FALSE;
		}
	}DATA, *LPDATA;

	virtual HRESULT	Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd );
	virtual VOID	Update();
	virtual VOID	Render();
	
	virtual INT		GetSceneNext();
	virtual INT		GetSceneState();

	VOID			CreateData( LPDATA _pData, LPWSTR _pFileName, D3DXVECTOR3 _vecPosition );
	VOID			ActivateRotate( INT _iIndex );

private:
	LPDIRECT3DDEVICE9		m_pD3dDevice;
	LPD3DXSPRITE			m_pSprite;
	HWND					m_hWnd;
	
	INT						m_scnNext;
	INT						m_scnState;

	LobbyGUI*				m_pLobbyGUI;
	OptionScene*			m_pOptionScene;
	
	LPDATA					m_aData;
	FLOAT					m_fChrRotate;
	FLOAT					m_fIncRotate;
	
	CLight*					m_pLight;
	CMatrices*				m_pMatrices;

	CMonster*				m_pClown;
	CMonster*				m_pPanda;


public:

};

#endif