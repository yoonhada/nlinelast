#ifndef _Lights_H_
#define _Lights_H_

class CLight
{
public:
	CLight(VOID);
	virtual ~CLight(VOID);

	VOID Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	VOID EnableLight( D3DXVECTOR3 _vecDir = D3DXVECTOR3( 1.0f, -1.0f, 0.0f ) );
	VOID EnableCharacterLight( D3DXVECTOR3 _vecDir = D3DXVECTOR3( 1.0f, -1.0f, 0.0f ) );
	VOID DisableLight();
	//HRESULT Release();

private:
	LPDIRECT3DDEVICE9 m_pD3dDevice;
	D3DMATERIAL9 m_Mtrl;
};

#endif