#ifndef _CIntroScene_H_
#define _CIntroScene_H_

#include "Scene.h"

class CIntroScene : public IScene
{
public:
	CIntroScene();
	virtual ~CIntroScene();

public:
	VOID Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT Release();
	VOID Update();
	VOID Render();

private:
	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스

};

#endif