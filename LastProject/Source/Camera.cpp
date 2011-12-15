#include "Stdafx.h"
#include "Camera.h"
#include "Monster.h"
#include "CameraWork.h"

CCamera::CCamera() 
: m_fMinZoom( 15.0f )
, m_fMaxZoom( 80.0f )
, m_fLowAngle( MTP_FUN::Deg2Rad<15>::radians )	// D3DXToRadian( 15.0f ) )
, m_fHighAngle( -MTP_FUN::Deg2Rad<60>::radians ) // D3DXToRadian( -60.0f ) )
{
	Clear();
}


CCamera::~CCamera()
{
	SAFE_DELETE ( m_pCameraWork[0] );
	SAFE_DELETE ( m_pCameraWork[1] );
	SAFE_DELETE_ARRAY ( m_pCameraWork ); 
}

VOID CCamera::Create( LPDIRECT3DDEVICE9 a_pD3dDevice ) 
{
	m_pD3dDevice = a_pD3dDevice; 
	CreateEventCamera(); 
}


VOID CCamera::Clear()
{
	D3DXMatrixIdentity( &m_matView );
	D3DXMatrixIdentity( &m_matInvView );

	m_vLook		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vPreLook  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vEye		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vPreEye	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vDir		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vPreDir	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	m_fZoom		= 80.0f;
	m_fZoomReduce = 0.0f;
	m_fYaw		= 0.0f;
	m_fPitch	= -5.0f;
	//m_fLock		= (D3DX_PI/2) - 0.05f;

	m_fEffectValue = 0.0f;

	m_pCameraWork = NULL;
}

VOID CCamera::SetCamera()
{
	if ( m_nEffect == MAP_WALK )
	{
		if ( m_pCameraWork[0]->Update() == FALSE)
		{
			CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_MAP_CAMERA_WALK_END, 0.01f );

			m_nEffect = NONE;
		}

		// Test Update
		D3DXVECTOR3 vecCameraPosition, vecCameraLookAt;
		m_pCameraWork[0]->GetCameraPosition( vecCameraPosition );
		m_pCameraWork[0]->GetCameraLookAt( vecCameraLookAt );

		D3DXVECTOR3		vecEyePt	= vecCameraPosition; 
		D3DXVECTOR3		vecLookatPt	= vecCameraLookAt;
		D3DXVECTOR3		vecUpVec( 0.0f, 1.0f, 0.0f );
		D3DXMATRIXA16	matView;
		D3DXMatrixLookAtLH( &matView, &vecEyePt, &vecLookatPt, &vecUpVec );
		m_pD3dDevice->SetTransform( D3DTS_VIEW, &matView );
	}
	else if ( m_nEffect == TARGET_SHOW )
	{
		if ( m_pCameraWork[1]->Update() == FALSE)
		{
			m_nEffect = NONE;
		}

		// Test Update
		D3DXVECTOR3 vecCameraPosition, vecCameraLookAt;
		m_pCameraWork[1]->GetCameraPosition( vecCameraPosition );
		m_pCameraWork[1]->GetCameraLookAt( vecCameraLookAt );

		D3DXVECTOR3		vecEyePt	= vecCameraPosition; 
		D3DXVECTOR3		vecLookatPt	= vecCameraLookAt;
		D3DXVECTOR3		vecUpVec( 0.0f, 1.0f, 0.0f );
		D3DXMATRIXA16	matView;
		D3DXMatrixLookAtLH( &matView, &vecEyePt, &vecLookatPt, &vecUpVec );
		m_pD3dDevice->SetTransform( D3DTS_VIEW, &matView );
	}
	else
	{
		//상하 각도 고정
		if ( m_fPitch < ( m_fHighAngle ) ) 
		{
			m_fPitch = m_fHighAngle;
		}
		if ( m_fPitch > ( m_fLowAngle ) ) 
		{
			m_fPitch = m_fLowAngle;
		}

		D3DXMATRIXA16   m;
		m_vDir  = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
		D3DXVec3TransformCoord( &m_vDir, &m_vDir, D3DXMatrixRotationX( &m, m_fPitch ) );
		D3DXVec3TransformCoord( &m_vDir, &m_vDir, D3DXMatrixRotationY( &m, m_fYaw ) );

		m_vEye = m_vLook;
		m_vDir *= m_fZoom;
		m_vEye -= m_vDir;
		if ( m_vEye.y < 0.0f )
			m_vEye.y = 0.0f;

		CheckObjectCollision( );
		D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLook, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
		m_pD3dDevice->SetTransform( D3DTS_VIEW, &m_matView );
		D3DXMatrixInverse( &m_matInvView, NULL, &m_matView );
		D3DXVec3Normalize( &m_vDir, &(m_vEye - m_vLook ) );

		m_vPreLook = m_vLook;	
		m_vPreDir = m_vDir;	
		m_vPreEye = m_vEye;
	}
}


