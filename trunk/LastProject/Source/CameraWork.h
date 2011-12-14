#pragma once
#ifndef _CAMERAWORK_H_
#define _CAMERAWORK_H_

#include "CameraWorkBase.h"

class CameraWork : public CameraWorkBase
{
private:
	enum {	CWK_POSITION_BOX0 = 0, CWK_POSITION_BOX1, CWK_POSITION_BOX2, CWK_POSITION_BOX3,
			CWK_LOOKAT_BOX0, CWK_LOOKAT_BOX1, 
			CWK_POSITIONTOLOOKAT_BOX0, CWK_POSITIONTOLOOKAT_BOX1, 
			CWK_END };

	VOID		Initialize();
	VOID		Release();

	BOOL		UpdateInfo( LPINFO _pInfo, LPD3DXVECTOR3 _pvecPosition );

public:
	enum { CWK_POSITION = 0, CWK_LOOKAT };

	CameraWork( LPDIRECT3DDEVICE9 _pd3dDevice ) : CameraWorkBase( _pd3dDevice )
	{
		this->Initialize();
	}
	virtual ~CameraWork()
	{
		this->Release();
	}

	typedef struct _PARAMETER
	{
		D3DXVECTOR3	avecPosition[ 4 ];
		D3DXVECTOR3	avecLookAt[ 2 ];
	}PARAMETER, *LPPARAMETER;

	VOID		AddData( DWORD _dID, LPPARAMETER _pParameter );

	//	전채 IncIndex, fIncInterporation를 조정
	VOID		SetWholeWorkingPeriod( DWORD _dID,DWORD _dType, DWORD _dTime );
	//	부분 fIncInterporation만 조정
	VOID		SetPartWorkingPeriod( DWORD _dID, DWORD _dType, DWORD _dTime, INT _iStartIndex, INT _iEndIndex );

	VOID		SelectData( DWORD _dID );
	VOID		EndWorking();

	BOOL		Update();

	VOID		GetCameraPosition( D3DXVECTOR3& _vecOut ){ _vecOut = m_vecCameraPosition; };
	VOID		GetCameraLookAt( D3DXVECTOR3& _vecOut ){ _vecOut = m_vecCameraLookAt; };

	VOID		DebugData( LPPARAMETER _pParameter );
	VOID		DebugUpdate();
	VOID		DebugRender();

private:
	DATAMAP			m_mapData;
	LPDATA			m_pdatCurrent;

	DATA			m_datDebug;

	DWORD			m_dFrameSpeed;

	IMAGE			m_aimgBoxs[ CWK_END ];
	INFO			m_infBoxLink;
	IMAGE			m_imgBoxLink;
	IMAGE			m_imgPositionCourse;
	IMAGE			m_imgLookAtCourse;
	INFO			m_infPositionToLookAtLink;
	IMAGE			m_imgPositionToLookAtLink;

	D3DXVECTOR3		m_vecCameraPosition;
	D3DXVECTOR3		m_vecCameraLookAt;

public:

};

#endif