#pragma once
#ifndef _GUIBTN_MANAGER_H_
#define _GUIBTN_MANAGER_H_

class GUIBtn;

#include <list>

class GUIBtnManager
{
private:
	VOID		Initialize();
	VOID		Release();

	VOID		Cleanup();

public:
	GUIBtnManager( LPDIRECT3DDEVICE9 _pd3dDevice ) : m_pd3dDevice( _pd3dDevice )
	{
		this->Initialize();
	}
	~GUIBtnManager()
	{
		this->Release();
	}

	typedef struct _DATA
	{
		DWORD		ID;
		GUIBtn*		pGUIBtn;

		_DATA()
		{
			pGUIBtn = NULL;
		}
	}DATA, *LPDATA;

	typedef std::list< LPDATA > DATALIST;

	VOID		Create(		DWORD _ID,						//	ID Resource
							DWORD _Style,					//	Style GBS_PUSH || GBS_CHECK
							FLOAT _fX, FLOAT _fY,			//	Button Position
							FLOAT _fWidth, FLOAT _fHeight,	//	Button Width, Height
							LPCWSTR _Normal,				//	Normal TextureFileName
							LPCWSTR _Hot,					//	Hot TextureFileName
							LPCWSTR _Down,					//	Down TextureFileName
							LPCWSTR _Disable				//	Disable TextureFileName
							);
	VOID		Update( INT x, INT y );
	VOID		Render();

	//	일단 INT값으로 마우스 좌표를 받읍시다~!
	VOID		OnDown( INT x, INT y );
	VOID		OnMove( INT x, INT y );
	VOID		OnUp( INT x, INT y );

	VOID		GetCommandID( DWORD& _ID );

private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;

	DATALIST				m_lstData;

public:

};

#endif