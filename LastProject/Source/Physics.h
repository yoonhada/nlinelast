#pragma once

class CQube;
class CPhysics : public CSingleton<CPhysics>
{
	friend class CSingleton<CPhysics>;

	BOOL IntersectTri(  const D3DXVECTOR3 &, 
		const D3DXVECTOR3 &, 
		const D3DXVECTOR3 &, 
		const D3DXVECTOR3 &, 
		const D3DXVECTOR3 &);

	// 내부 속도 증강을 위한 변수
	INT i;
	INT m_nCount;
	FLOAT m_fD_s;
	BOOL m_bRet;
	D3DXVECTOR3 m_vBBPos[8];
	D3DXPLANE m_plane;

	static INT nVI[2][6][3];
	D3DXVECTOR3 vN, vDir;
	D3DXVECTOR3 v0, v1, v2;

public:
	const D3DXVECTOR3 m_vGAccel;	///< 중력가속도
	const FLOAT m_fElastic;			///< 탄성계수
	const FLOAT m_fAirRegistance;	///< 공기저항계수
	D3DXVECTOR3 m_vColNormal;		///< 충돌면노말
	D3DXVECTOR3 m_vColPosition;		///< 충돌좌표

	CPhysics( void );
	virtual ~CPhysics( void );


	VOID Reflect( D3DXVECTOR3& _vOut );
	//VOID Reflect( D3DXVECTOR3& _vOut, const D3DXVECTOR3& _vN, const D3DXVECTOR3& _vP );
	//VOID Sliding( D3DXVECTOR3& _vOut );
	//VOID Sliding( D3DXVECTOR3& _vOut, const D3DXVECTOR3& _vN, const D3DXVECTOR3& _vP );

	//BOOL Collision( const D3DXVECTOR3 &vBegin, CBoundBox* _pCube2 );

	//BOOL Collision( const CBoundBox* _pCube1, const CBoundBox* _pCube2 );

	// 111025
	BOOL Collision( const D3DXVECTOR3 &vPosition, FLOAT fRadius, const D3DXVECTOR3 &vBegin, const D3DXVECTOR3 &vEnd );
	BOOL Collision( const D3DXVECTOR3* _vCenter1, FLOAT _fRadius1, const D3DXVECTOR3* _vCenter2, FLOAT _fRadius2 );
	BOOL Collision( const CBoundBox* _pCube1, D3DXVECTOR3 &vDirection, const CBoundBox* _pCube2 );	
	BOOL Collision( const D3DXVECTOR3 &vPosition, const D3DXVECTOR3 &vDirection, const CBoundBox *_pCube );
	BOOL Collision( const D3DXVECTOR3 &vCenter, FLOAT fRadius, const CBoundBox *pBB );
};