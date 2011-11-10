#ifndef _CScene_H_
#define _CScene_H_

class IScene
{
public:
	IScene() {}
	virtual ~IScene() {}

public:
	virtual VOID Clear() = 0;
	virtual HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice ) = 0;
	virtual HRESULT Release() = 0;
	virtual VOID Update() = 0;
	virtual VOID Render() = 0;

};

#endif