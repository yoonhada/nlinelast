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
	INT nAKeyFrameBegin;
	INT nAKeyFrameTime;
	INT nBKeyFrameBegin;
	INT nBKeyFrameTime;
	INT nWeaponType;
	INT nDelay;

	CBoundBox pBBA;

	VOID AKeyBB( const D3DXVECTOR3& vPos )
	{
		pBBA.SetPosition( vPos );
		pBBA.SetSize( 0, -9.0f );
		pBBA.SetSize( 1, -0.5f );
		pBBA.SetSize( 2,  2.0f );

		pBBA.SetSize( 3,  9.0f);
		pBBA.SetSize( 4,  0.5f );
		pBBA.SetSize( 5,  4.0f );
	}
	VOID BKeyBB( const D3DXVECTOR3& vPos )
	{
		pBBA.SetPosition( vPos );
		pBBA.SetSize( 0, -9.0f );
		pBBA.SetSize( 1, -0.5f );
		pBBA.SetSize( 2,  2.0f );

		pBBA.SetSize( 3,  9.0f);
		pBBA.SetSize( 4,  0.5f );
		pBBA.SetSize( 5,  4.0f );
	}

}WEAPONTYPE;

class CWeapon : public IObject
{
private:
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
	VOID SetType(INT nType)		{ m_WeaponType.nWeaponType = nType; }
	VOID SetKeyA( const D3DXVECTOR3& vPos );
	VOID SetKeyB( const D3DXVECTOR3& vPos );

	const D3DXMATRIXA16& Get_MatWorld();

	CBoundBox GetBoundBox()		{ return m_WeaponType.pBBA; }
};