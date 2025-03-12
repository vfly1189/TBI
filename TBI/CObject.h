#pragma once
#include "global.h"
#include "CCamera.h"

class CTextUI;

class CObject
{
private:
	GROUP_TYPE		m_ObjType;

	wstring			m_ObjName;
	Vec2			m_vPos;
	Vec2			m_vPrevPos;
	Vec2			m_vScale;
	Vec2			m_vRenderScale;

	//������Ʈ
	CTextUI*		m_pTextUI;

	bool			m_bAlive;			//�ڱ� �ڽ��� Ȱ��ȭ or ��Ȱ��ȭ. (���� ����)
	bool			m_bEnable;			//�Ͻ����� Ȱ��ȭ or ��Ȱ��ȭ. 

public:
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) {
		m_vScale = _vScale;
		m_vRenderScale = _vScale;
	}
	void SetObjType(GROUP_TYPE _eType) { m_ObjType = _eType; }
	void SetName(const wstring& _strName) { m_ObjName = _strName; }
	void SetRenderScale(Vec2 _vScale) { m_vRenderScale = _vScale; }
	void SetUIText(const std::wstring& text);

public:

	GROUP_TYPE GetObjType() { return m_ObjType; }
	Vec2 GetPos() { return m_vPos; }

	virtual Vec2 GetFinalPos() { return m_vPos; }

	Vec2 GetScale() { return m_vScale; }
	Vec2 GetRenderScale() { return m_vRenderScale; }

	const wstring& GetName() { return m_ObjName; }

public:
	bool IsDead() {
		return !m_bAlive;
	}

public:
	void CreateTextUI();

public:
	virtual void start() {};
	virtual void update();
	virtual void finalupdate();

	virtual void render(ID2D1HwndRenderTarget* _pRender);
	void component_render(ID2D1HwndRenderTarget* _pRender);

	virtual CObject* Clone() = 0;
public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

private:
	void SetDead() { m_bAlive = false; }

};