#pragma once
#ifndef _LOGOSCENE_H_
#define _LOGOSCENE_H_

#include "Scene.h"

class CCharactor;
class CameraWork;
//class CEfSurface;

class LogoScene : public IScene
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	LogoScene() 
	{
		this->Initialize();
	}
	virtual ~LogoScene()
	{
		this->Release();
	}

	typedef struct _DATA
	{
		BOOL	bActivate;
		DWORD	dFrameSpeed;
		DWORD	dBeginTime;
	}DATA, *LPDATA;

	virtual HRESULT	Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd );
	virtual VOID	Update();
	virtual VOID	Render();
	
	virtual INT		GetSceneNext();
	virtual INT		GetSceneState();

private:
	LPDIRECT3DDEVICE9		m_pD3dDevice;
	LPD3DXSPRITE			m_pSprite;
	HWND					m_hWnd;
	
	INT						m_scnNext;
	INT						m_scnState;

	CCharactor*				m_pLogo;
	CameraWork*				m_pCameraWork;

	CLight*					m_pLight;
	CMatrices*				m_pMatrices;

	DATA					m_datLogo;
	DATA					m_datScene;

	//CEfSurface	* m_pEff;
public:
};

#endif