VOID CCamera::SetEffect(INT nType)
{
	m_nEffect = nType;
}

VOID CCamera::Effect( D3DXVECTOR3 & a_vLook )
{
	switch ( m_nEffect )
	{
	case NONE:
		m_fEffectValue = 0;
		break;
	case SWING:
		{
			FLOAT fTime = D3DX_PI * 20.0f;
			D3DXVECTOR3 vUp(0, 1, 0);
			D3DXVec3Cross( &vUp, &m_vDir, &vUp );	// Side
			a_vLook += vUp * sin(m_fEffectValue) * 5.0f;
			m_fEffectValue += ( fTime * CFrequency::GetInstance()->getFrametime() );
			if ( m_fEffectValue > fTime )
			{
				m_nEffect = NONE;
				m_fEffectValue = 0;
			}
		}
		break;
	}	
}

VOID CCamera::SetView( const D3DXVECTOR3 &a_vLook, const D3DXVECTOR3 &a_vPreLook, FLOAT a_fY, FLOAT a_fZoom, FLOAT a_fYaw, FLOAT a_fPitch )
{
	D3DXVECTOR3 vLook;
	static FLOAT fZoom = m_fMaxZoom;
	if( CInput::GetInstance()->Get_Homebutton() == TRUE )
	{
		fZoom += 1.0f;
	}
	if( CInput::GetInstance()->Get_Endbutton() == TRUE )
	{
		fZoom -= 1.0f;
	}

	vLook = a_vLook;
	Effect( vLook );

	m_vLook = SpringDamp( m_vLook, vLook, a_vPreLook, 10.0f, 1.0f, 1.0f );
	//m_vLook  = a_vLook;
	m_vLook.y = a_fY;
	m_fZoom  = fZoom;
	m_fYaw   = a_fYaw;
	m_fPitch -= a_fPitch;

    SetCamera();
}


VOID CCamera::SetLookAt( D3DXVECTOR3 &a_vLook, FLOAT a_fY /*= 0.0f */ )
{
	//vLook.y += y;
	m_vLook = a_vLook;
	m_vLook.y = a_fY;
	SetCamera();
}


void CCamera::SetZoom( FLOAT a_fZoom )
{
	if ( a_fZoom <= 2.0f ) a_fZoom = 1.0f;

	m_fZoom = a_fZoom;
	SetCamera();
}


void CCamera::SetYaw( FLOAT a_fYaw )
{
	m_fYaw = a_fYaw;
	SetCamera();
}


void CCamera::SetPitch( FLOAT a_fPitch )
{
	m_fPitch += a_fPitch;
	SetCamera();
}

D3DXVECTOR3 CCamera::SpringDamp( 
					   D3DXVECTOR3 a_vCurPos,
					   D3DXVECTOR3 a_vTargetPos,
					   D3DXVECTOR3 a_vPreTrgPos,
					   FLOAT		a_fSpringConst,
					   FLOAT		a_fDampConst,
					   FLOAT		a_fSpringLength )
{
	//용수철 힘 계산
	D3DXVECTOR3 vDisp = a_vCurPos - a_vTargetPos;
	D3DXVECTOR3 vVelocity = (a_vPreTrgPos - a_vTargetPos) * CFrequency::GetInstance()->getFrametime();
	FLOAT fForceMag = a_fSpringConst * ( a_fSpringLength - D3DXVec3Length( &vDisp ) ) + a_fDampConst
		* ( D3DXVec3Dot(&vDisp, &vVelocity) / D3DXVec3Length( &vDisp ) );

	//용수철 힘을 적용
	D3DXVec3Normalize(&vDisp, &vDisp);
	vDisp *= fForceMag * CFrequency::GetInstance()->getFrametime();

	return a_vCurPos += vDisp;
}

