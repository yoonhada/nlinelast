#include "stdafx.h"
#include "GUIBase.h"

VOID GUIBase::Initialize()
{
	//	DrawFontOnTexture
	m_hFont = NULL;
	//	End
}

VOID GUIBase::Release()
{
}

VOID GUIBase::CreateImage( IMAGE& _Image, FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, LPCWSTR _pFileName )
{
	//	Init VertexBuffer, IndexBuffer
	VERTEX	Vertex[ 4 ];
	INDEX	Index[ 2 ];

	Vertex[ 0 ].vecPosition = D3DXVECTOR3( -0.5f,	-0.5f,	1.0f );
	Vertex[ 1 ].vecPosition = D3DXVECTOR3( 0.5f,	-0.5f,	1.0f );
	Vertex[ 2 ].vecPosition = D3DXVECTOR3( -0.5f,	0.5f,	1.0f );
	Vertex[ 3 ].vecPosition = D3DXVECTOR3( 0.5f,	0.5f,	1.0f );

	Vertex[ 0 ].vecTexcoord = D3DXVECTOR2( 0.0f, 0.0f );
	Vertex[ 1 ].vecTexcoord = D3DXVECTOR2( 1.0f, 0.0f );
	Vertex[ 2 ].vecTexcoord = D3DXVECTOR2( 0.0f, 1.0f );
	Vertex[ 3 ].vecTexcoord = D3DXVECTOR2( 1.0f, 1.0f );

	Index[ 0 ]._0 = 0;
	Index[ 0 ]._1 = 1;
	Index[ 0 ]._2 = 3;

	Index[ 1 ]._0 = 0;
	Index[ 1 ]._1 = 3;
	Index[ 1 ]._2 = 2;

	_Image.iNumVertices = 4;
	_Image.iNumIndices	= 2;

	CreateVB( &_Image.pVB, _Image.iNumVertices, sizeof( VERTEX ), VERTEX::FVF );
	SetVB( _Image.pVB, Vertex, _Image.iNumVertices, sizeof( VERTEX ) );

	CreateIB( &_Image.pIB, _Image.iNumIndices, sizeof( INDEX ) );
	SetIB( _Image.pIB, Index, _Image.iNumIndices, sizeof( INDEX ) );

	//	Init Matrix
	_Image.vecScale.x	= _fWidth;
	_Image.vecScale.y	= _fHeight;

	//	Rotate�� �ϴ� ����
	_Image.vecTrans.x	= _fX + _fWidth * 0.5f + 0.5f;	//	������ �ű�� ��
	_Image.vecTrans.y	= _fY + _fHeight * 0.5f + 0.5f;

	//	Init Texture
	if( _Image.pTex != NULL )
		_Image.pTex->Release();

	LoadTextureFromFile(	&_Image.pTex,
							_pFileName,
							static_cast< INT >( _fWidth ),
							static_cast< INT >( _fHeight ),
							D3DCOLOR_XRGB( 255, 0, 255 ) );
}

VOID GUIBase::CreateImage(	IMAGE& _Image, FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight )
{
	//	Init VertexBuffer, IndexBuffer
	VERTEX	Vertex[ 4 ];
	INDEX	Index[ 2 ];

	Vertex[ 0 ].vecPosition = D3DXVECTOR3( -0.5f,	-0.5f,	1.0f );
	Vertex[ 1 ].vecPosition = D3DXVECTOR3( 0.5f,	-0.5f,	1.0f );
	Vertex[ 2 ].vecPosition = D3DXVECTOR3( -0.5f,	0.5f,	1.0f );
	Vertex[ 3 ].vecPosition = D3DXVECTOR3( 0.5f,	0.5f,	1.0f );

	Vertex[ 0 ].vecTexcoord = D3DXVECTOR2( 0.0f, 0.0f );
	Vertex[ 1 ].vecTexcoord = D3DXVECTOR2( 1.0f, 0.0f );
	Vertex[ 2 ].vecTexcoord = D3DXVECTOR2( 0.0f, 1.0f );
	Vertex[ 3 ].vecTexcoord = D3DXVECTOR2( 1.0f, 1.0f );

	Index[ 0 ]._0 = 0;
	Index[ 0 ]._1 = 1;
	Index[ 0 ]._2 = 3;

	Index[ 1 ]._0 = 0;
	Index[ 1 ]._1 = 3;
	Index[ 1 ]._2 = 2;

	_Image.iNumVertices = 4;
	_Image.iNumIndices	= 2;

	CreateVB( &_Image.pVB, _Image.iNumVertices, sizeof( VERTEX ), VERTEX::FVF );
	SetVB( _Image.pVB, Vertex, _Image.iNumVertices, sizeof( VERTEX ) );

	CreateIB( &_Image.pIB, _Image.iNumIndices, sizeof( INDEX ) );
	SetIB( _Image.pIB, Index, _Image.iNumIndices, sizeof( INDEX ) );

	//	Init Matrix
	_Image.vecScale.x	= _fWidth;
	_Image.vecScale.y	= _fHeight;

	//	Rotate�� �ϴ� ����
	_Image.vecTrans.x	= _fX + _fWidth * 0.5f + 0.5f;	//	������ �ű�� ��
	_Image.vecTrans.y	= _fY + _fHeight * 0.5f + 0.5f;

	//	Init Texture
	if( _Image.pTex != NULL )
		_Image.pTex->Release();

	if( FAILED( m_pd3dDevice->CreateTexture(	static_cast<INT>( _fWidth ),
												static_cast<INT>( _fHeight ),
												0,
												0,
												D3DFMT_X8R8G8B8,
												D3DPOOL_MANAGED,
												&_Image.pTex,
												NULL ) ) )
	{
		MessageBox( NULL, L"CreateTexture failed...", NULL, MB_OK );
		return;
	}
	//	������� �ʱ�ȭ
	D3DLOCKED_RECT	d3drt;
	_Image.pTex->LockRect( 0, &d3drt, NULL, 0 );

	UINT* pBits = static_cast< UINT* >( d3drt.pBits );
	
	memset( pBits, 0xffff00ff, sizeof( UINT ) * _fWidth * _fHeight );
	
	_Image.pTex->UnlockRect( 0 );
}

