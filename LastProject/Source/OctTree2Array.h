#pragma once

class COctTree2Array
{
public:
	typedef struct _AREA
	{
		D3DXVECTOR3 vCenter;
		INT nIndex;
	}AREA, *LPAREA;

private:
	VOID		Initialize();
	VOID		Release();
	VOID		Cleanup();

	//	Build
	VOID		BuildOctTree( );

	COctTree2Array*	AddChild( INT );
	BOOL		SubDivide( );

	VOID		SetChildArea( LPAREA _pArea );

	//	GenerateBoundBox
	VOID		Traverse( D3DXVECTOR3 _Vertex );

	BOOL		CmpMinMax( const D3DXVECTOR3& _vMin, const D3DXVECTOR3& _vMax, const D3DXVECTOR3& _vVertex );

public:
	COctTree2Array()
	{
		this->Initialize();
	}

	~COctTree2Array()
	{
		this->Release();
	}

	VOID		Build( INT _nSize );
	VOID		GenerateIndex( D3DXVECTOR3& _pvVertex );

	// Set
	VOID		SetChildIndex( D3DXVECTOR3& _pvVertex, INT nIndex);

	// Get
	INT			GetChildIndex( D3DXVECTOR3& _pvVertex );
private:
	enum { LUF = 0, LUB, LDF, LDB, RUF, RUB, RDF, RDB };

	COctTree2Array*	m_pChild[ 8 ];
	AREA		m_Area;

	INT			m_nSize;
};