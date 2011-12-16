#ifndef _SOUND_H_
#define _SOUND_H_


class CSound : public CSingleton<CSound>
{
	friend class CSingleton<CSound>;

public:
	enum BGM_ID
	{
		BGM_MENU = 0,
		BGM_LOBBY,
		BGM_GAME,
		BGM_EVENT,

		BGM_END,
	};

	enum EFFECT_ID
	{
		EFFECT_BUTTON_OVER = 0,
		EFFECT_BUTTON_CLICKED,

		EFFECT_DAD_ATTACK1,
		EFFECT_MON_ATTACK1,
		EFFECT_BOY_ATTACK1,
		EFFECT_GIRL_ATTACK1,

		EFFECT_DAD_ATTACK2,
		EFFECT_MON_ATTACK2,
		EFFECT_BOY_ATTACK2,
		EFFECT_GIRL_ATTACK2,

		EFFECT_DAD_DAMAGED1,
		EFFECT_MOM_DAMAGED1,
		EFFECT_BOY_DAMAGED1,
		EFFECT_GIRL_DAMAGED1,

		EFFECT_DAD_DAMAGED2,
		EFFECT_MOM_DAMAGED2,
		EFFECT_BOY_DAMAGED2,
		EFFECT_GIRL_DAMAGED2,

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
	FMOD_SOUND*		m_pBGM[BGM_END];
	FMOD_SOUND*		m_pEffectSound[EFFECT_END];
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

	VOID LoadSoundFiles();
	VOID LoadBGMFiles( const CHAR* a_szFileName, INT a_i );
	VOID LoadEffectFiles( const CHAR* a_szFileName, INT a_i );

	VOID PlayBGM( INT a_iBGM );
	VOID PlayEffect( INT a_iEffect );

	VOID StopBGM( INT a_iBGM );

	VOID ErrorCheck( FMOD_RESULT a_Result );
	
};



#endif