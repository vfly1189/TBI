#include "global.h"
#include "CHorf_Mon.h"

#include "CObject.h"
#include "CMonster.h"
#include "CBomb.h"

#include "CCollider.h"

#include "CPlayer.h"
#include "CPlayerMgr.h"


void CHorf_Mon::update()
{
    CMonster::update();

}

void CHorf_Mon::OnCollisionEnter(CCollider* _pOther)
{
    if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::TEAR && _pOther->GetOwner()->GetName().compare(L"Player_Attack_Tear") == 0)
    {
        m_tInfo.m_iCurHP -= (int)dynamic_cast<CPlayer*>(CPlayerMgr::GetInstance()->GetPlayer())->GetPlayerStat().m_fAttackDmg;;
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

void CHorf_Mon::OnCollision(CCollider* _pOther)
{

}

void CHorf_Mon::OnCollisionExit(CCollider* _pOther)
{
}
