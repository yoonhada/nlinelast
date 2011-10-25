/**
@file	CManageCube.h
@date	2011/09/20
@author	
@brief	무기 클래스
*/

#pragma once
//class Map;
#include "Map.h"

class CWeapon : public Map
{
private:
	INT m_nFrame;
	INT m_nAKeyFrameTime;
	INT m_nBKeyFrameTime;
	INT m_nWeaponType;
	INT m_nState;
	INT m_nDelay;

public:
	enum _WEAPON { NONE, HAMMER, FRYPEN, GUITAR, CANDY };
	CWeapon( LPDIRECT3DDEVICE9	_pd3dDevice );

	virtual ~CWeapon();

	virtual VOID Clear();
	virtual HRESULT Create();
	virtual HRESULT Release();
	virtual VOID Render();
	virtual VOID Update();

	VOID SetType(INT nType)		{ m_nWeaponType = nType; }
	VOID SetKeyA();
	VOID SetKeyB();

	// Get 
	INT GetKeyFrame()			{ return m_nFrame; }
};