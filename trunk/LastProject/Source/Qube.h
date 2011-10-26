/**
@file	CQube.h
@date	2011/09/06
@author	
@brief	블럭큐브
*/

#pragma once

class CCharactor;

#include "CharCube.h"

class CQube : public CCharCube
{
	enum { GROUND = 0, };
private:
	D3DXVECTOR3 m_vMomentum;		///< 중력 가속도 누적
	D3DXVECTOR3 m_vAccelerate;		///< 가속도
	const FLOAT m_fItemLift;		///< 최소 가속도 
	//VOID Translate();

public:
	CQube();
	virtual ~CQube();

	VOID Update( CBoundBox * pBB );
	VOID RandMome( D3DXVECTOR3 vMomentum, FLOAT fRate = 1.0f );

private:
	D3DXVECTOR3 m_vPos;				///< 중심좌표
	D3DXVECTOR3 m_vAxisDir[3];		///< 큐브 세축의 단위벡터
	D3DXVECTOR3 m_vRotateTemp;		///< 회전 변화량
	D3DXVECTOR3 m_vRotate;			///< 현재 회전량
	BOOL m_bVisiable;				///< 
	FLOAT m_fSize;					///< 큐브 반지름
	D3DXMATRIXA16 m_matChara;		///< 생성 당시 캐릭터 월드 매트릭스 행렬 저장
	FLOAT m_fHeight;					///< 바닥으로 부터의 높이 보정값


	// 속도를 위한 임시 저장소
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