VOID GUIBase::RenderImage( IMAGE& _Image )
{
	//	SetWorldMatrix
	SetMatrix( _Image.vecScale, _Image.vecRotate, _Image.vecTrans );

	//	SetProjectionMatrix
	D3DVIEWPORT9 vp;
	GUIBase::m_pd3dDevice->GetViewport( &vp );
	D3DXMATRIX matOrtho;
	D3DXMatrixIdentity( &matOrtho );
	//D3DXMatrixOrthoLH( &matOrtho, vp.Width, vp.Height, 0, 1 );	//	�ٸ� ������ ����?? �𸣰ٳ�;;
	D3DXMatrixOrthoOffCenterLH( &matOrtho, 0.0f, static_cast< FLOAT>( vp.Width ), static_cast< FLOAT >( vp.Height ), 0.0f, 0, 1 );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matOrtho );

	//	SetTexture
	if( _Image.pTex != NULL )
		m_pd3dDevice->SetTexture( 0, _Image.pTex );

	//	Draw
	m_pd3dDevice->SetStreamSource( 0, _Image.pVB, 0, sizeof( VERTEX ) );
	m_pd3dDevice->SetFVF( VERTEX::FVF );
	m_pd3dDevice->SetIndices( _Image.pIB );
	m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, _Image.iNumVertices, 0, _Image.iNumIndices );

	//	Init WorldMatrix
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity( &matIdentity );
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matIdentity );

	//	Init ProjectionMatrix
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matIdentity );
}

VOID GUIBase::SetMatrix( D3DXVECTOR3& _vecScale, D3DXVECTOR3& _vecRotate, D3DXVECTOR3& _vecTranslate )
{
	D3DXMATRIX matScale;
	D3DXMatrixIdentity( &matScale );
	D3DXMatrixScaling( &matScale, _vecScale.x, _vecScale.y, _vecScale.z );

	D3DXMATRIX matRotateX;
	D3DXMatrixIdentity( &matRotateX );
	D3DXMatrixRotationX( &matRotateX, _vecRotate.x );

	D3DXMATRIX matRotateY;
	D3DXMatrixIdentity( &matRotateY );
	D3DXMatrixRotationY( &matRotateY, _vecRotate.y );

	D3DXMATRIX matRotateZ;
	D3DXMatrixIdentity( &matRotateZ );
	D3DXMatrixRotationZ( &matRotateZ, _vecRotate.z );

	D3DXMATRIX matTranslate;
	D3DXMatrixIdentity( &matTranslate );
	D3DXMatrixTranslation( &matTranslate, _vecTranslate.x, _vecTranslate.y, _vecTranslate.z );

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	matWorld = matScale * matRotateX * matRotateY * matRotateZ * matTranslate;
	
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
}

HRESULT	GUIBase::CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF )
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
HRESULT	GUIBase::SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size )
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

HRESULT	GUIBase::CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size )
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

HRESULT	GUIBase::SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size )
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

