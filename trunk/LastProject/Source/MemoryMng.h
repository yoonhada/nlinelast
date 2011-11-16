#ifndef _MEMORYMNG_H_
#define _MEMORYMNG_H_

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

static class MemoryMng
{
public:
    MemoryMng() 
    { 
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        //_CrtSetBreakAlloc(1215896);
    }
    ~MemoryMng()
    { 
        _ASSERTE(_CrtCheckMemory());
    }
} MemoryLeak;
#endif      // _DEBUG

#endif