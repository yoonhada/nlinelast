#pragma once 
#ifndef _ASEASEVIEWER_H_
#define _ASEASEVIEWER_H_

#include "ASEViewerBase.h"

class ASEParser;
class BBXParser;
class TileMap;

class ASEViewer : public ASEViewerBase
{
private:
	VOID		Initialize();
	
	VOID		InitASE( LPWSTR _ASEFileName );
	VOID		InitBBX( LPWSTR _BBXFileName );
	VOID		CreateDataFromASE( const INT _Index );
	VOID		CreateDataFromBBX( const INT _Index );
	VOID		RenderASEData( INT _Index );
	VOID		RenderBBXData( INT _Index );

public:
	ASEViewer( LPDIRECT3DDEVICE9 _pd3dDevice ) : ASEViewerBase( _pd3dDevice )
	{
		this->Initialize();
	}
	~ASEViewer()
	{
		this->Release();
	}

	VOID		Create( LPWSTR _ASEFileName, LPWSTR _BBXFileName );
	VOID		Update();
	VOID		Render();
	VOID		Render( D3DXMATRIX& _matCharacter );
	VOID		Release();

	INT			GetCurrentFrame();
	BOOL		AddAnimationData( const DWORD _dType, DWORD _dID, INT _iStartFrame, INT _iEndFrame, BOOL _bLoop );
	BOOL		SetAnimation( DWORD _dID );
	VOID		CleanupAnimationData();

	LPGRAPHINFO		GetGraphInfo() { return &m_GraphInfo; }
	TileMap*		GetTileMapInfo() { return m_pTileMap; }

private:
	LPDATA					m_pASEData;
	LPDATA					m_pBBXData;
	INT						m_iNumASEData;
	INT						m_iNumBBXData;

	GRAPHINFO				m_GraphInfo;

	ASEParser*				m_pASEParser;
	BBXParser*				m_pBBXParser;
	TileMap*				m_pTileMap;

	//std::vector<CBoundBox*> m_pBoundBoxVector;
};

#endif