#include "stdafx.h"
#include "GUIFont.h"

GUIFont& GUIFont::GetInstance()
{
	static GUIFont m_GUIFont;

	return m_GUIFont;
}

VOID GUIFont::Initialize()
{
	m_hWnd		= NULL;
	m_pd3dFont	= NULL;
	m_hFont		= NULL;
	m_hOldFont	= NULL;
	m_hDC		= NULL;
}

VOID GUIFont::Release()
{
}

VOID GUIFont::Cleanup()
{
	if( m_pd3dFont != NULL )
		m_pd3dFont->Release();
	
	if( m_hDC != NULL && m_hFont != NULL && m_hOldFont != NULL )
	{	
		SelectObject( m_hDC, m_hOldFont );
		DeleteObject( m_hFont );
		ReleaseDC( m_hWnd, m_hDC );
	}
}

VOID GUIFont::Create( LPWSTR _pFaceName, INT _iWidth, INT _iHeight, LPDIRECT3DDEVICE9 _pd3dDevice )
{
	if( m_hWnd == NULL )
		MessageBox( NULL, L"GUIFont::SetHWND( _hWnd )�� ���� �ʾҽ��ϴ�", NULL, MB_OK );

	this->Cleanup();

	//	Create d3dFont
	D3DXFONT_DESC desc;
	ZeroMemory( &desc, sizeof( D3DXFONT_DESC ) );

	desc.Width				=	_iWidth;
	desc.Height				=	_iHeight;
	desc.Weight				=	FW_NORMAL;
	desc.MipLevels			=	D3DX_DEFAULT;
	desc.Italic				=	FALSE;
	desc.CharSet			=	DEFAULT_CHARSET;
	desc.OutputPrecision	=	OUT_DEFAULT_PRECIS;
	desc.Quality			=	DEFAULT_QUALITY;
	desc.PitchAndFamily		=	DEFAULT_PITCH;

	lstrcpy( desc.FaceName, _pFaceName );

	if( FAILED( D3DXCreateFontIndirect( _pd3dDevice, &desc, &m_pd3dFont ) ) )
		MessageBox( NULL, L"D3DXCreateFontIndirect() failed", NULL, MB_OK );

	//	Create hFont
	//	Get HDC
	//	GetTextExtentPoint32W�� �� ���� ������ �ִ� HDC�� ���Ѵ�
	LOGFONTW lf;
	ZeroMemory( &lf, sizeof( LOGFONTW ) );

	lf.lfWidth				= desc.Width;		
	lf.lfHeight				= desc.Height;
	lf.lfWeight				= desc.Weight;
	lf.lfItalic				= desc.Italic;
	lf.lfCharSet			= desc.CharSet;
	lf.lfOutPrecision		= desc.OutputPrecision;
	lf.lfQuality			= desc.Quality;
	lf.lfPitchAndFamily		= desc.PitchAndFamily;

	lstrcpy( lf.lfFaceName, desc.FaceName );

	m_hDC		= GetDC( m_hWnd );
	m_hFont		= CreateFontIndirectW( &lf );
	m_hOldFont	= (HFONT)SelectObject( m_hDC, m_hFont );
}

VOID GUIFont::DrawOnTexture( LPWSTR _pStr, DWORD _dColor, LPDIRECT3DTEXTURE9 _pTexture, INT _itexX, INT _itexY, INT _itexWidth, INT _itexHeight )
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
	BitInfo.bmiHeader.biWidth			=	_itexWidth;
	BitInfo.bmiHeader.biHeight			=	_itexHeight;
	BitInfo.bmiHeader.biPlanes			=	1;							//	��ǥ������ �÷��� ��( �̹��� ���, bitmap�� layer�� �������� �������� �׻� 1 )
	BitInfo.bmiHeader.biBitCount		=	32;							//	�� �ȼ��� ��Ʈ��
	BitInfo.bmiHeader.biCompression		=	BI_RGB/*BI_PNG*/;						//	������( BI_RGB �Ǵ� 0 : �������Ʈ�� )
	BitInfo.bmiHeader.biSizeImage		=	0;							//	��Ʈ�� ����ũ��
	BitInfo.bmiHeader.biXPelsPerMeter	=	100;						//	���ʹ� �ȼ���
	BitInfo.bmiHeader.biYPelsPerMeter	=	100;						//	���ʹ� �ȼ���
	BitInfo.bmiHeader.biClrUsed			=	0;							//	���� �÷���
	BitInfo.bmiHeader.biClrImportant	=	0;							//	��Ʈ�� ���÷��̿� ���Ǵ� �÷� ��

	hDC		= CreateCompatibleDC( NULL );
	hBit	= CreateDIBSection( hDC, &BitInfo, DIB_RGB_COLORS, &pBits, NULL, 0 );

	RECT rt;
	SetRect( &rt, 0, 0, _itexWidth, _itexHeight );

	HBRUSH	hBackgroundBrush = CreateSolidBrush( 0x00dddddd );
	
	SelectObject( hDC, hBit );
	SelectObject( hDC, m_hFont );

	FillRect( hDC, &rt, hBackgroundBrush );	
	SetBkMode( hDC, TRANSPARENT );		//	TRANSPARENT : ������ ��� ����
	SetTextColor( hDC, _dColor );
	
	//	2,2 ��� �»��, �ߴ�, ���ϴ� 3������ ������ �� �� �ְ� �ϸ� ���� ��
	TextOut( hDC, _itexX, _itexY, _pStr, lstrlen( _pStr ) );
	
	D3DLOCKED_RECT	d3drt;
	_pTexture->LockRect( 0, &d3drt, NULL, 0 );

	UINT* pSrc32 = static_cast< UINT* >( pBits );
	UINT* pDst32 = static_cast< UINT* >( d3drt.pBits );
	
	pSrc32 += ( _itexHeight * _itexWidth ) - _itexWidth;
	UINT* pTemp = pSrc32;

	for( INT y=0 ; y<_itexHeight ; y++ )
	{
		for( INT x=0 ; x<_itexWidth ; x++ )
		{
			if( (*pSrc32) != 0x00dddddd )
				(*pDst32) = (*pSrc32) | 0xff000000;

			pSrc32++;
			pDst32++;
		}
		pTemp -= _itexWidth;
		pSrc32 = pTemp;
	}
	
	_pTexture->UnlockRect( 0 );

	DeleteObject( hBackgroundBrush );
	DeleteObject( hBit );
	DeleteDC( hDC );
}

VOID GUIFont::Render( LPWSTR _pStr, LPRECT _prt, DWORD _dColor )
{
	m_pd3dFont->DrawTextW( NULL, _pStr, -1, _prt, DT_LEFT, _dColor );
}

VOID GUIFont::SetHWND( HWND& _hWnd )
{
	m_hWnd = _hWnd;

	//	�ʱⰪ ����
	//this->Create( L"�޸ո���ü", 10, 20 );
}

VOID GUIFont::GetTextSize( LPWSTR _pStr, SIZE& _Size )
{
	GetTextExtentPoint32W( m_hDC, _pStr, lstrlen( _pStr ), &_Size );
}
