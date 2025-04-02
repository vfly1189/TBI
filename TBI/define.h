#pragma once

//매크로는 코드를 치환하는 것. 
//따라서 함수를 호출한 적이 없어서 스위칭이 없어서 더 빠름. 
//주의점은 매크로는 함수가 아니라, 그대로 코드를 치환하는거라서

//완전히 다른 느낌이 될 수 있음...

//이렇게 하면 내가 해제를 신경쓰지 않아도 된다는 점
//반대로 이렇게 하면의 단점. 
//끝까지 끌고 나가야 하는 방식.

//SingleTon 매크로. 

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

#define KEY_CHECK(key, state) CKeyMgr::GetInstance()->GetKeyState(key) == state

#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define MOUSE_POS CKeyMgr::GetInstance()->GetMousePos()

#define PI 3.1415926535f

#define CHARACTER_CNT 51

#define TILE_SIZE 64.f

typedef pair<int, int> pii;
int dx4[4] = { 0, 1, 0 , -1 };
int dy4[4] = { -1,0,1,0 };


enum class GROUP_TYPE {
	DEFAULT,
	
	
	IMAGE,
	DOOR,
	UI,
	TEAR,
	EFFECT,
	ITEM,
	PLAYER,
	WALL,
	TEST,
	END = 32,
};

enum class SCENE_TYPE {
	MAIN,
	TOOL,
	START,
	FIGHT,
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


enum class BRUSH_TYPE {
	HOLLOW,
	BLACK,
	END,
};

enum class PEN_TYPE {
	RED,
	GREEN,
	BLUE,
	END,
};

// 페이드 상태 열거형
enum class FADE_STATE {
	NONE,
	FADE_IN,
	FADE_OUT
};

enum class ROOM_INFO
{
	EMPTY,
	START,
	NORMAL,
	SHOP,
	TREASURE,
	BOSS,
};

