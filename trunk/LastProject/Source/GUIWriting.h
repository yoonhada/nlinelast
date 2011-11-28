#pragma once
#ifndef _GUIWRITING_H_
#define _GUIWRITING_H_

class GUIWriting
{
private:
	HWND		m_hWnd;

	TCHAR		m_Str[ 1024 ];
	INT			m_iNum;

public:
	static GUIWriting& GetInstance();

private:
	GUIWriting()
	{
		this->Initialize();
	}
	~GUIWriting()
	{
		this->Release();
	}

	VOID		Initialize();
	VOID		Release();

public:
	VOID			IMEUpdate( WPARAM& _wParam, LPARAM& _lParam );
	VOID			CHARUpdate( WPARAM& _wParam, LPARAM& _lParam );
	VOID			GetText( LPWSTR _pStr, INT& _iNum );	// Test

	VOID			Cleanup();

	//VOID			SetHWND( HWND& _hWnd );
	//const HWND&		GetHWND();
	//	문자열 길이를 넘기는 함수도 있어야 할듯	
};

#endif
