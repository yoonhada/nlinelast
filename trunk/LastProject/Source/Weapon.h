/**
@file	CManageCube.h
@date	2011/09/20
@author	
@brief	무기 클래스
*/

#pragma once

class Map;
typedef struct _WEAPONTYPE
{
	INT nAKeyFrameTime;
	INT nBKeyFrameTime;
	INT nWeaponType;
	INT nDelay;
}WEAPONTYPE;

class CWeapon : public IObject
{
private:
	INT m_nState;
	INT m_nFrame;
	WEAPONTYPE m_WeaponType;
	Map * m_pMap;

public:
	enum _WEAPON { NONE, HAMMER, FRYPEN, GUITAR, CANDY };
	CWeapon( LPDIRECT3DDEVICE9	_pd3dDevice );

	virtual ~CWeapon();

	virtual VOID Clear();
	virtual HRESULT Create();
	virtual HRESULT Release();
	virtual VOID Render( ){};
	virtual VOID Update();

	VOID Render( D3DXMATRIX _matCharacter );
	VOID SetType(INT nType)		{ m_WeaponType.nWeaponType = nType; }
	INT SetKeyA();
	INT SetKeyB();

	const D3DXMATRIXA16& Get_MatWorld();
};