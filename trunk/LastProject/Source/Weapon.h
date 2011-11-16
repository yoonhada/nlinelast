/**
@file	CManageCube.h
@date	2011/09/20
@author	
@brief	���� Ŭ����
*/

#pragma once

class Map;
class CCube;

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
private:
	enum { READ, WRITE };

	LPDIRECT3DDEVICE9 m_pd3dDevice;
	INT m_nState;
	INT m_nReversing;
	WEAPONTYPE m_WeaponType;
	Map * m_pMap;
	BOOL m_bAtkTime;
	FLOAT m_fZAng[10];
	FLOAT m_fBBSize[6];

	VOID PrivateProfile(BOOL = READ);

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
	VOID UpdateSRT();

	VOID Render( D3DXMATRIX _matCharacter );
	VOID SetType(INT nType)		{ m_WeaponType.nType = nType; }
	VOID SetKeyA();
	VOID SetKeyB();
	VOID SetBBx( const D3DXVECTOR3& vPos, const FLOAT fAngle );

	const D3DXMATRIXA16& Get_MatWorld();
	CBoundBox GetBoundBox()		{ return m_WeaponType.pBBA; }
	BOOL GetAtkTime()			{ return m_bAtkTime; }

	VOID AddAtkBBx( D3DXVECTOR3 &vPos, FLOAT fAngle );
	
	INT Get_nFrame();
	INT Get_nState();
	INT Get_Direction();
	VOID Set_Animation( INT _nState );
	VOID Set_nState( INT _nState );

	// Debug
	LPDIRECT3DVERTEXBUFFER9 m_pTotalVB;   ///< ť�� ���� ���ؽ�
	LPDIRECT3DINDEXBUFFER9  m_pTotalIB;   ///< ť�� �ε���
	CCube * m_pCube;
};