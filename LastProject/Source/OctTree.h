#pragma once

#include <list>

class OctTree
{
public:
	typedef struct _AREA
	{
		D3DXVECTOR3 vMin;
		D3DXVECTOR3 vMax;

		BOOL		bExist;

		_AREA()
		{
			bExist = FALSE;
		}
	}AREA, *LPAREA;

	typedef std::list< LPAREA > LPAREALIST;

private:
	VOID		Initialize();
	VOID		Release();
	VOID		Cleanup();

	//	Build
	VOID		GetMinMax( LPD3DXVECTOR3 _pvVertex, INT _iSize );
	VOID		BuildOctTree( INT _iLevel );

	OctTree*	AddChild( D3DXVECTOR3 _vMin, D3DXVECTOR3 _vMax );
	BOOL		SubDivide( INT _iLevel );
	BOOL		IsDivide();

	VOID		SetChildArea( LPAREA _pArea );

	//	GenerateBoundBox
	VOID		Traverse( D3DXVECTOR3 _Vertex );
	VOID		GetBoundBox( LPAREALIST& _pArealist );

	BOOL		CmpMinMax( const D3DXVECTOR3& _vMin, const D3DXVECTOR3& _vMax, const D3DXVECTOR3& _vVertex );

public:
	OctTree()
	{
		this->Initialize();
	}

	virtual ~OctTree()
	{
		this->Release();
	}

	VOID		Build( INT _iLevel, D3DXVECTOR3 _vMin, D3DXVECTOR3 _vMax );
	VOID		Build( INT _iLevel, LPD3DXVECTOR3 _pvVertex, INT _iSize );
	VOID		GenerateBoundBox( LPAREALIST& _pAreaList, LPD3DXVECTOR3 _pvVertex, INT _iSize );
	VOID		GenerateIndex( D3DXVECTOR3& _pvVertex );

private:
	enum { LUF = 0, LUB, LDF, LDB, RUF, RUB, RDF, RDB };

	OctTree*	m_pChild[ 8 ];
	AREA		m_Area;

	INT			m_iLevel;

public:

};
