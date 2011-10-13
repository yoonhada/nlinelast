#ifndef _Lights_H_
#define _Lights_H_

class CLight
{
public:
	CLight(VOID);
	~CLight(VOID);

	VOID Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	VOID EnableLight();
	VOID DisableLight();
	//HRESULT Release();

private:
	LPDIRECT3DDEVICE9 m_pD3dDevice;
	D3DMATERIAL9 m_Mtrl;
};

#endif