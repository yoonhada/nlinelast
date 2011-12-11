#pragma once
#ifndef _GUIBASE_H_
#define _GUIBASE_H_

//	Animation test
#include <vector>
//	End

class GUIBase
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		SetMatrix( D3DXVECTOR3& _vecScale, D3DXVECTOR3& _vecRotate, D3DXVECTOR3& _vecTranslate );

	HRESULT		CreateVB( LPDIRECT3DVERTEXBUFFER9* _ppVB, INT _nVertex, INT _Size, DWORD _FVF );
	HRESULT		SetVB( LPDIRECT3DVERTEXBUFFER9 _pVB, LPVOID _pvertices, INT _nVertex, INT _Size );

	HRESULT		CreateIB( LPDIRECT3DINDEXBUFFER9* _ppIB, INT _nIndex, INT _Size );
	HRESULT		SetIB( LPDIRECT3DINDEXBUFFER9 _pIB, LPVOID _indices, INT _nIndex, INT _Size );

	HRESULT		LoadTextureFromFile(	LPDIRECT3DTEXTURE9* _ppOutTexture, 
										LPCWSTR				_FileName, 
										UINT				_iWidth		= D3DX_DEFAULT_NONPOW2, 
										UINT				_iHeight	= D3DX_DEFAULT_NONPOW2, 
										D3DCOLOR			_colClear	= NULL );

