#pragma once

class CCollider;



union COOLIDER_ID {
	struct {
		UINT Left_id;
		UINT Right_id;
	};
	ULONGLONG ID;
};

class CCollisionMgr
{
	SINGLE(CCollisionMgr)
private:
	//������ ���� * ������ �����̰�, ���� ������ ã�ƿ� �� �־����. 
	// �ڷᱸ���� ���� HashTable�� �ؾ���. (Ž�� Ưȭ) ->
	// �˻� ������ �� �浹ü���� Key���� �־����. -> �浹ü ���� ������ ID�� �ʿ�. 

	unordered_map<ULONGLONG, bool>	m_mapColInfo;	//�浹ü ���� ���� ������ �浹 ����. 

	UINT m_arrCheck[(UINT)GROUP_TYPE::END];			//�׷찣�� �浹 üũ ��Ʈ����


public:
	void update();
	void CheckGroup(GROUP_TYPE eLeft, GROUP_TYPE e_Right);
	void Reset() {
		memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END);
		m_mapColInfo.clear();
	}


private:
	void CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);
};

