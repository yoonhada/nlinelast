#include "stdafx.h"

#include "Sound.h"


CSound::CSound()
{
	Initialize();
}


CSound::~CSound()
{
	Release();
}


VOID CSound::Initialize()
{
	// 초기화
	m_Result = FMOD_System_Create( &m_pSystem );
	ErrorCheck( m_Result );

	m_Result = FMOD_System_GetVersion( m_pSystem, &m_iVersion );
	ErrorCheck( m_Result );

	if( m_iVersion < FMOD_VERSION )
	{
#ifndef _DEBUG
		CDebugConsole::GetInstance()->Messagef( L"FMOD Error: old version FMOD. \n" );
#endif
	}

	m_Result = FMOD_System_Init( m_pSystem, 32, FMOD_INIT_NORMAL, NULL );

	LoadSoundFiles();
}


VOID CSound::Release()
{
	m_Result = FMOD_Sound_Release( m_pSound );
	ErrorCheck( m_Result );

	m_Result = FMOD_System_Close( m_pSystem );
	ErrorCheck( m_Result );

	m_Result = FMOD_System_Release( m_pSystem );
	ErrorCheck( m_Result );
}


VOID CSound::LoadSoundFiles()
{
	// BGM
//	m_Result = FMOD_System_CreateStream( m_pSystem, "c.ogg", FMOD_SOFTWARE | FMOD_CREATESTREAM, NULL, &m_pSound );
//	ErrorCheck( m_Result );

	// EFFECT
//	m_Result = FMOD_System_CreateSound( m_pSystem, "c.ogg", FMOD_HARDWARE | FMOD_CREATESAMPLE, NULL, &m_pSound );
//	ErrorCheck( m_Result );
}


VOID CSound::PlayBGM( INT a_iBGM )
{
	m_Result = FMOD_System_PlaySound( m_pSystem, FMOD_CHANNEL_FREE, m_pSound, FALSE, &m_pChannel );
	ErrorCheck( m_Result );

	// FMOD 상태를 업데이트
	FMOD_System_Update( m_pSystem );
}


VOID CSound::PlayEffect( INT a_iEffect )
{
	m_Result = FMOD_System_PlaySound( m_pSystem, FMOD_CHANNEL_FREE, m_pSound, FALSE, &m_pChannel );
	ErrorCheck( m_Result );

	// FMOD 상태를 업데이트
	FMOD_System_Update( m_pSystem );
}


VOID CSound::ErrorCheck( FMOD_RESULT a_Result )
{
	if( a_Result != FMOD_OK )
	{
#ifdef _DEBUG
		CDebugConsole::GetInstance()->Messagef( L"FMOD Error: (%d) %s \n", m_Result, FMOD_ErrorString( a_Result ) );
#endif
	}
}