#include "stdafx.h"

#include "Sound.h"


CSound::CSound()
{
	m_fBGMVolume	= 0.2f;
	m_fEffectVolume = 0.8f;

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
		"Sound/BGM/BGM_Menu.mp3",
		"Sound/BGM/BGM_Lobby.mp3",
		"Sound/BGM/BGM_Game.mp3",
		"Sound/BGM/BGM_Bear.mp3",	
		"Sound/BGM/BGM_Piero.mp3",
	};

	const CHAR* EffectFiles[EFFECT_END] = {
		"Sound/Effect/ButtonOver.wav",
		"Sound/Effect/ButtonClicked.wav",

		"Sound/Effect/SwingSmall.ogg",
		"Sound/Effect/SwingBig.ogg",

		"Sound/Effect/Hit.ogg",

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

		"Sound/Effect/Panda/Panda_Attack1.ogg",
		"Sound/Effect/Panda/Panda_Damaged1.ogg",

		"Sound/Effect/Clown/Clown_Laugh1.ogg",
		"Sound/Effect/Clown/Clown_Laugh2.ogg",
		"Sound/Effect/Clown/Clown_Down.ogg",
		"Sound/Effect/Clown/Clown_Move1.ogg",
		"Sound/Effect/Clown/Clown_Move2.ogg",
		"Sound/Effect/Clown/Clown_Move3.ogg",
		"Sound/Effect/Clown/Clown_Attack1.ogg",
		"Sound/Effect/Clown/Clown_Attack2.ogg",
		"Sound/Effect/Clown/Clown_Attack3.ogg",
		"Sound/Effect/Clown/Clown_Damaged1.ogg",
		"Sound/Effect/Clown/Clown_Damaged2.ogg",
		"Sound/Effect/Clown/Clown_Damaged3.ogg",
		"Sound/Effect/Clown/Clown_Damaged4.ogg",
		"Sound/Effect/Clown/Clown_Death.ogg",

		"Sound/Story/Story1.ogg",
		"Sound/Story/Story2.ogg",
		"Sound/Story/Story3.ogg",
		"Sound/Story/Story4.ogg",
		"Sound/Story/Story5.ogg",
		"Sound/Story/Story6.ogg",
		"Sound/Story/Story7.ogg",
		"Sound/Story/Story8.ogg",
		"Sound/Story/Story9.ogg",
		"Sound/Story/Story10.ogg",
		"Sound/Story/Story11.ogg",
		"Sound/Story/Story12.ogg",
		"Sound/Story/Story13.ogg",
		"Sound/Story/Story14.ogg",
		"Sound/Story/Story15.ogg",
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

	m_Result = FMOD_Channel_SetVolume( m_pBGMChannel, m_fBGMVolume );
	ErrorCheck( m_Result );
}


VOID CSound::PlayEffect( INT a_iEffect )
{
	m_Result = FMOD_System_PlaySound( m_pSystem, FMOD_CHANNEL_FREE, m_pEffectSound[a_iEffect], FALSE, &m_pEffectChannel );
	ErrorCheck( m_Result );

	m_Result = FMOD_Channel_SetVolume( m_pEffectChannel, m_fEffectVolume );
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