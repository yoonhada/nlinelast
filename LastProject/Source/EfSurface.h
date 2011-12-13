#pragma once

//typedef D3DXVECTOR3							VEC3;
//typedef D3DXVECTOR4							VEC4;
//
//typedef LPDIRECT3DDEVICE9					PDEV;
//
//typedef LPDIRECT3DTEXTURE9					PDTX;
//typedef LPDIRECT3DSURFACE9					PDSF;
//
//
class CEfSurface
{
public:
	struct VtxwDUV
	{
		D3DXVECTOR4	p;
		DWORD	d;
		FLOAT	u,v;

		VtxwDUV()	{}
		VtxwDUV(FLOAT X,FLOAT Y,FLOAT Z,FLOAT U,FLOAT V,DWORD D=0xFFFFFFFF) : p(X,Y,Z,1.F),u(U),v(V),d(D){}

		enum { FVF = (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)};
	};

protected:
	LPDIRECT3DDEVICE9		m_pDev;

	LPDIRECT3DTEXTURE9		m_pTx;			// Target Texture
	LPDIRECT3DSURFACE9		m_pTxSf;		// Target Surface	
	LPD3DXRENDERTOSURFACE	m_pTxRs;		// Render To Surface

	INT			m_iTxW;			// Render Target Texture Width

	VtxwDUV		m_pVtx[4];

public:
	CEfSurface();
	virtual ~CEfSurface();

	INT		Create(LPDIRECT3DDEVICE9 pDev);
	void	Destroy();

	INT		Restore();

	INT		Update( IObject * a_pObject );
	void	Render();
};