HRESULT GUIBase::LoadTextureFromFile( LPDIRECT3DTEXTURE9* _ppTexture, LPCWSTR FileName, D3DCOLOR colClear )
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
					colClear,				//	������ �Ǵ� D3DCOLOR��, �÷�Ű�� ��ȿ�� �ϴ� ���� 0�� ����
					NULL,					//	�ҽ� �̹��� ���ϳ��� �������� ����� �����ϴ� D3DXIMAGE INFO ����ü�� ������
					NULL,					//	�����ϴ� 256 �� �ȷ�Ʈ�� ��Ÿ���� PALETTEENTRY ����ü�� ������
					&(*_ppTexture) ) ) )		//	������ ť�� �ؽ�ó ��ü�� ��Ÿ���� IDirect3DTexture9 �������̽��� ������ �ּ�
	{
		MessageBox( NULL, L"LoadTextureFromFile() Failed.", NULL, MB_OK );
		return E_FAIL;
	}

	return S_OK;

}

HRESULT GUIBase::LoadTextureFromFile( LPDIRECT3DTEXTURE9* _ppOutTexture, LPCWSTR FileName, UINT iWidth, UINT iHeight, D3DCOLOR colClear )
{
	if( FAILED( D3DXCreateTextureFromFileEx(	
					m_pd3dDevice,
					FileName,				//	�����Ϸ� ������ Unicode�� �䱸�ϰ� �ִ� ��� ������ Ÿ�� LPCSTR�� LPCWSTR�� �ȴ�
					iWidth,					//	���� ũ�⸦ �޾ƿ´� 2�� �¼��ε� �޾ƿü� �ִ�	D3DX_DEFAULT_NONPOW2
					iHeight,				//	���� ũ�⸦ �޾ƿ´�							D3DX_DEFAULT_NONPOW2
					D3DX_DEFAULT,			//	�䱸�Ǵ� �ӷ����� ��, �� ���� 0�Ǵ� D3DX_DEFAULT�� ��� ������ �Ӹ� ü�� ����
					NULL,					//	���� �ؽ���
					D3DFMT_A8R8G8B8,		//	�ؽ�ó ���� D3DFMT_UNKKNOWN�� ��� ������ ���Ϸκ��� ���
					D3DPOOL_MANAGED,		//	�ؽ�ó�� ��ġó�� �Ǵ� �޸� Ŭ������ ����Ѵ�
					D3DX_DEFAULT,			//	���͸� ���, D3DX_DEFAULT�� D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER �� ����
					D3DX_DEFAULT,			//	���͸� ���, D3DX_DEFAULT�� D3DX_FILTER_BOX �� ����
					colClear,				//	������ �Ǵ� D3DCOLOR��, �÷�Ű�� ��ȿ�� �ϴ� ���� 0�� ����
					NULL,					//	�ҽ� �̹��� ���ϳ��� �������� ����� �����ϴ� D3DXIMAGE INFO ����ü�� ������
					NULL,					//	�����ϴ� 256 �� �ȷ�Ʈ�� ��Ÿ���� PALETTEENTRY ����ü�� ������
					&(*_ppOutTexture) ) ) )	//	������ ť�� �ؽ�ó ��ü�� ��Ÿ���� IDirect3DTexture9 �������̽��� ������ �ּ�
	{
		MessageBox( NULL, L"LoadTextureFromFile() Failed.", NULL, MB_OK );
		return E_FAIL;
	}

	return S_OK;

}

