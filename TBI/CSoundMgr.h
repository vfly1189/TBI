#pragma once
// sound �뵵 - ��� ���� �߿�

class CSound;
class CObject;

#define SOUNDBUFFERSIZE 64

//����ȭ�� ���� ���� ������� ���� ���. 
static wstring walkSoundKey[4] = { L"Step1", L"Step2",L"Step3",L"Step4" };
const float	   fWalkDuration = 0.3f;
float		   fWalkElapsed = 0.f;


struct SoundInfo {
	FMOD::Sound* m_pSound = nullptr;
	FMOD::Channel* m_pChannel = nullptr;
	bool			isBGM = false;
	bool isLoop = false;
};

class CSoundMgr
{
	SINGLE(CSoundMgr)

private:
	unordered_map <wstring, SoundInfo*> m_mapSounds;

	FMOD::System* m_pSystem;			//�ý��� ������
	FMOD::Sound* m_pSound;			//���� 1��. 
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
	void update();			//�ݺ������ ������ ���� ����.

public:
	void AddSound(wstring _keyName, wstring _fileName, bool _bgm = false, bool _loop = false);
	void Play(wstring _keyName, float _volume = 1.0f);
	void Stop(wstring _keyName);
	void Pause(wstring _keyName);
	void Resume(wstring _keyName);

	bool IsPlaySound(wstring& _keyName);
	bool IsPauseSound(wstring _keyName);

public:
};