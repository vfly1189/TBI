#pragma once

#include "global.h"

class CObject;

class CScene
{
	static bool isPause;

protected:
	vector<CObject*> m_arrObj[(UINT)GROUP_TYPE::END];		//���� �ȿ� ��� ������Ʈ ���� �ְڴ�. �̷� Ư��(���)�� ������ŭ �����ֱ�.
	//�޸����ϸ� �׷� ������ŭ �����ֱ�.

private:

	SCENE_TYPE		m_eType;
	wstring			m_strName; //Scene �̸�

	UINT			m_iTileX;		//Ÿ�� ���� ����
	UINT			m_iTileY;		//Ÿ�� ���� ����. 

	CObject* m_pPlayer;		//Player

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }

	SCENE_TYPE GetSceneType() { return m_eType; }
	void SetSceneType(SCENE_TYPE _eType) { m_eType = _eType; }
	CObject* GetPlayer() { return m_pPlayer; }

	virtual void start();
	virtual void update();
	virtual void finalupdate();

	virtual void render(ID2D1HwndRenderTarget* _pRender);

	virtual void Enter() = 0;		//�ش� Scene�� ���� �� ȣ��.
	virtual void Exit() = 0;		//�ش� Scene�� Ż�� �� ȣ��.

public:
	//Ŭ������ ����� �����ϸ� �ζ��� ó���� ��. 
	//���� �Լ� ȣ�� ����� �����. 
	void AddObject(CObject* _pObj, GROUP_TYPE _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}

	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }

	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType)
	{
		return m_arrObj[(UINT)_eType];
	}

	vector<CObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }

	void DeleteGroup(GROUP_TYPE _eGroup);
	void DeleteAll();

public:
	static bool GetPause() { return isPause; }

public:
	CScene();

	//�Ҹ����� �����Լ� �������. �� �Ŵ����� ��� Scene�� �θ� �����ͷ� ������.
	// CSceneMgr���� ���� �Ҹ��ų��, �Ҹ��ڴ� �θ��� CScene�� ȣ���.  
	virtual ~CScene();

};

