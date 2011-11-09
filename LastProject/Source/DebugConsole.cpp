#include "stdafx.h"
#include "DebugConsole.h"

VOID CDebugConsole::Initialize()
{
	if( !AllocConsole() )
	{
		MessageBox( NULL, L"CDebugConsole::AllocConsole()", NULL, MB_OK );
		return;
	}


	if( ( m_hOut = CreateFile( 
		L"CONOUT$",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		0) ) == INVALID_HANDLE_VALUE )
	{
		MessageBox( NULL, L"CDebugConsole::Initialize() failed", NULL, MB_OK );
		return;
	}

	SetConsoleTitle( L"1.0v" );
}

VOID CDebugConsole::Release()
{
	CloseHandle( m_hOut );
	FreeConsole();
}

VOID CDebugConsole::GetConsoleHwnd( HWND& _hWnd )
{
	const INT BUFSIZE = 1024;

	TCHAR pszNewWindowTitle[ BUFSIZE ]; // Contains fabricated
	TCHAR pszOldWindowTitle[ BUFSIZE ]; // Contains original

	// Fetch current window title.
	GetConsoleTitle(pszOldWindowTitle, BUFSIZE);

	wsprintf( pszNewWindowTitle, L"%d/%d", GetTickCount(), GetCurrentProcessId() );

	// Change current window title.
	SetConsoleTitle(pszNewWindowTitle);

	// Ensure window title has been updated.
	Sleep(40);

	// Look for NewWindowTitle.
	_hWnd = FindWindow(NULL, pszNewWindowTitle);

	// Restore original window title.
	SetConsoleTitle(pszOldWindowTitle);
}

VOID CDebugConsole::GetEdgePosition( DWORD _Edge )
{
	//	Get Display Impormation
	HDC hDC;
	hDC = CreateDC( TEXT("DISPLAY"), NULL, NULL, NULL );
	INT iWidth		= GetDeviceCaps( hDC, HORZRES );
	INT iHeight		= GetDeviceCaps( hDC, VERTRES );
	INT iNumMonitor = GetSystemMetrics( SM_CMONITORS);
	DeleteDC( hDC );

	this->Messagef( L"모니터 갯수 : %d\n 해상도 : %d * %d\n", iNumMonitor, iWidth, iHeight );

	iWidth *= iNumMonitor;

	RECT rt;
	GetClientRect( m_hWnd, &rt );
	INT x = 0, y = 0;

	switch( _Edge )
	{
	case CSL_TL:
		x = 0;
		y = 0;
		break;
	case CSL_TR:
		x = iWidth - rt.right;
		y = 0;
		break;
	case CSL_BL:
		x = 0;
		y = iHeight - rt.bottom;
		break;
	case CSL_BR:
		x = iWidth - rt.right;
		y = iHeight - rt.bottom;
		break;
	}

	MoveWindow( m_hWnd, x, y, rt.right, rt.bottom, FALSE );
}
VOID CDebugConsole::Message( LPWSTR _str )
{
	DWORD sz;

	//	멀티바이트 -> 유니코드
	CHAR str[ 256 ];
	WideCharToMultiByte( CP_ACP, 0, _str, -1, str, 256, NULL, NULL );
	//	End

	WriteFile( m_hOut, str, strlen( str ), &sz, NULL );
}

VOID CDebugConsole::Message( LPSTR _str )
{
	DWORD sz;

	WriteFile( m_hOut, _str, strlen( _str ), &sz, NULL );
}

VOID CDebugConsole::Messagef( LPWSTR _str, ... )
{
	//	멀티바이트 -> 유니코드
	CHAR str[ 256 ];
	WideCharToMultiByte( CP_ACP, 0, _str, -1, str, 256, NULL, NULL );
	//	End

	va_list		argp;
	CHAR		szBuf[ 1024 ];

	va_start( argp, _str );
	vsprintf_s( szBuf, str, argp );
	va_end( argp );


	DWORD sz;
	WriteFile( m_hOut, szBuf, strlen( szBuf ), &sz, NULL );
}

VOID CDebugConsole::Messagef( LPSTR _str, ... )
{
	va_list		argp;
	CHAR		szBuf[ 1024 ];

	va_start( argp, _str );
	vsprintf_s( szBuf, _str, argp );
	va_end( argp );


	DWORD sz;
	WriteFile( m_hOut, szBuf, strlen( szBuf ), &sz, NULL );
}

VOID CDebugConsole::SetPosition( DWORD _dEdge, BOOL _bTopMost )
{
	//	Get HWND
	GetConsoleHwnd( m_hWnd );

	//	Set Position
	GetEdgePosition( _dEdge );

	//	Set TopMost
	if( _bTopMost )
		SetWindowPos( m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
	else
		SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

}

VOID CDebugConsole::MessageQube(D3DXVECTOR3 *_vec)
{
	Messagef("          (%03.0f, %03.0f, %03.0f)----(%03.0f, %03.0f, %03.0f)\n", _vec[0].x, _vec[0].y, _vec[0].z, _vec[1].x, _vec[1].y, _vec[1].z);
	Messagef("        /|                 /|\n");
	Messagef("(%03.0f, %03.0f, %03.0f)-----(%03.0f, %03.0f, %03.0f)\n",  _vec[3].x, _vec[3].y, _vec[3].z, _vec[2].x, _vec[2].y, _vec[2].z);
	Messagef("       | |                | |\n");
	Messagef("       | |(%03.0f, %03.0f, %03.0f)-|-|(%03.0f, %03.0f, %03.0f)\n",  _vec[4].x, _vec[4].y, _vec[4].z, _vec[5].x, _vec[5].y, _vec[5].z);
	Messagef("       |/                 |/\n");
	Messagef("(%03.0f, %03.0f, %03.0f)-----(%03.0f, %03.0f, %03.0f)\n",  _vec[7].x, _vec[7].y, _vec[7].z, _vec[6].x, _vec[6].y, _vec[6].z);
}