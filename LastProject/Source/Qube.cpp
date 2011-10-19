#include "stdafx.h"
#include "Qube.h"

#include "Charactor.h"

CQube::CQube()
: m_fItemLift( 0.005f )
, m_vMomentum( 0.0f, 0.0f, 0.0f )
, m_vAccelerate( 0.0f, 0.0f, 0.0f )
, m_nType( 0 )
, m_bVisiable( TRUE )
, m_fSize( 0.5f )
, m_fHeight( 0.0f )
{
	//Create( _pd3dDevice, a_pVB, a_pIB, a_iStartVB, a_iStartIB, a_fCubeSize );
	m_fLongSize = pow(m_fSize, 0.3333333333f);
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ZeroMemory( &m_vAxisDir, sizeof(m_vAxisDir) );
	m_vRotateTemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity( &m_matChara );
}

CQube::~CQube()
{
	//Release();
}

VOID CQube::Update()
{
	D3DXVECTOR3 vDir;
	std::vector<CBoundBox*> * vecBoundBox;
	std::vector<CBoundBox*>::iterator Iter;

	// �ӵ��� ���� �Ҹ�ӵ� �ܼ� ���
	FLOAT fLen = ABSDEF( m_vAccelerate.x ) + ABSDEF( m_vAccelerate.y ) + ABSDEF( m_vAccelerate.z );
	if ( m_fItemLift > fLen )
	{
		m_bVisiable = FALSE;
		return;
	}

	// ���ӵ� ����
	m_vAccelerate *= CPhysics::GetInstance()->m_fAirRegistance;
	m_vMomentum -= CPhysics::GetInstance()->m_vGAccel;

	// �̵��� ��ġ ����
	vDir = /*m_vPos + */m_vAccelerate + m_vMomentum;

	vecBoundBox = CTree::GetInstance()->GetMapVector(CTree::GetInstance()->GetRoot(), m_vPos + vDir);
	if ( vecBoundBox != NULL && vecBoundBox->size() )
	{
		Iter = vecBoundBox->begin();
		while ( Iter != vecBoundBox->end() )
		{
			if( CPhysics::GetInstance()->Collision( m_vPos, vDir, ( *Iter ) ) )
			{
				CPhysics::GetInstance()->Reflect( vDir );
			}
			Iter++;
		}
	}

	vDir += m_vPos;
	//		if ( CPhysics::GetInstance()->Collision( this, *iterCube ) )
	//		{
	//			D3DXVECTOR3 vec = ( *iterCube )->GetPosition() - m_vPos;
	//			CPhysics::GetInstance()->Reflect( m_vMomentum, vec, m_vMomentum );
	//			m_vMomentum *= CPhysics::GetInstance()->m_fElastic;
	//			m_vRotateTemp *= CPhysics::GetInstance()->m_fElastic;
	//			vec = m_vPos + m_vMomentum;
	//		}

	//		iterCube++;
	//	}
	//}

	// ����üũ
	if (vDir.y < 0.0f /* - ( m_fHeight - 2.0f )*/ )
	{
		vDir.y = m_fLongSize -vDir.y;
		m_vMomentum.y *= -1.0f;
		m_vAccelerate.y *= -1.0f;
		m_vMomentum *= CPhysics::GetInstance()->m_fElastic;
		m_vRotateTemp *= CPhysics::GetInstance()->m_fElastic;
		vDir = m_vPos + m_vMomentum;

		//ȸ������ ���⼺ �߰�		
		m_vRotateTemp.x = -( m_vAccelerate + m_vMomentum ).z * CPhysics::GetInstance()->m_fElastic;
		m_vRotateTemp.z = -( m_vAccelerate + m_vMomentum ).x * CPhysics::GetInstance()->m_fElastic;
	}

	m_vPos = vDir;

	//D3DXMATRIXA16 mat;
	//D3DXMatrixIdentity( &m_matWorld );
	m_vRotate += m_vRotateTemp;
	Set_ControlRotate( 0, m_vRotate.x );
	Set_ControlRotate( 1, m_vRotate.y );
	Set_ControlRotate( 2, m_vRotate.z );
	Set_ControlTranslate( 0, m_vPos.x );
	Set_ControlTranslate( 1, m_vPos.y );
	Set_ControlTranslate( 2, m_vPos.z );

	Calcul_MatWorld();

	//m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );
}

//VOID CQube::Translate()
//{
//	
//}
//
//BOOL CQube::Translate( std::list<CCube*> &_listCube )
//{
//	float fLength;
//	D3DXVECTOR3 vec, vN, vP;
//	std::list <CCube*>::iterator Iter;
//	for ( Iter = _listCube.begin(); Iter != _listCube.end(); Iter++ )
//	{
//		vec = ( *Iter )->GetPosition() - m_vPos;
//		fLength = D3DXVec3Length( &vec );
//		if ( fLength < m_fSize + ( *Iter )->GetSize() )
//		{
//			CPhysics::GetInstance()->Reflect( m_vMomentum, vec, m_vMomentum );
//		}		
//	}
//	return TRUE;
//};
//
//BOOL CQube::Translate( INT nType )
//{
//	std::vector<IObject*> * vectorCube = CTree::GetInstance()->GetVector( m_vPos );
//
//	if ( vectorCube == NULL )
//		return FALSE;
//
//	float fLength;
//	D3DXVECTOR3 vec, vN, vP;
//	std::vector <IObject*>::iterator Iter;
//	CCube * pCube;
//	for ( Iter = vectorCube->begin(); Iter != vectorCube->end(); Iter++ )
//	{
//		pCube = dynamic_cast<CCube*>( *Iter );
//		vec = pCube->GetPosition() - m_vPos;
//		fLength = D3DXVec3Length( &vec );
//		if ( fLength < m_fSize + pCube->GetSize() )
//		{
//			CPhysics::GetInstance()->Reflect( m_vMomentum, vec, m_vMomentum );
//		}		
//	}
//
//	return TRUE;
//};

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
	INT n = static_cast<INT>( FastRand2() * 3.0f );

	switch( n )
	{
	case 0:
		m_vRotateTemp = D3DXVECTOR3( FastRand2() * fRate, FastRand2() * fRate, 0.0f );
		break;
	case 1:
		m_vRotateTemp = D3DXVECTOR3( FastRand2() * fRate, 0.0f, FastRand2() * fRate );
		break;
	case 2:
		m_vRotateTemp = D3DXVECTOR3( 0.0f, FastRand2() * fRate, FastRand2() * fRate );
		break;
	}
}

VOID CQube::RanderSize( FLOAT fRate )
{
	m_fSize = FastRand2() * fRate;
}

// vector�� �� ���� �ִ��� üũ.
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