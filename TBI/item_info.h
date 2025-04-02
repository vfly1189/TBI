#pragma once

enum class COLLECTIBLES_ITEM_TYPE
{
	ACTIVE,
	PASSIVE,
	FAMILIAR,
};

typedef struct _Item
{
	wstring						m_sItemTag;					//아이템 이름
	int							m_iNumber;					//아이템 번호
	COLLECTIBLES_ITEM_TYPE		m_eItemType;				//아이템 타입 ( 액티브 , 패시브 )

	float						m_fAddMaxHp;				//+최대 hp

	float						m_fAddAttackDmg;			//+공격력
	float						m_fAddAttackDmgCoef;		//+공격력 배수
	float						m_fAddAttackSpd;			//+공격속도

	float						m_fAddMoveSpd;				//+이동속도

	float						m_fAddAttackRange;			//+공격사거리
}Item;

vector<wstring> items_tags;

vector<Item> items = {
		{L"", 1, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.2f, 0.f, 0.f},
		{L"", 2, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 3.f, 0.f, 0.f, 0.f},
		{L"", 3, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 1.f, 3.f, 0.f, 0.f, 0.f},
		{L"", 4, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 1.f, 1.5f, 0.f, 0.f, 0.f},
		{L"", 5, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 3.f, 0.f, 0.f, 100.f},

		{L"", 6, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.5f, 0.f, -100.f},
		{L"", 7, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 3.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 8, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 9, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 10, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 0.f, 0.f},

		{L"", 11, COLLECTIBLES_ITEM_TYPE::PASSIVE, 3, 0.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 12, COLLECTIBLES_ITEM_TYPE::PASSIVE, 2, 1.f, 1.5f, 0.f, 0.f, 0.f},
		{L"", 13, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.2f, 100.f, 0.f},
		{L"", 14, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 200.f, 0.f},
		{L"", 15, COLLECTIBLES_ITEM_TYPE::PASSIVE, 2, 0.f, 0.f, 0.f, 0.f, 0.f},

		{L"", 16, COLLECTIBLES_ITEM_TYPE::PASSIVE, 3, 0.f, 0.f, 0.f, 50.f, 0.f},
		{L"", 17, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 18, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 19, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 20, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 100.f, 0.f},

		{L"", 21, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 22, COLLECTIBLES_ITEM_TYPE::PASSIVE, 1, 0.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 23, COLLECTIBLES_ITEM_TYPE::PASSIVE, 1, 0.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 24, COLLECTIBLES_ITEM_TYPE::PASSIVE, 1, 0.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 25, COLLECTIBLES_ITEM_TYPE::PASSIVE, 1, 0.f, 0.f, 0.f, 0.f, 0.f},

		{L"", 26, COLLECTIBLES_ITEM_TYPE::PASSIVE, 1, 0.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 27, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 100.f, 0.f},
		{L"", 28, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 100.f, 0.f},
		{L"", 29, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 0.f, 200.f},
		{L"", 30, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 0.f, 200.f},

		{L"", 31, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.f, 50.f, 50.f},
		{L"", 32, COLLECTIBLES_ITEM_TYPE::PASSIVE, 0, 0.f, 0.f, 0.5f, 0.f, 0.f},
		{L"", 33, COLLECTIBLES_ITEM_TYPE::ACTIVE, 1, 0.f, 0.f, 0.f, 0.f, 0.f},
		{L"", 34, COLLECTIBLES_ITEM_TYPE::ACTIVE, 0, 2.f, 1.f, 0.f, 0.f, 0.f},
		{L"", 35, COLLECTIBLES_ITEM_TYPE::ACTIVE, 0, 0.f, 0.f, 0.f, 0.f, 0.f},
};

