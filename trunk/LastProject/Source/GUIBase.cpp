#include "Stdafx.h"
#include "GUIBase.h"

VOID GUIBase::Initialize()
{
	//	DrawFontOnTexture
	m_hFont = NULL;
	//	End

	//	Test
	m_iFrameSpeed = 50;
	//	End
}

VOID GUIBase::Release()
{
}

VOID GUIBase::CreateImage( IMAGE& _Image, DWORD _dPivotType, FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, LPCWSTR _pFileName )
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

	switch( _dPivotType )
	{
	case GBS_TOPLEFT:
		_Image.vecTrans.x	= _fX + _fWidth * 0.5f + 0.5f;	//	중점을 옮기는 것
		_Image.vecTrans.y	= _fY + _fHeight * 0.5f + 0.5f;	//	fX, fY가 사각형의 좌상단을 가리킨다
		break;
	case GBS_CENTER:
		_Image.vecTrans.x	= _fX;
		_Image.vecTrans.y	= _fY;
		break;
	default:
		MessageBox( NULL, L"CreateImage( ..., _dPivotType, ... ) error", NULL, MB_OK );
		break;
	}

	//	Init Texture
	LPTEXTURE pTexture = new TEXTURE;
	pTexture->iFrameSpeed = m_iFrameSpeed;

	if( _pFileName != NULL )
	{	//	_pFileName != NULL LoadTexture
		LoadTextureFromFile(	&pTexture->pTex,
								_pFileName,
								static_cast<INT>( _fWidth ),
								static_cast<INT>( _fHeight ),
								D3DCOLOR_XRGB( 255, 0, 255 ) );

	}
	else
	{	//	_pFileName == NULL CreateTexture
		if( FAILED( m_pd3dDevice->CreateTexture(	static_cast<INT>( _fWidth ),
													static_cast<INT>( _fHeight ),
													0,
													0,
													D3DFMT_X8R8G8B8,
													D3DPOOL_MANAGED,
													&pTexture->pTex,
													NULL ) ) )
		{
			MessageBox( NULL, L"CreateImage()::CreateTexture failed...", NULL, MB_OK );
			return;
		}
		//	흰색으로 초기화
		D3DLOCKED_RECT	d3drt;
		pTexture->pTex->LockRect( 0, &d3drt, NULL, 0 );
	
		UINT* pBits = static_cast<UINT*>( d3drt.pBits );
		
		INT iWidth	= static_cast<INT>( _fWidth );
		INT iHeight	= static_cast<INT>( _fHeight );
		memset( pBits, 0xffff00ff, sizeof( UINT ) * iWidth * iHeight );
		
		pTexture->pTex->UnlockRect( 0 );
	}
	
	_Image.vec2Tex.resize( 1 );
	_Image.vec2Tex[ 0 ].push_back( pTexture );
}

