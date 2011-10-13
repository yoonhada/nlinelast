#include "Stdafx.h"
#include "Camera.h"

CCamera::CCamera()
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
	m_fZoom		= 15.0f;
	m_fYaw		= 0.0f;
	m_fPitch	= 0.0f;
	m_fLock		= (D3DX_PI/2) - 0.05f;
}

void CCamera::SetCamera()
{
	//»óÇÏ °¢µµ °íÁ¤
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


VOID CCamera::SetView( const D3DXVECTOR3 &a_vLook, const D3DXVECTOR3 &a_vPreLook, FLOAT a_fY, FLOAT a_fZoom, FLOAT a_fYaw, FLOAT a_fPitch )
{
	m_vLook = SpringDamp( m_vLook, a_vLook, a_vPreLook, 10.0f, 50.0f, 10.0f );
	//m_vLook  = a_vLook;
	m_vLook.y = a_fY;
	m_fZoom  = a_fZoom;
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
	//¿ë¼öÃ¶ Èû °è»ê
	D3DXVECTOR3 vDisp = a_vCurPos - a_vTargetPos;
	D3DXVECTOR3 vVelocity = (a_vPreTrgPos - a_vTargetPos) * CFrequency::GetInstance()->getFrametime();
	FLOAT fForceMag = a_fSpringConst * ( a_fSpringLength - D3DXVec3Length( &vDisp ) ) + a_fDampConst
		* ( D3DXVec3Dot(&vDisp, &vVelocity) / D3DXVec3Length( &vDisp ) );

	//¿ë¼öÃ¶ ÈûÀ» Àû¿ë
	D3DXVec3Normalize(&vDisp, &vDisp);
	vDisp *= fForceMag * CFrequency::GetInstance()->getFrametime();

	return a_vCurPos += vDisp;
}