#ifndef _CAMERA_H_
#define _CAMERA_H_


class CCamera
{


public:
	CCamera();
	virtual ~CCamera();

	VOID		 Create( LPDIRECT3DDEVICE9 a_pD3dDevice ) {  m_pD3dDevice = a_pD3dDevice; }

	VOID SetView( const D3DXVECTOR3 &a_vLook, const D3DXVECTOR3 &a_vPreLook, FLOAT a_fY, FLOAT a_fZoom, FLOAT a_fYaw, FLOAT a_fPitch );
	D3DXVECTOR3* GetCameraLookAt() { return &m_vLook; }
	D3DXVECTOR3* GetCameraEye() { return &m_vEye; }
	D3DXVECTOR3* GetCameraDir() { return &m_vDir; }

	VOID SetLookAt(D3DXVECTOR3 &a_vLook, FLOAT a_fY = 0.0f );
	VOID SetZoom( FLOAT a_fZoom );
	VOID SetYaw( FLOAT a_fYaw ); 
	VOID SetPitch( FLOAT a_fPitch ); 
	FLOAT GetZoom() { return m_fZoom; }
	FLOAT GetYaw() { return m_fYaw; }
	FLOAT GetPitch() { return m_fPitch; }
	const D3DXVECTOR3& GetEye() { return m_vEye; }
	const D3DXVECTOR3& GetDir() { return m_vDir; }
	const D3DXVECTOR3& GetLook() { return m_vLook; }

	D3DXMATRIXA16*	GetView()		{ return &m_matView; }
	D3DXMATRIXA16*	GetInvView()	{ return &m_matInvView; }

protected:
	VOID Clear();
	VOID SetCamera();

	LPDIRECT3DDEVICE9		m_pD3dDevice;              

	D3DXMATRIXA16	m_matView;
	D3DXMATRIXA16	m_matInvView; 

	FLOAT			m_fLock;            ///< 상하 각도 제한용 변수

	D3DXVECTOR3		m_vLook;
	D3DXVECTOR3		m_vPreLook;
	D3DXVECTOR3		m_vDir;
	D3DXVECTOR3		m_vEye;
	FLOAT			m_fZoom;            ///< 캐릭터와 카메라의 거리
	FLOAT			m_fYaw;             ///< X축 회전
	FLOAT			m_fPitch;			///< Y축 회전

	// 용수철
	D3DXVECTOR3 SpringDamp( 
		D3DXVECTOR3 a_vCurPos,
		D3DXVECTOR3 a_vTargetPos,
		D3DXVECTOR3 a_vPreTrgPos,
		FLOAT		a_fSpringConst,
		FLOAT		a_fDampConst,
		FLOAT		a_fSpringLength );

};

#endif 