VOID CCamera::UpdateMatrix()
{
	D3DXMATRIXA16 matVPInv;

	D3DXMatrixMultiply( &matVPInv, &m_matView, CMatrices::GetInstance()->Get_matProj() );
	D3DXMatrixInverse( &matVPInv, NULL, &matVPInv );

	D3DXVECTOR3 vZ = D3DXVECTOR3(m_matView._13, m_matView._23, m_matView._33);
	FLOAT		fNear = 0.1f;
	FLOAT		fFar	  = 10000.0f;
	D3DXVECTOR3 vEye = m_vEye;

	// Near
	D3DXVECTOR3 vNear;
	FLOAT		fDNear;

	vNear = -vZ;
	fDNear = -D3DXVec3Dot( &vNear, &vEye ) + fNear;
	m_Frst[1] = D3DXPLANE( vNear.x, vNear.y, vNear.z, fDNear );

	// Far
	D3DXVECTOR3 vFar;
	FLOAT		fDfar;

	vFar = vZ;
	fDfar = -D3DXVec3Dot( &vFar, &vEye ) - fFar;
	m_Frst[1] = D3DXPLANE( vFar.x, vFar.y, vFar.z, fDfar );

	// Left, Right, Up, Down
	D3DXVECTOR3 vPyr[4];
	vPyr[0] = D3DXVECTOR3( -1.0f, -1.0f, 0.0f );
	vPyr[1] = D3DXVECTOR3( -1.0f,  1.0f, 0.0f );
	vPyr[2] = D3DXVECTOR3(  1.0f,  1.0f, 0.0f );
	vPyr[3] = D3DXVECTOR3(  1.0f, -1.0f, 0.0f );

	// UnProjection, UnView
	for( INT Loop=0; Loop<4; ++Loop )
	{
		D3DXVec3TransformCoord( &vPyr[Loop], &vPyr[Loop], &matVPInv );
	}

	// 얻어진 월드 좌표로 프러스텀 평면 만들기, 벡터는 안에서 밖으로
	D3DXPlaneFromPoints( &m_Frst[2], vPyr+0, vPyr+1, &vEye ); ///< left
	D3DXPlaneFromPoints( &m_Frst[3], vPyr+2, vPyr+3, &vEye ); ///< right
	D3DXPlaneFromPoints( &m_Frst[4], vPyr+1, vPyr+2, &vEye ); ///< up
	D3DXPlaneFromPoints( &m_Frst[5], vPyr+3, vPyr+0, &vEye ); ///< down

}

VOID CCamera::CheckObjectCollision( )
{
	//CTree * pTree = CTree::GetInstance();
	//std::vector<CBoundBox*> * vecBoundBox = pTree->GetMapVector( pTree->GetRoot(), a_vLook );
	//std::vector<CBoundBox*>::iterator Iter;



	//D3DXVECTOR3 vDir = a_vPosCamera;
	//while( !Collision( vDir, a_vPosCharactor, a_fAngleChara ) && --n )
	////if ( Collision( a_vPosCamera, a_vPosCharactor, a_fAngleChara ) )
	//{
	//	vDir *= 0.9f;
	//}
	//m_vEye = vDir;
}

