#include "stdafx.h"
#include "Qube.h"

#include "Charactor.h"

CQube::CQube()
: m_fItemLift( 0.01f )
, m_vMomentum( 0.0f, 0.0f, 0.0f )
, m_vAccelerate( 0.0f, 0.0f, 0.0f )
, m_bVisiable( TRUE )
, m_fSize( 0.5f )
, m_fHeight( 0.0f )
{
	//Create( _pd3dDevice, a_pVB, a_pIB, a_iStartVB, a_iStartIB, a_fCubeSize );
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ZeroMemory( m_vAxisDir, sizeof(m_vAxisDir) );
	m_vRotateTemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXMatrixIdentity( &m_matChara );
}

CQube::~CQube()
{
	//Release();
}

//VOID CQube::Update()
VOID CQube::Update( CBoundBox * pBB )
{
	// 속도를 위해 소멸속도 단순 계산
	FLOAT fLen = ABSDEF( m_vAccelerate.x ) + ABSDEF( m_vAccelerate.y ) + ABSDEF( m_vAccelerate.z );
	if ( m_fItemLift > fLen )
	{
		m_bVisiable = FALSE;
	}

	m_vAccelerate *= CPhysics::GetInstance()->m_fAirRegistance;		// 가속도 공기저항 감속
	
	vDir = CPhysics::GetInstance()->m_vGAccel * CFrequency::GetInstance()->getFrametime() ;

	m_vMomentum = m_vMomentum - vDir;

	// 이동값 갱신
	vDir = m_vPos + m_vAccelerate + m_vMomentum;
	vecBoundBox = CTree::GetInstance()->GetMapVector(CTree::GetInstance()->GetRoot(), vDir );
	if ( !( vecBoundBox == NULL || vecBoundBox->empty() ) )
	{
		vDir = m_vAccelerate + m_vMomentum;
		Iter = vecBoundBox->begin();
		while ( Iter != vecBoundBox->end() )
		{
			if( CPhysics::GetInstance()->Collision( m_vPos, vDir, ( *Iter ) ) )
			{
				CPhysics::GetInstance()->Reflect( m_vAccelerate );
				CPhysics::GetInstance()->Reflect( m_vMomentum );
				m_vMomentum *= CPhysics::GetInstance()->m_fElastic;
				m_vRotateTemp *= CPhysics::GetInstance()->m_fElastic;
			}
			Iter++;
		}
	}

	// 지면체크
	vDir = m_vPos + m_vAccelerate + m_vMomentum;
	if ( vDir.y < m_fSize )
	{
		vDir.y = 2.0f * m_fSize - vDir.y ;
		m_vMomentum.y *= -1.0f;
		m_vAccelerate.y *= -1.0f;

		//회전량의 방향성 추가		
		m_vRotateTemp.x = -m_vAccelerate.z * CPhysics::GetInstance()->m_fElastic;
		m_vRotateTemp.z = -m_vAccelerate.x * CPhysics::GetInstance()->m_fElastic;

		m_vMomentum *= CPhysics::GetInstance()->m_fElastic;
		m_vRotateTemp *= CPhysics::GetInstance()->m_fElastic;
		vDir = m_vPos + m_vMomentum;

		if (m_vAccelerate.y < 0.5)
			vDir.y = m_fSize;
	}

	m_vPos = vDir;

	m_vRotate += m_vRotateTemp;
	Set_ControlRotate( 0, m_vRotate.x );
	Set_ControlRotate( 1, m_vRotate.y );
	Set_ControlRotate( 2, m_vRotate.z );
	Set_ControlTranslate( 0, m_vPos.x );
	Set_ControlTranslate( 1, m_vPos.y );
	Set_ControlTranslate( 2, m_vPos.z );

	Calcul_MatWorld();
}

VOID CQube::RandMome( D3DXVECTOR3 vMomentum, FLOAT fRate )
{
	m_vAccelerate = D3DXVECTOR3( 
		( FastRand2() - 0.5f ) * fRate, 
		( FastRand2() - 0.5f ) * fRate, 
		( FastRand2() - 0.5f ) * fRate ) + vMomentum;
}

VOID CQube::SetRander()
{
	RanderPos();
	RanderRotate();
	RanderSize();
}

VOID CQube::RanderPos( FLOAT fRate )
{
	m_vPos = D3DXVECTOR3( 
		( FastRand2() - 0.5f ) * fRate, 
		( FastRand2()        ) * fRate, 
		( FastRand2() - 0.5f ) * fRate );
}

VOID CQube::RanderRotate( FLOAT fRate )
{
	m_vRotateTemp.x = m_vAccelerate.z;
	m_vRotateTemp.z = m_vAccelerate.x;
	//INT n = static_cast<INT>( FastRand2() * 3.0f );

	//switch( n )
	//{
	//case 0:
	//	m_vRotateTemp = D3DXVECTOR3( FastRand2() * fRate, FastRand2() * fRate, 0.0f );
	//	break;
	//case 1:
	//	m_vRotateTemp = D3DXVECTOR3( FastRand2() * fRate, 0.0f, FastRand2() * fRate );
	//	break;
	//case 2:
	//	m_vRotateTemp = D3DXVECTOR3( 0.0f, FastRand2() * fRate, FastRand2() * fRate );
	//	break;
	//}
}

VOID CQube::RanderSize( FLOAT fRate )
{
	m_fSize = FastRand2() * fRate;
}

// vector가 블럭 내에 있는지 체크.
BOOL CQube::InRect(D3DXVECTOR3 &_vec)
{
	BOOL bRet = FALSE;
	D3DXVECTOR3 vec[6];
	//D3DXVec3TransformCoord(&vec[0], &vAxisDir[0], &m_matWorld);
	//if (  )
	//{
	//}
	return bRet;
}