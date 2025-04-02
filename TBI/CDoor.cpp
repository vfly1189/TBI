#include "global.h"
#include "CDoor.h"

#include "CObject.h"

#include "CCollider.h"

#include "CCamera.h"

CDoor::CDoor()
{
	CreateCollider();
}

CDoor::~CDoor()
{
}

void CDoor::start()
{

}

void CDoor::update()
{

}

void CDoor::render(ID2D1HwndRenderTarget* _pRender)
{
	component_render(_pRender);
}

void CDoor::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::PLAYER)
	{
		Vec2 vPos = CCamera::GetInstance()->GetLookAt();
		if (m_iDoorDir == 0)
		{
			vPos -= Vec2(0.f, 540.f);
		}
		else if (m_iDoorDir == 1)
		{
			vPos += Vec2(960.f, 0.f);
		}
		else if (m_iDoorDir == 2)
		{
			vPos += Vec2(0.f, 540.f);
		}
		else if (m_iDoorDir == 3)
		{
			vPos -= Vec2(960.f, 0.f);
		}
		CCamera::GetInstance()->SetLookAt(vPos);
	}
}

void CDoor::OnCollision(CCollider* _pOther)
{

}

void CDoor::OnCollisionExit(CCollider* _pOther)
{

}
