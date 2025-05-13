#include "global.h"
#include "CTear.h"

#include "CObject.h"
#include "CCollider.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CRigidBody.h"

#include "CTimeMgr.h"
#include "CPlayerMgr.h"
#include "Direct2DMgr.h"
#include "CCamera.h"
#include "CSoundMgr.h"
#include "CCore.h"

CTear::CTear(Vec2 _vStartPos, float _fRange, bool _bPlayerCollision)
	: m_vDir(Vec2(1.f, 1.f))
	, m_fSpeed(300.f)
{
	m_vStartPos = _vStartPos;
	m_fRange = _fRange;
	m_bPlayerCollision = _bPlayerCollision;

	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	CreateCollider();
	GetCollider()->SetScale(Vec2(32.f, 32.f));

	CreateAnimator();
	GetAnimator()->CreateAnimation(L"tear_explode_anim", pD2DMgr->GetStoredBitmap(L"tear_explode")
		, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.01f, 10, true, Vec2(0.f, 0.f));

	AddImage(CPlayerMgr::GetInstance()->GetTearImage(5));

	CreateRigidBody();
}

CTear::~CTear()
{

}

void CTear::update()
{
	if (m_bDestroy)
	{
		m_fAccTime += fDT;
		if (m_fAccTime >= 0.16f)
		{
			DeleteObject(this);
			return;
		}
	}

	CObject::update();

	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Vec2 vCameraLookAt = CCamera::GetInstance()->GetLookAt();
	Vec2 vPos = GetPos();

	//printf("눈물시작위치 : %f %f\n 눈물현재위치 : %f %f\n\n", m_vStartPos.x, m_vStartPos.y, vPos.x, vPos.y);
	if ((m_vStartPos - vPos).Length() >= m_fRange
		&& !m_bDestroy)
	{
		//printf("눈물충돌\n");
		CSoundMgr::GetInstance()->Play(L"tear block", 0.2f);
		DeleteImages();
		GetAnimator()->Play(L"tear_explode_anim", false, 1);
		m_bStop = true;
		m_bDestroy = true;	
		return;
	}
	
	if (!((vPos.x > vCameraLookAt.x - vResolution.x / 2.f)
		&& (vPos.x < vCameraLookAt.x + vResolution.x / 2.f)

		&& (vPos.y > vCameraLookAt.y - vResolution.y / 2.f)
		&& (vPos.y < vCameraLookAt.y + vResolution.y / 2.f)))
	{
		DeleteObject(this);
		return;
	}


	
	if(!m_bStop)
	{
		/*
		vPos.x += 300.f * m_vDir.x * fDT;
		vPos.y += 300.f * m_vDir.y * fDT;
		*/

		Vec2 moveVec = m_vDir * m_fSpeed; // 최종 속도
		
		GetRigidBody()->SetVelocity(moveVec);
	
		SetPos(vPos);
	}
}

void CTear::finalupdate()
{
	CObject::finalupdate();
}

void CTear::render(ID2D1HwndRenderTarget* _pRender)
{
	component_render(_pRender);
}

void CTear::OnCollision(CCollider* _pOther)
{

}

void CTear::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::WALL ||
		_pOther->GetOwner()->GetObjType() == GROUP_TYPE::DOOR)
	{
		CSoundMgr::GetInstance()->Play(L"tear block", 0.2f);
		DeleteImages();
		GetAnimator()->Play(L"tear_explode_anim", false, 1);
		m_bStop = true;
		m_bDestroy = true;
		m_fAccTime = 0.f;
		m_fSpeed = 0.f;
	}
	else if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::ENTITY)
	{
		if (_pOther->GetOwner()->GetName().compare(L"Rock") == 0)
		{
			CSoundMgr::GetInstance()->Play(L"tear block", 0.2f);
			DeleteImages();
			GetAnimator()->Play(L"tear_explode_anim", false, 1);
			m_bStop = true;
			m_bDestroy = true;
			m_fAccTime = 0.f;
			m_fSpeed = 0.f;
		}
		else if (_pOther->GetOwner()->GetName().compare(L"Horf_Mon") == 0)
		{
			if (GetName().compare(L"Player_Attack_Tear") == 0)
			{
				CSoundMgr::GetInstance()->Play(L"tear block", 0.2f);
				DeleteImages();
				GetAnimator()->Play(L"tear_explode_anim", false, 1);
				m_bStop = true;
				m_bDestroy = true;
				m_fAccTime = 0.f;
				m_fSpeed = 0.f;
			}
		}
		else if (_pOther->GetOwner()->GetName().compare(L"Fly_Mon") == 0)
		{
			if (GetName().compare(L"Player_Attack_Tear") == 0)
			{
				CSoundMgr::GetInstance()->Play(L"tear block", 0.2f);
				DeleteImages();
				GetAnimator()->Play(L"tear_explode_anim", false, 1);
				m_bStop = true;
				m_bDestroy = true;
				m_fAccTime = 0.f;
				m_fSpeed = 0.f;
			}
		}
		else if (_pOther->GetOwner()->GetName().compare(L"Baby_Plum_Boss") == 0)
		{
			if (GetName().compare(L"Player_Attack_Tear") == 0)
			{
				CSoundMgr::GetInstance()->Play(L"tear block", 0.2f);
				DeleteImages();
				GetAnimator()->Play(L"tear_explode_anim", false, 1);
				m_bStop = true;
				m_bDestroy = true;
				m_fAccTime = 0.f;
				m_fSpeed = 0.f;
			}
		}
	}
	else if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::PLAYER)
	{
		if (m_bPlayerCollision)
		{
			CSoundMgr::GetInstance()->Play(L"tear block", 0.2f);
			DeleteImages();
			GetAnimator()->Play(L"tear_explode_anim", false, 1);
			m_bStop = true;
			m_bDestroy = true;
			m_fAccTime = 0.f;
			m_fSpeed = 0.f;
		}
	}
}

void CTear::OnCollisionExit(CCollider* _pOther)
{

}
