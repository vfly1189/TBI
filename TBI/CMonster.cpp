#include "global.h"
#include "CMonster.h"

#include "CObject.h"


#include "CellMap.h"

#include "CCollider.h"
#include "AI.h"

CMonster::CMonster()
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(45.f, 45.f));

	CreateAnimator();
}

CMonster::~CMonster()
{
	if (nullptr != m_pAI)
		delete m_pAI;
	m_pOwner->DecreaseMobCount(1);
}

void CMonster::update()
{
	CObject::update();

	if (nullptr != m_pAI) m_pAI->update();
}

void CMonster::SetAI(AI* _AI)
{
	m_pAI = _AI;
	m_pAI->m_pOwner = this;
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{

}

void CMonster::OnCollision(CCollider* _pOther)
{
}

void CMonster::OnCollisionExit(CCollider* _pOther)
{
}

