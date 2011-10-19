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
public:
	CPhysics( void );
	virtual ~CPhysics( void );

	VOID Reflect( D3DXVECTOR3& _vOut, const D3DXVECTOR3& _vN, const D3DXVECTOR3& _vP );
	VOID Sliding( D3DXVECTOR3& _vOut );
	VOID Sliding( D3DXVECTOR3& _vOut, const D3DXVECTOR3& _vN, const D3DXVECTOR3& _vP );

	BOOL Collision( const D3DXVECTOR3* _vCenter1, FLOAT _fRadius1, const D3DXVECTOR3* _vCenter2, FLOAT _fRadius2 );
	BOOL Collision( const CBoundBox* _pCube1, const CBoundBox* _pCube2 );
	BOOL Collision( const D3DXVECTOR3 &vBegin, const D3DXVECTOR3 &vDirection, const CBoundBox *_pCube );
	BOOL Collision( const CBoundBox* _pCube1, D3DXVECTOR3 &vDirection, const CBoundBox* _pCube2 );
	BOOL Collision( const D3DXVECTOR3 &vBegin, CBoundBox* _pCube2 );

	const D3DXVECTOR3 m_vGAccel;	///< 중력가속도
	const FLOAT m_fElastic;			///< 탄성계수
	const FLOAT m_fAirRegistance;	///< 공기저항계수
	D3DXVECTOR3 m_vColNormal;		///< 충돌면노말
	D3DXVECTOR3 m_vColPosition;		///< 충돌좌표
};