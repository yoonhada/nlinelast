/**
@class	CTree
@date	2011/09/06
@author	
@brief	4트리
*/
#pragma once

class CBoundBox;
class CTree : public CSingleton<CTree>
{
	friend class CSingleton<CTree>;
	struct NODE
	{
		enum { TL, TR, BL, BR };
		NODE* _children[4];
		D3DXVECTOR3 _pos[4];

		std::vector <CBoundBox*> _data;

		NODE()
		{ 
			_children[TR] = _children[TL] = _children[BL] = _children[BR] = NULL; 
			ZeroMemory( &_children, sizeof(_children) );
		}
		RECT GetRect()	
		{
			RECT rect = {
				static_cast< LONG >( _pos[0].x), 
				static_cast< LONG >( _pos[0].z), 
				static_cast< LONG >( _pos[3].x), 
				static_cast< LONG >( _pos[3].z) };
			return rect;
		}
	};

private:	
	NODE * m_pRoot;		///< 루트
	NODE * m_pCurr;		///< 커서 

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
	std::vector <CBoundBox*> * GetVector( NODE *pNode, D3DXVECTOR3 vPos );

	std::vector <CBoundBox*> _data;
};