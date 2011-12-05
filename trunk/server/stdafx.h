#ifndef _STDAFX_H_
#define _STDAFX_H_


// 헤더
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <vector>
#include <map>

#include "Define.h"


// 라이브러리
#pragma comment( lib, "ws2_32.lib" )


using std::cin;
using std::cout;
using std::wcout;
using std::endl;
using std::vector;
using std::map;



// 안전한 메모리 해제를 위한 매크로
#define SAFE_DELETE(p)			{ if(p) { delete	(p);	(p) = NULL; } }


// 폰트 색 변경
void SetColor( CONST INT color );


#endif