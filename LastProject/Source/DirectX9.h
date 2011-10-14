/**
	@class	DirectX9
	@date	2011/08/26
	@author	김종철
	@brief	DirectX 클래스
*/


#ifndef _DIRECTX9_H_
#define _DIRECTX9_H_


class CDirectX9
{
public:
	CDirectX9();
	~CDirectX9();

	VOID	Clear();
	HRESULT	Create( HWND a_hWnd ); ///< 초기화
	VOID	Release(); ///< 오브젝트들 제거

	VOID beginScene(); ///< 렌더링 시작
	VOID endScene(); ///< 렌더링 끝

	LPDIRECT3DDEVICE9 GetDevice()	{ return m_pD3dDevice; } ///< 디바이스를 얻어낸다.
	D3DPRESENT_PARAMETERS GetD3dpp() { return m_D3dpp; } ///< 디바이스 구조체 반환

private:
	LPDIRECT3D9				m_pD3D; ///< D3D 객체
	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< D3D 디바이스
	D3DPRESENT_PARAMETERS	m_D3dpp; ///< 디바이스 생성에 사용하는 구조체
};


#endif