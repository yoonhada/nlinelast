#ifndef _CLobbyScene_H_
#define _CLobbyScene_H_

#include "Scene.h"

class CLobbyScene : public IScene
{
public:
	CLobbyScene();
	~CLobbyScene();

public:
	VOID			Clear();
	virtual HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT			Release();
	virtual VOID	Update();
	virtual VOID	Render();

	virtual INT		GetSceneNext();
	virtual INT		GetSceneState();

private:
	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스

	//네트워크
	WSADATA m_wsadata;

	INT						m_scnNext;
	INT						m_scnState;
};

#endif