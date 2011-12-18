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
	// BGM
	for( INT i=0; i<BGM_END; ++i )
	{
		m_Result = FMOD_Sound_Release( m_pBGM[i] );
		ErrorCheck( m_Result );
	}

	// Effect
	for( INT i=0; i<EFFECT_END; ++i )
	{
		m_Result = FMOD_Sound_Release( m_pEffectSound[i] );
		ErrorCheck( m_Result );
	}

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
	const CHAR* BGMFiles[BGM_END] = {
		"Sound/BGM/cb_cardbattle_bgm.mp3",
		"Sound/BGM/cb_cardbattle_bgm.mp3",
		"Sound/BGM/MainBGM.mp3",
		"Sound/BGM/cb_cardbattle_bgm.mp3",
	};

	const CHAR* EffectFiles[EFFECT_END] = {
		"Sound/Effect/drip.wav",
		"Sound/Effect/drip.wav",

		"Sound/Effect/Dad/Dad_Attack1.mp3",
		"Sound/Effect/Mom/Mom_Attack1.mp3",
		"Sound/Effect/Boy/Boy_Attack1.mp3",
		"Sound/Effect/Girl/Girl_Attack1.mp3",

		"Sound/Effect/Dad/Dad_Attack2.mp3",
		"Sound/Effect/Mom/Mom_Attack2.mp3",
		"Sound/Effect/Boy/Boy_Attack2.mp3",
		"Sound/Effect/Girl/Girl_Attack2.mp3",

		"Sound/Effect/Dad/Dad_Damaged1.mp3",
		"Sound/Effect/Mom/Mom_Damaged1.mp3",
		"Sound/Effect/Boy/Boy_Damaged1.mp3",
		"Sound/Effect/Girl/Girl_Damaged1.mp3",

		"Sound/Effect/Dad/Dad_Damaged2.mp3",
		"Sound/Effect/Mom/Mom_Damaged2.mp3",
		"Sound/Effect/Boy/Boy_Damaged2.mp3",
		"Sound/Effect/Girl/Girl_Damaged2.mp3",

		"Sound/Effect/Panda_Attack.wav",
		"Sound/Effect/Clown_Attack.wav",

		"Sound/Effect/Panda_Damaged.wav",
		"Sound/Effect/Clown_Damaged.wav",

		"Sound/Effect/Combo_Success.wav",
		"Sound/Effect/Combo_Fail.wav",

		"Sound/Effect/Event_Baby.wav",
		"Sound/Effect/Event_Clown.wav",
	};

	for( INT i=0; i<BGM_END; ++i )
	{
		LoadBGMFiles( BGMFiles[i], i );
	}

	for( INT i=0; i<EFFECT_END; ++i )
	{
		LoadEffectFiles( EffectFiles[i], i );
	}
}


VOID CSound::LoadBGMFiles( const CHAR* a_szFileName, INT a_i )
{
	// BGM
	m_Result = FMOD_System_CreateStream( m_pSystem, a_szFileName, FMOD_SOFTWARE | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, NULL, &m_pBGM[a_i] );
	ErrorCheck( m_Result );
}


VOID CSound::LoadEffectFiles( const CHAR* a_szFileName, INT a_i )
{
	// EFFECT
	m_Result = FMOD_System_CreateSound( m_pSystem, a_szFileName, FMOD_HARDWARE | FMOD_CREATESAMPLE | FMOD_LOOP_OFF, NULL, &m_pEffectSound[a_i] );
	ErrorCheck( m_Result );
}


VOID CSound::PlayBGM( INT a_iBGM )
{
	m_Result = FMOD_System_PlaySound( m_pSystem, FMOD_CHANNEL_FREE, m_pBGM[a_iBGM], FALSE, &m_pBGMChannel );
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