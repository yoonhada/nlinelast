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
					FileName,				//	�����Ϸ� ������ Unicode�� �䱸�ϰ� �ִ� ��� ������ Ÿ�� LPCSTR�� LPCWSTR�� �ȴ�
					D3DX_DEFAULT_NONPOW2,	//	���� ũ�⸦ �޾ƿ´� 2�� �¼��ε� �޾ƿü� �ִ�
					D3DX_DEFAULT_NONPOW2,	//	���� ũ�⸦ �޾ƿ´�
					D3DX_DEFAULT,			//	�䱸�Ǵ� �ӷ����� ��, �� ���� 0�Ǵ� D3DX_DEFAULT�� ��� ������ �Ӹ� ü�� ����
					NULL,					//	���� �ؽ���
					D3DFMT_X8R8G8B8,		//	�ؽ�ó ���� D3DFMT_UNKKNOWN�� ��� ������ ���Ϸκ��� ���
					D3DPOOL_MANAGED,		//	�ؽ�ó�� ��ġó�� �Ǵ� �޸� Ŭ������ ����Ѵ�
					D3DX_DEFAULT,			//	���͸� ���, D3DX_DEFAULT�� D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER �� ����
					D3DX_DEFAULT,			//	���͸� ���, D3DX_DEFAULT�� D3DX_FILTER_BOX �� ����
					NULL,					//	������ �Ǵ� D3DCOLOR��, �÷�Ű�� ��ȿ�� �ϴ� ���� 0�� ����
					NULL,					//	�ҽ� �̹��� ���ϳ��� �������� ����� �����ϴ� D3DXIMAGE INFO ����ü�� ������
					NULL,					//	�����ϴ� 256 �� �ȷ�Ʈ�� ��Ÿ���� PALETTEENTRY ����ü�� ������
					&(*_ppTexture) ) ) )		//	������ ť�� �ؽ�ó ��ü�� ��Ÿ���� IDirect3DTexture9 �������̽��� ������ �ּ�
	{
		//MessageBox( NULL, L"D3DCreateTextureFromFile() Failed.", NULL, MB_OK );
		return E_FAIL;
	}
	
	TCHAR	str[ 1024 ];
	wsprintf( str, L"LoadTexture : %s\n", FileName );
	//CDebugConsole::GetInstance()->Message( str );

	return S_OK;

}
