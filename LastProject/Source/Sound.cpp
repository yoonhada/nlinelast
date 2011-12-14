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
	m_Result = FMOD_Sound_Release( m_pBGM );
	ErrorCheck( m_Result );

	m_Result = FMOD_Sound_Release( m_pEffectSound[0] );
	ErrorCheck( m_Result );

	m_Result = FMOD_Sound_Release( m_pEffectSound[1] );
	ErrorCheck( m_Result );

	m_Result = FMOD_System_Close( m_pSystem );
	ErrorCheck( m_Result );

	m_Result = FMOD_System_Release( m_pSystem );
	ErrorCheck( m_Result );
}


VOID CSound::Update()
{
	// FMOD 상태를 업데이트
	FMOD_System_Update( m_pSystem );
}


VOID CSound::LoadSoundFiles()
{
	// BGM
	m_Result = FMOD_System_CreateStream( m_pSystem, "Sound/BGM/cb_cardbattle_bgm.mp3", FMOD_SOFTWARE | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, NULL, &m_pBGM );
	ErrorCheck( m_Result );

	// EFFECT
	m_Result = FMOD_System_CreateSound( m_pSystem, "Sound/Effect/Dad_Attack.wav", FMOD_HARDWARE | FMOD_CREATESAMPLE | FMOD_LOOP_OFF, NULL, &m_pEffectSound[0] );
	ErrorCheck( m_Result );

	m_Result = FMOD_System_CreateSound( m_pSystem, "Sound/Effect/drip.wav", FMOD_HARDWARE | FMOD_CREATESAMPLE | FMOD_LOOP_OFF, NULL, &m_pEffectSound[1] );
	ErrorCheck( m_Result );
}


VOID CSound::PlayBGM( INT a_iBGM )
{
	m_Result = FMOD_System_PlaySound( m_pSystem, FMOD_CHANNEL_FREE, m_pBGM, FALSE, &m_pBGMChannel );
	ErrorCheck( m_Result );
}


VOID CSound::PlayEffect( INT a_iEffect )
{
	m_Result = FMOD_System_PlaySound( m_pSystem, FMOD_CHANNEL_FREE, m_pEffectSound[a_iEffect], FALSE, &m_pEffectChannel );
	ErrorCheck( m_Result );
}


VOID CSound::StopBGM( INT a_iBGM )
{
	m_Result = FMOD_Channel_Stop( m_pBGMChannel );
	ErrorCheck( m_Result );
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