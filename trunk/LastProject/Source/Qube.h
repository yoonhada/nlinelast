/**
@file	CQube.h
@date	2011/09/06
@author	
@brief	��ť��
*/

#pragma once

class CCharactor;

#include "CharCube.h"

class CQube : public CCharCube
{
	enum { GROUND = 0, };
private:
	D3DXVECTOR3 m_vMomentum;		///< �߷� ���ӵ� ����
	D3DXVECTOR3 m_vAccelerate;		///< ���ӵ�
	const FLOAT m_fItemLift;		///< �ּ� ���ӵ� 
	//VOID Translate();

public:
	CQube();
	virtual ~CQube();

	VOID Update( CBoundBox * pBB );
	VOID RandMome( D3DXVECTOR3 vMomentum, FLOAT fRate = 1.0f );

private:
	D3DXVECTOR3 m_vPos;				///< �߽���ǥ
	D3DXVECTOR3 m_vAxisDir[3];		///< ť�� ������ ��������
	D3DXVECTOR3 m_vRotateTemp;		///< ȸ�� ��ȭ��
	D3DXVECTOR3 m_vRotate;			///< ���� ȸ����
	BOOL m_bVisiable;				///< 
	FLOAT m_fSize;					///< ť�� ������
	D3DXMATRIXA16 m_matChara;		///< ���� ��� ĳ���� ���� ��Ʈ���� ��� ����
	FLOAT m_fHeight;					///< �ٴ����� ������ ���� ������


	// �ӵ��� ���� �ӽ� �����
	D3DXVECTOR3 vDir;
	std::vector<CBoundBox*> * vecBoundBox;
	std::vector<CBoundBox*>::iterator Iter;

public:
	// Get
	FLOAT GetSize()					{ return m_fSize; }
	BOOL GetVisiable()				{ return m_bVisiable; }
	D3DXVECTOR3 GetPosition()		{ return m_vPos; }
	D3DXVECTOR3 GetAxisDir(INT n)	{ return m_vAxisDir[n]; }
	D3DXMATRIXA16 GetCharMatrix()
	{ 
		return m_matChara; 
	}

	// Set
	VOID SetVisiable( BOOL bVisi)			{ m_bVisiable = bVisi; }
	VOID SetPosition( D3DXVECTOR3 vec )		{ m_vPos = vec; }
	VOID SetCharMatrix( const D3DXMATRIXA16& a_matChara )
	{
		m_matChara = a_matChara;
	}
	VOID SetHeight( FLOAT a_fHeight )
	{
		m_fHeight = a_fHeight;
	}

	// temp
	virtual VOID SetRander();
	VOID RanderPos( FLOAT fRate = 10.0f );
	VOID RanderRotate( FLOAT fRate = 0.25f );
	VOID RanderSize( FLOAT fRate = 1.0f );

	BOOL InRect(D3DXVECTOR3 &_vec);
};