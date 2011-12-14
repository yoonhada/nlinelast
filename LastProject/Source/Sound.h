#ifndef _SOUND_H_
#define _SOUND_H_


class CSound : public CSingleton<CSound>
{
	friend class CSingleton<CSound>;

public:
	enum BGM_ID
	{
		BGM_START = 0,

		BGM_MENU ,
		BGM_LOBBY,
		BGM_GAME,
		BGM_EVENT,

		BGM_END,
	};

	enum EFFECT_ID
	{
		EFFECT_START = 0,

		EFFECT_BUTTON_CLICKED,

		EFFECT_DAD_ATTACK,
		EFFECT_MON_ATTACK,
		EFFECT_BOY_ATTACK,
		EFFECT_GIRL_ATTACK,

		EFFECT_DAD_DAMAGED,
		EFFECT_MOM_DAMAGED,
		EFFECT_BOY_DAMAGED,
		EFFECT_GIRL_DAMAGED,

		EFFECT_PANDA_ATTACK,
		EFFECT_CLOWN_ATTACK,

		EFFECT_PANDA_DAMAGED,
		EFFECT_CLOWN_DAMAGED,

		EFFECT_EVENT_COMBO_SUCCESS,
		EFFECT_EVENT_COMBO_FAIL,

		EFFECT_EVENT_BABY,
		EFFECT_EVENT_CLOWN,

		EFFECT_END,
	};


private:
	FMOD_SYSTEM*	m_pSystem;
	FMOD_SOUND*		m_pBGM;
	FMOD_SOUND*		m_pEffectSound[2];
	FMOD_CHANNEL*	m_pBGMChannel;
	FMOD_CHANNEL*	m_pEffectChannel;
	FMOD_RESULT		m_Result;
	UINT			m_iVersion;

	CSound();
	virtual ~CSound();


public:
	VOID Initialize();
	VOID Release();

	VOID Update();

	VOID PlayBGM( INT a_iBGM );
	VOID PlayEffect( INT a_iEffect );

	VOID StopBGM( INT a_iBGM );

	VOID LoadSoundFiles();
	VOID ErrorCheck( FMOD_RESULT a_Result );
	
};



#endif