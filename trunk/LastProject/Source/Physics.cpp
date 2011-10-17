#include "stdafx.h"

CPhysics::CPhysics( void )
// : m_vGAccel( 0.0f, 0.0027240694f, 0.0f )		///< 9.80665 * ( 1/60( f/s ) )^2
: m_vGAccel( 0.0f, 0.0108962777f, 0.0f )		///< 9.80665 * ( 1/30( f/s ) )^2
, m_fElastic( 0.5f )							///< 탄성도
, m_fAirRegistance( 0.99f )						///< 공기저항
, m_vColNormal(0.0f, 0.0f, 0.0f)				///< 충돌공간의 노멀
{
}

CPhysics::~CPhysics( void )
{
}

BOOL CPhysics::IntersectTri(D3DXVECTOR3 &v0, D3DXVECTOR3 &v1, D3DXVECTOR3 &v2, 
							const D3DXVECTOR3 &vBegin, const D3DXVECTOR3 &vDirection)
{
	BOOL bColl = FALSE;
	FLOAT fu, fv, fd;

	bColl = D3DXIntersectTri(&v0, &v1, &v2, &vBegin, &vDirection, &fu, &fv, &fd);

	if (bColl)
	{
		D3DXVec3Cross( &m_vColNormal, &(v1 - v0), &(v2 - v0) );
		D3DXVec3Normalize(&m_vColNormal, &m_vColNormal);
		m_vColPosition = v0 + fu * ( v1 - v0 ) + fv * ( v2 - v0 );
		//m_vcPick = V0 + u * (V1 - V0) + v * (V2 - V0);
	}

	return bColl;
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
//BOOL CPhysics::Collision( D3DXVECTOR3& vPos, D3DXVECTOR3& vMon, D3DXPLANE& plane )
//{
//	// vPos 현재좌표
//	// vMon 이동후 좌표
//	// plane 충돌검출 평면
//	// vOut 출력할 반사벡터
//	FLOAT D1 = D3DXPlaneDotCoord( &plane, &vPos );
//	FLOAT D2 = D3DXPlaneDotCoord( &plane, &vMon );
//
//	if ( D1 * D2 <= 0 )		
//	{
//		return TRUE;
//	}
//
//	return FALSE;
//}
//
BOOL CPhysics::Collision( const D3DXVECTOR3 &vPos, CBoundBox *_pCube )
{
	FRECT rect = _pCube->GetFRect();

	if ( ( rect.left < vPos.x ) && ( vPos.x < rect.right ) &&
		 ( rect.bottom < vPos.z ) && ( vPos.z < rect.top ) )
		 return TRUE;

	return FALSE;
}

BOOL CPhysics::Collision( CBoundBox* _pCube1, const D3DXVECTOR3 &vDirection, CBoundBox* _pCube2 )
{
	FLOAT fDistance, fRadius, fRadius1, fRadius2;
	D3DXVECTOR3 vDistance = _pCube2->GetPosition() - ( _pCube1->GetPosition() + vDirection );
	fDistance = vDistance.x * vDistance.x + vDistance.y * vDistance.y + vDistance.z * vDistance.z;
	fRadius1 = _pCube1->GetRadiusLong();
	fRadius2 = _pCube2->GetRadiusLong();
	fRadius =	fRadius1 * fRadius1 + fRadius1 * fRadius1 + fRadius1 * fRadius1 + 
				fRadius2 * fRadius2 + fRadius2 * fRadius2 + fRadius2 * fRadius2;
	
	if ( fDistance >= fRadius ) 
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CPhysics::Collision( CBoundBox* _pCube1, CBoundBox* _pCube2 )
{
	const FLOAT fCutOff = 0.999999f; 
	D3DXVECTOR3 vDiff = _pCube1->GetPosition() - _pCube2->GetPosition(); 

	FLOAT c[3][3];
	FLOAT absc[3][3];
	FLOAT d[3];
	FLOAT r, r1, r2;


	for( int i = 0 ; i < 3 ; ++i ) 
	{ 
		c[0][i] = D3DXVec3Dot( &(_pCube1->GetAxisDir( 0 )), &(_pCube2->GetAxisDir( i )) ); 
		absc[0][i] = ABSDEF( c[0][i] ); 
	} 
	
	d[0] = D3DXVec3Dot( &vDiff, &_pCube1->GetAxisDir( 0 ) ); 
	r = ABSDEF(d[0]);
	r1 = _pCube1->GetSize(3);
	r2 = _pCube2->GetSize(3) * absc[0][0] + _pCube2->GetSize(4) * absc[0][1] + _pCube2->GetSize(5) * absc[0][2];

	if ( r > r1 + r2 )
		return FALSE;

	for( int i = 0 ; i < 3 ; ++i ) 
	{ 
		c[2][i] = D3DXVec3Dot( &(_pCube1->GetAxisDir( 2 )), &(_pCube2->GetAxisDir( i )) ); 
		absc[2][i] = ABSDEF( c[2][i] ); 
	} 

	d[2] = D3DXVec3Dot( &vDiff, &_pCube1->GetAxisDir( 2 ) ); 
	r = ABSDEF(d[2]);
	r1 = _pCube1->GetSize(3);
	r2 = _pCube2->GetSize(3) * absc[2][2] + _pCube2->GetSize(4) * absc[2][1] + _pCube2->GetSize(5) * absc[2][2];

	if ( r > r1 + r2 )
		return FALSE;

	return TRUE;
}

BOOL CPhysics::Collision( const D3DXVECTOR3 &vBegin, const D3DXVECTOR3 &vDirection, CBoundBox *_pCube )
{
	D3DXVECTOR3 v0, v1, v2;

	// Front
	v0 = _pCube->GetPosition( 3 );
	v1 = _pCube->GetPosition( 2 );
	v2 = _pCube->GetPosition( 6 );
	if( IntersectTri(v0, v1, v2, vBegin, vDirection) )
		return TRUE;
	//v0 = _pCube->GetPosition( 3 );
	v1 = _pCube->GetPosition( 6 );
	v2 = _pCube->GetPosition( 7 );
	if( IntersectTri(v0, v1, v2, vBegin, vDirection) )
		return TRUE;

	// Back
	v0 = _pCube->GetPosition( 0 );
	v1 = _pCube->GetPosition( 4 );
	v2 = _pCube->GetPosition( 5 );
	if( IntersectTri(v0, v1, v2, vBegin, vDirection) )
		return TRUE;
	//v0 = _pCube->GetPosition( 0 );
	v1 = _pCube->GetPosition( 5 );
	v2 = _pCube->GetPosition( 1 );
	if( IntersectTri(v0, v1, v2, vBegin, vDirection) )
		return TRUE;

	// Left
	v0 = _pCube->GetPosition( 0 );
	v1 = _pCube->GetPosition( 3 );
	v2 = _pCube->GetPosition( 7 );
	if( IntersectTri(v0, v1, v2, vBegin, vDirection) )
		return TRUE;
	//v0 = _pCube->GetPosition( 0 );
	v1 = _pCube->GetPosition( 7 );
	v2 = _pCube->GetPosition( 4 );
	if( IntersectTri(v0, v1, v2, vBegin, vDirection) )
		return TRUE;

	// Right
	v0 = _pCube->GetPosition( 1 );
	v1 = _pCube->GetPosition( 5 );
	v2 = _pCube->GetPosition( 6 );
	if( IntersectTri(v0, v1, v2, vBegin, vDirection) )
		return TRUE;
	//v0 = _pCube->GetPosition( 1 );
	v1 = _pCube->GetPosition( 6 );
	v2 = _pCube->GetPosition( 2 );
	if( IntersectTri(v0, v1, v2, vBegin, vDirection) )
		return TRUE;

	// Up
	v0 = _pCube->GetPosition( 0 );
	v1 = _pCube->GetPosition( 1 );
	v2 = _pCube->GetPosition( 2 );
	if( IntersectTri(v0, v1, v2, vBegin, vDirection) )
		return TRUE;
	//v0 = _pCube->GetPosition( 0 );
	v1 = _pCube->GetPosition( 2 );
	v2 = _pCube->GetPosition( 3 );
	if( IntersectTri(v0, v1, v2, vBegin, vDirection) )
		return TRUE;

	// Down
	v0 = _pCube->GetPosition( 4 );
	v1 = _pCube->GetPosition( 7 );
	v2 = _pCube->GetPosition( 6 );
	if( IntersectTri(v0, v1, v2, vBegin, vDirection) )
		return TRUE;
	//v0 = _pCube->GetPosition( 4 );
	v1 = _pCube->GetPosition( 6 );
	v2 = _pCube->GetPosition( 5 );
	if( IntersectTri(v0, v1, v2, vBegin, vDirection) )
		return TRUE;



	return FALSE;
}