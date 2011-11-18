#include "stdafx.h"


// 넣는값 : 컬러값 
// 리턴값 : void 
// 하는일 : 해당 컬러로 바꾼다. 
void SetColor( CONST INT color) 
{ 
	static HANDLE std_output_handle = GetStdHandle( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute( std_output_handle, color ); 
}