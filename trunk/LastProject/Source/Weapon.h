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
	// <-----------Time-------------->
	// Begin
	// +-------------+----+----------+
	// <-----ATK----->
	// <------Delay------>
	INT nType;
	INT nFrameBegin[10];
	INT nFrameTime[10];
	INT nFrameAtk[10];
	INT nDelay[10];
	D3DXVECTOR3 vDir[10];

	CBoundBox pBBA;

}WEAPONTYPE;

class CWeapon : public IObject
{
public:
	INT Get_nFrame();
	INT Get_nState();
	VOID Set_nState( INT _nState );

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	INT m_nState;
	INT m_nReversing;
	WEAPONTYPE m_WeaponType;
	Map * m_pMap;
	BOOL m_bAtkTime;
	FLOAT m_afZAng[10];

public:
	enum _WEAPON { NONE, SPANNER, FRYPEN, GUITAR, MAGICSTICK };
	enum _ASEANI { IDLE = 1000, POST_IDLE };
	CWeapon( LPDIRECT3DDEVICE9	_pd3dDevice );

	virtual ~CWeapon();

	virtual VOID Clear();
	virtual HRESULT Create();
	virtual HRESULT Release();
	virtual VOID Render( ){};
	virtual VOID Update();

	VOID Render( D3DXMATRIX _matCharacter );
	VOID SetType(INT nType)		{ m_WeaponType.nType = nType; }
	VOID SetKeyA();
	VOID SetKeyB();
	VOID SetBBx( const D3DXVECTOR3& vPos, const FLOAT fAngle );

	const D3DXMATRIXA16& Get_MatWorld();
	CBoundBox GetBoundBox()		{ return m_WeaponType.pBBA; }
	BOOL GetAtkTime()			{ return m_bAtkTime; }

	VOID AddAtkBBx( D3DXVECTOR3 &vPos, FLOAT fAngle );
	
	INT GetDirection();
private:
	enum { READ, WRITE };

	VOID PrivateProfile(BOOL = READ);
};