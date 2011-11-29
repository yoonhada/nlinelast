#include "stdafx.h"
#include "GUIBase.h"

VOID GUIBase::Initialize()
{
}

VOID GUIBase::Release()
{
}

VOID GUIBase::CreateImage3D( IMAGE3D& _Image3D, IMAGEPARAM& _imgParam )
{
	//	Init VertexBuffer, IndexBuffer
	GUIVERTEX	Vertex[ 4 ];
	GUIINDEX	Index[ 2 ];

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

	INT iNumVertices	= 4;
	INT iNumIndices		= 2;

	CreateVB( &_Image3D.pVB, iNumVertices, sizeof( GUIVERTEX ), GUIVERTEX::FVF );
	SetVB( _Image3D.pVB, Vertex, iNumVertices, sizeof( GUIVERTEX ) );

	CreateIB( &_Image3D.pIB, iNumIndices, sizeof( GUIINDEX ) );
	SetIB( _Image3D.pIB, Index, iNumIndices, sizeof( GUIINDEX ) );

	//	Init Matrix
	_Image3D.vecScale.x	= _imgParam.fWidth;//_fWidth;
	_Image3D.vecScale.y	= _imgParam.fHeight;//_fHeight;

	switch( _imgParam.dPivotType )
	{
	case GBS_TOPLEFT:
		_Image3D.vecTrans.x	= _imgParam.fX + _imgParam.fWidth * 0.5f + 0.5f;	//	중점을 옮기는 것
		_Image3D.vecTrans.y	= _imgParam.fY + _imgParam.fHeight * 0.5f + 0.5f;	//	fX, fY가 사각형의 좌상단을 가리킨다
		break;
	case GBS_CENTER:
		_Image3D.vecTrans.x	= _imgParam.fX;
		_Image3D.vecTrans.y	= _imgParam.fY;
		break;
	default:
		MessageBox( NULL, L"CreateImage( ..., dPivotType, ... ) error", NULL, MB_OK );
		break;
	}

	//	Init Texture
	//	vec2FileName Error Check
	INT iNumAnimation	= _imgParam.vec2FileName.size();
	INT	iNumFrame		= 0;
	if( iNumAnimation != 0 )
		iNumFrame = _imgParam.vec2FileName[ 0 ].size();

	//	Not FileName
	if( iNumAnimation == 0 || iNumFrame == 0 )
	{
		//	Init Texture
		LPGUITEXTURE pGUITexture	= new GUITEXTURE;
		pGUITexture->iFrameSpeed		= 1000;

		//	_pFileName == NULL CreateTexture
		if( FAILED( m_pD3dDevice->CreateTexture(	static_cast<INT>( _imgParam.fWidth ),
													static_cast<INT>( _imgParam.fHeight ),
													0,
													0,
													D3DFMT_X8R8G8B8,
													D3DPOOL_MANAGED,
													&pGUITexture->pTex,
													NULL ) ) )
		{
			MessageBox( NULL, L"CreateImage()::CreateTexture failed...", NULL, MB_OK );
			return;
		}
		//	흰색으로 초기화
		D3DLOCKED_RECT	d3drt;
		pGUITexture->pTex->LockRect( 0, &d3drt, NULL, 0 );
	
		UINT* pBits = static_cast<UINT*>( d3drt.pBits );
		
		INT iWidth	= static_cast<INT>( _imgParam.fWidth );
		INT iHeight	= static_cast<INT>( _imgParam.fHeight );
		memset( pBits, 0xffffffff, sizeof( UINT ) * iWidth * iHeight );
		
		pGUITexture->pTex->UnlockRect( 0 );

		_Image3D.vec2Tex.resize( 1 );
		_Image3D.vec2Tex[ 0 ].push_back( pGUITexture );

		return;
	}
	
	//	Load Texture
	_Image3D.vec2Tex.resize( iNumAnimation );
	for( INT j=0 ; j<iNumAnimation ; j++ )
	{
		for( INT i=0 ; i<iNumFrame ; i++ )
			AddTexture( j, _Image3D, _imgParam.vec2FileName[ j ][ i ] );
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
	//	vec2FileName Error Check
	INT iNumAnimation	= _imgParam.vec2FileName.size();
	INT	iNumFrame		= 0;
	if( iNumAnimation != 0 )
		iNumFrame = _imgParam.vec2FileName[ 0 ].size();

	//	Not FileName
	if( iNumAnimation == 0 || iNumFrame == 0 )
	{
		LPGUITEXTURE pGUITexture	= new GUITEXTURE;
		pGUITexture->iFrameSpeed	= 1000;
		//	_pFileName == NULL CreateTexture
		if( FAILED( m_pD3dDevice->CreateTexture(	static_cast<INT>( _imgParam.fWidth ),
													static_cast<INT>( _imgParam.fHeight ),
													0,
													0,
													D3DFMT_X8R8G8B8,
													D3DPOOL_MANAGED,
													&pGUITexture->pTex,
													NULL ) ) )
		{
			MessageBox( NULL, L"CreateImage()::CreateTexture failed...", NULL, MB_OK );
			return;
		}
		//	흰색으로 초기화
		D3DLOCKED_RECT	d3drt;
		pGUITexture->pTex->LockRect( 0, &d3drt, NULL, 0 );
	
		UINT* pBits = static_cast<UINT*>( d3drt.pBits );
		
		INT iWidth	= static_cast<INT>( _imgParam.fWidth );
		INT iHeight	= static_cast<INT>( _imgParam.fHeight );
		memset( pBits, 0xffff00ff, sizeof( UINT ) * iWidth * iHeight );
		
		pGUITexture->pTex->UnlockRect( 0 );

		_Image2D.vec2Tex.resize( 1 );
		_Image2D.vec2Tex[ 0 ].push_back( pGUITexture );

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


VOID GUIBase::RenderImage3D( LPIMAGE3D _pImage3D )
{
	m_pD3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pD3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_pD3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	//	Set World, Projection Matrix
	SetMatrix( _pImage3D->vecScale, _pImage3D->vecRotate, _pImage3D->vecTrans );

	//	SetTexture
	LPGUITEXTURE pGUITexture = _pImage3D->vec2Tex[ _pImage3D->iCurrentAnimation ][ 0 ];
	
	if( _pImage3D->bAniPlay )
	{
		pGUITexture = _pImage3D->vec2Tex[ _pImage3D->iCurrentAnimation ][ _pImage3D->iCurrentFrame ];

		_pImage3D->dCurrentTime = timeGetTime();

		if( _pImage3D->dCurrentTime > _pImage3D->dBeginTime + pGUITexture->iFrameSpeed )
		{
			_pImage3D->iCurrentFrame++;
	
			_pImage3D->dBeginTime = _pImage3D->dCurrentTime;
	
			INT iFrameSize = _pImage3D->vec2Tex[ _pImage3D->iCurrentAnimation ].size();
			if( _pImage3D->iCurrentFrame >= iFrameSize )
				_pImage3D->iCurrentFrame = 0;
		}
	}

	if( pGUITexture->pTex != NULL )
		m_pD3dDevice->SetTexture( 0, pGUITexture->pTex );
	

	//	Draw
	INT iNumVertices	= 4;
	INT iNumIndices		= 2;

	m_pD3dDevice->SetStreamSource( 0, _pImage3D->pVB, 0, sizeof( GUIVERTEX ) );
	m_pD3dDevice->SetFVF( GUIVERTEX::FVF );
	m_pD3dDevice->SetIndices( _pImage3D->pIB );
	m_pD3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, iNumVertices, 0, iNumIndices );

	//	Identity WorldMatrix
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity( &matIdentity );
	m_pD3dDevice->SetTransform( D3DTS_WORLD, &matIdentity );

	//	Identity ProjectionMatrix
	m_pD3dDevice->SetTransform( D3DTS_PROJECTION, &matIdentity );

	m_pD3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

VOID GUIBase::RenderImage2D( LPIMAGE2D _pImage2D )
{
	//	SetTexture
	LPGUITEXTURE pGUITexture = _pImage2D->vec2Tex[ _pImage2D->iCurrentAnimation ][ 0 ];
	
	if( _pImage2D->bAniPlay )
	{
		pGUITexture = _pImage2D->vec2Tex[ _pImage2D->iCurrentAnimation ][ _pImage2D->iCurrentFrame ];

		_pImage2D->dCurrentTime = timeGetTime();

		if( _pImage2D->dCurrentTime > _pImage2D->dBeginTime + pGUITexture->iFrameSpeed )
		{
			_pImage2D->iCurrentFrame++;
	
			_pImage2D->dBeginTime = _pImage2D->dCurrentTime;
	
			INT iFrameSize = _pImage2D->vec2Tex[ _pImage2D->iCurrentAnimation ].size();
			if( _pImage2D->iCurrentFrame >= iFrameSize )
				_pImage2D->iCurrentFrame = 0;
		}
	}
	
	//	Draw
	//	Begin, End를 다른 곳으로 옮기자
	m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
	m_pSprite->Draw( pGUITexture->pTex, &_pImage2D->rtSource, &_pImage2D->vecCenter, &_pImage2D->vecPosition, 0xffffffff  );
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
	
	m_pD3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	//	Identity View Matrix
	D3DXMATRIX matView;
	D3DXMatrixIdentity( &matView );
	m_pD3dDevice->SetTransform( D3DTS_VIEW, &matView );

	//	Set Projection Matrix
	D3DVIEWPORT9 vp;
	GUIBase::m_pD3dDevice->GetViewport( &vp );
	D3DXMATRIX matOrtho;
	D3DXMatrixIdentity( &matOrtho );
	//D3DXMatrixOrthoLH( &matOrtho, vp.Width, vp.Height, 0, 1 );	//	다른 이유가 뭘까?? 모르겟네;;
	D3DXMatrixOrthoOffCenterLH( &matOrtho, 0.0f, static_cast< FLOAT>( vp.Width ), static_cast< FLOAT >( vp.Height ), 0.0f, 1, 2 );
	m_pD3dDevice->SetTransform( D3DTS_PROJECTION, &matOrtho );
}

HRESULT	GUIBase::CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF )
{
	if( FAILED( m_pD3dDevice->CreateVertexBuffer(	_nVertex * _Size,
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
	if( FAILED( m_pD3dDevice->CreateIndexBuffer(	_nIndex * _Size,
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

HRESULT GUIBase::LoadTextureFromFile( LPDIRECT3DTEXTURE9* _ppOutTexture, LPCWSTR _FileName, UINT _iWidth, UINT _iHeight, D3DCOLOR _colClear )
{
	if( FAILED( D3DXCreateTextureFromFileEx(	
					m_pD3dDevice,
					_FileName,				//	컴파일러 설정이 Unicode를 요구하고 있는 경우 데이터 타입 LPCSTR은 LPCWSTR이 된다
					_iWidth,				//	원본 크기를 받아온다 2의 승수로도 받아올수 있다	D3DX_DEFAULT_NONPOW2
					_iHeight,				//	원본 크기를 받아온다							D3DX_DEFAULT_NONPOW2
					D3DX_DEFAULT,			//	요구되는 밉레벨의 수, 이 값이 0또는 D3DX_DEFAULT의 경우 완전한 밉맵 체인 생성
					NULL,					//	동적 텍스쳐
					D3DFMT_A8R8G8B8,		//	텍스처 포멧 D3DFMT_UNKKNOWN의 경우 포멧은 파일로부터 취득
					D3DPOOL_MANAGED,		//	텍스처의 배치처가 되는 메모리 클래스를 기술한다
					D3DX_DEFAULT,			//	필터링 방법, D3DX_DEFAULT는 D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER 와 같다
					D3DX_DEFAULT,			//	필터링 방법, D3DX_DEFAULT는 D3DX_FILTER_BOX 와 같다
					_colClear,				//	투명이 되는 D3DCOLOR값, 컬러키를 무효로 하는 경우는 0을 지정
					NULL,					//	소스 이미지 파일내의 데이터의 기술을 저장하는 D3DXIMAGE INFO 구조체의 포인터
					NULL,					//	저장하는 256 색 팔레트를 나타내는 PALETTEENTRY 구조체의 포인터
					&(*_ppOutTexture) ) ) )	//	생성된 큐브 텍스처 개체를 나타내는 IDirect3DTexture9 인터페이스의 포인터 주소
	{
		CDebugConsole::GetInstance()->Messagef( L"%s\n", _FileName );
		MessageBox( NULL, L"LoadTextureFromFile() Failed.", NULL, MB_OK );
		return E_FAIL;
	}

	return S_OK;

}

VOID GUIBase::Image3DScale( LPIMAGE3D _pImage3D, FLOAT _fX, FLOAT _fY, FLOAT _fZ )
{
	_pImage3D->vecScale.x = _fX;
	_pImage3D->vecScale.y = _fY;
	_pImage3D->vecScale.z = _fZ;
}
VOID GUIBase::Image3DTranslate( LPIMAGE3D _pImage3D, FLOAT _fX, FLOAT _fY, FLOAT _fZ )
{
	_pImage3D->vecTrans.x = _fX;
	_pImage3D->vecTrans.y = _fY;
	_pImage3D->vecTrans.z = _fZ;
}

VOID GUIBase::AddTexture( INT _iNumAni, IMAGE3D& _Image3D, LPGUITEXTUREINFO _pGUITextureInfo )
{
	_Image3D.bAniPlay = TRUE;

	//	Init Texture
	LPGUITEXTURE pGUITexture	= new GUITEXTURE;
	pGUITexture->iFrameSpeed	= _pGUITextureInfo->iFrameSpeed;

	LoadTextureFromFile(	&pGUITexture->pTex,
							_pGUITextureInfo->FileName,
							static_cast<UINT>( _Image3D.vecScale.x ),
							static_cast<UINT>( _Image3D.vecScale.y ),
							D3DCOLOR_XRGB( 255, 0, 255 ) );

	_Image3D.vec2Tex[ _iNumAni ].push_back( pGUITexture );
}

VOID GUIBase::AddTexture( INT _iNumAni, IMAGE2D& _Image2D, LPGUITEXTUREINFO _pGUITextureInfo )
{
	_Image2D.bAniPlay = TRUE;

	//	Init Texture
	LPGUITEXTURE pGUITexture	= new GUITEXTURE;
	pGUITexture->iFrameSpeed	= _pGUITextureInfo->iFrameSpeed;

	LoadTextureFromFile(	&pGUITexture->pTex,
							_pGUITextureInfo->FileName,
							_Image2D.rtSource.right,
							_Image2D.rtSource.bottom,
							D3DCOLOR_XRGB( 255, 0, 255 ) );

	_Image2D.vec2Tex[ _iNumAni ].push_back( pGUITexture );
}

VOID GUIBase::AddFileName( INT _iNumAni, IMAGEPARAM& _imgParam, LPWSTR _pFileName, INT _iFrameSpeed )
{
	INT iNumAnimation = _imgParam.vec2FileName.size();

	//	순서대로 넣는 것이 아니면 중간에 빼먹고 하는 경우가 있으므로 ERROR 처리
	if( _iNumAni > iNumAnimation )
		MessageBox( NULL, L"AddFileName(){ ... resize() failed ... }", NULL, MB_OK );

	//	같으면 크기를 늘린다
	if( _iNumAni == iNumAnimation )
		_imgParam.vec2FileName.resize( _iNumAni + 1 );

	LPGUITEXTUREINFO pGUITextureInfo = new GUITEXTUREINFO;
	lstrcpy( pGUITextureInfo->FileName, _pFileName );
	
	pGUITextureInfo->iFrameSpeed = _iFrameSpeed;

	_imgParam.vec2FileName[ _iNumAni ].push_back( pGUITextureInfo );
}

VOID GUIBase::CreateImage3D( IMAGE3D& _Image3D, FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, IMAGEPARAM& _imgParam )
{
	//	Init VertexBuffer, IndexBuffer
	GUIVERTEX	Vertex[ 4 ];
	GUIINDEX	Index[ 2 ];

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

	INT iNumVertices	= 4;
	INT iNumIndices		= 2;

	CreateVB( &_Image3D.pVB, iNumVertices, sizeof( GUIVERTEX ), GUIVERTEX::FVF );
	SetVB( _Image3D.pVB, Vertex, iNumVertices, sizeof( GUIVERTEX ) );

	CreateIB( &_Image3D.pIB, iNumIndices, sizeof( GUIINDEX ) );
	SetIB( _Image3D.pIB, Index, iNumIndices, sizeof( GUIINDEX ) );

	//	Init Matrix
	_Image3D.vecScale.x	= _fWidth;//_fWidth;
	_Image3D.vecScale.y	= _fHeight;//_fHeight;

	switch( _imgParam.dPivotType )
	{
	case GBS_TOPLEFT:
		_Image3D.vecTrans.x	= _fX + _fWidth * 0.5f + 0.5f;	//	중점을 옮기는 것
		_Image3D.vecTrans.y	= _fY + _fHeight * 0.5f + 0.5f;	//	fX, fY가 사각형의 좌상단을 가리킨다
		break;
	case GBS_CENTER:
		_Image3D.vecTrans.x	= _fX;
		_Image3D.vecTrans.y	= _fY;
		break;
	default:
		MessageBox( NULL, L"CreateImage( ..., dPivotType, ... ) error", NULL, MB_OK );
		break;
	}

	//	Init Texture
	//	vec2FileName Error Check
	INT iNumAnimation	= _imgParam.vec2FileName.size();
	INT	iNumFrame		= 0;
	if( iNumAnimation != 0 )
		iNumFrame = _imgParam.vec2FileName[ 0 ].size();

	//	Not FileName
	if( iNumAnimation == 0 || iNumFrame == 0 )
	{
		//	Init Texture
		LPGUITEXTURE pGUITexture	= new GUITEXTURE;
		pGUITexture->iFrameSpeed	= 1000;

		//	_pFileName == NULL CreateTexture
		if( FAILED( m_pD3dDevice->CreateTexture(	static_cast<INT>( _fWidth ),
													static_cast<INT>( _fHeight ),
													0,
													0,
													D3DFMT_X8R8G8B8,
													D3DPOOL_MANAGED,
													&pGUITexture->pTex,
													NULL ) ) )
		{
			MessageBox( NULL, L"CreateImage()::CreateTexture failed...", NULL, MB_OK );
			return;
		}
		//	흰색으로 초기화
		D3DLOCKED_RECT	d3drt;
		pGUITexture->pTex->LockRect( 0, &d3drt, NULL, 0 );
	
		UINT* pBits = static_cast<UINT*>( d3drt.pBits );
		
		INT iWidth	= static_cast<INT>( _fWidth );
		INT iHeight	= static_cast<INT>( _fHeight );
		memset( pBits, 0xffffffff, sizeof( UINT ) * iWidth * iHeight );
		
		pGUITexture->pTex->UnlockRect( 0 );

		_Image3D.vec2Tex.resize( 1 );
		_Image3D.vec2Tex[ 0 ].push_back( pGUITexture );

		return;
	}
	
	//	Load Texture
	_Image3D.vec2Tex.resize( iNumAnimation );
	for( INT j=0 ; j<iNumAnimation ; j++ )
	{
		for( INT i=0 ; i<iNumFrame ; i++ )
			AddTexture( j, _Image3D, _imgParam.vec2FileName[ j ][ i ] );
	}
}
VOID GUIBase::CreateImage2D( IMAGE2D& _Image2D, FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, IMAGEPARAM& _imgParam )
{
	_Image2D.vecPosition.x = _fX;
	_Image2D.vecPosition.y = _fY;
	_Image2D.vecPosition.z = 0.0f;

	SetRect( &_Image2D.rtSource,	0, 0,
									static_cast<INT>( _fWidth ),
									static_cast<INT>( _fHeight ) );
								

	switch( _imgParam.dPivotType )
	{
	case GBS_TOPLEFT:
		_Image2D.vecCenter.x	= 0.0f;
		_Image2D.vecCenter.y	= 0.0f;
		_Image2D.vecCenter.z	= 0.0f;
		break;
	case GBS_CENTER:
		_Image2D.vecCenter.x	= _fWidth * 0.5f;	//	중점을 옮기는 것
		_Image2D.vecCenter.y	= _fHeight * 0.5f;	//	fX, fY가 사각형의 좌상단을 가리킨다
		_Image2D.vecCenter.z	= 0.0f;
		break;
	default:
		MessageBox( NULL, L"CreateImage( ..., dPivotType, ... ) error", NULL, MB_OK );
		break;
	}

	//	Init Texture
	//	vec2FileName Error Check
	INT iNumAnimation	= _imgParam.vec2FileName.size();
	INT	iNumFrame		= 0;
	if( iNumAnimation != 0 )
		iNumFrame = _imgParam.vec2FileName[ 0 ].size();

	//	Not FileName
	if( iNumAnimation == 0 || iNumFrame == 0 )
	{
		LPGUITEXTURE pGUITexture	= new GUITEXTURE;
		pGUITexture->iFrameSpeed	= 1000;
		//	_pFileName == NULL CreateTexture
		if( FAILED( m_pD3dDevice->CreateTexture(	static_cast<INT>( _fWidth ),
													static_cast<INT>( _fHeight ),
													0,
													0,
													/*D3DFMT_X8R8G8B8*/D3DFMT_A8R8G8B8,
													D3DPOOL_MANAGED,
													&pGUITexture->pTex,
													NULL ) ) )
		{
			MessageBox( NULL, L"CreateImage()::CreateTexture failed...", NULL, MB_OK );
			return;
		}
		//	흰색으로 초기화
		D3DLOCKED_RECT	d3drt;
		pGUITexture->pTex->LockRect( 0, &d3drt, NULL, 0 );
	
		UINT* pBits = static_cast<UINT*>( d3drt.pBits );
		
		INT iWidth	= static_cast<INT>( _fWidth );
		INT iHeight	= static_cast<INT>( _fHeight );
		memset( pBits, 0xffff00ff, sizeof( UINT ) * iWidth * iHeight );
		
		pGUITexture->pTex->UnlockRect( 0 );

		_Image2D.vec2Tex.resize( 1 );
		_Image2D.vec2Tex[ 0 ].push_back( pGUITexture );

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