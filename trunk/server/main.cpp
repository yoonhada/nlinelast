#define _DEBUG_


#include "stdafx.h"

#include "SocketServer.h"
#include "UI_Manager.h"

#ifdef _DEBUG_
#include <crtdbg.h>
#endif


VOID Draw_Menu()
{
	cout << "======================================================================" << endl;
	cout << "CLIENT_NUMBER      ID      STATE      HOST      PING      BUFFER_STATE" << endl;
	cout << "======================================================================" << endl;
}


INT main( INT argc, CHAR* argv[] )
{
#ifdef _DEBUG_
	// 메모릭 릭 탐지
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	CSocketServer SocketServer;
	SocketServer.Initialize();
	SocketServer.StartServer( 20202 );

	CHAR str[128];

//	Draw_Menu();

//	CUI_Manager manager;

//	manager.Initialize();

		
	while( 1 )
	{
		SocketServer.UpdateFrame();
		
		cin >> str;
		if( !strcmp( str, "quit" ) )
			break;

		Sleep( 1 );
	}

	return 0;
}