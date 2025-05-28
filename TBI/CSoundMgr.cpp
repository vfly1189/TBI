#include "global.h"
#include "CSoundMgr.h"
#include "CSoundMgr.h" 
#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"

CSoundMgr::CSoundMgr()
	: m_pSystem(nullptr)
	, m_pChannel(nullptr)
	, m_pSound(nullptr)
	, m_fMasterRatio(1.f)
	, m_fBGMRatio(1.f)
	, m_fSFXRatio(1.f)
	, m_iStepIdx(0)
{
	
}

CSoundMgr::~CSoundMgr()
{
	Release();
}

HRESULT CSoundMgr::init()
{
	FMOD_RESULT ret;
	ret = FMOD::System_Create(&m_pSystem);

	if (ret != FMOD_OK) {
		return E_FAIL;
	}

	ret = m_pSystem->init(SOUNDBUFFERSIZE, FMOD_INIT_NORMAL, 0);

	if (ret != FMOD_OK) {
		return E_FAIL;
	}

	ret = m_pSystem->createChannelGroup("BGM", &m_pBGMChannelGroup);
	if (ret != FMOD_OK) {
		return E_FAIL;
	}

	ret = m_pSystem->createChannelGroup("SFX", &m_pSFXChannelGroup);
	if (ret != FMOD_OK) {
		return E_FAIL;
	}

	return S_OK;
}

void CSoundMgr::Release()
{
	//Sound�� ���ο� �����ϴ� ���� �־ SAFE_DELETE_MAP �Լ� �� ����.
	for (auto soundData : m_mapSounds) {
		soundData.second->m_pSound->release();
		delete soundData.second;
	}
	m_mapSounds.clear();

	if (m_pBGMChannelGroup) {
		m_pBGMChannelGroup->release();
		m_pBGMChannelGroup = nullptr;
	}

	if (m_pSFXChannelGroup) {
		m_pSFXChannelGroup->release();
		m_pSFXChannelGroup = nullptr;
	}

	if (nullptr != m_pSystem) {
		m_pSystem->release();
		m_pSystem->close();
	}
}

void CSoundMgr::update()
{
	m_pSystem->update();
}

void CSoundMgr::AddSound(wstring _keyName, wstring _fileName, bool _bgm, bool _loop)
{
	FMOD_RESULT ret;

	auto soundIter = m_mapSounds.find(_keyName);

	if (soundIter != m_mapSounds.end()) {
		return;
	}


	SoundInfo* info = new SoundInfo;
	info->isBGM = _bgm;
	info->isLoop = _loop;

	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _fileName;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string strFilePathNarrow = converter.to_bytes(strFilePath);

	if (_bgm) {
		//BGM�϶�.

		if (_loop)
		{
			ret = m_pSystem->createStream(strFilePathNarrow.c_str(), FMOD_LOOP_NORMAL | FMOD_CREATECOMPRESSEDSAMPLE, nullptr, &info->m_pSound);
		}
		else
			ret = m_pSystem->createStream(strFilePathNarrow.c_str(), FMOD_DEFAULT | FMOD_CREATECOMPRESSEDSAMPLE, nullptr, &info->m_pSound);
	}
	else {
		//BGM�� �ƴ� SFX�϶�. 

		if (_loop) {
			ret = m_pSystem->createSound(strFilePathNarrow.c_str(), FMOD_LOOP_NORMAL | FMOD_CREATECOMPRESSEDSAMPLE, nullptr, &info->m_pSound);
		}
		else {
			ret = m_pSystem->createSound(strFilePathNarrow.c_str(), FMOD_DEFAULT | FMOD_CREATECOMPRESSEDSAMPLE, nullptr, &info->m_pSound);
		}
	}

	if (ret != FMOD_OK) assert(false);

	m_mapSounds.insert(make_pair(_keyName, info));
}

void CSoundMgr::Play(wstring _keyName, float _volume)
{
	auto soundIter = m_mapSounds.find(_keyName);

	if (soundIter == m_mapSounds.end()) {
		assert(false);
	}
	
	FMOD::Channel* newChannel = nullptr;

	if (soundIter->second->isBGM) {
		m_pSystem->playSound(soundIter->second->m_pSound, m_pBGMChannelGroup, false, &newChannel);
		newChannel->setVolume(_volume);
	}
	else {
		m_pSystem->playSound(soundIter->second->m_pSound, m_pSFXChannelGroup, false, &newChannel);
		newChannel->setVolume(_volume);

		// ���� ����
		if (soundIter->second->isLoop) {
			newChannel->setLoopCount(-1); // ���� ����
		}
	}

	// �� ä���� ���Ϳ� �߰�
	soundIter->second->m_pChannels.push_back(newChannel);
}

void CSoundMgr::Stop(wstring _keyName)
{
	auto soundIter = m_mapSounds.find(_keyName);
	if (soundIter == m_mapSounds.end()) {
		assert(false);
		return;
	}

	// ��� ä�� ����
	for (auto& channel : soundIter->second->m_pChannels) {
		if (channel) {
			channel->setLoopCount(0); // ���� ����
			channel->stop();
		}
	}

	// ä�� ��� ����
	soundIter->second->m_pChannels.clear();
}

void CSoundMgr::Pause(wstring _keyName)
{
	auto soundIter = m_mapSounds.find(_keyName);

	if (soundIter == m_mapSounds.end()) {
		assert(false);
	}

	// ��� ä�� ����
	for (auto& channel : soundIter->second->m_pChannels) {
		if (channel) {
			channel->setPaused(true);
		}
	}
}

void CSoundMgr::Resume(wstring _keyName)
{
	auto soundIter = m_mapSounds.find(_keyName);

	if (soundIter == m_mapSounds.end()) {
		assert(false);
	}

	// ��� ä�� �ٽ����
	for (auto& channel : soundIter->second->m_pChannels) {
		if (channel) {
			channel->setPaused(false);
		}
	}
}

void CSoundMgr::StopAllSound()
{
	unordered_map <wstring, SoundInfo*>::iterator iter;
	for (iter = m_mapSounds.begin(); iter != m_mapSounds.end(); iter++)
	{
		// ��� ä�� ����
		for (auto& channel : iter->second->m_pChannels) {
			if (channel) {
				channel->setLoopCount(0); // ���� ����
				channel->stop();
			}
		}
	}
}


bool CSoundMgr::IsPlaySound(wstring& _keyName)
{
	auto soundIter = m_mapSounds.find(_keyName);

	if (soundIter == m_mapSounds.end()) {
		assert(false);
	}

	bool isPlay = false;

	return isPlay;
}

bool CSoundMgr::IsPauseSound(wstring _keyName)
{
	return false;
}