//	DrawFontOnTexture
VOID GUIBase::SetFont( LPWSTR _FontType, INT _iSize, INT _iWeight )
{
	//	Cleanup
	if( m_hFont != NULL )
		DeleteObject( m_hFont );

	m_hFont = CreateFont(	_iSize, 0, 0, 0,			//	����, ��, ��ü ����, �� ������ ����		���̰��� �ְ� ���� 0���� �ָ� ���� �ڵ����� ����
							_iWeight,					//	����	���뱽��� FW_NORMAL
							FALSE, FALSE, FALSE,		//	�����ü �Ӽ�, ���� �Ӽ�, ����� �Ӽ�	DWORD�������� BOOL��ó�� ���
							ANSI_CHARSET,				//	���ڼ�	ANSI_CHARSET : ��������� ����ϴ� ���ڼ�
							OUT_DEFAULT_PRECIS,			//	��� ��Ȯ��		OUT_DEFAULT_PRECIS : ��Ʈ ������ ����Ʈ ������ �����Ѵ�
							CLIP_DEFAULT_PRECIS,		//	Ŭ���� ��Ȯ��	CLIP_DEFAULT_PRECIS : ����Ʈ Ŭ���� ����
							NONANTIALIASED_QUALITY,		//	���� ��Ʈ�� ������ ��Ʈ�� �󸶳� ������ų ���ΰ�
							DEFAULT_PITCH | FF_SWISS,	//	��Ʈ�� ��ġ�� �׷��� ����
							_FontType );
}
//	DrawFontOnTexture
VOID GUIBase::DrawFontOnTexture( LPWSTR _Str, COLORREF _Color, LPDIRECT3DTEXTURE9 _pTexture, INT _iWidth, INT  _iHeight )
{
	if( _pTexture == NULL )
	{
		MessageBox( NULL, L"DrawFontOnTexture{... _pTexture == NULL }", NULL, MB_OK );
		return;
	}

	HDC			hDC;
	HBITMAP		hBit;
	LPVOID		pBits;
	BITMAPINFO	BitInfo;

	ZeroMemory( &BitInfo, sizeof( BITMAPINFO ) );

	BitInfo.bmiHeader.biSize			=	sizeof( BITMAPINFOHEADER );	//	����ü ũ��
	BitInfo.bmiHeader.biWidth			=	_iWidth;
	BitInfo.bmiHeader.biHeight			=	_iHeight;
	BitInfo.bmiHeader.biPlanes			=	1;							//	��ǥ������ �÷��� ��( �̹��� ���, bitmap�� layer�� �������� �������� �׻� 1 )
	BitInfo.bmiHeader.biBitCount		=	32;							//	�� �ȼ��� ��Ʈ��
	BitInfo.bmiHeader.biCompression		=	BI_RGB;						//	������( BI_RGB �Ǵ� 0 : �������Ʈ�� )
	BitInfo.bmiHeader.biSizeImage		=	0;							//	��Ʈ�� ����ũ��
	BitInfo.bmiHeader.biXPelsPerMeter	=	100;						//	���ʹ� �ȼ���
	BitInfo.bmiHeader.biYPelsPerMeter	=	100;						//	���ʹ� �ȼ���
	BitInfo.bmiHeader.biClrUsed			=	0;							//	���� �÷���
	BitInfo.bmiHeader.biClrImportant	=	0;							//	��Ʈ�� ���÷��̿� ���Ǵ� �÷� ��

	hDC		= CreateCompatibleDC( NULL );
	hBit	= CreateDIBSection( hDC, &BitInfo, DIB_RGB_COLORS, &pBits, NULL, 0 );

	RECT rt;
	SetRect( &rt, 0, 0, _iWidth, _iHeight );

	HBRUSH	hBackgroundBrush = CreateSolidBrush( 0x00ff00ff );
	
	SelectObject( hDC, hBit );
	SelectObject( hDC, m_hFont );

	FillRect( hDC, &rt, hBackgroundBrush );	
	SetBkMode( hDC, TRANSPARENT );		//	TRANSPARENT : ������ ��� ����
	SetTextColor( hDC, _Color );
	
	//	2,2 ��� �»��, �ߴ�, ���ϴ� 3������ ������ �� �� �ְ� �ϸ� ���� ��
	TextOut( hDC, 2, 2, _Str, lstrlen( _Str ) );
	
	D3DLOCKED_RECT	d3drt;
	_pTexture->LockRect( 0, &d3drt, NULL, 0 );

	UINT* pSrc32 = static_cast< UINT* >( pBits );
	UINT* pDst32 = static_cast< UINT* >( d3drt.pBits );
	
	pSrc32 += ( _iHeight * _iWidth ) - _iWidth;
	UINT* pTemp = pSrc32;

	for( INT y=0 ; y<_iHeight ; y++ )
	{
		for( INT x=0 ; x<_iWidth ; x++ )
		{
			if( (*pSrc32) != 0x00ff00ff )
				(*pDst32) = (*pSrc32) | 0xff000000;

			pSrc32++;
			pDst32++;
		}
		pTemp -= _iWidth;
		pSrc32 = pTemp;
	}
	
	_pTexture->UnlockRect( 0 );

	DeleteObject( hBackgroundBrush );
	DeleteObject( hBit );
	DeleteDC( hDC );

}

VOID GUIBase::ImageScale( IMAGE& _Image, FLOAT _fX, FLOAT _fY, FLOAT _fZ )
{
	_Image.vecScale.x = _fX;
	_Image.vecScale.y = _fY;
	_Image.vecScale.z = _fZ;
}
VOID GUIBase::ImageTranslate( IMAGE& _Image, FLOAT _fX, FLOAT _fY, FLOAT _fZ )
{
	_Image.vecTrans.x = _fX;
	_Image.vecTrans.y = _fY;
	_Image.vecTrans.z = _fZ;
}