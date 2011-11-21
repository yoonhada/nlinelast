#include "Stdafx.h"
#include "Camera.h"

CCamera::CCamera() : m_fMinZoom(15.0f), m_fMaxZoom(50.0f)
{
	Clear();
}


CCamera::~CCamera()
{

}


VOID CCamera::Clear()
{
	D3DXMatrixIdentity( &m_matView );
	D3DXMatrixIdentity( &m_matInvView );

	m_vLook		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vPreLook  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vEye		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vDir		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_fZoom		= 50.0f;
	m_fZoomReduce = 0.0f;
	m_fYaw		= 0.0f;
	m_fPitch	= 0.0f;
	m_fLock		= (D3DX_PI/2) - 0.05f;

	m_fEffectValue = 0.0f;
}

void CCamera::SetCamera()
{
	//상하 각도 고정
	if ( m_fPitch > (m_fLock * 0.1f) ) m_fPitch = m_fLock * 0.1f;
	if ( m_fPitch < (m_fLock * -1.0f) ) m_fPitch = m_fLock * -1.0f;

	D3DXMATRIXA16   m;
    m_vDir  = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
    D3DXVec3TransformCoord( &m_vDir, &m_vDir, D3DXMatrixRotationX( &m, m_fPitch ) );
    D3DXVec3TransformCoord( &m_vDir, &m_vDir, D3DXMatrixRotationY( &m, m_fYaw ) );

    m_vEye    = m_vLook;
    m_vDir   *= m_fZoom;
    m_vEye   -= m_vDir;

    D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLook, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	m_pD3dDevice->SetTransform( D3DTS_VIEW, &m_matView );
	D3DXMatrixInverse( &m_matInvView, NULL, &m_matView );
	D3DXVec3Normalize( &m_vDir, &(m_vEye - m_vLook ) );
}


VOID CCamera::SetEffect(INT nType)
{
	m_nEffect = nType;
}

VOID CCamera::Effect( D3DXVECTOR3 & a_vLook )
{
	switch ( m_nEffect )
	{
	case 0:
		m_nEffect = 0;
		m_fEffectValue = 0;
		break;
	case 1:
		a_vLook += D3DXVECTOR3(1, 0, 1) * sin(m_fEffectValue) * 10.0f;
		m_fEffectValue += (D3DX_PI * 0.5f );
		if (m_fEffectValue > 100.0f)
		{
			m_nEffect = 0;
			m_fEffectValue = 0;
		}
		break;
	case 2:
		if (a_vLook == D3DXVECTOR3(300, 0, 0))
		{
			a_vLook = D3DXVECTOR3(300, 0, 300);
		}
		else 
		{
			a_vLook = D3DXVECTOR3(300, 0, 0);
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

	for (int i = 0; i < 10; ++i)
	{
		if( CInput::GetInstance()->Get_NumKey(i) )
			SetEffect(i);
	}

	vLook = a_vLook;
	Effect( vLook );

	m_vLook = SpringDamp( m_vLook, vLook, a_vPreLook, 10.0f, 1.0f, 1.0f );
	//m_vLook  = a_vLook;
	m_vLook.y = a_fY;
	m_fZoom  = fZoom;
	m_fYaw   = a_fYaw;
	m_fPitch += a_fPitch;
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

BOOL CCamera::Collision( const D3DXVECTOR3& a_vPosCamera, const D3DXVECTOR3& a_vPosCharactor, const FLOAT a_fAngleChara )
{
	//CDebugConsole::GetInstance()->Messagef( L"Camera Pos : %f / %f / %f\n", a_vPosCamera.x, a_vPosCamera.y, a_vPosCamera.z );
	//CDebugConsole::GetInstance()->Messagef( L"Chara Pos : %f / %f / %f\n", a_vPosCharactor.x, a_vPosCharactor.y, a_vPosCharactor.z );

	std::vector<CBoundBox*> * vecBoundBox = CTree::GetInstance()->GetMapVector(CTree::GetInstance()->GetRoot(), a_vPosCharactor);
	std::vector<CBoundBox*>::iterator Iter;

	BOOL bColl = TRUE;
	if ( vecBoundBox )
	{
		//int i = 0;
		for ( Iter = vecBoundBox->begin(); Iter != vecBoundBox->end(); ++Iter )
		{
			//(*Iter)->SetAngle( a_fAngleChara );
			if( CPhysics::GetInstance()->Collision( a_vPosCamera, a_vPosCharactor,  ( *Iter ) ) )
			{
				bColl = FALSE;
				//CDebugConsole::GetInstance()->Messagef( L"bCol : %d\n", bColl );
				break;
			}
		}
	}

	vecBoundBox = CTree::GetInstance()->GetMapVector(CTree::GetInstance()->GetRoot(), a_vPosCamera);

	//BOOL bColl = TRUE;
	if ( vecBoundBox )
	{
		//int i = 0;
		for ( Iter = vecBoundBox->begin(); Iter != vecBoundBox->end(); ++Iter )
		{
			//(*Iter)->SetAngle( a_fAngleChara );
			if( CPhysics::GetInstance()->Collision( a_vPosCamera, a_vPosCharactor,  ( *Iter ) ) )
			{
				bColl = FALSE;
				//CDebugConsole::GetInstance()->Messagef( L"bCol : %d\n", bColl );
				break;
			}
		}
	}

	return bColl;
}

VOID CCamera::CheckObjectCollision( const D3DXVECTOR3& a_vPosCamera, const D3DXVECTOR3& a_vPosCharactor, const FLOAT a_fAngleChara )
{
	static BOOL bCheck = FALSE;
	if ( Collision( a_vPosCamera, a_vPosCharactor, a_fAngleChara ) == FALSE )
	{
		if( m_fZoom > m_fMinZoom )
		{
			////CDebugConsole::GetInstance()->Messagef( L"IN : %f / %f\n", m_fZoomReduce, m_fZoom );
			m_fZoom -= 5.0f;
		}

		//bCheck = FALSE;

		//m_fZoomReduce += 1.0f;
		//m_fZoom -= m_fZoomReduce;

		//SetCamera();

		//CDebugConsole::GetInstance()->Messagef( L"IN : %f / %f\n", m_fZoomReduce, m_fZoom );
	}
	else
	{
		if( m_fZoom < m_fMaxZoom )
		{
			m_fZoom += 5.0f;
			bCheck = TRUE;
		}

		
		//m_fZoom += m_fZoomReduce;
		//m_fZoomReduce = 0.0f;
	}

	//CDebugConsole::GetInstance()->Messagef( L"OUT : %f / %f\n", m_fZoomReduce, m_fZoom );
}
