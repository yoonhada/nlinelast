#include "stdafx.h"
#include "ASEViewerBase.h"

VOID ASEViewerBase::Initialize()
{
}

VOID ASEViewerBase::Release()
{
}

HRESULT	ASEViewerBase::CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF )
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
HRESULT	ASEViewerBase::SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size )
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
HRESULT	ASEViewerBase::CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size )
{
	if( FAILED( m_pd3dDevice->CreateIndexBuffer(	_nIndex * _Size,
													0,
													D3DFMT_INDEX32,
													D3DPOOL_DEFAULT,
													&(*_ppIB),
													NULL ) ) )
	{
		MessageBox( NULL, TEXT("CreateIndexBuffer() failed."), NULL, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}
HRESULT	ASEViewerBase::SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size )
{
	LPVOID pIndices;
	if( FAILED( _pIB->Lock( 0, _nIndex * _Size, (VOID**)&pIndices, 0 ) ) )
	{
		MessageBox( NULL, L"m_pIB->Lock() failed.", NULL, MB_OK );
		return E_FAIL;
	}
	
		memcpy( pIndices, _indices, _nIndex * _Size );

	_pIB->Unlock();

	return S_OK;
}

HRESULT ASEViewerBase::LoadTextureFromFile( LPDIRECT3DTEXTURE9* _ppTexture, LPCWSTR FileName )
{
	if( FAILED( D3DXCreateTextureFromFileEx(	
					m_pd3dDevice,
					FileName,				//	컴파일러 설정이 Unicode를 요구하고 있는 경우 데이터 타입 LPCSTR은 LPCWSTR이 된다
					D3DX_DEFAULT_NONPOW2,	//	원본 크기를 받아온다 2의 승수로도 받아올수 있다
					D3DX_DEFAULT_NONPOW2,	//	원본 크기를 받아온다
					D3DX_DEFAULT,			//	요구되는 밉레벨의 수, 이 값이 0또는 D3DX_DEFAULT의 경우 완전한 밉맵 체인 생성
					NULL,					//	동적 텍스쳐
					D3DFMT_X8R8G8B8,		//	텍스처 포멧 D3DFMT_UNKKNOWN의 경우 포멧은 파일로부터 취득
					D3DPOOL_MANAGED,		//	텍스처의 배치처가 되는 메모리 클래스를 기술한다
					D3DX_DEFAULT,			//	필터링 방법, D3DX_DEFAULT는 D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER 와 같다
					D3DX_DEFAULT,			//	필터링 방법, D3DX_DEFAULT는 D3DX_FILTER_BOX 와 같다
					NULL,					//	투명이 되는 D3DCOLOR값, 컬러키를 무효로 하는 경우는 0을 지정
					NULL,					//	소스 이미지 파일내의 데이터의 기술을 저장하는 D3DXIMAGE INFO 구조체의 포인터
					NULL,					//	저장하는 256 색 팔레트를 나타내는 PALETTEENTRY 구조체의 포인터
					&(*_ppTexture) ) ) )		//	생성된 큐브 텍스처 개체를 나타내는 IDirect3DTexture9 인터페이스의 포인터 주소
	{
		//MessageBox( NULL, L"D3DCreateTextureFromFile() Failed.", NULL, MB_OK );
		return E_FAIL;
	}
	
	TCHAR	str[ 1024 ];
	wsprintf( str, L"LoadTexture : %s\n", FileName );
	//CDebugConsole::GetInstance()->Message( str );

	return S_OK;

}
