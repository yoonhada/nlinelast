#include "stdafx.h"

INT CPhysics::nVI[6][3] = 
{
	{ 3, 2, 6 },	// Front(3-2-6)
	{ 1, 0, 4 }, 	// Back	(1-0-4)
	{ 0, 3, 7 },	// Left	(0-3-7)
	{ 2, 1, 5 },	// Right(2-1-5)
	{ 0, 1, 2 },	// Up	(0-1-2)
	{ 4, 7, 6 },	// Down	(4-7-6)
};

CPhysics::CPhysics( void )
: m_vGAccel( 0.0f, 0.0108962777f, 0.0f )		///< 9.80665 * ( 1/30( f/s ) )^2
, m_fElastic( 0.667f )							///< 탄성도
, m_fAirRegistance( 0.99f )						///< 공기저항
, m_vColNormal(0.0f, 0.0f, 0.0f)				///< 충돌공간의 노멀
{
}

CPhysics::~CPhysics( void )
{
}

BOOL CPhysics::IntersectTri(const D3DXVECTOR3 &v0, 
							const D3DXVECTOR3 &v1, 
							const D3DXVECTOR3 &v2, 
							const D3DXVECTOR3 &vBegin, 
							const D3DXVECTOR3 &vDirection)
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

VOID CPhysics::Reflect( D3DXVECTOR3& _vOut, const D3DXVECTOR3& _vN, const D3DXVECTOR3& _vP )
{
	D3DXVec3Normalize( &vN, &_vN );
	// 2( ( N*L )N ) - L
	_vOut = 2.0f * ( D3DXVec3Dot( &-_vP, &vN ) * vN ) + _vP;
}

VOID CPhysics::Reflect( D3DXVECTOR3& _vOut )
{
	D3DXVECTOR3 vP = _vOut;
	D3DXVec3Normalize( &vN, &m_vColNormal );
	
	// 2( ( N*L )N ) - L
	_vOut = 2.0f * ( D3DXVec3Dot( &-vP, &vN ) * vN ) + vP;
}

VOID CPhysics::Sliding( D3DXVECTOR3& _vOut, const D3DXVECTOR3& _vN, const D3DXVECTOR3& _vP )
{
	D3DXVec3Normalize( &vN, &_vN );

	// P - n ( n * P )
	_vOut = _vP - D3DXVec3Dot( &_vP, &vN ) * vN;
}

VOID CPhysics::Sliding( D3DXVECTOR3& _vOut )
{
	D3DXVECTOR3 vP = _vOut;
	D3DXVec3Normalize( &vN, &m_vColNormal );

	// P - n ( n * P )
	_vOut = vP - D3DXVec3Dot( &vP, &vN ) * vN;
}

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

BOOL CPhysics::Collision( const D3DXVECTOR3 &vPos, CBoundBox *_pCube )
{
	D3DXPLANE Plane;
	D3DXVECTOR3 v[8];											//    v0----- v1
	for ( int i = 0; i < 8; ++i )								//   /|      /|
	{															//  v3------v2|
		v[i] = _pCube->GetPosition( i );						//  | |     | |
	}															//  | |v4---|-|v5
																//  |/      |/
	for (int i = 0; i < 6; ++i)									//  v7------v6
	{															// Plane A+B+C+D
		D3DXPlaneFromPoints( &Plane, 							// A = Nx		
							 &v[nVI[i][0]], 					// B = Ny
							 &v[nVI[i][1]], 					// C = Nz
							 &v[nVI[i][2]] );					// D = -(NdotX)	
		if (  D3DXPlaneDotCoord( &Plane, &vPos ) > 0 )
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CPhysics::Collision( const CBoundBox* _pCube1, const CBoundBox* _pCube2 )
{
	for ( int i = 0; i < 8; ++i)
	{
		if( CPhysics::GetInstance()->Collision( _pCube1->GetPosition(i), D3DXVECTOR3(0, 0, 0), _pCube2 ) )
		{
			return TRUE;
		}
	}
	for ( int i = 0; i < 8; ++i)
	{
		if( CPhysics::GetInstance()->Collision( _pCube2->GetPosition(i), D3DXVECTOR3(0, 0, 0), _pCube1 ) )
		{
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CPhysics::Collision( const CBoundBox* _pCube1, D3DXVECTOR3 &vDirection, const CBoundBox* _pCube2 )
{
	for ( int i = 0; i < 8; ++i)
	{
		if( CPhysics::GetInstance()->Collision( _pCube1->GetPosition(i), vDirection, _pCube2 ) )
		{
			return TRUE;
		}
	}
	for ( int i = 0; i < 8; ++i)
	{
		if( CPhysics::GetInstance()->Collision( _pCube2->GetPosition(i), -vDirection, _pCube1 ) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CPhysics::Collision( const D3DXVECTOR3 &vPosition, const D3DXVECTOR3 &vDirection, const CBoundBox *pBB )
{
	vDir = vPosition + vDirection;

	for ( i = 0; i < 8; ++i )
	{
		m_vBBPos[i] = pBB->GetPosition( i );
	}

	for ( i = 0; i < 6; ++i )
	{
		D3DXPlaneFromPoints(&m_plane, &m_vBBPos[nVI[i][0]], &m_vBBPos[nVI[i][1]], &m_vBBPos[nVI[i][2]] );	
		if (  D3DXPlaneDotCoord( &m_plane, &vDir ) <= 0 )
		{
			if ( D3DXPlaneDotCoord( &m_plane, &vPosition )  > 0 )
				m_vColNormal = D3DXVECTOR3( m_plane.a, m_plane.b, m_plane.c );

			continue;
		}

		return FALSE;
	}

	return TRUE;
}
