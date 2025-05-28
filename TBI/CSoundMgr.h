#pragma once
// sound 용도 - 헤더 순서 중요

class CSound;
class CObject;

#define SOUNDBUFFERSIZE 64


struct SoundInfo {
	FMOD::Sound* m_pSound = nullptr;
	//FMOD::Channel* m_pChannel = nullptr;
	std::vector<FMOD::Channel*> m_pChannels; // 채널을 벡터로 관리
	bool			isBGM = false;
	bool isLoop = false;
};

class CSoundMgr
{
	SINGLE(CSoundMgr)

private:
	unordered_map <wstring, SoundInfo*> m_mapSounds;

	FMOD::System* m_pSystem;			//시스템 포인터
	FMOD::Sound* m_pSound;			//사운드 1개. 
	FMOD::Channel* m_pChannel;

	FMOD::ChannelGroup* m_pBGMChannelGroup;
	FMOD::ChannelGroup* m_pSFXChannelGroup;

public:
	CObject* m_pMasterSoundRatio;
	CObject* m_pBGMSoundRatio;
	CObject* m_pSFXSoundRatio;

	float			m_fMasterRatio;
	float			m_fBGMRatio;
	float			m_fSFXRatio;

	int				m_iStepIdx;

public:
	HRESULT init();
	void Release();
	void update();			//반복해줘야 정상적 실행 가능.

public:
	void AddSound(wstring _keyName, wstring _fileName, bool _bgm = false, bool _loop = false);
	void Play(wstring _keyName, float _volume = 1.0f);
	void Stop(wstring _keyName);
	void Pause(wstring _keyName);
	void Resume(wstring _keyName);

	void StopAllSound();


	bool IsPlaySound(wstring& _keyName);
	bool IsPauseSound(wstring _keyName);

public:
};