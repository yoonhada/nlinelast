#ifndef _CLobbyScene_H_
#define _CLobbyScene_H_

#include "Scene.h"

class CLobbyScene : public IScene
{
public:
	CLobbyScene();
	~CLobbyScene();

public:
	VOID Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT Release();
	VOID Update();
	VOID Render();

private:
	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스

	//네트워크
	WSADATA m_wsadata;
	CNetwork* m_pNetwork;

};

#endif