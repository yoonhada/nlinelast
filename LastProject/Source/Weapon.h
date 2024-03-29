/**
@file	CManageCube.h
@date	2011/09/20
@author	
@brief	무기 클래스
*/

#pragma once

class Map;
class CCube;

typedef struct _WEAPONTYPE
{
	enum { CURRENTFRAME, FRAMEBEGIN, FRAMETIME, FRAMEATK, DELAY };
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

	LPDIRECT3DDEVICE9 m_pD3dDevice;
	INT m_nType;
	INT m_nState;
	INT m_nReversing;
	WEAPONTYPE m_WeaponType;
	Map * m_pMap;
	BOOL m_bAtkTime, m_bAtkTimeReset;
	FLOAT m_fScale;
	FLOAT m_fZAng[4][10];
	FLOAT m_fBBSize[4][6];

	VOID _GetProfileInt( INT *nOut, LPWSTR lpAppName, LPWSTR lpszBuf, INT nIndex, INT nDefault );
	VOID _GetProfileVector( D3DXVECTOR3 *vVec, LPWSTR lpAppName, LPWSTR lpszBuf, INT, INT, INT, INT, FLOAT );
	VOID PrivateProfile( LPWSTR lpwStr, BOOL = READ);
	//VOID LoadAniFrame( LPWSTR lpwStr );

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
	//VOID SetBBx( const D3DXVECTOR3& vPos, const FLOAT fAngle );
	VOID SetBBx( const FLOAT fAngle, D3DXMATRIX& _matCharacter);

	const D3DXMATRIXA16& Get_MatWorld();
	CBoundBox GetBoundBox()		{ return m_WeaponType.pBBA; }
	BOOL GetAtkTime()			{ return m_bAtkTime; }

	VOID AddAtkBBx( FLOAT fAngle, D3DXMATRIX& _matCharacter );
	
	VOID SetWeaponAni();
	INT Get_nFrame(INT = 0);
	INT Get_nState();
	D3DXVECTOR3 Get_Direction();
	VOID Set_Animation( INT _nState );
	VOID Set_nState( INT _nState );

#ifdef _DEBUG
	// Debug
	LPDIRECT3DVERTEXBUFFER9 m_pTotalVB;   ///< 큐브 통합 버텍스
	LPDIRECT3DINDEXBUFFER9  m_pTotalIB;   ///< 큐브 인덱스
	CCube * m_pCube;
#endif // _DEBUG
};