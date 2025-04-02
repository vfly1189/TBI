#include "global.h"
#include "CWall.h"
#include "CCollider.h"

CWall::CWall()
{
	CreateCollider();
}

CWall::~CWall()
{

}

void CWall::start()
{
	GetCollider()->SetScale(GetScale());
}

void CWall::update()
{

}

void CWall::render(ID2D1HwndRenderTarget* _pRender)
{
	component_render(_pRender);
}

void CWall::OnCollisionEnter(CCollider* _pOther)
{
}

void CWall::OnCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"PLAYER") {

		//파고 들었을 경우에 중심 위치에서. 
		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		if (((vPos.x - vScale.x / 2.f) < vObjPos.x )
			&& ((vPos.x + vScale.x / 2.f) > vObjPos.x )
			&& (vObjPos.y < vPos.y))
		{
			//Case 1 캐릭터가 위에 있을 때. 
			// X축 값으로 사이에 있고, 플레이어 Y값이 Ground보다 낮을 경우. 
			float fLen = abs(vObjPos.y - vPos.y);
			float fInterpolValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.y -= (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
		}
		else if (((vPos.y - vScale.y / 2.f) < vObjPos.y)
			&& ((vPos.y + vScale.y / 2.f) > vObjPos.y)
			&& (vPos.x < vObjPos.x)) {
			//Case 2 캐릭터가 오른쪽에 있을 때.
			//Y축 값으로 사이에 있고, 플레이어 X값이 Ground보다 높을 경우

			float fLen = abs(vObjPos.x - vPos.x);
			float fInterpolValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.x += (fInterpolValue);
			pOtherObj->SetPos(vObjPos);

		}
		else if (((vPos.x - vScale.x / 2.f) < vObjPos.x)
			&& ((vPos.x + vScale.x / 2.f) > vObjPos.x)
			&& (vObjPos.y > vPos.y)) {
			//Case 3 캐릭터가 아래에 있을 때.
			// X축 값으로 사이에 있고, 플레이어 Y값이 Ground보다 높을 경우. 
			float fLen = abs(vObjPos.y - vPos.y);
			float fInterpolValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.y += (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
		}
		else if (((vPos.y - vScale.y / 2.f) < vObjPos.y)
			&& ((vPos.y + vScale.y / 2.f) > vObjPos.y)
			&& (vPos.x > vObjPos.x)) {
			//Case 4 캐릭터가 왼쪽에 있을 때. 
			//Y축 값으로 사이에 있고, 플레이어 X값이 Ground보다 낮을 경우
			float fLen = abs(vObjPos.x - vPos.x);
			float fInterpolValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.x -= (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
		}
	}
}

void CWall::OnCollisionExit(CCollider* _pOther)
{
}
