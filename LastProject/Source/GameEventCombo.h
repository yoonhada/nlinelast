/**
	@class	CGameEventCombo
	@date	2011/12/05
	@author	yoonhada@gmail.com
	@brief	게임이벤트
*/

#pragma once

class GUIBase;
class GUIBackground;

class CGameEventCombo : public IObject
{
private:
	LPDIRECT3DDEVICE9		m_pD3dDevice;
	LPD3DXSPRITE			m_pSprite;
	HWND					m_hWnd;

	GUIBase*				m_pGUIBase;
	GUIBackground*			m_pGUIBackground[4];

	INT						m_nKindEvent[4];
public:
	CGameEventCombo( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite);
	virtual ~CGameEventCombo();

	VOID Clear();
	HRESULT Create();	///< 초기화
	HRESULT Release();	///< 오브젝트들 제거
	VOID Update();		///< Update
	VOID Render();		///< Render

	VOID AddCombo(INT nIndex, INT nKind)	{ m_nKindEvent[nIndex] = nKind; }
};