public:
	enum { GBS_TOPLEFT = 0, GBS_CENTER };

	//	struct Vertex
	typedef struct _GUIVERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };

		D3DXVECTOR3	vecPosition;
		D3DXVECTOR2	vecTexcoord;
	}GUIVERTEX, *LPGUIVERTEX;

	//	struct Index
	typedef struct _GUIINDEX
	{
		DWORD _0, _1, _2;
	}GUIINDEX, *LPGUIINDEX;

	//	struct Texture
	typedef struct _GUITEXTURE
	{
		LPDIRECT3DTEXTURE9	pTex;
		INT					iFrameSpeed;
		
		_GUITEXTURE()
		{	
			iFrameSpeed = 1000;
			pTex = NULL;
		}
		~_GUITEXTURE()
		{
			if( pTex != NULL )
				pTex->Release();
			pTex = NULL;
		}
	}GUITEXTURE, *LPGUITEXTURE;

	typedef std::vector< LPGUITEXTURE >			GUITEXTUREVECTOR;
	typedef std::vector< GUITEXTUREVECTOR* >	GUITEXTUREVECTOR2;

	//	struct Image 
	//	IMAGE -> IMAGE3D로 수정
	typedef struct _IMAGE3D
	{
		LPDIRECT3DVERTEXBUFFER9	pVB;
		LPDIRECT3DINDEXBUFFER9	pIB;
		
		GUITEXTUREVECTOR2		vec2Tex;	
		BOOL					bAniPlay;
		DWORD					dCurrentTime;
		DWORD					dBeginTime;
		INT						iCurrentFrame;
		INT						iCurrentAnimation;

		D3DXVECTOR3				vecScale;
		D3DXVECTOR3				vecRotate;
		D3DXVECTOR3				vecTrans;
		
		_IMAGE3D()
		{
			pVB		= NULL;
			pIB		= NULL;
			
			bAniPlay			= FALSE;
			dCurrentTime		= 0;
			dBeginTime			= 0;
			iCurrentFrame		= 0;
			iCurrentAnimation	= 0;

			vecScale	= D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
			vecRotate	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			vecTrans	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
		~_IMAGE3D()
		{
			if( pVB != NULL )
				pVB->Release();
			if( pIB != NULL )
				pIB->Release();
			
			GUITEXTUREVECTOR2::iterator itE2;
			for( itE2=vec2Tex.begin() ; itE2!=vec2Tex.end() ; itE2++ )
			{
				GUITEXTUREVECTOR::iterator itE;
				for( itE=(*itE2)->begin() ; itE!=(*itE2)->end() ; itE++ )
					delete (*itE);

				delete (*itE2);
			}

			pVB		= NULL;
			pIB		= NULL;
		}
	}IMAGE3D, *LPIMAGE3D;

	//	struct Image2D
	typedef struct _IMAGE2D
	{
		RECT				rtSource;
		D3DXVECTOR3			vecCenter;
		D3DXVECTOR3			vecPosition;

		GUITEXTUREVECTOR2	vec2Tex;	
		BOOL				bAniPlay;
		DWORD				dCurrentTime;
		DWORD				dBeginTime;
		INT					iCurrentFrame;
		INT					iCurrentAnimation;
	
		_IMAGE2D()
		{
			bAniPlay			= FALSE;
			dCurrentTime		= 0;
			dBeginTime			= 0;
			iCurrentFrame		= 0;
			iCurrentAnimation	= 0;
		}
		~_IMAGE2D()
		{
			GUITEXTUREVECTOR2::iterator itE2;
			for( itE2=vec2Tex.begin() ; itE2!=vec2Tex.end() ; itE2++ )
			{
				GUITEXTUREVECTOR::iterator itE;
				for( itE=(*itE2)->begin() ; itE!=(*itE2)->end() ; itE++ )
					delete (*itE);

				delete (*itE2);
			}
			
		}
	}IMAGE2D, *LPIMAGE2D;

	//	iFrameSpeed를 추가 해야 된다
	//	struct Param
	typedef struct _GUITEXTUREINFO
	{
		TCHAR	FileName[ 1024 ];
		INT		iFrameSpeed;
	}GUITEXTUREINFO, *LPGUITEXTUREINFO;

	typedef std::vector< LPGUITEXTUREINFO >			GUITEXTUREINFOVECTOR;
	typedef std::vector< GUITEXTUREINFOVECTOR* >	GUITEXTUREINFOVECTOR2;

	typedef struct _IMAGEPARAM
	{	//	fX, fY, fWidth, fHeight를 따로 나누는것은 고려해봐야 될 문제...
		//	텍스쳐를 생성할때 써야되니깐...
		//	분리 하고 싶은데 뭔가가 조금 찝찝해...
		//	외부에서 편집할 수 있게 할때 필요함!
		//	코드로 입력 받는것은 불편하니깐 일단 Create에서 imgParam의 fx, fy, fwidth, fheight를 입력받자
		DWORD					dPivotType;
		FLOAT					fX, fY;
		FLOAT					fWidth, fHeight;

		GUITEXTUREINFOVECTOR2	vec2FileName;

		_IMAGEPARAM()
		{
			dPivotType	= GBS_TOPLEFT;
			fX			= 0.0f;
			fY			= 0.0f;
			fWidth		= 10.0f;
			fHeight		= 10.0f;
		}
		~_IMAGEPARAM()
		{
			GUITEXTUREINFOVECTOR2::iterator itE2;
			for( itE2=vec2FileName.begin() ; itE2!=vec2FileName.end() ; itE2++ )
			{
				GUITEXTUREINFOVECTOR::iterator itE;
				for( itE=(*itE2)->begin() ; itE!=(*itE2)->end() ; itE++ )
					delete (*itE);

				//	Test
				delete (*itE2);
				//	End
			}
		}
	}IMAGEPARAM, *LPIMAGEPARAM;

	GUIBase( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : m_pd3dDevice( _pd3dDevice ), m_pSprite( _pSprite )
	{
		this->Initialize();
	}
	virtual ~GUIBase()
	{
		this->Release();
	}

	//	참조형 말고 포인트형으로 받자 
	//	통일통일~ 우리의 소원은 통일~
	//	외부 변형 툴 만들었을때 쓸꺼
	VOID		CreateImage3D( IMAGE3D& _Image3D, IMAGEPARAM& _imgParam );
	VOID		CreateImage2D( IMAGE2D& _Image2D, IMAGEPARAM& _imgParam );
	//	코드로 직접 입력할때 쓸꺼
	VOID		CreateImage3D( IMAGE3D& _Image3D, FLOAT _fX, FLOAT _fY, FLOAT _fWidht, FLOAT _fHeight, IMAGEPARAM& _imgParam );
	VOID		CreateImage2D( IMAGE2D& _Image2D, FLOAT _fX, FLOAT _fY, FLOAT _fWidht, FLOAT _fHeight, IMAGEPARAM& _imgParam );

	VOID		RenderImage3D( LPIMAGE3D _pImage3D );
	VOID		RenderImage2D( LPIMAGE2D _pImage2D );

	VOID		Image3DScale( LPIMAGE3D _pImage3D, FLOAT _fX, FLOAT _fY, FLOAT _fZ );
	VOID		Image3DTranslate( LPIMAGE3D _pImage3D, FLOAT _fX, FLOAT _fY, FLOAT _fZ );

	VOID		AddFileName( INT _iNumAni, IMAGEPARAM& _imgParam, LPWSTR _pFileName, INT _iFrameSpeed  = 1000 );
	VOID		AddTexture( INT _iNumAni, IMAGE3D& _Image3D, LPGUITEXTUREINFO _pGUITextureInfo );
	VOID		AddTexture( INT _iNumAni, IMAGE2D& _Image2D, LPGUITEXTUREINFO _pGUITextureInfo );
	
private:

public:
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	LPD3DXSPRITE		m_pSprite;
	
	
};

#endif