VOID GUIBase::CreateImage( IMAGE& _Image, IMAGEPARAM& _imgParam )
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
	_Image.vecScale.x	= _imgParam.fWidth;//_fWidth;
	_Image.vecScale.y	= _imgParam.fHeight;//_fHeight;

	switch( _imgParam.dPivotType )
	{
	case GBS_TOPLEFT:
		_Image.vecTrans.x	= _imgParam.fX + _imgParam.fWidth * 0.5f + 0.5f;	//	중점을 옮기는 것
		_Image.vecTrans.y	= _imgParam.fY + _imgParam.fHeight * 0.5f + 0.5f;	//	fX, fY가 사각형의 좌상단을 가리킨다
		break;
	case GBS_CENTER:
		_Image.vecTrans.x	= _imgParam.fX;
		_Image.vecTrans.y	= _imgParam.fY;
		break;
	default:
		MessageBox( NULL, L"CreateImage( ..., dPivotType, ... ) error", NULL, MB_OK );
		break;
	}

	////	Init Texture
	//LPTEXTURE pTexture = new TEXTURE;
	//pTexture->iFrameSpeed = m_iFrameSpeed;

	//	vec2FileName Error Check
	INT iNumAnimation	= _imgParam.vec2FileName.size();
	INT	iNumFrame		= 0;
	if( iNumAnimation != 0 )
		iNumFrame = _imgParam.vec2FileName[ 0 ].size();

	//	Not FileName
	if( iNumAnimation == 0 || iNumFrame == 0 )
	{
		//	Init Texture
		LPTEXTURE pTexture = new TEXTURE;
		pTexture->iFrameSpeed = m_iFrameSpeed;

		//	_pFileName == NULL CreateTexture
		if( FAILED( m_pd3dDevice->CreateTexture(	static_cast<INT>( _imgParam.fWidth ),
													static_cast<INT>( _imgParam.fHeight ),
													0,
													0,
													D3DFMT_X8R8G8B8,
													D3DPOOL_MANAGED,
													&pTexture->pTex,
													NULL ) ) )
		{
			MessageBox( NULL, L"CreateImage()::CreateTexture failed...", NULL, MB_OK );
			return;
		}
		//	흰색으로 초기화
		D3DLOCKED_RECT	d3drt;
		pTexture->pTex->LockRect( 0, &d3drt, NULL, 0 );
	
		UINT* pBits = static_cast<UINT*>( d3drt.pBits );
		
		INT iWidth	= static_cast<INT>( _imgParam.fWidth );
		INT iHeight	= static_cast<INT>( _imgParam.fHeight );
		memset( pBits, 0xffff00ff, sizeof( UINT ) * iWidth * iHeight );
		
		pTexture->pTex->UnlockRect( 0 );

		_Image.vec2Tex.resize( 1 );
		_Image.vec2Tex[ 0 ].push_back( pTexture );

		return;
	}
	
	//	Load Texture
	_Image.vec2Tex.resize( iNumAnimation );
	for( INT j=0 ; j<iNumAnimation ; j++ )
	{
		for( INT i=0 ; i<iNumFrame ; i++ )
			AddTexture( j, _Image, _imgParam.vec2FileName[ j ][ i ] );
	}
}

VOID GUIBase::CreateImage2D( IMAGE2D& _Image2D, IMAGEPARAM& _imgParam )
{
	_Image2D.vecPosition.x = _imgParam.fX;
	_Image2D.vecPosition.y = _imgParam.fY;
	_Image2D.vecPosition.z = 0.0f;

	SetRect( &_Image2D.rtSource,	0, 0,
									static_cast<INT>( _imgParam.fWidth ),
									static_cast<INT>( _imgParam.fHeight ) );
								

	switch( _imgParam.dPivotType )
	{
	case GBS_TOPLEFT:
		_Image2D.vecCenter.x	= 0.0f;
		_Image2D.vecCenter.y	= 0.0f;
		_Image2D.vecCenter.z	= 0.0f;
		break;
	case GBS_CENTER:
		_Image2D.vecCenter.x	= _imgParam.fWidth * 0.5f;	//	중점을 옮기는 것
		_Image2D.vecCenter.y	= _imgParam.fHeight * 0.5f;	//	fX, fY가 사각형의 좌상단을 가리킨다
		_Image2D.vecCenter.z	= 0.0f;
		break;
	default:
		MessageBox( NULL, L"CreateImage( ..., dPivotType, ... ) error", NULL, MB_OK );
		break;
	}

	//	Init Texture
	//LPTEXTURE pTexture = new TEXTURE;
	//pTexture->iFrameSpeed = m_iFrameSpeed;

	//	vec2FileName Error Check
	INT iNumAnimation	= _imgParam.vec2FileName.size();
	INT	iNumFrame		= 0;
	if( iNumAnimation != 0 )
		iNumFrame = _imgParam.vec2FileName[ 0 ].size();

	//	Not FileName
	if( iNumAnimation == 0 || iNumFrame == 0 )
	{
		LPTEXTURE pTexture = new TEXTURE;
		pTexture->iFrameSpeed = m_iFrameSpeed;
		//	_pFileName == NULL CreateTexture
		if( FAILED( m_pd3dDevice->CreateTexture(	static_cast<INT>( _imgParam.fWidth ),
													static_cast<INT>( _imgParam.fHeight ),
													0,
													0,
													D3DFMT_X8R8G8B8,
													D3DPOOL_MANAGED,
													&pTexture->pTex,
													NULL ) ) )
		{
			MessageBox( NULL, L"CreateImage()::CreateTexture failed...", NULL, MB_OK );
			return;
		}
		//	흰색으로 초기화
		D3DLOCKED_RECT	d3drt;
		pTexture->pTex->LockRect( 0, &d3drt, NULL, 0 );
	
		UINT* pBits = static_cast<UINT*>( d3drt.pBits );
		
		INT iWidth	= static_cast<INT>( _imgParam.fWidth );
		INT iHeight	= static_cast<INT>( _imgParam.fHeight );
		memset( pBits, 0xffff00ff, sizeof( UINT ) * iWidth * iHeight );
		
		pTexture->pTex->UnlockRect( 0 );

		_Image2D.vec2Tex.resize( 1 );
		_Image2D.vec2Tex[ 0 ].push_back( pTexture );

		return;
	}
	
	//	Load Texture
	_Image2D.vec2Tex.resize( iNumAnimation );
	for( INT j=0 ; j<iNumAnimation ; j++ )
	{
		for( INT i=0 ; i<iNumFrame ; i++ )
			AddTexture( j, _Image2D, _imgParam.vec2FileName[ j ][ i ] );
	}

}


