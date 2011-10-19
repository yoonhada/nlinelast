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

	const D3DXVECTOR3 m_vGAccel;	///< �߷°��ӵ�
	const FLOAT m_fElastic;			///< ź�����
	const FLOAT m_fAirRegistance;	///< �������װ��
	D3DXVECTOR3 m_vColNormal;		///< �浹��븻
	D3DXVECTOR3 m_vColPosition;		///< �浹��ǥ
};