#include "global.h"
#include "CEventMgr.h"

#include "CObject.h"
#include "CSceneMgr.h"
#include "CUIMgr.h"
#include "AI.h"
#include "CScene.h"

CEventMgr::CEventMgr()
	: m_vecEvent{}
{

}

CEventMgr::~CEventMgr() {


}

void CEventMgr::update()
{
	//==================================================
	//���� �����ӿ��� ������� Dead Object���� �����Ѵ�.
	//==================================================

	for (auto deadObjectPtr : m_setDeadScheduled) {
		delete deadObjectPtr;
	}

	m_setDeadScheduled.clear();
	///////
	//  ===============
	//    Event ó��.
	//  ===============
	/*
	for (const auto& event_node : m_vecEvent) {
		Excute(event_node);
	}*/
	for (size_t i = 0; i < m_vecEvent.size(); i++)
	{
		Excute(m_vecEvent[i]);
	}

	m_vecEvent.clear();
}

void CEventMgr::Excute(const tEvent& _eve)
{
	switch (_eve.eEven) {
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// lParam : Object Address
		// wParam : Group Type
		CObject* pNewObj = (CObject*)_eve.lParam;
		GROUP_TYPE eType = (GROUP_TYPE)_eve.wParam;


		CSceneMgr::GetInstance()->GetCurScene()->AddObject(pNewObj, eType);
	}
	break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam : Object Address
		// wParam : NULL
		// Object�� Dead ���·� �����ϰ� -> ���� ���� ������Ʈ���� ��Ƶд�.
		CObject* pDeleteObj = (CObject*)_eve.lParam;
		pDeleteObj->SetDead();
		m_setDeadScheduled.insert(pDeleteObj);
	}
	break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam : Next Scene Type
		// wParam : NULL
		CSceneMgr::GetInstance()->ChangeScene((SCENE_TYPE)_eve.lParam);

		//���� Scene�� ���� UI�� ����Ű�� �־��⿡ �װ� ����. 
		CUIMgr::GetInstance()->SetFocusedUI(nullptr);
	}
	break;
	case EVENT_TYPE::CHANGE_AI_STATE:
	{
		// lParam : AI PTR
		// wParam : Next Type
		AI* pAI = (AI*)_eve.lParam;
		MON_STATE eNextState = (MON_STATE)_eve.wParam;
		pAI->ChangeState(eNextState);
	}
	break;
	}
}
