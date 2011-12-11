#pragma once
#ifndef _CAMERAWORKBASE_H_
#define _CAMERAWORKBASE_H_

#include <map>

class CameraWorkBase
{
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;

	DWORD					m_dColBox;
	DWORD					m_dColBoxLink;
	DWORD					m_dColPosition;
	DWORD					m_dColLookAt;
	DWORD					m_dColPositionToLookAt;

public:
	typedef struct _VERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

		D3DXVECTOR3 vecPos;
		DWORD		dColor;
	}VERTEX, *LPVERTEX;

	typedef struct _INDEX
	{
		DWORD	_0, _1;
	}INDEX, *LPINDEX;

	typedef struct _IMAGE
	{
		LPDIRECT3DVERTEXBUFFER9	pVB;
		LPDIRECT3DINDEXBUFFER9	pIB;

		INT						iNumVertices;
		INT						iNumIndices;

		D3DXVECTOR3				vecScale;
		D3DXVECTOR3				vecRotate;
		D3DXVECTOR3				vecTranslate;

		_IMAGE()
		{
			pVB		= NULL;
			pIB		= NULL;

			vecScale		= D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
			vecRotate		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			vecTranslate	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
		~_IMAGE()
		{
			if( pVB != NULL )
				pVB->Release();
			if( pIB != NULL )
				pIB->Release();

			pVB = NULL;
			pIB = NULL;
		}
	}IMAGE, *LPIMAGE;

	typedef struct _INFO
	{
		LPVERTEX	pVertex;
		LPINDEX		pIndex;
		FLOAT*		pfIncInterporation;

		INT			iNumVertices;
		INT			iNumIndices;
		INT			iNumInterporation;

		DWORD		dBeginTime;
		DWORD		dCurrentTime;
		
		INT			iIncIndex;
		INT			iCurrentIndex;
		INT			iNextIndex;

		FLOAT		fInterporation;

		_INFO()
		{
			pVertex				= NULL;
			pIndex				= NULL;
			pfIncInterporation	= NULL;

			dBeginTime		= 0;
			dCurrentTime	= 0;

			iIncIndex		= 1;
			iCurrentIndex	= 0;
			iNextIndex		= 1;

			fInterporation	= 0.0f;
		}
		~_INFO()
		{
			if( pVertex != NULL )
				delete[] pVertex;
			if( pIndex != NULL )
				delete[] pIndex;
			if( pfIncInterporation != NULL )
				delete[] pfIncInterporation;

			pVertex				= NULL;
			pIndex				= NULL;
			pfIncInterporation	= NULL;
		}
	}INFO, *LPINFO;

	typedef struct _DATA
	{
		INFO		infPosition;
		INFO		infLookAt;

		D3DXVECTOR3	avecPoint[ 6 ];
	}DATA, *LPDATA;

	typedef std::map< DWORD, LPDATA > DATAMAP;

private:
	VOID		Initialize();
	VOID		Release();

	HRESULT		CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF );
	HRESULT		SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size );

	HRESULT		CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size );
	HRESULT		SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size );

	VOID		CheckNumVerticesOfLine4( INT& _iNumVertices );

	VOID		SetMatrix( LPD3DXVECTOR3 _pvecScale, LPD3DXVECTOR3 _pvecRotate, LPD3DXVECTOR3 _pvecTranslate );

public:
	CameraWorkBase( LPDIRECT3DDEVICE9 _pd3dDevice ) : m_pd3dDevice( _pd3dDevice )
	{
		this->Initialize();
	}
	
	virtual ~CameraWorkBase()
	{
		this->Release();
	}

	VOID		CreateInfo_BezierCurve( LPINFO _pInfo, INT _iNumVertices, D3DXVECTOR3& _vecPoint0, D3DXVECTOR3& _vecPoint1, D3DXVECTOR3& _vecPoint2, D3DXVECTOR3& _vecPosint3 );
	VOID		CreateInfo_Line2( LPINFO _pInfo, INT _iNumVertices, D3DXVECTOR3& _vecPoint0, D3DXVECTOR3& _vecPoint1 );
	VOID		CreateInfo_Line4( LPINFO _pInfo, INT _iNumVertices, D3DXVECTOR3& _vecPoint0, D3DXVECTOR3& _vecPoint1, D3DXVECTOR3& _vecPoint2, D3DXVECTOR3& _vecPoint3 );

	VOID		CreateImage( LPIMAGE _pImage, LPINFO _pInfo );
	
	VOID		RenderImage( LPIMAGE _pImage );

	//	박스만 예외로 한다 찝찝하긴 하지만...뭐...별 수 없네
	VOID		CreateImage_Box( LPIMAGE _pImage );

};

#endif