#include "global.h"
#include "CAnimator.h"
#include "CAnimation.h"

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
	, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	CAnimation* pAnim = FindAnimation(_animName);
	assert(nullptr == pAnim);

	pAnim = new CAnimation;
	pAnim->SetName(_animName);
	pAnim->m_pAnimator = this;
	pAnim->Create(_bitmap, _vLT, _vSliceSize, _vStep, _fDuration, _iFrameCount);

	m_mapAnim.insert(make_pair(_animName, pAnim));
}

void CAnimator::update()
{

}

void CAnimator::finalupdate()
{


	if (nullptr != m_pCurAnim) {
		m_pCurAnim->update();
		m_pCurAnim->finalupdate();
		if (m_bRepeat && m_pCurAnim->IsFinish()) {
			m_pCurAnim->SetFrame(0);
		}
	}
}

void CAnimator::render(ID2D1HwndRenderTarget* _pRender)
{
	if (nullptr != m_pCurAnim) {
		m_pCurAnim->render(_pRender);
	}
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	auto animIter = m_mapAnim.find(_strName);

	if (animIter == m_mapAnim.end()) return nullptr;

	return animIter->second;
}

void CAnimator::Play(const wstring& _animName, bool _bRepeat)
{
	printf("hi");

	m_pCurAnim = FindAnimation(_animName);

	m_bRepeat = _bRepeat;
}