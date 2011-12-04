#pragma once
#ifndef _CAMERAWORK_H_
#define _CAMERAWORK_H_

#include "CameraWorkBase.h"

class CameraWork : public CameraWorkBase
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		InitCameraPosition();
	VOID		InitCameraLookAt();
	VOID		InitPositionToLookAt();

	VOID		SetPositionCourse(	D3DXVECTOR3& _vecPoint0, 
									D3DXVECTOR3& _vecPoint1, 
									D3DXVECTOR3& _vecPoint2, 
									D3DXVECTOR3& _vecPoint3 );

	VOID		UpdatePosition();
	VOID		UpdateLookAt();

public:
	typedef struct _DATA
	{	
		IMAGE		imgCourse;
		COURSEINFO	infCourse;

		LPIMAGE		pimgBox;
		INT			iNumBox;

		IMAGE		imgBoxLink;
		COURSEINFO	infBoxLink;

		DWORD		dWorkingPeriod;

		DWORD		dCurrentTime;
		DWORD		dBeginTime;

		INT			iIncIndex;
		INT			iCurrentIndex;
		INT			iNextIndex;

		D3DXVECTOR3	vecCurrentPosition;

		_DATA()
		{
			pimgBox = NULL;

			dWorkingPeriod	= 5000;

			dCurrentTime	= 0;
			dBeginTime		= 0;

			iIncIndex		= 1;
			iCurrentIndex	= 0;
			iNextIndex		= 1;

			vecCurrentPosition	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
		~_DATA()
		{
			if( pimgBox != NULL )
				delete[] pimgBox;

			pimgBox = NULL;
		}
	}DATA, *LPDATA;


	CameraWork( LPDIRECT3DDEVICE9 _pd3dDevice ) : CameraWorkBase( _pd3dDevice )
	{
		this->Initialize();
	}
	~CameraWork()
	{
		this->Release();
	}

	VOID		Create();
	VOID		Update();
	VOID		Render();

	VOID		SetPosition_Box4( D3DXVECTOR3& _vecPosition0, D3DXVECTOR3& _vecPosition1, D3DXVECTOR3& _vecPosition2, D3DXVECTOR3& _vecPosition3 );
	VOID		SetLookAtCourse_Box2( D3DXVECTOR3& _vecPosition0, D3DXVECTOR3& _vecPosition1 );
	VOID		SetWorkingPeriod( DWORD _dTime );
	//VOID		AddWorkingPeriod( INT _iStartIndex, INT _iEndIndex, DWORD _dTime );

	VOID		GetCameraPosition( D3DXVECTOR3& _vecOut ){ _vecOut = m_vecCameraPosition; };
	VOID		GetCameraLookAt( D3DXVECTOR3& _vecOut ){ _vecOut = m_vecCameraLookAt; };

private:
	DATA		m_datPosition;			//	Camera Position
	DATA		m_datLookAt;			//	Camera LookAt
	
	DATA		m_datPositionToLookAt;	//	Link

	//	동일한 속도를 위한!
	DWORD		m_dFrameSpeed;
	
	D3DXVECTOR3	m_vecCameraPosition;
	D3DXVECTOR3 m_vecCameraLookAt;

	DWORD		m_dColCamera;
	DWORD		m_dColBox;
	DWORD		m_dColBoxCourse;
	DWORD		m_dColPositionCourse;
	DWORD		m_dColLookAtCourse;
	DWORD		m_dCameraLink;
	DWORD		m_dColPositionToLookAt;

public:

};

#endif