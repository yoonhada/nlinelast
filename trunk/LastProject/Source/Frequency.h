/**
	@file	Frequency.h
	@date	2011/09/17
	@author	Ȳ����
	@brief	������ ����
*/

#ifndef _Frequency_H_
#define _Frequency_H_

/**
	@class	CFrequency
	@date	2011/09/17
	@author	Ȳ����
	@brief	������ ���� Ŭ����
*/
class CFrequency : public CSingleton<CFrequency>
{
	friend class CSingleton<CFrequency>;

public:
	VOID Update();
	FLOAT getFrequency()	{ return m_fFrequency <= 0 ? 1 : m_fFrequency; }
	FLOAT getFrametime()	{ return m_fFrametime; }

private:
	CFrequency();
	~CFrequency();

	LARGE_INTEGER Frequency;
	LARGE_INTEGER StartCounter, EndCounter;

	INT m_nFrames;
	FLOAT m_fFrequency;
	FLOAT m_fFrametime;
};

#endif