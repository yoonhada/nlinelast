#ifndef _ASTAR_H_
#define _ASTAR_H_

#include "stdafx.h"


typedef struct TreeNode
{
	INT x;
	INT y;
	struct TreeNode *left, *right;
}TreeNode;


typedef struct TreeRoot
{
	TreeNode* root;
}TreeRoot;


typedef struct PathNode
{
	INT x, y;
	struct PathNode *next;
}PathNode;


typedef struct _PathMap
{
	INT map;
	FLOAT g;
	FLOAT h;
	FLOAT f;
	BOOL visit;
	INT cameX, cameY;
}PathMap;





class Astar : public CSingleton<Astar>
{
	friend class CSingleton<Astar>;

private:
	INT			m_iWidth;
	INT			m_iHeight;

	INT			m_Dir[8][2];

	PathMap**	m_Map;

	INT			m_Start[2];
	INT			m_End[2];

	TreeRoot*	m_pOpen;


private:
	/*m_ap*/
	VOID		createMap( INT* a_Map );
	VOID		setPath( INT a_iSx, INT a_iSy, INT a_iEx, INT a_iEy, PathNode* a_pPath );
	BOOL		isLoad( INT a_iX, INT a_iY );

	/*Tree*/
	TreeNode*	createNode( INT x, INT y );
	VOID		insert( TreeRoot* root, TreeNode* node );
	TreeNode*	insertNode( TreeNode* p, TreeNode* node );
	VOID		removeTree( TreeRoot* root );
	VOID		deleteNode( TreeNode* node );

	/*Path*/
	PathNode*	createPath( INT a_iX, INT a_iY );	
	
	/*Astar*/	
	VOID		openMap( INT bx, INT by, INT vx, INT vy );
	FLOAT		getDistG( INT bx, INT by, INT vx, INT vy );
	FLOAT		getDistH( INT a_iX, INT a_iY );
	FLOAT		getDistF( INT a_iX, INT a_iY );
	BOOL		getminimum( INT min[] );


public:
	Astar();
	~Astar();

	VOID		Initialize( INT a_iWidth, INT a_iHeight, INT* a_Map );
	VOID		clearMap();

	PathNode*	findPath( INT a_iSx, INT a_iSy, INT a_iEx, INT a_iEy );
	PathNode*	getPath( INT a_iSx, INT a_iSy, INT a_iEx, INT a_iEy );

	VOID		removePath( PathNode* a_pPath );
};

#endif
