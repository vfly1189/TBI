#pragma once

#include "define.h"

//키 상태.
enum class KEY_STATE {
	TAP,	//막 누른 시점
	HOLD,	//누르고 있는
	AWAY,	//막 뗀 시점
	NONE,	// 눌리지 않은 상태. 이전에도 눌리지 않은 상태. 
};

//지원해줄 수 있는 키 
enum class KEY {
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q,W,E,R,T,Y,U,I,O,P,A,S,D,F,G,H,J,K,L,Z,X,C,V,B,N,M,

	ALT,CTRL,LSHIFT,SPACE,ENTER,ESC,

	LBTN,RBTN,

	LAST,
};

struct tKeyInfo {
	KEY_STATE		eState;			//키의 상태값. 
	bool			bPrevPush;		//이전에 이 키가 눌렸는지 안 눌렸는지
};
class CKeyMgr
{
	SINGLE(CKeyMgr);

private:
	//벡터의 INDEX값이, 곧 KEY값. 
	vector<tKeyInfo>	m_vecKey;
	Vec2				m_vCurMousePos;

public:
	void init();
	void update();


public:
	KEY_STATE GetKeyState(KEY _eKey) {
		return m_vecKey[(int)_eKey].eState;
	};

	Vec2 GetMousePos() { return m_vCurMousePos; }
};

