#pragma once
#ifndef _GUICURSOR_H_
#define _GUICURSOR_H_

class GUICursor
{
private:
	HCURSOR		m_hCurArrow;

	HCURSOR		m_hCurSizeWE;		//	�¿�
	HCURSOR		m_hCurSizeNS;		//	����
	HCURSOR		m_hCurSizeNWSE;		//	�»����
	HCURSOR		m_hCurSizeNESW;		//	�������

	HCURSOR		m_hCurSizeAll;

	HCURSOR		m_hCurrentCursor;

public:
	enum { CSR_WE = 0, CSR_NS, CSR_NWSE, CSR_NESW, CSR_ARROW, CSR_ALL };

	static GUICursor& GetInstance();

private:
	GUICursor()
	{
		this->Initialize();
	}
	~GUICursor()
	{
		this->Release();
	}

	VOID		Initialize();
	VOID		Release();

public:
	VOID		Update( LPARAM _lParam );

	VOID		Change( DWORD _dType );

};

#endif