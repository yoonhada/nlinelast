#pragma once
#ifndef _GUICARET_H_
#define _GUICARET_H_

#include "GUIBase.h"

//	�̱��� �����ڸ� ����Ʈ��
//	���� ��Ű�� ����� �ٽ� ������ �ʿ䰡 �ִ�
//	��...���� ����
//	�̱������� ������ ��
//	�����ڿ��� LPDIRECT3DDEVICE9, LPD3DXSPRITE�� ������ �̷����� ����±���....
//	���Ϥ������𷱾ߤ���Ǥ��Ĥ���
class GUICaret : public GUIBase
{
private:
	VOID		Initialize();
	VOID		Release();

public:
	GUICaret( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) : GUIBase( _pd3dDevice, _pSprite )
	{
		this->Initialize();
	}

	virtual ~GUICaret()
	{
		this->Release();
	}

	VOID		Create( IMAGEPARAM& _imgParam );
	VOID		Render();
	VOID		Update();

	VOID		SetSize( INT _iWidth, INT _iHeight );

	VOID		ConnectText( LPWSTR _pText, FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight );
	VOID		DisconnectText();

private:
	IMAGE3D		m_img3DCaret;

	LPWSTR		m_pText;
	FLOAT		m_fX, m_fY;
	
public:

};

#endif