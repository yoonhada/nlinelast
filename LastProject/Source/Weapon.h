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

	class CGuideLine : public IObject
	{
	private:
		struct VERTEX
		{
			enum { FVF = ( D3DFVF_XYZ | D3DFVF_DIFFUSE ), };
			D3DXVECTOR3 _pos;
			DWORD _col;
		};

	protected:
		LPDIRECT3DDEVICE9		m_pd3dDevice;	///< D3D 디바이스
		LPDIRECT3DVERTEXBUFFER9	m_pVB;			///< 정점 버퍼
		FLOAT * m_pBBSize;

		HRESULT	InitVB( ) {
			UINT uLength = 6 * sizeof( VERTEX );
			if ( FAILED( m_pd3dDevice->CreateVertexBuffer( uLength, 0, VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL ) ) )
			{
				Release();
				return E_FAIL;
			}

			VERTEX* pVertices = NULL;
			if ( FAILED( m_pVB->Lock( 0, uLength, ( VOID** )&pVertices, 0 ) ) )
			{
				Release();
				return E_FAIL;
			}

			// TOP
			pVertices[0]._pos = D3DXVECTOR3( m_pBBSize[0], m_pBBSize[4], m_pBBSize[5] );		pVertices[0]._col = 0xFF00FF00;
			pVertices[1]._pos = D3DXVECTOR3( m_pBBSize[3], m_pBBSize[4], m_pBBSize[5] );		pVertices[1]._col = 0xFF00FF00;
			pVertices[2]._pos = D3DXVECTOR3( m_pBBSize[3], m_pBBSize[4], m_pBBSize[2] );		pVertices[2]._col = 0xFF00FF00;		
			pVertices[3]._pos = D3DXVECTOR3( m_pBBSize[0], m_pBBSize[4], m_pBBSize[5] );		pVertices[0]._col = 0xFF00FF00;
			pVertices[4]._pos = D3DXVECTOR3( m_pBBSize[3], m_pBBSize[4], m_pBBSize[2] );		pVertices[2]._col = 0xFF00FF00;
			pVertices[5]._pos = D3DXVECTOR3( m_pBBSize[0], m_pBBSize[4], m_pBBSize[2] );		pVertices[3]._col = 0xFF00FF00;

			//pVertices[4]._pos = D3DXVECTOR3( m_pBBSize[0], m_pBBSize[1], m_pBBSize[5] );		pVertices[4]._col = 0xFF00FF00;
			//pVertices[5]._pos = D3DXVECTOR3( m_pBBSize[3], m_pBBSize[1], m_pBBSize[5] );		pVertices[5]._col = 0xFF00FF00;
			//pVertices[6]._pos = D3DXVECTOR3( m_pBBSize[3], m_pBBSize[1], m_pBBSize[2] );		pVertices[6]._col = 0xFF00FF00;
			//pVertices[7]._pos = D3DXVECTOR3( m_pBBSize[0], m_pBBSize[1], m_pBBSize[2] );		pVertices[7]._col = 0xFF00FF00;

			m_pVB->Unlock();
			return S_OK;
		}

	public:
		CGuideLine(LPDIRECT3DDEVICE9 _pd3dDevice) : m_pd3dDevice(_pd3dDevice) , m_pVB(NULL)	{ }
		virtual ~CGuideLine() { Release(); }


		VOID Clear()		{}
		HRESULT Create()	{ return InitVB(); }
		HRESULT Release()	{ SAFE_RELEASE( m_pVB ); return S_OK; }
		VOID Update()		{ }
		VOID Render()		{ }
		VOID Render( D3DXMATRIX& matWorld )
		{
			m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
			m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

			m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof( VERTEX ) );
			m_pd3dDevice->SetFVF( VERTEX::FVF );

			m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 3 );
			m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
		}

		HRESULT Create( FLOAT fRange, INT nDeep );
		VOID SetpBBSize( FLOAT * aBBSize )	{ };
	};

	CGuideLine * m_pGL;

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
};