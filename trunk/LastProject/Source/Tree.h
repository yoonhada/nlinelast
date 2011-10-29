/**
@class	CTree
@date	2011/09/06
@author	
@brief	4트리
*/
#pragma once

class CBoundBox;

typedef struct _FRECT
{
	FLOAT left;
	FLOAT top;
	FLOAT right;
	FLOAT bottom;
} FRECT;


class CTree : public CSingleton<CTree>
{
	friend class CSingleton<CTree>;
	typedef struct _NODE
	{
		enum { LT, RT, LB, RB };
		_NODE* _children[4];
		D3DXVECTOR3 _pos[4];

		std::vector <CBoundBox*> _data;

		_NODE()
		{ 
			_children[RT] = _children[LT] = _children[LB] = _children[RB] = NULL; 
			ZeroMemory( &_children, sizeof(_children) );
		}
		FRECT GetRect()	
		{
			FRECT rect = { _pos[0].x, _pos[0].z, _pos[3].x, _pos[3].z };
			return rect;
		}
	}NODE;

private:	
	NODE * m_pRoot;		///< 루트
	NODE * m_pCurr;		///< 커서 

	std::vector <CBoundBox*> _vecCha;
	std::vector <CBoundBox*> _vecAtk;

	VOID CreateChild( NODE *pNode, FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom, INT nDeep );
	BOOL IsRect( NODE *, CBoundBox *);
	BOOL IsRect( NODE *, D3DXVECTOR3 &);

public:
	CTree();
	CTree( NODE* _pRoot );
	~CTree();

	// Get
	NODE* GetRoot ()				{ return m_pRoot; }

	// Set
	VOID SetRoot ( NODE* _pRoot )	{ SAFE_DELETE( m_pRoot ); m_pRoot = _pRoot; }

	// virtual 
	virtual HRESULT Create()		{ return S_OK; }
	virtual VOID Release();
	virtual VOID Draw()				{}
	virtual VOID Update()			{}

	// member
	HRESULT Create( FLOAT fRange, INT nDeep );
	VOID InsertObject( NODE *pNode, CBoundBox * pCube );
	VOID RemoveObject( NODE *pNode, CBoundBox * pCube );
	VOID RemoveAll( );
	std::vector <CBoundBox*> * GetMapVector( NODE *pNode, D3DXVECTOR3 vPos );
	std::vector <CBoundBox*> * GetChaVector()		{ return &_vecCha; }
	std::vector <CBoundBox*> * GetAtkVector()		{ return &_vecAtk; }
};