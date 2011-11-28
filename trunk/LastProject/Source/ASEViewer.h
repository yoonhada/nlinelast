#pragma once 
#ifndef _ASEASEVIEWER_H_
#define _ASEASEVIEWER_H_

#include "ASEViewerBase.h"

class ASEParser;
class BBXParser;

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
	ASEViewer( )
	{
	}
	ASEViewer( LPDIRECT3DDEVICE9 _pd3dDevice ) : ASEViewerBase( _pd3dDevice )
	{
		this->Initialize();
	}
	~ASEViewer()
	{
		this->Release();
	}

	VOID			Create( LPWSTR _ASEFileName, LPWSTR _BBXFileName );
	VOID			Update();
	VOID			Render();
	VOID			Render( D3DXMATRIX& _matCharacter );
	VOID			Release();

	INT				GetCurrentFrame();
	BOOL			AddAnimationData( const DWORD _dType, DWORD _dID, INT _iStartFrame, INT _iEndFrame, BOOL _bLoop );
	BOOL			SetAnimation( DWORD _dID );
	VOID			CleanupAnimationData();

private:
	LPDATA					m_pASEData;
	LPDATA					m_pBBXData;
	INT						m_iNumASEData;
	INT						m_iNumBBXData;

	ASEParser*				m_pASEParser;
	BBXParser*				m_pBBXParser;

	std::vector<CBoundBox*> m_pBoundBoxVector;
	//

public:
	VOID Set_D3DDevice( LPDIRECT3DDEVICE9 a_pd3dDevice )
	{
		m_pD3dDevice = a_pd3dDevice;
		this->Initialize();
	}

};

#endif