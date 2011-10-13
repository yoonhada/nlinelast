/**
	@file	I_ObjectSRT.h
	@date	2011/09/20
	@author	백경훈
	@brief	오브젝트 SRT 클래스
*/

#ifndef _ObjectSRT_H_
#define _ObjectSRT_H_

/**
	@class	IObjectSRT
	@date	2011/09/20
	@author	백경훈
	@brief	오브젝트 SRT 클래스
*/

class CObjectSRT
{
public:
	CObjectSRT(VOID);
	virtual ~CObjectSRT(VOID);

	VOID Set_ControlScale( INT a_Key, FLOAT a_Value )
	{
		m_ControlScale[a_Key] = a_Value;
	}

	VOID Set_ControlRotate( INT a_Key, FLOAT a_Value )
	{
		m_ControlRotate[a_Key] = a_Value;
	}

	VOID Set_ControlTranslate( INT a_Key, FLOAT a_Value )
	{
		m_ControlTranslate[a_Key] = a_Value;
	}

	const D3DXMATRIXA16& Get_MatWorld()
	{
		return m_matWorld;
	}

	VOID Calcul_MatWorld();


private:
	D3DXMATRIXA16 m_matScale;
	D3DXMATRIXA16 m_matRotate[3];
	D3DXMATRIXA16 m_matTranslate;
	D3DXMATRIXA16 m_matWorld;

	FLOAT m_ControlScale[3];
	FLOAT m_ControlRotate[3];
	FLOAT m_ControlTranslate[3];
	
};

#endif