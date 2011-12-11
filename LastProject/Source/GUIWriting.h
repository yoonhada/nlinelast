#pragma once
#ifndef _GUIWRITING_H_
#define _GUIWRITING_H_

class GUIWriting
{
private:
	HWND		m_hWnd;

	LPWSTR		m_pText;
	INT*		m_piTextLength;

	BOOL		m_bReturn;

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
	VOID		IMEUpdate( WPARAM& _wParam, LPARAM& _lParam );
	VOID		CHARUpdate( WPARAM& _wParam, LPARAM& _lParam );

	BOOL		GetReturn();

	VOID		ConnectText( LPWSTR _pText, INT* _piTextLength );
	VOID		DisconnectText();

};

#endif
