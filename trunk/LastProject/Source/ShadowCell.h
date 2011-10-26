#ifndef _CShadowCell_H_
#define _CShadowCell_H_

class CShadowCell : virtual public IObject, public CObjectSRT
{
public:
	CShadowCell(VOID);
	~CShadowCell(VOID);

	VOID Clear();
	HRESULT Create() { return S_OK; }
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, 
		FLOAT a_Vertex11, FLOAT a_Vertex12, FLOAT a_Vertex13,
		FLOAT a_Vertex21, FLOAT a_Vertex22, FLOAT a_Vertex23,
		FLOAT a_Vertex31, FLOAT a_Vertex32, FLOAT a_Vertex33,
		FLOAT a_Vertex41, FLOAT a_Vertex42, FLOAT a_Vertex43,
		FLOAT a_Uv11, FLOAT a_Uv12,
		FLOAT a_Uv21, FLOAT a_Uv22,
		FLOAT a_Uv31, FLOAT a_Uv32,
		FLOAT a_Uv41, FLOAT a_Uv42, 
		LPCWSTR a_FileName );
	
	VOID Update() {};
	VOID Render();
	HRESULT Release();
	
	HRESULT CreateVB( FLOAT a_Vertex11, FLOAT a_Vertex12, FLOAT a_Vertex13,
		FLOAT a_Vertex21, FLOAT a_Vertex22, FLOAT a_Vertex23,
		FLOAT a_Vertex31, FLOAT a_Vertex32, FLOAT a_Vertex33,
		FLOAT a_Vertex41, FLOAT a_Vertex42, FLOAT a_Vertex43,
		FLOAT a_Uv11, FLOAT a_Uv12,
		FLOAT a_Uv21, FLOAT a_Uv22,
		FLOAT a_Uv31, FLOAT a_Uv32,
		FLOAT a_Uv41, FLOAT a_Uv42 );
	
	HRESULT CreateIB();
	HRESULT LoadTexture( LPCWSTR a_FileName );

private:
	struct CELLVERTEX
	{
		D3DXVECTOR3	p;
		D3DXVECTOR2 t;
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
	};

	struct CELLINDEX
	{
		WORD _0, _1, _2;
	};

	LPDIRECT3DDEVICE9		m_pD3dDevice;
	LPDIRECT3DTEXTURE9		m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	CELLVERTEX				m_Vertices[4];
};

#endif