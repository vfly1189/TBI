#include "global.h"
#include "CUIMgr.h"

#include "CCore.h"

#include "CObject.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CUI.h"

#include "CkeyMgr.h"

CUIMgr::CUIMgr()
	: m_pFocusedUI(nullptr)
{

}

CUIMgr::~CUIMgr() {

}

void CUIMgr::update()
{
	bool bLbtnTap = KEY_TAP(KEY::LBTN);
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);
	// 1. FocusedUI Ȯ��
	m_pFocusedUI = GetFocusedUI();

	if (!m_pFocusedUI) return;

	// 2. FocusedUI ������ �θ� UI ����, �ڽ� UI�� �� ���� Ÿ���� �� UI�� �����´�. 
	CUI* pTargetUI = GetTargetedUI(m_pFocusedUI);

	if (nullptr != pTargetUI) {
		pTargetUI->MouseOn();

		if (bLbtnTap) {//��������� ��. 
			pTargetUI->MouseLbtnDown();
			pTargetUI->m_bLbtnDown = true;
		}
		else if (bLbtnAway)
		{
			//�̹��� �� �����µ�, �ش� ���� �ִ�.
			pTargetUI->MouseLbtnUp();
			if (pTargetUI->m_bLbtnDown) {
				pTargetUI->MouseLbtnClicked();
			}
			//���� ��ư ���� ���ȴ� üũ�� �ٽ� �����Ѵ�. 
			pTargetUI->m_bLbtnDown = false;
		}
	}
}


//Ư�� Ű ������ �� � UI�� ������� �Ѵ�. 
void CUIMgr::SetFocusedUI(CUI* _pUI)
{
	//�̹� ��Ŀ�� �� �ų�, nullptr���� ���� ��쿡. 
	if (m_pFocusedUI == _pUI || nullptr == _pUI) {
		m_pFocusedUI = _pUI;
		return;
	}

	m_pFocusedUI = _pUI;

	CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();

	vector<CObject*>::iterator CUIIter = vecUI.begin();
	//���� ��ư TAP�� �߻��ߴٴ� ������ �ִ� UI��. 
	for (; CUIIter != vecUI.end(); CUIIter++) {
		if (m_pFocusedUI == *CUIIter) {
			break;
		}
	}

	vecUI.erase(CUIIter);
	vecUI.push_back(m_pFocusedUI);
}

CUI* CUIMgr::GetFocusedUI()
{
	CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();

	bool bLbtnTap = KEY_TAP(KEY::LBTN);

	//���� ��Ŀ�� UI�� �޾Ƶΰ� ����Ǿ����� Ȯ���Ѵ�. 
	CUI* pFocusedUI = m_pFocusedUI;

	//���� Ŭ���� ���ٸ�, Focus�� ��ȯ�� ������ ���⿡ �׳� ����. 
	if (!bLbtnTap) return pFocusedUI;


	vector<CObject*>::iterator targetIter = vecUI.end();
	vector<CObject*>::iterator CUIIter = vecUI.begin();
	//���� ��ư TAP�� �߻��ߴٴ� ������ �ִ� UI��. 
	for (; CUIIter != vecUI.end(); CUIIter++) {
		if (((CUI*)*CUIIter)->IsMouseOn()) {
			targetIter = CUIIter;
		}
	}

	//���콺�� ������ �ߴµ� ����� ������ ���, �޾ư� ���� ����.
	if (vecUI.end() == targetIter) {
		return nullptr;
	}
	pFocusedUI = (CUI*)*targetIter;

	//��� ������ ��ü�Ǵٰ� ����, �������� Focus�� �������� UI�� �ȴ�. 
	//�׸��� ���� ���ο��� ������ �ٲ������. 

	vecUI.erase(targetIter);
	vecUI.push_back(pFocusedUI);

	return pFocusedUI;
}

CUI* CUIMgr::GetTargetedUI(CUI* _pParentUI)
{
	//1. �θ� UI����, ��� �ڽ� UI�� �˻�. �� �ȿ��� ���� �켱���� ���� �� ���� �Ʒ� �ڽ�.(�ڽ� �Ʒ����� ����������)

	//2. ���� UI�� Ÿ�����϶�, �켱 ������ ���� ���� �� �Ʒ� �ڽ�. 
	//3. UI�� �����ִٸ�? <- UI ��ġ���Ͱ� �߸��Ǿ���. 

	//�̷� Ʈ�� ���� ��ȸ�� Level ��ȸ��� �Ѵ�. -> ����Լ� ������� �ϸ� �θ� -> �ڽ� -> �� �ڽ� �������. 
	//�츮�� ���ϴ� ���� �� ��԰� �ƴ�, ���� ������ UI�� ���� �˻�. 
	//���� Queue�� Ȱ���Ѵ�. 
	//0(�θ�)���� 1,2(�ڽ�)�ְ�, 1���� 3,4,5.. �ְ�.. �̷��� �ȴ�. 
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	CUI* pTargetUI = nullptr;

	//��� �Լ� �� ������ �Ҵ��ϴ� �� ����.
	static list<CUI*> levelQueue;
	static vector<CUI*> vecNoneTarget;

	//�׳� ����ش�.
	levelQueue.clear();
	vecNoneTarget.clear();

	levelQueue.push_back(_pParentUI);

	while (!levelQueue.empty()) {
		CUI* pUI = levelQueue.front();

		levelQueue.pop_front();

		// ť���� ������ UI�� TargetUI���� Ȯ��.
		if (pUI->IsMouseOn()) {
			if (nullptr != pTargetUI) {
				vecNoneTarget.push_back(pTargetUI);
			}

			pTargetUI = pUI;
		}
		else {
			vecNoneTarget.push_back(pUI);
		}

		const vector<CUI*>& vecChild = pUI->GetChildsUI();
		for (auto& child : vecChild) {
			levelQueue.push_back(child);
		}
	}
	if (bLbtnAway) {
		for (auto& noneChild : vecNoneTarget) {
			noneChild->m_bLbtnDown = false;
		}
	}

	return pTargetUI;
}
