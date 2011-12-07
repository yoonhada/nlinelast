#include "stdafx.h"

//    v0----- v1
//   /|      /|
//  v3------v2|
//  | |     | |
//  | |v4---|-|v5
//  |/      |/
//  v7------v6


INT CPhysics::nVI[2][6][3] = 
{
	{
		{ 3, 2, 6 },	// Front(3-2-6)
		{ 1, 0, 4 }, 	// Back	(1-0-4)
		{ 0, 3, 7 },	// Left	(0-3-7)
		{ 2, 1, 5 },	// Right(2-1-5)
		{ 0, 1, 2 },	// Up	(0-1-2)
		{ 4, 7, 6 },	// Down	(4-7-6)
	}, 
	{
		{ 3, 6, 7 },	// Front(3-6-7)
		{ 1, 4, 5 }, 	// Back	(1-4-5)
		{ 0, 7, 4 },	// Left	(0-7-4)
		{ 2, 5, 6 },	// Right(2-5-6)
		{ 0, 2, 3 },	// Up	(0-2-3)
		{ 4, 6, 5 },	// Down	(4-6-5)
	}
};

CPhysics::CPhysics( void )
//: m_vGAccel( 0.0f, 0.0108962777f, 0.0f )		///< 9.80665 * ( 1/30( f/s ) )^2
: m_vGAccel( 0.0f, 98.0665f, 0.0f )		
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

	if ( _vOut.x == 0 && _vOut.y == 0 && _vOut.z == 0 )
	{
		_vOut = vN * m_fD_s;
	}
	else
	{
		// P - n ( n * P )
		_vOut = vP - D3DXVec3Dot( &vP, &vN ) * vN;
	}
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

BOOL CPhysics::Collision( const D3DXVECTOR3 &vPos, CBoundBox *_pCube)
{
	D3DXPLANE Plane;											//    v0----- v1
	D3DXVECTOR3 v[8];											//   /|      /|
	for ( int i = 0; i < 8; ++i )								//  v3------v2|
	{															//  | |     | |
		v[i] = _pCube->GetPosition( i );						//  | |v4---|-|v5
	}															//  |/      |/
																//  v7------v6
	for (int i = 0; i < 6; ++i)									
	{															// Plane A+B+C+D
		D3DXPlaneFromPoints( &Plane, 							// A = Nx		
							 &v[nVI[0][i][0]], 					// B = Ny
							 &v[nVI[0][i][1]], 					// C = Nz
							 &v[nVI[0][i][2]] );					// D = -(NdotX)	
		if (  D3DXPlaneDotCoord( &Plane, &vPos ) > 0 )
		{
			return FALSE;
		}
	}

	return TRUE;
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
			m_vColNormal = -m_vColNormal;
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

	//CDebugConsole::GetInstance()->MessageQube(m_vBBPos);
	for ( i = 0; i < 6; ++i )
	{
		D3DXPlaneFromPoints(&m_plane, &m_vBBPos[nVI[0][i][0]], &m_vBBPos[nVI[0][i][1]], &m_vBBPos[nVI[0][i][2]] );	
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

BOOL CPhysics::Collision( const D3DXVECTOR3 &vCenter, FLOAT fRadius, const CBoundBox * pBB )
{
	for ( i = 0; i < 8; ++i )
	{
		m_vBBPos[i] = pBB->GetPosition( i );
	}

	for ( i = 0; i < 4; ++i )
	{
		// 최단거리 계산 Ds = ( Pi - Ps ) DOT N
		D3DXPlaneFromPoints(&m_plane, &m_vBBPos[nVI[0][i][0]], &m_vBBPos[nVI[0][i][1]], &m_vBBPos[nVI[0][i][2]]);
		m_vColNormal = D3DXVECTOR3( m_plane.a, m_plane.b, m_plane.c );

		v0 = vCenter - m_vBBPos[nVI[0][i][0]];
		m_fD_s = D3DXVec3Dot( &v0, &m_vColNormal );
		
		if ( m_fD_s < fRadius )
		{
			CDebugConsole::GetInstance()->Messagef("%d : %0.2f\n", i, m_fD_s);
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CPhysics::Collision( INT nIndex, const D3DXVECTOR3 &vPosition, const D3DXVECTOR3 &vDirection, const CBoundBox *_pCube )
{
	D3DXVECTOR3 v[8];							
	for ( int i = 0; i < 8; ++i )				
	{											
		v[i] = _pCube->GetPosition( i );		
	}											

	BOOL bRet;
	bRet = IntersectTri ( v[nVI[0][nIndex][0]], v[nVI[0][nIndex][1]], v[nVI[0][nIndex][2]], vPosition, vDirection );
	if (bRet == TRUE)
	{
		return TRUE;
	}
	bRet = IntersectTri ( v[nVI[1][nIndex][0]], v[nVI[1][nIndex][1]], v[nVI[1][nIndex][2]], vPosition, vDirection );

	return bRet;
}