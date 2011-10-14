#pragma once

class CQube;
class CPhysics : public CSingleton<CPhysics>
{
	friend class CSingleton<CPhysics>;

	BOOL IntersectTri(D3DXVECTOR3 &, D3DXVECTOR3 &, D3DXVECTOR3 &, const D3DXVECTOR3 &, const D3DXVECTOR3 &);
public:
	CPhysics( void );
	virtual ~CPhysics( void );

	VOID Reflect( D3DXVECTOR3& vOut, D3DXVECTOR3& vN, D3DXVECTOR3& vP );
	//BOOL Collision( D3DXVECTOR3& V1, D3DXVECTOR3& V2, FLOAT fEpsilon );
	//BOOL Collision( D3DXVECTOR3& vPos, D3DXVECTOR3& vMon, D3DXPLANE& plane );

	BOOL Collision(const D3DXVECTOR3* SphereCenter1, FLOAT sphereRadius1, const D3DXVECTOR3* SphereCenter2, FLOAT sphereRadius2 );
	BOOL Collision( CBoundBox* _pCube1, CBoundBox* _pCube2 );
	BOOL Collision( const D3DXVECTOR3 &vBegin, const D3DXVECTOR3 &vDirection, CBoundBox *_pCube );
	BOOL Collision( CBoundBox* _pCube1, const D3DXVECTOR3 &vDirection, CBoundBox* _pCube2 );
	BOOL Collision( const D3DXVECTOR3& vPos, CBoundBox *_pCube );

	const D3DXVECTOR3 m_vGAccel;	///< �߷°��ӵ�
	const FLOAT m_fElastic;			///< ź�����
	const FLOAT m_fAirRegistance;	///< �������װ��
	D3DXVECTOR3 m_vColNormal;		///< �浹��븻
	D3DXVECTOR3 m_vColPosition;		///< �浹��ǥ
};