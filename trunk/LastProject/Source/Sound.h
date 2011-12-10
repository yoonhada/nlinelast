#ifndef _SOUND_H_
#define _SOUND_H_


class CSound : public CSingleton<CSound>
{
	friend class CSingleton<CSound>;

private:
	FMOD_SYSTEM*	m_pSystem;
	FMOD_SOUND*		m_pSound;
	FMOD_CHANNEL*	m_pChannel;
	FMOD_RESULT		m_Result;
	UINT			m_iVersion;

	CSound();
	~CSound();


public:
	VOID Initialize();
	VOID Release();

	VOID PlayBGM( INT a_iBGM );
	VOID PlayEffect( INT a_iEffect );

	VOID LoadSoundFiles();
	VOID ErrorCheck( FMOD_RESULT a_Result );
	
};



#endif