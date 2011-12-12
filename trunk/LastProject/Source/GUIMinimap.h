//#pragma once
//#ifndef _GUIMINIMAP_H_
//#define _GUIMINIMAP_H_
//
//class GUIBase;
//class GUIBackground;
//
//class GUIMinimap
//{
//private:
//	VOID		Initialize();
//	VOID		Release();
//
//public:
//	enum {	MNM_MOM = 0, MNM_DADDY, MNM_SON, MNM_DAUGHTER, 
//			MNM_FIRSTAIDKIT_0, MNM_FIRSTAIDKIT_1, MNM_FIRSTAIDKIT_2, MNM_FIRSTAIDKIT_3, MNM_FIRSTAIDKIT_4,
//			MNM_CROWN, MNM_PANDA, MNM_BEAR,
//			MNM_END };
//			
//	GUIMinimap( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : m_pd3dDevice( _pd3dDevice ), m_pSprite( _pSprite )
//	{
//		this->Initialize();
//	}
//	~GUIMinimap()
//	{
//		this->Release();
//	}
//
//	VOID		Create();
//	VOID		Update();
//	VOID		Render();
//
//	VOID		SetVisible( INT _iIndex, BOOL _bVisible );
//	VOID		SetPosition( INT _iIndex, LPD3DXVECTOR3 _pvecPosition );
//
//private:
//	typedef struct _DATA
//	{
//		BOOL			bVisible;
//		GUIBackground*	pImage;
//
//		_DATA()
//		{
//			bVisible	= FALSE;
//			pImage		= NULL;
//		}
//	}DATA, *LPDATA;
//
//	LPDIRECT3DDEVICE9	m_pd3dDevice;
//	LPD3DXSPRITE		m_pSprite;
//
//	GUIBase*			m_pGUIBase;
//
//	GUIBackground*		m_pMap;
//	DATA				m_aData[ 12 ];
//
//	FLOAT				m_fMapWidth,		m_fMapHeight;
//	FLOAT				m_fRatioX,			m_fRatioY;
//
//	FLOAT				m_fMiniMapX,		m_fMiniMapY;
//	FLOAT				m_fMiniMapWidth,	m_fMiniMapHeight;
//
//public:
//
//};
//
//#endif