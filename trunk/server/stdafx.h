#ifndef _STDAFX_H_
#define _STDAFX_H_


// ���
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <vector>
#include <map>

#include "Define.h"


// ���̺귯��
#pragma comment( lib, "ws2_32.lib" )


using std::cin;
using std::cout;
using std::wcout;
using std::endl;
using std::vector;
using std::map;



// ������ �޸� ������ ���� ��ũ��
#define SAFE_DELETE(p)			{ if(p) { delete	(p);	(p) = NULL; } }


// ��Ʈ �� ����
void SetColor( CONST INT color );


#endif