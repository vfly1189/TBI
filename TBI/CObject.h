#pragma once
#include "global.h"
#include "CCamera.h"

class CObject
{
private:
	GROUP_TYPE		m_ObjType;

	wstring			m_ObjName;
	Vec2			m_vPos;
	Vec2			m_vPrevPos;
	Vec2			m_vScale;
	Vec2			m_vRenderScale;

	bool			m_bAlive;			//자기 자신이 활성화 or 비활성화. (삭제 전용)
	bool			m_bEnable;			//일시적인 활성화 or 비활성화. 

public:
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) {
		m_vScale = _vScale;
		m_vRenderScale = _vScale;
	}

	GROUP_TYPE GetObjType() { return m_ObjType; }
	Vec2 GetPos() { return m_vPos; }

	virtual Vec2 GetFinalPos() { return m_vPos; }

	Vec2 GetScale() { return m_vScale; }
	Vec2 GetRenderScale() { return m_vRenderScale; }

	void SetObjType(GROUP_TYPE _eType) { m_ObjType = _eType; }
	void SetName(const wstring& _strName) { m_ObjName = _strName; }
	void SetRenderScale(Vec2 _vScale) { m_vRenderScale = _vScale; }

	const wstring& GetName() { return m_ObjName; }

	bool IsDead() {
		return !m_bAlive;
	}

private:
	void SetDead() { m_bAlive = false; }

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

};