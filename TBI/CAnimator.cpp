#include "global.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CObject.h"

CAnimator::CAnimator()
	: m_pOwner(nullptr)
	, m_pCurAnim(nullptr)
	, m_mapAnim{}
	, m_bRepeat(false)
{
}

CAnimator::~CAnimator()
{
	Safe_Delete_UnordedMap(m_mapAnim);
}

void CAnimator::CreateAnimation(const wstring& _animName, ID2D1Bitmap* _bitmap
	, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount, bool _bRepeat, Vec2 _offSet)
{
	CAnimation* pAnim = FindAnimation(_animName);
	assert(nullptr == pAnim);

	pAnim = new CAnimation;
	pAnim->SetName(_animName);
	pAnim->m_pAnimator = this;
	pAnim->Create(_bitmap, _vLT, _vSliceSize, _vStep, _fDuration, _iFrameCount, _offSet);
	pAnim->m_bRepeat = _bRepeat;

	m_mapAnim.insert(make_pair(_animName, pAnim));
}

void CAnimator::update()
{

}

void CAnimator::finalupdate()
{
	/*
	if (nullptr != m_pCurAnim) {
		m_pCurAnim->update();
		m_pCurAnim->finalupdate();
		if (m_pCurAnim->m_bRepeat && m_pCurAnim->IsFinish()) {
			m_pCurAnim->SetFrame(0);
		}
	}*/
	auto iter = m_vecActiveAnims.begin();
	while (iter != m_vecActiveAnims.end())
	{
		if ((*iter)->IsPaused())
		{
			delete* iter;
			iter = m_vecActiveAnims.erase(iter);
			continue;
		}


		(*iter)->update();
		(*iter)->finalupdate();

		if ((*iter)->IsFinish())
		{
			if ((*iter)->m_bRepeat) // 반복 설정 확인
			{
				(*iter)->SetFrame(0);
			}
			else
			{
				delete* iter;
				iter = m_vecActiveAnims.erase(iter);
				continue;
			}
		}
		++iter;
	}
}

void CAnimator::render(ID2D1HwndRenderTarget* _pRender)
{
	/*
	if (nullptr != m_pCurAnim) {
		m_pCurAnim->render(_pRender);
	}*/
	// 우선순위 기준 정렬 (낮은 값 먼저)
	std::sort(m_vecActiveAnims.begin(), m_vecActiveAnims.end(),
		[](CAnimation* a, CAnimation* b) {
			return a->GetRenderPriority() < b->GetRenderPriority();
		});


	
	for (auto& pAnim : m_vecActiveAnims)
	{
		pAnim->render(_pRender);
	}
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	auto animIter = m_mapAnim.find(_strName);

	if (animIter == m_mapAnim.end()) return nullptr;

	return animIter->second;
}

void CAnimator::Play(const wstring& _animName, bool _bRepeat, int _iPriority)
{
	m_pCurAnim = FindAnimation(_animName);

	//m_bRepeat = _bRepeat;

	CAnimation* pOrigin = FindAnimation(_animName);
	if (nullptr == pOrigin) return;

	CAnimation* pClone = pOrigin->Clone();
	pClone->m_bRepeat = _bRepeat; // 반복 설정
	pClone->SetRenderPriority(_iPriority);

	for (size_t i = 0; i < m_vecActiveAnims.size(); i++)
	{
		if (pClone->GetName().compare(m_vecActiveAnims[i]->GetName()) == 0)
			return;
	}
	m_vecActiveAnims.push_back(pClone);
}


void CAnimator::PauseAnimation(const wstring& _animName)
{
	for (auto& pAnim : m_vecActiveAnims)
	{
		if (pAnim->GetName().compare(_animName) == 0)
		{
		//	printf("PAUSE\n");
			pAnim->Pause();
			break;
		}
	}
}

void CAnimator::ResumeAnimation(const wstring& _animName)
{
	for (auto& pAnim : m_vecActiveAnims)
	{
		if (pAnim->GetName() == _animName)
		{
			pAnim->Resume();
			break;
		}
	}
}

void CAnimator::PauseAllAnimations()
{/*
	for (auto& pAnim : m_vecActiveAnims)
	{
		pAnim->Pause();
	}
	*/
	m_vecActiveAnims.clear();
}

void CAnimator::ResumeAllAnimations()
{
	for (auto& pAnim : m_vecActiveAnims)
	{
		pAnim->Resume();
	}
}