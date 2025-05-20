#include "global.h"
#include "CRock.h"

#include "CBomb.h"
#include "CCollider.h"
#include "CSoundMgr.h"

CRock::CRock()
{
	CreateCollider();
}

CRock::~CRock()
{

}

void CRock::start()
{

}

void CRock::update()
{

}

void CRock::render(ID2D1HwndRenderTarget* _pRender)
{
	component_render(_pRender);
}

void CRock::OnCollisionEnter(CCollider* _pOther)
{

}

void CRock::OnCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"PLAYER") {

		//�İ� ����� ��쿡 �߽� ��ġ����. 
		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		if (((vPos.x - vScale.x / 2.f) < vObjPos.x)
			&& ((vPos.x + vScale.x / 2.f) > vObjPos.x)
			&& (vObjPos.y < vPos.y))
		{
			//Case 1 ĳ���Ͱ� ���� ���� ��. 
			// X�� ������ ���̿� �ְ�, �÷��̾� Y���� Ground���� ���� ���. 
			float fLen = abs(vObjPos.y - vPos.y);
			float fInterpolValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.y -= (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
		}
		else if (((vPos.y - vScale.y / 2.f) < vObjPos.y)
			&& ((vPos.y + vScale.y / 2.f) > vObjPos.y)
			&& (vPos.x < vObjPos.x)) {
			//Case 2 ĳ���Ͱ� �����ʿ� ���� ��.
			//Y�� ������ ���̿� �ְ�, �÷��̾� X���� Ground���� ���� ���

			float fLen = abs(vObjPos.x - vPos.x);
			float fInterpolValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.x += (fInterpolValue);
			pOtherObj->SetPos(vObjPos);

		}
		else if (((vPos.x - vScale.x / 2.f) < vObjPos.x)
			&& ((vPos.x + vScale.x / 2.f) > vObjPos.x)
			&& (vObjPos.y > vPos.y)) {
			//Case 3 ĳ���Ͱ� �Ʒ��� ���� ��.
			// X�� ������ ���̿� �ְ�, �÷��̾� Y���� Ground���� ���� ���. 
			float fLen = abs(vObjPos.y - vPos.y);
			float fInterpolValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.y += (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
		}
		else if (((vPos.y - vScale.y / 2.f) < vObjPos.y)
			&& ((vPos.y + vScale.y / 2.f) > vObjPos.y)
			&& (vPos.x > vObjPos.x)) {
			//Case 4 ĳ���Ͱ� ���ʿ� ���� ��. 
			//Y�� ������ ���̿� �ְ�, �÷��̾� X���� Ground���� ���� ���
			float fLen = abs(vObjPos.x - vPos.x);
			float fInterpolValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.x -= (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
		}
	}
	else if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::BOMB)
	{
		CBomb* bomb = (CBomb*)(_pOther->GetOwner());

		if (bomb->GetBombState() == BOMB_STATE::EXPLODE)
		{
			int rockCrumbleSound = rand() % 3;

			if (rockCrumbleSound == 0) CSoundMgr::GetInstance()->Play(L"rock crumble 0", 0.5f);
			else if (rockCrumbleSound == 1) CSoundMgr::GetInstance()->Play(L"rock crumble 2", 0.5f);
			else if (rockCrumbleSound == 2) CSoundMgr::GetInstance()->Play(L"rock crumble 3", 0.5f);




			DeleteObject(this);
		}
	}
}

void CRock::OnCollisionExit(CCollider* _pOther)
{

}
