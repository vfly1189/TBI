#include "global.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"

//���Ͽ��� ���� ū ����� ��������, �޸𸮸� ������. 

CCollisionMgr::CCollisionMgr()
	:m_arrCheck{}
{


}

CCollisionMgr::~CCollisionMgr() {

}

void CCollisionMgr::update()
{
	if (CScene::GetPause()) return;
	//ǥ�� Ȯ���ϸ鼭, ��Ʈ���� ON�Ǿ� �ִٸ� �浹�ϰ�,
	//OFF��� �浹���� ����.

	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; iRow++) {
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; iCol++) {
			if (m_arrCheck[iRow] & (1 << iCol)) {
				//�ش� �׷찣�� �浹�� �����Ѵ�. 
				//���� ���� Scene���� �浹ü�� �˻��Ѵ�. 

				CollisionGroupUpdate((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}
}

void CCollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();


	//�츮�� �Ϸ��� �ǵ��� ���� ��߳��� �κ��� ����.
	//��ȯŸ���� �����̴�, �츮�� ���۷����� ������ ��. 
	//vecLeft�� �ּҰ��� ���纻(��������)��. ���� &�� �޾ƾ���. 
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);

	unordered_map<ULONGLONG, bool>::iterator colliderMapIter;


	for (auto leftIDX = 0; leftIDX < vecLeft.size(); leftIDX++) {

		//�浹ü�� �������� �ʴ� ��� 
		if (nullptr == vecLeft[leftIDX]->GetCollider()) continue;

		for (auto rightIDX = 0; rightIDX < vecRight.size(); rightIDX++) {

			//�浹ü�� �������� �ʴ� ���. or �ڱ� �ڽŰ��� �浹�� ���. 
			if (nullptr == vecRight[rightIDX]->GetCollider() || vecLeft[leftIDX] == vecRight[rightIDX]) continue;


			//�� �� �ϳ��� �浹 ������ false�� ��� continue
			if (false == vecRight[rightIDX]->GetCollider()->GetActive() || false == vecLeft[leftIDX]->GetCollider()->GetActive()) continue;


			CCollider* pLeftCollider = vecLeft[leftIDX]->GetCollider();
			CCollider* pRightCollider = vecRight[rightIDX]->GetCollider();


			//�浹ü�� ���� ���� ID
			COOLIDER_ID ID;

			ID.Left_id = pLeftCollider->GetID();
			ID.Right_id = pRightCollider->GetID();

			colliderMapIter = m_mapColInfo.find(ID.ID);

			//�浹 ������ �� ��� ������ ��� ���(�浹���� �ʾҴ�. ��)
			if (colliderMapIter == m_mapColInfo.end()) {
				//�� ������ �˻縦 ���ʷ� ���� ���. 
				m_mapColInfo.insert(make_pair(ID.ID, false));
				colliderMapIter = m_mapColInfo.find(ID.ID);
			}

			//���� �浹�� ���. 
			if (IsCollision(pLeftCollider, pRightCollider)) {
				if (colliderMapIter->second == false) {
					//�̹� �����ӿ� �� �浹�� ���.

					//���� ���� ������ ��, �浹�� ��. �� �浹�� ������ ���� ��.
					if (!vecLeft[leftIDX]->IsDead() && !vecRight[rightIDX]->IsDead()) {
						pLeftCollider->OnCollisionEnter(vecRight[rightIDX]->GetCollider());
						pRightCollider->OnCollisionEnter(vecLeft[leftIDX]->GetCollider());
						colliderMapIter->second = true;
					}
				}
				else if (colliderMapIter->second == true) {
					//���� �����ӿ��� �浹�ϰ� �־��� ���. 

					//���� �״� �ִ� �浹���� ����ٰ� ��������� ����. 
					if (vecLeft[leftIDX]->IsDead() || vecRight[rightIDX]->IsDead()) {
						pLeftCollider->OnCollisionExit(vecRight[rightIDX]->GetCollider());
						pRightCollider->OnCollisionExit(vecLeft[leftIDX]->GetCollider());
						colliderMapIter->second = false;
					}
					else {
						pLeftCollider->OnCollision(vecRight[rightIDX]->GetCollider());
						pRightCollider->OnCollision(vecLeft[leftIDX]->GetCollider());
					}

				}

			}
			else {
				//�浹���� ���� ���. 
				if (colliderMapIter->second == true) {
					//�� ���� �浹���� ��� ���.
					pLeftCollider->OnCollisionExit(vecRight[rightIDX]->GetCollider());
					pRightCollider->OnCollisionExit(vecLeft[leftIDX]->GetCollider());
					colliderMapIter->second = false;
				}

			}
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if ((abs(vRightPos.x - vLeftPos.x) <= (vLeftScale.x + vRightScale.x) / 2.f) &&
		(abs(vRightPos.y - vLeftPos.y) <= (vLeftScale.y + vRightScale.y) / 2.f)) {

		return true;
	}

	return false;
}

void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	//�׷찪 �߿��� �� ���� Ÿ���� ������, �� ū ���� ��(��Ʈ)�� ���. 
	//


	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iCol < iRow) {
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol)) {
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else {
		m_arrCheck[iRow] |= (1 << iCol);
	}
}


