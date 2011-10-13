#include "stdafx.h"

CPhysics::CPhysics( void )
// : m_vGAccel( 0.0f, 0.0027240694f, 0.0f )		///< 9.80665 * ( 1/60( f/s ) )^2
: m_vGAccel( 0.0f, 0.0108962777f, 0.0f )		///< 9.80665 * ( 1/30( f/s ) )^2
, m_fElastic( 0.5f )							///< 탄성도
, m_fAirRegistance( 0.99f )					///< 공기저항
{
}

CPhysics::~CPhysics( void )
{
}

VOID CPhysics::Reflect( D3DXVECTOR3& _vOut, D3DXVECTOR3& _vN, D3DXVECTOR3& _vP )
{
	D3DXVECTOR3 vN;
	D3DXVec3Normalize( &vN, &_vN );
	// 2( ( N*L )N ) - L
	_vOut = 2.0f * ( D3DXVec3Dot( &-_vP, &vN ) * vN ) + _vP;
}

///< Point To Point
//BOOL CPhysics::Collision(D3DXVECTOR3& V1, D3DXVECTOR3& V2, FLOAT fEpsilon=0.0001f )
//{
//	BOOL bRet = FALSE;
//
//	if( fabsf(V2.x - V1.x) < fEpsilon &&
//		fabsf(V2.y - V1.y) < fEpsilon &&
//		fabsf(V2.z - V1.z) < fEpsilon )
//		bRet = TRUE;
//
//	return bRet;
//}

///< Sphere To Sphere
BOOL CPhysics::Collision(const D3DXVECTOR3* SphereCenter1, FLOAT sphereRadius1, 
						 const D3DXVECTOR3* SphereCenter2, FLOAT sphereRadius2 )
{
	BOOL bRet = FALSE;
	FLOAT fDistance;

	D3DXVECTOR3 vcTemp = *SphereCenter1 - *SphereCenter2;

	fDistance = D3DXVec3Length( &vcTemp );

	if( fDistance <= ( sphereRadius1 + sphereRadius2 ) )
		bRet = TRUE;

	return bRet;
}

///< Line to Plane
BOOL CPhysics::Collision( D3DXVECTOR3& vPos, D3DXVECTOR3& vMon, D3DXPLANE& plane )
{
	// vPos 현재좌표
	// vMon 이동후 좌표
	// plane 충돌검출 평면
	// vOut 출력할 반사벡터
	FLOAT D1 = D3DXPlaneDotCoord( &plane, &vPos );
	FLOAT D2 = D3DXPlaneDotCoord( &plane, &vMon );

	if ( D1 * D2 <= 0 )		
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CPhysics::Collision( D3DXVECTOR3 &vPos, CBoundBox *_pCube )
{
	RECT rect = _pCube->GetRect();

	if ( ( rect.left < vPos.x ) && ( vPos.x < rect.right ) &&
		 ( rect.bottom < vPos.y ) && ( vPos.y < rect.top ) )
		 return TRUE;

	return FALSE;
}

BOOL CPhysics::Collision( CBoundBox* _pCube1, CBoundBox* _pCube2 )
{
	RECT rect1 = _pCube1->GetRect();
	RECT rect2 = _pCube2->GetRect();

	if (rect1.right <= rect2.left || rect1.left >= rect2.right ) return FALSE;
	if (rect1.top <= rect2.bottom || rect1.bottom >= rect2.top ) return FALSE;

	return TRUE;
}