#pragma once

//��ũ�δ� �ڵ带 ġȯ�ϴ� ��. 
//���� �Լ��� ȣ���� ���� ��� ����Ī�� ��� �� ����. 
//�������� ��ũ�δ� �Լ��� �ƴ϶�, �״�� �ڵ带 ġȯ�ϴ°Ŷ�

//������ �ٸ� ������ �� �� ����...

//�̷��� �ϸ� ���� ������ �Ű澲�� �ʾƵ� �ȴٴ� ��
//�ݴ�� �̷��� �ϸ��� ����. 
//������ ���� ������ �ϴ� ���.

//SingleTon ��ũ��. 

#define SINGLE(type)public: \
					static type* GetInstance()\
					{\
						static type mgr;\
						return &mgr;\
					}\
					private:\
						type();\
						~type();

#define fDT CTimeMgr::GetInstance()->GetfDT()
#define DT	CTimeMgr::GetInstance()->GetDT()
#define fDTN CTimeMgr::GetInstance()->GetfDTN()
#define DTN CTimeMgr::GetInstance()->GetDTN()

#define CLONE(type) type* Clone() {return new type(*this);}

#define KEY_CHECK(key, state) CkeyMgr::GetInstance()->GetKeyState(key) == state

#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define MOUSE_POS CkeyMgr::GetInstance()->GetMousePos()

#define PI 3.1415926535f

#define CHARACTER_CNT 51


enum class GROUP_TYPE {
	DEFAULT,
	TILE,
	GROUND,
	MONSTER,
	PLAYER,
	WEAPON,
	DROP_ITEM,
	PROJ_PLAYER,
	PROJ_MONSTER,
	IMAGE,
	UI = 31,
	END = 32,
};

enum class SCENE_TYPE {
	MAIN,
	TOOL,
	START,
	SELECT_CHARACTER,
	SELECT_WEAPON,
	SHOP,
	TEST,

	STAGE_01,
	STAGE_02,



	RUN_END,
	END,
};


enum class EVENT_TYPE {
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	CHANGE_AI_STATE,




	END,
};



enum class TextUIMode
{
	TEXT,
	NUMBER,
	COUNT_DOWN,
};

enum class FONT_TYPE
{
	DEFALUT,
	KR,
	END,
};

enum class UI_TYPE
{
	DEFAULT,
	PANEL,
	SPRITE_UI,
	BTN,
	END,
};

enum class ITEM_TYPE
{
	WEAPON,
	PASSIVE,
	CHARACTER,
	END,
};
