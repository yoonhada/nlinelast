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
	INT nType;
	INT nFrameBegin[10];
	INT nFrameTime[10];
	INT nFrameAtk[10];
	INT nDelay[10];
	D3DXVECTOR3 vDir[10];

	CBoundBox pBBA;

	VOID AKeyBB( const D3DXVECTOR3& vPos, const FLOAT fAngle )
	{
		D3DXVECTOR3 vMax( 9.0f, 0.5f,  4.0f );
		D3DXVECTOR3 vMin(-9.0f,-0.5f, 20.0f );
		
		pBBA.SetPosition( vPos );
		pBBA.SetAngle( fAngle );

		pBBA.SetSize( 0, vMin.x );
		pBBA.SetSize( 1, vMin.y );
		pBBA.SetSize( 2, vMin.z );

		pBBA.SetSize( 3, vMax.x );
		pBBA.SetSize( 4, vMax.y );
		pBBA.SetSize( 5, vMax.z );

		pBBA.SetDirection( vDir[0] );
	}
	VOID BKeyBB( const D3DXVECTOR3& vPos, const FLOAT fAngle )
	{
		D3DXVECTOR3 vMax( 0.5f,  9.0f, 4.0f );
		D3DXVECTOR3 vMin(-0.5f, -9.0f, 20.0f );

		pBBA.SetPosition( vPos );
		pBBA.SetAngle( fAngle );

		pBBA.SetSize( 0, vMin.x );
		pBBA.SetSize( 1, vMin.y );
		pBBA.SetSize( 2, vMin.z );

		pBBA.SetSize( 3, vMax.x );
		pBBA.SetSize( 4, vMax.y );
		pBBA.SetSize( 5, vMax.z );

		pBBA.SetDirection( vDir[1] );
	}

}WEAPONTYPE;

class CWeapon : public IObject
{
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	INT m_nState;
	INT m_nFrame;
	WEAPONTYPE m_WeaponType;
	Map * m_pMap;

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
	VOID SetKeyA( const D3DXVECTOR3& vPos, const FLOAT fAngle );
	VOID SetKeyB( const D3DXVECTOR3& vPos, const FLOAT fAngle );

	const D3DXMATRIXA16& Get_MatWorld();

	CBoundBox GetBoundBox()		{ return m_WeaponType.pBBA; }
};