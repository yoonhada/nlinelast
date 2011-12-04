#pragma once
#ifndef _CAMERAWORKBASE_H_
#define _CAMERAWORKBASE_H_

class CameraWorkBase
{
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;

public:
	typedef struct _VERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

		D3DXVECTOR3 vecPos;
		DWORD		dColor;
	}VERTEX, *LPVERTEX;

	typedef struct _INDEX
	{
		DWORD		_0, _1;
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

	typedef struct _COURSEINFO
	{
		LPVERTEX	pVertex;
		FLOAT*		pfIncInterporation;

		INT			iNumVertices;

		_COURSEINFO()
		{
			pVertex				= NULL;
			pfIncInterporation	= NULL;

			iNumVertices		= 100;		//	초기값을 100으로 주자꾸나 기본적으로 있어야 되고 나중에 정밀도를 변경하는 것이니
		}
		~_COURSEINFO()
		{
			if( pVertex != NULL )
				delete[] pVertex;
			if( pfIncInterporation != NULL )
				delete[] pfIncInterporation;

			pVertex				= NULL;
			pfIncInterporation	= NULL;
		}
	}COURSEINFO, *LPCOURSEINFO;

private:
	VOID		Initialize();
	VOID		Release();

	HRESULT		CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF );
	HRESULT		SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size );

	HRESULT		CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size );
	HRESULT		SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size );

public:
	CameraWorkBase( LPDIRECT3DDEVICE9 _pd3dDevice ) : m_pd3dDevice( _pd3dDevice )
	{
		this->Initialize();
	}
	~CameraWorkBase()
	{
		this->Release();
	}

	VOID		SetMatrix( LPD3DXVECTOR3 _pvecScale, LPD3DXVECTOR3 _pvecRotate, LPD3DXVECTOR3 _pvecTranslate );
	
	VOID		CreateImage_Camera( LPIMAGE _pimgCamera, DWORD _dColor = 0xffffffff );
	VOID		CreateImage_Box( LPIMAGE _pimgBox, DWORD _dColor = 0xffff00ff );
	VOID		CreateImage_Course( LPIMAGE _pimgCourse, LPCOURSEINFO _pinfCourse );
	
	VOID		CreateBoxLink( LPIMAGE _pimgBoxLink, LPIMAGE _pimgBox, DWORD _dColor = 0xffff00ff );
	//VOID		SetBoxPosition( LPD3DXVECTOR3 _vecBoxPosition, FLOAT _fX, FLOAT _fY, FLOAT _fZ );

	VOID		RenderImage( LPIMAGE _pImage );

};

#endif