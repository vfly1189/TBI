#pragma once

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr)

private:
	

private:
	CScene* m_arrScene[(UINT)SCENE_TYPE::END];	//��� ���� ���� ������ �� ����
	CScene* m_pCurScene;						//���� �� 

public:
	

public:
	void init();

	void update();
	void render(ID2D1HwndRenderTarget* _pRender);

	bool IsWaveScene() {
		if (m_pCurScene == m_arrScene[(UINT)SCENE_TYPE::START]) return true;
		return false;
	}

public:
	CScene* GetCurScene() { return m_pCurScene; }
	CScene* GetScene(SCENE_TYPE _eType) { return m_arrScene[(UINT)_eType]; }

private:
	void ChangeScene(SCENE_TYPE _eNext);
};
