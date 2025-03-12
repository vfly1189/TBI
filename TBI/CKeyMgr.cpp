#include "global.h"
#include "CkeyMgr.h"
#include "CCore.h"


int g_arrVK[(int)KEY::LAST] = {
	VK_LEFT,	//LEFT,
	VK_RIGHT,	//RIGHT,
	VK_UP,		//UP,
	VK_DOWN,	//DOWN,

	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',

	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN,
	VK_ESCAPE,

	VK_LBUTTON,
	VK_RBUTTON,

	//LAST,
};

CKeyMgr::CKeyMgr() {

}

CKeyMgr::~CKeyMgr() {

}

void CKeyMgr::init()
{

	for (int keyIDX = 0; keyIDX < (int)KEY::LAST; keyIDX++) {
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}

	//�Ʒ�ó�� ����Ȯ��, ������ ���ȴ���.
	//m_vecKey[(int)KEY::LEFT].bPrev
	//m_vecKey[(int)KEY::LEFT].eState
}

//������ �ϰ� �ִٰ�, â�� �ٲٸ� Ű�� ������ �ͱ����� �����ؾ����� ������?
//TAB���¿��� �ִ� �ٷ� ���� AWAY�� ������ -> NONE
//HOLD���� �ִ� AWAY�� ���ٰ� -> NONE

void CKeyMgr::update() {

	//������ ��Ŀ�� �˾Ƴ��� 
	HWND hMainWnd = CCore::GetInstance()->GetMainHwnd();
	HWND hWnd = GetFocus();//���� ��Ŀ�� �Ǿ� �ִ� �������� �ڵ鰪�� �˷���. 


	//���� ��Ŀ�� ���ΰ�, �� ���α׷��� ���� �������� ������ �ƴ����� Ȯ�����ָ� ��.
	//������ ��Ŀ�� ���� ��, Ű �̺�Ʈ ����. 
	if (hMainWnd == hWnd) {

		for (int keyIDX = 0; keyIDX < (int)KEY::LAST; keyIDX++) {
			//Ű�� ���ȴٸ�. 
			if (GetAsyncKeyState(g_arrVK[keyIDX]) & 0x8000) {
				if (m_vecKey[keyIDX].bPrevPush) {
					//������ �����־���. 
					m_vecKey[keyIDX].eState = KEY_STATE::HOLD;
				}
				else {
					//������ �������� �ʾҴ�. 
					m_vecKey[keyIDX].eState = KEY_STATE::TAP;
				}
				m_vecKey[keyIDX].bPrevPush = true;
			}
			else {//���� Ű�� �������� �ʴٸ�. 
				if (m_vecKey[keyIDX].bPrevPush) {
					//������ Ű�� �����ִٸ�. 
					m_vecKey[keyIDX].eState = KEY_STATE::AWAY;
				}
				else {
					//������ Ű�� �������� �ʾҴٸ�. 
					m_vecKey[keyIDX].eState = KEY_STATE::NONE;
				}
				m_vecKey[keyIDX].bPrevPush = false;
			}
		}

		//Mouse ��ġ ���
		POINT ptPos = {};
		GetCursorPos(&ptPos);
		ScreenToClient(CCore::GetInstance()->GetMainHwnd(), &ptPos);

		m_vCurMousePos = Vec2((float)ptPos.x, (float)ptPos.y);
	}
	else {
		//������ ��Ŀ�� ���� ����
		for (int keyIDX = 0; keyIDX < (int)KEY::LAST; keyIDX++) {
			m_vecKey[keyIDX].bPrevPush = false;
			if (m_vecKey[keyIDX].eState == KEY_STATE::TAP || m_vecKey[keyIDX].eState == KEY_STATE::HOLD) {
				m_vecKey[keyIDX].eState = KEY_STATE::AWAY;
			}
			else if (m_vecKey[keyIDX].eState == KEY_STATE::AWAY) {
				m_vecKey[keyIDX].eState = KEY_STATE::NONE;
			}
		}
	}
}