VOID GUIBase::RenderImage( IMAGE& _Image )
{
	//	Set World, Projection Matrix
	SetMatrix( _Image.vecScale, _Image.vecRotate, _Image.vecTrans );

	//	SetTexture
	LPTEXTURE pTexture = _Image.vec2Tex[ _Image.iCurrentAnimation ][ 0 ];
	
	if( _Image.bAniPlay )
	{
		pTexture = _Image.vec2Tex[ _Image.iCurrentAnimation ][ _Image.iCurrentFrame ];

		_Image.dCurrentTime = timeGetTime();

		if( _Image.dCurrentTime > _Image.dBeginTime + pTexture->iFrameSpeed )
		{
			_Image.iCurrentFrame++;
	
			_Image.dBeginTime = _Image.dCurrentTime;
	
			if( _Image.iCurrentFrame >= _Image.vec2Tex[ _Image.iCurrentAnimation ].size() )
				_Image.iCurrentFrame = 0;
		}
	}

	if( pTexture->pTex != NULL )
		m_pd3dDevice->SetTexture( 0, pTexture->pTex );
	

	//	Draw
	m_pd3dDevice->SetStreamSource( 0, _Image.pVB, 0, sizeof( VERTEX ) );
	m_pd3dDevice->SetFVF( VERTEX::FVF );
	m_pd3dDevice->SetIndices( _Image.pIB );
	m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, _Image.iNumVertices, 0, _Image.iNumIndices );

	//	Identity WorldMatrix
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity( &matIdentity );
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matIdentity );

	//	Identity ProjectionMatrix
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matIdentity );
}

VOID GUIBase::RenderImage2D( IMAGE2D& _Image2D )
{
	//	SetTexture
	LPTEXTURE pTexture = _Image2D.vec2Tex[ _Image2D.iCurrentAnimation ][ 0 ];
	
	if( _Image2D.bAniPlay )
	{
		pTexture = _Image2D.vec2Tex[ _Image2D.iCurrentAnimation ][ _Image2D.iCurrentFrame ];

		_Image2D.dCurrentTime = timeGetTime();

		if( _Image2D.dCurrentTime > _Image2D.dBeginTime + pTexture->iFrameSpeed )
		{
			_Image2D.iCurrentFrame++;
	
			_Image2D.dBeginTime = _Image2D.dCurrentTime;
	
			if( _Image2D.iCurrentFrame >= _Image2D.vec2Tex[ _Image2D.iCurrentAnimation ].size() )
				_Image2D.iCurrentFrame = 0;
		}
	}

	//	Draw
	//	Begin, End를 다른 곳으로 옮기자
	m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
	m_pSprite->Draw( pTexture->pTex, &_Image2D.rtSource, &_Image2D.vecCenter, &_Image2D.vecPosition, 0xffff00ff );
	m_pSprite->End();
}

