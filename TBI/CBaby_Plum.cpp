#include "global.h"
#include "CBaby_Plum.h"

#include "CCollider.h"
#include "CObject.h"
#include "CWall.h"

#include "CPlayerMgr.h"
#include "CPlayer.h"

#include "CellMap.h"

CBaby_Plum::CBaby_Plum()
{
}

CBaby_Plum::~CBaby_Plum()
{
	m_pOwner->DecreaseFlyCount(1);
}


void CBaby_Plum::update()
{
	CMonster::update();
}

void CBaby_Plum::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::WALL)
	{
		int wallPos = dynamic_cast<CWall*>(_pOther->GetOwner())->GetWallPos();

		if (wallPos == 0)
		{
			if		(m_vDirBackBounce.x == 1 && m_vDirBackBounce.y == -1) m_vDirBackBounce = Vec2(1, 1);
			else if (m_vDirBackBounce.x == -1 && m_vDirBackBounce.y == -1) m_vDirBackBounce = Vec2(-1, 1);
		}
		else if (wallPos == 1)
		{
			if		(m_vDirBackBounce.x == 1 && m_vDirBackBounce.y == 1) m_vDirBackBounce = Vec2(-1, 1);
			else if (m_vDirBackBounce.x == 1 && m_vDirBackBounce.y == -1) m_vDirBackBounce = Vec2(-1, -1);
		}
		else if (wallPos == 2)
		{
			if		(m_vDirBackBounce.x == 1 && m_vDirBackBounce.y == 1) m_vDirBackBounce = Vec2(1, -1);
			else if (m_vDirBackBounce.x == -1 && m_vDirBackBounce.y == 1) m_vDirBackBounce = Vec2(-1, -1);
		}
		else if (wallPos == 3)
		{
			if		(m_vDirBackBounce.x == -1 && m_vDirBackBounce.y == 1) m_vDirBackBounce = Vec2(1, 1);
			else if (m_vDirBackBounce.x == -1 && m_vDirBackBounce.y == -1) m_vDirBackBounce = Vec2(1, -1);
		}
	}



	if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::TEAR && _pOther->GetOwner()->GetName().compare(L"Player_Attack_Tear") == 0)
	{
		m_tInfo.m_iCurHP -= dynamic_cast<CPlayer*>(CPlayerMgr::GetInstance()->GetPlayer())->GetPlayerStat().m_fAttackDmg;
	}
}

void CBaby_Plum::OnCollision(CCollider* _pOther)
{

}

void CBaby_Plum::OnCollisionExit(CCollider* _pOther)
{

}

