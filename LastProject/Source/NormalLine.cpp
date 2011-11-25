#include "stdafx.h"
#include "NormalLine.h"


VOID NormalLine::Initialize()
{
}

VOID NormalLine::Release()
{
	LPDATALIST::iterator itE;
	for( itE = m_pDataList.begin() ; itE != m_pDataList.end() ; itE++ )
		SAFE_DELETE( (*itE) );
	m_pDataList.clear();
}

VOID NormalLine::Create( D3DCOLOR _Color, INT _iNumVertex, LPD3DXVECTOR3 _pPosition, LPD3DXVECTOR3 _pNormal )
{
	INT iNumVertex = _iNumVertex * 2;
	LPVERTEX pVertex = new VERTEX[ iNumVertex ];

	INT k = 0;
	for( INT i=0 ; i<iNumVertex ; i+=2 )
	{
		pVertex[ i   ].pos		= _pPosition[ k ];
		pVertex[ i   ].color	= _Color;

		pVertex[ i+1 ].pos		= _pPosition[ k ] + _pNormal[ k ];
		pVertex[ i+1 ].color	= _Color;

		k++;
	}
	
	LPDATA	pData = new DATA;

	pData->iNumLine = _iNumVertex;

	CreateVB( &pData->pVB, iNumVertex, sizeof( VERTEX ), VERTEX::FVF );
	SetVB( pData->pVB, pVertex, iNumVertex,  sizeof( VERTEX ) );

	m_pDataList.push_back( pData );

	SAFE_DELETE_ARRAY( pVertex );
}

VOID NormalLine::Render()
{
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	LPDATALIST::iterator itE;

	for( itE = m_pDataList.begin() ; itE != m_pDataList.end() ; itE++ )
	{
		m_pd3dDevice->SetStreamSource( 0, (*itE)->pVB, 0, sizeof( VERTEX ) );
		m_pd3dDevice->SetFVF( VERTEX::FVF );
		m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, (*itE)->iNumLine );
	}

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}
	
HRESULT	NormalLine::CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF )
{
	if( FAILED( m_pd3dDevice->CreateVertexBuffer(	_nVertex * _Size,
													0,
													_FVF,
													D3DPOOL_DEFAULT,
													&(*_ppVB),
													NULL ) ) )
	{
		MessageBox( NULL, L"CreateVertexBuffer() failed.", NULL, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}
HRESULT	NormalLine::SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size )
{
	LPVOID pvertices;
	if( FAILED( _pVB->Lock( 0, _nVertex *  _Size, (VOID**)&pvertices, 0 ) ) )
	{
		MessageBox( NULL, L"m_pVB->Lock() failed.", NULL, MB_OK );
		return E_FAIL;
	}
	
		memcpy( pvertices, _pvertices, _nVertex * _Size );

	_pVB->Unlock();

	return S_OK;
}
