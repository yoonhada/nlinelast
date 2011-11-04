#ifndef _STDAFX_H_
#define _STDAFX_H_


// DirectX ���̺귯��
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )


// Window ���̺귯��
#pragma comment ( lib, "winmm.lib" )
#pragma comment( lib, "ws2_32.lib" )

// TBB ���̺귯��
//#pragma  comment ( lib, "tbb.lib" )

// AntTwearBar ���̺귯��
#pragma comment (lib, "AntTweakBar.lib")
#include "AntTweakBar.h"


// ���
#include <winsock2.h>
#include <Windows.h>
#include <process.h>
#include <assert.h>

//#include <WindowsX.h>
//#include <CommCtrl.h>
//#include <process.h>

//#include <list>
#include <vector>
#include <algorithm>
//#include <string>

//#include <cstdio>
//#include <fstream>
#include <tchar.h>
#include <time.h>

#include <d3d9.h>
#include <d3dx9core.h>
#include <D3dx9math.h>

// TBB
//#include <tbb/tbb.h>

// ������
#include "Define.h"

// Singleton
#include "Singleton.h"

// Singleton Module
#include "Matrices.h"
#include "Frequency.h"
#include "DebugConsole.h"
#include "Tree.h"
#include "Physics.h"
#include "TextureManage.h"
#include "Input.h"
#include "DebugInterface.h"

// None Sigleton Module
#include "Light.h"

// �߻�ȭ ���
#include "Object.h"

// Object
#include "ObjectSRT.h"
#include "Grid.h"
#include "BoundBox.h"

// Manage
#include "MainManage.h"

// DirectX
#include "DirectX9.h"

// win
#include "WinBase.h"

// �����Լ� ��� ����
#pragma warning( disable:4996 )

#endif