#pragma once
#ifndef _GUICARET_H_
#define _GUICARET_H_

#include "GUIBase.h"

//	싱글톤 생성자를 포인트로
//	생성 시키는 방법을 다시 생각할 필요가 있다
//	아...거지 같아
//	싱글톤으로 만들어야 되
//	생성자에서 LPDIRECT3DDEVICE9, LPD3DXSPRITE를 받으면 이런일이 생기는구나....
//	음니ㅏ럼ㄴ디런야ㅣ허ㅗㅑㅔㄴㅇ
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