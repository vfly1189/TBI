#include "global.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CScene_Main.h"
#include "CScene_Fight.h"
#include "CScene_Test.h"

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
	, m_arrScene{}
{

}

CSceneMgr::~CSceneMgr() {
	for (UINT sceneIDX = 0; sceneIDX < (UINT)SCENE_TYPE::END; sceneIDX++) {

		if (m_arrScene[sceneIDX] != nullptr) {
			delete m_arrScene[sceneIDX];
		}
	}
}

void CSceneMgr::init()
{

	//Pause Init

	//Scene 생성

	
	m_arrScene[(UINT)SCENE_TYPE::TEST] = new CScene_Test;
	m_arrScene[(UINT)SCENE_TYPE::TEST]->SetName(L"CScene_Test");
	m_arrScene[(UINT)SCENE_TYPE::TEST]->SetSceneType(SCENE_TYPE::TEST);
	
	m_arrScene[(UINT)SCENE_TYPE::MAIN] = new CScene_Main;
	m_arrScene[(UINT)SCENE_TYPE::MAIN]->SetName(L"CScene_Main");
	m_arrScene[(UINT)SCENE_TYPE::MAIN]->SetSceneType(SCENE_TYPE::MAIN);

	
	m_arrScene[(UINT)SCENE_TYPE::FIGHT] = new CScene_Fight;
	m_arrScene[(UINT)SCENE_TYPE::FIGHT]->SetName(L"CScene_Fight");
	m_arrScene[(UINT)SCENE_TYPE::FIGHT]->SetSceneType(SCENE_TYPE::FIGHT);
	

	//현재 씬 설정
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::MAIN];

	m_pCurScene->Enter();

	//부모 포인터라 Start()를 호출할 수가 없음. 
}

void CSceneMgr::update()
{
	m_pCurScene->update();
	m_pCurScene->finalupdate();
}


void CSceneMgr::render(ID2D1HwndRenderTarget* _pRender)
{
	m_pCurScene->render(_pRender);
}

//씬을 바꿀 때, 기존 씬에 있던 포인터나 그런 거. 
//한 프레임은 작업을 마무리하고, Scene을 업데이트 해야만 함. 
//여기서 해줄 건 이벤트 등록. 다음 프레임부터 다음 씬으로 가도록 이벤트를 넣어준다. 
void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{
	m_pCurScene->Exit();
	m_pCurScene = m_arrScene[(UINT)_eNext];
	m_pCurScene->Enter();
}

//DontDestroy 오브젝트를 구현해보기. 


