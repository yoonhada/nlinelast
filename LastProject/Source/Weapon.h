/**
@file	CManageCube.h
@date	2011/09/20
@author	
@brief	무기 클래스
*/

#pragma once
class CWeapon : public virtual IObject
{
private:
	const INT m_nFrame;
	INT m_nType;
	INT m_nState;
	INT m_nKeyFrame;
	// 0x01 A
	// 0x10 B

public:
	enum _WEAPON { NONE, HAMMER, FRYPEN, GUITAR, CANDY };
	CWeapon( );
	virtual ~CWeapon();

	virtual VOID Clear()			{}
	virtual HRESULT Create();
	virtual HRESULT Release();
	virtual VOID Render();
	virtual VOID Update();

	VOID SetType(INT _nType)		{ m_nType = _nType; };
	VOID SetKeyA();
	VOID SetKeyB();
};