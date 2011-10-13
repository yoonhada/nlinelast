#include "stdafx.h"
#include "DebugConsole.h"

VOID CDebugConsole::Initialize()
{
	if( !AllocConsole() )
	{
		MessageBox(NULL, L"Console::AllocConsole()", NULL, MB_OK );
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
		MessageBox(NULL, L"Console::Initialize() failed", NULL, MB_OK );
		return;
	}
	
}

VOID CDebugConsole::Release()
{
	CloseHandle( m_hOut );
	FreeConsole();
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