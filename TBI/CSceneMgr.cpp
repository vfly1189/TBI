#include "global.h"
#include "CSceneMgr.h"
#include "CScene.h"

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

	//Scene ����

	
	m_arrScene[(UINT)SCENE_TYPE::TEST] = new CScene_Test;
	m_arrScene[(UINT)SCENE_TYPE::TEST]->SetName(L"CScene_Test");
	m_arrScene[(UINT)SCENE_TYPE::TEST]->SetSceneType(SCENE_TYPE::TEST);
	

	//���� �� ����
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::TEST];

	m_pCurScene->Enter();

	//�θ� �����Ͷ� Start()�� ȣ���� ���� ����. 
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

//���� �ٲ� ��, ���� ���� �ִ� �����ͳ� �׷� ��. 
//�� �������� �۾��� �������ϰ�, Scene�� ������Ʈ �ؾ߸� ��. 
//���⼭ ���� �� �̺�Ʈ ���. ���� �����Ӻ��� ���� ������ ������ �̺�Ʈ�� �־��ش�. 
void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{
	m_pCurScene->Exit();
	m_pCurScene = m_arrScene[(UINT)_eNext];
	m_pCurScene->Enter();
}

//DontDestroy ������Ʈ�� �����غ���. 


