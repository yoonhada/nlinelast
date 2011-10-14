/**
	@class	DirectX9
	@date	2011/08/26
	@author	����ö
	@brief	DirectX Ŭ����
*/


#ifndef _DIRECTX9_H_
#define _DIRECTX9_H_


class CDirectX9
{
public:
	CDirectX9();
	~CDirectX9();

	VOID	Clear();
	HRESULT	Create( HWND a_hWnd ); ///< �ʱ�ȭ
	VOID	Release(); ///< ������Ʈ�� ����

	VOID beginScene(); ///< ������ ����
	VOID endScene(); ///< ������ ��

	LPDIRECT3DDEVICE9 GetDevice()	{ return m_pD3dDevice; } ///< ����̽��� ����.
	D3DPRESENT_PARAMETERS GetD3dpp() { return m_D3dpp; } ///< ����̽� ����ü ��ȯ

private:
	LPDIRECT3D9				m_pD3D; ///< D3D ��ü
	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< D3D ����̽�
	D3DPRESENT_PARAMETERS	m_D3dpp; ///< ����̽� ������ ����ϴ� ����ü
};


#endif