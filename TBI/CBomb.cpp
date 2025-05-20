#include "global.h"
#include "CBomb.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CCollider.h"

#include "Direct2DMgr.h"
#include "CSoundMgr.h"
#include "CTimeMgr.h"

CBomb::CBomb()
	: m_fAccTime(0.f)
{
	m_eState = BOMB_STATE::IGNITE;

	CreateAnimator();
	CreateCollider();
	GetCollider()->SetScale(Vec2(32.f, 32.f) * 2.f);
	
}

CBomb::~CBomb()
{

}

void CBomb::start()
{
	
}

void CBomb::update()
{
	CObject::update();

	UpdateRenderScale();

	m_fAccTime += fDT;

	if (m_fAccTime > m_fExplosionDuration)
	{
		if (!m_bAnimationStopFlag)
		{
			m_eState = BOMB_STATE::EXPLODE;
			m_bAnimationStopFlag = true;
			GetAnimator()->PauseAllAnimations();
			GetAnimator()->Play(L"explosion", false, 1);

			int bombSound = rand() % 2;
			if (bombSound == 0) CSoundMgr::GetInstance()->Play(L"boss explosions 0", 0.5f);
			else CSoundMgr::GetInstance()->Play(L"boss explosions 1", 0.5f);


			GetCollider()->SetScale(Vec2(96.f, 96.f) * 2.f);
		}
		
		if (m_fAccTime > m_fExplosionDuration + m_fAfterExplosionDuration)
		{
			GetAnimator()->PauseAllAnimations();

			DeleteObject(this);
			return;
		}
	}
}

void CBomb::finalupdate()
{
	CObject::finalupdate();
}

void CBomb::render(ID2D1HwndRenderTarget* _pRender)
{
	CObject::render(_pRender);
}

void CBomb::OnCollisionEnter(CCollider* _pOther)
{

}

void CBomb::OnCollision(CCollider* _pOther)
{

}

void CBomb::OnCollisionExit(CCollider* _pOther)
{

}

void CBomb::UpdateRenderScale()
{
	
}
