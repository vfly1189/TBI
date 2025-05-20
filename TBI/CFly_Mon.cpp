#include "global.h"
#include "CFly_Mon.h"
#include "CCollider.h"
#include "CObject.h"
#include "CBomb.h"

#include "CellMap.h"

#include "CPlayer.h"
#include "CPlayerMgr.h"
#include "CSoundMgr.h"
#include "CWall.h"

CFly_Mon::~CFly_Mon()
{
    m_pOwner->DecreaseFlyCount(1);
}

void CFly_Mon::update()
{
    CMonster::update();
}

void CFly_Mon::OnCollisionEnter(CCollider* _pOther)
{
    if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::TEAR && _pOther->GetOwner()->GetName().compare(L"Player_Attack_Tear") == 0)
    {
        m_tInfo.m_iCurHP -= (int)dynamic_cast<CPlayer*>(CPlayerMgr::GetInstance()->GetPlayer())->GetPlayerStat().m_fAttackDmg;
    }
    else if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::WALL)
    {
        printf("º® Ãæµ¹\n");
    }
    else if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::BOMB)
    {
        CBomb* bomb = (CBomb*)(_pOther->GetOwner());

        if (bomb->GetBombState() == BOMB_STATE::EXPLODE)
        {
            m_tInfo.m_iCurHP -= 100;
        }
    }
}

void CFly_Mon::OnCollision(CCollider* _pOther)
{
	
}

void CFly_Mon::OnCollisionExit(CCollider* _pOther)
{

}
