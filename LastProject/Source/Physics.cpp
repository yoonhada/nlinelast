#include "stdafx.h"

CPhysics::CPhysics( void )
: m_vGAccel( 0.0f, 0.0108962777f, 0.0f )		///< 9.80665 * ( 1/30( f/s ) )^2
, m_fElastic( 0.5f )							///< 탄성도
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
	D3DXVECTOR3 vN;
	D3DXVec3Normalize( &vN, &_vN );
	// 2( ( N*L )N ) - L
	_vOut = 2.0f * ( D3DXVec3Dot( &-_vP, &vN ) * vN ) + _vP;
}

VOID CPhysics::Reflect( D3DXVECTOR3& _vOut )
{
	D3DXVECTOR3 vP = _vOut;
	D3DXVECTOR3 vN;
	D3DXVec3Normalize( &vN, &m_vColNormal );
	
	// 2( ( N*L )N ) - L
	_vOut = 2.0f * ( D3DXVec3Dot( &-vP, &vN ) * vN ) + vP;
}

VOID CPhysics::Sliding( D3DXVECTOR3& _vOut, const D3DXVECTOR3& _vN, const D3DXVECTOR3& _vP )
{
	D3DXVECTOR3 vN;
	D3DXVec3Normalize( &vN, &_vN );

	// P - n ( n * P )
	_vOut = _vP - D3DXVec3Dot( &_vP, &vN ) * vN;
}

VOID CPhysics::Sliding( D3DXVECTOR3& _vOut )
{
	D3DXVECTOR3 vP = _vOut;
	D3DXVECTOR3 vN;
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
	FRECT rect = _pCube->GetFRect();

	if ( ( rect.left < vPos.x ) && ( vPos.x < rect.right ) &&
		( rect.bottom < vPos.z ) && ( vPos.z < rect.top ) )
		return TRUE;

	return FALSE;
}

BOOL CPhysics::Collision( const CBoundBox* _pCube1, const CBoundBox* _pCube2 )
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

BOOL CPhysics::Collision( const D3DXVECTOR3 &vBegin, const D3DXVECTOR3 &vDirection, const CBoundBox *_pCube )
{
	D3DXVECTOR3 vDir = vBegin + vDirection;
	D3DXVECTOR3 v0, v1, v2;
	D3DXVECTOR3 v[8];//0, v1, v2;
	//D3DXVECTOR3 vD;

	//    v0----- v1
	//   /|      /|
	//  v3------v2|
	//  | |     | |
	//  | |v4---|-|v5
	//  |/      |/
	//  v7------v6

	for ( int i = 0; i < 8; ++i )
	{
		v[i] = _pCube->GetPosition( i );
	}

	// Plane A+B+C+D
	// A = Nx
	// B = Ny
	// C = Nz
	// D = -(NdotX)

	INT nVI[6][3] = 
	{
		{ 3, 2, 6 },	// Front(3-2-6)
		{ 1, 0, 4 }, 	// Back	(1-0-4)
		{ 0, 3, 7 },	// Left	(0-3-7)
		{ 2, 1, 5 },	// Right(2-1-5)
		{ 0, 1, 2 },	// Up	(0-1-2)
		{ 4, 7, 6 },	// Down	(4-7-6)
	};

	INT nCount = 0;
	D3DXPLANE Plane;
	for (int i = 0; i < 6; ++i)
	{
		D3DXPlaneFromPoints(&Plane, &v[nVI[i][0]], &v[nVI[i][1]], &v[nVI[i][2]] );	
		if (  D3DXPlaneDotCoord( &Plane, &vDir ) <= 0 )
		{
			if ( D3DXPlaneDotCoord( &Plane, &vBegin )  > 0 )
				m_vColNormal = D3DXVECTOR3( Plane.a, Plane.b, Plane.c );

			nCount++;
		}
	}

	if (nCount == 6)	return TRUE;
	else				return FALSE;
}

BOOL CPhysics::Collision( const CBoundBox* _pCube1, D3DXVECTOR3 &vDirection, const CBoundBox* _pCube2 )
{
	std::vector<CBoundBox*> * vecBoundBox;
	std::vector<CBoundBox*>::iterator Iter;

	D3DXVECTOR3 vDir;
	for ( int i = 0; i < 8; ++i )
	{
		vDir = _pCube1->GetPosition(i) + vDirection;
		vecBoundBox = CTree::GetInstance()->GetMapVector(CTree::GetInstance()->GetRoot(), vDir);

		if ( vecBoundBox != NULL && vecBoundBox->size() )
		{
			Iter = vecBoundBox->begin();
			while ( Iter != vecBoundBox->end() )
			{
				if( CPhysics::GetInstance()->Collision( _pCube1->GetPosition(i), vDirection, ( *Iter ) ) )
				{
					CPhysics::GetInstance()->Sliding( vDirection );
				}
				Iter++;
			}
		}
	}

	for ( int i = 0; i < 8; ++i )
	{
		vDir = _pCube1->GetPosition(i) + vDirection;
		vecBoundBox = CTree::GetInstance()->GetMapVector(CTree::GetInstance()->GetRoot(), vDir);

		if ( vecBoundBox != NULL && vecBoundBox->size() )
		{
			Iter = vecBoundBox->begin();
			while ( Iter != vecBoundBox->end() )
			{
				if( CPhysics::GetInstance()->Collision( _pCube1->GetPosition(i), vDirection, ( *Iter ) ) )
				{
					CPhysics::GetInstance()->Sliding( vDirection );
				}
				Iter++;
			}
		}
	}
	//
	//BOOL CPhysics::Collision( const D3DXVECTOR3 &vBegin, const D3DXVECTOR3 &vDirection, const CBoundBox *_pCube )
	//	if( Collision(_pCube1->) || Collision() )
	//		return TRUE;
	//
	//	return FALSE;

	return TRUE;
}

