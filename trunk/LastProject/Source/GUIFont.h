#pragma once
#ifndef _GUIFONT_H_
#define _GUIFONT_H_

class GUIFont
{
private:
	HWND			m_hWnd;

	LPD3DXFONT		m_pd3dFont;
	HDC				m_hDC;
	HFONT			m_hFont, m_hOldFont;

public:
	static GUIFont& GetInstance();

private:
	GUIFont()
	{
		this->Initialize();
	}

	~GUIFont()
	{
		this->Release();
	}

	VOID		Initialize();
	VOID		Release();
	VOID		Cleanup();
public:
	VOID		Create( LPWSTR _pFaceName, INT _iWidht, INT _iHeight, LPDIRECT3DDEVICE9 _pd3dDevice );
	VOID		DrawOnTexture( LPWSTR _pStr, DWORD _dColor, LPDIRECT3DTEXTURE9 _pTexture, INT _itexX, INT _itexY, INT _itexWidth, INT _itexHeight );
	VOID		Render( LPWSTR _pStr, LPRECT _prt, DWORD _dColor );

	VOID		SetHWND( HWND& _hWnd );	//	m_hWnd를 먼저 셋팅해주어야 한다

	VOID		GetTextSize( LPWSTR _pStr, SIZE& _Size );

};

#endif
