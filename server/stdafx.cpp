#include "stdafx.h"


// �ִ°� : �÷��� 
// ���ϰ� : void 
// �ϴ��� : �ش� �÷��� �ٲ۴�. 
void SetColor( CONST INT color) 
{ 
	static HANDLE std_output_handle = GetStdHandle( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute( std_output_handle, color ); 
}