VOID GUIBase::SetMatrix( D3DXVECTOR3& _vecScale, D3DXVECTOR3& _vecRotate, D3DXVECTOR3& _vecTranslate )
{
	//	Set World Matrix
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

	//	Set Projection Matrix
	D3DVIEWPORT9 vp;
	GUIBase::m_pd3dDevice->GetViewport( &vp );
	D3DXMATRIX matOrtho;
	D3DXMatrixIdentity( &matOrtho );
	//D3DXMatrixOrthoLH( &matOrtho, vp.Width, vp.Height, 0, 1 );	//	다른 이유가 뭘까?? 모르겟네;;
	D3DXMatrixOrthoOffCenterLH( &matOrtho, 0.0f, static_cast< FLOAT>( vp.Width ), static_cast< FLOAT >( vp.Height ), 0.0f, 0, 1 );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matOrtho );
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

HRESULT GUIBase::LoadTextureFromFile( LPDIRECT3DTEXTURE9* _ppOutTexture, LPCWSTR FileName, UINT iWidth, UINT iHeight, D3DCOLOR colClear )
{
	if( FAILED( D3DXCreateTextureFromFileEx(	
					m_pd3dDevice,
					FileName,				//	컴파일러 설정이 Unicode를 요구하고 있는 경우 데이터 타입 LPCSTR은 LPCWSTR이 된다
					iWidth,					//	원본 크기를 받아온다 2의 승수로도 받아올수 있다	D3DX_DEFAULT_NONPOW2
					iHeight,				//	원본 크기를 받아온다							D3DX_DEFAULT_NONPOW2
					D3DX_DEFAULT,			//	요구되는 밉레벨의 수, 이 값이 0또는 D3DX_DEFAULT의 경우 완전한 밉맵 체인 생성
					NULL,					//	동적 텍스쳐
					D3DFMT_A8R8G8B8,		//	텍스처 포멧 D3DFMT_UNKKNOWN의 경우 포멧은 파일로부터 취득
					D3DPOOL_MANAGED,		//	텍스처의 배치처가 되는 메모리 클래스를 기술한다
					D3DX_DEFAULT,			//	필터링 방법, D3DX_DEFAULT는 D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER 와 같다
					D3DX_DEFAULT,			//	필터링 방법, D3DX_DEFAULT는 D3DX_FILTER_BOX 와 같다
					colClear,				//	투명이 되는 D3DCOLOR값, 컬러키를 무효로 하는 경우는 0을 지정
					NULL,					//	소스 이미지 파일내의 데이터의 기술을 저장하는 D3DXIMAGE INFO 구조체의 포인터
					NULL,					//	저장하는 256 색 팔레트를 나타내는 PALETTEENTRY 구조체의 포인터
					&(*_ppOutTexture) ) ) )	//	생성된 큐브 텍스처 개체를 나타내는 IDirect3DTexture9 인터페이스의 포인터 주소
	{
		CDebugConsole::GetInstance()->Messagef( L"%s\n", FileName );
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

	m_hFont = CreateFont(	_iSize, 0, 0, 0,			//	높이, 폭, 전체 각도, 한 글자의 각도		높이값만 주고 폭을 0으로 주면 폭은 자동으로 설정
							_iWeight,					//	굵기	보통굵기는 FW_NORMAL
							FALSE, FALSE, FALSE,		//	기울임체 속성, 밑줄 속성, 취소헌 속성	DWORD형이지만 BOOL형처럼 사용
							ANSI_CHARSET,				//	문자셋	ANSI_CHARSET : 윈도우즈에서 사용하는 문자셋
							OUT_DEFAULT_PRECIS,			//	출력 정확도		OUT_DEFAULT_PRECIS : 폰트 매퍼의 디폴트 동작을 정의한다
							CLIP_DEFAULT_PRECIS,		//	클리핑 정확도	CLIP_DEFAULT_PRECIS : 디폴트 클리핑 동작
							NONANTIALIASED_QUALITY,		//	논리적 폰트를 물리적 폰트에 얼마나 근접시킬 것인가
							DEFAULT_PITCH | FF_SWISS,	//	폰트의 피치와 그룹을 설정
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

	BitInfo.bmiHeader.biSize			=	sizeof( BITMAPINFOHEADER );	//	구조체 크기
	BitInfo.bmiHeader.biWidth			=	_iWidth;
	BitInfo.bmiHeader.biHeight			=	_iHeight;
	BitInfo.bmiHeader.biPlanes			=	1;							//	목표장차의 플레인 수( 이미지 장수, bitmap은 layer가 존재하지 않음으로 항상 1 )
	BitInfo.bmiHeader.biBitCount		=	32;							//	각 픽셀의 비트수
	BitInfo.bmiHeader.biCompression		=	BI_RGB;						//	압축방법( BI_RGB 또는 0 : 무압축비트맵 )
	BitInfo.bmiHeader.biSizeImage		=	0;							//	비트맵 영상크기
	BitInfo.bmiHeader.biXPelsPerMeter	=	100;						//	미터당 픽셀수
	BitInfo.bmiHeader.biYPelsPerMeter	=	100;						//	미터당 픽셀수
	BitInfo.bmiHeader.biClrUsed			=	0;							//	사용된 컬러수
	BitInfo.bmiHeader.biClrImportant	=	0;							//	비트맵 디스플레이에 사용되는 컬러 수

	hDC		= CreateCompatibleDC( NULL );
	hBit	= CreateDIBSection( hDC, &BitInfo, DIB_RGB_COLORS, &pBits, NULL, 0 );

	RECT rt;
	SetRect( &rt, 0, 0, _iWidth, _iHeight );

	HBRUSH	hBackgroundBrush = CreateSolidBrush( 0x00ff00ff );
	
	SelectObject( hDC, hBit );
	SelectObject( hDC, m_hFont );

	FillRect( hDC, &rt, hBackgroundBrush );	
	SetBkMode( hDC, TRANSPARENT );		//	TRANSPARENT : 투명한 배경 색상
	SetTextColor( hDC, _Color );
	
	//	2,2 대신 좌상단, 중단, 좌하단 3가지로 나누어 할 수 있게 하면 좋을 듯
	TextOut( hDC, 10, 10, _Str, lstrlen( _Str ) );
	
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

VOID GUIBase::AddTexture( INT _iNumAni, IMAGE& _Image, LPWSTR _pFileName )
{
	_Image.bAniPlay = TRUE;

	//	Init Texture
	LPTEXTURE pTexture		= new TEXTURE;
	pTexture->iFrameSpeed	= m_iFrameSpeed;

	LoadTextureFromFile(	&pTexture->pTex,
							_pFileName,
							static_cast<UINT>( _Image.vecScale.x ),
							static_cast<UINT>( _Image.vecScale.y ),
							D3DCOLOR_XRGB( 255, 0, 255 ) );

	_Image.vec2Tex[ _iNumAni ].push_back( pTexture );
}

VOID GUIBase::AddTexture( INT _iNumAni, IMAGE2D& _Image2D, LPWSTR _pFileName )
{
	_Image2D.bAniPlay = TRUE;

	//	Init Texture
	LPTEXTURE pTexture		= new TEXTURE;
	pTexture->iFrameSpeed	= m_iFrameSpeed;

	LoadTextureFromFile(	&pTexture->pTex,
							_pFileName,
							_Image2D.rtSource.right,
							_Image2D.rtSource.bottom,
							D3DCOLOR_XRGB( 255, 0, 255 ) );

	_Image2D.vec2Tex[ _iNumAni ].push_back( pTexture );
}

VOID GUIBase::AddFileName( INT _iNumAni, IMAGEPARAM& _imgParam, LPWSTR _pFileName )
{
	INT iNumAnimation = _imgParam.vec2FileName.size();

	//	순서대로 넣는 것이 아니면 중간에 빼먹고 하는 경우가 있으므로 ERROR 처리
	if( _iNumAni > iNumAnimation )
		MessageBox( NULL, L"AddFileName(){ ... resize() failed ... }", NULL, MB_OK );

	//	같으면 크기를 늘린다
	if( _iNumAni == iNumAnimation )
		_imgParam.vec2FileName.resize( _iNumAni + 1 );

	LPWSTR pFileName = new TCHAR[ 1024 ];
	
	lstrcpy( pFileName, _pFileName );

	_imgParam.vec2FileName[ _iNumAni ].push_back( pFileName );
}