BOOL CCamera::Collision( const D3DXVECTOR3& a_vPosCamera, const D3DXVECTOR3& a_vPosCharactor, const FLOAT a_fAngleChara )
{
	CTree * pTree = CTree::GetInstance();
	std::vector<CBoundBox*> * vecBoundBox = pTree->GetMapVector( pTree->GetRoot(), a_vPosCharactor );
	std::vector<CBoundBox*>::iterator Iter;
	D3DXVECTOR3 vPos = a_vPosCamera;
	D3DXVECTOR3 vDir = a_vPosCamera - a_vPosCharactor;

	BOOL bColl = FALSE;
	if ( vecBoundBox )
	{
		FLOAT fRadious;

		for ( Iter = vecBoundBox->begin(); Iter != vecBoundBox->end(); ++Iter )
		{
			fRadious = (*Iter)->GetSize( CBoundBox::PLUSX );
			if ( fRadious < (*Iter)->GetSize( CBoundBox::PLUSZ ) ) 
			{
				fRadious = (*Iter)->GetSize( CBoundBox::PLUSZ );
			}

			if ( CPhysics::GetInstance()->Collision( (*Iter)->GetPosition(), fRadious, a_vPosCharactor, a_vPosCamera ) )
			{
				bColl = TRUE;
			}
		}
	}

	return bColl;
}

VOID CCamera::CreateEventCamera()
{
	//	Test Create
	//	Create CameraWork
	m_pCameraWork = new CameraWork*[2];
	m_pCameraWork[0] = new CameraWork( m_pD3dDevice );
	m_pCameraWork[1] = new CameraWork( m_pD3dDevice );

	CameraWork::PARAMETER prmCameraWork;
	
	prmCameraWork.avecPosition[ 0 ] = D3DXVECTOR3( 364.0f, 10.0f, -777.0f );
	prmCameraWork.avecPosition[ 1 ] = D3DXVECTOR3( -1456.0f, 300.0f, 1083.0f );
	prmCameraWork.avecPosition[ 2 ] = D3DXVECTOR3( 934.0f, 250.0f, 2092.0f );
	prmCameraWork.avecPosition[ 3 ] = D3DXVECTOR3( 231.0f, 100.0f, -580.0f );

	prmCameraWork.avecLookAt[ 0 ] = D3DXVECTOR3( -46.0f, 10.0f, 770.0f );
	prmCameraWork.avecLookAt[ 1 ] = D3DXVECTOR3( 244.0f, 10.0f, -194.0f );

	m_pCameraWork[0]->AddData( CWK_MAIN_EVENT0, &prmCameraWork );
	m_pCameraWork[0]->SetWholeWorkingPeriod( CWK_MAIN_EVENT0, CameraWork::CWK_POSITION, 5000 );
	m_pCameraWork[0]->SetPartWorkingPeriod( CWK_MAIN_EVENT0, CameraWork::CWK_POSITION, 3000, 0, 20 );
	m_pCameraWork[0]->SetWholeWorkingPeriod( CWK_MAIN_EVENT0, CameraWork::CWK_LOOKAT, 7000 );
	m_pCameraWork[0]->SelectData( CWK_MAIN_EVENT0 );

	prmCameraWork.avecPosition[ 0 ] = D3DXVECTOR3( -184.0f, 289.0f, 666.0f );
	prmCameraWork.avecPosition[ 1 ] = D3DXVECTOR3( -361.0f, 178.0f, 73.0f );
	prmCameraWork.avecPosition[ 2 ] = D3DXVECTOR3( -340.0f, 58.0f, 1294.0f );
	prmCameraWork.avecPosition[ 3 ] = D3DXVECTOR3( 69.0f, 80.0f, 669.0f );

	prmCameraWork.avecLookAt[ 0 ] = D3DXVECTOR3( -185.0f, 342.0f, 665.0f );
	prmCameraWork.avecLookAt[ 1 ] = D3DXVECTOR3( -153.0f, 48.0f, 676.0f );
	
	m_pCameraWork[1]->AddData( CWK_MAIN_EVENT1, &prmCameraWork );
	m_pCameraWork[1]->SetWholeWorkingPeriod( CWK_MAIN_EVENT1, CameraWork::CWK_POSITION, 2500 );
	m_pCameraWork[1]->SetWholeWorkingPeriod( CWK_MAIN_EVENT1, CameraWork::CWK_LOOKAT, 1300 );
	m_pCameraWork[1]->SelectData( CWK_MAIN_EVENT1 );
}