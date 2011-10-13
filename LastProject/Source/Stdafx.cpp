#include "Stdafx.h"
#include "MemoryMng.h"

#define RAND_TABLE 65536
FLOAT aFastRandTable[RAND_TABLE];
unsigned short int iArray;

VOID FastRand2Init()
{
	srand ( ( unsigned int )time( NULL ) );
	for( int i=0 ; i<RAND_TABLE ; ++i )
	{
		aFastRandTable[i] = static_cast<FLOAT>( rand() ) / static_cast<FLOAT>( RAND_MAX );
	}
}

FLOAT FastRand2()
{
	if ( iArray >= RAND_TABLE )
	{
		iArray = 0;
	}

	return aFastRandTable[++iArray];
}

INT APIENTRY _tWinMain(HINSTANCE a_hInstance,
					   HINSTANCE a_hPrevInstance,
					   LPTSTR    a_lpCmdLine,
					   INT       a_nCmdShow)
{
	HINSTANCE hPrevInstance = a_hPrevInstance;
	LPTSTR    lpCmdLine = a_lpCmdLine;
	INT       nCmdShow = a_nCmdShow;
	hPrevInstance = NULL;
	lpCmdLine = NULL;
	nCmdShow = NULL ;

	return CWinBase::MessageLoop(a_hInstance);
}