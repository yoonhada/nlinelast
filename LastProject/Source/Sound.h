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

		EFFECT_SWING_SMALL,
		EFFECT_SWING_BIG,

		EFFECT_HIT,

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

		EFFECT_PANDA_ATTACK1,
		EFFECT_PANDA_DAMAGED1,

		EFFECT_CLOWN_LAUGH1,
		EFFECT_CLOWN_LAUGH2,
		EFFECT_CLOWN_DOWN,
		EFFECT_CLOWN_MOVE1,
		EFFECT_CLOWN_MOVE2,
		EFFECT_CLOWN_MOVE3,
		EFFECT_CLOWN_ATTACK1,
		EFFECT_CLOWN_ATTACK2,
		EFFECT_CLOWN_ATTACK3,
		EFFECT_CLOWN_DAMAGED1,
		EFFECT_CLOWN_DAMAGED2,
		EFFECT_CLOWN_DAMAGED3,
		EFFECT_CLOWN_DAMAGED4,
		EFFECT_CLOWN_DEATH,

		STROY_STORY1,
		STROY_STORY2,
		STROY_STORY3,
		STROY_STORY4,
		STROY_STORY5,
		STROY_STORY6,
		STROY_STORY7,
		STROY_STORY8,
		STROY_STORY9,
		STROY_STORY10,
		STROY_STORY11,
		STROY_STORY12,
		STROY_STORY13,
		STROY_STORY14,
		STROY_STORY15,

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

	FLOAT			m_fBGMVolume;
	FLOAT			m_fEffectVolume;

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

	VOID SetBGMVolume( FLOAT a_fVolume )		{ m_fBGMVolume = a_fVolume; FMOD_Channel_SetVolume( m_pBGMChannel, m_fBGMVolume ); }
	VOID SetEffectVolume( FLOAT a_fVolume )		{ m_fEffectVolume = a_fVolume; }
};



#endif