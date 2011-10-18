/**
	@file	Frequency.cpp
	@date	2011/09/17
	@author	황윤하
	@brief	프레임 측정
*/

#include "stdafx.h"

CFrequency::CFrequency() : m_fFrequency( 0.0f ) 
						 , m_fFrametime( 0.0f ) 
						 , m_nFrames( 0 )
{
	QueryPerformanceFrequency( &Frequency );
}

CFrequency::~CFrequency()
{
}

VOID CFrequency::Update()
{
	QueryPerformanceCounter( &EndCounter );
	m_fFrametime = static_cast<FLOAT>( EndCounter.QuadPart - StartCounter.QuadPart ) / static_cast<FLOAT>(Frequency.QuadPart);
	if( m_fFrametime > 1.0f )
	{
		m_fFrametime = 0.0f;
	}

	StartCounter = EndCounter;
	m_fFrequency = 1.0f / m_fFrametime;

	m_nFrames = 0;
}